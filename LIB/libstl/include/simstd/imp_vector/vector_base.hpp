#ifndef LIBSTL_VECTOR_VECTOR_BASE_HPP_
#define LIBSTL_VECTOR_VECTOR_BASE_HPP_

#include <simstd/types.hpp>
#include <simstd/iterator>
#include <simstd/memory>
#include <simstd/vector>

namespace simstd {

	namespace pvt {
		template<typename Type, typename Allocator>
		struct vector_base
		{
			typedef vector_base this_type;

			typedef Type value_type;
			typedef Allocator allocator_type;
			typedef allocator_traits<allocator_type> traits_type;
			typedef typename traits_type::size_type size_type;
			typedef typename traits_type::pointer pointer;

			allocator_type allocator;
			pointer begin;
			pointer end;
			pointer end_of_storage;

			~vector_base();
			vector_base();
			vector_base(size_type capa, const allocator_type& alloc = allocator_type());
//			vector_base(size_type capa, pointer first, pointer last);
			vector_base(this_type&& other);
			vector_base(vector_base&& other, const allocator_type& alloc);
			void swap(this_type& other);

			template<typename ... Args>
			void construct(pointer ptr, Args&&... args);

			void destroy(pointer first, pointer last);
			void reserve(size_type newCapacity);
			void adjust_capacity(size_type addToSize);
			bool check_capacity_if_size_grows(size_type addToSize) const;
			size_type get_new_capacity(size_type addToSize) const;
			size_type capacity() const;
			size_type size() const;
			size_type max_size() const;

		private:
			void create_storage(size_type capa);
		};

		template<typename Type, typename Allocator>
		vector_base<Type, Allocator>::~vector_base()
		{
			if (begin) {
				destroy(begin, end);
				end = begin;
				traits_type::deallocate(allocator, begin, capacity());
			}
		}

		template<typename Type, typename Allocator>
		vector_base<Type, Allocator>::vector_base() :
			allocator(),
			begin(0),
			end(0),
			end_of_storage(0)
		{
		}

		template<typename Type, typename Allocator>
		vector_base<Type, Allocator>::vector_base(size_type capa, const allocator_type& alloc) :
			allocator(alloc),
			begin(0),
			end(0),
			end_of_storage(0)
		{
			create_storage(capa);
		}

//		template<typename Type, typename Allocator>
//		vector_base<Type, Allocator>::vector_base(size_type capa, pointer first, pointer last) :
//			allocator(),
//			begin(0),
//			end(0),
//			end_of_storage(0)
//		{
//			create_storage(capa);
//			simstd::uninitialized_move(first, last, end);
//			end += (last - first);
//		}

		template<typename Type, typename Allocator>
		vector_base<Type, Allocator>::vector_base(this_type&& other) :
			allocator(simstd::move(other.allocator)),
			begin(0),
			end(0),
			end_of_storage(0)
		{
			swap(other);
		}

		template<typename Type, typename Allocator>
		vector_base<Type, Allocator>::vector_base(vector_base&& other, const allocator_type& alloc) :
			allocator(allocator),
			begin(0),
			end(0),
			end_of_storage(0)
		{
			if (other.allocator == alloc) {
				swap(other);
			} else {
//				size_t __n = __x._M_impl._M_finish - __x._M_impl._M_start;
//				_M_create_storage(__n);
			}
		}

		template<typename Type, typename Allocator>
		void vector_base<Type, Allocator>::swap(this_type& other)
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
		void vector_base<Type, Allocator>::create_storage(size_type capa)
		{
			end = begin = (capa) ? traits_type::allocate(allocator, capa) : 0;
			end_of_storage = begin + (begin ? capa : 0);
		}

		template<typename Type, typename Allocator>
		template<typename ... Args>
		void vector_base<Type, Allocator>::construct(pointer ptr, Args&&... args)
		{
			traits_type::construct(allocator, ptr, simstd::forward<Args>(args)...);
		}

		template<typename Type, typename Allocator>
		void vector_base<Type, Allocator>::destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				traits_type::destroy(allocator, first);
		}

		template<typename Type, typename Allocator>
		void vector_base<Type, Allocator>::reserve(size_type newCapacity)
		{
			if (capacity() < newCapacity) {
				this_type tmp(newCapacity, allocator);
				tmp.end = simstd::uninitialized_copy(simstd::make_move_iterator(begin), simstd::make_move_iterator(end), tmp.begin);
				tmp.swap(*this);

				tmp.destroy(tmp.begin, tmp.end);
			}
		}

		template<typename Type, typename Allocator>
		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::size() const
		{
			return end - begin;
		}

		template<typename Type, typename Allocator>
		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::max_size() const
		{
			return 0xFFFFFFFFu;
		}

		template<typename Type, typename Allocator>
		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::capacity() const
		{
			return end_of_storage - begin;
		}

		template<typename Type, typename Allocator>
		bool vector_base<Type, Allocator>::check_capacity_if_size_grows(size_type addToSize) const
		{
			if (addToSize > end_of_storage - end)
				return false;
			return true;
		}

		template<typename Type, typename Allocator>
		typename vector_base<Type, Allocator>::size_type vector_base<Type, Allocator>::get_new_capacity(size_type addToSize) const
		{
			return size() + simstd::max(size_type(4), simstd::max(size(), addToSize));
		}

		template<typename Type, typename Allocator>
		void vector_base<Type, Allocator>::adjust_capacity(size_type addToSize)
		{
			if (!check_capacity_if_size_grows(addToSize))
				reserve(get_new_capacity(addToSize));
		}

		///=========================================================================================
		template<typename Type>
		struct vector_base<Type, simstd::pvt::movable_allocator<Type> > : private simstd::pvt::movable_allocator<Type>
		{
			typedef vector_base<Type, simstd::pvt::movable_allocator<Type> > this_type;
			typedef Type value_type;
			typedef simstd::pvt::movable_allocator<Type> allocator_type;
			typedef typename allocator_traits<allocator_type>::size_type size_type;
			typedef typename allocator_traits<allocator_type>::pointer pointer;
			typedef typename allocator_type::movable_handle movable_handle;

			mutable pointer begin;
			mutable pointer end;
			mutable pointer end_of_storage;

			~vector_base();
			vector_base();
			vector_base(size_type capa);
			vector_base(size_type capa, pointer first, pointer last);
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
		vector_base<Type, simstd::pvt::movable_allocator<Type> >::~vector_base()
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
		vector_base<Type, simstd::pvt::movable_allocator<Type> >::vector_base() :
			begin(0),
			end(0),
			end_of_storage(0),
			handle(0)
		{
		}

		template<typename Type>
		vector_base<Type, simstd::pvt::movable_allocator<Type> >::vector_base(size_type capa) :
			begin(0),
			end(0),
			end_of_storage(0),
			handle(0)
		{
			create_storage(capa);
		}

		template<typename Type>
		vector_base<Type, simstd::pvt::movable_allocator<Type> >::vector_base(size_type capa, pointer first, pointer last) :
			begin(0),
			end(0),
			end_of_storage(0),
			handle(0)
		{
			create_storage(capa);
			lock();
			simstd::uninitialized_copy(first, last, end);
			end += (last - first);
			unlock();
		}

		template<typename Type>
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::swap(this_type& other)
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
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::create_storage(size_type capa)
		{
			handle = (capa) ? allocator_type::allocate(capa) : 0;
			end = begin = 0;
			end_of_storage = begin + (handle ? capa : 0);
		}

		template<typename Type>
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::destroy(pointer first, pointer last)
		{
			for (; first != last; ++first)
				allocator_type::destroy(first);
		}

		template<typename Type>
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::reserve(size_type newCapacity)
		{
			if (capacity() < newCapacity) {
				lock();
				this_type tmp(newCapacity, begin, end);
				unlock();
				tmp.swap(*this);
			}
		}

		template<typename Type>
		typename vector_base<Type, simstd::pvt::movable_allocator<Type> >::size_type vector_base<Type, simstd::pvt::movable_allocator<Type> >::size() const
		{
			return end - begin;
		}

		template<typename Type>
		typename vector_base<Type, simstd::pvt::movable_allocator<Type> >::size_type vector_base<Type, simstd::pvt::movable_allocator<Type> >::max_size() const
		{
			return 0xFFFFFFFFu;
		}

		template<typename Type>
		typename vector_base<Type, simstd::pvt::movable_allocator<Type> >::size_type vector_base<Type, simstd::pvt::movable_allocator<Type> >::capacity() const
		{
			return end_of_storage - begin;
		}

		template<typename Type>
		bool vector_base<Type, simstd::pvt::movable_allocator<Type> >::check_capacity(size_type addToSize) const
		{
			if ((size() + addToSize) > capacity())
				return false;
			return true;
		}

		template<typename Type>
		typename vector_base<Type, simstd::pvt::movable_allocator<Type> >::size_type vector_base<Type, simstd::pvt::movable_allocator<Type> >::get_new_capacity(size_type addToSize) const
		{
			return size() + simstd::max(size_type(4), simstd::max(size(), addToSize));
		}

		template<typename Type>
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::adjust_capacity(size_type addToSize)
		{
			if (!check_capacity(addToSize))
				reserve(get_new_capacity(addToSize));
		}

		template<typename Type>
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::lock() const
		{
			pointer oldBegin = begin;
			begin = (pointer)allocator_type::lock(handle);
			ptrdiff_t diff = begin - oldBegin;
			end += diff;
			end_of_storage += diff;
		}

		template<typename Type>
		void vector_base<Type, simstd::pvt::movable_allocator<Type> >::unlock() const
		{
			allocator_type::unlock(handle);
		}
	}
}

#endif
