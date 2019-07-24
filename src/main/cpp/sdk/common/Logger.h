#ifndef MQ_LOGGER_H
#define MQ_LOGGER_H

#include <string>
#include <mutex>

#include <cstdlib>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

#define CPP_SDK_LEVEL_TRACE 0
#define CPP_SDK_LEVEL_DEBUG 1
#define CPP_SDK_LEVEL_INFO 2
#define CPP_SDK_LEVEL_WARN 3
#define CPP_SDK_LEVEL_ERROR 4
#define CPP_SDK_LEVEL_CRITICAL 5
#define CPP_SDK_LEVEL_OFF 6

namespace rocketmq {
    enum log_level {
        LOGLEVEL_TRACE = CPP_SDK_LEVEL_TRACE,
        LOGLEVEL_DEBUG = CPP_SDK_LEVEL_DEBUG,
        LOGLEVEL_INFO = CPP_SDK_LEVEL_INFO,
        LOGLEVEL_WARN = CPP_SDK_LEVEL_WARN,
        LOGLEVEL_ERROR = CPP_SDK_LEVEL_ERROR,
        LOGLEVEL_FATAL = CPP_SDK_LEVEL_CRITICAL,
        LOGLEVEL_OFF = CPP_SDK_LEVEL_OFF
    };

    class spd_log {
    public:
        static std::once_flag once_log_flag;

        static void init_log0(std::string path, int level);

        static std::string getDefaultLoggerPath();

        static void initLogger();

        static void initLogger(std::string path, enum log_level level);

        static void destoryLogger();

        static void flushLogger();

    private:
        static int isDir(std::string path);

    };
}

#endif //MQ_LOGGER_H
