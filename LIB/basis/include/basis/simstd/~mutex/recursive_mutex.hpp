#ifndef BASIS_MUTEX_RECURSIVE_MUTEX_HPP_
#define BASIS_MUTEX_RECURSIVE_MUTEX_HPP_

#include <basis/sys/~sync/Mutex.hpp>

namespace simstd {

	class recursive_mutex: private sync::Mutex
	{
		typedef sync::Mutex base_type;

	public:
		typedef base_type::native_handle_type native_handle_type;

		using base_type::lock;

		using base_type::try_lock;

		using base_type::unlock;

		using base_type::native_handle;
	};

}    // namespace simstd

#endif
