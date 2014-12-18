#include <fsys.hpp>
#include <global.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/cstr.hpp>

namespace fsys {

	File::~File()
	{
		LogTraceObj();
		LogNoise(L"'%s'\n", m_name.c_str());
		++global::statistics().fileObjectsDestroyed;
	}

	File::File(const fsys::Sequence::FindStat& info, Node_t parent) :
		Node(info.name(), parent),
		m_size(info.size()),
		m_mtime(info.mtime()),
		m_attr(info.attr()),
		m_volume_sn(),
		m_inode()
	{
		LogTraceObj();
		LogNoise(L"'%s'\n", m_name.c_str());
		++global::statistics().fileObjectsCreated;
	}

	ustring File::get_full_path() const
	{
		if (m_full_path.empty())
			m_full_path = Node::get_full_path();
		return m_full_path;
	}

	size_t File::volume_sn() const
	{
		if (m_volume_sn == decltype(m_volume_sn)())
			refresh_handle_info();
		return m_volume_sn;
	}

	uint64_t File::inode() const
	{
		if (m_inode == decltype(m_inode)())
			refresh_handle_info();
		return m_inode;
	}

	const File::hash_type & File::get_head_hash() const
	{
		if (m_headHash.empty()) {
			if (count_hash(m_headHash, get_full_path(), 0, global::options().firstBlockHash))
				++global::statistics().headHashesCalculated;
			else
				++global::statistics().headHashCalculationFailed;
		}
		return m_headHash;
	}

	const File::hash_type & File::get_tail_hash() const
	{
		if (m_tailHash.empty()) {
			if (count_hash(m_tailHash, get_full_path(), size() - global::options().firstBlockHash, size()))
				++global::statistics().tailHashesCalculated;
			else
				++global::statistics().tailHashCalculationFailed;
		}
		return m_tailHash;
	}

	const File::hash_type & File::get_whole_hash() const
	{
		if (m_wholeHash.empty()) {
			if (count_hash(m_wholeHash, get_full_path(), 0, size()))
				++global::statistics().wholeHashesCalculated;
			else
				++global::statistics().wholeHashCalculationFailed;
		}
		return m_wholeHash;
	}

	void File::refresh_handle_info() const
	{
		fsys::Stat stat(fsys::stat(get_full_path().c_str()));
		if (stat) {
			m_volume_sn = stat->volume_sn();
			m_inode     = stat->inode();
		}
		LogDebug(L"vol: 0x%08X, inode: 0x%016X '%s'\n", m_volume_sn, m_inode, m_name.c_str());
	}

	bool File::count_hash(hash_type & out, ustring path, uint64_t first, uint64_t last)
	{
		auto file(fsys::file::open(path));
		if (file && file->set_position(static_cast<int64_t>(first), fsys::file::Seek::FromBeginOfFile)) {
			crypt::HashHolder hasher(crypt::hash(global::vars().cryptProvider));
			if (hasher) {
				uint64_t size = last - first;
				while (size) {
					const size_t BUF_SIZE = 1024 * 1024;
					char buf[BUF_SIZE];
					size_t bytes_to_read = simstd::min(BUF_SIZE, size);
					size_t bytes_read = 0;
					file->read(buf, bytes_to_read, bytes_read);
					hasher->process(buf, bytes_read);
					size -= bytes_read;
				}

				out.resize(hasher->get_size());
				hasher->get_hash(&out[0], out.size());
			}
		}

		return false;
	}

	bool compare_head_hash(const File & file1, const File & file2)
	{
		return !file1.get_head_hash().empty() && file1.get_head_hash() == file2.get_head_hash();
	}

	bool compare_tail_hash(const File & file1, const File & file2)
	{
		return !file1.get_tail_hash().empty() && file1.get_tail_hash() == file2.get_tail_hash();
	}

	bool compare_whole_hash(const File & file1, const File & file2)
	{
		return !file1.get_whole_hash().empty() && file1.get_whole_hash() == file2.get_whole_hash();
	}

	bool compare_hash(const File & file1, const File & file2)
	{
		if (file1.size() <= (global::options().firstBlockHash * 16))
			return compare_whole_hash(file1, file2);

		return compare_head_hash(file1, file2) && compare_tail_hash(file1, file2) && compare_whole_hash(file1, file2);
	}

}
