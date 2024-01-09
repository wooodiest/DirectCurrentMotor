#pragma once

/* Client log macros

	LH_TRACE(...)   
	LH_INFO(...)    
	LH_WARN(...)    
	LH_ERROR(...)   
	LH_CRITICAL(...)

*/

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "Luha/Core/ApplicationSettings.h"

namespace Luha {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static void UpdateLogger();

		static bool LogToFile;
		static bool LogToConsole;

		static spdlog::level::level_enum s_CoreLevel;
		static spdlog::level::level_enum s_ClientLevel;

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		static spdlog::sink_ptr s_CoreFileSink;
		static spdlog::sink_ptr s_CoreConsoleSink;
		static spdlog::sink_ptr s_ClientFileSink;
		static spdlog::sink_ptr s_ClientConsoleSink;
	};
}


// Core log macros
#define LH_CORE_TRACE(...)    ::Luha::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LH_CORE_INFO(...)     ::Luha::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LH_CORE_WARN(...)     ::Luha::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LH_CORE_ERROR(...)    ::Luha::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LH_CORE_CRITICAL(...) ::Luha::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LH_TRACE(...)         ::Luha::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LH_INFO(...)          ::Luha::Log::GetClientLogger()->info(__VA_ARGS__)
#define LH_WARN(...)          ::Luha::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LH_ERROR(...)         ::Luha::Log::GetClientLogger()->error(__VA_ARGS__)
#define LH_CRITICAL(...)      ::Luha::Log::GetClientLogger()->critical(__VA_ARGS__)


