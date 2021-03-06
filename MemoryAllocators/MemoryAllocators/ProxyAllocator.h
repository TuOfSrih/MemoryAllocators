#pragma once
#include "Allocator.h"
#include <vector>
#include <functional>
class ProxyAllocator : public Allocator
{
public:
	ProxyAllocator(const size_t blocksize, void* const memory_block, Allocator& allocator);
	ProxyAllocator(const size_t blocksize, Allocator& allocator);

	ProxyAllocator(const ProxyAllocator& alloc) = delete;
	ProxyAllocator(ProxyAllocator&& alloc) = delete;
	ProxyAllocator& operator= (const ProxyAllocator& alloc) = delete;
	ProxyAllocator& operator= (ProxyAllocator&& alloc) = delete;

	void* allocate(const size_t size, const size_t alignment) override;
	void  deallocate(void* const p) override;
	void  clear() override;

	void registerAllocateCallback(const std::function<void(const size_t, const size_t)> func);
	void registerDeallocateCallback(const std::function<void(void*)> func);
	void registerClearCallback(const std::function<void()> func);
	
	

	~ProxyAllocator();

private:

	Allocator& allocator;

	std::vector<std::function<void(const size_t, const size_t)>> allocateCallbacks;
	std::vector<std::function<void(void*)>> deallocateCallbacks;
	std::vector<std::function<void()>> clearCallbacks;
	
};

