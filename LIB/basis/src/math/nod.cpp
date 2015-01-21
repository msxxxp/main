#include <basis/math.hpp>

ssize_t math::nod(ssize_t left, ssize_t right)
{
	while (left && right) {
		if (left < right)
			right %= left;
		else
			left %= right;
	}

	return (left + right);
}
