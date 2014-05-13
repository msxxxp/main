#ifndef LIBBASE_BIT_STR_HPP_
#define LIBBASE_BIT_STR_HPP_

#include <extra/bits.hpp>
#include <system/sstr.hpp>

#include <simstd/string>

namespace bits {

	template<typename Type>
	struct BitMask {
		static Type from_str(const ustring & in, size_t lim = 0)
		{
			// count bits from 1
			Type Result = 0;
			intmax_t bit = 0;
			ustring tmp(in);
			lim = bits::Bits::Limit<Type>(lim);
			while (sstr::Cut(tmp, bit)) {
				if (bits::Bits::GOOD_BIT<Type>(--bit))
					bits::Bits::set(Result, bit);
			}
			return Result;
		}

		static Type from_str_0(const ustring & in, size_t lim = 0)
		{
			// count bits from zero
			Type Result = 0;
			intmax_t bit = 0;
			ustring tmp(in);
			lim = bits::Bits::Limit < Type > (lim);
			while (sstr::Cut(tmp, bit)) {
				if (bits::Bits::GOOD_BIT< Type > (bit))
					bits::Bits::set(Result, bit);
			}
			return Result;
		}

		static ustring to_str(Type in, size_t lim = 0)
		{
			// count bits from 1
			ustring Result;
			lim = bits::Bits::Limit < Type > (lim);
			for (size_t bit = 0; bit < lim; ++bit) {
				if (bits::Bits::check(in, bit)) {
					sstr::inplace::add_word(Result, to_str(bit + 1), L",");
				}
			}
			return Result;
		}

		static ustring to_str_0(Type in, size_t lim = 0)
		{
			// count bits from zero
			ustring Result;
			lim = bits::Bits::Limit < Type > (lim);
			for (size_t bit = 0; bit < lim; ++bit) {
				if (bits::Bits::check(in, bit)) {
					sstr::inplace::add_word(Result, to_str(bit), L",");
				}
			}
			return Result;
		}

		static ustring to_str_bin(Type in, size_t lim = 0)
		{
			ustring Result;
			uintmax_t flag = (uintmax_t)1 << (bits::Bits::Limit < Type > (lim) - 1);
			while (flag) {
				Result += bits::Flags::check(in, (Type)flag) ? L'1' : L'0';
				flag >>= 1;
			}
			return Result;
		}

		static ustring to_str_num(Type in, size_t lim = 0)
		{
			ustring Result;
			uintmax_t flag = (uintmax_t)1 << (bits::Bits::Limit < Type > (lim) - 1);
			while (flag) {
				if (bits::Flags::check(in, (Type)flag)) {
					wchar_t buf[64];
					cstr::convert_num(buf, flag);
					sstr::inplace::add_word(Result, buf, L",");
				}
				flag >>= 1;
			}
			return Result;
		}
	};

}

#endif
