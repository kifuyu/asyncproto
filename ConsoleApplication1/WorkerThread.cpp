#include "pch.h"
#include "WorkerThread.h"
#include <iostream>

void WorkerThread::Delay(int sec)
{
	// current fiber
	m_current->SetDelay(sec);
	Switch();
}
void WorkerThread::Switch()
{
	FiberBase* nextfib = nullptr;
	if (m_current == nullptr)
	{
		nextfib = &m_fibers.front();
	}
	else
	{
		nextfib = m_current->GetNext();
	}
	while (true)
	{
		if (nextfib == nullptr || nextfib->GetFiber() == nullptr)
		{
			// main‚Ü‚Å–ß‚Á‚Ä‚«‚½
			m_current = nullptr;
			SwitchToFiber(m_pMainFiber);
			return;
		}
		if (nextfib->IsFnished())
		{
			nextfib = nextfib->GetNext();
			continue;
		}
		if (nextfib->Check()) break;
	}

	m_current = nextfib;
	SwitchToFiber(nextfib->GetFiber());
}
void WorkerThread::Terminate()
{
	m_fibNum = 100;
	m_bTerminate = true;
	m_cond.notify_all();
	m_thread->join();
}
void WorkerThread::Init()
{
	m_thread = new std::thread([this]() {


		m_fibNum = 0;
		std::unique_lock<std::mutex> lk(m_rmtx);
		while (true)
		{
			m_cond.wait(lk, [this]() {
				return m_fibNum > 0;
			});
			if (m_bTerminate)
			{
				break;
			}
			{
				std::lock_guard<std::mutex> g(m_fmtx);

				// fiber‚É•ÏŠ·
				{
					//std::cout << "start main fiber ==================" << std::endl;
					m_pMainFiber = ConvertThreadToFiber(nullptr);
				}

				Switch();

				ConvertFiberToThread();

			}
		}
	});
}
