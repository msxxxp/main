#include <system/configure.hpp>
#include <system/thread.hpp>
#include <system/sync.hpp>

#include <liblog/logger.hpp>

namespace thread {
	namespace {
		logger::Module_i * get_logger_module()
		{
			auto static module = logger::get_module(L"threads");
			return module;
		}
	}

	DWORD WINAPI ThreadRoutine_i::run_thread(void * routine)
	{
		LogNoise(L"%p\n", routine);
		return reinterpret_cast<ThreadRoutine_i*>(routine)->run(nullptr);
	}

	VOID WINAPI ThreadRoutine_i::alert_thread(ULONG_PTR routine)
	{
		LogNoise(L"%p\n", routine);
		reinterpret_cast<ThreadRoutine_i*>(routine)->alert(nullptr);
	}

	ThreadRoutine_i::ThreadRoutine_i()
	{
		LogTrace();
	}

	ThreadRoutine_i::~ThreadRoutine_i() noexcept
	{
		LogTrace();
	}

	void ThreadRoutine_i::alert(void * data)
	{
		UNUSED(data);
		LogNoise(L"data: %p\n", data);
	}

	size_t ThreadRoutine_i::run(void * data)
	{
		UNUSED(data);
		LogNoise(L"data: %p\n", data);
		return 0;
	}

	void ThreadRoutine_i::put_message(const sync::Message & message)
	{
		UNUSED(message);
		LogNoise(L"type: %Id, code: %Id, param: %Id, data: %p\n", message.get_type(), message.get_code(), message.get_param(), message.get_data());
	}
}
