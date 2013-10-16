#ifndef _LIBBASE_REF_CNT_HPP_
#define _LIBBASE_REF_CNT_HPP_

#include <libbase/std.hpp>

namespace Base {

	struct ref_counter {
		virtual ~ref_counter();

		ref_counter();

		void increase_ref();

		void decrease_ref();

//		void mark_unshareable() {m_shareable = false;}

//		bool is_shareable() const {return m_shareable;}

		bool is_shared() const;

		size_t count_ref() const;

	private:
		virtual void destroy() const;

		virtual void deallocate() const = 0;

		ref_counter(const ref_counter & rhs) = delete; // forbidden

		ref_counter & operator = (const ref_counter & rhs) = delete; // forbidden

	private:
		size_t m_refcnt;
//		size_t m_shareable:1;
	};

	inline ref_counter::~ref_counter()
	{
	}

	inline ref_counter::ref_counter() :
		m_refcnt(1)
//		, m_shareable(true)
	{
	}

//	void ref_counter::free()
//	{
//		delete this;
//	}

//	ref_counter::ref_counter(const ref_counter &/*rhs*/) :
//		m_refcnt(0), m_shareable(true)
//	{
//	}
//
//	ref_counter& ref_counter::operator=(const ref_counter &/*rhs*/)
//	{
//		return *this;
//	}

	inline void ref_counter::increase_ref()
	{
		++m_refcnt;
	}

	inline void ref_counter::decrease_ref()
	{
		if (--m_refcnt == 0) {
			destroy();
			deallocate();
		}
	}

	inline bool ref_counter::is_shared() const
	{
		return m_refcnt > 1;
	}

	inline size_t ref_counter::count_ref() const
	{
		return m_refcnt;
	}

	inline void ref_counter::destroy() const
	{
	}

}

#endif
