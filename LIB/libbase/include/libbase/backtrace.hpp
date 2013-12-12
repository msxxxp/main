#ifndef LIBBASE_BACKTRACE_HPP_
#define	LIBBASE_BACKTRACE_HPP_

#include <libbase/std.hpp>

#include <vector>

namespace Backtrace {

	struct Frame: private Base::Uncopyable
	{
		~Frame();

		Frame(size_t frame);

		Frame(Frame && right);

		Frame & operator = (Frame && right);

		void swap(Frame & right);

		size_t frame() const {return m_frame;}

		const ustring & source() const;

		const ustring & func() const;

		const ustring & module() const;

		size_t addr() const;

		size_t offset() const;

		size_t line() const;

		ustring to_str() const;

	private:
		void InitData() const;

		size_t m_frame;

		struct Data;
		mutable Data * m_data;
	};

	struct Enum: private std::vector<Frame> {
		Enum(PCWSTR path = nullptr, size_t depth = MAX_DEPTH);

		using std::vector<Frame>::begin;
		using std::vector<Frame>::end;
		using std::vector<Frame>::empty;
		using std::vector<Frame>::size;
		using std::vector<Frame>::operator[];

		void Print() const;

	private:
		static const size_t MAX_DEPTH = 64;
	};
}

#endif
