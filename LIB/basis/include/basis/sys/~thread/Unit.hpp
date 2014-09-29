#ifndef BASIS_SYS_THREAD_UNIT_HPP_
#define BASIS_SYS_THREAD_UNIT_HPP_

#include <basis/sys/thread.hpp>
#include <basis/sys/sync.hpp>
#include <basis/ext/pattern.hpp>

namespace thread {

	class Unit: private pattern::Uncopyable {
	public:
		typedef HANDLE handle_type;
		typedef DWORD id_type;

	public:
		~Unit() noexcept;

		Unit(Routine * routine, bool suspended = false, size_t stack_size = 0);

		Unit(Routine * routine, void * data, bool suspended = false, size_t stack_size = 0);

		Unit(Unit && right);

		Unit & operator = (Unit && right);

		void swap(Unit & right) noexcept;

		bool is_valid() const;

		bool alert();

		bool alert(void * data);

		bool set_priority(Priority prio);

		bool set_io_priority(IoPriority prio);

		size_t get_exitcode() const;

		id_type get_id() const;

		handle_type get_handle() const;

		Priority get_priority() const;

		IoPriority get_io_priority() const;

		Routine * get_routine() const;

		bool suspend() const;

		bool resume() const;

		sync::WaitResult_t wait(sync::Timeout_t timeout = sync::WAIT_FOREVER) const;

	private:
		Routine * m_routine;
		handle_type m_handle;
		id_type m_id;
	};

}

#endif
