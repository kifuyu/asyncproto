#pragma once

#include <future>
#include <list>
#include "FiberBase.h"
#include "IFiber.h"

template <class F, typename R, typename... ArgTypes> R _getResult(R(F::*)(ArgTypes...));
template <class F, typename R, typename... ArgTypes> R _getResult(R(F::*)(ArgTypes...) const);
template <class F> using getResultType = decltype(_getResult(&F::operator()));


template<class T>
class TFiber : public IFiber
{
public:
	TFiber(T f, std::promise<getResultType<T>> prm) : func_(f), prm_(std::move(prm))
	{
	}
	virtual void Run() override
	{
		prm_.set_value(func_());
	}
private:
	std::function<getResultType<T>()> func_;
	std::promise<getResultType<T>> prm_;
};
class WorkerThread
{
public:
	WorkerThread() : m_bTerminate(false) {}
	template <class T>
	std::future<getResultType<T>> Async(T f)
	{
		std::promise<getResultType<T>> p;
		auto ret = p.get_future();
		auto fib = new TFiber<T>(f, std::move(p));
		{
			std::lock_guard<std::mutex> g(m_fmtx);
			if (m_fibers.size() > 0)
			{
				auto& prev = m_fibers.back();
				m_fibers.emplace_back(fib, this);
				prev.SetNext(&m_fibers.back());
			}
			else
			{
				m_fibers.emplace_back(fib, this);
			}
			m_fibers.back().Init();
		}
		m_fibNum++;
		m_cond.notify_one();
		return ret;
	}
	void RegisterAsyncMethod(IFiber* fib)
	{
		{
			std::lock_guard<std::mutex> g(m_fmtx);
			if (m_fibers.size() > 0)
			{
				auto& prev = m_fibers.back();
				m_fibers.emplace_back(fib, this);
				prev.SetNext(&m_fibers.back());
			}
			else
			{
				m_fibers.emplace_back(fib, this);
			}
			m_fibers.back().Init();
		}
		m_fibNum++;
		m_cond.notify_one();
	}
	void Delay(int sec);
	void Switch();
	void Init();
	void Terminate();
private:
	LPVOID m_pMainFiber;
	std::thread* m_thread;
	std::mutex m_fmtx;
	std::mutex m_rmtx;
	std::condition_variable m_cond;
	std::atomic_int m_fibNum;
	std::list<FiberBase> m_fibers;
	FiberBase* m_current;
	bool m_bTerminate;
};