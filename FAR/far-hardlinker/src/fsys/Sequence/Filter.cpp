#include <fsys.hpp>

fsys::Sequence::Filter::Filter(Type type, const ustring& name):
	name(name),
	mask(),
	type(type),
	minSize(0), maxSize(static_cast<uint64_t>(-1)),
	minWrTime(0), maxWrTime(static_cast<uint64_t>(-1)),
	minCrTime(0), maxCrTime(static_cast<uint64_t>(-1)),
	minAcTime(0), maxAcTime(static_cast<uint64_t>(-1)),
	minChTime(0), maxChTime(static_cast<uint64_t>(-1)),
	enabledAttr(static_cast<size_t>(-1)), disabledAttr(0)
{
}

bool fsys::Sequence::Filter::operator ()(const FindStat& stat, Statistics& statistics) const
{
	return apply_attributes(stat, statistics) && apply_mask(stat, statistics);
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

bool fsys::Sequence::Filter::apply_attributes(const FindStat& stat, Statistics& /*statistics*/) const
{ // FIXME
	bool ret = false;
	if (type == Type::Include)
		ret = (stat.attr() & enabledAttr) == stat.attr() && (stat.attr() & disabledAttr) == Attr();
	else
		ret = (stat.attr() & enabledAttr) == Attr() && (stat.attr() & disabledAttr) != Attr();
	return ret;
}

bool fsys::Sequence::Filter::apply_mask(const FindStat& stat, Statistics& /*statistics*/) const
{ // FIXME
	return false;
	return stat.name() == mask;
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

void fsys::Sequence::Filter::set_ch_time(const Time& minChTime, const Size& maxChTime)
{
	this->minChTime = minChTime;
	this->maxChTime = maxChTime;
}

void fsys::Sequence::Filter::set_attr(const Attr& enabledAttr, const Attr& disabledAttr)
{
	this->enabledAttr = enabledAttr;
	this->disabledAttr = disabledAttr;
}
