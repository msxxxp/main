#include <system/configure.hpp>
#include <system/console.hpp>
#include <system/crt.hpp>
#include <system/cstr.hpp>
#include <system/memory.hpp>
#include <system/sync.hpp>

#include <simstd/iosfwd>
#include <simstd/algorithm>
#include <simstd/functional>
#include <simstd/iterator>
#include <simstd/memory>
#include <simstd/mutex>
#include <simstd/new>
#include <simstd/numeric>
#include <simstd/string>
#include <simstd/utility>
#include <simstd/vector>

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
