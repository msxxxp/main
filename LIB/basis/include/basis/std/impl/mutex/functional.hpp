#ifndef BASIS_MUTEX_FUNCTIONAL_HPP_
#define BASIS_MUTEX_FUNCTIONAL_HPP_

#include <basis/std/impl/mutex/types.hpp>
#include <basis/std/impl/mutex/unique_lock.hpp>

#include <tuple>

namespace simstd {

	namespace pvt {
		template<typename Mutex>
		unique_lock<Mutex> try_to_lock(Mutex & l)
		{
			return unique_lock<Mutex>(l, simstd::try_to_lock);
		}

		template<int Index, bool Continue = true>
		struct try_lock_impl
		{
			template<typename ... Mutex>
			static int do_try_lock(std::tuple<Mutex & ...> & locks)
			{
				int idx = Index;
				auto lock = try_to_lock(std::get<Index>(locks));
				if (lock.owns_lock()) {
					idx = try_lock_impl<Index + 1, Index + 2 < sizeof...(Mutex)>::do_try_lock(locks);
					if (idx == -1)
					lock.release();
				}
				return idx;
			}
		};

		template<int Index>
		struct try_lock_impl<Index, false>
		{
			template<typename ... Mutex>
			static int do_try_lock(std::tuple<Mutex & ...> & locks)
			{
				int idx = Index;
				auto lock = try_to_lock(std::get<Index>(locks));
				if (lock.owns_lock()) {
					lock.release();
					idx = -1;
				}
				return idx;
			}
		};

	}

	template<typename Mutex1, typename Mutex2, typename ... Mutex3>
	int try_lock(Mutex1 & l1, Mutex2 & l2, Mutex3 & ... l3)
	{
		auto locks = std::tie(l1, l2, l3...);
		return pvt::try_lock_impl<0>::do_try_lock(locks);
	}

	template<typename Mutex1, typename Mutex2, typename ... Mutex3>
	void lock(Mutex1 & l1, Mutex2 & l2, Mutex3 & ... l3)
	{
		while (true) {
			unique_lock<Mutex1> first(l1);
			auto locks = std::tie(l2, l3...);
			if (pvt::try_lock_impl<0, sizeof...(Mutex3)>::do_try_lock(locks) == -1) {
				first.release();
				return;
			}
		}
	}

} // namespace simstd

#endif
