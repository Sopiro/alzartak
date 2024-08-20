set(TEST_PROJECT test)

add_executable(${TEST_PROJECT} main.cpp)

set_target_properties(${TEST_PROJECT} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(${TEST_PROJECT} PUBLIC alzartak)

if(MSVC)
    target_compile_options(${TEST_PROJECT} PRIVATE /W4 /WX /wd4819)
else()
    target_compile_options(${TEST_PROJECT} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    target_link_options(${TEST_PROJECT} PRIVATE
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sASSERTIONS=1
        -sSTACK_SIZE=50MB
        -sINITIAL_MEMORY=100MB
        -sMAXIMUM_MEMORY=1000MB
        -sUSE_GLFW=3
        -sUSE_WEBGL2=1
        -sMAX_WEBGL_VERSION=2
        -sMIN_WEBGL_VERSION=2
        -O3
        -std=c++20
    )

    # add_custom_command(
    #     TARGET ${TEST_PROJECT} POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_PROJECT}.html ${CMAKE_CURRENT_SOURCE_DIR}/../${TEST_PROJECT}.html
    #     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_PROJECT}.js ${CMAKE_CURRENT_SOURCE_DIR}/../${TEST_PROJECT}.js
    #     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_PROJECT}.wasm ${CMAKE_CURRENT_SOURCE_DIR}/../${TEST_PROJECT}.wasm
    # )
    
endif()
