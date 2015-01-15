#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Stat.hpp>

#include <basis/simstd/vector>

namespace {
	typedef simstd::AllocatorHeap<memory::heap::AllocatedItem, memory::heap::Default> Allocator;
	typedef simstd::vector<memory::heap::AllocatedItem, Allocator> vector_type;
}

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
}

memory::heap::StatCount::StatCount():
	allocations(),
	frees(),
	allocSize(),
	freeSize(),
	database()
{
//#ifndef NDEBUG
	TraceFunc();
//	database = new vector_type;
//#endif
}

void* memory::heap::StatCount::commit_alloc(void* ptr, uint64_t size, const char* function, int line)
{
	TraceFunc();
	UNUSED(function);
	UNUSED(line);

	if (ptr) {
		++allocations;
		allocSize += size;

//		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//#ifndef NDEBUG
//		printf("%s:%d size: %Iu\n", __PRETTY_FUNCTION__, __LINE__, (unsigned)reinterpret_cast<vector_type*>(database)->size());
//		reinterpret_cast<vector_type*>(database)->emplace_back(nullptr, 0, "", 0);
//		reinterpret_cast<vector_type*>(database)->emplace_back(ptr, size, function, line);
//#endif
//		printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
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

