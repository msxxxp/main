#ifndef LIBSTL_SYSTEM_MEMORY_AUTO_ARRAY_HPP_
#define LIBSTL_SYSTEM_MEMORY_AUTO_ARRAY_HPP_

#include <system/memory.hpp>

namespace memory {

	template<typename Type>
	class auto_array: private pattern::Uncopyable {
		typedef auto_array this_type;
		typedef Type value_type;
		typedef Type * pointer;
		typedef const Type * const_pointer;
		typedef Type & reference;
		typedef const Type & const_reference;
		typedef size_t size_type;

	public:
		~auto_array()
		{
			memory::free(m_ptr);
		}

		explicit auto_array(size_type count, const_pointer data = nullptr) :
			m_ptr(memory::calloc<pointer>(count)), m_count(count)
		{
			if (data)
				simstd::uninitialized_copy(data, data + m_count, m_ptr);
		}

		auto_array(this_type && rhs) :
			m_ptr(nullptr), m_count(0)
		{
			swap(rhs);
		}

		this_type & operator =(this_type && rhs)
		{
			if (this != &rhs)
				this_type(simstd::move(rhs)).swap(*this);
			return *this;
		}

		void reserve(size_type new_count)
		{
			if (size() < new_count) {
				memory::realloc(m_ptr, new_count * sizeof(value_type), HEAP_ZERO_MEMORY);
				m_count = new_count;
			}
		}

		size_type size() const
		{
			return m_count;
		}

		size_type size_in_bytes() const
		{
			return m_count * sizeof(value_type);
		}

		operator pointer() const
		{
			return m_ptr;
		}

		pointer data()
		{
			return m_ptr;
		}

		const_pointer data() const
		{
			return m_ptr;
		}

		reference operator [](int ind)
		{
			return m_ptr[ind];
		}

		const_reference operator [](int ind) const
		{
			return m_ptr[ind];
		}

		bool operator ==(const this_type & rhs) const
		{
			return (m_count == rhs.m_count) ? simstd::equal(m_ptr, m_ptr + m_count, rhs.m_ptr) : false;
		}

		void detach(pointer & ptr, size_t & size)
		{
			ptr = m_ptr;
			size = m_count;
			m_ptr = nullptr;
			m_count = 0;
		}

		void swap(this_type & rhs)
		{
			using simstd::swap;
			swap(m_ptr, rhs.m_ptr);
			swap(m_count, rhs.m_count);
		}

	private:
		pointer m_ptr;
		size_type m_count;
	};

	template<typename Type>
	void swap(auto_array<Type> & b1, auto_array<Type> & b2)
	{
		b1.swap(b2);
	}

}

#endif
