#ifndef LIBSTL_SYSTEM_BACKTRACE_HPP_
#define LIBSTL_SYSTEM_BACKTRACE_HPP_

#include <system/configure.hpp>
#include <extra/pattern.hpp>

#include <simstd/iosfwd>
#include <simstd/vector>

namespace backtrace {

	struct Frame: private pattern::Uncopyable
	{
		~Frame();

		Frame(size_t frame);

		Frame(Frame && right);

		Frame & operator = (Frame && right);

		void swap(Frame & right);

		size_t frame() const {return m_frame;}

		const simstd::wstring & source() const;

		const simstd::wstring & func() const;

		const simstd::wstring & module() const;

		size_t addr() const;

		size_t offset() const;

		size_t line() const;

		simstd::wstring to_str() const;

	private:
		void InitData() const;

		size_t m_frame;

		struct Data;
		mutable Data * m_data;
	};

	struct Enum: private simstd::vector<Frame> {
		typedef simstd::vector<Frame> base_type;

		Enum(PCWSTR path = nullptr, size_t depth = MAX_DEPTH);

		using base_type::begin;
		using base_type::end;
		using base_type::empty;
		using base_type::size;
		using base_type::operator[];

		void Print() const;

	private:
		static const size_t MAX_DEPTH = 64;
	};
}

#endif
