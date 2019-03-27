// MemoryAllocators.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Allocator.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "FreeListAllocator.h"
#include "PoolAllocator.h"
#include "ProxyAllocator.h"
#include <vector>
#include <chrono>
#include <stack>

constexpr const size_t TESTSIZE = 65536;
struct Test4 {
	uint8_t byte;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;

	Test4() : byte(0xFF), byte2(0x00), byte3(0x00), byte4(0xFF){}
};
template<unsigned int u>
struct Test {
	uint8_t arr[u];
};

void testRandom(Allocator& alloc, std::vector<void*>& pointers) {

	for (int i = 0; i < 1024; ++i) {

		pointers.push_back(alloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}

	for (int i = 0; i < 1024; ++i) {

		int random = rand() % pointers.size();
		alloc.deallocate(pointers.at(random));
		pointers.erase(pointers.begin() + random);

	}
}

void testLinear(){

	std::cout << "Hello" << std::endl;
	//void* p = malloc(TESTSIZE);
	std::vector<Test4*> vec(16385);

	auto start_standard = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1638400; ++i) {

		malloc(sizeof(Test<4>));
		//vec.at(i) = reinterpret_cast<Test*>(malloc(sizeof(Test)));
		//std::cout << "Allocation: " << i << ": " << vec.at(i) << std::endl;
		//vec.at(i) = new (vec.at(i)) Test();
	}
	auto end_standard = std::chrono::high_resolution_clock::now();
	auto diff_standard = std::chrono::duration_cast<std::chrono::microseconds>(end_standard - start_standard);
	std::cout << "Standard Allocation needed: " << diff_standard.count() << " microseconds" << std::endl;


	auto start_lin = std::chrono::high_resolution_clock::now();
	LinearAllocator linAlloc(TESTSIZE);
	for (int i = 0; i < 1638400; ++i) {

		linAlloc.allocate(sizeof(Test<4>), alignof(Test<4>));
		//vec.at(i) = Memory::allocate<Test>(linAlloc);
		//std::cout << "Allocation: " << i << ": " << vec.at(i) << std::endl;
	}
	auto end_lin = std::chrono::high_resolution_clock::now();
	auto diff_lin = std::chrono::duration_cast<std::chrono::microseconds>(end_lin - start_lin);
	std::cout << "Linear Allocation needed: " << diff_lin.count() << " microseconds" << std::endl;
}

void testStack() {
	
	std::stack<void*> pointers;

	StackAllocator stackAlloc(TESTSIZE);

	for (int i = 0; i < 1024; ++i) {

		pointers.push(stackAlloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}


	for (int i = 0; i < 1024; ++i) {

		stackAlloc.deallocate(pointers.top());
		pointers.pop();
	}

	stackAlloc.clear();

	for (int i = 0; i < 1024; ++i) {

		pointers.push(stackAlloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}

	stackAlloc.clear();
	while (!pointers.empty()) pointers.pop();

	for (int i = 0; i < 1024; ++i) {

		pointers.push(stackAlloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}

	for (int i = 0; i < 512; ++i) {

		stackAlloc.deallocate(pointers.top());
		pointers.pop();
	}

	for (int i = 0; i < 512; ++i) {

		pointers.push(stackAlloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}

	for (int i = 0; i < 1024; ++i) {

		stackAlloc.deallocate(pointers.top());
		pointers.pop();
	}

	
}

void testFreeList() {

	FreeListAllocator listAlloc(TESTSIZE);
	std::vector<void*> pointers;

	for (int i = 0; i < 5; ++i) {

		pointers.push_back(listAlloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}

	listAlloc.deallocate(pointers.at(0));
	listAlloc.deallocate(pointers.at(4));
	listAlloc.deallocate(pointers.at(2));
	listAlloc.deallocate(pointers.at(1));
	listAlloc.deallocate(pointers.at(3));

	listAlloc.clear();
	pointers.clear();

	testRandom(listAlloc, pointers);

	for (int i = 0; i < 1024; ++i) {

		pointers.push_back(listAlloc.allocate(sizeof(Test<4>), alignof(Test<4>)));
	}

	listAlloc.clear();
}

void testPool() {

	PoolAllocator poolAlloc(TESTSIZE, sizeof(Test<4>), alignof(Test<4>));

	std::vector<void*> pointers;

	testRandom(poolAlloc, pointers);

	poolAlloc.clear();

	testRandom(poolAlloc, pointers);
}

void testProxy() {

	PoolAllocator poolAlloc(TESTSIZE, sizeof(Test<4>), alignof(Test<4>));

	ProxyAllocator proxAlloc(TESTSIZE, poolAlloc);

	proxAlloc.registerAllocateCallback([](size_t size, size_t alignment) { std::cout << size << std::endl; });

	proxAlloc.registerDeallocateCallback([](void* p) { std::cout << p << std::endl; });

	std::vector<void*> pointers;

	testRandom(proxAlloc, pointers);

	proxAlloc.clear();
	pointers.clear();

	testRandom(proxAlloc, pointers);

}

int main()
{
	testLinear();
	testStack();
	testFreeList();
	testPool();
	testProxy();
}

