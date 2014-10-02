#ifndef BASIS_STRING_STR_IMPL_HPP_
#define BASIS_STRING_STR_IMPL_HPP_

#include <basis/simstd/string>

#include <basis/ext/pattern.hpp>

#include <basis/sys/logger.hpp>

namespace simstd {

	namespace pvt {

		struct Str_rep {
			virtual ~Str_rep() = default;

			Str_rep(size_t capa);

			void increase_ref();

			bool decrease_ref();

			size_t m_len;
			size_t m_cap;
			size_t m_ref;
		};

		inline Str_rep::Str_rep(size_t capa):
			m_len(0),
			m_cap(capa),
			m_ref(1)
		{
			LogTraceObj();
		}

		inline void Str_rep::increase_ref()
		{
			++m_ref;
		}

		inline bool Str_rep::decrease_ref()
		{
			LogTraceObj();
			return --m_ref == 0;
		}

		template<typename Type, typename Allocator>
		struct Str_impl: public Str_rep, public Allocator {
			Str_impl(const Allocator& otherAlloc, size_t capa);

			Type m_data[1];

			const Allocator& get_allocator() const {return *static_cast<Allocator*>(const_cast<Str_impl*>(this));}

			static const Type m_terminal_char = static_cast<Type>(0);
		};

		template<typename T, typename A>
		Str_impl<T, A>::Str_impl(const A& otherAlloc, size_t capa):
			Str_rep(capa),
			A(otherAlloc)
		{
			LogTraceObj();
		}

	}

}

#endif
