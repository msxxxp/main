#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		List_node_base::List_node_base() noexcept:
			m_next(this),
			m_prev(this)
		{
		}

		void List_node_base::transfer(const this_type * first, const this_type * last) noexcept
		{
			this_type* first1 = m_prev;

			m_prev = last->m_prev;
			m_prev->m_next = this;

			first->m_prev->m_next = const_cast<this_type*>(last);
			const_cast<this_type*>(last)->m_prev = first->m_prev;

			first1->m_next = const_cast<this_type*>(first);
			const_cast<this_type*>(first)->m_prev = first1;
		}

		void List_node_base::reverse() noexcept
		{
			auto cur = this;
			do
			{
				auto next = cur->m_next;
				cur->m_next = cur->m_prev;
				cur->m_prev = next;
				cur         = cur->m_prev;
			}
			while(cur != this);
		}

		void List_node_base::hook(const this_type * pos) noexcept
		{
//			LogTraceObj();
			this_type * prev = pos->m_prev;
			this_type * next = const_cast<this_type*>(pos);
//			LogDebug(L"prev: %p\n", prev);
//			LogDebug(L"this: %p\n", this);
//			LogDebug(L"next: %p\n", next);
//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"pos->m_next: %p\n", next->m_next);
//			LogDebug(L"pos->m_prev: %p\n", next->m_prev);

			m_next = const_cast<this_type*>(next);
			m_prev = prev;
			prev->m_next = this;
			next->m_prev = this;

//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"pos->m_next: %p\n", next->m_next);
//			LogDebug(L"pos->m_prev: %p\n", next->m_prev);
		}

		void List_node_base::unhook() noexcept
		{
//			LogTraceObj();
			this_type * prev = m_prev;
			this_type * next = m_next;
//			LogDebug(L"prev: %p\n", prev);
//			LogDebug(L"this: %p\n", this);
//			LogDebug(L"next: %p\n", next);
//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"next->m_next: %p\n", next->m_next);
//			LogDebug(L"next->m_prev: %p\n", next->m_prev);

			prev->m_next = next;
			next->m_prev = prev;

//			LogDebug(L"prev->m_next: %p\n", prev->m_next);
//			LogDebug(L"prev->m_prev: %p\n", prev->m_prev);
//			LogDebug(L"this->m_next: %p\n", m_next);
//			LogDebug(L"this->m_prev: %p\n", m_prev);
//			LogDebug(L"next->m_next: %p\n", next->m_next);
//			LogDebug(L"next->m_prev: %p\n", next->m_prev);
		}

		void List_node_base::swap(this_type & a, this_type & b) noexcept
		{
//			LogTrace();
			using simstd::swap;
			swap(a.m_next, b.m_next);
			swap(a.m_prev, b.m_prev);

			if (a.m_next == &b)
				a.m_next = a.m_prev = &a;
			else
				a.m_next->m_prev = a.m_prev->m_next = &a;

			if(b.m_next == &a)
				b.m_next = b.m_prev = &b;
			else
				b.m_next->m_prev = b.m_prev->m_next = &b;

//			LogDebug(L"&a: %p\n", &a);
//			LogDebug(L"a.m_next: %p\n", a.m_next);
//			LogDebug(L"a.m_prev: %p\n", a.m_prev);
//			LogDebug(L"&b: %p\n", &b);
//			LogDebug(L"b.m_next: %p\n", b.m_next);
//			LogDebug(L"b.m_prev: %p\n", b.m_prev);
		}

	}

}
