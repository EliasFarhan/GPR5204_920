#include "custom_allocator.h"
#include "custom_allocator.h"

LinearAllocator::LinearAllocator(void* rootPtr, std::size_t totalSize) :
    Allocator(rootPtr, totalSize),
    currentPtr_(rootPtr)
{
}

void* LinearAllocator::Allocate(std::size_t allocationSize, std::size_t alignment)
{
    const auto adjustment = alignForwardAdjustment(currentPtr_, alignment);
    if (usedMemory_ + adjustment + allocationSize > totalSize_) [[unlikely]]
        return nullptr;

    const auto alignedAddress = reinterpret_cast<std::uintptr_t>(currentPtr_) + adjustment;
    currentPtr_ = reinterpret_cast<void*>(alignedAddress + allocationSize);
    usedMemory_ += allocationSize + adjustment;
    numAllocations_++;
    return reinterpret_cast<void*>(alignedAddress);
}

void LinearAllocator::Deallocate(void* ptr)
{
}

void LinearAllocator::Clear()
{
    currentPtr_ = rootPtr_;
    numAllocations_ = 0;
    usedMemory_ = 0;
}

StackAllocator::StackAllocator(void* rootPtr, std::size_t totalSize) :
    Allocator(rootPtr, totalSize),
    currentPos_(rootPtr)
{
}

void* StackAllocator::Allocate(std::size_t allocationSize, std::size_t alignment)
{
    const auto adjustment = alignForwardAdjustmentWithHeader(currentPos_, alignment);
    if (usedMemory_ + adjustment + allocationSize > totalSize_) [[unlikely]]
        return nullptr;
    const std::uintptr_t alignedAddress = reinterpret_cast<std::uintptr_t>(currentPos_) + adjustment;
    auto* header = reinterpret_cast<AllocationHeader*>(alignedAddress - sizeof(AllocationHeader));
    header->adjustment = adjustment;

    currentPos_ = reinterpret_cast<void*>(alignedAddress + allocationSize);
    usedMemory_ += allocationSize + adjustment;
    numAllocations_++;
    return reinterpret_cast<void*>(alignedAddress);
}

void StackAllocator::Deallocate(void* ptr)
{
    const auto* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<std::uintptr_t>(ptr) - sizeof(AllocationHeader));
    usedMemory_ -= reinterpret_cast<std::uintptr_t>(currentPos_) - reinterpret_cast<std::uintptr_t>(ptr) + header->adjustment;
    currentPos_ = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(ptr)- header->adjustment);
}

std::uintptr_t StackAllocator::alignForwardAdjustmentWithHeader(const void* address, std::uintptr_t alignment)
{
    auto adjustment = alignForwardAdjustment(address, alignment);
    if(adjustment == 0)
    {
        adjustment += alignment;
    }
    return adjustment;
}
