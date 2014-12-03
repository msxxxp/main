#include <process.hpp>
#include <global.hpp>
#include <file_system.hpp>

#include <basis/std/algorithm>
#include <algorithm>

//bool is_filtered_folder(const fsys::Sequence::FindStat & stat)
//{
//	using namespace global;
//	bool ret = false;
//
//	if (options().doRecursive) {
//		if (stat.is_link()) {
//			++statistics().FoundDirLink;
//			if (options().skipDirLink)
//			{
//				++statistics().IgnoredDirLink;
//				LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [link]: ");
//			}
//		} else if (options().skipDirHidden && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
//			++statistics().IgnoredDirHidden;
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [hidden]: ");
//		} else if (options().skipDirSystem && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
//			++statistics().IgnoredDirSystem;
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [system]: ");
//		} else {
//			++statistics().FoundDirs;
//			ret = true;
//		}
//	} else {
//		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [not recursive]: ");
//	}
//
//	LogConsoleDebug(-1, L"'%s'\n", stat.name());
//	return ret;
//}
//
//bool is_filtered_file(const fsys::Sequence::FindStat & stat)
//{
//	using namespace global;
//	bool ret = false;
//
//	++statistics().FoundFiles;
//	statistics().FoundFilesSize += stat.size();
//
//	if (options().skipFileHidden && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
//		++statistics().IgnoredHidden;
//		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [hidden]: ");
//	} else if (options().skipFileSystem && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
//		++statistics().IgnoredSystem;
//		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [system]: ");
//	} else if (stat.size() == 0LL) {
//		++statistics().IgnoredZero;
//		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [zero]: ");
//	} else if (options().skipFileSmall && stat.size() < options().minFileSize) {
//		++statistics().IgnoredSmall;
//		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [small]: ");
//	} else {
//		ret = true;
//	}
//
//	LogConsoleDebug(-1, L"'%s' -> %d\n", stat.name(), ret);
//	return ret;
//}

void scan_single_folder(file_system::Node_t folder)
{
//	fsys::Sequence dir(folder->get_full_path().c_str(), L"*");
//	for (auto it = dir.begin(); it != dir.end(); ++it) {
////		LogDebug(L"%s\n", it->name());
//		if (it->is_file()) {
//			if (is_filtered_file(*it))
//				global::vars().files.emplace_back(file_system::File_t(new file_system::File(*it, folder)));
//		} else if (is_filtered_folder(*it)) {
//			scan_single_folder(file_system::Node_t(new file_system::Folder(it->name(), folder)));
//		}
//	}
}

bool CompareBySizeLess(const file_system::File_t & file1, const file_system::File_t & file2)
{
	return file1->size() < file2->size();
}

bool CompareBySizeAndTimeLess(const file_system::File_t & file1, const file_system::File_t & file2)
{
	return (file1->size() == file2->size()) ? file1->mtime() < file2->mtime() : file1->size() < file2->size();
}

bool CompareByVolumeEqual(const file_system::File_t & file1, const file_system::File_t & file2)
{
	return file1->volume_sn() == file2->volume_sn();
}

bool CompareByInodeEqual(const file_system::File_t & file1, const file_system::File_t & file2)
{
	return file1->inode() == file2->inode();
}

bool CompareAndLink(file_system::Files_t::iterator it1, file_system::Files_t::iterator it2)
{
	auto file1 = *it1;
	auto file2 = *it2;
	simstd::wstring path1(file1->get_full_path());
	simstd::wstring path2(file2->get_full_path());

	LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN, L"Comparing files [size = %I64u]:\n", file2->size());
	LogConsoleDebug(-1, L"  %s\n", path1.c_str());
	LogConsoleDebug(-1, L"  %s\n", path2.c_str());
	++global::statistics().fileCompares;

	if (global::options().attrMustMatch && file1->attr() != file2->attr()) {
		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  Attributes of files do not match, skipping\n");
		++global::statistics().fileMetaDataMismatch;
		return false;
	}

	if (global::options().attrMustMatch && file1->attr() != file2->attr()) {
		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  Attributes of files do not match, skipping\n");
		++global::statistics().fileMetaDataMismatch;
		return false;
	}

	if (global::options().timeMustMatch && file1->mtime() != file2->mtime()) {
		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  Modification timestamps of files do not match, skipping\n");
		++global::statistics().fileMetaDataMismatch;
		return false;
	}

	if (!CompareByVolumeEqual(file1, file2)) {
		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  Files ignored - on different volumes\n");
		++global::statistics().filesOnDifferentVolumes;
		return false;
	}

	if (CompareByInodeEqual(file1, file2)) {
		++global::statistics().fileAlreadyLinked;
		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_GREEN, L"  Files ignored - already linked\n");
		return false;
	}

	if (file_system::compare_hash(*file1, *file2)) {
		++global::statistics().fileContentSame;
		LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN, L"Comparing files [size = %I64u]:\n", file1->size());
		LogConsoleReport(-1, L"  %s\n", path1.c_str());
		LogConsoleReport(-1, L"  %s\n", path2.c_str());
		LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  Files are equal, hard link possible\n");

//		if (global::options().doHardlink)
//			;//file1->hardlink(*file2);
		global::statistics().FreeSpaceIncrease += file2->size();
		return true;
	}

	LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  Files differ in content (hash)\n");
	++global::statistics().hashComparesHit1;
	return false;
}

void process_equal_sized_files(file_system::Files_t & files)
{
	LogDebug(L"size [%I64u] count [%I64u]\n", files.begin()->get()->size(), files.size());
	while (files.size() > 1)
	{
		auto keyIt = files.end() - 1;
		LogDebug(L"[%I64u] '%s'\n", keyIt->get()->size(), keyIt->get()->get_name().c_str());

		for (auto curIt = files.begin(); curIt != keyIt; ++curIt) {
			LogDebug(L"[%I64u] '%s'\n", curIt->get()->size(), curIt->get()->get_name().c_str());
			if (CompareAndLink(keyIt, curIt))
				break;
		}

		files.erase(keyIt);
	}

//		if (file1->size() > FirstBlock) {
//			if (!file1->LoadHashMini()) {
//				break;
//			}
//			if (!file2->LoadHashMini()) {
//				it = data.erase(it);
//				continue;
//			}
//		} else {
//			if (!file1->LoadHashFull()) {
//				break;
//			}
//			if (!file2->LoadHashFull()) {
//				it = data.erase(it);
//				continue;
//			}
//		}
}

ssize_t process_all_paths()
{
	LogTrace();
	LogConsoleInfo(-1, L"Folders to process: %I64u\n", global::vars().folders.size());

	auto it = global::vars().folders.begin(), end = global::vars().folders.end();
	for (; it != end; ++it) {
		scan_single_folder(*it);
	}

	LogConsoleInfo(-1, L"Files to process: %I64u\n", global::vars().files.size());

	if (global::vars().files.empty())
		return 1;

	{
		using namespace global;
		std::sort(vars().files.begin(), vars().files.end(), CompareBySizeAndTimeLess);

		while (!vars().files.empty()) {
//			logCounter(L"Files left:\t%8llu", std::distance(srch, data.end()));
			auto bounds = std::equal_range(vars().files.begin(), vars().files.end(), vars().files.front(), CompareBySizeLess);
			if (std::distance(bounds.first, bounds.second) == 1) {
				++statistics().filesFoundUnique;
				LogDebug(L"unique [%I64u] '%s'\n", bounds.first->get()->size(), bounds.first->get()->get_name().c_str());
			} else {
//				file_system::Files_t files(bounds.first, bounds.second);
				file_system::Files_t files;
				std::copy(bounds.first, bounds.second, std::back_inserter(files));
				process_equal_sized_files(files);
			}
			vars().files.erase(bounds.first, bounds.second);
		}
	}

	return 0;
}
