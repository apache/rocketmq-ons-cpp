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

//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#include "spdlog/common.h"
#include "spdlog/details/os.h"

#include <string>
#include <utility>

namespace spdlog {
namespace details {
struct log_msg
{

    log_msg(source_loc loc, const std::string *loggers_name, level::level_enum lvl, string_view_t view)
        : logger_name(loggers_name)
        , level(lvl)
#ifndef SPDLOG_NO_DATETIME
        , time(os::now())
#endif

#ifndef SPDLOG_NO_THREAD_ID
        , thread_id(os::thread_id())
#endif
        , source(loc)
        , payload(view)
    {
    }

    log_msg(const std::string *loggers_name, level::level_enum lvl, string_view_t view)
        : log_msg(source_loc{}, loggers_name, lvl, view)
    {
    }

    log_msg(const log_msg &other) = default;

    const std::string *logger_name{nullptr};
    level::level_enum level{level::off};
    log_clock::time_point time;
    size_t thread_id{0};
    size_t msg_id{0};

    // wrapping the formatted text with color (updated by pattern_formatter).
    mutable size_t color_range_start{0};
    mutable size_t color_range_end{0};

    source_loc source;
    const string_view_t payload;
};
} // namespace details
} // namespace spdlog
