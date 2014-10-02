#ifndef BASIS_STRING_STR_IMPL_HPP_
#define BASIS_STRING_STR_IMPL_HPP_

#include <basis/simstd/string>

#include <basis/ext/pattern.hpp>

#include <basis/sys/logger.hpp>

namespace simstd {

	namespace pvt {

		struct Str_rep {
			Str_rep(size_t capa);

			void increase_ref();

			bool decrease_ref();

			size_t get_size() const;

			size_t get_capa() const;

			size_t m_size;
			size_t m_capa;
			size_t m_refc;
		};

		inline Str_rep::Str_rep(size_t capa):
			m_size(0),
			m_capa(capa),
			m_refc(1)
		{
			LogTraceObj();
		}

		inline void Str_rep::increase_ref()
		{
			++m_refc;
		}

		inline bool Str_rep::decrease_ref()
		{
			LogTraceObj();
			return --m_refc == 0;
		}

		inline size_t Str_rep::get_size() const
		{
			return m_size;
		}

		inline size_t Str_rep::get_capa() const
		{
			return m_capa;
		}

		template<typename Type, typename Allocator>
		struct Str_impl: public Str_rep, public Allocator {
			Str_impl(const Allocator& otherAlloc, size_t capa);

			Type m_data[1];

			const Allocator& get_allocator() const {return *static_cast<Allocator*>(const_cast<Str_impl*>(this));}

			Type* get_data() const;

			void set_size(size_t size);

			static const Type m_terminal_char = static_cast<Type>(0);
		};

		template<typename T, typename A>
		Str_impl<T, A>::Str_impl(const A& otherAlloc, size_t capa):
			Str_rep(capa),
			A(otherAlloc)
		{
			LogTraceObj();
		}

		template<typename T, typename A>
		T* Str_impl<T, A>::get_data() const
		{
			return const_cast<T*>(m_data);
		}

		template<typename T, typename A>
		void Str_impl<T, A>::set_size(size_t size)
		{
			m_size = size;
			m_data[size] = static_cast<T>(0);
		}

	}

}

#endif
