add_executable(test main.cpp)

set_target_properties(test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(test PUBLIC alzartak)

if(MSVC)
    # target_compile_options(test PRIVATE /W4 /WX)
else()
    # target_compile_options(test PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

