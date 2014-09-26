#ifndef BASIS_TYPE_TRAITS_IS_TYPE_HPP_
#define BASIS_TYPE_TRAITS_IS_TYPE_HPP_

#include <basis/simstd/type_traits>

namespace simstd {

	namespace pvt {
		template<typename >
		struct is_void: public false_type
		{
		};

		template<>
		struct is_void<void> : public true_type
		{
		};

		template<typename >
		struct is_integral: public false_type
		{
		};

		template<>
		struct is_integral<bool> : public true_type
		{
		};

		template<>
		struct is_integral<char> : public true_type
		{
		};

		template<>
		struct is_integral<signed char> : public true_type
		{
		};

		template<>
		struct is_integral<unsigned char> : public true_type
		{
		};

		template<>
		struct is_integral<wchar_t> : public true_type
		{
		};

		template<>
		struct is_integral<char16_t> : public true_type
		{
		};

		template<>
		struct is_integral<char32_t> : public true_type
		{
		};

		template<>
		struct is_integral<short> : public true_type
		{
		};

		template<>
		struct is_integral<unsigned short> : public true_type
		{
		};

		template<>
		struct is_integral<int> : public true_type
		{
		};

		template<>
		struct is_integral<unsigned int> : public true_type
		{
		};

		template<>
		struct is_integral<long> : public true_type
		{
		};

		template<>
		struct is_integral<unsigned long> : public true_type
		{
		};

		template<>
		struct is_integral<long long> : public true_type
		{
		};

		template<>
		struct is_integral<unsigned long long> : public true_type
		{
		};

//		template<>
//		struct is_integral<__int128> : public true_type
//		{
//		};
//
//		template<>
//		struct is_integral<unsigned __int128> : public true_type
//		{
//		};

		template<typename >
		struct is_floating_point: public false_type
		{
		};

		template<>
		struct is_floating_point<float> : public true_type
		{
		};

		template<>
		struct is_floating_point<double> : public true_type
		{
		};

		template<>
		struct is_floating_point<long double> : public true_type
		{
		};

		template<>
		struct is_floating_point<__float128> : public true_type
		{
		};

		template<typename >
		struct is_pointer: public false_type
		{
		};

		template<typename Type>
		struct is_pointer<Type*> : public true_type
		{
		};

	}

	template<typename >
	struct is_const: public false_type
	{
	};

	template<typename Type>
	struct is_const<Type const> : public true_type
	{
	};

	template<typename >
	struct is_volatile: public false_type
	{
	};

	template<typename Type>
	struct is_volatile<Type volatile> : public true_type
	{
	};

	template<typename Type>
	struct is_void: public integral_constant<bool, pvt::is_void<typename remove_cv<Type>::type>::value>
	{
	};

	template<typename Type>
	struct is_integral: public integral_constant<bool, pvt::is_integral<typename remove_cv<Type>::type>::value>
	{
	};

	template<typename Type>
	struct is_floating_point: public integral_constant<bool, pvt::is_floating_point<typename remove_cv<Type>::type>::value>
	{
	};

	template<typename >
	struct is_array: public false_type
	{
	};

	template<typename Type, size_t Size>
	struct is_array<Type[Size]> : public true_type
	{
	};

	template<typename Type>
	struct is_array<Type[]> : public true_type
	{
	};

	template<typename Type>
	struct is_pointer: public integral_constant<bool, pvt::is_pointer<typename remove_cv<Type>::type>::value>
	{
	};

	template<typename >
	struct is_lvalue_reference: public false_type
	{
	};

	template<typename Type>
	struct is_lvalue_reference<Type&> : public true_type
	{
	};

	template<typename >
	struct is_rvalue_reference: public false_type
	{
	};

	template<typename Type>
	struct is_rvalue_reference<Type&&> : public true_type
	{
	};

	template<typename >
	struct is_function: public false_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes...)> : public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes......)> : public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes...) const> : public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes......) const>: public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes...) volatile> : public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes......) volatile>: public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes...) const volatile> : public true_type
	{
	};

	template<typename _Res, typename ... _ArgTypes>
	struct is_function<_Res(_ArgTypes......) const volatile>: public true_type
	{
	};

}

#endif
