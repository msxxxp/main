#ifndef LIBSTL_SYSTEM_MEMORY_AUTO_CLOSE_HPP_
#define LIBSTL_SYSTEM_MEMORY_AUTO_CLOSE_HPP_

#include <system/memory.hpp>

namespace memory {

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

}

#endif
