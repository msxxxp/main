#ifndef _LIBBASE_REF_CNT_HPP_
#define _LIBBASE_REF_CNT_HPP_

#include <libbase/std.hpp>

namespace Base {

	struct ref_counter {
		virtual ~ref_counter();

		ref_counter();

		void increase_ref() {++m_refcnt;}

		void decrease_ref();

		void mark_unshareable() {m_shareable = false;}

		bool is_shareable() const {return m_shareable;}

		bool is_shared() const {return m_refcnt > 1;}

		size_t count_ref() const {return m_refcnt;}

	private:
		virtual void destroy() const;

		virtual void deallocate() const = 0;

		ref_counter(const ref_counter & rhs); // forbidden

		ref_counter & operator = (const ref_counter & rhs); // forbidden

	private:
		size_t m_refcnt;
		size_t m_shareable:1;
	};
}

#endif
