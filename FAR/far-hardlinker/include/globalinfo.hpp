/**
	hardlinker: Search duplicates and make hardlinks
	FAR3-lua plugin

	© 2014 Andrew Grechkin

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

#ifndef _GLOBALINFO_HPP_
#define _GLOBALINFO_HPP_

#include <far3/plugin.hpp>

#include <basis/ext/pattern.hpp>
#include <basis/simstd/string>

struct FarGlobalInfo: public far3::GlobalInfo_i, private pattern::Uncopyable {
	~FarGlobalInfo();

	FarGlobalInfo();

	const wchar_t* get_author() const override;

	const wchar_t* get_description() const override;

	const GUID* get_guid() const override;

	const wchar_t* get_title() const override;

	VersionInfo get_version() const override;

	VersionInfo get_min_version() const override;

	far3::Plugin_i* CreatePlugin(const PluginStartupInfo* info) const override;

	void load_settings();

	void save_settings() const;

	wchar_t prefix[32];

	enum class CheckBoxMasks: uint64_t {
		Operation        = bin<000000000000000000000111>::value, // 0b000000000000000000000111,
		PathMask         = bin<000000000000000000001000>::value,  // 0b000000000000000000001000,
		DoHardlink       = bin<000000000000000000010000>::value,  // 0b000000000000000000010000,
		DoRecursive      = bin<000000000000000000100000>::value,  // 0b000000000000000000100000,
		AskConfirmation  = bin<000000000000000001000000>::value,  // 0b000000000000000001000000,
		Euristic         = bin<000000000000000010000000>::value,  // 0b000000000000000010000000,
	};

	int64_t m_cbOperation;
	int64_t m_cbMask;
	int64_t m_cbDoHardlink;
	int64_t m_cbDoRecursive;
	int64_t m_cbAskConfirmation;
	int64_t m_cbEuristic;
	int64_t m_cbRestrictionFileTime;
	int64_t m_cbRestrictionFileAttributes;
	int64_t m_cbFilterFileSize;
	int64_t m_cbFilterFileReadOnly;
	int64_t m_cbFilterFileHidden;
	int64_t m_cbFilterFileSystem;
	int64_t m_cbFilterFileLink;
	int64_t m_cbFilterDirReadOnly;
	int64_t m_cbFilterDirHidden;
	int64_t m_cbFilterDirSystem;
	int64_t m_cbFilterDirLink;
	ustring m_edMask;
};

FarGlobalInfo * get_global_info();

#endif
