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
#include <atomic>

int simpleLoopRelax(int n) {
    std::atomic_long s(0);
    for (int i = 0; i < n; ++i) {
        s.fetch_add(1, std::memory_order_relaxed);
    }
    return s.load(std::memory_order_relaxed);
}

int simpleLoopSeq(int n) {
    std::atomic_long s(0);
    for (int i = 0; i < n; ++i) {
        s.fetch_add(1, std::memory_order_seq_cst);
    }
    return s.load(std::memory_order_seq_cst);
}

static void BM_simpleLoopRelaxed(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(simpleLoopRelax(state.range(0)));
    }
}

static void BM_simpleLoopSeq(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(simpleLoopSeq(state.range(0)));
    }
}

BENCHMARK(BM_simpleLoopRelaxed)
->RangeMultiplier(2)->Range(1, 1<<10)->UseRealTime()->
Unit(benchmark::kNanosecond);
BENCHMARK(BM_simpleLoopSeq)
->RangeMultiplier(2)->Range(1, 1<<10)->UseRealTime()->
Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();