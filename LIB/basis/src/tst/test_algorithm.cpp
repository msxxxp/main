#include <basis/std/algorithm>

#include <cassert>

namespace {
	template <typename ForwardIt, typename PrintFunc>
	void print(ForwardIt first, ForwardIt last, PrintFunc printFunc)
	{
		for (auto it = first; it != last; ++it)
		{
			printFunc(L"%Id ", *it);
		}
		printFunc(L"\n");
	}
}

ssize_t simstd::Test::_algorithm(simstd::Test::wPrintFunc printFunc)
{
	ssize_t dim1[] = {-10, 4, 52, -22, 0, 4, 0, 0, 31, -4, 15};
	size_t size = sizeof(dim1) / sizeof(dim1[0]);

	auto first = dim1;
	auto last = dim1 + size;


	printFunc(L"Test sort:\n");
	printFunc(L"before: ");
	print(first, last, printFunc);
	simstd::sort(first, last);

	printFunc(L"after: ");
	print(first, last, printFunc);

	printFunc(L"Test binary search:\n");
	printFunc(L"[-1000]: %d\n", simstd::binary_search(first, last, -1000));
	printFunc(L"[+1000]: %d\n", simstd::binary_search(first, last, +1000));
	printFunc(L"[0]: %d\n", simstd::binary_search(first, last, 0));
	printFunc(L"[52]: %d\n", simstd::binary_search(first, last, 52));
	printFunc(L"[-22]: %d\n", simstd::binary_search(first, last, -22));
	printFunc(L"[53]: %d\n", simstd::binary_search(first, last, 53));
	printFunc(L"[-23]: %d\n", simstd::binary_search(first, last, -23));

	printFunc(L"Test lower bound\n");
	printFunc(L"[-1000]: %Id\n", simstd::lower_bound(first, last, -1000) - first);
	printFunc(L"[+1000]: %Id\n", simstd::lower_bound(first, last, +1000) - first);
	printFunc(L"[0]: %Id\n", simstd::lower_bound(first, last, 0) - first);
	printFunc(L"[52]: %Id\n", simstd::lower_bound(first, last, 52) - first);
	printFunc(L"[-22]: %Id\n", simstd::lower_bound(first, last, -22) - first);
	printFunc(L"[53]: %Id\n", simstd::lower_bound(first, last, 53) - first);
	printFunc(L"[-23]: %Id\n", simstd::lower_bound(first, last, -23) - first);

	printFunc(L"Test upper bound\n");
	printFunc(L"[-1000]: %Id\n", simstd::upper_bound(first, last, -1000) - first);
	printFunc(L"[+1000]: %Id\n", simstd::upper_bound(first, last, +1000) - first);
	printFunc(L"[0]: %Id\n", simstd::upper_bound(first, last, 0) - first);
	printFunc(L"[52]: %Id\n", simstd::upper_bound(first, last, 52) - first);
	printFunc(L"[-22]: %Id\n", simstd::upper_bound(first, last, -22) - first);
	printFunc(L"[53]: %Id\n", simstd::upper_bound(first, last, 53) - first);
	printFunc(L"[-23]: %Id\n", simstd::upper_bound(first, last, -23) - first);

	return 0;
}
