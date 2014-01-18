#ifndef LIBSTL_REF_COUNTER_HPP_
#define LIBSTL_REF_COUNTER_HPP_

#include <simstl/impl/types.hpp>

namespace simstd {
	namespace pvt {

		struct ref_counter
		{
			virtual ~ref_counter();

			ref_counter();

			void increase_ref();

			void decrease_ref();

			bool is_shared() const;

			simstd::size_t count_ref() const;

		private:
			virtual void destroy() const;

			virtual void deallocate() const = 0;

			ref_counter(const ref_counter & rhs);    // forbidden

			ref_counter & operator =(const ref_counter & rhs);    // forbidden

		private:
			simstd::size_t m_refcnt;
		};

		inline ref_counter::ref_counter() :
			m_refcnt(1)
		{
		}

		inline void ref_counter::increase_ref()
		{
			++m_refcnt;
		}

		inline bool ref_counter::is_shared() const
		{
			return m_refcnt > 1;
		}

		inline simstd::size_t ref_counter::count_ref() const
		{
			return m_refcnt;
		}

	}
}

#endif
