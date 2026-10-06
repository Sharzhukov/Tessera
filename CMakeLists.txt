cmake_minimum_required(VERSION 3.25)

project(Tessera
    VERSION      0.1.0
    DESCRIPTION  "Math expression parser, evaluator and visualizer"
    HOMEPAGE_URL "https://github.com/Sharzhukov/Tessera"
    LANGUAGES    CXX
)

# ============================================================
#  Common settings
# ============================================================
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands" FORCE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

if(APPLE AND NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)
endif()

if(UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

if(MSVC)
    add_compile_options(/utf-8)
endif()

# ============================================================
#  Options
# ============================================================
option(TESSERA_BUILD_LIBRARY      "Build Tessera library"                     ON)
option(TESSERA_BUILD_APP          "Build main executable"                     ON)
option(TESSERA_BUILD_SANDBOX      "Build sandbox executable"                  ON)
option(TESSERA_BUILD_TESTS        "Build tests"                               ON)
option(TESSERA_WARNINGS_AS_ERRORS "Treat warnings as errors"                  OFF)
option(TESSERA_ENABLE_SANITIZERS  "Enable ASan + UBSan"                       OFF)
option(BUILD_SHARED_LIBS          "Build shared library instead of static"    OFF)

# ============================================================
#  Helpers
# ============================================================
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

function(tessera_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /EHsc)
        if(TESSERA_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
        target_compile_options(${target} PRIVATE
            -Wall -Wextra -Wpedantic -Wshadow
            -Wnon-virtual-dtor -Wcast-align -Wunused -Woverloaded-virtual
        )
        if(TESSERA_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

function(tessera_apply_sanitizers target)
    if(NOT TESSERA_ENABLE_SANITIZERS)
        return()
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    elseif(MSVC)
        target_compile_options(${target} PRIVATE /fsanitize=address)
    endif()
endfunction()

# ============================================================
#  Core library — GLOB, no need to edit CMake when adding files
# ============================================================
if(TESSERA_BUILD_LIBRARY)

    file(GLOB_RECURSE TESSERA_CORE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    )

    if(NOT TESSERA_CORE_SOURCES)
        message(STATUS "tessera: no sources in src/ yet — using placeholder")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/_placeholder.cpp"
             "// placeholder — remove when first real .cpp is added\n")
        set(TESSERA_CORE_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/_placeholder.cpp")
    endif()

    add_library(TesseraCore ${TESSERA_CORE_SOURCES}
        include/tessera/common/common.hpp
        src/common/common.cpp
        include/tessera/core/command.hpp
        src/core/command.cpp
        include/tessera/core/commandRegister.hpp
        src/core/commandRegister.cpp
        include/tessera/core/operations/matrix.hpp
        src/core/operations/matrix.cpp
        include/tessera/core/context.hpp
        src/core/context.cpp
        include/tessera/core/token.hpp
        src/core/token.cpp)
    add_library(Tessera::Core ALIAS TesseraCore)

    # Windows: export all symbols for shared library builds
    if(BUILD_SHARED_LIBS AND WIN32)
        set_target_properties(TesseraCore PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
    endif()

    target_include_directories(TesseraCore
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_compile_features(TesseraCore PUBLIC cxx_std_17)

    if(UNIX AND NOT APPLE)
        target_link_libraries(TesseraCore PRIVATE Threads::Threads)
    endif()

    tessera_apply_warnings(TesseraCore)
    tessera_apply_sanitizers(TesseraCore)

    # ---- Install rules ----
    if(PROJECT_IS_TOP_LEVEL)
        install(TARGETS TesseraCore
            EXPORT  TesseraTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
            install(DIRECTORY include/
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
                FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
            )
        endif()

        install(EXPORT TesseraTargets
            FILE      TesseraTargets.cmake
            NAMESPACE Tessera::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Tessera
        )

        configure_package_config_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TesseraConfig.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/TesseraConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Tessera
        )

        write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/TesseraConfigVersion.cmake"
            VERSION       ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion
        )

        install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/TesseraConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/TesseraConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Tessera
        )
    endif()
endif()

# ============================================================
#  app / sandbox / tests
# ============================================================
if(TESSERA_BUILD_APP AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/app/CMakeLists.txt")
    add_subdirectory(app)
endif()

if(TESSERA_BUILD_SANDBOX AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sandbox/CMakeLists.txt")
    add_subdirectory(sandbox)
endif()

if(TESSERA_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    if(PROJECT_IS_TOP_LEVEL)
        enable_testing()
    endif()
    add_subdirectory(tests)
endif()

# ============================================================
#  Status
# ============================================================
if(PROJECT_IS_TOP_LEVEL)
    if(BUILD_SHARED_LIBS)
        set(_tessera_lib_type "shared")
    else()
        set(_tessera_lib_type "static")
    endif()

    message(STATUS "")
    message(STATUS "Tessera ${PROJECT_VERSION}")
    message(STATUS "  Build type : ${CMAKE_BUILD_TYPE}")
    message(STATUS "  Compiler   : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "  Library    : ${TESSERA_BUILD_LIBRARY} (${_tessera_lib_type})")
    message(STATUS "  App        : ${TESSERA_BUILD_APP}")
    message(STATUS "  Sandbox    : ${TESSERA_BUILD_SANDBOX}")
    message(STATUS "  Tests      : ${TESSERA_BUILD_TESTS}")
    message(STATUS "  Werror     : ${TESSERA_WARNINGS_AS_ERRORS}")
    message(STATUS "  Sanitizers : ${TESSERA_ENABLE_SANITIZERS}")
    message(STATUS "")
endif()