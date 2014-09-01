#ifndef _FSYS_NODE_HPP_
#define _FSYS_NODE_HPP_

#include <fsys.hpp>

#include <basis/std/vector>
#include <vector>

namespace fsys {

	class Node;

	typedef simstd::shared_ptr<Node> Node_t;
	typedef std::vector<Node_t> Folders_t;

	class Node {
	public:
		virtual ~Node() = default;

		Node(const ustring & name);

		Node(const ustring & name, Node_t parent);

		bool is_equal_path(Node * other) const;

		ustring get_name() const;

		virtual ustring get_full_path() const;

	protected:
		ustring  m_name;
		Node_t      m_parent;
	};

	class Folder: public Node {
	public:
		~Folder();

		Folder(const ustring & name);

		Folder(const ustring & name, Node_t parent);
	};

	class File: public Node {
	public:
		typedef simstd::vector<char> HashVal;

		~File();

		File(const fsys::Sequence::FindStat & info, Node_t parent);

		ustring get_full_path() const;

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

		static bool count_hash(simstd::vector<char> & out, ustring path, uint64_t first, uint64_t last);

		uint64_t             m_size;
		uint64_t             m_mtime;
		size_t               m_attr;
		mutable HashVal      m_headHash;
		mutable HashVal      m_tailHash;
		mutable HashVal      m_wholeHash;
		mutable size_t       m_volume_sn;
		mutable uint64_t     m_inode;
		mutable ustring m_full_path;

		friend bool compare_head_hash(const File & file1, const File & file2);
		friend bool compare_tail_hash(const File & file1, const File & file2);
		friend bool compare_whole_hash(const File & file1, const File & file2);
		friend bool compare_hash(const File & file1, const File & file2);
	};

	bool compare_hash(const File & file1, const File & file2);

	typedef simstd::shared_ptr<File> File_t;
	typedef std::vector<File_t> Files_t;

	inline Node::Node(const ustring & name) :
		m_name(name),
		m_parent(nullptr)
	{

	}

	inline Node::Node(const ustring & name, Node_t parent) :
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
