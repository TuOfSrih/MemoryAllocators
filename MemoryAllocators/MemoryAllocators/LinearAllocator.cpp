#include "pch.h"
#include "LinearAllocator.h"
#include <cassert>
#include <iostream>


LinearAllocator::LinearAllocator(size_t blocksize, void* memory_block) : Allocator(blocksize, memory_block), current(memory_block)
{
	assert(blocksize);
}

LinearAllocator::LinearAllocator(size_t blocksize) : Allocator(blocksize)
{
	assert(blocksize);

	current = memory_block = malloc(blocksize);
}


LinearAllocator::~LinearAllocator()
{
	current = nullptr;
	number_allocations = 0;
	used_memory = 0;
}

void* LinearAllocator::allocate(size_t size, size_t alignment)
{
	uint8_t adjust = Memory::alignForwardAdjustment(current, alignment);

	if (adjust + size + used_memory > blocksize) {

		return nullptr;
	}

	used_memory += size;
	number_allocations++;
	
	void* adjustedAdress = reinterpret_cast<void*>(reinterpret_cast<size_t>(current) + static_cast<unsigned int>(adjust));
	current				 = reinterpret_cast<void*>(reinterpret_cast<size_t>(adjustedAdress) + size);

	return adjustedAdress;
}

void LinearAllocator::deallocate(void* p)
{
	std::cerr << "Cannot deallocate single allocation with linear allocator" << std::endl;
	assert(false);
}

void LinearAllocator::clear()
{
	current = memory_block;
	number_allocations = 0;
	used_memory = 0;
}
