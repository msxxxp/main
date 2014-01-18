#ifndef LIBSTL_ITERATOR_BACK_INSERT_ITERATOR_HPP_
#define LIBSTL_ITERATOR_BACK_INSERT_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"

namespace simstd
{
	template<typename Container>
	class back_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
	{
		typedef back_insert_iterator<Container> this_type;
		typedef typename Container::value_type value_type;

	public:
		typedef Container container_type;

		explicit back_insert_iterator(container_type & c) : container(&c) {}
		this_type & operator =(const value_type & value) {container->push_back(value); return *this;}
		this_type & operator *()    {return *this;}
		this_type & operator++()    {return *this;}
		this_type & operator++(int) {return *this;}

	protected:
		container_type * container;
	};

	template<typename Container>
	back_insert_iterator<Container> back_inserter(Container & c)
	{
		return back_insert_iterator<Container>(c);
	}
}

#endif
