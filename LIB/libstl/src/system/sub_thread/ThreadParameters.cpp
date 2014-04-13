#include <system/thread.hpp>

namespace thread {

	struct ThreadParameters {
		static DWORD WINAPI run_thread_with_param(void * param);

		static VOID WINAPI alert_thread_with_param(ULONG_PTR param);

		ThreadParameters(Routine * r, void * d = nullptr);

		Routine * routine;
		void * data;
	};


	ThreadParameters::ThreadParameters(Routine * r, void * d):
		routine(r),
		data(d)
	{
	}

	DWORD WINAPI ThreadParameters::run_thread_with_param(void * param) {
		ThreadParameters* p_param = reinterpret_cast<ThreadParameters*>(param);
		ThreadParameters l_param(*p_param);
		delete p_param;
		return l_param.routine->run(l_param.data);
	}

	VOID WINAPI ThreadParameters::alert_thread_with_param(ULONG_PTR param) {
		ThreadParameters* p_param = reinterpret_cast<ThreadParameters*>(param);
		ThreadParameters l_param(*p_param);
		delete p_param;
		return l_param.routine->alert(l_param.data);
	}


	Thread::Thread(Routine * routine, void * data, bool suspended, size_t stack_size):
		m_routine(routine),
		m_handle(::CreateThread(nullptr, stack_size, ThreadParameters::run_thread_with_param, new ThreadParameters(m_routine, data), suspended ? CREATE_SUSPENDED : 0, &m_id))
	{
	}

	void Thread::alert(void * data) {
		::QueueUserAPC(ThreadParameters::alert_thread_with_param, m_handle, (ULONG_PTR)new ThreadParameters(m_routine, data));
	}

}
