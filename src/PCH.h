#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <random>
#include <queue>

#include <spdlog/sinks/basic_file_sink.h>

#include "pugixml.hpp"
#include "nlohmann/json.hpp"

namespace WinAPI = SKSE::WinAPI;
namespace logger = SKSE::log;
namespace fs = std::filesystem;

using namespace std::literals;
using json = nlohmann::json;

#define DLLEXPORT __declspec(dllexport)
