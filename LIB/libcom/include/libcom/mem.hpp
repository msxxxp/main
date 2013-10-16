#ifndef _LIBCOM_MEM_HPP_
#define _LIBCOM_MEM_HPP_

#include <libbase/std.hpp>

namespace Com {

	template<typename Type>
	struct CoMem: private Base::Uncopyable {
		~CoMem()
		{
			clean();
		}

		CoMem() :
			m_ptr(nullptr)
		{
		}

		CoMem(size_t size) :
			m_ptr(::CoTaskMemAlloc(size))
		{
		}

		operator Type() const
		{
			return m_ptr;
		}

		Type * operator &()
		{
			clean();
			return &m_ptr;
		}

		Type operator ->() const
		{
			return m_ptr;
		}

		void reserve(size_t size)
		{
			PVOID tmp(::CoTaskMemRealloc(m_ptr, size));
			if (tmp)
				m_ptr = tmp;
		}

	private:
		void clean()
		{
			::CoTaskMemFree(m_ptr);
		}

		Type m_ptr;
	};

}

#endif
