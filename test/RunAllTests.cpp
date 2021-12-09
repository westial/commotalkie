#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(Packing);
IMPORT_TEST_GROUP(Validating);
IMPORT_TEST_GROUP(Encrypting);
IMPORT_TEST_GROUP(Publishing);
IMPORT_TEST_GROUP(Subscription);
IMPORT_TEST_GROUP(PullApp);
IMPORT_TEST_GROUP(PublishApp);
IMPORT_TEST_GROUP(PublisherBuilder);
IMPORT_TEST_GROUP(SubscriberBuilder);
IMPORT_TEST_GROUP(DriverConfiguring);
IMPORT_TEST_GROUP(DriverSending);
IMPORT_TEST_GROUP(DriverReceiving);
IMPORT_TEST_GROUP(Timer);
IMPORT_TEST_GROUP(Sleepable);

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}