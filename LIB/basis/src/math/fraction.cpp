#include <basis/math.hpp>

math::fraction::fraction(ssize_t numerator, ssize_t denominator):
	numerator(numerator),
	denominator(denominator)
{
}

void math::fraction::reduce()
{
	const int nod = math::nod(math::abs(numerator), math::abs(denominator));
	numerator /= nod;
	denominator /= nod;
}
