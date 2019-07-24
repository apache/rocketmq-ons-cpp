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

#include "spdlog/details/log_msg.h"
#include "spdlog/details/pattern_formatter.h"
#include "spdlog/formatter.h"

namespace spdlog {
namespace sinks {
class sink
{
public:
    sink() = default;

    explicit sink(std::unique_ptr<spdlog::formatter> formatter)
        : formatter_{std::move(formatter)}
    {
    }

    virtual ~sink() = default;
    virtual void log(const details::log_msg &msg) = 0;
    virtual void flush() = 0;
    virtual void set_pattern(const std::string &pattern) = 0;
    virtual void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) = 0;

    bool should_log(level::level_enum msg_level) const
    {
        return msg_level >= level_.load(std::memory_order_relaxed);
    }

    void set_level(level::level_enum log_level)
    {
        level_.store(log_level);
    }

    level::level_enum level() const
    {
        return static_cast<spdlog::level::level_enum>(level_.load(std::memory_order_relaxed));
    }

protected:
    // sink log level - default is all
    level_t level_{level::trace};

    // sink formatter - default is full format
    std::unique_ptr<spdlog::formatter> formatter_{details::make_unique<spdlog::pattern_formatter>()};
};

} // namespace sinks
} // namespace spdlog
