#ifndef _LIBBASE_THREADPOOL_HPP_
#define _LIBBASE_THREADPOOL_HPP_

#include <libbase/std.hpp>
#include <libbase/uncopyable.hpp>
#include <libbase/thread.hpp>

namespace Base {

	struct ThreadPool: private Base::Uncopyable {
		ThreadPool();

		~ThreadPool();

		size_t size() const
		{
			return m_threads.size();
		}

		template< class... Args >
		void emplace_back(Args&&... args)
		{
			m_threads.emplace_back(args...);
			m_handles.push_back(m_threads.back().get_handle());
		}

//		void push_back(Thread thread)
//		{
//			m_threads.push_back(thread);
//			m_handles.push_back(thread.get_handle());
//		}

		Thread & back()
		{
			return m_threads.back();
		}

		Thread & operator [](size_t i)
		{
			return m_threads[i];
		}

		WaitResult_t wait_all(Timeout_t timeout = WAIT_FOREVER) const;

		WaitResult_t wait_any(size_t & index, Timeout_t timeout = WAIT_FOREVER) const;

	private:
		std::vector<Thread> m_threads;
		std::vector<Thread::handle_t> m_handles;
	};

}

#endif
