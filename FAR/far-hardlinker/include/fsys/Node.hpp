#ifndef _FSYS_NODE_HPP_
#define _FSYS_NODE_HPP_

#include <fsys.hpp>

#include <basis/simstd/list>
#include <basis/simstd/vector>

namespace fsys {

	class Node;
	class Folder;
	class File;

	typedef simstd::shared_ptr<Node> Node_t;
//	typedef simstd::vector<Node_t>   Nodes_t;
	typedef simstd::list<Node_t>     Folders_t;
	typedef simstd::shared_ptr<File> File_t;
	typedef simstd::vector<File_t>   Files_t;

	class Node {
	public:
		virtual ~Node() = default;

		Node(const ustring& name);

		Node(const ustring& name, Node_t parent);

		bool is_equal_path(Node_t other) const;

		ustring get_name() const;

		virtual ustring get_full_path() const;

	protected:
		ustring  m_name;
		Node_t   m_parent;
	};

	class Folder: public Node {
	public:
		~Folder();

		Folder(const ustring& name);

		Folder(const ustring& name, Node_t parent);
	};

	class File: public Node {
	public:
		typedef simstd::vector<char> hash_type;

		~File();

		File(const fsys::Sequence::FindStat & info, Node_t parent);

		ustring get_full_path() const;

		size_t attr() const;

		uint64_t size() const;

		uint64_t mtime() const;

		size_t volume_sn() const;

		uint64_t inode() const;

		const hash_type & get_head_hash() const;

		const hash_type & get_tail_hash() const;

		const hash_type & get_whole_hash() const;

	private:
		void refresh_handle_info() const;

		static bool count_hash(hash_type & out, ustring path, uint64_t first, uint64_t last);

		uint64_t             m_size;
		uint64_t             m_mtime;
		size_t               m_attr;
		mutable hash_type    m_headHash;
		mutable hash_type    m_tailHash;
		mutable hash_type    m_wholeHash;
		mutable size_t       m_volume_sn;
		mutable uint64_t     m_inode;
		mutable ustring m_full_path;

		friend bool compare_head_hash(const File & file1, const File & file2);
		friend bool compare_tail_hash(const File & file1, const File & file2);
		friend bool compare_whole_hash(const File & file1, const File & file2);
		friend bool compare_hash(const File & file1, const File & file2);
	};

	bool compare_hash(const File & file1, const File & file2);

	inline Node::Node(const ustring& name) :
		m_name(name),
		m_parent(nullptr)
	{
	}

	inline Node::Node(const ustring& name, Node_t parent) :
		m_name(name),
		m_parent(parent)
	{
	}

	inline ustring Node::get_name() const
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
