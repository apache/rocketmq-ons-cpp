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

#include"Logger.h"
#include <sys/stat.h>

namespace rocketmq {
    std::once_flag spd_log::once_log_flag;

    void spd_log::init_log0(std::string path, int level) {
        spdlog::level::level_enum sdplevel = (spdlog::level::level_enum) level;
        spdlog::set_level(sdplevel);
        spdlog::flush_on(spdlog::level::warn);
        spdlog::flush_every(std::chrono::seconds(60));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e %z] [%n] [%^---%L---%$] [thread %t] %v");
        ///auto file_logger = spdlog::basic_logger_mt("ONSClient4CPP", path);
        auto file_logger = spdlog::rotating_logger_mt("ONSClient4CPP", path, 1024 * 1024 * 10, 10);
        spdlog::set_default_logger(file_logger);
    }

    void spd_log::initLogger() {
        initLogger(getDefaultLoggerPath(), LOGLEVEL_DEBUG);
    }

    void spd_log::initLogger(std::string path, enum log_level level) {
        if (path.empty() || path.length() == 0) {
            std::call_once(once_log_flag, init_log0, getDefaultLoggerPath(), level);
        } else {
            int r = isDir(path);
            if (r == 0) {
                std::call_once(once_log_flag, init_log0, path + "/ONSClient4CPP.log", level);
            } else if (r == 1) {
                std::call_once(once_log_flag, init_log0, path, level);
            } else {
                std::call_once(once_log_flag, init_log0, getDefaultLoggerPath(), level);
            }
        }
    }

    void spd_log::destoryLogger() {
        //spdlog::shutdown();
    }

    void spd_log::flushLogger() {
    }

    std::string spd_log::getDefaultLoggerPath() {
        char *home = getenv("HOME");
        std::string log_home(home);
        log_home.append("/logs/ONSClient4CPP.log");
        return log_home;
    }

    int spd_log::isDir(std::string path) {
        if (path.find('/') == path.npos && path.find('\\') == path.npos) {
            std::string msgs;
            msgs.append("It is not a path:").append(path);
            perror(msgs.c_str());
            return -1;
        }
        if (path.rfind('/') == path.length() || path.rfind('\\') == path.length()) {
            return 0;
        }
        struct stat sb;
        if (stat(path.c_str(), &sb) != -1) {//exist
            switch (sb.st_mode & S_IFMT) {
                case S_IFDIR:
                    return 0;
                case S_IFREG:
                    return 1;
                default:
                    return 2;
            }
        }
        //default: it is a file.
        return 1;

    }

}