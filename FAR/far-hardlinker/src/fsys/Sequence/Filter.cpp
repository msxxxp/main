#include <fsys.hpp>

const wchar_t* to_str(fsys::Sequence::FiltersBunch::Type type)
{
	auto ret = L"I";
	switch (type) {
		case fsys::Sequence::FiltersBunch::Type::IncludeOnly:
			break;
		case fsys::Sequence::FiltersBunch::Type::ExcludeAll:
			ret = L"E";
			break;
	}
	return ret;
}

fsys::Sequence::FiltersBunch::FiltersBunch(Type type, const ustring& name):
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

bool fsys::Sequence::FiltersBunch::operator ()(const FindStat& stat, Statistics& statistics) const
{ // return true if skip this item
	LogConsoleDebug2(-1, L"   appply filter [%s, %s] on '%s'\n", to_str(type), name.c_str(), stat.name());
	bool passed =
		apply_attributes(stat, statistics) &&
		apply_size(stat, statistics) &&
		apply_wr_time(stat, statistics) &&
		apply_cr_time(stat, statistics) &&
		apply_ac_time(stat, statistics) &&
		apply_mask(stat, statistics);

	return (type == Type::IncludeOnly && !passed) || (type == Type::ExcludeAll && passed);
}

bool fsys::Sequence::FiltersBunch::apply_mask(const FindStat& /*stat*/, Statistics& /*statistics*/) const
{
	bool passed = true;

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"mask(%s)]\n", mask.c_str());

	return passed;
}

bool fsys::Sequence::FiltersBunch::apply_size(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minSize, stat.size(), maxSize);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"size(%I64u, %I64u)]: %I64u\n", minSize, maxSize, stat.size());

	return passed;
}

bool fsys::Sequence::FiltersBunch::apply_wr_time(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minWrTime, stat.mtime(), maxWrTime);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"wrtime(%I64d, %I64d)]: %I64d\n", minWrTime, maxWrTime, stat.mtime());

	return passed;
}

bool fsys::Sequence::FiltersBunch::apply_cr_time(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minCrTime, stat.ctime(), maxCrTime);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"crtime(%I64d, %I64d)]: %I64d\n", minCrTime, maxCrTime, stat.ctime());

	return passed;
}

bool fsys::Sequence::FiltersBunch::apply_ac_time(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = simstd::between(minAcTime, stat.atime(), maxAcTime);

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"actime(%I64d, %I64d)]: %I64d\n", minAcTime, maxAcTime, stat.atime());

	return passed;
}

bool fsys::Sequence::FiltersBunch::apply_attributes(const FindStat& stat, Statistics& /*statistics*/) const
{
	bool passed = (stat.attr() | enabledAttr) == stat.attr() && (stat.attr() & disabledAttr) == Attr();

	if (passed)
		LogConsoleDebug2(-1, L"    passed [");
	else
		LogConsoleDebug2(-1, L"    ignore [");
	LogConsoleDebug2(-1, L"attr(0x%08X && !0x%08X)]: 0x%08X\n", enabledAttr, disabledAttr, stat.attr());

	return passed;
}

void fsys::Sequence::FiltersBunch::set_type()
{
	this->type = type;
}

void fsys::Sequence::FiltersBunch::set_name(const ustring& name)
{
	this->name = name;
}

void fsys::Sequence::FiltersBunch::set_mask(const ustring& mask)
{
	this->mask = mask;
}

void fsys::Sequence::FiltersBunch::set_size(const Size& minSize, const Size& maxSize)
{
	this->minSize = minSize;
	this->maxSize = maxSize;
}

void fsys::Sequence::FiltersBunch::set_wr_time(const Time& minWrTime, const Size& maxWrTime)
{
	this->minWrTime = minWrTime;
	this->maxWrTime = maxWrTime;
}

void fsys::Sequence::FiltersBunch::set_cr_time(const Time& minCrTime, const Size& maxCrTime)
{
	this->minCrTime = minCrTime;
	this->maxCrTime = maxCrTime;
}

void fsys::Sequence::FiltersBunch::set_ac_time(const Time& minAcTime, const Size& maxAcTime)
{
	this->minAcTime = minAcTime;
	this->maxAcTime = maxAcTime;
}

void fsys::Sequence::FiltersBunch::set_attr(const Attr& enabledAttr, const Attr& disabledAttr)
{
	this->enabledAttr = enabledAttr;
	this->disabledAttr = disabledAttr;
	this->enabledAttr = this->enabledAttr & ~this->disabledAttr;
	LogNoise(L"{ea: 0x%08X, da: 0x%08X}\n", this->enabledAttr, this->disabledAttr);
}

fsys::Sequence::FiltersBunch::Type fsys::Sequence::FiltersBunch::get_type() const
{
	return type;
}
