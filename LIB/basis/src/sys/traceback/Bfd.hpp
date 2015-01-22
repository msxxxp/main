#ifndef SYS_TRACEBACK_BFD_HPP_
#define SYS_TRACEBACK_BFD_HPP_

#if defined(__GNUC__) && defined(DEBUG) && !defined(_MSC_VER)
#include <bfd.h>
#include <cxxabi.h>

#include <basis/ext/pattern.hpp>
#include <basis/simstd/vector>

namespace traceback {

	void bfd_fill(const char*& file, const char*& func, size_t& line, const wchar_t* image, void* address);

	struct Bfd
	{
		struct Info {
			asymbol**   symbol;
			bfd_vma     counter;
			const char* file;
			const char* func;
			unsigned    line;
			size_t      displacement;
		};

		struct Context: pattern::Uncopyable
		{
			~Context();
			Context(const wchar_t* image);

			Context(Context && other);

			void find(const char*& file, const char*& func, size_t& line, void* address);

			static void lookup_section(bfd* abfd, asection* sec, void* opaque_data);

			const wchar_t* image;
			bfd*           handle;
			asymbol**      symbol;
		};

		~Bfd();

		Bfd();

		void fill(const char*& file, const char*& func, size_t& line, const wchar_t* image, void* address);

	private:
		simstd::vector<Context> contxts;
	};

}

#endif

#endif
