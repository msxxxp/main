#ifndef LIBSTL_MUTEX_RECURSIVE_TIMED_MUTEX_HPP_
#define LIBSTL_MUTEX_RECURSIVE_TIMED_MUTEX_HPP_

#include <system/sub_sync/Mutex.hpp>

#include <chrono>

namespace simstd {

	class recursive_timed_mutex: private sync::Mutex
	{
		typedef sync::Mutex base_type;

	public:
		typedef base_type::native_handle_type native_handle_type;

		using base_type::lock;

		bool try_lock();

		using base_type::unlock;

		using base_type::native_handle;

		template<class Rep, class Period>
		bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration);

		template<class Clock, class Duration>
		bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time);
	};

	inline bool recursive_timed_mutex::try_lock()
	{
		return base_type::try_lock();
	}

	template<class Rep, class Period>
	bool recursive_timed_mutex::try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration)
	{
		using namespace std::chrono;
		return base_type::try_lock(duration_cast<milliseconds>(timeout_duration).count());
	}

	template<class Clock, class Duration>
	bool recursive_timed_mutex::try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
	{
		typedef std::chrono::steady_clock clock_t;

		using namespace std::chrono;
		return base_type::try_lock(duration_cast<milliseconds>(timeout_time - clock_t::now()).count());
	}

}    // namespace simstd

#endif
