#include <basis/sys/configure.hpp>
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

#include <basis/std/iosfwd>
#include <basis/std/algorithm>
//#include <basis/std/chrono>
#include <basis/std/functional>
#include <basis/std/iterator>
#include <basis/std/memory>
#include <basis/std/mutex>
#include <basis/std/new>
#include <basis/std/numeric>
#include <basis/std/string>
#include <basis/std/utility>
//#include <basis/std/ratio>
#include <basis/std/vector>
//#include <basis/std/type_traits>

#include <basis/ext/bits.hpp>
#include <basis/ext/pattern.hpp>

int stub()
{
	cstr::length("qwerty");

	return 0;
}
