#include "CppUTest/TestHarness.h"
extern "C" {
#include <string.h>
#include "MessageSubscriber.h"
}

// -----------------------------------------------------------------------------

static int stub_message_fn(const char* address, const char* content, const int size) {
  memcpy((void *)content, "0123456789AB", MESSAGE_LENGTH);
  return MESSAGE_LENGTH;
}
static int mock_address_fn(const char* address, const char* content, const int size) {
  MEMCMP_EQUAL(address, "address", 7);
  return 7;
}

TEST_GROUP(Subscription) {
};

TEST(Subscription, SuccessedPull) {
  Result result;
  Message message;
  MessageSubscriber_Create((void *) stub_message_fn, "address");
  result = MessageSubscriber_Pull(&message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Success, result);
  MEMCMP_EQUAL(message.meta, "012", MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(message.body, "3456789AB", MESSAGE_BODY_LENGTH);
};

TEST(Subscription, PullFromCorrectTopic) {
  Message message;
  MessageSubscriber_Create((void *) mock_address_fn, "address");
  MessageSubscriber_Pull(&message);
  MessageSubscriber_Destroy();
};