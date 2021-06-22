#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <spdlog/sinks/basic_file_sink.h>

#include "pugixml.hpp"
#include "nlohmann/json.hpp"

namespace WinAPI
{
	using namespace SKSE::WinAPI;
}

namespace fs = std::filesystem;
using json = nlohmann::json;

using namespace std::literals;

namespace logger
{
	using namespace SKSE::log;
}

#define DLLEXPORT __declspec(dllexport)
