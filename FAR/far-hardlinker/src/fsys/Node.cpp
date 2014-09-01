#include <fsys.hpp>
#include <global.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/cstr.hpp>

namespace fsys {

	bool Node::is_equal_path(Node * other) const
	{
		if (other == nullptr || cstr::compare_ci(m_name.c_str(), other->m_name.c_str()) != 0)
			return false;
		if (m_parent)
			return m_parent->is_equal_path(other->m_parent.get());
		return true;
	}

	ustring Node::get_full_path() const
	{
		if (m_parent) {
			ustring path = m_parent->get_full_path();
			path += PATH_SEPARATOR;
			path += m_name;
			return path;
		}
		return m_name;
	}

}
