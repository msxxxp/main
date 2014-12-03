#include <file_system.hpp>
#include <global.hpp>
#include <crypt.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/cstr.hpp>

namespace file_system {

	bool Node::is_equal_path(Node * other) const
	{
		if (other == nullptr || cstr::compare_ci(m_name.c_str(), other->m_name.c_str()) != 0)
			return false;
		if (m_parent)
			return m_parent->is_equal_path(other->m_parent.get());
		return true;
	}

	simstd::wstring Node::get_full_path() const
	{
		if (m_parent) {
			simstd::wstring path = m_parent->get_full_path();
			path += PATH_SEPARATOR;
			path += m_name;
			return path;
		}
		return m_name;
	}

	Folder::~Folder()
	{
		++global::statistics().folderObjDestroyed;
		LogDebug(L"'%s'\n", m_name.c_str());
	}

	Folder::Folder(const simstd::wstring & name) :
		Node(name, Node_t(nullptr))
	{
		LogDebug(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjCreated;
	}

	Folder::Folder(const simstd::wstring & name, Node_t parent) :
		Node(name, parent)
	{
		LogDebug(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjCreated;
	}

	File::~File()
	{
		++global::statistics().fileObjDestroyed;
		LogDebug(L"'%s'\n", m_name.c_str());
	}

//	File::File(const fsys::Sequence::FindStat & info, Node_t parent) :
//		Node(info.name(), parent),
//		m_size(info.size()),
//		m_mtime(info.mtime()),
//		m_attr(info.attr()),
//		m_volume_sn(),
//		m_inode()
//	{
//		LogDebug(L"'%s'\n", m_name.c_str());
//		++global::statistics().fileObjCreated;
//	}

	simstd::wstring File::get_full_path() const
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

	const File::HashVal & File::get_head_hash() const
	{
		if (m_headHash.empty()) {
			if (count_hash(m_headHash, get_full_path(), 0, global::options().firstBlockHash))
				++global::statistics().headHashesCalculated;
			else
				++global::statistics().headHashCalculationFailed;
		}
		return m_headHash;
	}

	const File::HashVal & File::get_tail_hash() const
	{
		if (m_tailHash.empty()) {
			if (count_hash(m_tailHash, get_full_path(), size() - global::options().firstBlockHash, size()))
				++global::statistics().tailHashesCalculated;
			else
				++global::statistics().tailHashCalculationFailed;
		}
		return m_tailHash;
	}

	const File::HashVal & File::get_whole_hash() const
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
//		fsys::File::Facade file(get_full_path().c_str());
//		m_volume_sn = file.get_volume_sn();
//		m_inode     = file.get_inode();
		LogDebug(L"0x%08X 0x%016X '%s'\n", m_volume_sn, m_inode, m_name.c_str());
	}

	bool File::count_hash(std::vector<char> & out, simstd::wstring path, uint64_t first, uint64_t last)
	{
//		fsys::File::Facade file(path.c_str());
//		file.set_position(first);
//
//		uint64_t size = last - first;
//		crypt::Hash hasher(*global::vars().cryptProvider, CALG_SHA1);
//		while (size) {
//			const size_t BUF_SIZE = 1024 * 1024;
//			char buf[BUF_SIZE];
//			size_t bytes_to_read = std::min(BUF_SIZE, size);
//			size_t bytes_read = file.read(buf, bytes_to_read);
//			hasher.process(buf, bytes_read);
//			size -= bytes_read;
//		}
//
//		out.resize(hasher.get_size());
//		hasher.get_hash(&out[0], out.size());
		return true;
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
