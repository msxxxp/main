﻿#ifndef _FSYS_SEQUENCE_FILTER_HPP_
#define _FSYS_SEQUENCE_FILTER_HPP_

namespace fsys {

	class Sequence::Filter {
	public:
		enum class Type: ssize_t
		{
			Include,
			Exclude,
		};

		Filter(Type type, const ustring& name);

		void set_type();
		void set_name(const ustring& name);
		void set_mask(const ustring& mask);
		void set_size(const Size& minSize, const Size& maxSize);
		void set_wr_time(const Time& minWrTime, const Size& maxWrTime);
		void set_cr_time(const Time& minCrTime, const Size& maxCrTime);
		void set_ac_time(const Time& minAcTime, const Size& maxAcTime);
		void set_ch_time(const Time& minChTime, const Size& maxChTime);
		void set_attr(const Attr& enabledAttr, const Attr& disabledAttr);

		bool operator ()(const FindStat& stat, Statistics& statistics) const;

	private:
//		bool apply_to_folder(const FindStat& stat, Statistics& statistics) const;
//		bool apply_to_file(const FindStat& stat, Statistics& statistics) const;

		bool apply_attributes(const FindStat& stat, Statistics& statistics) const;
		bool apply_mask(const FindStat& stat, Statistics& statistics) const;

		ustring name;
		ustring mask;
		Type    type;
		Size    minSize, maxSize;
		Time    minWrTime, maxWrTime;
		Time    minCrTime, maxCrTime;
		Time    minAcTime, maxAcTime;
		Time    minChTime, maxChTime;
		Attr    enabledAttr, disabledAttr;
	};

}

#endif