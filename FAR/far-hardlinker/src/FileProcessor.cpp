#include <basis/sys/logger.hpp>

#include <FileProcessor.hpp>
#include <global.hpp>
#include <fsys.hpp>

#include <basis/simstd/algorithm>
//#include <list>
//#include <algorithm>


bool CompareBySizeLess(const fsys::File_t & file1, const fsys::File_t & file2)
{
	return file1->size() < file2->size();
}

bool CompareBySizeAndTimeLess(const fsys::File_t & file1, const fsys::File_t & file2)
{
	return (file1->size() == file2->size()) ? file1->mtime() < file2->mtime() : file1->size() < file2->size();
}

bool CompareByVolumeEqual(const fsys::File_t & file1, const fsys::File_t & file2)
{
	return file1->volume_sn() == file2->volume_sn();
}

bool CompareByInodeEqual(const fsys::File_t & file1, const fsys::File_t & file2)
{
	return file1->inode() == file2->inode();
}

bool CompareAndLink(fsys::Files_t::iterator it1, fsys::Files_t::iterator it2)
{
	auto file1 = *it1;
	auto file2 = *it2;
	ustring path1(file1->get_full_path());
	ustring path2(file2->get_full_path());

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

	if (fsys::compare_hash(*file1, *file2)) {
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

void process_equal_sized_files(fsys::Files_t & files)
{
//	LogDebug(L"size [%I64u] count [%I64u]\n", files.begin()->get()->size(), files.size());
	while (files.size() > 1)
	{
		auto keyIt = files.end() - 1;
//		LogDebug(L"[%I64u] '%s'\n", keyIt->get()->size(), keyIt->get()->get_name().c_str());

		for (auto curIt = files.begin(); curIt != keyIt; ++curIt) {
//			LogDebug(L"[%I64u] '%s'\n", curIt->get()->size(), curIt->get()->get_name().c_str());
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

void scan_single_folder(fsys::Node_t folder)
{
	auto fullPath = folder->get_full_path();
	LogConsoleDebug(-1, L"processing: '%s'\n", fullPath.c_str());

	fsys::Sequence::SearchOptions opt;
	opt.flags |= global::options().doRecursive ? 0 : fsys::Sequence::SearchFlags::folderSkipAll;

	fsys::Sequence dir(fullPath, opt, global::statistics());
	for (auto it = dir.begin(); it != dir.end(); ++it) {
//		LogDebug(L"%s\n", it->name());
		if (it->is_dir()) {
			global::vars().folders.emplace_back(fsys::Node_t(new fsys::Folder(it->name(), folder)));
		} else {
			global::vars().files.emplace_back(fsys::File_t(new fsys::File(*it, folder)));
		}
	}
}

ssize_t FileProcessor::execute()
{
	LogTrace();
	LogConsoleInfo(-1, L"Folders to process: %I64u\n", global::vars().folders.size());

	while (!global::vars().folders.empty())
	{
		auto folder = global::vars().folders.back();
		global::vars().folders.pop_back();
		scan_single_folder(folder);
	}

	if (global::vars().files.empty()) {
		LogConsoleInfo(-1, L"No files to process\n");
		return 1;
	} else {
		LogConsoleInfo(-1, L"Files to process: %I64u\n", global::vars().files.size());
	}

	using namespace global;
	simstd::sort(vars().files.begin(), vars().files.end(), CompareBySizeAndTimeLess);

	while (!vars().files.empty()) {
//		logCounter(L"Files left:\t%8llu", std::distance(srch, data.end()));
		auto bounds = simstd::equal_range(vars().files.begin(), vars().files.end(), vars().files.front(), CompareBySizeLess);
		if (simstd::distance(bounds.first, bounds.second) == 1) {
			++statistics().filesFoundUnique;
			LogDebug(L"unique [%I64u] '%s'\n", bounds.first->get()->size(), bounds.first->get()->get_name().c_str());
		} else {
			fsys::Files_t files(bounds.first, bounds.second);
//			fsys::Files_t files;
//			simstd::copy(bounds.first, bounds.second, simstd::back_inserter(files));
			process_equal_sized_files(files);
		}
		vars().files.erase(bounds.first, bounds.second);
	}

	return 0;
}
