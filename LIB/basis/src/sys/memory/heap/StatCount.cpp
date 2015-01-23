#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Stat.hpp>

#include <basis/simstd/vector>

#ifndef NDEBUG
namespace {
	typedef simstd::AllocatorHeap<memory::heap::AllocatedItem, memory::heap::Default> Allocator;
	typedef simstd::vector<memory::heap::AllocatedItem, Allocator> vector_type;
}
#endif

memory::heap::AllocatedItem::AllocatedItem(void* ptr, uint64_t size, const char* function, int line) :
	size(size),
	ptr(ptr),
	function(function),
	line(line)
{
	TraceFunc();
}

memory::heap::StatCount::~StatCount()
{
	TraceFunc();
#ifndef NDEBUG
	reinterpret_cast<vector_type*>(database)->~vector();
	HostFree(memory::heap::Default, database);
#endif
}

memory::heap::StatCount::StatCount():
	allocations(),
	frees(),
	allocSize(),
	freeSize(),
	database()
{
	TraceFunc();
#ifndef NDEBUG
	database = HostAlloc(memory::heap::Default, sizeof(vector_type));
	new (database, simstd::nothrow) vector_type();
#endif
}

void* memory::heap::StatCount::commit_alloc(void* ptr, uint64_t size, const char* function, int line)
{
	TraceFunc();
	UNUSED(function);
	UNUSED(line);

	if (ptr) {
		++allocations;
		allocSize += size;

#ifndef NDEBUG
		reinterpret_cast<vector_type*>(database)->emplace_back(ptr, size, function, line);
#endif
	}

	return ptr;
}

void memory::heap::StatCount::commit_free(const void* ptr, uint64_t size, const char* function, int line)
{
	TraceFunc();
	UNUSED(function);
	UNUSED(line);

	if (ptr) {
		++frees;
		freeSize += size;
	}
}

void memory::heap::StatCount::get_database(AllocatedItem*& db, size_t& size) const
{
	db = nullptr;
	size = 0;
}

void memory::heap::StatCount::free_database(AllocatedItem* db) const
{
	delete db;
}

