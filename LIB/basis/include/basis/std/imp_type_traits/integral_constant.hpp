#ifndef BASIS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP_
#define BASIS_TYPE_TRAITS_INTEGRAL_CONSTANT_HPP_

namespace simstd {

	template<typename Type, Type Val>
	struct integral_constant
	{
		static constexpr Type value = Val;
		typedef Type value_type;
		typedef integral_constant<Type, Val> type;
		constexpr operator value_type()
		{
			return value;
		}
	};

	template<typename Type, Type Val>
	constexpr Type integral_constant<Type, Val>::value;

	typedef integral_constant<bool, true> true_type;

	typedef integral_constant<bool, false> false_type;

}

#endif
