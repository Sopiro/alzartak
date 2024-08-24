#pragma once

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#else
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <array>
#include <format>
#include <functional>
#include <numeric>
#include <span>
#include <unordered_map>

#include "wak/bounding_box.h"
#include "wak/hash.h"
#include "wak/math.h"
#include "wak/random.h"

using namespace wak;
