#ifndef _LIBBASE_REF_CNT_HPP_
#define _LIBBASE_REF_CNT_HPP_

#include <libbase/std.hpp>

namespace Base {

	struct ref_counter {
		virtual ~ref_counter();

		ref_counter();

		void increase_ref() {++m_refcnt;}

		void decrease_ref();

		void mark_unshareable() {m_shareable = false;}

		bool is_shareable() const {return m_shareable;}

		bool is_shared() const {return m_refcnt > 1;}

		size_t count() const {return m_refcnt;}

	private:
		virtual void cleenup() const;

		virtual void free() const = 0;

		ref_counter(const ref_counter & rhs);

		ref_counter & operator = (const ref_counter & rhs);

	private:
		size_t m_refcnt;
		size_t m_shareable:1;
	};

//	template<class Type>
//	struct rc_ptr {
//		~rc_ptr()
//		{
//			static_assert(std::is_base_of<ref_counter, Type>::value, "Type must be derived from ref_counter");
//			if (m_ptr)
//				m_ptr->del_ref();
//		}
//
//		rc_ptr(Type* ptr = nullptr) :
//			m_ptr(ptr)
//		{
//			init();
//		}
//
//		rc_ptr(const rc_ptr &rhs) :
//			m_ptr(rhs.m_ptr)
//		{
//			init();
//		}
//
//		rc_ptr& operator=(const rc_ptr &rhs)
//		{
//			if (m_ptr != rhs.m_ptr) {
//				if (m_ptr)
//					m_ptr->del_ref();
//				m_ptr = rhs.m_ptr;
//				init();
//			}
//			return *this;
//		}
//
//		Type* operator->() const
//		{
//			return m_ptr;
//		}
//
//		Type& operator*() const
//		{
//			return *m_ptr;
//		}
//
//	private:
//		void init()
//		{
//			if (!m_ptr)
//				return;
//			if (!m_ptr->is_shareable())
//				m_ptr = new Type(*m_ptr);
//			m_ptr->add_ref();
//		}
//
//		Type* m_ptr;
//	};

}

#endif
