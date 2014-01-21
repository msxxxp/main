#ifndef LIBSTL_MUTEX_RECURSIVE_MUTEX_HPP_
#define LIBSTL_MUTEX_RECURSIVE_MUTEX_HPP_

#include <simstl/impl/mutex/recursive_mutex_base.hpp>

namespace simstd {

	class recursive_mutex: public pvt::recursive_mutex_base
	{
		typedef pvt::recursive_mutex_base base_type;

	public:
		typedef base_type::native_handle_type native_handle_type;

		using base_type::lock;

		using base_type::try_lock;

		using base_type::unlock;

		using base_type::native_handle;
	};

}    // namespace simstd

#endif
