// MemoryAllocators.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Allocator.h"
#include "LinearAllocator.h"
#include <vector>
#include <chrono>

constexpr size_t TESTSIZE = 65536;
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

int main()
{
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

