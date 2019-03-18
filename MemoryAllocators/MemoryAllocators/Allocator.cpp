#include "pch.h"
#include "Allocator.h"
#include <iostream>

#include <cassert>


Allocator::Allocator(size_t blocksize, void* memory_block): blocksize(blocksize), memory_block(memory_block)
{

}


Allocator::~Allocator()
{
	assert(memory_block == nullptr && blocksize == 0 && number_allocations == 0 && used_memory == 0);
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
	return size_t();
}

size_t Allocator::get_used_memory()
{
	return size_t();
}

//namespace Memory {

	uint8_t Memory::alignForwardAdjustment(void* p, uint8_t alignment) {

		uint8_t&& offset = reinterpret_cast<uint8_t>(p) & (alignment - 1);
		uint8_t&& inverse_offset = alignment - offset;

		//Modulo alignment
		inverse_offset -= alignment * (inverse_offset == alignment);
		
		return inverse_offset;
	}
//}
