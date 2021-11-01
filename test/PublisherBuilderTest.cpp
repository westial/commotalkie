#include <CppUTest/TestHarness.h>
#include "Spy.h"

#include "Message.h"
#include "Publish.h"
#include "PublisherBuilder.h"

#include "helper/PublishTestHelper.cpp"

// -----------------------------------------------------------------------------

TEST_GROUP(PublisherBuilder) {
  void setup() override {
    push_fn_spy.calledCount = 0;
  }
};

TEST(PublisherBuilder, BuildAPublisher) {
  const char salt[] = "salt";
  unsigned char body[MESSAGE_BODY_LENGTH];
  PublisherBuilder_Create();
  PublisherBuilder_SetSalt(salt);
  PublisherBuilder_SetSendCallback(fake_push_fn);
  CHECK_TRUE(PublisherBuilder_Build());
  PublisherBuilder_Destroy();
  Publish_Invoke((const unsigned char *)"topic", 0xA5, 0xB6, body);
  Publish_Destroy();
  CHECK_EQUAL(1, push_fn_spy.calledCount);
}

TEST(PublisherBuilder, SaltIsRequired) {
  PublisherBuilder_Create();
  PublisherBuilder_SetSendCallback(fake_push_fn);
  CHECK_FALSE(PublisherBuilder_Build());
  PublisherBuilder_Destroy();
}

TEST(PublisherBuilder, EmptySaltIsNotAllowed) {
  const char salt[] = "";
  PublisherBuilder_Create();
  PublisherBuilder_SetSalt(salt);
  PublisherBuilder_SetSendCallback(fake_push_fn);
  CHECK_FALSE(PublisherBuilder_Build());
  PublisherBuilder_Destroy();
}

TEST(PublisherBuilder, SendingCallbackMissed) {
  const char salt[] = "salt";
  PublisherBuilder_Create();
  PublisherBuilder_SetSalt(salt);
  CHECK_FALSE(PublisherBuilder_Build());
  PublisherBuilder_Destroy();
}
