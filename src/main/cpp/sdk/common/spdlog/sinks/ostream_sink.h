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

#ifndef SPDLOG_H
#include "spdlog/spdlog.h"
#endif

#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/base_sink.h"

#include <mutex>
#include <ostream>

namespace spdlog {
namespace sinks {
template<typename Mutex>
class ostream_sink final : public base_sink<Mutex>
{
public:
    explicit ostream_sink(std::ostream &os, bool force_flush = false)
        : ostream_(os)
        , force_flush_(force_flush)
    {
    }
    ostream_sink(const ostream_sink &) = delete;
    ostream_sink &operator=(const ostream_sink &) = delete;

protected:
    void sink_it_(const details::log_msg &msg) override
    {
        fmt::memory_buffer formatted;
        sink::formatter_->format(msg, formatted);
        ostream_.write(formatted.data(), static_cast<std::streamsize>(formatted.size()));
        if (force_flush_)
        {
            ostream_.flush();
        }
    }

    void flush_() override
    {
        ostream_.flush();
    }

    std::ostream &ostream_;
    bool force_flush_;
};

using ostream_sink_mt = ostream_sink<std::mutex>;
using ostream_sink_st = ostream_sink<details::null_mutex>;

} // namespace sinks
} // namespace spdlog
