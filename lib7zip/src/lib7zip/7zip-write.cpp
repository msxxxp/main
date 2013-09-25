#include <lib7zip/7zip.hpp>
#include <libcom/bstr.hpp>
#include <libext/exception.hpp>
#include <libbase/console.hpp>
#include <libbase/path.hpp>
#include <libbase/logger.hpp>
#include <libbase/pcstr.hpp>

namespace SevenZip {
	DirItem::DirItem(const ustring & file_path, const ustring & file_name) :
		Fsys::Stat(Base::MakePath(file_path, file_name).c_str()),
		path(file_path),
		name(file_name)
	{
	}

	///=========================================================================== ArchiveProperties
	CompressProperties::CompressProperties() :
		level(5),
		solid(false),
		encrypt_header(false),
		AskPassword(false),
		IgnoreErrors(true),
		silent(true)
	{
		LogNoise(L"%Iu, %d, %d\n", level, solid, encrypt_header);
	}

	void CompressProperties::add(const ustring & add_path)
	{
		LogNoise(L"'%s'\n", add_path.c_str());
		ustring path(Base::PathNice(add_path));
		path = Base::Path::get_fullpath(Base::Path::ensure_no_end_separator(path));
		path = Base::Path::ensure_prefix(path);
		LogNoise(L"'%s'\n", path.c_str());
		if (Fsys::is_exist(path)) {
			size_t pos = path.find_last_of(Base::PATH_SEPARATORS);
			if (pos != ustring::npos) {
				base_add(path.substr(0, pos), path.substr(pos + 1));
			}
		}
	}

	void CompressProperties::writeln(PCWSTR str) const
	{
		if (!silent)
			printf(L"%s\n", str);
	}

	void CompressProperties::writeln(const ustring & str) const
	{
		writeln(str.c_str());
	}

	void CompressProperties::printf(PCWSTR format, ...) const
	{
		va_list vl;
		va_start(vl, format);
		Base::Console::vprintf(Base::Console::Handle::OUTPUT, format, vl);
		va_end(vl);
	}

	void CompressProperties::base_add(const ustring & base_path, const ustring & name)
	{
//		LogNoise(L"'%s', '%s'\n", base_path.c_str(), name.c_str());
		emplace_back(base_path, name);
		ustring path(Base::MakePath(base_path, name));
		if (Fsys::is_dir(path)) {
			Fsys::Sequence dir(path);
			for (auto it = dir.begin(); it != dir.end(); ++it) {
				if (it->is_dir()) {
					base_add(base_path, Base::MakePath(name, it->name()));
				} else {
					base_add(base_path, Base::MakePath(name, it->name()));
//					push_back(DirItem(base_path, MakePath(name, it.name())));
				}
			}
		}
	}

	///============================================================================== UpdateCallback
	UpdateCallback::~UpdateCallback()
	{
		LogTrace();
	}

	UpdateCallback::UpdateCallback(const CompressProperties & props, FailedFiles & ffiles) :
		m_props(props),
		m_ffiles(ffiles)
	{
		LogTrace();
	}

	ULONG WINAPI UpdateCallback::AddRef()
	{
		return UnknownImp::AddRef();
	}

	ULONG WINAPI UpdateCallback::Release()
	{
		return UnknownImp::Release();
	}

	HRESULT WINAPI UpdateCallback::QueryInterface(REFIID riid, void** object)
	{
//		LogTrace();
		if (IsEqualIID(riid, IID_IArchiveUpdateCallback2) && object) {
//			LogNoise(L"IID_IArchiveUpdateCallback2\n");
			*object = static_cast<IArchiveUpdateCallback2*>(this);
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_ICryptoGetTextPassword2) && object) {
//			LogNoise(L"IID_ICryptoGetTextPassword2\n");
			*object = static_cast<ICryptoGetTextPassword2*>(this);
			AddRef();
			return S_OK;
		}
		return UnknownImp::QueryInterface(riid, object);
	}

	HRESULT WINAPI UpdateCallback::SetTotal(UInt64 size)
	{
		LogNoise(L"%I64u\n", size);
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::SetCompleted(const UInt64 * completeValue)
	{
		LogNoiseIf(completeValue, L"%I64u\n", *completeValue);
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::GetUpdateItemInfo(UInt32 /*index*/, Int32 * newData, Int32 * newProperties, UInt32 * indexInArchive)
	{
		if (newData)
			*newData = Int32(true);
		if (newProperties)
			*newProperties = Int32(true);
		if (indexInArchive)
			*indexInArchive = (UInt32)-1;
//		LogNoise(L"%u, %d, %d, %d\n", index, *newData, *newProperties, *indexInArchive);
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::GetProperty(UInt32 index, PROPID propID, PROPVARIANT * value)
	{
		Com::PropVariant prop;

		const DirItem & dirItem = m_props.at(index);
		switch (propID) {
			case kpidIsDir:
				prop = dirItem.is_dir();
				break;

			case kpidPosixAttrib:
//				prop = (uint32_t)0777;
				break;

			case kpidMTime:
				prop = dirItem.mtime_ft();
				break;

			case kpidPath:
				prop = dirItem.name.c_str();
				break;

			case kpidUser:
//				prop = L"user";
				break;

			case kpidGroup:
//				prop = L"group";
				break;

			case kpidSize:
				prop = dirItem.is_dir() ? 0ull : dirItem.size();
				break;

			case kpidAttrib:
				prop = (uint32_t)dirItem.attr();
				break;

			case kpidCTime:
				prop = dirItem.ctime_ft();
				break;

			case kpidATime:
				prop = dirItem.atime_ft();
				break;

			case kpidIsAnti:
//				prop = false;
				break;

			case kpidTimeType:
				prop = (uint32_t)NFileTimeType::kWindows;
				break;

			default:
				LogNoise(L"%u, %2u, '%s'\n", index, propID, Base::NamedValues<int>::GetName(ArcItemPropsNames, Base::lengthof(ArcItemPropsNames), propID));
				break;
		}
		Com::PropVariant tmp(prop);
//		LogNoise(L"%u, %2u, '%s' -> '%s'\n", index, propID, Base::NamedValues<int>::GetName(ArcItemPropsNames, Base::lengthof(ArcItemPropsNames), propID), tmp.as_str().c_str());
		prop.detach(value);
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::GetStream(UInt32 index, ISequentialInStream ** inStream)
	try
	{
		const DirItem & dirItem = m_props.at(index);
		m_props.writeln(dirItem.name);

		LogNoise(L"%u, '%s'\n", index, dirItem.name.c_str());
		if (dirItem.is_dir())
			return S_OK;

		try {
			FileReadStream * stream = new FileReadStream(Base::MakePath(dirItem.path, dirItem.name));
			Com::Object<ISequentialInStream>(stream).detach(*inStream);
		} catch (Ext::AbstractError & e) {
			m_ffiles.push_back(FailedFile(dirItem.name, e.code()));
			m_props.writeln(e.what());
			return S_FALSE;
		}
		return S_OK;
	}
	catch (std::exception & /*e*/) {
		return S_FALSE;
	}

	HRESULT WINAPI UpdateCallback::SetOperationResult(Int32 operationResult)
	{
		LogNoise(L"%d\n", operationResult);
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::GetVolumeSize(UInt32 index, UInt64 * size)
	{
		if (index >= m_props.size())
			index = m_props.size() - 1;
		*size = m_props.VolumesSizes.at(index);
		LogNoise(L"%u, %I64u\n", index, *size);
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::GetVolumeStream(UInt32 index, ISequentialOutStream ** volumeStream)
	try
	{
		LogNoise(L"%u\n", index);
		PCWSTR const VOLUME_FORMAT = L"%s.%02d%s";
		WCHAR vname[Base::MAX_PATH_LEN];
		_snwprintf(vname, Base::lengthof(vname), VOLUME_FORMAT, m_props.VolName.c_str(), index + 1, m_props.VolExt.c_str());
//		printf(L"%S [%d, %s]\n", __PRETTY_FUNCTION__, index, vname);
		FileWriteStream * stream(new FileWriteStream(vname/*, CREATE_ALWAYS*/));
		Com::Object < ISequentialOutStream > (stream).detach(*volumeStream);
		return S_OK;
	}
	catch (Ext::AbstractError & e) {
		return S_FALSE;
	}

	HRESULT WINAPI UpdateCallback::CryptoGetTextPassword2(Int32 * passwordIsDefined, BSTR * password)
	{
		if (m_props.password.empty() && m_props.AskPassword) {
			// You can ask real password here from user
			// Password = GetPassword(OutStream);
			// PasswordIsDefined = true;
			m_props.writeln(L"Password is not defined");
			return E_ABORT;
		}
		try {
			*passwordIsDefined = !m_props.password.empty();
			Com::BStr(m_props.password).detach(*password);
			LogNoise(L"%d\n", *passwordIsDefined);
		} catch (Ext::AbstractError & e) {
			return E_ABORT;
		}
		return S_OK;
	}

	///=============================================================================== CreateArchive
	CreateArchive::CreateArchive(const Lib & lib, const ustring & codec) :
		m_lib(lib),
		m_codec(codec)
	{
		LogNoise(L"%p, '%s', '%s'\n", &lib, codec.c_str(), m_lib.codecs().at(codec)->guid.as_str().c_str());
		CheckCom(m_lib.CreateObject(&m_lib.codecs().at(codec)->guid, &IID_IOutArchive, (PVOID* )&m_arc));
	}

	void CreateArchive::compress(const ustring & path)
	{
		LogNoise(L"'%s'\n", path.c_str());
		set_properties();

		Com::Object<IOutStream> outStream(new FileWriteStream(path/* + L"." + m_codec*/, CREATE_NEW));
		Com::Object<IArchiveUpdateCallback2> updateCallback(new UpdateCallback(*this, m_ffiles));

		CheckCom(m_arc->UpdateItems(outStream, CompressProperties::size(), updateCallback));
	}

//	Object<IOutArchive> CreateArchive::operator ->() const
//	{
//		return m_arc;
//	}

	void CreateArchive::set_properties()
	{
		LogNoise(L"level: %Iu, solid: %d, encrypt: %d\n", level, solid, encrypt_header);
		Com::Object<ISetProperties> setProperties;
		m_arc->QueryInterface(IID_ISetProperties, (PVOID*)&setProperties);
		if (setProperties) {
			std::vector<PCWSTR> prop_names;
			std::vector<Com::PropVariant> prop_vals;

			prop_names.push_back(L"x");
			prop_vals.push_back(Com::PropVariant((UInt32)level));
			if (m_codec == L"7z") {
//				prop_names.push_back(L"0"); prop_vals.push_back(PropVariant(m_lib.methods().at(method)->name));
				prop_names.push_back(L"V");
				prop_vals.push_back(Com::PropVariant(true));
				prop_names.push_back(L"tc");
				prop_vals.push_back(Com::PropVariant(true));
				prop_names.push_back(L"s");
				prop_vals.push_back(Com::PropVariant(solid));
				prop_names.push_back(L"he");
				prop_vals.push_back(Com::PropVariant(encrypt_header));
			}
			CheckCom(setProperties->SetProperties(&prop_names[0], &prop_vals[0], prop_names.size()));
		}
	}
}
