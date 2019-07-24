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
// Copyright(c) 2018 spdlog
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#ifndef SPDLOG_H
#include "spdlog/spdlog.h"
#endif

#ifdef _WIN32
#include "spdlog/sinks/wincolor_sink.h"
#else
#include "spdlog/sinks/ansicolor_sink.h"
#endif

namespace spdlog {
namespace sinks {
#ifdef _WIN32
using stdout_color_sink_mt = wincolor_stdout_sink_mt;
using stdout_color_sink_st = wincolor_stdout_sink_st;
using stderr_color_sink_mt = wincolor_stderr_sink_mt;
using stderr_color_sink_st = wincolor_stderr_sink_st;
#else
using stdout_color_sink_mt = ansicolor_stdout_sink_mt;
using stdout_color_sink_st = ansicolor_stdout_sink_st;
using stderr_color_sink_mt = ansicolor_stderr_sink_mt;
using stderr_color_sink_st = ansicolor_stderr_sink_st;
#endif
} // namespace sinks

template<typename Factory = default_factory>
inline std::shared_ptr<logger> stdout_color_mt(const std::string &logger_name)
{
    return Factory::template create<sinks::stdout_color_sink_mt>(logger_name);
}

template<typename Factory = default_factory>
inline std::shared_ptr<logger> stdout_color_st(const std::string &logger_name)
{
    return Factory::template create<sinks::stdout_color_sink_st>(logger_name);
}

template<typename Factory = default_factory>
inline std::shared_ptr<logger> stderr_color_mt(const std::string &logger_name)
{
    return Factory::template create<sinks::stderr_color_sink_mt>(logger_name);
}

template<typename Factory = default_factory>
inline std::shared_ptr<logger> stderr_color_st(const std::string &logger_name)
{
    return Factory::template create<sinks::stderr_color_sink_st>(logger_name);
}
} // namespace spdlog
