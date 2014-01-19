#ifndef PATTERNS_COMMAND_HPP_
#define PATTERNS_COMMAND_HPP_

#include <simstl/types.hpp>

namespace Pattern {
	struct Command
	{
		virtual ~Command() = default;

		virtual ssize_t execute() = 0;
	};
}

#endif
