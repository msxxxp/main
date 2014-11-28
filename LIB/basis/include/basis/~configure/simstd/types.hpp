#ifndef BASIS_CONFIGURE_SIMSTD_TYPES_HPP_
#define BASIS_CONFIGURE_SIMSTD_TYPES_HPP_

namespace simstd {

	typedef decltype(nullptr) nullptr_t;

	template<typename Type>
	Type* addressof(Type& ref) noexcept
	{
		return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(ref)));
	}

	namespace pvt {
		template<typename Type>
		struct remove_reference {
			typedef Type type;
		};

		template<typename Type>
		struct remove_reference<Type&> {
			typedef Type type;
		};

		template<typename Type>
		struct remove_reference<Type&&> {
			typedef Type type;
		};
	}

#if defined(__GNUC__)
	template<typename Type>
	constexpr Type&& forward(typename pvt::remove_reference<Type>::type& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	constexpr Type&& forward(typename pvt::remove_reference<Type>::type&& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	constexpr typename pvt::remove_reference<Type>::type&& move(Type&& val) noexcept
	{
		return static_cast<typename pvt::remove_reference<Type>::type&&>(val);
	}
#else
	template<typename Type>
	Type&& forward(typename pvt::remove_reference<Type>::type& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	Type&& forward(typename pvt::remove_reference<Type>::type&& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	typename pvt::remove_reference<Type>::type&& move(Type&& val) noexcept
	{
		return static_cast<typename pvt::remove_reference<Type>::type&&>(val);
	}
#endif

}

namespace simstd {
//#ifdef __x86_64__
//	typedef long long unsigned int size_t;
//	typedef long long int ssize_t;
//	typedef long long int ptrdiff_t;
//#else
//	typedef unsigned int size_t;
//	typedef int ssize_t;
//	typedef int ptrdiff_t;
//#endif

	namespace Test {
		typedef int (*aPrintFunc)(const char *, ...);
		typedef int (*wPrintFunc)(const wchar_t *, ...);
	}
}

#endif
