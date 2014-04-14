#include <system/thread.hpp>
#include <system/totext.hpp>
#include <simstd/string>

#include <liblog/logger.hpp>

namespace thread {

	struct Parameters {
		static DWORD WINAPI run_thread_with_param(void * param);

		static VOID WINAPI alert_thread_with_param(ULONG_PTR param);

		Parameters(Routine * r, void * d = nullptr);

		Routine * routine;
		void * data;
	};

	Parameters::Parameters(Routine * r, void * d) :
		routine(r),
		data(d)
	{
	}

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

	Unit::Unit(Routine * routine, void * data, bool suspended, size_t stack_size) :
		m_routine(routine),
		m_handle(::CreateThread(nullptr, stack_size, Parameters::run_thread_with_param, new Parameters(m_routine, data), suspended ? CREATE_SUSPENDED : 0, &m_id)),
		m_id()
	{
		LogDebugIf(is_valid(), L"id: %u\n", m_id);
		LogFatalIf(!is_valid(), L"can't create thread (%p, %Iu) -> %s\n", routine, stack_size, totext::api_error().c_str());
	}

	void Unit::alert(void * data)
	{
		::QueueUserAPC(Parameters::alert_thread_with_param, m_handle, (ULONG_PTR)new Parameters(m_routine, data));
	}

}
