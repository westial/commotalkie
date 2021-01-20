#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(Packing);
IMPORT_TEST_GROUP(Validating);
IMPORT_TEST_GROUP(Encrypting);
IMPORT_TEST_GROUP(Publishing);
IMPORT_TEST_GROUP(Subscription);

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}