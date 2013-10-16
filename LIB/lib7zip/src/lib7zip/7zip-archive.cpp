#include <lib7zip/7zip.hpp>
#include <libcom/bstr.hpp>
#include <libext/exception.hpp>
#include <liblog/logger.hpp>

namespace SevenZip {
	///===================================================================================== Archive
	Archive::Archive(const Lib & lib, const ustring & path)
	{
		LogNoise(L"%p, '%s', 0x%Iu\n", &lib, path.c_str());
		open_archive(lib, path);
		init_props();
	}

	Archive::Archive(Com::Object<IInArchive> arc) :
		m_arc(arc)
	{
		LogTrace();
		init_props();
	}

	void Archive::open_archive(const Lib & lib, const ustring & path)
	{
		LogNoise(L"%p, '%s'\n", &lib, path.c_str());
		Com::Object<IInStream> stream(new FileReadStream(path));
		Com::Object<IArchiveOpenCallback> openCallback(new OpenCallback);
		for (auto it = lib.codecs().begin(); it != lib.codecs().end(); ++it) {
			LogNoise(L"codec: '%s'\n", it->name.c_str());
			CheckCom(stream->Seek(0, STREAM_SEEK_SET, nullptr));
			CheckCom(lib.CreateObject(&it->guid, &IID_IInArchive, (PVOID*)&m_arc));
			if (m_arc->Open(stream, &max_check_size, openCallback) == S_OK) {
				m_codec = it;
				return;
			}
		}
		CheckApiError(ERROR_INVALID_DATA);
	}

	void Archive::init_props()
	{
		LogTrace();
		CheckApiError(m_arc->GetNumberOfItems(&m_size));
		m_props.cache(m_arc);
	}

	const Codec & Archive::codec() const
	{
		return *(m_codec);
	}

	const Props & Archive::props() const
	{
		return m_props;
	}

	Com::Object<IInArchive> Archive::operator ->() const
	{
		return m_arc;
	}

	Archive::operator Com::Object<IInArchive>() const
	{
		return m_arc;
	}

	bool Archive::empty() const
	{
		return m_size == 0;
	}

	size_t Archive::size() const
	{
		return m_size;
	}

	///===================================================================================== Archive
	ArchiveSequence::ArchiveSequence(const Lib & lib, const ustring & path, flags_type flags) :
		Archive(lib, path),
		m_flags(flags)
	{
	}

	ArchiveSequence::ArchiveSequence(const Lib & lib, const ustring & path, const ustring & /*mask*/, flags_type flags) :
		Archive(lib, path),
		m_flags(flags)
	{
	}

	ArchiveSequence::ArchiveSequence(Com::Object<IInArchive> arc, flags_type flags) :
		Archive(arc),
		m_flags(flags)
	{
	}

	ArchiveSequence::const_iterator ArchiveSequence::begin() const
	{
		return const_iterator(*this);
	}

	ArchiveSequence::const_iterator ArchiveSequence::end() const
	{
		return const_iterator();
	}

	ArchiveSequence::const_iterator ArchiveSequence::at(size_t index) const
	{
		if (index >= size())
			CheckCom(TYPE_E_OUTOFBOUNDS);
		return const_iterator(*this, index);
	}

	ArchiveSequence::flags_type ArchiveSequence::flags() const
	{
		return m_flags;
	}

	ArchiveSequence::const_input_iterator::this_type & ArchiveSequence::const_input_iterator::operator ++()
	{
		flags_type flags = m_seq->flags();
		while (true) {
			if (++m_index >= m_seq->size()) {
				m_end = true;
				break;
			}

			if ((flags & skipHidden) && ((attr() & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)) {
				continue;
			}

			if ((flags & skipDirs) && is_dir()) {
				continue;
			}

			if ((flags & skipFiles) && is_file()) {
				continue;
			}
			break;
		}
		return *this;
	}

	ArchiveSequence::const_input_iterator::this_type ArchiveSequence::const_input_iterator::operator ++(int)
	{
		this_type ret(*this);
		operator ++();
		return ret;
	}

	ustring ArchiveSequence::const_input_iterator::path() const
	{
		return get_prop(kpidPath).as_str();
	}

	uint64_t ArchiveSequence::const_input_iterator::size() const
	{
		return get_prop(kpidSize).as_uint();
	}

	size_t ArchiveSequence::const_input_iterator::attr() const
	{
		return get_prop(kpidAttrib).as_uint();
	}

	FILETIME ArchiveSequence::const_input_iterator::mtime() const
	{
		return get_prop(kpidMTime).as_time();
	}

	bool ArchiveSequence::const_input_iterator::is_file() const
	{
		return !is_dir();
	}

	bool ArchiveSequence::const_input_iterator::is_dir() const
	{
		return get_prop(kpidIsDir).as_bool();
	}

	size_t ArchiveSequence::const_input_iterator::get_props_count() const
	{
		UInt32 props = 0;
		static_cast<Com::Object<IInArchive> >(*m_seq)->GetNumberOfProperties(&props);
		return props;
	}

	bool ArchiveSequence::const_input_iterator::get_prop_info(size_t index, ustring & name, PROPID & id) const
	{
		Com::BStr m_nm;
		VARTYPE type;
		HRESULT err = static_cast<Com::Object<IInArchive> >(*m_seq)->GetPropertyInfo(index, &m_nm, &id, &type);
		if (err == S_OK && m_nm)
			name = m_nm.c_str();
		return err == S_OK;
	}

	Com::PropVariant ArchiveSequence::const_input_iterator::get_prop(PROPID id) const
	{
		Com::PropVariant prop;
		static_cast<Com::Object<IInArchive> >(*m_seq)->GetProperty(m_index, id, prop.ref());
		return prop;
	}

	bool ArchiveSequence::const_input_iterator::operator ==(const this_type & rhs) const
	{
		if (m_end && rhs.m_end)
			return true;
		return m_seq == rhs.m_seq && m_index == rhs.m_index;
	}

	bool ArchiveSequence::const_input_iterator::operator !=(const this_type & rhs) const
	{
		return !operator==(rhs);
	}

	ArchiveSequence::const_input_iterator::const_input_iterator() :
		m_seq(nullptr),
		m_index(0),
		m_end(true)
	{
	}

	ArchiveSequence::const_input_iterator::const_input_iterator(const ArchiveSequence & seq) :
		m_seq((ArchiveSequence*)&seq),
		m_index(0),
		m_end(!m_seq->size())
	{
	}

	ArchiveSequence::const_input_iterator::const_input_iterator(const ArchiveSequence & seq, UInt32 index) :
		m_seq((ArchiveSequence*)&seq),
		m_index(index),
		m_end(!m_seq->size() || index >= m_seq->size())
	{
	}

	///============================================================================== ArchiveExtract
	ArchiveExtract::ArchiveExtract(const Lib & lib, const ustring & arcPath, const ustring & destPath):
		m_archive(lib, arcPath),
		m_dest(destPath)
	{
	}

	ssize_t ArchiveExtract::execute()
	{
		LogTrace();
		Com::Object<IArchiveExtractCallback> extractCallback(new ExtractCallback(m_archive, m_dest));
		CheckCom(m_archive->Extract(nullptr, (UInt32 )-1, false, extractCallback));
		return 0;
	}

	///================================================================================= ArchiveTest
	ArchiveTest::ArchiveTest(const Lib & lib, const ustring & path):
		m_archive(lib, path)
	{
	}

	ssize_t ArchiveTest::execute()
	{
		LogTrace();
		ExtractCallback * callback(new ExtractCallback(m_archive));
		LogTrace();
		Com::Object<IArchiveExtractCallback> extractCallback(callback);
		LogTrace();
		m_archive->Extract(nullptr, (UInt32)-1, true, extractCallback);
		LogTrace();
		return static_cast<ssize_t>(callback->failed_files.size());
	}
}
