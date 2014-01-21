#ifndef LIBSTL_SYSTEM_MEMORY_HPP_
#define LIBSTL_SYSTEM_MEMORY_HPP_

#include <system/configure.hpp>
#include <simstl/algorithm>
#include <simstl/memory>
#include <extra/pattern.hpp>
#include <system/sub_memory/memory.hpp>

#include <algorithm> // TODO

namespace memory {

	template<typename Type>
	inline Type & reverse_bytes(Type & inout)
	{
		simstd::reverse((char*)&inout, ((char*)&inout) + sizeof(inout));
		return inout;
	}

	inline WORD & swap_bytes(WORD & inout)
	{
		inout = static_cast<WORD>(inout >> 8 | inout << 8);
		return inout;
	}

	inline DWORD & swap_words(DWORD &inout)
	{
		inout = inout >> 16 | inout << 16;
		return inout;
	}

	inline intmax_t mega_to_bytes(size_t in)
	{
		return (in != 0) ? (intmax_t)in << 20 : -1ll;
	}

	inline size_t bytes_to_mega(intmax_t in)
	{
		return (in > 0) ? in >> 20 : 0;
	}

	///▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ constraints
	template<typename Type>
	struct must_be_pointer {
		static bool constraints(const Type & type_is_not_pointer)
		{
			return (bool)sizeof(0[type_is_not_pointer]);
		}
	};

	template<>
	struct must_be_pointer<PVOID> {
		static bool constraints(const PVOID &)
		{
			return true;
		}
	};

	///==================================================================================== auto_buf
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
				this_type(rhs).swap(*this);
			return *this;
		}

		void reserve(size_type nsize)
		{
			if (size() < nsize) {
				memory::realloc(m_ptr, nsize);
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

	///==================================================================================== auto_buf
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
				this_type(std::move(rhs)).swap(*this);
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

	///================================================================================== auto_close
	template<typename Type>
	class auto_close: private pattern::Uncopyable {
		typedef Type value_type;

	public:
		~auto_close()
		{
			delete m_impl;
		}

		explicit auto_close(value_type ptr) :
			m_impl(new auto_close_impl(ptr))
		{
		}

		template<typename Deleter>
		explicit auto_close(value_type ptr, Deleter del) :
			m_impl(new auto_close_deleter_impl<Deleter>(ptr, del))
		{
		}

		operator value_type()
		{
			return m_impl->m_ptr;
		}

		value_type * operator &()
		{
			return &(m_impl->m_ptr);
		}

		void swap(auto_close & rhs)
		{
			using simstd::swap;
			swap(m_impl, rhs.m_impl);
		}

	private:
		struct auto_close_impl {
			auto_close_impl(value_type ptr) :
				m_ptr(ptr)
			{
			}

			virtual ~auto_close_impl()
			{
//				delete m_ptr;
			}
			value_type m_ptr;
		};

		template<typename Deleter>
		struct auto_close_deleter_impl: public auto_close_impl {
			auto_close_deleter_impl(value_type ptr, Deleter d) :
				auto_close_impl(ptr), m_deleter(d)
			{
			}
			virtual ~auto_close_deleter_impl()
			{
				m_deleter(this->m_ptr);
//				this->m_ptr = nullptr;
			}
			Deleter m_deleter;
		};

		auto_close_impl * m_impl;
	};

	template<typename Type>
	inline void swap(auto_close<Type> & b1, auto_close<Type> & b2)
	{
		b1.swap(b2);
	}

	template<>
	struct auto_close<HANDLE> {
		typedef auto_close this_type;
		typedef HANDLE value_type;

		~auto_close()
		{
			close();
		}

		explicit auto_close(value_type hndl = nullptr) :
			m_hndl(hndl)
		{
		}

		auto_close(const this_type & rhs);

		auto_close(this_type && rhs);

		this_type & operator =(const this_type & rhs);

		this_type & operator =(this_type && rhs);

		value_type * operator &();

		operator value_type() const
		{
			return m_hndl;
		}

		value_type data() const
		{
			return m_hndl;
		}

		bool is_valid() const
		{
			return m_hndl && m_hndl != INVALID_HANDLE_VALUE;
		}

		void close();

		void swap(auto_close & rhs)
		{
			using simstd::swap;
			swap(m_hndl, rhs.m_hndl);
		}

	private:
		value_type m_hndl;
	};

	inline void swap(auto_close<HANDLE> & b1, auto_close<HANDLE> & b2)
	{
		b1.swap(b2);
	}

	///================================================================================ auto_destroy
	template<typename Pointer>
	struct auto_destroy: private pattern::Uncopyable {
		~auto_destroy()
		{
			m_ptr->destroy();
		}

		auto_destroy(Pointer ptr) :
			m_ptr(ptr)
		{
		}

		auto_destroy(auto_destroy && right) :
			m_ptr(nullptr)
		{
			swap(right);
		}

		auto_destroy & operator =(auto_destroy && right)
		{
			auto_destroy(std::move(right)).swap(*this);
			return *this;
		}

		Pointer operator ->()
		{
			return m_ptr;
		}

		const Pointer operator ->() const
		{
			return m_ptr;
		}

		void swap(auto_destroy & right)
		{
			using simstd::swap;
			swap(m_ptr, right.m_ptr);
		}

	private:
		Pointer m_ptr;
	};

}

#endif
