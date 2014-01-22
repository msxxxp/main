#ifndef LIBSTL_SYSTEM_SYNC_SEMAPHORE_HPP_
#define LIBSTL_SYSTEM_SYNC_SEMAPHORE_HPP_

#include <extra/pattern.hpp>

namespace sync {

	class Semaphore: private pattern::Uncopyable
	{
		struct native_handle;

	public:
		typedef native_handle * native_handle_type;

		~Semaphore();

		Semaphore(const wchar_t * name = nullptr);

		void lock();

		bool try_lock(size_t timeout_millisec = 0);

		void unlock(size_t cnt = 1);

		native_handle_type native_handle();

	private:
		native_handle_type m_handle;
	};

}

#endif
