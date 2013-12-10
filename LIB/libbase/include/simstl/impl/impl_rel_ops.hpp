#ifndef SARALIB_STL_GENERIC_HPP_
#define SARALIB_STL_GENERIC_HPP_

#include "impl_types.hpp"

namespace sarastd {
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
