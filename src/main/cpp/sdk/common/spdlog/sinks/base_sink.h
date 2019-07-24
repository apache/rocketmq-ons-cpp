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
//
// base sink templated over a mutex (either dummy or real)
// concrete implementation should override the sink_it_() and flush_()  methods.
// locking is taken care of in this class - no locking needed by the
// implementers..
//

#include "spdlog/common.h"
#include "spdlog/details/log_msg.h"
#include "spdlog/formatter.h"
#include "spdlog/sinks/sink.h"

namespace spdlog {
namespace sinks {
template<typename Mutex>
class base_sink : public sink
{
public:
    base_sink() = default;
    base_sink(const base_sink &) = delete;
    base_sink &operator=(const base_sink &) = delete;

    void log(const details::log_msg &msg) final
    {
        std::lock_guard<Mutex> lock(mutex_);
        sink_it_(msg);
    }

    void flush() final
    {
        std::lock_guard<Mutex> lock(mutex_);
        flush_();
    }

    void set_pattern(const std::string &pattern) final
    {
        std::lock_guard<Mutex> lock(mutex_);
        set_pattern_(pattern);
    }

    void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) final
    {
        std::lock_guard<Mutex> lock(mutex_);
        set_formatter_(std::move(sink_formatter));
    }

protected:
    virtual void sink_it_(const details::log_msg &msg) = 0;
    virtual void flush_() = 0;

    virtual void set_pattern_(const std::string &pattern)
    {
        set_formatter_(details::make_unique<spdlog::pattern_formatter>(pattern));
    }

    virtual void set_formatter_(std::unique_ptr<spdlog::formatter> sink_formatter)
    {
        formatter_ = std::move(sink_formatter);
    }
    Mutex mutex_;
};
} // namespace sinks
} // namespace spdlog
