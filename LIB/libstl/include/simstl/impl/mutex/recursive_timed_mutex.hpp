#ifndef LIBSTL_MUTEX_RECURSIVE_TIMED_MUTEX_HPP_
#define LIBSTL_MUTEX_RECURSIVE_TIMED_MUTEX_HPP_

#include <simstl/impl/mutex/recursive_mutex_base.hpp>

#include <chrono>

namespace simstd {

	class recursive_timed_mutex: public pvt::recursive_mutex_base
	{
		typedef pvt::recursive_mutex_base base_type;

	public:
		typedef base_type::native_handle_type native_handle_type;

		using base_type::lock;

		using base_type::try_lock;

		using base_type::unlock;

		using base_type::native_handle;

		template<class Rep, class Period>
		bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration);

		template<class Clock, class Duration>
		bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time);

	private:
		bool _try_lock_for(size_t timeout_millisec);
	};

	template<class Rep, class Period>
	bool recursive_timed_mutex::try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration)
	{
		using namespace std::chrono;
		return _try_lock_for(duration_cast<milliseconds>(timeout_duration).count());
	}

	template<class Clock, class Duration>
	bool recursive_timed_mutex::try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
	{
		typedef std::chrono::steady_clock clock_t;

		using namespace std::chrono;
		return _try_lock_for(duration_cast<milliseconds>(timeout_time - clock_t::now()).count());
	}

}    // namespace simstd

#endif
