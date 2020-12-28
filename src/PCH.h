#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <filesystem>

#include <spdlog/sinks/basic_file_sink.h>

#include "pugixml.hpp"
#include "nlohmann/json.hpp"

namespace WinAPI
{
	using namespace SKSE::WinAPI;

	//void(SHGetKnownFolderPath)();
}

namespace fs = std::filesystem;
using json = nlohmann::json;

#ifndef NDEBUG
#include <spdlog/sinks/base_sink.h>

namespace logger
{
	template <class Mutex>
	class msvc_sink :
		public spdlog::sinks::base_sink<Mutex>
	{
	private:
		using super = spdlog::sinks::base_sink<Mutex>;

	public:
		explicit msvc_sink() {}

	protected:
		void sink_it_(const spdlog::details::log_msg& a_msg) override
		{
			spdlog::memory_buf_t formatted;
			super::formatter_->format(a_msg, formatted);
			WinAPI::OutputDebugString(fmt::to_string(formatted).c_str());
		}

		void flush_() override {}
	};

	using msvc_sink_mt = msvc_sink<std::mutex>;
	using msvc_sink_st = msvc_sink<spdlog::details::null_mutex>;

	using windebug_sink_mt = msvc_sink_mt;
	using windebug_sink_st = msvc_sink_st;
}
#endif

using namespace std::literals;

namespace logger
{
	using namespace SKSE::log;
}

#define DLLEXPORT __declspec(dllexport)