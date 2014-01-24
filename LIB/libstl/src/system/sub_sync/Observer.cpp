#include <system/configure.hpp>
#include <system/sync.hpp>

#include "MessageManager.hpp"

namespace sync {

	Observer::~Observer()
	{
		m_manager->unregister_all(this);
	}

}
