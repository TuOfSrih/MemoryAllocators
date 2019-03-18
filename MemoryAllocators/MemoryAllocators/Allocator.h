#pragma once
#include <inttypes.h>
class Allocator
{
public:
	
	Allocator(size_t size, void* memory_block);
	virtual ~Allocator();
	Allocator(const Allocator& alloc)				= delete;
	Allocator(Allocator&& alloc)					= delete;
	Allocator& operator= (const Allocator& alloc)	= delete;
	Allocator& operator= (Allocator&& alloc)		= delete;

	virtual void*	allocate(size_t size, size_t alignment) = 0;
	virtual void	deallocate(void* p) = 0;
	virtual void	clear() = 0;

	void*			get_memory_block();
	size_t			get_size();
	size_t			get_number_allocations();
	size_t			get_used_memory();

protected:

	void*			memory_block;
	size_t			blocksize;
	size_t			number_allocations = 0;
	size_t			used_memory = 0;
};


namespace Memory
{
	template <typename T> 
	T* allocate(Allocator& allocator) {

		return new (allocator.allocate(sizeof(T), alignof(T))) T;
	}

	template <typename T>
	T* allocate(Allocator& allocator, const T& t) {

		return new (allocator.allocate(sizeof(T), alignof(T))) T(t);
	}

	template <typename T>
	void deallocate(Allocator& allocator, T& object) {

		object.~T();
		allocator.deallocate(&object);
	}

	inline uint8_t alignForwardAdjustment(void* p, uint8_t alignment);

};

