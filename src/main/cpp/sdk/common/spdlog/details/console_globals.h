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

#pragma once
//
// Copyright(c) 2018 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#include "spdlog/details/null_mutex.h"
#include <cstdio>
#include <mutex>

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX // prevent windows redefining min/max
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#endif

namespace spdlog {
namespace details {
struct console_stdout
{
    static std::FILE *stream()
    {
        return stdout;
    }
#ifdef _WIN32
    static HANDLE handle()
    {
        return ::GetStdHandle(STD_OUTPUT_HANDLE);
    }
#endif
};

struct console_stderr
{
    static std::FILE *stream()
    {
        return stderr;
    }
#ifdef _WIN32
    static HANDLE handle()
    {
        return ::GetStdHandle(STD_ERROR_HANDLE);
    }
#endif
};

struct console_mutex
{
    using mutex_t = std::mutex;
    static mutex_t &mutex()
    {
        static mutex_t s_mutex;
        return s_mutex;
    }
};

struct console_nullmutex
{
    using mutex_t = null_mutex;
    static mutex_t &mutex()
    {
        static mutex_t s_mutex;
        return s_mutex;
    }
};
} // namespace details
} // namespace spdlog
