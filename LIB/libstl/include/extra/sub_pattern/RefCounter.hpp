#ifndef LIBSTL_EXTRA_PATTERN_REFCOUNTER_HPP_
#define LIBSTL_EXTRA_PATTERN_REFCOUNTER_HPP_

#include <simstl/types.hpp>

namespace pattern {

	struct RefCounter
	{
		virtual ~RefCounter();

		RefCounter();

		void increase_ref();

		void decrease_ref();

		bool is_shared() const;

		size_t count_ref() const;

	private:
		virtual void destroy() const;

		virtual void deallocate() const = 0;

		RefCounter(const RefCounter & rhs);                 // forbidden

		RefCounter & operator =(const RefCounter & rhs);    // forbidden

	private:
		size_t m_refcnt;
	};

	inline RefCounter::RefCounter() :
		m_refcnt(1)
	{
	}

	inline void RefCounter::increase_ref()
	{
		++m_refcnt;
	}

	inline bool RefCounter::is_shared() const
	{
		return m_refcnt > 1;
	}

	inline size_t RefCounter::count_ref() const
	{
		return m_refcnt;
	}

}

#endif
