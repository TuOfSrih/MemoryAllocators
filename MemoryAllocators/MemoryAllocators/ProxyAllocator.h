#pragma once
#include "Allocator.h"
class ProxyAllocator : public Allocator
{
public:
	ProxyAllocator();
	~ProxyAllocator();
};

