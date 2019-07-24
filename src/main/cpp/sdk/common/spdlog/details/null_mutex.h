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

#include <atomic>
// null, no cost dummy "mutex" and dummy "atomic" int

namespace spdlog {
namespace details {
struct null_mutex
{
    void lock() {}
    void unlock() {}
    bool try_lock()
    {
        return true;
    }
};

struct null_atomic_int
{
    int value;
    null_atomic_int() = default;

    explicit null_atomic_int(int val)
        : value(val)
    {
    }

    int load(std::memory_order) const
    {
        return value;
    }

    void store(int val)
    {
        value = val;
    }
};

} // namespace details
} // namespace spdlog
