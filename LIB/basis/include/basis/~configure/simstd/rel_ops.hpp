#ifndef BASIS_CONFIGURE_SIMSTD_REL_OPS_HPP_
#define BASIS_CONFIGURE_SIMSTD_REL_OPS_HPP_

namespace simstd {

	namespace rel_ops {

		template<typename T1, typename T2>
		bool operator ==(const T1& x, const T2& y)
		{
			return !(x != y);
		}

		template<typename T1, typename T2>
		bool operator !=(const T1& x, const T2& y)
		{
			return !(x == y);
		}

		template<typename T1, typename T2>
		bool operator <(const T1& x, const T2& y)
		{
			return (y > x);
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
