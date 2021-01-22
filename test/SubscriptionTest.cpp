#include "CppUTest/TestHarness.h"
extern "C" {
#include <string.h>
#include "MessageSubscriber.h"
#include "MessageFormatter.h"
#include "MessagePublisher.h"
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

static char stub_message_content[MESSAGE_LENGTH];

static unsigned long stub_push_fn(const char* address, const char* content, unsigned long size) {
  memcpy(stub_message_content, content, size);
  return size;
}

static int stub_pull_fn(const char* address, const char* content, const int size) {
  memcpy((void *)content, stub_message_content, size);
  return size;
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

TEST(Subscription, PushAndPull) {
  Message sent_message;
  MessageFormatter_Pack("0123456789AB", &sent_message);
  MessagePublisher_Create((const void *) stub_push_fn, "address");
  MessagePublisher_Push(&sent_message);
  MessagePublisher_Destroy();
  Message received_message;
  MessageSubscriber_Create((void *) stub_pull_fn, "address");
  MessageSubscriber_Pull(&received_message);
  MessageSubscriber_Destroy();
  MEMCMP_EQUAL(sent_message.meta, received_message.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(sent_message.body, received_message.body, MESSAGE_BODY_LENGTH);
};