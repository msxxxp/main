#include <basis/simstd/string>
#include <basis/simstd/memory>
#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>

void test_string()
{
	typedef int TypeTag;
	typedef typename memory::heap::SpecialLogged<TypeTag> Heap;
	typedef typename simstd::AllocatorHeap<char, Heap> Allocator;
	typedef typename simstd::basic_string<char, simstd::char_traits<char>, Allocator> tstring;

	LogTrace();

	Heap::init();

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
	{
		const memory::heap::Stat& stat = Heap::get_stat();
		console::printf(L"stat alloc: %I64u, %I64u \n", stat.allocations, stat.allocSize);
		console::printf(L"stat free : %I64u, %I64u \n", stat.frees, stat.freeSize);
		console::printf(L"stat diff : %I64d \n", stat.allocSize - stat.freeSize);
	}
}
