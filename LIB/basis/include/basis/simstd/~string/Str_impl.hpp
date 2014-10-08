#ifndef BASIS_STRING_STR_IMPL_HPP_
#define BASIS_STRING_STR_IMPL_HPP_

#include <basis/simstd/string>

#include <basis/ext/pattern.hpp>

namespace simstd {
	namespace pvt {

		struct StrImplBase {
			StrImplBase(size_t capa);

			void increase_ref();

			bool decrease_ref();

			bool is_shared();

			size_t get_size() const;

			size_t get_capa() const;

			size_t m_size;
			size_t m_capa;
			size_t m_refc;
		};

		inline StrImplBase::StrImplBase(size_t capa):
			m_size(0),
			m_capa(capa),
			m_refc(1)
		{
		}

		inline void StrImplBase::increase_ref()
		{
			++m_refc;
		}

		inline bool StrImplBase::decrease_ref()
		{
			return --m_refc == 0;
		}

		inline bool StrImplBase::is_shared()
		{
			return m_refc > 1;
		}

		inline size_t StrImplBase::get_size() const
		{
			return m_size;
		}

		inline size_t StrImplBase::get_capa() const
		{
			return m_capa;
		}

		template<typename Type, typename Allocator>
		struct StrImpl: public StrImplBase, public Allocator {
			StrImpl(size_t capa, const Allocator& alloc);

			const Allocator& get_allocator() const {return *static_cast<Allocator*>(const_cast<StrImpl*>(this));}

			Type* get_data() const;

			Type* get_end() const;

			void set_size(size_t size);

		private:
			Type m_data[1];

			static const Type m_terminal_char = static_cast<Type>(0);
		};

		template<typename T, typename A>
		StrImpl<T, A>::StrImpl(size_t capa, const A& alloc):
			StrImplBase(capa),
			A(alloc)
		{
			*m_data = m_terminal_char;
		}

		template<typename T, typename A>
		T* StrImpl<T, A>::get_data() const
		{
			return const_cast<T*>(m_data);
		}

		template<typename T, typename A>
		T* StrImpl<T, A>::get_end() const
		{
			return const_cast<T*>(m_data) + m_size;
		}

		template<typename T, typename A>
		void StrImpl<T, A>::set_size(size_t size)
		{
			m_size = size;
			m_data[size] = m_terminal_char;
		}

	}
}

#endif
