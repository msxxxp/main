#ifndef BASIS_MATH_FIXED_HPP_
#define BASIS_MATH_FIXED_HPP_

namespace math {

	template<int FractalBits>
	struct fixed
	{
		typedef fixed<FractalBits> this_type;
		typedef ssize_t value_type;
		typedef double floating_point_type;

		value_type value;

		static const value_type FRACTAL_BITS = FractalBits;
		static const value_type BASE_NUMBER = (value_type)1 << FRACTAL_BITS;
//		static const int MAX_INTEGER = (SSIZE_MAX >> BASE_DIGITS);
//		static const int MIN_INTEGER = -1 - MAX_INTEGER;

		fixed();
		fixed(int other);
		fixed(int numerator, int denominator);
		fixed(fraction fract);
		fixed(floating_point_type other);
		fixed(const this_type& other);

		void swap(this_type& other);

		this_type& operator =(int other);
		this_type& operator =(floating_point_type other);
		this_type& operator =(const this_type& other);

		this_type& operator +=(const this_type& other);
		this_type& operator -=(const this_type& other);
		this_type& operator *=(const this_type& other);
		this_type& operator /=(const this_type& other);

		this_type& operator >>=(int count);
		this_type& operator <<=(int count);

		this_type operator -() const;
		operator value_type() const {return value >> FRACTAL_BITS;}
		operator floating_point_type() const {return floating_point_type(value) / this_type::BASE_NUMBER;}

		this_type fraq() const;
		this_type floor() const;
		this_type round() const;
		this_type ceil() const;
		this_type abs() const;
	};

}

#endif
