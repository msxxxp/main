#include <libbase/std.hpp>
#include <libbase/ref_cnt.hpp>

namespace Base {

	ref_counter::~ref_counter()
	{
	}

	ref_counter::ref_counter() :
		m_refcnt(1),
		m_shareable(true)
	{
	}

	void ref_counter::destroy()
	{
	}

	void ref_counter::free()
	{
		delete this;
	}

	//	ref_counter::ref_counter(const ref_counter &/*rhs*/) :
//		m_refcnt(0), m_shareable(true)
//	{
//	}
//
//	ref_counter& ref_counter::operator=(const ref_counter &/*rhs*/)
//	{
//		return *this;
//	}

	void ref_counter::decrease_ref()
	{
		if (--m_refcnt == 0) {
			destroy();
			free();
		}
	}

}
