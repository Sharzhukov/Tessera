if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_executable(tessera main.cpp)
else()
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/_placeholder.cpp"
         "int main() { return 0; }\n")
    add_executable(tessera "${CMAKE_CURRENT_BINARY_DIR}/_placeholder.cpp")
endif()

target_link_libraries(tessera PRIVATE Tessera::Core)

tessera_apply_warnings(tessera)
tessera_apply_sanitizers(tessera)

set_target_properties(tessera PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)