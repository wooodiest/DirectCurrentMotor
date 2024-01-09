#include "lhpch.h"

#include "Luha/Core/Log.h"
#include "Luha/Core/ApplicationSettings.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace Luha {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	bool Log::LogToFile    = true;
	bool Log::LogToConsole = true;

	spdlog::level::level_enum Log::s_CoreLevel   = spdlog::level::trace;
	spdlog::level::level_enum Log::s_ClientLevel = spdlog::level::trace;

	spdlog::sink_ptr Log::s_CoreFileSink;
	spdlog::sink_ptr Log::s_CoreConsoleSink;
	spdlog::sink_ptr Log::s_ClientFileSink;
	spdlog::sink_ptr Log::s_ClientConsoleSink;

	void Log::Init()
	{
		// Set up sinks
		s_CoreConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_ClientConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		s_CoreConsoleSink->set_pattern("%^[%T] %n: %v%$");
		s_ClientConsoleSink->set_pattern("%^[%T] %n: %v%$");

		s_CoreFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("data/Luha.log", true);
		s_ClientFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("data/Luha.log", true);
		s_CoreFileSink->set_pattern("[%T] [%l] %n: %v");
		s_ClientFileSink->set_pattern("[%T] [%l] %n: %v");

		// Set up loggers
		s_CoreLogger = std::make_shared<spdlog::logger>("Luha");
		s_ClientLogger = std::make_shared<spdlog::logger>("App");

		spdlog::register_logger(s_CoreLogger);
		spdlog::register_logger(s_ClientLogger);

		///
		UpdateLogger();
	}

	void Log::UpdateLogger()
	{
		s_CoreLogger->sinks().clear();
		s_ClientLogger->sinks().clear();

#if defined(LH_PLATFORM_WINDOWS) && defined(LH_DIST) && LH_CONSOLE_IN_DISTRIBUTION_BUILD == 0
		LogToConsole = false;
#endif
		if (LogToConsole)
		{
			s_CoreLogger->sinks().emplace_back(s_CoreConsoleSink);
			s_ClientLogger->sinks().emplace_back(s_ClientConsoleSink);

			s_CoreLogger->set_level(s_CoreLevel);
			s_ClientLogger->set_level(s_ClientLevel);
		}
		if (LogToFile)
		{
			s_CoreLogger->sinks().emplace_back(s_CoreFileSink);
			s_ClientLogger->sinks().emplace_back(s_ClientFileSink);

			s_CoreLogger->flush_on(s_CoreLevel);
			s_ClientLogger->flush_on(s_ClientLevel);
		}
	}

}