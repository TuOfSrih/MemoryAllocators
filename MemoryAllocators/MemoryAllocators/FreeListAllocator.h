#pragma once
#include "Allocator.h"

class FreeListAllocator : public Allocator
{
public:

	FreeListAllocator(const size_t blocksize, void* memory_block);
	FreeListAllocator(const size_t blocksize);

	FreeListAllocator(const FreeListAllocator& alloc) = delete;
	FreeListAllocator(FreeListAllocator&& alloc) = delete;
	FreeListAllocator& operator= (const FreeListAllocator& alloc) = delete;
	FreeListAllocator& operator= (FreeListAllocator&& alloc) = delete;

	void* allocate(const size_t size, const size_t alignment) override;
	void  deallocate(void* p) override;
	void  clear() override;

	~FreeListAllocator();

private:

	struct AllocationHeader {

		size_t size;
		uint8_t adjustment;
	};

	struct FreeBlock {

		size_t size;
		FreeBlock* next;
	};

	FreeBlock* free_blocks;

};

