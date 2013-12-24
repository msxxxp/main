#ifndef HDLINK_PATH_HPP_
#define HDLINK_PATH_HPP_

#include <FsNode.hpp>

struct Path: public FsNode {
	~Path();

	Path(const ustring & name, parent_type parent = parent_type(nullptr));
};

#endif /* HDLINK_PATH_HPP_ */
