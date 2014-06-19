#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

#include "MessageManager.hpp"

namespace sync {

	Observer::~Observer()
	{
		m_manager->unregister_all(this);
	}

}
