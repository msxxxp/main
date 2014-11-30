#ifndef BASIS_SYS_SYNC_SEMAPHORE_HPP_
#define BASIS_SYS_SYNC_SEMAPHORE_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	class Semaphore: private pattern::Uncopyable
	{
		struct native_imp_type;

	public:
		typedef native_imp_type * native_handle_type;

		~Semaphore();

		Semaphore(const wchar_t* name = nullptr);

		void lock();

		bool try_lock(size_t timeout_millisec = 0);

		WaitResult_t try_lock_ex(size_t timeout_millisec = 0);

		void unlock(size_t cnt = 1);

		native_handle_type native_handle();

	private:
		native_handle_type m_handle;
	};

}

#endif
