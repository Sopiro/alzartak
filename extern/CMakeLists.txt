include(FetchContent)

# external dependencies

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# wak
FetchContent_Declare(
    wak
    GIT_REPOSITORY https://github.com/Sopiro/wak.git
    GIT_TAG master
)

FetchContent_MakeAvailable(wak)

# glad
add_subdirectory(glad)

# glfw
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)

if(NOT EMSCRIPTEN)
    FetchContent_MakeAvailable(glfw)
endif()

# add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
# message(${glfw_SOURCE_DIR})

# imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.0
)
FetchContent_Populate(imgui)

file(GLOB IMGUI_HEADER_FILES ${imgui_SOURCE_DIR}/*.h)
file(GLOB IMGUI_SOURCE_FILES ${imgui_SOURCE_DIR}/*.cpp)

source_group(TREE ${imgui_SOURCE_DIR} PREFIX "include" FILES ${IMGUI_HEADER_FILES})
source_group(TREE ${imgui_SOURCE_DIR} PREFIX "src" FILES ${IMGUI_SOURCE_FILES})

add_library(imgui ${IMGUI_HEADER_FILES} ${IMGUI_SOURCE_FILES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

set_target_properties(imgui PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(imgui glfw)
