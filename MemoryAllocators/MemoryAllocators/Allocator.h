#pragma once
class Allocator
{
public:
	
	Allocator(size_t size, void* memory_block);
	virtual ~Allocator();

	virtual void*	allocate(size_t size, size_t alignment) = 0;
	virtual void	deallocate(void* p) = 0;

	void*			get_memory_block();
	size_t			get_size();
	size_t			get_number_allocations();
	size_t			get_used_memory();

protected:

	void*			memory_block;
	size_t			size;
	size_t			number_allocations = 0;
	size_t			used_memory = 0;
};


namespace Allocation
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



	template <typename T>
	T* allocateArray(Allocator& allocator, size_t length) {

		uint8_t padding = sizeof(size_t) - (sizeof(T) * length) % sizeof(size_t);

		T* p = Allocator.allocate(sizeof(T) * length + padding, alignof(T));
	}

	template <typename T>
	void deallocateArray(Allocator& allocator, T* array) {

		object.~T();
		allocator.deallocate(&object);
	}

};

