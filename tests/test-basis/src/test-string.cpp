#include <basis/simstd/string>
#include <basis/simstd/memory>
#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>

void test_string()
{
	LogTrace();

	struct HeapTag {};
	typedef memory::heap::DecoratorStat<memory::heap::Default, memory::heap::StatCount, HeapTag> heap_type;
	typedef simstd::AllocatorHeap<char, heap_type> Allocator;
	typedef simstd::basic_string<char, simstd::char_traits<char>, Allocator> tstring;

	heap_type::init();
	{
		tstring str1;

		tstring str2(10, 'A');
		LogInfo(L"str2: size: %Iu, capa: %Iu '%S'\n", str2.size(), str2.capacity(), str2.c_str());

		tstring str3(str2, 2);
		LogInfo(L"str3: size: %Iu, capa: %Iu '%S'\n", str3.size(), str3.capacity(), str3.c_str());

		char str[20] = {'0','1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9'};

		tstring str4(str, 5);
		LogInfo(L"str4: size: %Iu, capa: %Iu '%S'\n", str4.size(), str4.capacity(), str4.c_str());

		tstring str5(str);
		LogInfo(L"str5: size: %Iu, capa: %Iu '%S'\n", str5.size(), str5.capacity(), str5.c_str());

		tstring str6(simstd::begin(str), simstd::end(str));
		LogInfo(L"str6: size: %Iu, capa: %Iu '%S'\n", str6.size(), str6.capacity(), str6.c_str());

		tstring str7(str6);
		LogInfo(L"str7: size: %Iu, capa: %Iu '%S'\n", str7.size(), str7.capacity(), str7.c_str());

		str6.append(6, 'z');
		LogInfo(L"str6: size: %Iu, capa: %Iu '%S'\n", str6.size(), str6.capacity(), str6.c_str());

		str6.append(str7);
		LogInfo(L"str6: size: %Iu, capa: %Iu '%S'\n", str6.size(), str6.capacity(), str6.c_str());

		str6.append(str7, 3);
		LogInfo(L"str6: size: %Iu, capa: %Iu '%S'\n", str6.size(), str6.capacity(), str6.c_str());

		str6.append(1, 'a');
		LogInfo(L"str6: size: %Iu, capa: %Iu '%S'\n", str6.size(), str6.capacity(), str6.c_str());
	}
	heap_type::destroy();

	{
		const auto stat = heap_type::get_stat();
		LogReport(L"Heap '%S' statistics:\n", heap_type::get_name());
		LogReport(L"  alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"  free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"  diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}
}
