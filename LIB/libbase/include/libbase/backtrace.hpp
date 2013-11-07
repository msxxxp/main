#ifndef LIBBASE_BACKTRACE_HPP_
#define	LIBBASE_BACKTRACE_HPP_

#include <libbase/std.hpp>

//#include <iosfwd>
#include <vector>

#include <dbghelp.h>

#ifndef ENABLE_LOGGER
#define LogBacktrace()
#else
#define LogBacktrace() Base::Backtrace().Print()
#endif

namespace Base {

	struct FrameInfo: private Base::Uncopyable
	{
		~FrameInfo();

		FrameInfo(size_t frame);

//		FrameInfo(const FrameInfo & right);

		FrameInfo(FrameInfo && right);

//		FrameInfo & operator = (const FrameInfo & right);

		FrameInfo & operator = (FrameInfo && right);

		void swap(FrameInfo & right);

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

	struct Backtrace: private std::vector<FrameInfo> {
		~Backtrace();

		Backtrace(PCWSTR path = nullptr, size_t depth = MAX_DEPTH);

		using std::vector<FrameInfo>::begin;
		using std::vector<FrameInfo>::end;
		using std::vector<FrameInfo>::empty;
		using std::vector<FrameInfo>::size;
		using std::vector<FrameInfo>::operator[];

		void Print() const;

	private:
		static const size_t MAX_DEPTH = 64;
	};
}

#endif
