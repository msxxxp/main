#include <fsys.hpp>

const wchar_t* to_str(fsys::Sequence::Filter::Type type)
{
	auto ret = L"I";
	switch (type) {
		case fsys::Sequence::Filter::Type::IncludeOnly:
			break;
		case fsys::Sequence::Filter::Type::ExcludeAll:
			ret = L"E";
			break;
	}
	return ret;
}

inline bool skip_or_not(fsys::Sequence::Filter::Type type, bool passed)
{
	return (type == fsys::Sequence::Filter::Type::ExcludeAll && passed) || (type == fsys::Sequence::Filter::Type::IncludeOnly && !passed);
}

fsys::Sequence::Filter::Filter(Type type, const ustring& name):
	name(name),
	mask(),
	type(type),
	minSize(0), maxSize(UINT64_MAX),
	minWrTime(0), maxWrTime(INT64_MAX),
	minCrTime(0), maxCrTime(INT64_MAX),
	minAcTime(0), maxAcTime(INT64_MAX),
	enabledAttr(0), disabledAttr(0)
{
	LogTraceObj();
}

bool fsys::Sequence::Filter::operator ()(const FindStat& stat, Statistics& statistics) const
{ // return true if skip this item
	LogConsoleDebug2(-1, L"   test [%s]: '%s'\n", to_str(type), stat.name());
	bool passed =
		apply_attributes(stat, statistics) &&
		apply_size(stat, statistics) &&
		apply_wr_time(stat, statistics) &&
		apply_cr_time(stat, statistics) &&
		apply_ac_time(stat, statistics) &&
		apply_mask(stat, statistics);

	return (type == Type::IncludeOnly && !passed) || (type == Type::ExcludeAll && passed);
}

//	bool Sequence::Filter::apply_to_folder(const FindStat& stat, Statistics& statistics) const
//	{
//		bool ret = false;
//
//		statistics.folder_found(stat);
//
//		if (flags & folderSkipAll) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [not recursive]:");
//			statistics.folder_ignored();
//		} else if ((flags & folderSkipLink) && stat.is_link()) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [link]:");
//			statistics.folder_ignored_link();
//		} else if ((flags & folderSkipArchive) && (stat.attr() & FILE_ATTRIBUTE_ARCHIVE)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [archive]:");
//			statistics.folder_ignored_archive();
//		} else if ((flags & folderSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [read only]:");
//			statistics.folder_ignored_readonly();
//		} else if ((flags & folderSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [hidden]:");
//			statistics.folder_ignored_hidden();
//		} else if ((flags & folderSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [system]:");
//			statistics.folder_ignored_system();
//		} else if (!(flags & folderIncludeDots) && ((cstr::compare(stat.name(), L".") == 0 || cstr::compare(stat.name(), L"..") == 0))) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [invalid]:");
//		} else {
//			ret = true;
//		}
//
//		if (ret)
//			LogConsoleDebug(-1, L"  folder accepted: '%s'\n", stat.name());
//		else
//			LogConsoleDebug(-1, L" '%s'\n", stat.name());
//		return ret;
//	}
//
//	bool Sequence::Filter::apply_to_file(const FindStat& stat, Statistics& statistics) const
//	{
//		bool ret = false;
//
//		statistics.file_found(stat);
//
//		if ((flags & fileSkipLink) && stat.is_link()) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [link]:");
//			statistics.file_ignored_link();
//		} else if ((flags & fileSkipArchive) && (stat.attr() & FILE_ATTRIBUTE_ARCHIVE)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [read only]:");
//			statistics.file_ignored_archive();
//		} else if ((flags & fileSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [read only]:");
//			statistics.file_ignored_readonly();
//		} else if ((flags & fileSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [hidden]:");
//			statistics.file_ignored_hidden();
//		} else if ((flags & fileSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [system]:");
//			statistics.file_ignored_system();
////		} else if ((flags & fileSkipStreamed) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
////			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"file ignored [streamed]:");
////			statistics.file_ignored_streamed();
//		} else if ((flags & fileSkipCompressed) && (stat.attr() & FILE_ATTRIBUTE_COMPRESSED)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [compressed]:");
//			statistics.file_ignored_compressed();
//		} else if ((flags & fileSkipEncrypted) && (stat.attr() & FILE_ATTRIBUTE_ENCRYPTED)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [encrypted]:");
//			statistics.file_ignored_encrypted();
//		} else if ((flags & fileSkipSparse) && (stat.attr() & FILE_ATTRIBUTE_SPARSE_FILE)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [sparse]:");
//			statistics.file_ignored_sparse();
//		} else if ((flags & fileSkipTemporary) && (stat.attr() & FILE_ATTRIBUTE_TEMPORARY)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [temporary]:");
//			statistics.file_ignored_temporary();
//		} else if ((flags & fileSkipOffline) && (stat.attr() & FILE_ATTRIBUTE_OFFLINE)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [offline]:");
//			statistics.file_ignored_offline();
//		} else if ((flags & fileSkipZeroSize) && stat.size() == 0LL) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [zero]: ");
//			statistics.file_ignored_zero();
//		} else if (stat.size() < fileMinSize) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [to small]:");
//			statistics.file_ignored_to_small();
//		} else if (stat.size() > fileMaxSize) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [to big]:");
//			statistics.file_ignored_to_big();
//		} else {
//			ret = true;
//		}
//
//		if (ret)
//			LogConsoleDebug(-1, L"  file accepted: '%s'\n", stat.name());
//		else
//			LogConsoleDebug(-1, L" '%s'\n", stat.name());
//		return ret;
//	}

bool fsys::Sequence::Filter::apply_mask(const FindStat& /*stat*/, Statistics& /*statistics*/) const
{
	bool passed = true;

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"mask(%s)]\n", mask.c_str());

	return passed;
}

bool fsys::Sequence::Filter::apply_size(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minSize, stat.size(), maxSize);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"size(%I64u, %I64u)]: %I64u\n", minSize, maxSize, stat.size());

	return passed;
}

bool fsys::Sequence::Filter::apply_wr_time(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minWrTime, stat.mtime(), maxWrTime);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"wrtime(%I64d, %I64d)]: %I64d\n", minWrTime, maxWrTime, stat.mtime());

	return passed;
}

bool fsys::Sequence::Filter::apply_cr_time(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minCrTime, stat.ctime(), maxCrTime);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"crtime(%I64d, %I64d)]: %I64d\n", minCrTime, maxCrTime, stat.ctime());

	return passed;
}

bool fsys::Sequence::Filter::apply_ac_time(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minAcTime, stat.atime(), maxAcTime);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"actime(%I64d, %I64d)]: %I64d\n", minAcTime, maxAcTime, stat.atime());

	return passed;
}

bool fsys::Sequence::Filter::apply_attributes(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = (stat.attr() | enabledAttr) == stat.attr() && (stat.attr() & disabledAttr) == Attr();

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"attr(0x%08X && !0x%08X)]: 0x%08X\n", enabledAttr, disabledAttr, stat.attr());

	return passed;
}

void fsys::Sequence::Filter::set_type()
{
	this->type = type;
}

void fsys::Sequence::Filter::set_name(const ustring& name)
{
	this->name = name;
}

void fsys::Sequence::Filter::set_mask(const ustring& mask)
{
	this->mask = mask;
}

void fsys::Sequence::Filter::set_size(const Size& minSize, const Size& maxSize)
{
	this->minSize = minSize;
	this->maxSize = maxSize;
}

void fsys::Sequence::Filter::set_wr_time(const Time& minWrTime, const Size& maxWrTime)
{
	this->minWrTime = minWrTime;
	this->maxWrTime = maxWrTime;
}

void fsys::Sequence::Filter::set_cr_time(const Time& minCrTime, const Size& maxCrTime)
{
	this->minCrTime = minCrTime;
	this->maxCrTime = maxCrTime;
}

void fsys::Sequence::Filter::set_ac_time(const Time& minAcTime, const Size& maxAcTime)
{
	this->minAcTime = minAcTime;
	this->maxAcTime = maxAcTime;
}

void fsys::Sequence::Filter::set_attr(const Attr& enabledAttr, const Attr& disabledAttr)
{
	this->enabledAttr = enabledAttr;
	this->disabledAttr = disabledAttr;
	this->enabledAttr = this->enabledAttr & ~this->disabledAttr;
	LogNoise(L"{ea: 0x%08X, da: 0x%08X}\n", this->enabledAttr, this->disabledAttr);
}

fsys::Sequence::Filter::Type fsys::Sequence::Filter::get_type() const
{
	return type;
}
