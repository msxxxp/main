#include <FsNode.hpp>

ustring FsNode::get_name() const
{
	return m_name;
}

ustring FsNode::get_full_path() const
{
	if (m_parent) {
		ustring path = m_parent->get_full_path();
		path += Base::PATH_SEPARATOR;
		path += m_name;
		return path;
	}
	return m_name;
}

bool FsNode::path_equals(FsNode * other) const
{
	if (other == nullptr || Cstr::compare_ci(m_name.c_str(), other->m_name.c_str()) != 0) {
		return false;
	}
	if (m_parent) {
		return m_parent->path_equals(other->m_parent.get());
	}
	return true;
}

