#include "CppUTest/TestHarness.h"

extern "C" {
#include "MessagePublisher.h"
#include "MessageFormatter.h"
}

// -----------------------------------------------------------------------------

unsigned long send_fn(const char* address, const char* content, unsigned long size) {
  return size;
}
unsigned long send_fail_fn(const char* address, const char* content, unsigned long size) {
  return size - 1;
}

TEST_GROUP(Publishing) {
};

TEST(Publishing, SendText) {
  Sender_Create((const void *) send_fn);
  CHECK_EQUAL(12, Sender_Send("address", "01234567890A", 12));
  Sender_Destroy();
}

TEST(Publishing, PublishMessage) {
  Message message;
  MessageFormatter_Pack("01234567890A", &message);
  MessagePublisher_Create((const void *) send_fn, "address");
  CHECK_EQUAL(1, MessagePublisher_Send(&message));
  MessagePublisher_Destroy();
}

TEST(Publishing, FailPublishing) {
  Message message;
  MessageFormatter_Pack("01234567890A", &message);
  MessagePublisher_Create((const void *) send_fail_fn, "address");
  CHECK_EQUAL(0, MessagePublisher_Send(&message));
  MessagePublisher_Destroy();
}