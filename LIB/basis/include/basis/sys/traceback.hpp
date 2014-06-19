#ifndef BASIS_SYS_TRACEBACK_HPP_
#define BASIS_SYS_TRACEBACK_HPP_

#include <basis/sys/configure.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/std/iosfwd>
#include <basis/std/vector>

namespace traceback {

	void init(const wchar_t * path = nullptr);

	struct Frame_i
	{
		virtual ~Frame_i();

		virtual void * address() const = 0;

		virtual const ustring & module() const = 0;

		virtual const ustring & file() const = 0;

		virtual const ustring & function() const = 0;

		virtual size_t line() const = 0;

		virtual size_t offset() const = 0;

		virtual ustring to_str() const = 0;
	};

	Frame_i * read_frame_data(void * address);

	struct LazyFrame: public Frame_i, private pattern::Uncopyable
	{
		~LazyFrame();

		LazyFrame(void * address);

		LazyFrame(LazyFrame && other);

		LazyFrame & operator = (LazyFrame && right);

		void swap(LazyFrame & other);

		void * address() const override;

		const ustring & module() const override;

		const ustring & file() const override;

		const ustring & function() const override;

		size_t line() const override;

		size_t offset() const override;

		ustring to_str() const override;

	private:
		void init_data() const;

		void * m_address;
		mutable Frame_i * m_data;
	};

	struct Enum: private simstd::vector<LazyFrame> {
		typedef simstd::vector<LazyFrame> base_type;

		Enum(size_t depth = get_max_depth());

		Enum(PCONTEXT context, void* address = nullptr, size_t depth = get_max_depth());

		static size_t get_max_depth();

		using base_type::begin;
		using base_type::end;
		using base_type::empty;
		using base_type::size;
		using base_type::operator[];
	};
}

#endif
