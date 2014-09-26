#ifndef BASIS_MEMORY_AUTO_PTR_HPP_
#define BASIS_MEMORY_AUTO_PTR_HPP_

namespace simstd {

	template<typename Type>
	class auto_ptr {
	public:
		typedef Type element_type;

		~auto_ptr() throw();

		template<typename U>
		auto_ptr(auto_ptr<U>& a) throw();
		auto_ptr(auto_ptr& a) throw();
		explicit auto_ptr(element_type* p = 0) throw();

		template<typename U>
		auto_ptr& operator =(auto_ptr<U>& a) throw();
		auto_ptr& operator =(auto_ptr& a) throw();

		element_type& operator *() const throw();
		element_type* operator->() const throw();
		element_type* get() const throw();
		element_type* release() throw();

		void reset(element_type* p = 0) throw();

		template<typename U>
		operator auto_ptr<U>() throw()
		{
			return auto_ptr<U>(this->release());
		}

	private:
		Type* m_ptr;
	};

	template<typename Type>
	auto_ptr<Type>::~auto_ptr() throw()
	{
		delete m_ptr;
	}

	template<typename Type>
	auto_ptr<Type>::auto_ptr(element_type* p) throw() :
		m_ptr(p)
	{
	}

	template<typename Type>
	auto_ptr<Type>::auto_ptr(auto_ptr& a) throw() :
		m_ptr(a.release())
	{
	}

	template<typename Type>
	template<typename U>
	auto_ptr<Type>::auto_ptr(auto_ptr<U>& a) throw() :
		m_ptr(a.release())
	{
	}

	template<typename Type>
	auto_ptr<Type>& auto_ptr<Type>::operator =(auto_ptr& a) throw()
	{
		reset(a.release());
		return *this;
	}

	template<typename Type>
	template<typename U>
	auto_ptr<Type>& auto_ptr<Type>::operator =(auto_ptr<U>& a) throw()
	{
		reset(a.release());
		return *this;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type& auto_ptr<Type>::operator *() const throw()
	{
		return *m_ptr;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type* auto_ptr<Type>::operator->() const throw()
	{
		return m_ptr;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type* auto_ptr<Type>::get() const throw()
	{
		return m_ptr;
	}

	template<typename Type>
	typename auto_ptr<Type>::element_type* auto_ptr<Type>::release() throw()
	{
		element_type* tmp = m_ptr;
		m_ptr = 0;
		return tmp;
	}

	template<typename Type>
	void auto_ptr<Type>::reset(element_type* p) throw()
	{
		if (p != m_ptr) {
			delete m_ptr;
			m_ptr = p;
		}
	}

}

#endif
