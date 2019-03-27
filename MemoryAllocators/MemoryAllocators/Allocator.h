#pragma once
#include <inttypes.h>
class Allocator
{
public:
	
	Allocator(const size_t blocksize, void* memory_block);
	Allocator(const size_t blocksize);
	virtual ~Allocator();

	Allocator(const Allocator& alloc)				= delete;
	Allocator(Allocator&& alloc)					= delete;
	Allocator& operator= (const Allocator& alloc)	= delete;
	Allocator& operator= (Allocator&& alloc)		= delete;

	virtual void*	allocate(const size_t size, const size_t alignment) = 0;
	virtual void	deallocate(void* p) = 0;
	virtual void	clear() = 0;

	void*			get_memory_block() const;
	size_t			get_size() const;
	size_t			get_number_allocations() const;
	size_t			get_used_memory() const;

protected:

	void*			memory_block;
	size_t			blocksize;
	size_t			number_allocations = 0;
	size_t			used_memory = 0;
};


namespace Memory
{
	template <typename T> 
	T* allocate(const Allocator& allocator) {

		return new (allocator.allocate(sizeof(T), alignof(T))) T;
	}

	template <typename T>
	T* allocate(const Allocator& allocator, const T& t) {

		return new (allocator.allocate(sizeof(T), alignof(T))) T(t);
	}

	template <typename T>
	void deallocate(const Allocator& allocator, T& object) {

		object.~T();
		allocator.deallocate(&object);
	}
	void alignForward(void* p, const uint8_t alignment);

	uint8_t alignForwardAdjustment(const void* p, const uint8_t alignment);

	uint8_t alignForwardAdjustmentWithHeader(const void* p, const uint8_t alignment, uint8_t headersize);
};

