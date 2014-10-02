#ifndef BASIS_STRING_STR_BASE_HPP_
#define BASIS_STRING_STR_BASE_HPP_

#include <basis/simstd/string>
#include <basis/simstd/~algorithm/base.hpp>

#include <basis/sys/logger.hpp>

namespace simstd {

	namespace pvt {

		template<typename Type, typename Allocator>
		class Str_base {
			const size_t MIN_CAPACITY = 16;

		public:
			~Str_base();

			Str_base(const Allocator& alloc);

			Str_base(const Allocator& alloc, size_t capa);

			Str_base(Str_base && other);

			const Allocator& get_allocator() const;

		protected:
			typedef Str_impl<Type, Allocator> impl_type;
			typedef typename Allocator::template rebind<impl_type>::other ImplAllocator;
			typedef typename Allocator::template rebind<char>::other RawAllocator;

			impl_type* m_impl;

		private:
			template<typename ... Args>
			static impl_type* new_impl(const Allocator& alloc, size_t capa, Args &&... args);

			static void delete_impl(const impl_type* ptr);
		};

		template<typename T, typename A>
		Str_base<T, A>::~Str_base()
		{
			LogTraceObj();
			if (m_impl->decrease_ref())
				delete_impl(m_impl);
			m_impl = nullptr;
		}

		template<typename T, typename A>
		Str_base<T, A>::Str_base(const A& alloc) :
			m_impl(new_impl(alloc, MIN_CAPACITY))
		{
			LogTraceObj();
		}

		template<typename T, typename A>
		Str_base<T, A>::Str_base(const A& alloc, size_t capa) :
			m_impl(new_impl(alloc, simstd::max(MIN_CAPACITY, capa)))
		{
			LogTraceObj();
		}

		template<typename T, typename A>
		Str_base<T, A>::Str_base(Str_base && other) :
			m_impl(new_impl(A(), MIN_CAPACITY))
		{
			LogTraceObj();
		}

		template<typename T, typename A>
		const A& Str_base<T, A>::get_allocator() const
		{
			return m_impl->get_allocator();
		}

		template<typename T, typename A>
		template<typename ... Args>
		typename Str_base<T, A>::impl_type* Str_base<T, A>::new_impl(const A& alloc, size_t capa, Args &&... args)
		{
			LogTrace();
			RawAllocator rawAlloc(alloc);
			size_t size = capa * sizeof(T) + sizeof(impl_type);
			auto ret = reinterpret_cast<Str_base<T, A>::impl_type*>(rawAlloc.allocate(size));

			ImplAllocator implAlloc(alloc);
			implAlloc.construct(ret, alloc, capa, simstd::forward<Args>(args)...);
			return ret;
		}

		template<typename T, typename A>
		void Str_base<T, A>::delete_impl(const impl_type* ptr)
		{
			LogTrace();
			auto impl = const_cast<impl_type*>(ptr);
			ImplAllocator alloc = impl->get_allocator();
			alloc.destroy(impl);
			alloc.deallocate(impl, 1);
		}

	}

}

#endif
