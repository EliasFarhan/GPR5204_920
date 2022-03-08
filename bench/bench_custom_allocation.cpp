#include <chrono>
#include <benchmark/benchmark.h>

#include "custom_allocator.h"

constexpr auto allocationSize = 8;
constexpr auto alignment = 8;

static void BM_Malloc(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        for (int i = 0; i < state.range(0); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto* ptr = std::malloc(allocationSize);
            benchmark::DoNotOptimize(ptr);
            auto end = std::chrono::high_resolution_clock::now();
            std::free(ptr);
            totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
        }
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_Malloc)->Range( 1, 512 )->UseManualTime();


static void BM_LinearAllocator(benchmark::State& state) {
    // Perform setup here
    const std::size_t totalSize = allocationSize * state.range(0);
    void* rootPtr = std::malloc(totalSize);

    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        LinearAllocator allocator(rootPtr, totalSize);
        for (int i = 0; i < state.range(0); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto* ptr = allocator.Allocate(allocationSize, alignment);
            benchmark::DoNotOptimize(ptr);
            auto end = std::chrono::high_resolution_clock::now();
            allocator.Deallocate(ptr);
            totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
        }
        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_LinearAllocator)->Range(1, 512)->UseManualTime();


static void BM_StackAllocator(benchmark::State& state) {
    // Perform setup here
    const std::size_t totalSize = allocationSize * state.range(0) * 2;
    void* rootPtr = std::malloc(totalSize);
    struct ReverseLinkedListNode
    {
        ReverseLinkedListNode* previousNode = nullptr;
    };
    ReverseLinkedListNode lastNode;
    for (auto _ : state) {
        std::chrono::duration<double> totalTime{};
        StackAllocator allocator(rootPtr, totalSize);
        for (int i = 0; i < state.range(0); i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto* ptr = allocator.Allocate(allocationSize, alignment);
            auto end = std::chrono::high_resolution_clock::now();
            auto* currentNode = static_cast<ReverseLinkedListNode*>(ptr);
            *currentNode = lastNode;
            lastNode.previousNode = static_cast<ReverseLinkedListNode*>(ptr);
            totalTime +=
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    end - start);
        }


        state.SetIterationTime(totalTime.count());
        benchmark::ClobberMemory();
        while (lastNode.previousNode != nullptr)
        {
            ReverseLinkedListNode* currentLastNode = lastNode.previousNode;
            ReverseLinkedListNode* preNode = currentLastNode->previousNode;
            allocator.Deallocate(currentLastNode);
            lastNode.previousNode = preNode;
        }
    }
}
// Register the function as a benchmark
BENCHMARK(BM_StackAllocator)->Range(1, 512)->UseManualTime();