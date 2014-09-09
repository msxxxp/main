#include <basis/std/list>

namespace simstd {

	namespace pvt {

		List_node_base::List_node_base() noexcept:
			m_prev(),
			m_next()
		{
		}

		List_node_base::List_node_base(this_type * prev, this_type * next) noexcept:
			m_prev(prev),
			m_next(next)
		{
		}

		void List_node_base::transfer(this_type * const first, this_type * const last) noexcept
		{
		}

		void List_node_base::reverse() noexcept
		{
		}

		void List_node_base::hook(this_type * const position) noexcept
		{
		}

		void List_node_base::unhook() noexcept
		{
		}

		void List_node_base::swap(this_type & a, this_type & b) noexcept
		{
		}

	}

}

#endif
