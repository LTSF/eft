#pragma once

#include <Windows.h>
#include <thread>
#include <mutex>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <atomic>

#include <utilities/io.hpp>
#include <dependencies/udlib/ud.hpp>
#include <dependencies/MinHook/MinHook.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"