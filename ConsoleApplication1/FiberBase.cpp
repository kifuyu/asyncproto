#include "pch.h"
#include "FiberBase.h"
#include "IFiber.h"
#include "WorkerThread.h"
#include <iostream>

VOID WINAPI _fib_main(LPVOID arg)
{
	FiberBase* fib = (FiberBase*)arg;
	fib->DoRun();

	fib->SwitchToNext();
}

FiberBase::FiberBase(IFiber* fib, WorkerThread * root) : m_pRoot(root), m_fiber(fib), m_delay(0),m_pNext(nullptr) {}
void FiberBase::SetDelay(int sec)
{
	m_delay = sec;
	m_start = time(nullptr);
}
bool FiberBase::Check()
{
	if (m_delay == 0) return true;
	auto now = time(nullptr);
	bool ret = ((now - m_start) * 1000) > m_delay;
	m_start = now;
	if (!ret)
	{
		//std::cout << ".";
	}
	return ret;
}
void FiberBase::DoRun()
{
	m_bFin = false;
	m_fiber->Run();
	m_bFin = true;
}
bool FiberBase::IsFnished() { return m_bFin;  }
bool FiberBase::Init()
{
	m_bFin = false;
	m_sysFiber = CreateFiber(2048, _fib_main, this);
	return m_sysFiber != nullptr;
}
FiberBase::~FiberBase()
{
}
LPVOID FiberBase::GetFiber() { return m_sysFiber; }
void FiberBase::SwitchToNext()
{
	m_pRoot->Switch();
}
void FiberBase::SetNext(FiberBase* pNext)
{
	m_pNext = pNext;
}
FiberBase* FiberBase::GetNext()
{
	return m_pNext;
}