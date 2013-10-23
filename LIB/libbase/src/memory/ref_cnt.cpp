#include <libbase/std.hpp>
#include <libbase/pvt/ref_cnt.hpp>

namespace Base {
	ref_counter::~ref_counter()
	{
	}

	void ref_counter::destroy() const
	{
	}
}
