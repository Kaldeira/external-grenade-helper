#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_ENABLE_FREETYPE
#define STB_IMAGE_IMPLEMENTATION
#define JSON_NOEXCEPTION
#define ASMJIT_STATIC
#define CURL_STATICLIB

#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <TlHelp32.h>
#include <wrl.h>
#include <iostream>
#include <random>
#include <filesystem>
#include <deque>
#include <string>
#include <shared_mutex>
#include <omp.h>

#include <d3d11_2.h>
#include <d2d1_3helper.h>
#include <dwrite_3.h>
#include <dcomp.h>
#include <dwmapi.h>


//Libs
#pragma comment( lib, "dxgi" )
//#pragma comment( lib, "d2d1" )
#pragma comment( lib, "d3d11" )
#pragma comment( lib, "dcomp" )
#pragma comment( lib, "dwrite" )
#pragma comment(lib, "dwmapi.lib")

#include "Utils/nlohmann/json.hpp"

#include "Controller/Singleton.h"

#include "Utils/vectors.h"
#include "Controller/Globals.h"

using json = nlohmann::json;