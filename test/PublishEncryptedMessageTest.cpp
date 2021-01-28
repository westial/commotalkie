#include <CppUTest/TestHarness.h>

extern "C" {
#include "Message.h"
#include <string.h>
#include "PublishEncryptedMessage.h"
}

// -----------------------------------------------------------------------------

struct Spy {
  int calledCount;
};

static struct Spy push_fn_spy;
static char spy_expected[MESSAGE_LENGTH];
static char spy_pushed_content[MESSAGE_LENGTH];

static unsigned long mock_push_fn(const char* address, const char* content, unsigned long size) {
  push_fn_spy.calledCount ++;
  memcpy(spy_pushed_content, content, MESSAGE_LENGTH);
  return size;
}

TEST_GROUP(PublishEncryptedMessage) {
  void setup() {
    push_fn_spy.calledCount = 0;
    memcpy(spy_expected, "000000000000000000000", MESSAGE_LENGTH);
    memcpy(spy_pushed_content, "000000000000000000000", MESSAGE_LENGTH);
  }
};

TEST(PublishEncryptedMessage, Publish) {
  spy_expected[1] = 0x05;
  spy_expected[2] = 0x06;
  PublishEncryptedMessage_Create("topic", "salt", (const void *) mock_push_fn);
  unsigned char body[MESSAGE_BODY_LENGTH];
  memcpy(body, "23456789AB", MESSAGE_BODY_LENGTH);
  PublishEncryptedMessage_Invoke(0x05, 0x06, body);
  PublishEncryptedMessage_Destroy();
  CHECK_EQUAL(1, push_fn_spy.calledCount);
  CHECK_EQUAL(spy_expected[1], spy_pushed_content[1]);
  CHECK_EQUAL(spy_expected[2], spy_pushed_content[2]);
  CHECK_FALSE(spy_expected[3] == spy_pushed_content[3]);
}