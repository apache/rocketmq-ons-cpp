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

#include <benchmark/benchmark.h>
#include "rocketmq.h"
#include "rocketmq-ons-cpp-full.h"
#include <cstring>

static void BM_ProducerSendMessage(benchmark::State &state) {
    state.PauseTiming();
    graal_isolatethread_t *thread_;
    graal_create_isolate(NULL, NULL, &thread_);

    factory_property property = {
            .group_id = "GID_opensource_unit_test",
            .access_key = "AK",
            .access_secret = "SK",
            .name_srv_addr =
            "47.100.33.127:9876"};

    int instance_id = create_producer(thread_, &property);
    char *data = new char[state.range(0)];
    message m;
    m.topic = "t_opensource_unit_test";
    m.tags = "tagA";
    m.body = data;
    m.body_size = state.range(0);
    m.key = "abc";

    send_result sr;
    state.ResumeTiming();
    for (auto _ : state) {
        send_message(thread_, instance_id, &m, &sr);
    }

    delete[](data);
    destroy_instance(thread_, instance_id);
    graal_detach_thread(thread_);
}

BENCHMARK(BM_ProducerSendMessage)
->RangeMultiplier(2)->Range(1<<6, 1<<10)->UseRealTime()->
Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
