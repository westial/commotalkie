#include "CppUTest/TestHarness.h"

extern "C" {
#include "MessageSender.h"
#include "MessageFormatter.h"
}

// -----------------------------------------------------------------------------

unsigned long send_fn(const char* address, const char* content, unsigned long size) {
  return size;
}
unsigned long send_fail_fn(const char* address, const char* content, unsigned long size) {
  return size - 1;
}

TEST_GROUP(Sending) {
};

TEST(Sending, SenderClient) {
  Sender_Create((const void *) send_fn);
  CHECK_EQUAL(12, Sender_Send("address", "01234567890A", 12));
  Sender_Destroy();
}

TEST(Sending, SendMessage) {
  Message message;
  MessageFormatter_Pack("01234567890A", &message);
  MessageSender_Create((const void *) send_fn, "address");
  CHECK_EQUAL(1, MessageSender_Send(&message));
  MessageSender_Destroy();
}

TEST(Sending, SendingFailure) {
  Message message;
  MessageFormatter_Pack("01234567890A", &message);
  MessageSender_Create((const void *) send_fail_fn, "address");
  CHECK_EQUAL(0, MessageSender_Send(&message));
  MessageSender_Destroy();
}