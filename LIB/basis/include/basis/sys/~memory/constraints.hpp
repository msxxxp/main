#ifndef BASIS_SYS_MEMORY_CONSTRAINTS_HPP_
#define BASIS_SYS_MEMORY_CONSTRAINTS_HPP_

#include <basis/sys/memory.hpp>

namespace memory {

	template<typename Type>
	struct must_be_pointer
	{
		static bool constraints(const Type & type_is_not_pointer)
		{
			return (bool)sizeof(0[type_is_not_pointer]);
		}
	};

	template<>
	struct must_be_pointer<PVOID>
	{
		static bool constraints(const PVOID &)
		{
			return true;
		}
	};

}

#endif
