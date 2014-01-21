#ifndef LIBSTL_MEMORY_SHARED_PTR_HPP_
#define LIBSTL_MEMORY_SHARED_PTR_HPP_

#include <extra/pattern.hpp>

namespace simstd {

	template<typename Type>
	struct default_delete
	{
		void operator ()(Type * ptr) const
		{
			delete ptr;
		}
	};

	template<typename Type>
	struct default_delete<Type[]>
	{
		void operator ()(Type * ptr) const
		{
			delete [] ptr;
		}
	};

	template<typename Type>
	class shared_ptr {
		typedef shared_ptr<Type> this_type;

	public:
		typedef Type element_type;
		typedef size_t size_type;

		~shared_ptr() throw()
		{
			reset();
		}

		shared_ptr() throw() :
			m_impl(0)
		{
		}

		explicit shared_ptr(element_type * ptr) :
			m_impl(new shared_ptr_impl(ptr))
		{
		}

		template<typename Deleter>
		shared_ptr(element_type * ptr, Deleter d) :
			m_impl(new shared_ptr_impl_deleter<Deleter>(ptr, d))
		{
		}

		shared_ptr(const this_type & other) :
			m_impl(0)
		{
			if (other.m_impl) {
				m_impl = other.m_impl;
				m_impl->increase_ref();
			}
		}

		this_type & operator =(const this_type & other)
		{
			if (m_impl != other.m_impl) {
				shared_ptr(other).swap(*this);
			}
			return *this;
		}

		template<typename newType>
		operator shared_ptr<newType>()
		{
			return shared_ptr<newType>(m_impl); // TODO
		}

		void reset()
		{
			if (m_impl) {
				m_impl->decrease_ref();
				m_impl = 0;
			}
		}

		void reset(element_type * ptr)
		{
			shared_ptr(ptr).swap(*this);
		}

		template<typename Deleter>
		void reset(element_type * ptr, Deleter d)
		{
			shared_ptr(ptr, d).swap(*this);
		}

		element_type & operator *() const
		{
			return *(m_impl->get());
		}

		element_type * operator ->() const
		{
			return m_impl->get();
		}

		element_type * get() const
		{
			return (m_impl) ? m_impl->get() : 0;
		}

		bool unique() const
		{
			return !m_impl || m_impl->count_ref() == 1;
		}

		size_type use_count() const
		{
			return (m_impl) ? m_impl->count_ref() : 0;
		}

		operator bool() const
		{
			return m_impl && m_impl->get();
		}

		void swap(this_type & b) throw()
		{
#if defined(__GNUC__) && (__GNUC__ < 3)
			simstd::swap(m_impl, b.m_impl);
#else
			using simstd::swap;
			swap(m_impl, b.m_impl);
#endif
		}

	private:
		typedef typename simstd::default_delete<Type> default_deleter;

		struct shared_ptr_impl: public pattern::RefCounter
		{
			shared_ptr_impl(element_type * ptr) : m_ptr(ptr) {}

			element_type * get() const {return m_ptr;}

		private:
			void destroy() const {default_deleter()(m_ptr);}

			void deallocate() const {delete this;}

			element_type * m_ptr;
		};

		template<typename Deleter>
		struct shared_ptr_impl_deleter: public shared_ptr_impl {
			shared_ptr_impl_deleter(element_type * ptr, Deleter d) : shared_ptr_impl(ptr), m_deleter(d) {}

		private:
			void destroy() const {m_deleter(get());}

			Deleter m_deleter;
		};

		shared_ptr_impl * m_impl;
	};

	template<typename T, typename U>
	bool operator ==(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() == b.get();
	}

	template<typename T, typename U>
	bool operator <(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() < b.get();
	}

	template<typename T, typename U>
	bool operator !=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return simstd::rel_ops::operator !=(a, b);
	}

	template<typename T, typename U>
	bool operator >(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return simstd::rel_ops::operator >(a, b);
	}

	template<typename T, typename U>
	bool operator <=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return simstd::rel_ops::operator <=(a, b);
	}

	template<typename T, typename U>
	bool operator >=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return simstd::rel_ops::operator >=(a, b);
	}

	template<typename T>
	void swap(shared_ptr<T> & a, shared_ptr<T> & b) throw()
	{
		a.swap(b);
	}

}

#endif
