#include <libbase/ThreadPool.hpp>
#include <libbase/err.hpp>
#include <libbase/logger.hpp>

namespace Base {
	static Base::Logger::Module_i * get_logger_module()
	{
		auto static module = Base::Logger::get_module(L"threads");
		return module;
	}

	ThreadPool::ThreadPool()
	{
		LogTrace();
	}

	ThreadPool::~ThreadPool()
	{
		LogTrace();
	}

	WaitResult_t ThreadPool::wait_all(Timeout_t timeout) const
	{
		LogTrace();
		WaitResult_t ret = (WaitResult_t)::WaitForMultipleObjectsEx(m_handles.size(), &m_handles[0], TRUE, timeout, TRUE);
		LogErrorIf(ret == WaitResult_t::FAILED, L"%s\n", ErrAsStr().c_str());
		LogNoise(L"ret: '%s'\n", to_str(ret));
		return ret;
	}

	WaitResult_t ThreadPool::wait_any(size_t & index, Timeout_t timeout) const
	{
		LogTrace();
		DWORD result = ::WaitForMultipleObjectsEx(m_handles.size(), &m_handles[0], FALSE, timeout, TRUE);
		WaitResult_t ret = WaitResult_t::FAILED;
		if (result == WAIT_IO_COMPLETION || result == WAIT_TIMEOUT || result == WAIT_FAILED) {
			ret = (WaitResult_t)result;
		} else if (result >= WAIT_ABANDONED_0) {
			index = result - WAIT_ABANDONED_0;
			ret = WaitResult_t::ABANDONED;
		} else {
			index = result - WAIT_OBJECT_0;
			ret = WaitResult_t::SUCCESS;
		}
		LogErrorIf(ret == WaitResult_t::FAILED, L"%s\n", ErrAsStr().c_str());
		LogNoise(L"ret: '%s'\n", to_str(ret));
		return ret;
	}

}