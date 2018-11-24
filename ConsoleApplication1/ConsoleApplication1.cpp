
#include "pch.h"

#include <cstddef>
#include <windows.h>

#include <future>
#include <iostream>
#include <experimental/coroutine>  // CoroutinesTS
#include <experimental/resumable>  

#include "WorkerThread.h"
#include "IFiber.h"
#include "AsyncSample.h"

WorkerThread wt;



int main()
{
	wt.Init();
	auto ret1 = wt.Async([]() {
		std::cout << "start 1" << std::endl;
		wt.Delay(2);
		std::cout << "end 1" << std::endl;
		return "ope1";
	});
	auto ret2 = wt.Async([]() {
		std::cout << "start 2" << std::endl;
		wt.Delay(2);
		std::cout << "end 2" << std::endl;
		return 2;
	});

	AsyncSample cls(&wt);
	auto ret3 = cls.DoAsync();

	auto v1 = ret1.get();
	auto v2 = ret2.get();
	auto v3 = ret3.get();
	std::cout << v1 << ":" << v2 << ":" << v3 << std::endl;

	wt.Terminate();
}
