#include <CppUTest/TestHarness.h>
#include "Spy.h"

#include <cstring>
#include "Pull.h"
#include "Message.h"
#include "SubscriberBuilder.h"

#include "helper/TimerTestHelper.cpp"
#include "helper/PullTestHelper.cpp"

TEST_GROUP(SubscriberBuilder) {
  void setup() override {
    pull_fn_spy.calledCount = 0;
    port = 0;
    id = 0;
    memset(body, 0, MESSAGE_BODY_LENGTH);
    nothing_until_zero = 10000;
  }
};

TEST(SubscriberBuilder, BuiltSubscriberPullsForIdOnly) {
  const unsigned long timeout = 999;
  id = '2';
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  SubscriberBuilder_SetTimeService(fake_epoch_ms_fn);
  SubscriberBuilder_SetReceiverStateCallback(fake_turn_on_fn, fake_turn_off_fn);
  SubscriberBuilder_SetTimeout(&timeout);
  SubscriberBuilder_SetId(&id);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
  Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(1, pull_fn_spy.calledCount);
}

TEST(SubscriberBuilder, BuiltSubscriberPullsForAnyId) {
  const unsigned long timeout = 999;
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  SubscriberBuilder_SetTimeService(fake_epoch_ms_fn);
  SubscriberBuilder_SetReceiverStateCallback(fake_turn_on_fn, fake_turn_off_fn);
  SubscriberBuilder_SetTimeout(&timeout);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
  Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(1, pull_fn_spy.calledCount);
}

TEST(SubscriberBuilder, BuildASubscriberNoTimeout) {
  Result result;
  id = '2';
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_pull_nothing_yet_fn);
  SubscriberBuilder_SetReceiverStateCallback(fake_turn_on_fn, fake_turn_off_fn);
  SubscriberBuilder_SetId(&id);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(10000, pull_fn_spy.calledCount);
  CHECK_EQUAL(Success, result);
}

TEST(SubscriberBuilder, BuildWithMinimumRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  SubscriberBuilder_SetReceiverStateCallback(fake_turn_on_fn, fake_turn_off_fn);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
}

TEST(SubscriberBuilder, SaltIsRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  SubscriberBuilder_SetReceiverStateCallback(fake_turn_on_fn, fake_turn_off_fn);
  CHECK_FALSE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
}

TEST(SubscriberBuilder, ListenCallbackIsRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetReceiverStateCallback(fake_turn_on_fn, fake_turn_off_fn);
  CHECK_FALSE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
}

TEST(SubscriberBuilder, ReceiverTurnOnCallbackIsRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  CHECK_FALSE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
}