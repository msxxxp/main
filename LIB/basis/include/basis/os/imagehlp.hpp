#ifndef BASIS_OS_IMAGEHLP_
#define BASIS_OS_IMAGEHLP_

#include <basis/sys/linkage.hpp>

namespace os {

	struct imagehlp_dll: private linkage::DynamicLibrary {
		typedef PVOID (WINAPI *FImageDirectoryEntryToData)(PVOID Base, BOOLEAN MappedAsImage, USHORT DirectoryEntry, PULONG Size);

		DEFINE_FUNC(ImageDirectoryEntryToData);

		static imagehlp_dll & inst();

	private:
		imagehlp_dll();
	};

}

#endif
