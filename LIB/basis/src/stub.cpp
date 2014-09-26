#include <basis/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/linkage.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/process.hpp>
#include <basis/sys/sstr.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/thread.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/sys/win64.hpp>

#include <basis/simstd/iosfwd>
#include <basis/simstd/algorithm>
//#include <basis/simstd/chrono>
#include <basis/simstd/functional>
#include <basis/simstd/iterator>
#include <basis/simstd/memory>
#include <basis/simstd/mutex>
#include <basis/simstd/new>
#include <basis/simstd/numeric>
#include <basis/simstd/string>
#include <basis/simstd/utility>
//#include <basis/simstd/ratio>
#include <basis/simstd/vector>
//#include <basis/simstd/type_traits>

#include <basis/ext/bits.hpp>
#include <basis/ext/pattern.hpp>

int stub()
{
	cstr::length("qwerty");

	return 0;
}
