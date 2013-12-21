#ifndef HDLINK_FSNODE_HPP_
#define HDLINK_FSNODE_HPP_

#include <libbase/std.hpp>
#include <libbase/string.hpp>

#include <memory>

struct FsNode;
typedef std::shared_ptr<FsNode> parent_type;

struct FsNode {
	virtual ~FsNode() = default;

	FsNode(parent_type parent, const ustring & name);

	bool path_equals(FsNode * other) const;

	ustring get_name() const;

	ustring get_full_path() const;

protected:
	parent_type m_parent;
	ustring m_name;
};

#endif /* HDLINK_FSNODE_HPP_ */
