#ifndef HDLINK_FILE_SYSTEM_HPP_
#define HDLINK_FILE_SYSTEM_HPP_

#include <basis/ext/pattern.hpp>
#include <basis/sys/logger.hpp>
#include <basis/std/memory>
#include <basis/std/string>

#include <vector>

namespace file_system {
	class Node;
	typedef simstd::shared_ptr<Node> Node_t;
	typedef std::vector<Node_t> Folders_t;

	class Node {
	public:
		virtual ~Node() = default;

		Node(const simstd::wstring & name);

		Node(const simstd::wstring & name, Node_t parent);

		bool is_equal_path(Node * other) const;

		simstd::wstring get_name() const;

		virtual simstd::wstring get_full_path() const;

	protected:
		simstd::wstring  m_name;
		Node_t      m_parent;
	};

	class Folder: public Node {
	public:
		~Folder();

		Folder(const simstd::wstring & name);

		Folder(const simstd::wstring & name, Node_t parent);
	};

	class File: public Node {
	public:
		typedef std::vector<char> HashVal;

		~File();

//		File(const fsys::Sequence::FindStat & info, Node_t parent);

		simstd::wstring get_full_path() const;

		size_t attr() const;

		uint64_t size() const;

		uint64_t mtime() const;

		size_t volume_sn() const;

		uint64_t inode() const;

		const HashVal & get_head_hash() const;

		const HashVal & get_tail_hash() const;

		const HashVal & get_whole_hash() const;

	private:
		void refresh_handle_info() const;

		static bool count_hash(std::vector<char> & out, simstd::wstring path, uint64_t first, uint64_t last);

		uint64_t             m_size;
		uint64_t             m_mtime;
		size_t               m_attr;
		mutable HashVal      m_headHash;
		mutable HashVal      m_tailHash;
		mutable HashVal      m_wholeHash;
		mutable size_t       m_volume_sn;
		mutable uint64_t     m_inode;
		mutable simstd::wstring m_full_path;

		friend bool compare_head_hash(const File & file1, const File & file2);
		friend bool compare_tail_hash(const File & file1, const File & file2);
		friend bool compare_whole_hash(const File & file1, const File & file2);
		friend bool compare_hash(const File & file1, const File & file2);
	};

	bool compare_hash(const File & file1, const File & file2);

	typedef simstd::shared_ptr<File> File_t;
	typedef std::vector<File_t> Files_t;

	inline Node::Node(const simstd::wstring & name) :
		m_name(name),
		m_parent(nullptr)
	{
	}

	inline Node::Node(const simstd::wstring & name, Node_t parent) :
		m_name(name),
		m_parent(parent)
	{
	}

	inline simstd::wstring Node::get_name() const
	{
		return m_name;
	}

	inline size_t File::attr() const
	{
		return m_attr;
	}

	inline uint64_t File::size() const
	{
		return m_size;
	}

	inline uint64_t File::mtime() const
	{
		return m_mtime;
	}
}

#endif
