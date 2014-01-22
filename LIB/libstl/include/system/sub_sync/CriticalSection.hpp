#ifndef LIBSTL_SYSTEM_SYNC_CRITICALSECTION_HPP_
#define LIBSTL_SYSTEM_SYNC_CRITICALSECTION_HPP_

#include <extra/pattern.hpp>

namespace sync {

	class CriticalSection: private pattern::Uncopyable
	{
		struct native_handle;

	public:
		typedef native_handle * native_handle_type;

		~CriticalSection();

		CriticalSection();

		void lock();

		bool try_lock();

		void unlock();

		native_handle_type native_handle();

	private:
		native_handle_type m_handle;
	};

}

#endif
