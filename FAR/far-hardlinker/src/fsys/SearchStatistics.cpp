#include <fsys.hpp>

#include <atomic>

namespace fsys {

	struct Sequence::Statistics::Impl {
		std::atomic<uint64_t> filesFound;
		std::atomic<uint64_t> filesFoundSize;
		std::atomic<uint64_t> filesLinksFound;
		std::atomic<uint64_t> filesLinksFoundSize;
		std::atomic<uint64_t> filesIgnoredMinSize;
		std::atomic<uint64_t> filesIgnoredMaxSize;
		std::atomic<uint64_t> filesIgnoredArchive;
		std::atomic<uint64_t> filesIgnoredReadOnly;
		std::atomic<uint64_t> filesIgnoredHidden;
		std::atomic<uint64_t> filesIgnoredSystem;
		std::atomic<uint64_t> filesIgnoredLink;
		std::atomic<uint64_t> filesIgnoredStreamed;
		std::atomic<uint64_t> filesIgnoredCompressed;
		std::atomic<uint64_t> filesIgnoredEncrypted;
		std::atomic<uint64_t> filesIgnoredSparse;
		std::atomic<uint64_t> filesIgnoredTemporary;
		std::atomic<uint64_t> filesIgnoredOffline;
		std::atomic<uint64_t> filesIgnoredZeroSize;
		std::atomic<uint64_t> foldersFound;
		std::atomic<uint64_t> foldersLinksFound;
		std::atomic<uint64_t> foldersIgnored;
		std::atomic<uint64_t> foldersIgnoredArchive;
		std::atomic<uint64_t> foldersIgnoredReadOnly;
		std::atomic<uint64_t> foldersIgnoredHidden;
		std::atomic<uint64_t> foldersIgnoredSystem;
		std::atomic<uint64_t> foldersIgnoredLink;
	};

	Sequence::Statistics::Statistics() :
		m_impl(nullptr)
	{
	}

	void Sequence::Statistics::init_statistics()
	{
		if (!m_impl)
			m_impl = new Impl;
	}

	void Sequence::Statistics::free_statistics()
	{
		if (m_impl) {
			delete m_impl;
			m_impl = nullptr;
		}
	}

	void Sequence::Statistics::folder_found(const FindStat& stat)
	{
		if (m_impl) {
			++m_impl->foldersFound;
			if (stat.is_link()) {
				++m_impl->foldersLinksFound;
			} else {
				++m_impl->foldersFound;
			}
		}
	}

	void Sequence::Statistics::folder_ignored()
	{
	}

	void Sequence::Statistics::folder_ignored_archive()
	{
	}

	void Sequence::Statistics::folder_ignored_readonly()
	{
	}

	void Sequence::Statistics::folder_ignored_hidden()
	{
	}

	void Sequence::Statistics::folder_ignored_system()
	{
	}

	void Sequence::Statistics::folder_ignored_link()
	{
	}

	void Sequence::Statistics::file_found(const FindStat& stat)
	{
		if (m_impl) {
			++m_impl->filesFound;
			if (stat.is_link()) {
				++m_impl->filesLinksFound;
				m_impl->filesLinksFoundSize += stat.size();
			} else {
				++m_impl->filesFound;
				m_impl->filesFoundSize += stat.size();
			}
		}
	}

	void Sequence::Statistics::file_ignored()
	{
	}

	void Sequence::Statistics::file_ignored_to_small()
	{
	}

	void Sequence::Statistics::file_ignored_to_big()
	{
	}

	void Sequence::Statistics::file_ignored_archive()
	{
	}

	void Sequence::Statistics::file_ignored_readonly()
	{
	}

	void Sequence::Statistics::file_ignored_hidden()
	{
	}

	void Sequence::Statistics::file_ignored_system()
	{
	}

	void Sequence::Statistics::file_ignored_link()
	{
	}

	void Sequence::Statistics::file_ignored_streamed()
	{
	}

	void Sequence::Statistics::file_ignored_compressed()
	{
	}

	void Sequence::Statistics::file_ignored_encrypted()
	{
	}

	void Sequence::Statistics::file_ignored_sparse()
	{
	}

	void Sequence::Statistics::file_ignored_temporary()
	{
	}

	void Sequence::Statistics::file_ignored_offline()
	{
	}

	void Sequence::Statistics::file_ignored_zero()
	{
	}

}
