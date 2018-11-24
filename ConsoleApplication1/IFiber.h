#pragma once
#include <future>

class IFiber
{
public:
	virtual void Run() = 0;
};
