#ifndef LIBSTL_ITERATOR_FUNCTIONAL_HPP_
#define LIBSTL_ITERATOR_FUNCTIONAL_HPP_

#include <simstl/impl/iterator/reverse_iterator.hpp>
#include <simstl/impl/types.hpp>

namespace simstd {
	template<typename Container>
	typename Container::iterator begin(Container & c)
	{
		return c.begin();
	}

	template<typename Container>
	typename Container::const_iterator begin(const Container & c)
	{
		return c.begin();
	}

	template<typename Type, simstd::size_t N>
	Type* begin(Type (&array)[N])
	{
		return array;
	}

	template<typename Type, simstd::size_t N>
	const Type* cbegin(Type (&array)[N])
	{
		return array;
	}

	template<typename Container>
	typename Container::const_iterator cbegin(const Container & c)
	{
		begin(c);
	}

	template<typename Container>
	typename Container::iterator end(Container & c)
	{
		return c.end();
	}

	template<typename Container>
	typename Container::const_iterator end(const Container & c)
	{
		return c.end();
	}

	template<typename Type, simstd::size_t N>
	Type* end(Type (&array)[N])
	{
		return array + N;
	}

	template<typename Type, simstd::size_t N>
	const Type* cend(Type (&array)[N])
	{
		return array + N;
	}

	template<typename Container>
	typename Container::const_iterator cend(const Container & c)
	{
		end(c);
	}

	///=============================================================================================
	template<typename Container>
	typename Container::reverse_iterator rbegin(Container & c)
	{
		return c.rbegin();
	}

	template<typename Container>
	typename Container::const_reverse_iterator rbegin(const Container & c)
	{
		return c.rbegin();
	}

	template<typename Type, simstd::size_t N>
	reverse_iterator<Type*> rbegin(Type (&array)[N])
	{
		return reverse_iterator<Type*>(array + N);
	}

	template<typename Type, simstd::size_t N>
	reverse_iterator<const Type*> crbegin(Type (&array)[N])
	{
		return reverse_iterator<const Type*>(array + N);
	}

	template<typename Container>
	typename Container::const_reverse_iterator crbegin(const Container & c)
	{
		rbegin(c);
	}

	template<typename Container>
	typename Container::reverse_iterator rend(Container & c)
	{
		return c.rend();
	}

	template<typename Container>
	typename Container::const_reverse_iterator rend(const Container & c)
	{
		return c.rend();
	}

	template<typename Type, simstd::size_t N>
	reverse_iterator<Type*> rend(Type (&array)[N])
	{
		return reverse_iterator<Type*>(array);
	}

	template<typename Type, simstd::size_t N>
	reverse_iterator<const Type*> crend(Type (&array)[N])
	{
		return reverse_iterator<const Type*>(array);
	}

	template<typename Container>
	typename Container::const_reverse_iterator crend(const Container & c)
	{
		rend(c);
	}
}

#endif
