#ifndef BASIS_MATH_IMPL_FIXED_HPP_
#define BASIS_MATH_IMPL_FIXED_HPP_

namespace math {

	template<int FractalBits>
	fixed<FractalBits>::fixed():
		value()
	{
	}

	template<int FractalBits>
	fixed<FractalBits>::fixed(int other):
		value(value_type(other) << FRACTAL_BITS)
	{
	}

	template<int FractalBits>
	fixed<FractalBits>::fixed(int numerator, int denominator)
	{
		value_type sign = (numerator < 0 && 0 < denominator) || (0 < numerator && denominator < 0) ? -1 : 1;
		value_type l_num = numerator < 0 ? -numerator : numerator;
		value_type l_den = denominator < 0 ? -denominator : denominator;

		value = ((l_num / l_den) << FRACTAL_BITS) | ((l_num % l_den) * BASE_NUMBER / l_den);
		value *= sign;
	}

	template<int FractalBits>
	fixed<FractalBits>::fixed(fraction fract):
		fixed(fract.numerator, fract.denominator)
	{
	}

	template<int FractalBits>
	fixed<FractalBits>::fixed(floating_point_type other):
		value(value_type(other * BASE_NUMBER))
	{
	}

	template<int FractalBits>
	fixed<FractalBits>::fixed(const this_type& other):
		value(other.value)
	{
	}

	template<int FractalBits>
	void fixed<FractalBits>::swap(this_type& other)
	{
		auto tmp = value;
		value = other.value;
		other.value = tmp;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator =(int other)
	{
		this_type(other).swap(*this);
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator =(floating_point_type other)
	{
		this_type(other).swap(*this);
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator =(const this_type& other)
	{
		value = other.value;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator +=(const this_type& other)
	{
		value += other.value;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator -=(const this_type& other)
	{
		value -= other.value;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator *=(const this_type& other)
	{
		value *= other;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator /=(const this_type& other)
	{
		value /= other;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator <<=(int count)
	{
		value <<= count;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type& fixed<FractalBits>::operator >>=(int count)
	{
		value >>= count;
		return *this;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type fixed<FractalBits>::fraq() const
	{
		this_type ret;
		ret.value = value & (this_type::BASE_NUMBER - 1);
		return ret;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type fixed<FractalBits>::floor() const
	{
		this_type ret;
		ret.value = value & ~(this_type::BASE_NUMBER - 1);
		return ret;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type fixed<FractalBits>::round() const
	{
		this_type ret;
		ret.value = (value + this_type::BASE_NUMBER / 2) & ~(this_type::BASE_NUMBER - 1);
		return ret;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type fixed<FractalBits>::ceil() const
	{
		this_type ret;
		ret.value = (value + this_type::BASE_NUMBER - 1) & ~(this_type::BASE_NUMBER - 1);
		return ret;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type fixed<FractalBits>::abs() const
	{
		this_type ret;
		ret.value = value < 0 ? -value : value;
		return ret;
	}

	template<int FractalBits>
	typename fixed<FractalBits>::this_type fixed<FractalBits>::operator -() const
	{
		this_type ret;
		ret.value = -value;
		return ret;
	}

	template<int FractalBits>
	bool operator ==(const fixed<FractalBits>& left, const fixed<FractalBits>& right)
	{
		return left.value == right.value;
	}

	template<int FractalBits>
	bool operator !=(const fixed<FractalBits>& left, const fixed<FractalBits>& right)
	{
		return !operator ==(left, right);
	}

	template<int FractalBits>
	bool operator <(const fixed<FractalBits>& left, const fixed<FractalBits>& right)
	{
		return left.value < right.value;
	}

	template<int FractalBits>
	bool operator >(const fixed<FractalBits>& left, const fixed<FractalBits>& right)
	{
		return operator <(right, left);
	}

	template<int FractalBits>
	bool operator <=(const fixed<FractalBits>& left, const fixed<FractalBits>& right)
	{
		return !operator <(right, left);
	}

	template<int FractalBits>
	bool operator >=(const fixed<FractalBits>& left, const fixed<FractalBits>& right)
	{
		return !operator <(left, right);
	}

}

#endif
