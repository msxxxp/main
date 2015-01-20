#include <tests.hpp>

#include <basis/math.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <stdio.h>

void test_math()
{
	LogWarn(L"\n");

	math::fixed<32> v0;
//	math::fixed<32> v1(14, 3);
//	math::fixed<32> v_1(-14, 3);
	math::fixed<32> v1(13, 3);
	math::fixed<32> v_1(-13, 3);
	math::fixed<32> v1_25(1.25);
	math::fixed<32> v_1_25(-1.25);
	math::fixed<32> v1_5(1.5);
	math::fixed<32> v_1_5(-1.5);
	math::fixed<32> v13_14(13.125);
	math::fixed<32> v_13_14(-13.125);
	math::fixed<32> v__13_14(-v_13_14);
//	math::fixed<32> v23__7(23, 7);

	LogReport(L"v0:       %08X.%08X -> %d\n", high_part_64(v0.value), low_part_64(v0.value), int(1000 * double(v0)));
	LogReport(L"v1:       %08X.%08X -> %d\n", high_part_64(v1.value), low_part_64(v1.value), int(1000 * double(v1)));
	LogReport(L"v_1:      %08X.%08X -> %d\n", high_part_64(v_1.value), low_part_64(v_1.value), int(1000 * double(v_1)));
	LogReport(L"v1_25:    %08X.%08X -> %d\n", high_part_64(v1_25.value), low_part_64(v1_25.value), int(1000 * double(v1_25)));
	LogReport(L"v_1_25:   %08X.%08X -> %d\n", high_part_64(v_1_25.value), low_part_64(v_1_25.value), int(1000 * double(v_1_25)));
	LogReport(L"v1_5:     %08X.%08X -> %d\n", high_part_64(v1_5.value), low_part_64(v1_5.value), int(1000 * double(v1_5)));
	LogReport(L"v_1_5:    %08X.%08X -> %d\n", high_part_64(v_1_5.value), low_part_64(v_1_5.value), int(1000 * double(v_1_5)));
	LogReport(L"v13_14:   %08X.%08X -> %d\n", high_part_64(v13_14.value), low_part_64(v13_14.value), int(1000 * double(v13_14)));
	LogReport(L"v_13_14:  %08X.%08X -> %d\n", high_part_64(v_13_14.value), low_part_64(v_13_14.value), int(1000 * double(v_13_14)));
	LogReport(L"v__13_14: %08X.%08X -> %d\n", high_part_64(v__13_14.value), low_part_64(v__13_14.value), int(1000 * double(v__13_14)));
	printf("v0:       %f\n", double(v0));
	printf("v1:       %f\n", double(v1));
	printf("v_1:      %f\n", double(v_1));
	printf("v1_25:    %f\n", double(v1_25));
	printf("v_1_25:   %f\n", double(v_1_25));
	printf("v1_5:     %f\n", double(v1_5));
	printf("v_1_5:    %f\n", double(v_1_5));
	printf("v13_14:   %f\n", double(v13_14));
	printf("v_13_14:  %f\n", double(v_13_14));
	printf("v__13_14: %f\n", double(v__13_14));

	math::fraction sp1(v1);
	math::fraction sp_1(v_1);
	math::fraction sp1_25(v1_25);
	math::fraction sp_1_25(v_1_25);
	math::fraction sp1_5(v1_5);
	math::fraction sp_1_5(v_1_5);
	math::fraction sp13_14(v13_14);
	math::fraction sp_13_14(v_13_14);
	math::fraction sp__13_14(v__13_14);
	LogReport(L"sp1:       %d/%d\n", sp1.numerator, sp1.denominator);
	LogReport(L"sp_1:      %d/%d\n", sp_1.numerator, sp_1.denominator);
	LogReport(L"sp1_25:    %d/%d\n", sp1_25.numerator, sp1_25.denominator);
	LogReport(L"sp_1_25:   %d/%d\n", sp_1_25.numerator, sp_1_25.denominator);
	LogReport(L"sp1_5:     %d/%d\n", sp1_5.numerator, sp1_5.denominator);
	LogReport(L"sp_1_5:    %d/%d\n", sp_1_5.numerator, sp_1_5.denominator);
	LogReport(L"sp13_14:   %d/%d\n", sp13_14.numerator, sp13_14.denominator);
	LogReport(L"sp_13_14:  %d/%d\n", sp_13_14.numerator, sp_13_14.denominator);
	LogReport(L"sp__13_14: %d/%d\n", sp__13_14.numerator, sp__13_14.denominator);
}
