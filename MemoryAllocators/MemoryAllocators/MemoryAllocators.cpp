// MemoryAllocators.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Allocator.h"
#include "LinearAllocator.h"

constexpr size_t TESTSIZE = 1024;
struct test {
	uint8_t byte;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;

};

int main()
{
    std::cout << "Hello" << std::endl; 
	void* p = malloc(TESTSIZE);

	LinearAllocator linAlloc(TESTSIZE, p);
	std::cout << "Adress" << p << std::endl;
	for (int i = 0; i < 257; ++i) {

		std::cout << "Allocation: " << i << ": " << Memory::allocate<test>(linAlloc) << std::endl;
	}

	free(p);
}

