#ifndef BASIS_CONFIGURE_OBLIGATORY_MAKE_LL_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_MAKE_LL_HPP_

inline uint64_t make_uint64(uint32_t high, uint32_t low)
{
	return uint64_t(high) << 32 | low;
}

inline uint32_t high_part_64(uint64_t arg64)
{
	return uint32_t(arg64 >> 32);
}

inline uint32_t low_part_64(uint64_t arg64)
{
	return uint32_t(arg64 & 0xFFFFFFFFULL);
}

#endif
