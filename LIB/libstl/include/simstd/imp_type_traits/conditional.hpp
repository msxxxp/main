#ifndef LIBSTL_TYPE_TRAITS_CONDITIONAL_HPP_
#define LIBSTL_TYPE_TRAITS_CONDITIONAL_HPP_

namespace simstd {

	template<bool Condition, typename IfTrue, typename IfFalse>
	struct conditional
	{
		typedef IfTrue type;
	};

	template<typename IfTrue, typename IfFalse>
	struct conditional<false, IfTrue, IfFalse>
	{
		typedef IfFalse type;
	};

	template<typename ...>
	struct __or_;

	template<>
	struct __or_<> : public false_type
	{
	};

	template<typename _B1>
	struct __or_<_B1> : public _B1
	{
	};

	template<typename _B1, typename _B2>
	struct __or_<_B1, _B2> : public conditional<_B1::value, _B1, _B2>::type
	{
	};

	template<typename _B1, typename _B2, typename _B3, typename ... _Bn>
	struct __or_<_B1, _B2, _B3, _Bn...> : public conditional<_B1::value, _B1, __or_ <_B2, _B3, _Bn...>>::type
	{
	};

	template<typename ...>
	struct __and_;

	template<>
	struct __and_<> : public true_type
	{
	};

	template<typename _B1>
	struct __and_<_B1> : public _B1
	{
	};

	template<typename _B1, typename _B2>
	struct __and_<_B1, _B2> : public conditional<_B1::value, _B2, _B1>::type
	{
	};

	template<typename _B1, typename _B2, typename _B3, typename ... _Bn>
	struct __and_<_B1, _B2, _B3, _Bn...> : public conditional<_B1::value, __and_ <_B2, _B3, _Bn...>, _B1>::type
	{
	};

	template<typename _Pp>
	struct __not_: public integral_constant<bool, !_Pp::value>
	{
	};

}

#endif
