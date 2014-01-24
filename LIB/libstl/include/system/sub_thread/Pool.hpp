#ifndef LIBSTL_SYSTEM_THREAD_POOL_HPP_
#define LIBSTL_SYSTEM_THREAD_POOL_HPP_

#include <system/configure.hpp>
#include <system/thread.hpp>
#include <system/sync.hpp>
#include <extra/pattern.hpp>

#include <simstd/vector>

namespace thread {

	struct ThreadPool: private pattern::Uncopyable, private simstd::vector<Thread> {
		typedef simstd::vector<Thread> base_type;
		using base_type::back;
		using base_type::begin;
		using base_type::end;
		using base_type::size;
		using base_type::empty;
		using base_type::operator[];

		ThreadPool();

		~ThreadPool();

//		template<class... Args>
//		void create_thread(Args&&... args)
//		{
//			base_type::emplace_back(args...);
//			m_handles.push_back(base_type::back().get_handle());
//		}

//		void push_back(Thread thread)
//		{
//			m_threads.push_back(thread);
//			m_handles.push_back(thread.get_handle());
//		}

//		Thread & operator [](size_t i)
//		{
//			return m_threads[i];
//		}

		sync::WaitResult_t wait_all(sync::Timeout_t timeout = sync::WAIT_FOREVER) const;

		sync::WaitResult_t wait_any(size_t & index, sync::Timeout_t timeout = sync::WAIT_FOREVER) const;

	private:
		simstd::vector<Thread::handle_t> m_handles;
	};

}

#endif
