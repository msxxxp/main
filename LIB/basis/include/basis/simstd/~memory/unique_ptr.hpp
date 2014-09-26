#ifndef BASIS_MEMORY_UNIQUE_PTR_HPP_
#define BASIS_MEMORY_UNIQUE_PTR_HPP_

#include <basis/simstd/~memory/default_deleters.hpp>
#include <basis/simstd/~functional/functional.hpp>

#include <type_traits>

namespace simstd {

	template<typename Type, typename Deleter = default_delete<Type> >
	class unique_ptr {
		class Pointer {
			typedef typename std::remove_reference<Deleter>::type _Deleter;

			template<typename TypeI>
			static typename TypeI::pointer test_pointer(typename TypeI::pointer*);

			template<typename TypeI>
			static Type* test_pointer(...);

		public:
			typedef decltype(test_pointer<_Deleter>(0)) type;
		};

	public:
		typedef typename Pointer::type pointer;
		typedef Type                   element_type;
		typedef Deleter                deleter_type;

		~unique_ptr() noexcept
		{
			if (m_ptr != nullptr)
				get_deleter()(m_ptr);
			m_ptr = pointer();
		}

		constexpr unique_ptr() noexcept :
			m_ptr(),
			m_dltr(deleter_type())
		{
			static_assert(!std::is_pointer<deleter_type>::value, "constructed with null function pointer deleter");
		}

		explicit unique_ptr(pointer ptr) noexcept :
			m_ptr(ptr),
			m_dltr(deleter_type())
		{
			static_assert(!std::is_pointer<deleter_type>::value, "constructed with null function pointer deleter");
		}

		unique_ptr(pointer ptr, typename std::conditional<std::is_reference<deleter_type>::value, deleter_type, const deleter_type&>::type dltr) noexcept :
			m_ptr(ptr),
			m_dltr(dltr)
		{
		}

		unique_ptr(pointer ptr, typename std::remove_reference<deleter_type>::type&& dltr) noexcept :
			m_ptr(simstd::move(ptr)),
			m_dltr(simstd::move(dltr))
		{
			static_assert(!std::is_reference<deleter_type>::value, "rvalue deleter bound to reference");
		}

		constexpr unique_ptr(nullptr_t) noexcept :
			unique_ptr()
		{
		}

		unique_ptr(unique_ptr && other) noexcept :
			m_ptr(other.release()),
			m_dltr(simstd::forward<deleter_type>(other.get_deleter()))
		{
		}

//		template<typename _Up, typename _Ep, typename = std::_Require<std::is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>,std::__not_<std::is_array<_Up>>, typename std::conditional<std::is_reference<Deleter>::value, std::is_same<_Ep, Deleter>, std::is_convertible<_Ep, Deleter>>::type>>
		template<typename OType, typename ODeleter>
		unique_ptr(unique_ptr<OType, ODeleter> && other) noexcept :
			m_ptr(other.release()),
			m_dltr(simstd::forward<ODeleter>(other.get_deleter()))
		{
		}

		unique_ptr& operator =(unique_ptr && other) noexcept
		{
			reset(other.release());
			get_deleter() = simstd::forward<deleter_type>(other.get_deleter());
			return *this;
		}

//		template<typename _Up, typename _Ep>
//		typename enable_if< __and_<is_convertible<typename unique_ptr<_Up, _Ep>::pointer, pointer>, __not_<is_array<_Up>>>::value, unique_ptr&>::type
//		operator = (unique_ptr<_Up, _Ep>&& other) noexcept
//		{
//			reset(other.release());
//			get_deleter() = std::forward<_Ep>(other.get_deleter());
//			return *this;
//		}
		template<typename OType, typename ODeleter>
		unique_ptr & operator = (unique_ptr<OType, ODeleter> && other) noexcept
		{
			reset(other.release());
			get_deleter() = simstd::forward<ODeleter>(other.get_deleter());
			return *this;
		}

		unique_ptr & operator = (nullptr_t) noexcept
		{
			reset();
			return *this;
		}

		typename std::add_lvalue_reference<element_type>::type operator *() const
		{
			return *get();
		}

		pointer operator->() const noexcept
		{
			return get();
		}

		pointer get() const noexcept
		{
			return m_ptr;
		}

		deleter_type& get_deleter() noexcept
		{
			return m_dltr;
		}

		const deleter_type& get_deleter() const noexcept
		{
			return m_dltr;
		}

		explicit operator bool() const noexcept
		{
			return get() == pointer() ? false : true;
		}

		pointer release() noexcept
		{
			pointer ptr = get();
			m_ptr = pointer();
			return ptr;
		}

		void reset(pointer ptr = pointer()) noexcept
		{
			using simstd::swap;
			swap(m_ptr, ptr);
			if (ptr != pointer())
				get_deleter()(ptr);
		}

		void swap(unique_ptr& other) noexcept
		{
			using simstd::swap;
			swap(m_ptr, other.m_ptr);
			swap(m_dltr, other.m_dltr);
		}

	private:
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

		typename Pointer::type m_ptr;
		Deleter                m_dltr;
	};

	template<typename Type, typename Deleter>
	inline void swap(unique_ptr<Type, Deleter>& a, unique_ptr<Type, Deleter>& b) noexcept
	{
		a.swap(b);
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	inline bool operator ==(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return a.get() == b.get();
	}

	template<typename Type, typename Deleter>
	inline bool operator ==(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return !a;
	}

	template<typename Type, typename Deleter>
	inline bool operator ==(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return !b;
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	inline bool operator <(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		typedef typename std::common_type<typename unique_ptr<Type, Deleter>::pointer, typename unique_ptr<_Up, _Ep>::pointer>::type CommonType;
		return simstd::less<CommonType>()(a.get(), b.get());
	}

	template<typename Type, typename Deleter>
	inline bool operator <(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return simstd::less<typename unique_ptr<Type, Deleter>::pointer>()(a.get(), nullptr);
	}

	template<typename Type, typename Deleter>
	inline bool operator <(nullptr_t, const unique_ptr<Type, Deleter>& a)
	{
		return simstd::less<typename unique_ptr<Type, Deleter>::pointer>()(nullptr, a.get());
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	inline bool operator !=(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return a.get() != b.get();
	}

	template<typename Type, typename Deleter>
	inline bool operator !=(const unique_ptr<Type, Deleter>& a, nullptr_t) noexcept
	{
		return (bool)a;
	}

	template<typename Type, typename Deleter>
	inline bool operator !=(nullptr_t, const unique_ptr<Type, Deleter>& b) noexcept
	{
		return (bool)b;
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	inline bool operator <=(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return !(b < a);
	}

	template<typename Type, typename Deleter>
	inline bool operator <=(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return !(nullptr < a);
	}

	template<typename Type, typename Deleter>
	inline bool operator <=(nullptr_t, const unique_ptr<Type, Deleter>& a)
	{
		return !(a < nullptr);
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	inline bool operator >(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return (b < a);
	}

	template<typename Type, typename Deleter>
	inline bool operator >(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return simstd::less<typename unique_ptr<Type, Deleter>::pointer>()(nullptr, a.get());
	}

	template<typename Type, typename Deleter>
	inline bool operator >(nullptr_t, const unique_ptr<Type, Deleter>& a)
	{
		return simstd::less<typename unique_ptr<Type, Deleter>::pointer>()(a.get(), nullptr);
	}

	template<typename Type, typename Deleter, typename _Up, typename _Ep>
	inline bool operator >=(const unique_ptr<Type, Deleter>& a, const unique_ptr<_Up, _Ep>& b)
	{
		return !(a < b);
	}

	template<typename Type, typename Deleter>
	inline bool operator >=(const unique_ptr<Type, Deleter>& a, nullptr_t)
	{
		return !(a < nullptr);
	}

	template<typename Type, typename Deleter>
	inline bool operator >=(nullptr_t, const unique_ptr<Type, Deleter>& a)
	{
		return !(nullptr < a);
	}

}

#endif
