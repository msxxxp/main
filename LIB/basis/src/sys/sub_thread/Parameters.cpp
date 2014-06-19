#include <basis/sys/thread.hpp>
#include <basis/sys/totext.hpp>
#include <basis/std/string>

namespace thread {

	struct Parameters {
		static DWORD WINAPI run_thread_with_param(void * param);

		static VOID WINAPI alert_thread_with_param(ULONG_PTR param);

		Parameters(Routine * r, void * d = nullptr);

		Routine * routine;
		void * data;
	};

	DWORD WINAPI Parameters::run_thread_with_param(void * param)
	{
		Parameters* p_param = reinterpret_cast<Parameters*>(param);
		Parameters l_param(*p_param);
		delete p_param;
		return l_param.routine->run(l_param.data);
	}

	VOID WINAPI Parameters::alert_thread_with_param(ULONG_PTR param)
	{
		Parameters* p_param = reinterpret_cast<Parameters*>(param);
		Parameters l_param(*p_param);
		delete p_param;
		return l_param.routine->alert(l_param.data);
	}

	Parameters::Parameters(Routine * r, void * d) :
		routine(r),
		data(d)
	{
	}

	Unit::Unit(Routine * routine, void * data, bool suspended, size_t stack_size) :
		m_routine(routine),
		m_handle(::CreateThread(nullptr, stack_size, Parameters::run_thread_with_param, new Parameters(m_routine, data), suspended ? CREATE_SUSPENDED : 0, &m_id))
	{
		LogDebugIf(is_valid(), L"id: %u\n", m_id);
		LogFatalIf(!is_valid(), L"can't create thread (%p, %Iu) -> %s\n", routine, stack_size, totext::api_error().c_str());
	}

	bool Unit::alert(void * data)
	{
		bool ret = ::QueueUserAPC(Parameters::alert_thread_with_param, m_handle, reinterpret_cast<ULONG_PTR>(new Parameters(m_routine, data)));
		LogDebugIf(ret, L"id: %u\n", m_id);
		LogErrorIf(!ret, L"-> %s\n", totext::api_error().c_str());
		return ret;
	}

}
