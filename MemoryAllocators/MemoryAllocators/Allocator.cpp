#include "pch.h"
#include "Allocator.h"

#include <cassert>


Allocator::Allocator(size_t size, void* memory_block): size(size), memory_block(memory_block)
{
}


Allocator::~Allocator()
{
	assert(memory_block == nullptr && size == 0 && number_allocations == 0 && used_memory == 0);
}

void* Allocator::get_memory_block()
{
	return memory_block;
}

size_t Allocator::get_size()
{
	return size;
}

size_t Allocator::get_number_allocations()
{
	return size_t();
}

size_t Allocator::get_used_memory()
{
	return size_t();
}
