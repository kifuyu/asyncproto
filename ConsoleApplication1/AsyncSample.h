#pragma once
#include "IFiber.h"
#include "WorkerThread.h"

class AsyncSample :
	public IFiber
{
public:
	AsyncSample(WorkerThread* wt);
	~AsyncSample();
	std::future<const char*> DoAsync();
private:
	virtual void Run() override;

private:
	std::promise<const char*> prm_;
	WorkerThread* wt_;
};

