#include "pch.h"
#include "StackAllocator.h"
#include <cassert>
#include <stdlib.h>

struct AllocationHeader {
	
	uint8_t adjustment;
#if _DEBUG
	void* previous_address;
#endif
};


StackAllocator::StackAllocator(const size_t blocksize, void* memory_block): Allocator(blocksize, memory_block)
{
}

StackAllocator::StackAllocator(const size_t blocksize): Allocator(blocksize)
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
	uint8_t adjust = Memory::alignForwardAdjustmentWithHeader(top, alignment, sizeof(AllocationHeader));

	if (adjust + size + used_memory > blocksize) return nullptr;

	void* adjustedAdress = reinterpret_cast<void*>(reinterpret_cast<size_t>(top) + adjust);

	AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<size_t>(adjustedAdress) - sizeof(AllocationHeader));
	header->adjustment = adjust;

#if _DEBUG
	header->previous_address = top;
	last_allocation = adjustedAdress;
#endif

	top = reinterpret_cast<void*>(reinterpret_cast<size_t>(adjustedAdress) + size);
	used_memory += size + adjust;
	number_allocations++;

	return adjustedAdress;
}

void StackAllocator::deallocate(void* p)
{
	assert(p && p == last_allocation);

	const AllocationHeader* header = reinterpret_cast<const AllocationHeader*>(p) - sizeof(AllocationHeader);
	used_memory -= reinterpret_cast<size_t>(top) - reinterpret_cast<size_t>(p) + header->adjustment;
	top = reinterpret_cast<void*>(reinterpret_cast<size_t>(p) - header->adjustment);

#if _DEBUG
	last_allocation = header->previous_address;
#endif

	number_allocations--;
}

void StackAllocator::clear()
{
	top = memory_block;
	used_memory = 0;
	number_allocations = 0;
}
