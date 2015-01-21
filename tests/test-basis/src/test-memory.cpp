#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>

void test_memory()
{
	LogWarn(L"\n");

	auto heap = ::GetProcessHeap();


	PROCESS_HEAP_ENTRY entry;
	memory::zero(entry);

	LogReport(L"Walking heap %p...\n\n", heap);
	while (::HeapWalk(heap, &entry) != FALSE) {
		if ((entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) != 0) {
			LogReport(L"Allocated block");

			if ((entry.wFlags & PROCESS_HEAP_ENTRY_MOVEABLE) != 0) {
				LogReport(L", movable with HANDLE %p", entry.Block.hMem);
			}

			if ((entry.wFlags & PROCESS_HEAP_ENTRY_DDESHARE) != 0) {
				LogReport(L", DDESHARE");
			}
		}
		else if ((entry.wFlags & PROCESS_HEAP_REGION) != 0) {
			LogReport(L"Region\n  %d bytes committed\n" \
			         L"  %d bytes uncommitted\n  First block address: %p\n" \
			         L"  Last block address: %p\n",
			         entry.Region.dwCommittedSize,
			         entry.Region.dwUnCommittedSize,
			         entry.Region.lpFirstBlock,
			         entry.Region.lpLastBlock);
		}
		else if ((entry.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) != 0) {
			LogReport(L"Uncommitted range\n");
		}
		else {
			LogReport(L"Block\n");
		}

		LogReport(L"  Data portion begins at: %p\n  Size: %d bytes\n" \
		         L"  Overhead: %d bytes\n  Region index: %d\n\n",
		         entry.lpData,
		         entry.cbData,
		         entry.cbOverhead,
		         entry.iRegionIndex);
	}
//	LastError = GetLastError();
//	if (LastError != ERROR_NO_MORE_ITEMS) {
//		_tprintf(TEXT("HeapWalk failed with LastError %d.\n"), LastError);
//	}

	return;


	struct TypeTag {};
	typedef typename memory::heap::DecoratorStat<memory::heap::Default, memory::heap::StatLog, TypeTag> heap_type;

	struct TypeTag1 {};
	typedef typename memory::heap::DecoratorStat<memory::heap::Default, memory::heap::StatLog, TypeTag1> heap_type1;

	struct TypeTag2 {};
	typedef typename memory::heap::DecoratorStat<memory::heap::Default, memory::heap::StatLog, TypeTag2> heap_type2;

	auto ptr = HostAlloc(heap_type, 47);
	HostFree(heap_type, ptr);
	HostAlloc(heap_type, 42);
	{
		const auto stat = heap_type::get_stat();
		LogReport(L"stat alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"stat free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"stat diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}

	{
		const auto stat = heap_type1::get_stat();
		LogReport(L"stat1 alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"stat1 free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"stat1 diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}
	HostAlloc(heap_type1, 17);
	HostAlloc(heap_type1, 12);

	HostAlloc(heap_type2, 71);
	HostAlloc(heap_type2, 22);

	{
		const auto stat = heap_type1::get_stat();
		LogReport(L"stat1 alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"stat1 free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"stat1 diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}

	{
		const auto stat = heap_type2::get_stat();
		LogReport(L"stat2 alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"stat2 free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"stat2 diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}

}
