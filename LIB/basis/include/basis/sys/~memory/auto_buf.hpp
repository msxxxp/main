#ifndef BASIS_SYS_MEMORY_AUTO_BUF_HPP_
#define BASIS_SYS_MEMORY_AUTO_BUF_HPP_

namespace memory {

	template<typename Type>
	struct auto_buf: private pattern::Uncopyable {
		typedef auto_buf this_type;
		typedef Type value_type;
		typedef size_t size_type;

		~auto_buf()
		{
			must_be_pointer<Type>::constraints(m_ptr);
			HostFree(memory::heap::DefaultStat, m_ptr);
		}

		auto_buf() :
			m_ptr(nullptr)
		{
		}

		explicit auto_buf(size_type size_bytes) :
			m_ptr(static_cast<value_type>(HostAlloc(memory::heap::DefaultStat, size_bytes)))
		{
		}

		auto_buf(this_type&& other) :
			m_ptr(nullptr)
		{
			swap(other);
		}

		this_type& operator =(this_type&& other)
		{
			if (m_ptr != other.m_ptr)
				this_type(simstd::move(other)).swap(*this);
			return *this;
		}

		void reserve(size_type new_size_bytes)
		{
			if (size() < new_size_bytes) {
				memory::realloc(m_ptr, new_size_bytes);
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
		{ // FIXME
			return m_ptr;
		}

		void attach(value_type& ptr)
		{ //  FIXME
			memory::free(m_ptr);
			m_ptr = ptr;
		}

		void detach(value_type& ptr)
		{
			ptr = m_ptr;
			m_ptr = nullptr;
		}

		void swap(value_type& ptr) noexcept
		{
			using simstd::swap;
			swap(m_ptr, ptr);
		}

		void swap(this_type& other) noexcept
		{
			using simstd::swap;
			swap(m_ptr, other.m_ptr);
		}

	private:
		value_type m_ptr;
	};

	template<typename Type>
	void swap(auto_buf<Type>& b1, auto_buf<Type>& b2)
	{
		b1.swap(b2);
	}

}

#endif
