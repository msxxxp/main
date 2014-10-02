#include <basis/simstd/string>
#include <basis/simstd/memory>
#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=f;target=co");
	}
}

int main()
{
	typedef int TypeTag;
	typedef typename memory::HeapSpecialLogged<TypeTag> Heap;
	typedef typename simstd::AllocatorLogged<char, Heap> Allocator;
	typedef typename simstd::basic_string2<char, simstd::char_traits<char>, Allocator> tstring;

	setup_logger();

	LogTrace();

	Heap::init();

	using namespace simstd;

	tstring str1;

	return 0;
}
