#pragma once
#include "Allocator.h"



class StackAllocator : public Allocator
{
public:

	StackAllocator(const size_t blocksize, void* memory_block);
	StackAllocator(const size_t blocksize);

	StackAllocator(const StackAllocator& alloc) = delete;
	StackAllocator(StackAllocator&& alloc) = delete;
	StackAllocator& operator= (const StackAllocator& alloc) = delete;
	StackAllocator& operator= (StackAllocator&& alloc) = delete;

	void* allocate(const size_t size, const size_t alignment) override;
	void  deallocate(const void* p) override;
	void  clear() override;

	~StackAllocator();

private: 

	void* top;
#if _DEBUG
	void* last_allocation;
#endif
};

