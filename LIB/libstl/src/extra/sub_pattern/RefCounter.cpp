#include <extra/pattern.hpp>

namespace pattern {
	RefCounter::~RefCounter()
	{
	}

	void RefCounter::decrease_ref()
	{
		if (--m_refcnt == 0) {
			destroy();
			deallocate();
		}
	}

	void RefCounter::destroy() const
	{
	}
}
