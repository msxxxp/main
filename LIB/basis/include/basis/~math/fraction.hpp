#ifndef BASIS_MATH_FRACTION_HPP_
#define BASIS_MATH_FRACTION_HPP_

namespace math {

	struct fraction
	{
		ssize_t numerator;
		ssize_t denominator;

		fraction(ssize_t numerator, ssize_t denominator = 1);

		template<typename FixedType>
		fraction(const FixedType& fixed);

		void reduce();
	};

}

#endif
