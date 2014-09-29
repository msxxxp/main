#ifndef BASIS_SYS_MEMORY_AUTO_DESTROY_HPP_
#define BASIS_SYS_MEMORY_AUTO_DESTROY_HPP_

#include <basis/sys/memory.hpp>

namespace memory {

	template<typename Pointer>
	struct auto_destroy: private pattern::Uncopyable {
		~auto_destroy()
		{
			if (m_ptr)
				m_ptr->destroy();
		}

		auto_destroy(Pointer ptr) :
			m_ptr(ptr)
		{
		}

		auto_destroy(auto_destroy && right) :
			m_ptr(nullptr)
		{
			swap(right);
		}

		auto_destroy & operator =(auto_destroy && right)
		{
			auto_destroy(simstd::move(right)).swap(*this);
			return *this;
		}

		Pointer operator ->()
		{
			return m_ptr;
		}

		const Pointer operator ->() const
		{
			return m_ptr;
		}

		void swap(auto_destroy & right)
		{
			using simstd::swap;
			swap(m_ptr, right.m_ptr);
		}

	private:
		Pointer m_ptr;
	};

}

#endif
