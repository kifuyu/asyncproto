#include "pch.h"
#include "AsyncSample.h"
#include <iostream>

AsyncSample::AsyncSample(WorkerThread* wt) : wt_(wt)
{
}

AsyncSample::~AsyncSample()
{
}

std::future<const char*> AsyncSample::DoAsync()
{
	auto ret = prm_.get_future();
	wt_->RegisterAsyncMethod(this);
	return ret;
}

void AsyncSample::Run()
{
	std::cout << "start async class ----------------" << std::endl;
	wt_->Delay(2);
	std::cout << "---------------- finish async class " << std::endl;

	prm_.set_value("did async");
}
