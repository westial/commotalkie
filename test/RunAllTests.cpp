#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(Packing);
IMPORT_TEST_GROUP(Validating);

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}