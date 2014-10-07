#ifndef BASIS_STRING_STR_BASE_HPP_
#define BASIS_STRING_STR_BASE_HPP_

#include <basis/simstd/string>
#include <basis/simstd/~algorithm/base.hpp>

#include <basis/sys/logger.hpp>

namespace simstd {

	namespace pvt {

		template<typename Type, typename Allocator>
		class StrBase {
		public:
			~StrBase();

			StrBase(const Allocator& alloc, size_t capa = 0);

			StrBase(const StrBase & other);

			StrBase(const StrBase & other, const Allocator& alloc);

			StrBase(StrBase && other);

			StrBase(StrBase && other, const Allocator& alloc);

			const Allocator& get_allocator() const;

		protected:
			typedef StrImpl<Type, Allocator> impl_type;
			typedef typename Allocator::template rebind<impl_type>::other ImpAllocator;
			typedef typename Allocator::template rebind<char>::other      RawAllocator;

			impl_type* m_impl;

		private:
			static impl_type* new_impl(size_t capa, const Allocator& alloc);
			static void       del_impl(const impl_type* ptr);

			const size_t MIN_CAPACITY = 4;
		};

		template<typename T, typename A>
		StrBase<T, A>::~StrBase()
		{
			LogTraceObj();
			if (m_impl && m_impl->decrease_ref())
				del_impl(m_impl);
			m_impl = nullptr;
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(const A& alloc, size_t capa) :
			m_impl(nullptr)
		{
			LogTraceObj();
			m_impl = new_impl(simstd::max(MIN_CAPACITY, capa), alloc);
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(const StrBase & other) :
			m_impl(other.m_impl)
		{
			LogTraceObj();
			m_impl->increase_ref();
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(const StrBase & other, const A& alloc) :
			m_impl(other.m_impl)
		{
			LogTraceObj();
			if (alloc == other.get_allocator()) {
				m_impl->increase_ref();
			} else {
				m_impl = new_impl(other.m_impl->get_capa(), alloc);
			}
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(StrBase && other) :
			m_impl(nullptr)
		{
			LogTraceObj();
			using simstd::swap;
			swap(m_impl, other.m_impl);
		}


		template<typename T, typename A>
		StrBase<T, A>::StrBase(StrBase && other, const A& alloc) :
			m_impl(nullptr)
		{
			LogTraceObj();
			if (alloc == other.get_allocator()) {
				using simstd::swap;
				swap(m_impl, other.m_impl);
			} else {
				m_impl = new_impl(other.m_impl->get_capa(), alloc);
			}
		}

		template<typename T, typename A>
		const A& StrBase<T, A>::get_allocator() const
		{
			return m_impl->get_allocator();
		}

		template<typename T, typename A>
		typename StrBase<T, A>::impl_type* StrBase<T, A>::new_impl(size_t capa, const A& alloc)
		{
			RawAllocator rawAlloc(alloc);
			size_t size = capa * sizeof(T) + sizeof(impl_type);
			auto ret = reinterpret_cast<StrBase<T, A>::impl_type*>(rawAlloc.allocate(size));

			ImpAllocator implAlloc(alloc);
			implAlloc.construct(ret, capa, alloc);
			return ret;
		}

		template<typename T, typename A>
		void StrBase<T, A>::del_impl(const impl_type* ptr)
		{
			auto impl = const_cast<impl_type*>(ptr);
			ImpAllocator alloc = impl->get_allocator();
			alloc.destroy(impl);
			alloc.deallocate(impl, 1);
		}

	}

}

#endif
