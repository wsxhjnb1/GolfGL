#pragma once

/*
    Class Log generates two loggers,
    one for Core(Engine) logging
    and one for logging from Engine Client(Game)
*/

#include <spdlog/spdlog.h>

class Log
{
public:
    Log(const Log &) = delete;
    void operator=(const Log &) = delete;

    static void Init();

    inline static spdlog::logger &GetLogger()
    {
        return *ms_Logger;
    }

private:
    inline static std::shared_ptr<spdlog::logger> ms_Logger;
};

#define LOG_TRACE( ... ) Log::GetLogger().trace( __VA_ARGS__ )
#define LOG_INFO( ... ) Log::GetLogger().info( __VA_ARGS__ )
#define LOG_WARN( ... ) Log::GetLogger().warn( __VA_ARGS__ )
#define LOG_ERROR( ... ) Log::GetLogger().error( __VA_ARGS__ )
#define LOG_FATAL( ... ) Log::GetLogger().critical( __VA_ARGS__ )
