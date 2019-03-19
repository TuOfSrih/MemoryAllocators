#include "pch.h"
#include "Allocator.h"
#include <iostream>

#include <cassert>


Allocator::Allocator(size_t blocksize, void* memory_block): blocksize(blocksize), memory_block(memory_block)
{

}

Allocator::Allocator(size_t blocksize) : blocksize(blocksize)
{
	memory_block = malloc(blocksize);
}


Allocator::~Allocator()
{
	free(memory_block);
	assert(number_allocations == 0 && used_memory == 0);
}

void* Allocator::get_memory_block()
{
	return memory_block;
}

size_t Allocator::get_size()
{
	return blocksize;
}

size_t Allocator::get_number_allocations()
{
	return number_allocations;
}

size_t Allocator::get_used_memory()
{
	return used_memory;
}


uint8_t Memory::alignForwardAdjustment(void* p, uint8_t alignment) {

	uint8_t offset = reinterpret_cast<uint8_t>(p) & (alignment - 1);
	uint8_t inverse_offset = alignment - offset;

	//Modulo alignment
	inverse_offset -= alignment * (inverse_offset == alignment);
	
	return inverse_offset;
}
