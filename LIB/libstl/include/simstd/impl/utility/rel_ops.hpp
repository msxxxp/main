#ifndef LIBSTL_GENERIC_HPP_
#define LIBSTL_GENERIC_HPP_

#include <simstd/types.hpp>

namespace simstd {
	namespace rel_ops {
		template<typename T1, typename T2>
		bool operator !=(const T1& x, const T2& y)
		{
			return !(x == y);
		}

		template<typename T1, typename T2>
		bool operator >(const T1& x, const T2& y)
		{
			return (y < x);
		}

		template<typename T1, typename T2>
		bool operator <=(const T1& x, const T2& y)
		{
			return !(y < x);
		}

		template<typename T1, typename T2>
		bool operator >=(const T1& x, const T2& y)
		{
			return !(x < y);
		}
	}
}

#endif
