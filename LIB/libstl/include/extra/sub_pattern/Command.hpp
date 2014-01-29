#ifndef LIBSTL_EXTRA_PATTERN_COMMAND_HPP_
#define LIBSTL_EXTRA_PATTERN_COMMAND_HPP_

namespace pattern {
	struct Command
	{
		virtual ~Command() = default;

		virtual ssize_t execute() = 0;
	};
}

#endif
