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
	assert(blocksize);


}

FreeListAllocator::~FreeListAllocator()
{
}

void* FreeListAllocator::allocate(const size_t size, const size_t alignment)
{
	FreeBlock* suitable_block = free_blocks;
	FreeBlock* previous_block = nullptr;

	while (suitable_block != nullptr) {
		
		uint8_t adjustment = Memory::alignForwardAdjustmentWithHeader(suitable_block, alignment, sizeof(AllocationHeader));
		

		//Iterate over free blocks until first suitable is found
		if (suitable_block->size < size + adjustment) {

			previous_block = suitable_block;
			suitable_block = suitable_block->next;
			continue;
		}

		void* allocation_address = reinterpret_cast<void*>(reinterpret_cast<size_t>(suitable_block) + adjustment);
		void* header_address = reinterpret_cast<void*>(reinterpret_cast<size_t>(allocation_address) - sizeof(AllocationHeader));
		size_t total_size = size + adjustment;

		//Entire free block is used or just a part
		if (suitable_block->size - total_size <= sizeof(AllocationHeader) + 1) {

			//Allocate the entire block
			total_size = suitable_block->size;

			//Update start of blocklist or previous block
			if (previous_block == nullptr) {

				free_blocks = suitable_block;
			}
			else {

				previous_block->next = suitable_block->next;
			}
		}
		else {

			//Write a new free block
			FreeBlock* new_free_block = reinterpret_cast<FreeBlock*>(reinterpret_cast<size_t>(allocation_address) + size);
			new_free_block->size = suitable_block->size - total_size;
			new_free_block->next = suitable_block->next;

			//Update start of blocklist or previous block
			if (previous_block == nullptr) {

				free_blocks = new_free_block;
			}
			else {

				previous_block->next = new_free_block;
			}
		}

		//Write allocation header
		AllocationHeader* header = reinterpret_cast<AllocationHeader*>(header_address);
		header->adjustment = adjustment;
		header->size = total_size;

		number_allocations++;
		used_memory += total_size;

		return allocation_address;
	}

	//No block of fitting size found
	return nullptr;

}

void FreeListAllocator::deallocate(const void* p)
{
}

void FreeListAllocator::clear()
{
	free_blocks = reinterpret_cast<FreeBlock*>(memory_block);
	free_blocks->next = nullptr;
	free_blocks->size = blocksize;


	number_allocations = 0;
	used_memory = 0;
}


