#include <precomp.h>
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


void Log::Init()
{	
	spdlog::set_pattern("%^[%T] %n: %v%$");

	ms_Logger = std::move(spdlog::stdout_color_mt("LOG"));
	ms_Logger->set_level(spdlog::level::trace);		
}
