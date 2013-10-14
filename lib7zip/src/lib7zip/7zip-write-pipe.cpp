#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>
#include <liblog/logger.hpp>
#include <libbase/Thread.hpp>

namespace SevenZip {
	struct ArchiveRoutine : public Base::ThreadRoutine_i {
		Com::Object<IOutArchive> outArchive;
		Com::Object<IOutStream> outStream;
		Com::Object<IArchiveUpdateCallback2> updateCallback;
		size_t size;

		ArchiveRoutine();

		size_t run(void * data) override;
	};

	ArchiveRoutine::ArchiveRoutine():
		size()
	{
		LogTrace();
	}

	size_t ArchiveRoutine::run(void * data)
	{
		UNUSED(data);
		LogTrace();
		return outArchive->UpdateItems(outStream, size, updateCallback);
	}

	///========================================================================== CreateArchivePiped
	CreateArchivePiped::CreateArchivePiped(const Lib & lib, const CompressProperties & midProperties, const ustring & destPath, const CompressProperties & destProperties) :
		CreateArchive(lib, destPath, destProperties),
		m_midProperties(midProperties)
	{
		LogNoise(L"%p, '%s', '%s'\n", &lib, m_midProperties.codec.c_str(), m_lib.codecs().at(m_midProperties.codec)->guid.as_str().c_str());
		CheckCom(m_lib.CreateObject(&m_lib.codecs().at(m_midProperties.codec)->guid, &IID_IOutArchive, (void**)&m_midArchive));
	}

	ssize_t CreateArchivePiped::execute()
	{
		LogNoise(L"'%s'\n", m_destPath.c_str());
		Com::Object<VirtualReadWriteStream> virtualStream(new VirtualReadWriteStream);
		Com::Object<FileWriteStream> destStream(new FileWriteStream(m_destPath, CREATE_NEW));
		destStream->set_del_on_close(true);

		set_properties(m_destArchive, m_destProperties);
		Com::Object<UpdateCallback> destUpdateCallback(new UpdateCallback(m_destProperties, m_ffiles, virtualStream));

		ArchiveRoutine packRoutine;
		packRoutine.outArchive = m_destArchive;
		packRoutine.outStream = destStream;
		packRoutine.updateCallback = destUpdateCallback;
		packRoutine.size = m_destProperties.size();

		Base::Thread packThread(&packRoutine);

		try {
			set_properties(m_midArchive, m_midProperties);
			Com::Object<UpdateCallback> midUpdateCallback(new UpdateCallback(m_midProperties, m_ffiles));
			CheckCom(m_midArchive->UpdateItems(virtualStream, m_midProperties.size(), midUpdateCallback));
			destStream->set_del_on_close(false);
			virtualStream->close_write();
			packThread.wait();
		} catch (Ext::AbstractError & e) {
			virtualStream->close_write();
			packThread.wait();
			throw;
		}

		return packThread.get_exitcode();
	}
}
