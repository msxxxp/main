#ifndef LIBSTL_SYSTEM_THREAD_UNIT_HPP_
#define LIBSTL_SYSTEM_THREAD_UNIT_HPP_

#include <system/thread.hpp>
#include <system/sync.hpp>
#include <extra/pattern.hpp>

namespace thread {

//	template <typename Type, size_t (Type::*mem_func)(void *)>
//	DWORD WINAPI member_thunk(void * ptr)
//	{ // http://www.rsdn.ru/Forum/Message.aspx?mid=753888&only=1
//		return (static_cast<Type*>(ptr)->*mem_func)(nullptr);
//	}
//
//	template <typename Type, void (Type::*mem_func)(void *)>
//	VOID WINAPI apc_thunk(ULONG_PTR ptr)
//	{
//		(((Type*)(ptr))->*mem_func)(nullptr);
//	}

	struct Thread: private pattern::Uncopyable {
		typedef HANDLE handle_t;
		typedef DWORD id_t;

		~Thread() noexcept;

		Thread(Routine * routine, bool suspended = false);

		Thread(Routine * routine, void * data, bool suspended = false, size_t stack_size = 0);

		Thread(Thread && right);

		Thread & operator = (Thread && right);

		void swap(Thread & right) noexcept;

		void alert();

		void alert(void * data);

		bool set_priority(Priority prio);

		bool set_io_priority(IoPriority prio);

		size_t get_exitcode() const;

		id_t get_id() const
		{
			return m_id;
		}

		Thread::handle_t get_handle() const
		{
			return m_handle;
		}

		Priority get_priority() const;

		IoPriority get_io_priority() const;

		Routine * get_routine() const
		{
			return m_routine;
		}

		bool suspend() const;

		bool resume() const;

		sync::WaitResult_t wait(sync::Timeout_t timeout = sync::WAIT_FOREVER) const;

	private:
		Routine * m_routine;
		handle_t m_handle;
		id_t m_id;
	};

}

#endif
