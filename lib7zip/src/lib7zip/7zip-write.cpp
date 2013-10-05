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

	DirItem::DirItem(const ustring & file_name, bool virtualItem) :
		name(file_name)
	{
		UNUSED(virtualItem);
	}

	///=========================================================================== ArchiveProperties
	CompressProperties::CompressProperties() :
		level(CompressLevel::NORMAL),
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

	void CompressProperties::add_virtual(const ustring & name)
	{
		LogNoise(L"'%s'\n", name.c_str());
		emplace_back(name, true);
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
		m_ffiles(ffiles),
		m_totalSize(0ULL),
		m_completedSize(0ULL)
	{
		LogTrace();
	}

	UpdateCallback::UpdateCallback(const CompressProperties & props, FailedFiles & ffiles, const Com::Object<ISequentialInStream> & midStream) :
		m_props(props),
		m_ffiles(ffiles),
		m_midStream(midStream),
		m_totalSize(0ULL),
		m_completedSize(0ULL)
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
		m_totalSize = size;
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::SetCompleted(const UInt64 * completeValue)
	{
		LogNoise(L"%I64u\n", completeValue ? *completeValue : 0ULL);
		if (completeValue)
			m_completedSize = *completeValue;
		return S_OK;
	}

	HRESULT WINAPI UpdateCallback::GetUpdateItemInfo(UInt32 index, Int32 * newData, Int32 * newProperties, UInt32 * indexInArchive)
	{
		UNUSED(index);

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
	try {
		const DirItem & dirItem = m_props.at(index);

		Com::PropVariant prop;
		switch (propID) {
			case kpidIsDir:
				prop = dirItem.is_dir();
				break;
			case kpidPath:
				prop = dirItem.name.c_str();
				break;
			case kpidSize:
				prop = dirItem.is_dir() ? 0ULL : dirItem.size();
				break;
			case kpidAttrib:
				prop = (uint32_t)dirItem.attr();
				break;
			case kpidMTime:
				prop = dirItem.mtime_ft();
				break;
			case kpidCTime:
				prop = dirItem.ctime_ft();
				break;
			case kpidATime:
				prop = dirItem.atime_ft();
				break;
			case kpidTimeType:
				prop = (uint32_t)NFileTimeType::kWindows;
				break;
			case kpidIsAnti:
//				prop = false;
				break;
			case kpidUser:
				prop = L"user";
				break;
			case kpidGroup:
				prop = L"group";
				break;
			case kpidPosixAttrib:
				prop = (uint32_t)0777;
				break;
			default:
				LogWarn(L"%u, %2u, '%s'\n", index, propID, Base::NamedValues<int>::GetName(ArcItemPropsNames, Base::lengthof(ArcItemPropsNames), propID));
				break;
		}
		prop.detach(value);
		return S_OK;
	} catch (Ext::AbstractError & e) {
		LogError(L"%u, %2u, '%s' -> '%s'\n", index, propID, Base::NamedValues<int>::GetName(ArcItemPropsNames, Base::lengthof(ArcItemPropsNames), propID), e.what().c_str());
		return S_FALSE;
	} catch (std::out_of_range & e) {
		LogError(L"%S\n", e.what());
		return S_FALSE;
	}

	HRESULT WINAPI UpdateCallback::GetStream(UInt32 index, ISequentialInStream ** inStream)
	try
	{
		const DirItem & dirItem = m_props.at(index);
		m_props.writeln(dirItem.name);

		LogNoise(L"%u, '%s'\n", index, dirItem.name.c_str());
		if (dirItem.is_dir())
			return S_OK;

		if (m_props.size() == 1 && m_midStream) {
			m_midStream.detach(*inStream);
		} else {
			try {
				FileReadStream * stream = new FileReadStream(Base::MakePath(dirItem.path, dirItem.name));
				Com::Object<ISequentialInStream>(stream).detach(*inStream);
			} catch (Ext::AbstractError & e) {
				LogError(L"%u -> '%s'\n", index, e.what().c_str());
				m_ffiles.push_back(FailedFile(dirItem.name, e.code()));
				m_props.writeln(e.what());
				return S_FALSE;
			}
		}
		return S_OK;
	}
	catch (std::exception & e) {
		UNUSED(e);
		LogError(L"%S\n", e.what());
		return S_FALSE;
	}

	HRESULT WINAPI UpdateCallback::SetOperationResult(Int32 operationResult)
	{
		UNUSED(operationResult);
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
		Com::Object<ISequentialOutStream>(stream).detach(*volumeStream);
		return S_OK;
	}
	catch (Ext::AbstractError & e) {
		LogError(L"%u -> '%s'\n", index, e.what().c_str());
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
	CreateArchive::CreateArchive(const Lib & lib, const ustring & destPath, const CompressProperties & destProperties) :
		m_lib(lib),
		m_destPath(destPath),
		m_destProperties(destProperties)
	{
		LogNoise(L"%p, '%s', '%s'\n", &lib, m_destProperties.codec.c_str(), m_lib.codecs().at(m_destProperties.codec)->guid.as_str().c_str());
		CheckCom(m_lib.CreateObject(&m_lib.codecs().at(m_destProperties.codec)->guid, &IID_IOutArchive, (void**)&m_destArchive));
	}

	ssize_t CreateArchive::execute()
	{
		LogNoise(L"'%s'\n", m_destPath.c_str());
		Com::Object<IOutStream> outStream(new FileWriteStream(m_destPath/* + L"." + m_properties.codec*/, CREATE_NEW));

		set_properties(m_destArchive, m_destProperties);
		Com::Object<IArchiveUpdateCallback2> updateCallback(new UpdateCallback(m_destProperties, m_ffiles));

		CheckCom(m_destArchive->UpdateItems(outStream, m_destProperties.size(), updateCallback));
		return 0;
	}

//	Object<IOutArchive> CreateArchive::operator ->() const
//	{
//		return m_arc;
//	}

	void CreateArchive::set_properties(Com::Object<IOutArchive> arc, const CompressProperties & props)
	{
		LogNoise(L"level: %Id, solid: %d, encrypt: %d\n", props.level, props.solid, props.encrypt_header);
		Com::Object<ISetProperties> setProperties;
		arc->QueryInterface(IID_ISetProperties, (PVOID*)&setProperties);
		if (setProperties) {
			std::vector<PCWSTR> prop_names;
			std::vector<Com::PropVariant> prop_vals;

			prop_names.push_back(L"x");
			prop_vals.push_back(Com::PropVariant(static_cast<UInt32>(props.level)));
			if (props.codec == L"7z") {
//				prop_names.push_back(L"0"); prop_vals.push_back(PropVariant(m_lib.methods().at(method)->name));
				prop_names.push_back(L"V");
				prop_vals.push_back(Com::PropVariant(true));
				prop_names.push_back(L"tc");
				prop_vals.push_back(Com::PropVariant(true));
				prop_names.push_back(L"s");
				prop_vals.push_back(Com::PropVariant(props.solid));
				prop_names.push_back(L"he");
				prop_vals.push_back(Com::PropVariant(props.encrypt_header));
			}
			CheckCom(setProperties->SetProperties(&prop_names[0], &prop_vals[0], prop_names.size()));
		}
	}

}
