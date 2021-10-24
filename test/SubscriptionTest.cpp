#include "CppUTest/TestHarness.h"

#include "MessageFormatter.h"
#include "MessagePublisher.h"
#include "MessageSubscriber.h"
#include <cstring>

#include "helper/TimerTestHelper.cpp"

// -----------------------------------------------------------------------------

static int stub_message_fn(const char *, const char *, unsigned long);
static int mock_address_fn(const char *, const char *, int);
static unsigned long stub_push_fn(const char *, const char *, unsigned long);
static int stub_pull_fn(const char *, const char *, int);
static int stub_force_error_pull_fn(const char *, const char *, int);
static int stub_pull_nothing_yet_fn(const char *, const char *, int);

static unsigned int spy_state_on_counter;
static unsigned int spy_state_off_counter;
static int spy_receiver_state;
static void spy_turn_on_receiver_fn();
static void spy_turn_off_receiver_fn();

static char stub_message_content[MESSAGE_LENGTH];
static unsigned long nothing_until_zero;

// -----------------------------------------------------------------------------

int stub_message_fn(const char *address, const char *content,
                    const unsigned long size) {
  memcpy((void *)content, "0123456789AB", MESSAGE_LENGTH);
  return 1;
}
int mock_address_fn(const char *address, const char *content, const int size) {
  MEMCMP_EQUAL(address, "address", 7);
  return 7;
}

unsigned long stub_push_fn(const char *address, const char *content,
                           unsigned long size) {
  memcpy(stub_message_content, content, size);
  return size;
}

int stub_pull_fn(const char *address, const char *content, const int size) {
  memcpy((void *)content, stub_message_content, size);
  return size;
}

int stub_force_error_pull_fn(const char *address, const char *content,
                             const int size) {
  return -1;
}

int stub_pull_nothing_yet_fn(const char *address, const char *content,
                             const int size) {
  if (--nothing_until_zero)
    return 0;
  else
    return MESSAGE_LENGTH;
}

void spy_turn_on_receiver_fn() {
  spy_state_on_counter++;
  spy_receiver_state = 1;
}
void spy_turn_off_receiver_fn() {
  spy_state_off_counter++;
  spy_receiver_state = 0;
}

// -----------------------------------------------------------------------------

TEST_GROUP(Subscription){void setup() override{timerHelperSetup();
nothing_until_zero = 10000;
spy_receiver_state = -1;
spy_state_on_counter = 0;
spy_state_off_counter = 0;
}
}
;

TEST(Subscription, Timeout) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *)stub_message_fn, (void *)stub_progressive_epoch_ms_fn,
      (void *)spy_turn_on_receiver_fn, (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_CountDown(999);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Timeout, result);
  CHECK_EQUAL(1, spy_state_on_counter);
  CHECK_EQUAL(0, spy_receiver_state);
};

TEST(Subscription, NoTimeout) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *)stub_pull_nothing_yet_fn, (void *)stub_progressive_epoch_ms_fn,
      (void *)spy_turn_on_receiver_fn, (void *)spy_turn_off_receiver_fn);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(0, nothing_until_zero);
  CHECK_EQUAL(1, spy_state_on_counter);
  CHECK_EQUAL(0, spy_receiver_state);
};

TEST(Subscription, ReceiverIsOffByDefault) {
  MessageSubscriber_Create(
      (void *)stub_pull_nothing_yet_fn, (void *)stub_progressive_epoch_ms_fn,
      (void *)spy_turn_on_receiver_fn, (void *)spy_turn_off_receiver_fn);
  CHECK_EQUAL(0, spy_receiver_state);
};

TEST(Subscription, IOError) {
  Result result;
  Message message;
  MessageSubscriber_Create(
      (void *)stub_force_error_pull_fn, (void *)fake_epoch_ms_fn,
      (void *)spy_turn_on_receiver_fn, (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_CountDown(999);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(IOError, result);
};

TEST(Subscription, SucceededPull) {
  Result result;
  Message message;
  MessageSubscriber_Create((void *)stub_message_fn, (void *)fake_epoch_ms_fn,
                           (void *)spy_turn_on_receiver_fn,
                           (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_CountDown(999);
  result = MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Success, result);
  MEMCMP_EQUAL(message.meta, "012", MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(message.body, "3456789AB", MESSAGE_BODY_LENGTH);
};

TEST(Subscription, PullFromCorrectTopic) {
  Message message;
  MessageSubscriber_Create((void *)mock_address_fn, (void *)fake_epoch_ms_fn,
                           (void *)spy_turn_on_receiver_fn,
                           (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_CountDown(999);
  MessageSubscriber_Pull("address", &message);
  MessageSubscriber_Destroy();
};

TEST(Subscription, PushAndPull) {
  Message sent_message;
  MessageFormatter_Pack("0123456789AB", &sent_message);
  MessagePublisher_Create((const void *)stub_push_fn);
  MessagePublisher_Push("address", &sent_message);
  MessagePublisher_Destroy();
  Message received_message;
  MessageSubscriber_Create((void *)stub_pull_fn, (void *)fake_epoch_ms_fn,
                           (void *)spy_turn_on_receiver_fn,
                           (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_CountDown(999);
  MessageSubscriber_Pull("address", &received_message);
  MessageSubscriber_Destroy();
  MEMCMP_EQUAL(sent_message.meta, received_message.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(sent_message.body, received_message.body, MESSAGE_BODY_LENGTH);
};