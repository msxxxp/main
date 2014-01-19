#include <simstl/iosfwd>
#include <simstl/algorithm>
#include <simstl/functional>
#include <simstl/iterator>
#include <simstl/memory>
#include <simstl/new>
#include <simstl/numeric>
#include <simstl/string>
#include <simstl/utility>
#include <simstl/vector>

#include <cstdio>
#include <cstring>
#include <cwchar>

int main()
{
	simstd::Test::a_string(std::printf);
	simstd::Test::w_string(std::wprintf);
	simstd::Test::_vector(std::printf);
	simstd::Test::_algorithm(std::wprintf);
}
