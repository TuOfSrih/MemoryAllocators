#include "pch.h"
#include "StackAllocator.h"
#include <cassert>
#include <stdlib.h>

struct AllocationHeader {
	
#if _DEBUG
	void* previous_address;
#endif
	uint8_t adjustment;

};


StackAllocator::StackAllocator(const size_t blocksize, void* const memory_block): Allocator(blocksize, memory_block)
{
	assert(false);
}

StackAllocator::StackAllocator(const size_t blocksize): Allocator(blocksize), top(memory_block)
{
}

StackAllocator::~StackAllocator()
{
	top = nullptr;
	number_allocations = 0;
	used_memory = 0;
}

void* StackAllocator::allocate(const size_t size, const size_t alignment)
{
	const uint8_t adjust = Memory::alignForwardAdjustmentWithHeader(top, alignment, sizeof(AllocationHeader));

	if (adjust + size + used_memory > blocksize) return nullptr;

	void* const adjustedAdress = reinterpret_cast<void*>(reinterpret_cast<size_t>(top) + adjust);

	AllocationHeader* const header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<size_t>(adjustedAdress) - sizeof(AllocationHeader));
	header->adjustment = adjust;

#if _DEBUG
	header->previous_address = previous_allocation;
	previous_allocation = adjustedAdress;
#endif

	top = reinterpret_cast<void*>(reinterpret_cast<size_t>(adjustedAdress) + size);
	used_memory += size + adjust;
	number_allocations++;

	return adjustedAdress;
}

void StackAllocator::deallocate(void* p)
{
	assert(p && p == previous_allocation);

	const AllocationHeader* const header = reinterpret_cast<const AllocationHeader*>(reinterpret_cast<size_t>(p) - sizeof(AllocationHeader));
	used_memory -= reinterpret_cast<size_t>(top) - reinterpret_cast<size_t>(p) + header->adjustment;
	top = reinterpret_cast<void*>(reinterpret_cast<size_t>(p) - header->adjustment);

#if _DEBUG
	previous_allocation = header->previous_address;
#endif

	number_allocations--;
}

void StackAllocator::clear()
{
	top = memory_block;
	used_memory = 0;
	number_allocations = 0;
}
