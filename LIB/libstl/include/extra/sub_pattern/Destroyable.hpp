#ifndef LIBSTL_EXTRA_PATTERN_DESTROYABLE_HPP_
#define LIBSTL_EXTRA_PATTERN_DESTROYABLE_HPP_

namespace pattern {
	struct Destroyable
	{
		virtual ~Destroyable() = default;

		virtual void destroy() const = 0;
	};
}

#endif
