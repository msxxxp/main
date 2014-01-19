#ifndef LIBBASE_THREADPOOL_HPP_
#define LIBBASE_THREADPOOL_HPP_

#include <libbase/std.hpp>
#include <libbase/thread.hpp>
#include <patterns/Uncopyable.hpp>

#include <vector>

namespace Base {

	struct ThreadPool: private Pattern::Uncopyable, private std::vector<Thread> {
		typedef std::vector<Thread> base_type;
		using base_type::back;
		using base_type::begin;
		using base_type::end;
		using base_type::size;
		using base_type::empty;
		using base_type::operator[];

		ThreadPool();

		~ThreadPool();

		template<class... Args>
		void create_thread(Args&&... args)
		{
			base_type::emplace_back(args...);
			m_handles.push_back(base_type::back().get_handle());
		}

//		void push_back(Thread thread)
//		{
//			m_threads.push_back(thread);
//			m_handles.push_back(thread.get_handle());
//		}

//		Thread & operator [](size_t i)
//		{
//			return m_threads[i];
//		}

		WaitResult_t wait_all(Timeout_t timeout = WAIT_FOREVER) const;

		WaitResult_t wait_any(size_t & index, Timeout_t timeout = WAIT_FOREVER) const;

	private:
		std::vector<Thread::handle_t> m_handles;
	};

}

#endif
