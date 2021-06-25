#pragma once
#include <spdlog/spdlog.h>

/** @class Log
 * Singleton logger with static access
 * Wrapper for spdlog
 *
 * \see ![Official](https://github.com/gabime/spdlog)
 *
*/
class Log
{
public:
    Log(const Log &) = delete;
    void operator=(const Log &) = delete;

    static void Init();

    inline static spdlog::logger& GetLogger()
    {
        return *ms_Logger;
    }

private:
    inline static std::shared_ptr<spdlog::logger> ms_Logger = nullptr;
};

#define LOG_TRACE( ... ) Log::GetLogger().trace( __VA_ARGS__ )
#define LOG_INFO( ... )  Log::GetLogger().info( __VA_ARGS__ )
#define LOG_WARN( ... )  Log::GetLogger().warn( __VA_ARGS__ )
#define LOG_ERROR( ... ) Log::GetLogger().error( __VA_ARGS__ )
#define LOG_FATAL( ... ) Log::GetLogger().critical( __VA_ARGS__ )