#include <system/thread.hpp>

#include <liblog/logger.hpp>

namespace thread {

	namespace {
		logger::Module_i * get_logger_module()
		{
			auto static module = logger::get_module(L"threads");
			return module;
		}
	}

	DWORD WINAPI Routine::run_thread(void * routine)
	{
		LogNoise(L"%p\n", routine);
		Routine * l_routine = reinterpret_cast<Routine*>(routine);
		return l_routine->run(l_routine->get_parameter());
	}

	VOID WINAPI Routine::alert_thread(ULONG_PTR routine)
	{
		LogNoise(L"%p\n", routine);
		reinterpret_cast<Routine*>(routine)->alert(nullptr);
	}

	Routine::Routine()
	{
		LogTrace();
	}

	Routine::~Routine()
	{
		LogTrace();
	}

	void Routine::alert(void * data)
	{
		UNUSED(data);
		LogNoise(L"data: %p\n", data);
	}

	size_t Routine::run(void * data)
	{
		UNUSED(data);
		LogNoise(L"data: %p\n", data);
		return 0;
	}

	void * Routine::get_parameter()
	{
		return nullptr;
	}

	void Routine::put_message(const sync::Message & message)
	{
		UNUSED(message);
		LogNoise(L"type: %Id, code: %Id, param: %Id, data: %p\n", message.get_type(), message.get_code(), message.get_param(), message.get_data());
	}
}
