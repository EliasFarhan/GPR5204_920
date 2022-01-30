//
// Created by efarhan on 30/01/2022.
//
#include <benchmark/benchmark.h>
#include <atomic>
#include <memory>

class MyIntAtomic
{
public:
    MyIntAtomic(int count): count(count){}
    void Increment()
    {
        count.fetch_add(1, std::memory_order_relaxed);
    }
    int GetNumber() const {return count;}
private:
    std::atomic<int> count;
};

class MyIntLock
{
public:
    MyIntLock(int count): count(count){}
    void Increment()
    {
        std::lock_guard<std::mutex> lock(m);
        count++;
    }
    int GetNumber() const {return count;}
private:
    int count;
    mutable std::mutex m;
};

static void BM_Atomic(benchmark::State& state) {
    static std::unique_ptr<MyIntAtomic> counter;
    if (state.thread_index() == 0) {
        // Setup code here.
        counter = std::make_unique<MyIntAtomic>(0);
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter->Increment();
    }
    if (state.thread_index() == 0) {
        // Teardown code here.

    }
}
BENCHMARK(BM_Atomic)->ThreadRange(1, 64);

static void BM_Lock(benchmark::State& state) {
    static std::unique_ptr<MyIntLock> counter;
    if (state.thread_index() == 0) {
        // Setup code here.
        counter = std::make_unique<MyIntLock>(0);
    }
    for (auto _ : state) {
        // Run the test as normal.
        counter->Increment();
    }
    if (state.thread_index() == 0) {
        // Teardown code here.

    }
}
BENCHMARK(BM_Lock)->ThreadRange(1, 64);