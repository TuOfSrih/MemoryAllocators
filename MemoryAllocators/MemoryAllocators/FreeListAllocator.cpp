#include "pch.h"
#include "FreeListAllocator.h"
#include <cassert>


FreeListAllocator::FreeListAllocator(const size_t blocksize, void* memory_block) : Allocator(blocksize, memory_block), free_blocks(reinterpret_cast<FreeBlock*>(memory_block))
{
	assert(blocksize > sizeof(FreeBlock));

	free_blocks->next = nullptr;
	free_blocks->size = blocksize;
}

FreeListAllocator::FreeListAllocator(const size_t blocksize) : Allocator(blocksize)
{
}

void * FreeListAllocator::allocate(const size_t size, const size_t alignment)
{
	return nullptr;
}

void FreeListAllocator::deallocate(const void * p)
{
}

void FreeListAllocator::clear()
{
}


FreeListAllocator::~FreeListAllocator()
{
}
