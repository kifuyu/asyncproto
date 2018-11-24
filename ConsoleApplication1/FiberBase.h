#pragma once

#include <windows.h>

class WorkerThread;
class IFiber;

class FiberBase
{
public:
	FiberBase(IFiber* fib, WorkerThread * root);
	~FiberBase();
	void SetDelay(int sec);
	bool Check();
	void DoRun();
	bool Init();
	bool IsFnished();

	LPVOID GetFiber();
	void SwitchToNext();
	void SetNext(FiberBase* pNext);
	FiberBase* GetNext();
private:
	LPVOID m_sysFiber;
	WorkerThread* m_pRoot;
	FiberBase* m_pNext;
	IFiber* m_fiber;
	int m_delay;
	time_t m_start;
	bool m_bFin;
};
