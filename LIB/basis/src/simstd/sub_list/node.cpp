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

		void List_node_base::hook(this_type * const /*position*/) noexcept
		{
		}

		void List_node_base::unhook() noexcept
		{
		}

		void List_node_base::swap(this_type & /*a*/, this_type & /*b*/) noexcept
		{
		}

	}

}
