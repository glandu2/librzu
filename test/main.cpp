#include "RunTests.h"
#include "gtest/gtest.h"

static void initConfigs() {}

int main(int argc, char** argv) {
	TestRunner testRunner(argc, argv, &initConfigs);

	return testRunner.runTests();
}
