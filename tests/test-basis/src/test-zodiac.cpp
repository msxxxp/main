#include "test-zodiac/test-zodiac.hpp"

#include <tests.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>

#include <cstdlib>
#include <ctime>

void test_zodiac()
{
	LogTrace();

	LogConsoleReport(-1, L"Вносим элемент случайности\n");
	std::srand(std::time(NULL));

	ssize_t ret = Main().main();

	LogNoise(L"-> %Id\n", ret);
}
