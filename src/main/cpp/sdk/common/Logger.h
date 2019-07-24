/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
