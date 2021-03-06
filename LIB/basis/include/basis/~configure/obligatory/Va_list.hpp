﻿#ifndef BASIS_CONFIGURE_OBLIGATORY_VA_LIST_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_VA_LIST_HPP_

struct Va_list {
	~Va_list();

	operator va_list& ()
	{
		return m_args;
	}

	operator va_list* ()
	{
		return &m_args;
	}

	va_list* operator &()
	{
		return &m_args;
	}
	
	Va_list& operator =(va_list vl)
	{
		m_args = vl;
		return *this;
	}

private:
	va_list m_args;
};

#endif
