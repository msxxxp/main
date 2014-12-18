#ifndef _FSYS_SEQUENCE_OPTIONS_HPP_
#define _FSYS_SEQUENCE_OPTIONS_HPP_

#include <basis/simstd/vector>

namespace fsys {

	class Sequence::Options {
	public:
		mutable Statistics statistics;

		Options();

		FiltersBunch& add_filter_bunch(FiltersBunch::Type type, const ustring& name);

		bool apply_filters(const FindStat& stat) const;

	private:
		typedef simstd::vector<FiltersBunch> Filters;
		Filters filters;
	};

}

#endif
