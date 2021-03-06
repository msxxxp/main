#include <lib7zip/7zip.hpp>
#include <libcom/bstr.hpp>
#include <libext/exception.hpp>
#include <libbase/path.hpp>

/*#define UNKNOWN_IMPL_ITF(iid) \
	if (riid == IID_##iid) { *object = this; AddRef(); return S_OK; }*/

namespace SevenZip {
	///============================================================================= ExtractCallback
	struct ExtractCallback::CurrItem {
		ustring path;
		Com::Object<FileWriteStream> stream;
		ArchiveSequence::iterator item;
		Int32 mode;

		CurrItem(Int32 m, const ustring & p, ArchiveSequence::iterator i) :
			path(p + i.path()),
			item(i),
			mode(m)
		{
		}
	};

	ULONG WINAPI ExtractCallback::AddRef()
	{
		return UnknownImp::AddRef();
	}

	ULONG WINAPI ExtractCallback::Release()
	{
		return UnknownImp::Release();
	}

	HRESULT WINAPI ExtractCallback::QueryInterface(REFIID riid, void** object) {
//		LogTrace();
		if (IsEqualIID(riid, IID_IArchiveExtractCallback) && object) {
//			LogNoise(L"IID_IArchiveExtractCallback\n");
			*object = static_cast<IArchiveExtractCallback*>(this);
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_ICryptoGetTextPassword) && object) {
//			LogNoise(L"IID_ICryptoGetTextPassword\n");
			*object = static_cast<ICryptoGetTextPassword*>(this);
			AddRef();
			return S_OK;
		}
		return UnknownImp::QueryInterface(riid, object);
	}

	ExtractCallback::~ExtractCallback()
	{
		LogTrace();
	}

	ExtractCallback::ExtractCallback(const ArchiveSequence & arc, const ustring & dest_path, const ustring & pass) :
		Password(pass),
		m_wa(arc),
		m_dest(/*Base::MakeGoodPath(*/dest_path)
	{
		LogNoise(L"'%s'\n", dest_path.c_str());
		Base::Path::ensure_end_separator(m_dest);
	}

	HRESULT WINAPI ExtractCallback::SetTotal(UInt64 size)
	{
		UNUSED(size);

		LogNoise(L"%I64u\n", size);

		return S_OK;
	}

	HRESULT WINAPI ExtractCallback::SetCompleted(const UInt64 *completeValue)
	{
		UNUSED(completeValue);

		LogNoise(L"%I64u\n", completeValue ? *completeValue : 0ULL);

//		return processed size
//		if (completeValue) {
//			printf(L"ArchiveExtractCallback::SetCompleted(%d)\n", *completeValue);
//		}
		return S_OK;
	}

	HRESULT WINAPI ExtractCallback::GetStream(UInt32 index, ISequentialOutStream ** outStream, Int32 askExtractMode)
	{
		LogNoise(L"%u, %u\n", index, askExtractMode);

		*outStream = nullptr;

		m_curr.reset(new CurrItem(askExtractMode, m_dest, m_wa.at(index)));

		if (askExtractMode != NArchive::NExtract::NAskMode::kExtract)
			return S_OK;

		try {
			if (m_curr->item.is_dir()) {
				Fsys::Directory::create_full(m_curr->path);
			} else {
				// Create folders for file
				size_t pos = m_curr->path.find_last_of(Base::PATH_SEPARATORS);
				if (pos != ustring::npos) {
					Fsys::Directory::create_full(m_curr->path.substr(0, pos));
				}

				if (Fsys::File::is_exist(m_curr->path)) {
					Fsys::File::del(m_curr->path);
				}

				FileWriteStream * tmp(new FileWriteStream(m_curr->path, true));
				Com::Object<FileWriteStream> stream(tmp);
				m_curr->stream = stream;
				stream.detach(tmp);
				*outStream = tmp;
			}
		} catch (Ext::AbstractError & e) {
			return E_ABORT;
		}
		return S_OK;
	}

	HRESULT WINAPI ExtractCallback::PrepareOperation(Int32 askExtractMode)
	{
		LogNoise(L"%u\n", askExtractMode);
		switch (askExtractMode) {
			case NArchive::NExtract::NAskMode::kExtract:
//				printf(L"Extract: %s\n", m_curr->item.path().c_str());
				break;
			case NArchive::NExtract::NAskMode::kTest:
//				printf(L"Test: %s\n", m_curr->item.path().c_str());
				break;
			case NArchive::NExtract::NAskMode::kSkip:
//				printf(L"Skip: %s\n", m_curr->item.path().c_str());
				break;
		};
		return S_OK;
	}

	HRESULT WINAPI ExtractCallback::SetOperationResult(Int32 operationResult)
	{
		LogNoise(L"%d\n", operationResult);

		if (operationResult != NArchive::NExtract::NOperationResult::kOK) {
			failed_files.push_back(FailedFile(m_curr->item.path(), operationResult));
		} else {
			if (m_curr->mode == NArchive::NExtract::NAskMode::kExtract) {
				Fsys::set_attr(m_curr->path, m_curr->item.attr());
				if (m_curr->stream) {
					m_curr->stream->set_mtime(m_curr->item.mtime());
				}
			}
		}
		return S_OK;
	}

	HRESULT WINAPI ExtractCallback::CryptoGetTextPassword(BSTR *pass)
	{
		LogTrace();
		if (Password.empty()) {
			// You can ask real password here from user
			// PrintError("Password is not defined");
			//		return E_ABORT;
		}
		Com::BStr(Password).detach(*pass);
		return S_OK;
	}
}
