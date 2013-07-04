#ifndef SARALIB_STL_ITERATOR_FRONT_INSERT_ITERATOR_HPP_
#define SARALIB_STL_ITERATOR_FRONT_INSERT_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"

namespace sarastd
{
	template<typename Container>
	class front_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
	{
		typedef front_insert_iterator<Container> this_type;
		typedef typename Container::value_type value_type;

	public:
		typedef Container container_type;

		explicit front_insert_iterator(container_type & c) : container(&c) {}
		this_type & operator =(const value_type & value) {container->push_front(value); return *this;}
		this_type & operator *()    {return *this;}
		this_type & operator++()    {return *this;}
		this_type & operator++(int) {return *this;}

	protected:
		container_type * container;
	};

	template<typename Container>
	front_insert_iterator<Container> front_inserter(Container & c)
	{
		return front_insert_iterator<Container>(c);
	}
}

#endif
