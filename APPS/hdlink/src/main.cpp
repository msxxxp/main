﻿/**
 hdlink console tool
 Search duplicate files and make hardlinks

 © 2013 Andrew Grechkin

 Some code was adopted from:
 DFHL - Duplicate File Hard Linker, a small tool to gather some space
 from duplicate files on your hard disk
 Copyright (C) 2004, 2005 Jens Scheffler & Oliver Schneider
 http://www.jensscheffler.de/dfhl

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "main.h"
#include "version.h"

typedef std::vector<shared_ptr<Path> > PathsList;
typedef std::vector<shared_ptr<File> > FilesList;
typedef FilesList::iterator FilesListIt;
typedef PathsList::iterator PathListIt;



///====================================================================================== FileSystem
class FileSystem {
	PathsList paths;
	FilesList data;

	bool recursive;
	bool hardlink;
	bool AttrMustMatch;
	bool TimeMustMatch;
	bool LinkSmall;
	bool SkipJunct;
	bool SkipHidden;
	bool SkipSystem;

	Path* parsePath(PCWSTR path)
	{
		auto_array < WCHAR > extendedPath(MAX_PATH_LEN);
		Copy(extendedPath, PATH_PREFIX_NT, extendedPath.size());
		Cat(extendedPath, FullPath(path).c_str(), extendedPath.size());
		::GetLongPathName(extendedPath, extendedPath, extendedPath.size());
		PWSTR tmp = (PWSTR)find_last_not_of((PCWSTR)extendedPath, L"\\ ");
		if (tmp && (tmp - extendedPath) < (ssize_t)extendedPath.size()) {
			tmp[1] = STR_END;		//erase tailing path separators
		}

		Path* Result = nullptr;
		if (Empty(extendedPath) || !is_exists(extendedPath)) {
			logError(L"Path \"%s\" is not existing or accessible!\n", extendedPath.data());
		} else {
			{
				ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				logInfo(L"Adding directory: ");
			}
			logInfo(L"\"%s\"\n", path);
			Result = new Path(shared_ptr<Path>(), AutoUTF(extendedPath));
		}
		return Result;
	}

	bool getFolderContent(shared_ptr<Path> folder)
	{
		WinDir dir(folder->path(), L"*");
		for (WinDir::iterator it = dir.begin(); it != dir.end(); ++it) {
			if (it.is_file()) {
				++Statistics::getInstance()->FoundFiles;
				Statistics::getInstance()->FoundFilesSize += it.size();
				if (SkipHidden && (it.attr() & FILE_ATTRIBUTE_HIDDEN)) {
					++Statistics::getInstance()->IgnoredHidden;
					{
						ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						logDebug(L"File ignored [hidden]: ");
					}
					logDebug(L"\"%s\"\n", it.name());
				} else if (SkipSystem && (it.attr() & FILE_ATTRIBUTE_SYSTEM)) {
					++Statistics::getInstance()->IgnoredSystem;
					{
						ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						logDebug(L"File ignored [system]: ");
					}
					logDebug(L"\"%s\"\n", it.name());
				} else if (it.size() == 0LL) {
					++Statistics::getInstance()->IgnoredZero;
					{
						ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						logDebug(L"File ignored [zero]: ");
					}
					logDebug(L"\"%s\"\n", it.name());
				} else if (!LinkSmall && (it.size() < MIN_FILE_SIZE)) {
					++Statistics::getInstance()->IgnoredSmall;
					{
						ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						logDebug(L"File ignored [small]: ");
					}
					logDebug(L"\"%s\"\n", it.name());
				} else {
					//logFile(info);
					data.push_back(shared_ptr < File > (new File(folder, it.name())));
				}
			} else {
				if (SkipJunct && it.is_link()) {
					++Statistics::getInstance()->IgnoredJunc;
					{
						ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
						logDebug(L"Dir  ignored [junction]: ");
					}
					logDebug(L"\"%s\"\n", it.name());
				} else {
					if (it.is_link())
						++Statistics::getInstance()->FoundJuncs;
					else
						++Statistics::getInstance()->FoundDirs;
//					logFile(info);
					if (recursive) {
						bool Result = getFolderContent(shared_ptr < Path > (new Path(folder, it.name())));
						if (!Result)
							return false;
					}
				}
			}
		}

//		WIN32_FIND_DATAW	info;
//		HANDLE	hFind = ::FindFirstFileW(mask.c_str(), &info);
//		if (hFind == INVALID_HANDLE_VALUE) {
//			logError(::GetLastError(), L"Unable to read folder \"%s\" content.\n", path.c_str());
//		} else {
//			do {
//				if (is_valid_filename(info.cFileName)) {
//					if (!(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//						uintmax_t	filesize = MyUI64(info.nFileSizeLow, info.nFileSizeHigh);
//						++Statistics::getInstance()->FoundFiles;
//						Statistics::getInstance()->FoundFilesSize += filesize;
//						if (SkipHidden && (info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
//							++Statistics::getInstance()->IgnoredHidden;
//							{
//								ConsoleColor	col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
//								logDebug(L"File ignored [hidden]: ");
//							}
//							logDebug(L"\"%s\"\n", info.cFileName);
//						} else if (SkipSystem && (info.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
//							++Statistics::getInstance()->IgnoredSystem;
//							{
//								ConsoleColor	col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
//								logDebug(L"File ignored [system]: ");
//							}
//							logDebug(L"\"%s\"\n", info.cFileName);
//						} else if (filesize == 0LL) {
//							++Statistics::getInstance()->IgnoredZero;
//							{
//								ConsoleColor	col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
//								logDebug(L"File ignored [zero]: ");
//							}
//							logDebug(L"\"%s\"\n", info.cFileName);
//						} else if (!LinkSmall && (filesize < MIN_FILE_SIZE)) {
//							++Statistics::getInstance()->IgnoredSmall;
//							{
//								ConsoleColor	col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
//								logDebug(L"File ignored [small]: ");
//							}
//							logDebug(L"\"%s\"\n", info.cFileName);
//						} else {
//							logFile(info);
//							data.push_back(shared_ptr<File>(new File(folder, info.cFileName)));
//						}
//					} else {
//						if (SkipJunct && info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
//							++Statistics::getInstance()->IgnoredJunc;
//							{
//								ConsoleColor	col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
//								logDebug(L"Dir  ignored [junction]: ");
//							}
//							logDebug(L"\"%s\"\n", info.cFileName);
//						} else {
//							if (info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
//								++Statistics::getInstance()->FoundJuncs;
//							else
//								++Statistics::getInstance()->FoundDirs;
//							logFile(info);
//							if (recursive) {
//								bool	Result = getFolderContent(shared_ptr<Path>(new Path(folder, info.cFileName)));
//								if (!Result)
//									return	false;
//							}
//						}
//					}
//				}
//			} while (::FindNextFile(hFind, &info) != 0);
//			dwError = ::GetLastError();
//			::FindClose(hFind);
//			if (dwError != ERROR_NO_MORE_FILES) {
//				logError(dwError, L"FindNextFile error\n");
//				return	false;
//			}
//		}
		return true;
	}

public:
	~FileSystem()
	{
	}

	FileSystem()
	{
		recursive = hardlink = false;
		AttrMustMatch = TimeMustMatch = false;
		LinkSmall = SkipJunct = SkipHidden = SkipSystem = false;
	}

	bool ParseCommandLine(int argc, PWSTR argv[])
	{
		bool ret = false;

		if (argc == 1) {
			logInfo(L"%s\n", argv[0]);
			PrintHelp();
			return ret;
		}

		for (int i = 1; i < argc; ++i) {
			// first check if command line option
			if ((argv[i][0] == L'-' || argv[i][0] == L'/')) {
				if (Len(argv[i]) == 2) {
					switch (argv[i][1]) {
						case L'?':
							logInfo(L"%s\n", argv[0]);
							PrintHelp();
							return false;
							break;
						case L'a':
							AttrMustMatch = true;
							break;
						case L'd':
							setLogLevel (LOG_DEBUG);
							break;
						case L'h':
							SkipHidden = true;
							break;
						case L'j':
							SkipJunct = true;
							break;
						case L'l':
							hardlink = true;
							break;
						case L'm':
							LinkSmall = true;
							break;
						case L'q':
							setLogLevel (LOG_ERROR);
							showStatistics = false;
							break;
						case L'r':
							recursive = true;
							break;
						case L's':
							SkipSystem = true;
							break;
						case L't':
							TimeMustMatch = true;
							break;
						case L'v':
							setLogLevel (LOG_VERBOSE);
							break;
						case L'w':
							showStatistics = true;
							break;
						default:
							logError(L"Illegal Command line option! Use /? to see valid options!\n");
							return false;
					}
				} else {
					logError(L"Illegal Command line option! Use /? to see valid options!\n");
					return false;
				}
			} else {
				Path* dir = parsePath(argv[i]);
				if (dir) {
					AddPath(dir);
					ret = true;
				}
			}
		}

		if (!ret) {
			logError(L"You need to specify at least one folder to process!\nUse /? to see valid options!\n");
		}
		return ret;
	}

	void Process()
	{
		PathListIt it = paths.begin();
		while (it != paths.end()) {
			getFolderContent(*it);
			++it;
		}
		logInfo(L"Files to process:\t%8llu\n", data.size());

		logDebug(L"");
		std::sort(data.begin(), data.end(), CompareBySizeAndTime);
		std::pair<FilesListIt, FilesListIt> bounds;
		FilesListIt srch = data.begin();
		while (srch != data.end()) {
			logCounter(L"Files left:\t%8llu", std::distance(srch, data.end()));
			bounds = std::equal_range(srch, data.end(), *srch, CompareBySize);
			if (std::distance(bounds.first, bounds.second) == 1) {
				++Statistics::getInstance()->filesFoundUnique;
				data.erase(bounds.first);
			} else {
				while (srch != bounds.second) {
					FilesListIt it = srch;
					++it;
					shared_ptr<File>& f1 = *srch;
					while (it != bounds.second) {
						shared_ptr<File>& f2 = *it;
						AutoUTF buf1(f1->path());
						AutoUTF buf2(f2->path());
						{
							ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
							logDebug(L"Comparing files [size = %I64u]:\n", f1->size());
						}
						logDebug(L"  %s\n", buf1.c_str());
						logDebug(L"  %s\n", buf2.c_str());
						++Statistics::getInstance()->fileCompares;
						f1->refresh();
						f2->refresh();
						if (AttrMustMatch && f1->attr() != f2->attr()) {
							{
								ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
								logDebug(L"  Attributes of files do not match, skipping\n");
							}
							Statistics::getInstance()->fileMetaDataMismatch++;
							++it;
							break;
						}
						if (TimeMustMatch && f1->mtime() != f2->mtime()) {
							{
								ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
								logDebug(L"  Modification timestamps of files do not match, skipping\n");
							}
							Statistics::getInstance()->fileMetaDataMismatch++;
							++it;
							break;
						}
						if (!isSameVolume(*f1, *f2)) {
							{
								ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
								logDebug(L"  Files ignored - on different volumes\n");
							}
							++Statistics::getInstance()->filesOnDifferentVolumes;
							++it;
							break;
						}
						if (f1 == f2) {
							++Statistics::getInstance()->fileAlreadyLinked;
							{
								ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
								logDebug(L"  Files ignored - already linked\n");
							}
							++it;
							break;
						}
						if (f1->size() > FirstBlock) {
							if (!f1->LoadHashMini()) {
								break;
							}
							if (!f2->LoadHashMini()) {
								it = data.erase(it);
								continue;
							}
						} else {
							if (!f1->LoadHashFull()) {
								break;
							}
							if (!f2->LoadHashFull()) {
								it = data.erase(it);
								continue;
							}
						}
						if (isIdentical(*f1, *f2)) {
							++Statistics::getInstance()->fileContentSame;
							if (logLevel == LOG_VERBOSE) {
								{
									ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
									logVerbose(L"Comparing files [size = %I64u]:\n", f1->size());
								}
								logVerbose(L"  %s\n", buf1.c_str());
								logVerbose(L"  %s\n", buf2.c_str());
							}
							{
								ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
								logVerbose(L"  Files are equal, hard link possible\n");
							}
							if (hardlink) {
								f1->hardlink(*f2);
							}
							Statistics::getInstance()->FreeSpaceIncrease += f1->size();
							it = data.erase(it);
						} else {
							{
								ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
								logDebug(L"  Files differ in content (hash)\n");
							}
							Statistics::getInstance()->hashComparesHit1++;
							++it;
						}
					}
					srch = data.erase(srch);
				}
			}
			srch = bounds.second;
		}
		logCounter(L"                              ");
	}

	void AddPath(Path* p)
	{
		paths.push_back(shared_ptr < Path > (p));
	}
};
