#ifndef SARALIB_STL_MEMORY_HPP_
#define SARALIB_STL_MEMORY_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"
#include "impl_new.hpp"
#include "impl_utility.hpp"

namespace sarastd {

	namespace pvt {
		template<typename Type>
		Type* _addressof(Type& r)
		{
			return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(r)));
		}

		template<typename Type>
		Type* _allocate(sarastd::size_t n)
		{
			return (Type*)::operator new(n * sizeof(Type), sarastd::nothrow);
		}

		template<typename Type>
		void _deallocate(Type* ptr, sarastd::size_t /*n*/ = 0)
		{
			::operator delete(ptr, sarastd::nothrow);
		}

		template<typename Type1, typename Type2>
		void _construct(Type1* p, const Type2& value)
		{
			::new (static_cast<void*>(p), sarastd::nothrow) Type1(value);
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
//
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
		void deallocate(pointer ptr, size_type n);

		void construct(pointer ptr, const_reference val);
		void destroy(pointer ptr);
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
		return sarastd::pvt::_allocate<Type>(n);
	}

	template<typename Type>
	void allocator<Type>::deallocate(pointer ptr, size_type n)
	{
		sarastd::pvt::_deallocate(ptr, n);
	}

	template<typename Type>
	void allocator<Type>::construct(pointer ptr, const_reference val)
	{
		sarastd::pvt::_construct(ptr, val);
	}

	template<typename Type>
	void allocator<Type>::destroy(pointer ptr)
	{
		sarastd::pvt::_destroy(ptr);
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
			return (movable_handle)_system_movable_malloc(sizeof(Type) * n);
		}

		template<typename Type>
		void _movable_allocator<Type>::deallocate(movable_handle h)
		{
			_system_movable_free(h);
		}

		template<typename Type>
		typename _movable_allocator<Type>::pointer _movable_allocator<Type>::lock(movable_handle h) const
		{
			return (pointer)_system_movable_lock(h);
		}

		template<typename Type>
		void _movable_allocator<Type>::unlock(movable_handle h) const
		{
			_system_movable_unlock(h);
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

		element_type& operator *() const throw();
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

	namespace pvt {
		struct ref_counter {
			virtual ~ref_counter();

			ref_counter();

			void increase_ref();

			void decrease_ref();

			sarastd::size_t count() const;

		private:
			virtual void destroy() const;

			virtual void deallocate() const = 0;

			ref_counter(const ref_counter & rhs); // forbidden

			ref_counter & operator = (const ref_counter & rhs); // forbidden

		private:
			sarastd::size_t m_refcnt;
		};

		ref_counter::~ref_counter() {}

		ref_counter::ref_counter() : m_refcnt(1) {}

		void ref_counter::increase_ref() {++m_refcnt;}

		void ref_counter::decrease_ref()
		{
			if (--m_refcnt == 0) {
				destroy();
				deallocate();
			}
		}

		sarastd::size_t ref_counter::count() const {return m_refcnt;}

		void ref_counter::destroy() const {}
	}

	template<typename Type>
	struct default_delete
	{
		void operator ()(Type * ptr) const
		{
			delete(ptr);
		}
	};

	template<typename Type> struct default_delete<Type[]>
	{
		void operator ()(Type * ptr) const
		{
			delete [](ptr);
		}
	};

	template<typename Type>
	class shared_ptr {
		typedef Type element_type;
		typedef shared_ptr this_type;

	public:
		typedef size_t size_type;

		~shared_ptr() throw()
		{
			reset();
		}

		shared_ptr() throw() :
			m_impl(0)
		{
		}

		explicit shared_ptr(element_type * ptr) :
			m_impl(new shared_ptr_impl(ptr))
		{
		}

		template<typename Deleter>
		shared_ptr(element_type * ptr, Deleter d) :
			m_impl(new shared_ptr_impl_deleter<Deleter>(ptr, d))
		{
		}

		shared_ptr(const this_type & other) :
			m_impl(0)
		{
			if (other.m_impl) {
				m_impl = other.m_impl;
				m_impl->increase_ref();
			}
		}

		this_type & operator =(const this_type & other)
		{
			if (m_impl != other.m_impl) {
				shared_ptr(other).swap(*this);
			}
			return *this;
		}

		template<typename newType>
		operator shared_ptr<newType>()
		{
			return shared_ptr<newType>(m_impl); // TODO
		}

		void reset()
		{
			if (m_impl) {
				m_impl->decrease_ref();
				m_impl = 0;
			}
		}

		void reset(element_type * ptr)
		{
			shared_ptr(ptr).swap(*this);
		}

		template<typename Deleter>
		void reset(element_type * ptr, Deleter d)
		{
			shared_ptr(ptr, d).swap(*this);
		}

		element_type & operator *() const
		{
			return *(m_impl->get());
		}

		element_type * operator ->() const
		{
			return m_impl->get();
		}

		element_type * get() const
		{
			return (m_impl) ? m_impl->get() : 0;
		}

		bool unique() const
		{
			return !m_impl || m_impl->count() == 1;
		}

		size_type use_count() const
		{
			return (m_impl) ? m_impl->count() : 0;
		}

		operator bool() const
		{
			return m_impl && m_impl->get();
		}

		void swap(this_type & b) throw()
		{
#if defined(__GNUC__) && (__GNUC__ < 3)
			sarastd::swap(m_impl, b.m_impl);
#else
			using sarastd::swap;
			swap(m_impl, b.m_impl);
#endif
		}

	private:
		typedef typename sarastd::default_delete<Type> default_deleter;

		struct shared_ptr_impl: public pvt::ref_counter
		{
			shared_ptr_impl(element_type * ptr) : m_ptr(ptr) {}

			element_type * get() const {return m_ptr;}

		private:
			void destroy() const {default_deleter()(m_ptr);}

			void deallocate() const {delete this;}

			element_type * m_ptr;
		};

		template<typename Deleter>
		struct shared_ptr_impl_deleter: public shared_ptr_impl {
			shared_ptr_impl_deleter(element_type * ptr, Deleter d) : shared_ptr_impl(ptr), m_deleter(d) {}

		private:
			void destroy() const {m_deleter(get());}

			Deleter m_deleter;
		};

		shared_ptr_impl * m_impl;
	};

	template<typename T, typename U>
	bool operator ==(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() == b.get();
	}

	template<typename T, typename U>
	bool operator !=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return sarastd::pvt::ops::operator !=(a, b);
	}

	template<typename T, typename U>
	bool operator <(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() < b.get();
	}

	template<typename T, typename U>
	bool operator >(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return sarastd::pvt::ops::operator >(a, b);
	}

	template<typename T, typename U>
	bool operator <=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return sarastd::pvt::ops::operator <=(a, b);
	}

	template<typename T, typename U>
	bool operator >=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return sarastd::pvt::ops::operator >=(a, b);
	}

	template<typename T>
	void swap(shared_ptr<T> & a, shared_ptr<T> & b) throw()
	{
		a.swap(b);
	}

}

#endif
