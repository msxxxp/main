#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		List_node_base::List_node_base() noexcept:
			m_next(),
			m_prev()
		{
		}

		List_node_base::List_node_base(this_type * prev, this_type * next) noexcept:
			m_next(next),
			m_prev(prev)
		{
		}

		void List_node_base::transfer(this_type * const /*first*/, this_type * const /*last*/) noexcept
		{
		}

		void List_node_base::reverse() noexcept
		{
		}

		void List_node_base::hook(const this_type * pos) noexcept
		{
			this->m_next = const_cast<this_type*>(pos);
			this->m_prev = pos->m_prev;
			this->m_prev->m_next = this;
			this->m_next->m_prev = this;
		}

		void List_node_base::unhook() noexcept
		{
		}

		void List_node_base::swap(this_type & a, this_type & b) noexcept
		{
			LogTrace();
			using simstd::swap;
			swap(a.m_next, b.m_next);
			swap(a.m_prev, b.m_prev);
			LogDebug(L"&a: %p\n", &a);
			LogDebug(L"&a.m_prev: %p\n", a.m_next);
			LogDebug(L"&a.m_next: %p\n", a.m_prev);
			LogDebug(L"&b: %p\n", &b);
			LogDebug(L"&b.m_prev: %p\n", b.m_next);
			LogDebug(L"&b.m_next: %p\n", b.m_prev);
		}

	}

}
