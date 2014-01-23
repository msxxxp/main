#ifndef LIBSTL_SYSTEM_SYNC_MUTEX_HPP_
#define LIBSTL_SYSTEM_SYNC_MUTEX_HPP_

#include <extra/pattern.hpp>

namespace sync {

	class Mutex: private pattern::Uncopyable
	{
		struct native_impl_type;

	public:
		typedef native_impl_type * native_handle_type;

		~Mutex();

		Mutex(const wchar_t * name = nullptr);

		void lock();

		bool try_lock(size_t timeout_millisec = 0);

		void unlock();

		native_handle_type native_handle();

	private:
		native_handle_type m_handle;
	};

}

#endif
