#ifndef BASIS_SYS_MEMORY_AUTO_BUF_HPP_
#define BASIS_SYS_MEMORY_AUTO_BUF_HPP_

#include <basis/sys/memory.hpp>

namespace memory {

	template<typename Type>
	struct auto_buf: private pattern::Uncopyable {
		typedef auto_buf this_type;
		typedef Type value_type;
		typedef size_t size_type;

		~auto_buf()
		{
			must_be_pointer<Type>::constraints(m_ptr);
			memory::free(m_ptr);
		}

		auto_buf() :
			m_ptr(nullptr)
		{
		}

		explicit auto_buf(size_type size) :
			m_ptr(memory::malloc<value_type>(size))
		{
		}

		auto_buf(this_type && rhs) :
			m_ptr(nullptr)
		{
			swap(rhs);
		}

		this_type & operator =(this_type && rhs)
		{
			if (this != &rhs)
				this_type(simstd::move(rhs)).swap(*this);
			return *this;
		}

		void reserve(size_type new_size)
		{
			if (size() < new_size) {
				memory::realloc(m_ptr, new_size);
			}
		}

		size_type size() const
		{
			return memory::size(m_ptr);
		}

		value_type operator &() const
		{
			return m_ptr;
		}

		value_type operator ->() const
		{
			return m_ptr;
		}

		operator value_type() const
		{
			return m_ptr;
		}

		value_type data() const
		{
			return m_ptr;
		}

		bool operator !() const
		{
			return m_ptr;
		}

		void attach(value_type & ptr)
		{
			memory::free(m_ptr);
			m_ptr = ptr;
		}

		void detach(value_type & ptr)
		{
			ptr = m_ptr;
			m_ptr = nullptr;
		}

		void swap(value_type & ptr) noexcept
		{
			using simstd::swap;
			swap(m_ptr, ptr);
		}

		void swap(this_type & rhs) noexcept
		{
			using simstd::swap;
			swap(m_ptr, rhs.m_ptr);
		}

	private:
		value_type m_ptr;
	};

	template<typename Type>
	void swap(auto_buf<Type> & b1, auto_buf<Type> & b2)
	{
		b1.swap(b2);
	}

}

#endif
