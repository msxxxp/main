#include <tests.hpp>

#include <basis/math.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>

void test_math()
{
	LogWarn(L"\n");

	math::fixed<32> v0;
	math::fixed<32> v1(15, 64);
	math::fixed<32> v_1(-7, 64);
	math::fixed<32> v1_25(1.25);
	math::fixed<32> v_1_25(-1.25);
	math::fixed<32> v1_5(1.5);
	math::fixed<32> v_1_5(-1.5);
	math::fixed<32> v13_14(13.14);
	math::fixed<32> v_13_14(-13.14);
	math::fixed<32> v__13_14(-v_13_14);
//	math::fixed<32> v23__7(23, 7);

	LogReport(L"v0:       %08X.%08X -> %f\n", high_part_64(v0.value), low_part_64(v0.value), double(v0));
	LogReport(L"v1:       %08X.%08X -> %f\n", high_part_64(v1.value), low_part_64(v1.value), double(v1));
	LogReport(L"v_1:      %08X.%08X -> %f\n", high_part_64(v_1.value), low_part_64(v_1.value), double(v_1));
	LogReport(L"v1_25:    %08X.%08X -> %f\n", high_part_64(v1_25.value), low_part_64(v1_25.value), double(v1_25));
	LogReport(L"v_1_25:   %08X.%08X -> %f\n", high_part_64(v_1_25.value), low_part_64(v_1_25.value), double(v_1_25));
	LogReport(L"v1_5:     %08X.%08X -> %f\n", high_part_64(v1_5.value), low_part_64(v1_5.value), double(v1_5));
	LogReport(L"v_1_5:    %08X.%08X -> %f\n", high_part_64(v_1_5.value), low_part_64(v_1_5.value), double(v_1_5));
	LogReport(L"v13_14:   %08X.%08X -> %f\n", high_part_64(v13_14.value), low_part_64(v13_14.value), double(v13_14));
	LogReport(L"v_13_14:  %08X.%08X -> %f\n", high_part_64(v_13_14.value), low_part_64(v_13_14.value), double(v_13_14));
	LogReport(L"v__13_14: %08X.%08X -> %f\n", high_part_64(v__13_14.value), low_part_64(v__13_14.value), double(v__13_14));

	math::fraction sp0(v0);
	math::fraction sp1(v1);
	math::fraction sp_1(v_1);
	math::fraction sp1_25(v1_25);
	math::fraction sp_1_25(v_1_25);
	math::fraction sp1_5(v1_5);
	math::fraction sp_1_5(v_1_5);
	math::fraction sp13_14(v13_14);
	math::fraction sp_13_14(v_13_14);
	math::fraction sp__13_14(v__13_14);
	LogReport(L"sp0:       %Id/%Id\n", sp0.numerator, sp0.denominator);
	LogReport(L"sp1:       %Id/%Id\n", sp1.numerator, sp1.denominator);
	LogReport(L"sp_1:      %Id/%Id\n", sp_1.numerator, sp_1.denominator);
	LogReport(L"sp1_25:    %Id/%Id\n", sp1_25.numerator, sp1_25.denominator);
	LogReport(L"sp_1_25:   %Id/%Id\n", sp_1_25.numerator, sp_1_25.denominator);
	LogReport(L"sp1_5:     %Id/%Id\n", sp1_5.numerator, sp1_5.denominator);
	LogReport(L"sp_1_5:    %Id/%Id\n", sp_1_5.numerator, sp_1_5.denominator);
	LogReport(L"sp13_14:   %Id/%Id\n", sp13_14.numerator, sp13_14.denominator);
	LogReport(L"sp_13_14:  %Id/%Id\n", sp_13_14.numerator, sp_13_14.denominator);
	LogReport(L"sp__13_14: %Id/%Id\n", sp__13_14.numerator, sp__13_14.denominator);
}
