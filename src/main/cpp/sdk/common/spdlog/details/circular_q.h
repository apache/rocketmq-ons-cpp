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

// cirucal q view of std::vector.
#pragma once

#include <vector>

namespace spdlog {
namespace details {
template<typename T>
class circular_q
{
public:
    using item_type = T;

    explicit circular_q(size_t max_items)
        : max_items_(max_items + 1) // one item is reserved as marker for full q
        , v_(max_items_)
    {
    }

    // push back, overrun (oldest) item if no room left
    void push_back(T &&item)
    {
        v_[tail_] = std::move(item);
        tail_ = (tail_ + 1) % max_items_;

        if (tail_ == head_) // overrun last item if full
        {
            head_ = (head_ + 1) % max_items_;
            ++overrun_counter_;
        }
    }

    // Pop item from front.
    // If there are no elements in the container, the behavior is undefined.
    void pop_front(T &popped_item)
    {
        popped_item = std::move(v_[head_]);
        head_ = (head_ + 1) % max_items_;
    }

    bool empty()
    {
        return tail_ == head_;
    }

    bool full()
    {
        // head is ahead of the tail by 1
        return ((tail_ + 1) % max_items_) == head_;
    }

    size_t overrun_counter() const
    {
        return overrun_counter_;
    }

private:
    size_t max_items_;
    typename std::vector<T>::size_type head_ = 0;
    typename std::vector<T>::size_type tail_ = 0;

    std::vector<T> v_;

    size_t overrun_counter_ = 0;
};
} // namespace details
} // namespace spdlog
