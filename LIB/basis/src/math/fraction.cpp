#include <basis/math.hpp>

#include <basis/sys/logger.hpp>

math::fraction::fraction(ssize_t numerator, ssize_t denominator):
	numerator(numerator),
	denominator(denominator)
{
}

void math::fraction::reduce()
{
	const auto nod = math::nod(math::abs(numerator), math::abs(denominator));
	numerator /= nod;
	denominator /= nod;
}
