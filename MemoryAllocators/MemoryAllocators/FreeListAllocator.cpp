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
	assert(size && alignment);

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
	assert(p);
	
	//Acquire important pointers
	const AllocationHeader* header = reinterpret_cast<AllocationHeader*>((reinterpret_cast<size_t>(p) - sizeof(AllocationHeader)));
	void* const block_start = reinterpret_cast<void*>(reinterpret_cast<size_t>(p) - header->adjustment);

	FreeBlock* next_block = free_blocks;
	FreeBlock* previous_block = nullptr;

	//Find previous and next free block wrt. to deallocation
	while (next_block <= block_start && previous_block != nullptr) {

		previous_block = next_block;
		next_block = next_block->next;
	}

	FreeBlock* new_free_block = reinterpret_cast<FreeBlock*>(block_start);

	//Deallocation at the start or somewhere else
	if (previous_block == nullptr) {

		next_block = reinterpret_cast<FreeBlock*>(free_blocks);
		free_blocks = new_free_block;
		new_free_block->size = header->size;
		
	}
	else {

		//Current block is at the start of an allocated block
		if (reinterpret_cast<size_t>(previous_block) + previous_block->size == reinterpret_cast<size_t>(new_free_block)) {

			//Merge with previous block
			new_free_block = previous_block;
			new_free_block->size += header->size;
		}
		else {

			//Simply set size
			new_free_block->size = header->size;
		}
	}

	//Current Block is at the end of an allocated block
	if (reinterpret_cast<size_t>(new_free_block) + header->size == reinterpret_cast<size_t>(next_block) ){

		//Merge with next block
		new_free_block->size += next_block->size;
		new_free_block->next = next_block->next;
	}
	else {

		//Simply set next
		new_free_block->next = next_block;
	}
	//Update tracking stats
	number_allocations--;
	used_memory -= header->size;
}

void FreeListAllocator::clear()
{
	free_blocks = reinterpret_cast<FreeBlock*>(memory_block);
	free_blocks->next = nullptr;
	free_blocks->size = blocksize;


	number_allocations = 0;
	used_memory = 0;
}


