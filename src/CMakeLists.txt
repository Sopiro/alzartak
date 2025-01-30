file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/${PROJECT_NAME}/*.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "src" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include" PREFIX "include" FILES ${HEADER_FILES})

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ../include/${PROJECT_NAME}/common.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${stb_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /wd4819 /wd4996 /wd4459)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-error -Wno-missing-field-initializers)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    imgui glad glfw wak
)
