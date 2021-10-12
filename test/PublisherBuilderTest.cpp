#include <CppUTest/TestHarness.h>
#include "Spy.h"

#include <cstring>
#include "Message.h"
#include "Publish.h"
#include "PublisherBuilder.h"


// -----------------------------------------------------------------------------

static struct Spy push_fn_spy;
static unsigned long fake_push_fn(const char *, const char *, unsigned long);

// -----------------------------------------------------------------------------

unsigned long fake_push_fn(
    const char *address, const char *content, unsigned long size) {
  push_fn_spy.calledCount++;
  return 0;
}

// -----------------------------------------------------------------------------

TEST_GROUP(PublisherBuilder) {
  void setup() override {
    push_fn_spy.calledCount = 0;
  }
};

TEST(PublisherBuilder, BuildAPublisher) {
  const char salt[] = "salt";
  char body[MESSAGE_BODY_LENGTH];
  PublisherBuilder_Create();
  PublisherBuilder_SetSalt(salt);
  PublisherBuilder_SetSendCallback(fake_push_fn);
  CHECK_TRUE(PublisherBuilder_Build());
  PublisherBuilder_Destroy();
  Publish_Invoke("topic", 0x05, 0x06, body);
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
