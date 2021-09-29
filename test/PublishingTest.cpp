#include "CppUTest/TestHarness.h"

extern "C" {
#include "MessagePublisher.h"
#include "MessageFormatter.h"
}

// -----------------------------------------------------------------------------

static unsigned long push_fn(const char*, const char*, unsigned long);
static unsigned long mock_address_fn(const char*, const char*, unsigned long);
static unsigned long push_fail_fn(const char*, const char*, unsigned long);

// -----------------------------------------------------------------------------

unsigned long push_fn(const char* address, const char* content, unsigned long size) {
  return size;
}
unsigned long mock_address_fn(const char* address, const char* content, unsigned long size) {
  MEMCMP_EQUAL(address, content, size);
  return size;
}
unsigned long push_fail_fn(const char* address, const char* content, unsigned long size) {
  return size - 1;
}

// -----------------------------------------------------------------------------

TEST_GROUP(Publishing) {
};

TEST(Publishing, SendText) {
  Sender_Create((const void *) push_fn);
  CHECK_EQUAL(12, Sender_Send("address", "0123456789AB", 12));
  Sender_Destroy();
}

TEST(Publishing, PublishMessage) {
  Message message;
  MessageFormatter_Pack("0123456789AB", &message);
  MessagePublisher_Create((const void *) push_fn);
  CHECK_EQUAL(1, MessagePublisher_Push("address", &message));
  MessagePublisher_Destroy();
}

TEST(Publishing, PublishToCorrectTopic) {
  Message message;
  MessageFormatter_Pack("message=topi", &message);
  MessagePublisher_Create((const void *) mock_address_fn);
  MessagePublisher_Push("message=topi", &message);
  MessagePublisher_Destroy();
}

TEST(Publishing, FailPublishing) {
  Message message;
  MessageFormatter_Pack("0123456789AB", &message);
  MessagePublisher_Create((const void *) push_fail_fn);
  CHECK_EQUAL(0, MessagePublisher_Push("address", &message));
  MessagePublisher_Destroy();
}