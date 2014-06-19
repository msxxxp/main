#ifndef LIBBASE_SHARED_PTR_HPP_
#define LIBBASE_SHARED_PTR_HPP_

#include <libbase/std.hpp>
#include <libbase/pvt/ref_cnt.hpp>
#include <algorithm>

namespace Base {

	template<typename Type>
	struct default_delete
	{
		void operator ()(Type * ptr) const
		{
			delete ptr;
		}
	};

	template<typename Type>
	struct default_delete<Type []>
	{
		void operator ()(Type * ptr) const
		{
			delete [] ptr;
		}
	};

	template<typename Type>
	class shared_ptr {
		typedef Type element_type;
		typedef shared_ptr this_type;

	public:
		typedef size_t size_type;

		~shared_ptr() noexcept
		{
			reset();
		}

		shared_ptr() noexcept :
			m_impl(nullptr)
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

		shared_ptr(const this_type & sh_ptr) :
			m_impl(nullptr)
		{
			if (sh_ptr.m_impl) {
				m_impl = sh_ptr.m_impl;
				m_impl->increase_ref();
			}
		}

		this_type & operator =(const this_type & sh_ptr)
		{
			if (m_impl != sh_ptr.m_impl) {
				shared_ptr(sh_ptr).swap(*this);
			}
			return *this;
		}

		template<typename newType>
		operator shared_ptr<newType>()
		{
			return shared_ptr<newType>(m_impl);
		}

		void reset()
		{
			if (m_impl) {
				m_impl->decrease_ref();
				m_impl = nullptr;
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
			return (m_impl) ? m_impl->get() : nullptr;
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

		void swap(this_type & other) noexcept
		{
			using std::swap;
			swap(m_impl, other.m_impl);
		}

	private:
		typedef typename Base::default_delete<Type> default_deleter;

		struct shared_ptr_impl: public ref_counter {
			shared_ptr_impl(element_type * ptr) : m_ptr(ptr) {}

			element_type * get() const {return m_ptr;}

		private:
			void destroy() const override {default_deleter()(m_ptr);}

			void deallocate() const override {delete this;}

			element_type * m_ptr;
		};

		template<typename Deleter>
		struct shared_ptr_impl_deleter: public shared_ptr_impl {
			shared_ptr_impl_deleter(element_type * ptr, Deleter d) : shared_ptr_impl(ptr), m_deleter(d) {}

		private:
			void destroy() const override {m_deleter(get());}

			Deleter m_deleter;
		};

		shared_ptr_impl * m_impl;
	};

	template<class T, class U>
	bool operator ==(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() == b.get();
	}

	template<class T, class U>
	bool operator !=(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() != b.get();
	}

	template<class T, class U>
	bool operator <(const shared_ptr<T> & a, const shared_ptr<U> & b)
	{
		return a.get() < b.get();
	}

	template<class T>
	void swap(shared_ptr<T> & a, shared_ptr<T> & b) noexcept
	{
		a.swap(b);
	}

	template<class T, class ... Args>
	shared_ptr<T> make_shared(Args&&... args)
	{
		return shared_ptr<T>(new T(args...));
	}
}

#endif
