#ifndef BASIS_SYS_MEMORY_AUTO_ARRAY_HPP_
#define BASIS_SYS_MEMORY_AUTO_ARRAY_HPP_

namespace memory {

	template<typename Type>
	class auto_array: private pattern::Uncopyable {
		typedef auto_array this_type;
		typedef Type value_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef size_t size_type;

	public:
		~auto_array()
		{
			memory::free(m_ptr);
		}

		explicit auto_array(size_type count) :
			m_ptr(memory::calloc<pointer>(count)),
			m_count(count)
		{
		}

		auto_array(this_type&& other) :
			m_ptr(nullptr),
			m_count(0)
		{
			swap(other);
		}

		this_type& operator =(this_type&& other)
		{
			if (m_ptr != other.m_ptr)
				this_type(simstd::move(other)).swap(*this);
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

		void detach(pointer& ptr, size_t& size)
		{
			ptr = m_ptr;
			size = m_count;
			m_ptr = nullptr;
			m_count = 0;
		}

		void swap(this_type & other)
		{
			using simstd::swap;
			swap(m_ptr, other.m_ptr);
			swap(m_count, other.m_count);
		}

	private:
		pointer m_ptr;
		size_type m_count;
	};

	template<typename Type>
	void swap(auto_array<Type>& b1, auto_array<Type>& b2)
	{
		b1.swap(b2);
	}

}

#endif
