#ifndef SARALIB_STL_ITERATOR_HPP_
#define SARALIB_STL_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_utility.hpp"

namespace sarastd
{
	struct input_iterator_tag
	{
	};

	struct output_iterator_tag
	{
	};

	struct forward_iterator_tag : public input_iterator_tag
	{
	};


	struct bidirectional_iterator_tag : public forward_iterator_tag
	{
	};


	struct random_access_iterator_tag : public bidirectional_iterator_tag
	{
	};

	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
	};

	template <typename Type>
	struct iterator_traits<Type*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef Type                       value_type;
		typedef sarastd::ptrdiff_t         difference_type;
		typedef Type*                      pointer;
		typedef Type&                      reference;
	};

	template <typename Type>
	struct iterator_traits<const Type*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef Type                       value_type;
		typedef sarastd::ptrdiff_t         difference_type;
		typedef const Type*                pointer;
		typedef const Type&                reference;
	};

	namespace pvt {
		template<typename Iterator>
		typename iterator_traits<Iterator>::iterator_category _iterator_category(const Iterator &)
		{
			return typename iterator_traits<Iterator>::iterator_category();
		}
	}

	///=============================================================================================
	namespace pvt {
		template<typename InputIterator>
		typename iterator_traits<InputIterator>::difference_type _distance(InputIterator first, InputIterator last, input_iterator_tag)
		{
			typename iterator_traits<InputIterator>::difference_type n = 0;
			while (first != last)
			{
				++first;
				++n;
			}
			return n;
		}

		template<typename RandomIterator>
		typename iterator_traits<RandomIterator>::difference_type _distance(RandomIterator first, RandomIterator last, random_access_iterator_tag)
		{
			return last - first;
		}
	}

	template<typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
	{
		return sarastd::pvt::_distance(first, last, sarastd::pvt::_iterator_category(first));
	}

	namespace pvt {
		template<typename InputIterator, typename Distance>
		void _advance(InputIterator& current, Distance n, input_iterator_tag)
		{
			while (n--)
				++current;
		}

		template<typename BidirIterator, typename Distance>
		void _advance(BidirIterator & current, Distance n, bidirectional_iterator_tag)
		{
			if (n > 0)
				while (n--)
					++current;
			else
				while (n++)
					--current;
		}

		template<typename RandomIterator, typename Distance>
		void _advance(RandomIterator & current, Distance n, random_access_iterator_tag)
		{
			current += n;
		}
	}

	template<typename InputIterator, typename Distance>
	void advance(InputIterator & current, Distance n)
	{
		typename iterator_traits<InputIterator>::difference_type d = n;
		sarastd::pvt::_advance(current, d, sarastd::pvt::_iterator_category(current));
	}

	template<typename ForwardIterator>
	ForwardIterator next(ForwardIterator current, typename iterator_traits<ForwardIterator>::difference_type n = 1)
	{
		advance(current, n);
		return current;
	}

	template<typename BidirIterator>
	BidirIterator prev(BidirIterator current, typename iterator_traits<BidirIterator>::difference_type n = 1)
	{
		advance(current, -n);
		return current;
	}

	///=============================================================================================
	template<
	typename Category,
	typename Type,
	typename Distance = sarastd::ptrdiff_t,
	typename Pointer = Type*,
	typename Reference = Type&>
	struct iterator
	{
		typedef Category  iterator_category;
		typedef Type      value_type;
		typedef Distance  difference_type;
		typedef Pointer   pointer;
		typedef Reference reference;
	};

	///=============================================================================================
	namespace pvt {
		template<typename Iterator>
		class _normal_iterator
		{
			typedef Iterator                                iterator_type;
			typedef _normal_iterator<Iterator>               this_type;
			typedef iterator_traits<Iterator>               traits_type;

		public:
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type        value_type;
			typedef typename traits_type::difference_type   difference_type;
			typedef typename traits_type::pointer           pointer;
			typedef typename traits_type::reference         reference;

			_normal_iterator() : current() {}
			explicit _normal_iterator(Iterator it) : current(it) {}
			_normal_iterator(const this_type& ri) : current(ri.current) {}

			template <typename U>
			_normal_iterator(const _normal_iterator<U>& ri) : current(ri.base()) {}

			template <typename U>
			_normal_iterator<Iterator>& operator =(const _normal_iterator<U>& ri) {current = ri.base(); return *this;}

			iterator_type base() const     {return current;}

			// Forward iterator requirements
			reference  operator * () const {return *current;}
			pointer    operator ->() const {return current;}
			this_type& operator ++()       {++current; return *this;}
			this_type  operator ++(int)    {return this_type(current++);}

			// Bidirectional iterator requirements
			this_type& operator --()       {--current; return *this;}
			this_type  operator --(int)    {return this_type(current--);}

			// Random access iterator requirements
			reference  operator [](const difference_type& n) const {return current[n];}
			this_type& operator +=(const difference_type& n)       {current += n; return *this;}
			this_type  operator +(const difference_type& n) const  {return this_type(current + n);}
			this_type& operator -=(const difference_type& n)       {current -= n; return *this;}
			this_type  operator -(const difference_type& n) const  {return this_type(current - n);}

		protected:
			iterator_type current;
		};

		template<typename Iterator1, typename Iterator2>
		bool operator ==(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template<typename Iterator1, typename Iterator2>
		bool operator !=(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::operator !=(lhs, rhs);
		}

		template<typename Iterator1, typename Iterator2>
		bool operator <(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return lhs.base() < rhs.base();
		}

		template<typename Iterator1, typename Iterator2>
		bool operator <=(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::operator <=(lhs, rhs);
		}

		template<typename Iterator1, typename Iterator2>
		bool operator >(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::operator >(lhs, rhs);
		}

		template<typename Iterator1, typename Iterator2>
		bool operator >=(const _normal_iterator<Iterator1>& lhs, const _normal_iterator<Iterator2>& rhs)
		{
			return sarastd::pvt::operator >=(lhs, rhs);
		}

		template<typename Iterator>
		_normal_iterator<Iterator> operator +(typename _normal_iterator<Iterator>::difference_type n, const _normal_iterator<Iterator> & it)
		{
			return it + n;
		}

		template<typename Iterator>
		typename
		_normal_iterator<Iterator>::difference_type operator -(const _normal_iterator<Iterator>& lhs, const _normal_iterator<Iterator>& rhs)
		{
			return lhs.base() - rhs.base();
		}
	}

	///=============================================================================================
	template<typename Iterator>
	class reverse_iterator
	{
		typedef Iterator                                iterator_type;
		typedef reverse_iterator<Iterator>              this_type;
		typedef iterator_traits<Iterator>               traits_type;

	public:
		typedef typename traits_type::iterator_category iterator_category;
		typedef typename traits_type::value_type        value_type;
		typedef typename traits_type::difference_type   difference_type;
		typedef typename traits_type::reference         reference;
		typedef typename traits_type::pointer           pointer;

		reverse_iterator() : current() {}
		explicit reverse_iterator(Iterator it) : current(it) {}
		reverse_iterator(const this_type& ri) : current(ri.current) {}

		template <typename U>
		reverse_iterator(const reverse_iterator<U>& ri) : current(ri.base()) {}

		template <typename U>
		reverse_iterator<Iterator>& operator =(const reverse_iterator<U>& ri) {current = ri.base(); return *this;}

		iterator_type base() const                         {return current;}

		// Forward iterator requirements
		reference     operator * () const                  {iterator_type tmp = current; return *--tmp;}
		pointer       operator ->() const                  {return &(operator*());}
		this_type&    operator ++()                        {--current; return *this;}
		this_type     operator ++(int)                     {this_type tmp = *this; --current; return tmp;}

		// Bidirectional iterator requirements
		this_type&    operator --()                        {++current; return *this;}
		this_type     operator --(int)                     {this_type tmp = *this; ++current; return tmp;}

		// Random access iterator requirements
		reference     operator [](difference_type n) const {return *(*this + n);}
		this_type     operator +(difference_type n) const  {return this_type(current - n);}
		this_type&    operator +=(difference_type n)       {current -= n; return *this;}
		this_type     operator -(difference_type n) const  {return this_type(current + n);}
		this_type&    operator -=(difference_type n)       {current += n; return *this;}

	protected:
		iterator_type current;
	};

	template<typename Iterator1, typename Iterator2>
	bool operator ==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator !=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::operator !=(lhs, rhs);
	}

	template<typename Iterator1, typename Iterator2>
	bool operator <(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator <=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::operator <=(lhs, rhs);
	}

	template<typename Iterator1, typename Iterator2>
	bool operator >(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::operator >(lhs, rhs);
	}

	template<typename Iterator1, typename Iterator2>
	bool operator >=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::operator >=(lhs, rhs);
	}

	template<typename Iterator>
	reverse_iterator<Iterator> operator +(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> & it)
	{
		return it + n;
	}

	template<typename Iterator>
	typename
	reverse_iterator<Iterator>::difference_type operator -(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}

	///=============================================================================================
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

	///=============================================================================================
	namespace pvt {
		template<typename Iterator>
		struct _const_value_iterator : public iterator<random_access_iterator_tag, void, void, void, void>
		{
			typedef Iterator                                iterator_type;
			typedef _const_value_iterator<Iterator>         this_type;
			typedef iterator_traits<Iterator>               traits_type;
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type        value_type;
			typedef typename traits_type::difference_type   difference_type;
			typedef typename traits_type::reference         reference;
			typedef typename traits_type::pointer           pointer;
			typedef sarastd::size_t size_type;

			_const_value_iterator(iterator_type v, iterator_type it): value(v), current(it) {}

			iterator_type base() const        {return current;}

			reference     operator * () const {return *value;}
			pointer       operator ->() const {return value;}
			this_type&    operator ++()       {++current; return *this;}
			this_type     operator ++(int)    {this_type tmp = *this; ++current; return tmp;}

			// Bidirectional iterator requirements
			this_type&    operator --()       {--current; return *this;}
			this_type     operator --(int)    {this_type tmp = *this; --current; return tmp;}

			// Random access iterator requirements
			reference     operator [](difference_type n) const {return *value;}
			this_type     operator +(difference_type n) const  {return this_type(value, current + n);}
			this_type&    operator +=(difference_type n)       {current += n; return *this;}
			this_type     operator -(difference_type n) const  {return this_type(value, current - n);}
			this_type&    operator -=(difference_type n)       {current -= n; return *this;}

		private:
			iterator_type value;
			iterator_type current;
		};

		template<typename Iterator>
		bool operator ==(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template<typename Iterator>
		bool operator !=(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return sarastd::pvt::operator !=(lhs, rhs);
		}

		template<typename Iterator>
		bool operator <(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return lhs.base() < rhs.base();
		}

		template<typename Iterator>
		bool operator <=(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return sarastd::pvt::operator <=(lhs, rhs);
		}

		template<typename Iterator>
		bool operator >(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return sarastd::pvt::operator >(lhs, rhs);
		}

		template<typename Iterator>
		bool operator >=(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return sarastd::pvt::operator >=(lhs, rhs);
		}

		template<typename Iterator>
		_const_value_iterator<Iterator> operator +(typename _const_value_iterator<Iterator>::difference_type n, const _const_value_iterator<Iterator> & it)
		{
			return it + n;
		}

		template<typename Iterator>
		typename
		_const_value_iterator<Iterator>::difference_type operator -(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return lhs.base() - rhs.base();
		}

		template<typename Type>
		class value_generator
		{
			typedef value_generator<Type> this_type;
			typedef Type value_type;
			typedef sarastd::size_t size_type;
			typedef const Type* const_pointer;
			typedef _const_value_iterator<const_pointer> const_iterator;

		public:
			explicit value_generator(size_type n, const value_type & v) : value(v), begin_(0), end_(begin_ + n) {}
			value_generator(const this_type & other) : value(other.value), begin_(other.begin_), end_(other.end_) {}

			const_iterator begin() const {return const_iterator(&value, begin_);}
			const_iterator end() const {return const_iterator(&value, end_);}

		private:
			value_type value;
			const_pointer begin_;
			const_pointer end_;
		};
	}

	///=============================================================================================
	template<typename Container>
	insert_iterator<Container> inserter(Container & c, typename Container::iterator current)
	{
		return insert_iterator<Container>(c, current);
	}

	template<typename Container>
	front_insert_iterator<Container> front_inserter(Container & c)
	{
		return front_insert_iterator<Container>(c);
	}

	template<typename Container>
	back_insert_iterator<Container> back_inserter(Container & c)
	{
		return back_insert_iterator<Container>(c);
	}

	///=============================================================================================
	template<typename Container>
	typename
	Container::iterator begin(Container & c) {return c.begin();}

	template<typename Container>
	typename
	Container::const_iterator begin(const Container & c) {return c.begin();}

	template<typename Type, sarastd::size_t N>
	Type* begin(Type (&array)[N]) {return array;}

	template<typename Type, sarastd::size_t N>
	const Type* cbegin(Type (&array)[N]) {return array;}

	template<typename Container>
	typename
	Container::const_iterator cbegin(const Container & c) {begin(c);}

	template<typename Container>
	typename
	Container::iterator end(Container & c) {return c.end();}

	template<typename Container>
	typename
	Container::const_iterator end(const Container & c) {return c.end();}

	template<typename Type, sarastd::size_t N>
	Type* end(Type (&array)[N]) {return array + N;}

	template<typename Type, sarastd::size_t N>
	const Type* cend(Type (&array)[N]) {return array + N;}

	template<typename Container>
	typename
	Container::const_iterator cend(const Container & c) {end(c);}

	template<typename Container>
	typename
	Container::reverse_iterator rbegin(Container & c) {return c.rbegin();}

	template<typename Container>
	typename
	Container::const_reverse_iterator rbegin(const Container & c) {return c.rbegin();}

	template<typename Type, sarastd::size_t N>
	reverse_iterator<Type*> rbegin(Type (&array)[N]) {return reverse_iterator<Type*>(array + N);}

	template<typename Type, sarastd::size_t N>
	reverse_iterator<const Type*> crbegin(Type (&array)[N]) {return reverse_iterator<const Type*>(array + N);}

	template<typename Container>
	typename
	Container::const_reverse_iterator crbegin(const Container & c) {rbegin(c);}

	template<typename Container>
	typename
	Container::reverse_iterator rend(Container & c) {return c.rend();}

	template<typename Container>
	typename
	Container::const_reverse_iterator rend(const Container & c) {return c.rend();}

	template<typename Type, sarastd::size_t N>
	reverse_iterator<Type*> rend(Type (&array)[N]) {return reverse_iterator<Type*>(array);}

	template<typename Type, sarastd::size_t N>
	reverse_iterator<const Type*> crend(Type (&array)[N]) {return reverse_iterator<const Type*>(array);}

	template<typename Container>
	typename
	Container::const_reverse_iterator crend(const Container & c) {rend(c);}
}

#endif
