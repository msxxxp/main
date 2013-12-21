#ifndef HDLINK_PATH_HPP_
#define HDLINK_PATH_HPP_

#include <FsNode.hpp>

struct Path: public FsNode {
	~Path();

	Path(parent_type parent, const ustring & name);
};

#endif /* HDLINK_PATH_HPP_ */
