#ifndef PATTERNS_DESTROYABLE_HPP_
#define PATTERNS_DESTROYABLE_HPP_

namespace Pattern {
	struct Destroyable
	{
		virtual ~Destroyable() = default;

		virtual void destroy() const = 0;
	};
}

#endif
