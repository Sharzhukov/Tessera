include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE TESSERA_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(NOT TESSERA_TEST_SOURCES)
    message(STATUS "tests: no test files yet — using placeholder")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/_placeholder.cpp"
         "int main() { return 0; }\n")
    set(TESSERA_TEST_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/_placeholder.cpp")
    add_executable(tessera_core_tests ${TESSERA_TEST_SOURCES})
else()
    add_executable(tessera_core_tests ${TESSERA_TEST_SOURCES})
    target_link_libraries(tessera_core_tests
        PRIVATE
            Tessera::Core
            Catch2::Catch2WithMain
    )
endif()

tessera_apply_warnings(tessera_core_tests)
tessera_apply_sanitizers(tessera_core_tests)

set_target_properties(tessera_core_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)

include(Catch)
catch_discover_tests(tessera_core_tests)