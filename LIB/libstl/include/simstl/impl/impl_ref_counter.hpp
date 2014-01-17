#ifndef SARALIB_STL_REF_COUNTER_HPP_
#define SARALIB_STL_REF_COUNTER_HPP_

#include "impl_types.hpp"

namespace sarastd {
	namespace pvt {

		struct ref_counter {
			virtual ~ref_counter();

			ref_counter();

			void increase_ref();

			void decrease_ref();

			bool is_shared() const;

			sarastd::size_t count_ref() const;

		private:
			virtual void destroy() const;

			virtual void deallocate() const = 0;

			ref_counter(const ref_counter & rhs); // forbidden

			ref_counter & operator = (const ref_counter & rhs); // forbidden

		private:
			sarastd::size_t m_refcnt;
		};

		inline ref_counter::~ref_counter() {}

		inline ref_counter::ref_counter() : m_refcnt(1) {}

		inline void ref_counter::increase_ref() {++m_refcnt;}

		inline void ref_counter::decrease_ref()
		{
			if (--m_refcnt == 0) {
				destroy();
				deallocate();
			}
		}

		inline bool ref_counter::is_shared() const {return m_refcnt > 1;}

		inline sarastd::size_t ref_counter::count_ref() const {return m_refcnt;}

		inline void ref_counter::destroy() const {}

	}
}

#endif
