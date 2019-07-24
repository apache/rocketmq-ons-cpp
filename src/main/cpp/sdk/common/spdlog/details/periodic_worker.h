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
// Copyright(c) 2018 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

// periodic worker thread - periodically executes the given callback function.
//
// RAII over the owned thread:
//    creates the thread on construction.
//    stops and joins the thread on destruction (if the thread is executing a callback, wait for it to finish first).

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
namespace spdlog {
namespace details {

class periodic_worker
{
public:
    periodic_worker(const std::function<void()> &callback_fun, std::chrono::seconds interval)
    {
        active_ = (interval > std::chrono::seconds::zero());
        if (!active_)
        {
            return;
        }

        worker_thread_ = std::thread([this, callback_fun, interval]() {
            for (;;)
            {
                std::unique_lock<std::mutex> lock(this->mutex_);
                if (this->cv_.wait_for(lock, interval, [this] { return !this->active_; }))
                {
                    return; // active_ == false, so exit this thread
                }
                callback_fun();
            }
        });
    }

    periodic_worker(const periodic_worker &) = delete;
    periodic_worker &operator=(const periodic_worker &) = delete;

    // stop the worker thread and join it
    ~periodic_worker()
    {
        if (worker_thread_.joinable())
        {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                active_ = false;
            }
            cv_.notify_one();
            worker_thread_.join();
        }
    }

private:
    bool active_;
    std::thread worker_thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
};
} // namespace details
} // namespace spdlog
