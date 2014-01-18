#ifndef LIBSTL_FUNCTIONAL_HPP_
#define LIBSTL_FUNCTIONAL_HPP_

#include "impl_types.hpp"
#include "impl_rel_ops.hpp"

namespace simstd {
	template<typename T>
	struct plus
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a + b;
		}
	};

	template<typename T>
	struct minus
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a - b;
		}
	};

	template<typename T>
	struct multiplies
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a * b;
		}
	};

	template<typename T>
	struct divides
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a / b;
		}
	};

	template<typename T>
	struct modulus
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a % b;
		}
	};

	template<typename T>
	struct negate
	{
		typedef T result_type;
		typedef T argument_type;

		result_type operator ()(const argument_type& a) const
		{
			return -a;
		}
	};

	template<typename T>
	struct equal_to
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a == b;
		}
	};

	template<typename T>
	struct not_equal_to
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return simstd::rel_ops::operator !=(a, b);
		}
	};

	template<typename T>
	struct less
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a < b;
		}
	};

//	template<typename T1, typename T2 = T1>
//	struct less {
//		typedef bool result_type;
//		typedef T1 first_argument_type;
//		typedef T2 second_argument_type;
//
//		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
//		{
//			return a < b;
//		}
//	};
//
	template<typename T>
	struct less_equal
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return simstd::rel_ops::operator <=(a, b);
		}
	};

	template<typename T>
	struct greater
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return simstd::rel_ops::operator >(a, b);
		}
	};

	template<typename T>
	struct greater_equal
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return simstd::rel_ops::operator >=(a, b);
		}
	};

	template<typename T>
	struct logical_and
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a && b;
		}
	};

	template<typename T>
	struct logical_or
	{
		typedef bool result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a || b;
		}
	};

	template<typename T>
	struct logical_not
	{
		typedef bool result_type;
		typedef T argument_type;

		result_type operator ()(const argument_type& a) const
		{
			return !a;
		}
	};

	template<typename T>
	struct bit_and
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a & b;
		}
	};

	template<typename T>
	struct bit_or
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a | b;
		}
	};

	template<typename T>
	struct bit_xor
	{
		typedef T result_type;
		typedef T first_argument_type;
		typedef T second_argument_type;

		result_type operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return a ^ b;
		}
	};

	template<typename Predicate>
	class unary_negate
	{
	public:
		typedef bool result_type;
		typedef typename Predicate::argument_type argument_type;

		explicit unary_negate(const Predicate& a) :
			mPredicate(a)
		{
		}

		bool operator ()(const argument_type& a) const
		{
			return !mPredicate(a);
		}

	protected:
		Predicate mPredicate;
	};

	template<typename Predicate>
	unary_negate<Predicate> not1(const Predicate& predicate)
	{
		return unary_negate<Predicate>(predicate);
	}

	template<typename Predicate>
	class binary_negate
	{
	public:
		typedef bool result_type;
		typedef typename Predicate::first_argument_type first_argument_type;
		typedef typename Predicate::second_argument_type second_argument_type;

		explicit binary_negate(const Predicate& a) :
			mPredicate(a)
		{
		}

		bool operator ()(const first_argument_type& a, const second_argument_type& b) const
		{
			return !mPredicate(a, b);
		}

	protected:
		Predicate mPredicate;
	};

	template<typename Predicate>
	binary_negate<Predicate> not2(const Predicate& predicate)
	{
		return binary_negate<Predicate>(predicate);
	}

}

#endif
