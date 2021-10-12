#include "CppUTest/TestHarness.h"

#include <cstring>
#include "MessageSubscriber.h"
#include "MessageFormatter.h"
#include "MessagePublisher.h"


// -----------------------------------------------------------------------------

static int stub_message_fn(const char*, const char*, int);
static int mock_address_fn(const char*, const char*, int);
static unsigned long stub_push_fn(const char*, const char*, unsigned long);
static int stub_pull_fn(const char*, const char*, int);
static int stub_force_error_pull_fn(const char*, const char*, int);
static int stub_pull_nothing_yet_fn(const char*, const char*, int);
static unsigned long fake_epoch_ms_fn();
static unsigned long stub_progressive_epoch_ms_fn();

static char stub_message_content[MESSAGE_LENGTH];
static unsigned long nothing_until_zero;
static unsigned long progressive_ms;

// -----------------------------------------------------------------------------

int stub_message_fn(const char* address, const char* content, const int size) {
  memcpy((void *)content, "0123456789AB", MESSAGE_LENGTH);
  return MESSAGE_LENGTH;
}
int mock_address_fn(const char* address, const char* content, const int size) {
  MEMCMP_EQUAL(address, "address", 7);
  return 7;
}

unsigned long stub_push_fn(const char* address, const char* content, unsigned long size) {
  memcpy(stub_message_content, content, size);
  return size;
}

int stub_pull_fn(const char* address, const char* content, const int size) {
  memcpy((void *)content, stub_message_content, size);
  return size;
}

int stub_force_error_pull_fn(const char* address, const char* content, const int size) {
  return -1;
}

int stub_pull_nothing_yet_fn(const char* address, const char* content, const int size) {
  if (--nothing_until_zero) return 0;
  else return MESSAGE_LENGTH;
}

unsigned long fake_epoch_ms_fn() {
  return 100;
}

unsigned long stub_progressive_epoch_ms_fn() {
  // It returns 1 for the Timer_Start and 1001 for the Timer_GetMillis
  return progressive_ms += 1000;
}

// -----------------------------------------------------------------------------

TEST_GROUP(Subscription) {
  void setup() override {
    progressive_ms = 1;
    nothing_until_zero = 10000;
  }
};

TEST(Subscription, Timeout) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *) stub_message_fn,
      (void *) stub_progressive_epoch_ms_fn);
  MessageSubscriber_CountDown(999);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Timeout, result);
};

TEST(Subscription, NoTimeout) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *) stub_pull_nothing_yet_fn,
      (void *) stub_progressive_epoch_ms_fn);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(0, nothing_until_zero);
};

TEST(Subscription, IOError) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *) stub_force_error_pull_fn,
      (void *) fake_epoch_ms_fn);
  MessageSubscriber_CountDown(999);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(IOError, result);
};

TEST(Subscription, SucceededPull) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *) stub_message_fn,
      (void *) fake_epoch_ms_fn);
  MessageSubscriber_CountDown(999);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Success, result);
  MEMCMP_EQUAL(message.meta, "012", MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(message.body, "3456789AB", MESSAGE_BODY_LENGTH);
};

TEST(Subscription, PullFromCorrectTopic) {
  Message message;
  MessageSubscriber_Create(
      (void *) mock_address_fn,
      (void *) fake_epoch_ms_fn);
  MessageSubscriber_CountDown(999);
  MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
};

TEST(Subscription, PushAndPull) {
  Message sent_message;
  MessageFormatter_Pack("0123456789AB", &sent_message);
  MessagePublisher_Create((const void *) stub_push_fn);
  MessagePublisher_Push("address", &sent_message);
  MessagePublisher_Destroy();
  Message received_message;
  MessageSubscriber_Create(
      (void *) stub_pull_fn,
      (void *) fake_epoch_ms_fn);
  MessageSubscriber_CountDown(999);
  MessageSubscriber_Pull("address", &received_message);
  MessageSubscriber_Destroy();
  MEMCMP_EQUAL(sent_message.meta, received_message.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(sent_message.body, received_message.body, MESSAGE_BODY_LENGTH);
};