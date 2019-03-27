#include "pch.h"
#include "Allocator.h"
#include <iostream>

#include <cassert>


Allocator::Allocator(const size_t blocksize, void* const memory_block): blocksize(blocksize), memory_block(memory_block)
{
	assert(blocksize && memory_block);
}

Allocator::Allocator(const size_t blocksize) : blocksize(blocksize), memory_block(malloc(blocksize))
{
	assert(blocksize && memory_block);
}

Allocator::~Allocator()
{
	free(memory_block);
	assert(number_allocations == 0 && used_memory == 0);
}

void* Allocator::get_memory_block() const 
{
	return memory_block;
}

size_t Allocator::get_size() const
{
	return blocksize;
}

size_t Allocator::get_number_allocations() const
{
	return number_allocations;
}

size_t Allocator::get_used_memory() const
{
	return used_memory;
}


void* Memory::alignForward(void* const p, const uint8_t alignment)
{
	return reinterpret_cast<void*>((reinterpret_cast<size_t>(p) + alignment - 1) & (~(alignment - 1)));
}

uint8_t Memory::alignForwardAdjustment(const void* const p, const uint8_t alignment) {

	assert(p && alignment);

	const uint8_t offset = reinterpret_cast<uint8_t>(p) & (alignment - 1);
	uint8_t inverse_offset = alignment - offset;

	//Modulo alignment
	inverse_offset -= alignment * (inverse_offset == alignment);
	
	return inverse_offset;
}

uint8_t Memory::alignForwardAdjustmentWithHeader(const void* const p, const uint8_t alignment, const uint8_t headersize) {

	assert(p && alignment && headersize);

	uint8_t adjustment = alignForwardAdjustment(p, alignment);

	if (adjustment < headersize) {

		const uint8_t additional_memory = headersize - adjustment;
		adjustment += alignment * additional_memory / alignment;

		//if (additional_memory % alignment)==0 add alignment onto adjustment
		adjustment += (additional_memory % alignment) * alignment;
	}

	return adjustment;
}
