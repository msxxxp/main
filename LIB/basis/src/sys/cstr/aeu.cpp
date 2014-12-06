#include <basis/sys/cstr.hpp>
//#include <basis/sys/memory.hpp>

namespace cstr {

	aeu::~aeu()
	{
		memory::free(ptr);
	}

	aeu::aeu(const_pointer str) :
		ptr(cstr::dup(str))
	{
	}

	aeu::aeu(const_pointer str, size_type length) :
		ptr(cstr::dup(str, length))
	{
	}

	aeu::aeu(this_type&& other):
		ptr(nullptr)
	{
		swap(other);
	}

	aeu::this_type& aeu::operator =(this_type&& other)
	{
		this_type(simstd::move(other)).swap(*this);
		return *this;
	}

	void aeu::swap(this_type& other)
	{
		using simstd::swap;
		swap(ptr, other.ptr);
	}

	aeu::size_type aeu::size() const
	{
		return cstr::length(ptr);
	}

	aeu::size_type aeu::length() const
	{
		return cstr::length(ptr);
	}

	aeu::operator aeu::const_pointer() const
	{
		return ptr;
	}

	aeu::const_pointer aeu::c_str() const
	{
		return ptr;
	}

	aeu::const_reference aeu::operator [](size_type index) const
	{
		return ptr[index];
	}

	aeu::reference aeu::operator [](size_type index)
	{
		return ptr[index];
	}

}
