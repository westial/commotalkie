#include "CppUTest/TestHarness.h"

#include "MessageFormatter.h"
#include "MessagePublisher.h"
#include "MessageSubscriber.h"

#include "helper/TimerTestHelper.cpp"
#include "helper/PublishTestHelper.cpp"
#include "helper/PullTestHelper.cpp"

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
  MessageSubscriber_StartCountDown(999);
  result = MessageSubscriber_Pull((const unsigned char *)"address", &message);
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
  result = MessageSubscriber_Pull((const unsigned char *)"address", &message);
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
  MessageSubscriber_StartCountDown(999);
  result = MessageSubscriber_Pull((const unsigned char *)"address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(IOError, result);
};

TEST(Subscription, SucceededPull) {
  Result result;
  Message message;
  MessageSubscriber_Create((void *)stub_message_fn, (void *)fake_epoch_ms_fn,
                           (void *)spy_turn_on_receiver_fn,
                           (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_StartCountDown(999);
  result = MessageSubscriber_Pull((const unsigned char *)"address", &message);
  MessageSubscriber_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(0xED, message.meta[0]);   // checksum byte
  MEMCMP_EQUAL("12", message.meta + 1, MESSAGE_META_LENGTH - 1);
  MEMCMP_EQUAL("3456789AB", message.body, MESSAGE_BODY_LENGTH);
};

TEST(Subscription, PullFromCorrectTopic) {
  Message message;
  MessageSubscriber_Create(
      (void *)spy_address_on_pull_fn, (void *)fake_epoch_ms_fn,
      (void *)spy_turn_on_receiver_fn, (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_StartCountDown(999);
  MessageSubscriber_Pull((const unsigned char *)"address", &message);
  MessageSubscriber_Destroy();
};

TEST(Subscription, PushAndPull) {
  Message sent_message;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &sent_message);
  MessagePublisher_Create((const void *)stub_push_fn);
  MessagePublisher_Push((const unsigned char *)"address", &sent_message);
  MessagePublisher_Destroy();
  Message received_message;
  MessageSubscriber_Create((void *)stub_pull_fn, (void *)fake_epoch_ms_fn,
                           (void *)spy_turn_on_receiver_fn,
                           (void *)spy_turn_off_receiver_fn);
  MessageSubscriber_StartCountDown(999);
  MessageSubscriber_Pull((const unsigned char *)"address", &received_message);
  MessageSubscriber_Destroy();
  MEMCMP_EQUAL(sent_message.meta, received_message.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(sent_message.body, received_message.body, MESSAGE_BODY_LENGTH);
};