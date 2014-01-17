﻿#ifndef LIBSTL_VECTOR_IMPL_HPP_
#define LIBSTL_VECTOR_IMPL_HPP_

#include "impl_types.hpp"
#include "impl_memory.hpp"

namespace simstd {

	namespace pvt {
		template<typename Type, typename Allocator>
		struct _vector_impl: private Allocator {
			typedef _vector_impl this_type;
			typedef Type value_type;
			typedef Allocator allocator_type;
			typedef typename allocator_traits<allocator_type>::size_type size_type;
			typedef typename allocator_traits<allocator_type>::pointer pointer;

			pointer begin;
			pointer end;
			pointer end_of_storage;

			~_vector_impl();
			_vector_impl();
			_vector_impl(size_type capa);
			_vector_impl(size_type capa, pointer first, pointer last);
			void swap(this_type& other);
			void destroy(pointer first, pointer last);
			void reserve(size_type newCapacity);
			void adjust_capacity(size_type addToSize);
			bool check_capacity(size_type addToSize) const;
			size_type get_new_capacity(size_type addToSize) const;
			size_type capacity() const;
			size_type size() const;
			size_type max_size() const;

		private:
			void create_storage(size_type capa);
		};

		template<typename Type, typename Allocator>
		_vector_impl<Type, Allocator>::~_vector_impl()
		{
			if (begin)
				allocator_type::deallocate(begin, capacity());
		}

		template<typename Type, typename Allocator>
		_vector_impl<Type, Allocator>::_vector_impl() :
			begin(0), end(0), end_of_storage(0)
		{
		}

		template<typename Type, typename Allocator>
		_vector_impl<Type, Allocator>::_vector_impl(size_type capa) :
			begin(0), end(0), end_of_storage(0)
		{
			create_storage(capa);
		}

		template<typename Type, typename Allocator>
		_vector_impl<Type, Allocator>::_vector_impl(size_type capa, pointer first, pointer last) :
			begin(0), end(0), end_of_storage(0)
		{
			create_storage(capa);
			simstd::uninitialized_copy(first, last, end);
			end += (last - first);
		}

		template<typename Type, typename Allocator>
		void _vector_impl<Type, Allocator>::swap(this_type& other)
		{
#if defined(__GNUC__) && (__GNUC__ < 3)
			simstd::swap(begin, other.begin);
			simstd::swap(end, other.end);
			simstd::swap(end_of_storage, other.end_of_storage);
#else
			using simstd::swap;
			swap(begin, other.begin);
			swap(end, other.end);
			swap(end_of_storage, other.end_of_storage);
#endif
		}

		template<typename Type, typename Allocator>
		void _vector_impl<Type, Allocator>::create_storage(size_type capa)
		{
			end = begin = (capa) ? allocator_type::allocate(capa) : 0;
			end_of_storage = begin + (begin ? capa : 0);
		}

		template<typename Type, typename Allocator>
		void _vector_impl<Type, Allocator>::destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				allocator_type::destroy(first);
		}

		template<typename Type, typename Allocator>
		void _vector_impl<Type, Allocator>::reserve(size_type newCapacity)
		{
			if (capacity() < newCapacity)
				this_type(newCapacity, begin, end).swap(*this);
		}

		template<typename Type, typename Allocator>
		typename
		_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::size() const
		{
			return end - begin;
		}

		template<typename Type, typename Allocator>
		typename
		_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::max_size() const
		{
			return 0xFFFFFFFFu;
		}

		template<typename Type, typename Allocator>
		typename
		_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::capacity() const
		{
			return end_of_storage - begin;
		}

		template<typename Type, typename Allocator>
		bool _vector_impl<Type, Allocator>::check_capacity(size_type addToSize) const
		{
			if ((size() + addToSize) > capacity())
				return false;
			return true;
		}

		template<typename Type, typename Allocator>
		typename
		_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::get_new_capacity(size_type addToSize) const
		{
			return size() + simstd::max(size_type(4), simstd::max(size(), addToSize));
		}

		template<typename Type, typename Allocator>
		void _vector_impl<Type, Allocator>::adjust_capacity(size_type addToSize)
		{
			if (!check_capacity(addToSize))
				reserve(get_new_capacity(addToSize));
		}

		///=========================================================================================
		template<typename Type>
		struct _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >: private simstd::pvt::_movable_allocator<Type> {
			typedef _vector_impl<Type, simstd::pvt::_movable_allocator<Type> > this_type;
			typedef Type value_type;
			typedef simstd::pvt::_movable_allocator<Type> allocator_type;
			typedef typename allocator_traits<allocator_type>::size_type size_type;
			typedef typename allocator_traits<allocator_type>::pointer pointer;
			typedef typename allocator_type::movable_handle movable_handle;

			mutable pointer begin;
			mutable pointer end;
			mutable pointer end_of_storage;

			~_vector_impl();
			_vector_impl();
			_vector_impl(size_type capa);
			_vector_impl(size_type capa, pointer first, pointer last);
			void swap(this_type& other);
			void destroy(pointer first, pointer last);
			void reserve(size_type newCapacity);
			void adjust_capacity(size_type addToSize);
			bool check_capacity(size_type addToSize) const;
			size_type get_new_capacity(size_type addToSize) const;
			size_type capacity() const;
			size_type size() const;
			size_type max_size() const;

			void lock() const;
			void unlock() const;

		private:
			void create_storage(size_type capa);

			movable_handle handle;
		};

		template<typename Type>
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::~_vector_impl()
		{
			if (handle) {
				lock();
				while (end != begin)
					allocator_type::destroy(--end);
				unlock();
				allocator_type::deallocate(handle);
			}
		}

		template<typename Type>
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::_vector_impl() :
			begin(0), end(0), end_of_storage(0), handle(0)
		{
		}

		template<typename Type>
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::_vector_impl(size_type capa) :
			begin(0), end(0), end_of_storage(0), handle(0)
		{
			create_storage(capa);
		}

		template<typename Type>
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::_vector_impl(size_type capa, pointer first, pointer last) :
			begin(0), end(0), end_of_storage(0), handle(0)
		{
			create_storage(capa);
			lock();
			simstd::uninitialized_copy(first, last, end);
			end += (last - first);
			unlock();
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::swap(this_type& other)
		{
#if defined(__GNUC__) && (__GNUC__ < 3)
			simstd::swap(begin, other.begin);
			simstd::swap(end, other.end);
			simstd::swap(end_of_storage, other.end_of_storage);
			simstd::swap(handle, other.handle);
#else
			using simstd::swap;
			swap(begin, other.begin);
			swap(end, other.end);
			swap(end_of_storage, other.end_of_storage);
			swap(handle, other.handle);
#endif
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::create_storage(size_type capa)
		{
			handle = (capa) ? allocator_type::allocate(capa) : 0;
			end = begin = 0;
			end_of_storage = begin + (handle ? capa : 0);
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				allocator_type::destroy(first);
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::reserve(size_type newCapacity)
		{
			if (capacity() < newCapacity) {
				lock();
				this_type tmp(newCapacity, begin, end);
				unlock();
				tmp.swap(*this);
			}
		}

		template<typename Type>
		typename
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::size_type _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::size() const
		{
			return end - begin;
		}

		template<typename Type>
		typename
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::size_type _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::max_size() const
		{
			return 0xFFFFFFFFu;
		}

		template<typename Type>
		typename
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::size_type _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::capacity() const
		{
			return end_of_storage - begin;
		}

		template<typename Type>
		bool _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::check_capacity(size_type addToSize) const
		{
			if ((size() + addToSize) > capacity())
				return false;
			return true;
		}

		template<typename Type>
		typename
		_vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::size_type _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::get_new_capacity(size_type addToSize) const
		{
			return size() + simstd::max(size_type(4), simstd::max(size(), addToSize));
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::adjust_capacity(size_type addToSize)
		{
			if (!check_capacity(addToSize))
				reserve(get_new_capacity(addToSize));
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::lock() const
		{
			pointer oldBegin = begin;
			begin = (pointer)allocator_type::lock(handle);
			simstd::ptrdiff_t diff = begin - oldBegin;
			end += diff;
			end_of_storage += diff;
		}

		template<typename Type>
		void _vector_impl<Type, simstd::pvt::_movable_allocator<Type> >::unlock() const
		{
			allocator_type::unlock(handle);
		}
	}
}

#endif
