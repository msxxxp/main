#ifndef SARALIB_STL_MEMORY_HPP_
#define SARALIB_STL_MEMORY_HPP_

#include "impl_types.hpp"
#include "impl_iterator.hpp"
#include "impl_new.hpp"
#include "impl_utility.hpp"

namespace sarastd {

	namespace pvt {
		template<typename Type>
		Type* _addressof(Type& r)
		{
			return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(r)));
		}

		template<typename Type1, typename Type2>
		void _construct(Type1* p, const Type2& value)
		{
			::new (static_cast<void*>(p)) Type1(value);
		}

		template<typename Type>
		void _destroy(Type* pointer)
		{
			pointer->~Type();
		}

		template<typename ForwardIterator, typename Type>
		void _destroy(ForwardIterator first, ForwardIterator last)
		{
			for (; first != last; ++first)
				first->~Type();
		}
	}

//	template<bool>
//	struct _Destroy_aux {
//		template<typename ForwardIterator>
//		static void _destroy(ForwardIterator first, ForwardIterator last)
//		{
//			for (; first != last; ++first)
//				sarastd::_destroy(sarastd::_addressof(*first));
//		}
//	};
//
//	template<>
//	struct _Destroy_aux<true> {
//		template<typename ForwardIterator>
//		static void _destroy(ForwardIterator, ForwardIterator)
//		{
//		}
//	};
//
//	template<typename ForwardIterator>
//	void _destroy(ForwardIterator first, ForwardIterator last)
//	{
//		typedef typename iterator_traits<ForwardIterator>::value_type Value_type;
//		sarastd::_Destroy_aux<__has_trivial_destructor(Value_type)>::_destroy(first, last);
//	}
//
//	template<typename ForwardIterator, typename Allocator>
//	void _destroy(ForwardIterator first, ForwardIterator last, Allocator& alloc)
//	{
//		for (; first != last; ++first)
//			alloc.destroy(sarastd::_addressof(*first));
//	}

//	template<typename ForwardIterator, typename Type>
//	void _destroy(ForwardIterator first, ForwardIterator last, allocator<Type>&)
//	{
//		_destroy(first, last);
//	}

	template<typename Type>
	sarastd::pair<Type*, sarastd::ptrdiff_t> get_temporary_buffer(sarastd::ptrdiff_t count)
	{
		Type* ptr = static_cast<Type*>(::operator new(count * sizeof(Type), sarastd::nothrow));
		return sarastd::pair<Type*, ptrdiff_t>(static_cast<Type*>(ptr), ptr ? count : 0);
	}

	template<typename Type>
	void return_temporary_buffer(Type* p)
	{
		::operator delete(p, sarastd::nothrow);
	}

	///=============================================================================================
	template<typename Allocator>
	struct allocator_traits {
		typedef Allocator allocator_type;
		typedef typename Allocator::value_type value_type;
		typedef typename Allocator::size_type size_type;
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;
		typedef void* void_pointer;
		typedef const void* const_void_pointer;
	};

	///=============================================================================================
	template<typename Type>
	struct allocator {
		typedef Type value_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef sarastd::size_t size_type;
		typedef sarastd::ptrdiff_t difference_type;

		~allocator();
		allocator();

		pointer allocate(size_type n);
		void deallocate(pointer p, size_type /*n*/);

		void construct(pointer p, const_reference val);
		void destroy(pointer p);
	};

	template<typename Type>
	allocator<Type>::~allocator()
	{
	}

	template<typename Type>
	allocator<Type>::allocator()
	{
	}

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::allocate(size_type n)
	{
		return (pointer)::operator new(n * sizeof(Type), sarastd::nothrow);
	}

	template<typename Type>
	void allocator<Type>::deallocate(pointer p, size_type /*n*/)
	{
		::operator delete(p, sarastd::nothrow);
	}

	template<typename Type>
	void allocator<Type>::construct(pointer p, const_reference val)
	{
		sarastd::pvt::_construct(p, val);
	}

	template<typename Type>
	void allocator<Type>::destroy(pointer p)
	{
		sarastd::pvt::_destroy(p);
	}

	///=============================================================================================
	namespace pvt {
		template<typename Type>
		struct _movable_allocator {
			typedef Type value_type;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef sarastd::size_t size_type;
			typedef sarastd::ptrdiff_t difference_type;
			typedef void* movable_handle;

			~_movable_allocator();
			_movable_allocator();

			movable_handle allocate(size_type n);
			void deallocate(movable_handle h);

			pointer lock(movable_handle h) const;
			void unlock(movable_handle h) const;

			void construct(pointer p, const_reference val);
			void destroy(pointer p);
		};

		template<typename Type>
		_movable_allocator<Type>::~_movable_allocator()
		{
		}

		template<typename Type>
		_movable_allocator<Type>::_movable_allocator()
		{
		}

		template<typename Type>
		typename _movable_allocator<Type>::movable_handle _movable_allocator<Type>::allocate(size_type n)
		{
			return (movable_handle)malloc(sizeof(Type) * n); // TODO
		}

		template<typename Type>
		void _movable_allocator<Type>::deallocate(movable_handle h)
		{
			free(h); // TODO
		}

		template<typename Type>
		typename _movable_allocator<Type>::pointer _movable_allocator<Type>::lock(movable_handle h) const
		{
			return (pointer)h; // TODO
		}

		template<typename Type>
		void _movable_allocator<Type>::unlock(movable_handle /*h*/) const
		{
			// TODO
		}

		template<typename Type>
		void _movable_allocator<Type>::construct(pointer p, const_reference val)
		{
			sarastd::pvt::_construct(p, val);
		}

		template<typename Type>
		void _movable_allocator<Type>::destroy(pointer p)
		{
			sarastd::pvt::_destroy(p);
		}
	}

	///=============================================================================================
	template<typename InputIt, typename ForwardIt>
	ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first)
	{
		for (; first != last; ++first, ++d_first) {
			sarastd::pvt::_construct(sarastd::pvt::_addressof(*d_first), *first);
		}
		return d_first;
	}

	template<typename InputIt, typename Size, typename ForwardIt>
	ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first)
	{
		for (; count > 0; ++first, ++d_first, --count) {
			sarastd::pvt::_construct(sarastd::pvt::_addressof(*d_first), *first);
		}
		return d_first;
	}

	template<typename ForwardIt, typename T>
	void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value)
	{
		for (; first != last; ++first) {
			sarastd::pvt::_construct(sarastd::pvt::_addressof(*first), value);
		}
	}

	template<typename ForwardIt, typename Size, typename T>
	void uninitialized_fill_n(ForwardIt first, Size count, const T& value)
	{
		for (; count > 0; ++first, --count) {
			sarastd::pvt::_construct(sarastd::pvt::_addressof(*first), value);
		}
	}

	///=============================================================================================
	template<typename OutputIterator, typename Type>
	class raw_storage_iterator: public iterator<output_iterator_tag, void, void, void, void> {
	public:
		explicit raw_storage_iterator(OutputIterator x);

		raw_storage_iterator& operator *();
		raw_storage_iterator& operator =(const Type& element);

		raw_storage_iterator& operator ++();
		raw_storage_iterator operator ++(int);

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
		sarastd::pvt::_construct(sarastd::pvt::_addressof(*_M_iter), element);
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

	///=============================================================================================
	template<typename Type>
	struct auto_ptr_ref {
		explicit auto_ptr_ref(Type* p);

		Type* m_ptr;
	};

	template<typename Type>
	auto_ptr_ref<Type>::auto_ptr_ref(Type* p) :
		m_ptr(p)
	{
	}

	template<typename Type>
	class auto_ptr {
	public:
		typedef Type element_type;

		~auto_ptr();

		template<typename U>
		auto_ptr(auto_ptr<U>& a) throw();
		auto_ptr(auto_ptr& a) throw();
		explicit auto_ptr(element_type* p = 0) throw();
		auto_ptr(auto_ptr_ref<element_type> ref) throw();

		template<typename U>
		auto_ptr& operator =(auto_ptr<U>& a) throw();
		auto_ptr& operator =(auto_ptr& a) throw();
		auto_ptr& operator =(auto_ptr_ref<element_type> ref) throw();

		element_type& operator*() const throw();

		element_type* operator->() const throw();

		element_type* get() const throw();

		element_type* release() throw();

		void reset(element_type* p = 0) throw();

		template<typename U>
		operator auto_ptr_ref<U>() throw()
		{
			return auto_ptr_ref<U>(this->release());
		}

		template<typename U>
		operator auto_ptr<U>() throw()
		{
			return auto_ptr<U>(this->release());
		}

	private:
		Type* m_ptr;
	};

	template<typename Type>
	auto_ptr<Type>::~auto_ptr()
	{
		delete m_ptr;
	}

	template<typename Type>
	auto_ptr<Type>::auto_ptr(element_type* p) throw() :
		m_ptr(p)
	{
	}

	template<typename Type>
	auto_ptr<Type>::auto_ptr(auto_ptr& a) throw() :
		m_ptr(a.release())
	{
	}

	template<typename Type>
	template<typename U>
	auto_ptr<Type>::auto_ptr(auto_ptr<U>& a) throw() :
		m_ptr(a.release())
	{
	}

	template<typename Type>
	auto_ptr<Type>& auto_ptr<Type>::operator =(auto_ptr& a) throw()
	{
		reset(a.release());
		return *this;
	}

	template<typename Type>
	template<typename U>
	auto_ptr<Type>& auto_ptr<Type>::operator =(auto_ptr<U>& a) throw()
	{
		reset(a.release());
		return *this;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type& auto_ptr<Type>::operator *() const throw()
	{
		return *m_ptr;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type* auto_ptr<Type>::operator->() const throw()
	{
		return m_ptr;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type* auto_ptr<Type>::get() const throw()
	{
		return m_ptr;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type* auto_ptr<Type>::release() throw()
	{
		element_type* tmp = m_ptr;
		m_ptr = 0;
		return tmp;
	}

	template<typename Type>
	void auto_ptr<Type>::reset(element_type* p) throw()
	{
		if (p != m_ptr) {
			delete m_ptr;
			m_ptr = p;
		}
	}

	template<typename Type>
	auto_ptr<Type>::auto_ptr(auto_ptr_ref<element_type> ref) throw() :
		m_ptr(ref.m_ptr)
	{
	}

	template<typename Type>
	auto_ptr<Type>& auto_ptr<Type>::operator =(auto_ptr_ref<element_type> ref) throw()
	{
		if (ref.m_ptr != this->get()) {
			delete m_ptr;
			m_ptr = ref.m_ptr;
		}
		return *this;
	}

}

#endif
