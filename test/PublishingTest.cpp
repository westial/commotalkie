#include "CppUTest/TestHarness.h"

#include "MessageFormatter.h"
#include "MessagePublisher.h"

#include "helper/PublishTestHelper.cpp"

// -----------------------------------------------------------------------------

TEST_GROUP(Publishing){};

TEST(Publishing, SendText) {
  Sender_Create((const void *)push_fn);
  CHECK_EQUAL(12, Sender_Send((const unsigned char *)"address",
                              (const unsigned char *)"0123456789AB", 12));
  Sender_Destroy();
}

TEST(Publishing, PublishMessage) {
  Message message;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &message);
  MessagePublisher_Create((const void *)push_fn);
  CHECK_EQUAL(
      1, MessagePublisher_Push((const unsigned char *)"address", &message));
  MessagePublisher_Destroy();
}

TEST(Publishing, PublishToCorrectTopic) {
  Message message;
  MessageFormatter_Pack((unsigned char *)"message=topi", &message);
  MessagePublisher_Create((const void *)spy_address_on_push_fn);
  MessagePublisher_Push((const unsigned char *)"message=topi", &message);
  MessagePublisher_Destroy();
}

TEST(Publishing, FailPublishing) {
  Message message;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &message);
  MessagePublisher_Create((const void *)push_fail_fn);
  CHECK_EQUAL(
      0, MessagePublisher_Push((const unsigned char *)"address", &message));
  MessagePublisher_Destroy();
}