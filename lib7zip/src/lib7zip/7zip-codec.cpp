#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>

#include <algorithm>

namespace SevenZip {
	///======================================================================================= Codec
	Codec::Codec(const Lib & arc_lib, size_t idx)
	{
		CheckApiError(arc_lib.get_prop(idx, NArchive::kClassID, guid));
		CheckApiError(arc_lib.get_prop(idx, NArchive::kName, name));
		CheckApiError(arc_lib.get_prop(idx, NArchive::kUpdate, updatable));
		arc_lib.get_prop(idx, NArchive::kExtension, ext);
		arc_lib.get_prop(idx, NArchive::kAddExtension, add_ext);
		arc_lib.get_prop(idx, NArchive::kAssociate, kAssociate);
		arc_lib.get_prop(idx, NArchive::kStartSignature, start_sign);
		arc_lib.get_prop(idx, NArchive::kFinishSignature, finish_sign);
		arc_lib.get_prop(idx, NArchive::kKeepName, kKeepName);
	}

	bool Codec::operator ==(const ustring & name) const
	{
		return this->name == name;
	}

	bool Codec::operator <(const Codec & rhs) const
	{
		return name < rhs.name;
	}

	bool Codec::operator ==(const Codec & rhs) const
	{
		return name == rhs.name;
	}

	bool Codec::operator !=(const Codec & rhs) const
	{
		return name != rhs.name;
	}

	///====================================================================================== Codecs
	Codecs::const_iterator Codecs::at(const ustring & name) const
	{
		auto it = std::find(begin(), end(), name);
		CheckApiThrowError(it != end(), ERROR_NOT_SUPPORTED);
		return it;
	}

	Codecs::Codecs()
	{
	}

	void Codecs::cache(const Lib & lib)
	{
		UInt32 num_formats = 0;
		CheckCom(lib.GetNumberOfFormats(&num_formats));

		clear();

		for (size_t idx = 0; idx < num_formats; ++idx) {
			emplace_back(lib, idx);
		}
	}
}
