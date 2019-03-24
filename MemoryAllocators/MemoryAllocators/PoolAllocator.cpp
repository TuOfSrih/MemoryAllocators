#include "pch.h"
#include "PoolAllocator.h"
#include <cassert>

void PoolAllocator::cleanSetup(const size_t object_size, const uint8_t object_alignment, const size_t blocksize) {
	
	uint8_t adjustment = Memory::alignForwardAdjustment(memory_block, object_alignment);
	free_list = reinterpret_cast<void**>(reinterpret_cast<size_t>(memory_block) + adjustment);

	void** current = free_list;
	size_t number_objects = (blocksize - adjustment) / object_size;

	for (int i = 0; i < number_objects - 1; i++) {

		*current = reinterpret_cast<void**>(reinterpret_cast<size_t>(current) + object_size);
		current = reinterpret_cast<void**>(*current);
	}

	*current = nullptr;
}

PoolAllocator::PoolAllocator(const size_t object_size, const uint8_t object_alignment, const size_t blocksize, void* memory_block) : Allocator(blocksize, memory_block)
#if _DEBUG
	, object_size(object_size), object_alignment(object_alignment)
#endif 

{
	cleanSetup(object_size, object_alignment, blocksize);
}

PoolAllocator::PoolAllocator(const size_t object_size, const uint8_t object_alignment, const size_t blocksize) : Allocator(blocksize)
#if _DEBUG
, object_size(object_size), object_alignment(object_alignment)
#endif 
{
	cleanSetup(object_size, object_alignment, blocksize);
	
}

void* PoolAllocator::allocate(const size_t size, const size_t alignment)
{
	assert(size == object_size && alignment == object_alignment);
	
	if (free_list == nullptr) return nullptr;
	void* p = free_list;
	free_list = reinterpret_cast<void**>(*free_list);

	used_memory += object_size;
	number_allocations++;

	return p;
}

void PoolAllocator::deallocate(void* p)
{
	*reinterpret_cast<void**>(p) = reinterpret_cast<void*>(free_list);

	free_list = reinterpret_cast<void**>(p);

	number_allocations--;
	used_memory -= object_size;
}

void PoolAllocator::clear()
{

	cleanSetup(object_size, object_alignment, blocksize);
}

PoolAllocator::~PoolAllocator()
{
}
