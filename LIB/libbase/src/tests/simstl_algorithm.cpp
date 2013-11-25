#include <libbase/console.hpp>
#include <liblog/logger.hpp>

#include <simstl/algorithm>

#include <cassert>

namespace {
	template <typename ForwardIt>
	void print(ForwardIt first, ForwardIt last)
	{
		for (auto it = first; it != last; ++it)
		{
			Base::Console::printf(L"%Id ", *it);
		}
		Base::Console::printf(L"\n");
	}
}

ssize_t sarastd::algorithm_test()
{
	LogTrace();

	ssize_t dim1[] = {-10, 4, 52, -22, 0, 4, 0, 0, 31, -4, 15};
	size_t size = Base::lengthof(dim1);

	auto first = dim1;
	auto last = dim1 + size;


	LogForce(L"Test sort:\n");
	LogForce(L"before: ");
	print(first, last);
	sarastd::sort(first, last);

	LogForce(L"after: ");
	print(first, last);

	LogForce(L"Test binary search:\n");
	LogForce(L"[-1000]: %d\n", sarastd::binary_search(first, last, -1000));
	LogForce(L"[+1000]: %d\n", sarastd::binary_search(first, last, +1000));
	LogForce(L"[0]: %d\n", sarastd::binary_search(first, last, 0));
	LogForce(L"[52]: %d\n", sarastd::binary_search(first, last, 52));
	LogForce(L"[-22]: %d\n", sarastd::binary_search(first, last, -22));
	LogForce(L"[53]: %d\n", sarastd::binary_search(first, last, 53));
	LogForce(L"[-23]: %d\n", sarastd::binary_search(first, last, -23));

	LogForce(L"Test lower bound\n");
	LogForce(L"[-1000]: %Id\n", sarastd::lower_bound(first, last, -1000) - first);
	LogForce(L"[+1000]: %Id\n", sarastd::lower_bound(first, last, +1000) - first);
	LogForce(L"[0]: %Id\n", sarastd::lower_bound(first, last, 0) - first);
	LogForce(L"[52]: %Id\n", sarastd::lower_bound(first, last, 52) - first);
	LogForce(L"[-22]: %Id\n", sarastd::lower_bound(first, last, -22) - first);
	LogForce(L"[53]: %Id\n", sarastd::lower_bound(first, last, 53) - first);
	LogForce(L"[-23]: %Id\n", sarastd::lower_bound(first, last, -23) - first);

	LogForce(L"Test upper bound\n");
	LogForce(L"[-1000]: %Id\n", sarastd::upper_bound(first, last, -1000) - first);
	LogForce(L"[+1000]: %Id\n", sarastd::upper_bound(first, last, +1000) - first);
	LogForce(L"[0]: %Id\n", sarastd::upper_bound(first, last, 0) - first);
	LogForce(L"[52]: %Id\n", sarastd::upper_bound(first, last, 52) - first);
	LogForce(L"[-22]: %Id\n", sarastd::upper_bound(first, last, -22) - first);
	LogForce(L"[53]: %Id\n", sarastd::upper_bound(first, last, 53) - first);
	LogForce(L"[-23]: %Id\n", sarastd::upper_bound(first, last, -23) - first);

	return 0;
}
