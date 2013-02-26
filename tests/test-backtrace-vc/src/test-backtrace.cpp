#include <iostream>
using namespace std;

#include "backtrace.hpp"

using namespace Base;

void test_backtrace() {
	Base::Backtrace bt;
	for (auto it = bt.begin(); it != bt.end(); ++it) {
		printf("%S\n", it->to_str().c_str());
	}
}

int main() {
	test_backtrace();
	return 0;
}
