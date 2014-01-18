#include <simstl/impl/utility/ref_counter.hpp>

namespace simstd {
	namespace pvt {

		ref_counter::~ref_counter()
		{
		}

		void ref_counter::decrease_ref()
		{
			if (--m_refcnt == 0) {
				destroy();
				deallocate();
			}
		}

		void ref_counter::destroy() const
		{
		}

	}
}
