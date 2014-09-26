#ifndef BASIS_TYPE_TRAITS_REMOVE_CV_HPP_
#define BASIS_TYPE_TRAITS_REMOVE_CV_HPP_

namespace simstd {

	template<typename Type>
	struct remove_const
	{
		typedef Type type;
	};

	template<typename Type>
	struct remove_const<Type const>
	{
		typedef Type type;
	};

	template<typename Type>
	struct remove_volatile
	{
		typedef Type type;
	};

	template<typename Type>
	struct remove_volatile<Type volatile>
	{
		typedef Type type;
	};

	template<typename Type>
	struct remove_cv
	{
		typedef typename remove_const<typename remove_volatile<Type>::type>::type type;
	};

}

#endif
