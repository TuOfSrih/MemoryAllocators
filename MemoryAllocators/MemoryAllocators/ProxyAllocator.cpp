#include "pch.h"
#include "ProxyAllocator.h"
#include <cassert>


ProxyAllocator::ProxyAllocator(const size_t blocksize, void* memory_block, Allocator& allocator) : Allocator(blocksize, memory_block), allocator(allocator)
{

}

ProxyAllocator::ProxyAllocator(const size_t blocksize, Allocator& allocator) : Allocator(blocksize), allocator(allocator)
{
}

void* ProxyAllocator::allocate(const size_t size, const size_t alignment)
{
	assert(size && alignment);

	for (auto& func : allocateCallbacks) func(size, alignment);

	return allocator.allocate(size, alignment);

}

void ProxyAllocator::deallocate(void* p)
{
	assert(p);

	for (auto& func : deallocateCallbacks) func(p);

	allocator.deallocate(p);
}

void ProxyAllocator::clear()
{
	for (auto& func : clearCallbacks) func();

	allocator.clear();
}

void ProxyAllocator::registerAllocateCallback(std::function<void*(const size_t, const size_t)>& func)
{
	allocateCallbacks.push_back(func);
}

void ProxyAllocator::registerDeallocateCallback(std::function<void(void*)>& func)
{
	deallocateCallbacks.push_back(func);
}

void ProxyAllocator::registerClearCallback(std::function<void()>& func)
{
	clearCallbacks.push_back(func);
}


ProxyAllocator::~ProxyAllocator()
{
}
