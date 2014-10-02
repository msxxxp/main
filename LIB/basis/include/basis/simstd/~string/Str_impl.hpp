#ifndef BASIS_STRING_STR_IMPL_HPP_
#define BASIS_STRING_STR_IMPL_HPP_

#include <basis/simstd/string>

#include <basis/ext/pattern.hpp>

namespace simstd {

	namespace pvt {

		struct Str_rep {
			Str_rep();

			size_t m_len;
			size_t m_cap;
		};

		template<typename Type, typename Allocator>
		struct Str_impl: public Str_rep, public Allocator {
			Str_impl() = default;
			Str_impl(const Allocator& otherAlloc): Allocator(otherAlloc) {}
			Str_impl(Allocator&& otherAlloc): Allocator(simstd::move(otherAlloc)) {}

			Type m_data[1];

			const Allocator& get_allocator() const {return static_cast<Allocator>(*this);}

			static const Type m_terminal_char = static_cast<Type>(0);
		};

	}

}

#endif
