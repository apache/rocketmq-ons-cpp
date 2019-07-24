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
// Copyright (c) 2015 David Schury, Gabi Melman
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#ifndef SPDLOG_H
#include "spdlog/spdlog.h"
#endif

#include "base_sink.h"
#include "spdlog/details/log_msg.h"
#include "spdlog/details/null_mutex.h"

#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>

// Distribution sink (mux). Stores a vector of sinks which get called when log
// is called

namespace spdlog {
namespace sinks {

template<typename Mutex>
class dist_sink : public base_sink<Mutex>
{
public:
    dist_sink() = default;
    dist_sink(const dist_sink &) = delete;
    dist_sink &operator=(const dist_sink &) = delete;

    void add_sink(std::shared_ptr<sink> sink)
    {
        std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
        sinks_.push_back(sink);
    }

    void remove_sink(std::shared_ptr<sink> sink)
    {
        std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
        sinks_.erase(std::remove(sinks_.begin(), sinks_.end(), sink), sinks_.end());
    }

    void set_sinks(std::vector<std::shared_ptr<sink>> sinks)
    {
        std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
        sinks_ = std::move(sinks);
    }

protected:
    void sink_it_(const details::log_msg &msg) override
    {

        for (auto &sink : sinks_)
        {
            if (sink->should_log(msg.level))
            {
                sink->log(msg);
            }
        }
    }

    void flush_() override
    {
        for (auto &sink : sinks_)
        {
            sink->flush();
        }
    }

    void set_pattern_(const std::string &pattern) override
    {
        set_formatter_(details::make_unique<spdlog::pattern_formatter>(pattern));
    }

    void set_formatter_(std::unique_ptr<spdlog::formatter> sink_formatter) override
    {
        base_sink<Mutex>::formatter_ = std::move(sink_formatter);
        for (auto &sink : sinks_)
        {
            sink->set_formatter(base_sink<Mutex>::formatter_->clone());
        }
    }
    std::vector<std::shared_ptr<sink>> sinks_;
};

using dist_sink_mt = dist_sink<std::mutex>;
using dist_sink_st = dist_sink<details::null_mutex>;

} // namespace sinks
} // namespace spdlog
