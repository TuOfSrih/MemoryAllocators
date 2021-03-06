#pragma once
#include "Allocator.h"
class PoolAllocator : public Allocator
{
public:
	PoolAllocator(const size_t blocksize, void* const memory_block, const size_t object_size, const uint8_t object_alignment);
	PoolAllocator(const size_t blocksize, const size_t object_size, const uint8_t object_alignment);
	

	PoolAllocator(const PoolAllocator& alloc) = delete;
	PoolAllocator(PoolAllocator&& alloc) = delete;
	PoolAllocator& operator= (const PoolAllocator& alloc) = delete;
	PoolAllocator& operator= (PoolAllocator&& alloc) = delete;

	void* allocate(const size_t size, const size_t alignment) override;
	void  deallocate(void* const p) override;
	void  clear() override;

	~PoolAllocator();

private:

	void cleanSetup(const size_t object_size, const uint8_t object_alignment, const size_t blocksize);

	void** free_list;
	const size_t object_size;
	const uint8_t object_alignment;
	
};

