#ifndef BASIS_STRING_STR_BASE_HPP_
#define BASIS_STRING_STR_BASE_HPP_

#include <basis/simstd/string>

namespace simstd {

	namespace pvt {

		template<typename Type, typename Allocator>
		class Str_base {
		public:
			Str_base(const Allocator& allocator) ;

			Str_base(Str_base && other);

			const Allocator& get_allocator() const;

		protected:
			typedef Str_impl<Type, Allocator> impl_type;
			typedef typename Allocator::template rebind<impl_type>::other ImplAllocator;
			typedef typename Allocator::template rebind<char>::other RawAllocator;

			impl_type* m_impl;

		private:
			template<typename ... Args>
			static impl_type* new_impl(const Allocator& alloc, Args &&... args);

			static void delete_impl(const impl_type* ptr);
		};

		template<typename T, typename A>
		Str_base<T, A>::Str_base(const A& alloc) :
			m_impl(new_impl(alloc))
		{
		}

		template<typename T, typename A>
		Str_base<T, A>::Str_base(Str_base && other) :
			m_impl(new_impl(other.get_allocator()))
		{
		}

		template<typename T, typename A>
		const A& Str_base<T, A>::get_allocator() const
		{
			return m_impl->get_allocator();
		}

		template<typename T, typename A>
		template<typename ... Args>
		typename Str_base<T, A>::impl_type* Str_base<T, A>::new_impl(const A& alloc, Args &&... args)
		{
			const RawAllocator& rawAlloc(alloc);
			size_t size = 0;
			auto ret = reinterpret_cast<Str_base<T, A>::impl_type*>(rawAlloc.allocate(size));
			alloc.construct(ret, simstd::forward<Args>(args)...);
			return ret;
		}

		template<typename T, typename A>
		void Str_base<T, A>::delete_impl(const impl_type* ptr)
		{
			auto impl = const_cast<impl_type*>(ptr);
			A alloc = impl->get_allocator();
			alloc.destroy(impl);
			alloc.deallocate(impl, 1);
		}

	}

}

#endif
