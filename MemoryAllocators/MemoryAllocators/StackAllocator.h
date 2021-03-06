#pragma once
#include "Allocator.h"



class StackAllocator : public Allocator
{
public:

	StackAllocator(const size_t blocksize, void* const memory_block);
	StackAllocator(const size_t blocksize);

	StackAllocator(const StackAllocator& alloc) = delete;
	StackAllocator(StackAllocator&& alloc) = delete;
	StackAllocator& operator= (const StackAllocator& alloc) = delete;
	StackAllocator& operator= (StackAllocator&& alloc) = delete;

	void* allocate(const size_t size, const size_t alignment) override;
	void  deallocate(void* const p) override;
	void  clear() override;

	~StackAllocator();

private: 

	void* top;
#if _DEBUG
	void* previous_allocation;
#endif
};

