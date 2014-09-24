#ifndef BASIS_LIST_NODE_HPP_
#define BASIS_LIST_NODE_HPP_

#include <basis/simstd/list>
//#include <basis/sys/logger.hpp>

namespace simstd {

	namespace pvt {

		struct List_node_base {
			typedef List_node_base this_type;

			List_node_base() noexcept;

			void transfer(this_type * const first, this_type * const last) noexcept;

			void reverse() noexcept;

			void hook(const this_type * pos) noexcept;

			void unhook() noexcept;

			static void swap(this_type & a, this_type & b) noexcept;

		public:
			this_type * m_next;
			this_type * m_prev;
		};

		template<typename Type>
		struct List_node: public List_node_base {

			template<typename ... Args>
			List_node(Args &&... args);

		public:
			Type m_data;
		};


		template<typename Type>
		template<typename ... Args>
		List_node<Type>::List_node(Args &&... args) :
			m_data(simstd::forward<Args>(args)...)
		{
//			LogTraceObj();
		}

	}

}

#endif
