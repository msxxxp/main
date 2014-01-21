#ifndef LIBSTL_MUTEX_RECURSIVE_MUTEX_BASE_HPP_
#define LIBSTL_MUTEX_RECURSIVE_MUTEX_BASE_HPP_

#include <extra/pattern.hpp>

namespace simstd {
	namespace pvt {

		class recursive_mutex_base: private pattern::Uncopyable
		{
			struct native_handle;

		public:
			typedef native_handle * native_handle_type;

			~recursive_mutex_base();

			recursive_mutex_base();

			void lock();

			bool try_lock();

			void unlock();

			native_handle_type native_handle();

		protected:
			native_handle_type m_handle;
		};

	}    // namespace pvt
}    // namespace simstd

#endif
