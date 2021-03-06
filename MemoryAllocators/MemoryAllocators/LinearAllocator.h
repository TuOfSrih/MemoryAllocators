#pragma once
#include "Allocator.h"
class LinearAllocator :	public Allocator {

public:
	LinearAllocator(const size_t blocksize, void* const memory_block);
	LinearAllocator(const size_t blocksize);
	~LinearAllocator();

	void* allocate(const size_t size, const size_t alignment) override;
	void  deallocate(void* const p) override;
	void  clear() override;

private:

	void* current;
};

