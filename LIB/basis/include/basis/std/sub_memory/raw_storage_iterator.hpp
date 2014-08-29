#ifndef BASIS_MEMORY_RAW_STORAGE_ITERATOR_HPP_
#define BASIS_MEMORY_RAW_STORAGE_ITERATOR_HPP_

#include <basis/std/impl/iterator/base.hpp>

namespace simstd {

	template<typename OutputIterator, typename Type>
	class raw_storage_iterator: public iterator<output_iterator_tag, void, void, void, void> {
	public:
		explicit raw_storage_iterator(OutputIterator x);

		raw_storage_iterator& operator *();
		raw_storage_iterator& operator =(const Type& element);

		raw_storage_iterator& operator ++();
		raw_storage_iterator  operator ++(int);

	protected:
		OutputIterator _M_iter;
	};

	template<typename OutputIterator, typename Type>
	raw_storage_iterator<OutputIterator, Type>::raw_storage_iterator(OutputIterator x) :
		_M_iter(x)
	{
	}

	template<typename OutputIterator, typename Type>
	raw_storage_iterator<OutputIterator, Type>& raw_storage_iterator<OutputIterator, Type>::operator *()
	{
		return *this;
	}

	template<typename OutputIterator, typename Type>
	raw_storage_iterator<OutputIterator, Type>& raw_storage_iterator<OutputIterator, Type>::operator =(const Type& element)
	{
		simstd::pvt::_construct(simstd::addressof(*_M_iter), element);
		return *this;
	}

	template<typename OutputIterator, typename Type>
	raw_storage_iterator<OutputIterator, Type>& raw_storage_iterator<OutputIterator, Type>::operator ++()
	{
		++_M_iter;
		return *this;
	}

	template<typename OutputIterator, typename Type>
	raw_storage_iterator<OutputIterator, Type> raw_storage_iterator<OutputIterator, Type>::operator ++(int)
	{
		return raw_storage_iterator(_M_iter++);
	}

}

#endif
