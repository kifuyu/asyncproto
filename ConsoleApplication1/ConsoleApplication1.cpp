
#include "pch.h"

#include <cstddef>
#include <windows.h>

#include <future>
#include <iostream>
#include <experimental/coroutine>  // CoroutinesTS
#include <experimental/resumable>  

#include "WorkerThread.h"
#include "IFiber.h"


WorkerThread wt;

int main()
{
	wt.Init();
	auto ret1 = wt.Async([]() {
		std::cout << "start 1" << std::endl;
		wt.Delay(5);
		std::cout << "end 1" << std::endl;
		return "ope1";
	});
	auto ret2 = wt.Async([]() {
		std::cout << "start 2" << std::endl;
		wt.Delay(5);
		std::cout << "end 2" << std::endl;
		return 2;
	});
	auto v1 = ret1.get();
	auto v2 = ret2.get();
	std::cout << v1 << ":" << v2;

	wt.Terminate();
}
