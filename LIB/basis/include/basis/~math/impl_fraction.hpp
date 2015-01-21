#ifndef BASIS_MATH_IMPL_FRACTION_HPP_
#define BASIS_MATH_IMPL_FRACTION_HPP_

namespace math {

	template<typename FixedType>
	fraction::fraction(const FixedType& fixed):
		numerator(),
		denominator(1)
	{
		auto l_fixed(fixed.abs());

		numerator = l_fixed;
		if (l_fixed.fraq() != FixedType(0))
		{
			const int prec = 1000;
			denominator = prec * FixedType::BASE_NUMBER / l_fixed.fraq().value;
			numerator =  prec + numerator * denominator;
//			denominator = FixedType::BASE_NUMBER;
//			numerator =  l_fixed.value;
			reduce();
		}

		numerator *= math::sign(fixed);
	}

}

#endif
