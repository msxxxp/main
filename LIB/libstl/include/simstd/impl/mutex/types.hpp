#ifndef LIBSTL_MUTEX_TYPES_HPP_
#define LIBSTL_MUTEX_TYPES_HPP_

namespace simstd {

	/// Do not acquire ownership of the mutex.
	struct defer_lock_t
	{
	};

	/// Try to acquire ownership of the mutex without blocking.
	struct try_to_lock_t
	{
	};

	/// Assume the calling thread has already obtained mutex ownership
	/// and manage it.
	struct adopt_lock_t
	{
	};

	constexpr defer_lock_t defer_lock{};

	constexpr try_to_lock_t try_to_lock{};

	constexpr adopt_lock_t adopt_lock{};

}    // namespace simstd

#endif
