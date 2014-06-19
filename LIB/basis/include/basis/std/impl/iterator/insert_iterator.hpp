#ifndef BASIS_ITERATOR_INSERT_ITERATOR_HPP_
#define BASIS_ITERATOR_INSERT_ITERATOR_HPP_

#include <basis/std/impl/iterator/base.hpp>

namespace simstd
{
	template<typename Container>
	class insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
	{
		typedef insert_iterator<Container> this_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::const_iterator iterator;
	public:
		typedef Container container_type;

		insert_iterator(container_type & c, iterator current) : container(&c), iter(current) {}
		iterator    base() const {return iter;}
		this_type & operator =(const value_type & value) {iter = container->insert(iter, value); ++iter; return *this;}
		this_type & operator *()    {return *this;}
		this_type & operator++()    {return *this;}
		this_type & operator++(int) {return *this;}

	protected:
		container_type * container;
		iterator iter;
	};

	template<typename Container>
	insert_iterator<Container> inserter(Container & c, typename Container::iterator current)
	{
		return insert_iterator<Container>(c, current);
	}
}

#endif
