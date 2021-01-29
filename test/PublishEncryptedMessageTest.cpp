#include <CppUTest/TestHarness.h>
#include "Spy.h"

extern "C" {
#include <string.h>
#include "Message.h"
#include "PublishEncryptedMessage.h"
#include "Result.h"
#include "ReadEncryptedMessage.h"
}

// -----------------------------------------------------------------------------

static struct Spy push_fn_spy;
static char spy_expected[MESSAGE_LENGTH];
static char spy_pushed_content[MESSAGE_LENGTH];

static unsigned long mock_push_fn(const char* address, const char* content, unsigned long size) {
  push_fn_spy.calledCount ++;
  memcpy(spy_pushed_content, content, MESSAGE_LENGTH);
  return size;
}

static int mock_pull_fn(const char* address, const char* content, const int size) {
  memcpy((void *)content, spy_pushed_content, MESSAGE_LENGTH);
  return size;
}

TEST_GROUP(PublishEncryptedMessage) {
  void setup() {
    push_fn_spy.calledCount = 0;
    memset(spy_expected, '\0', MESSAGE_LENGTH);
    memset(spy_pushed_content, '\0', MESSAGE_LENGTH);
  }
};

TEST(PublishEncryptedMessage, Publish) {
  unsigned char body[MESSAGE_BODY_LENGTH];
  spy_expected[1] = 0x05;
  spy_expected[2] = 0x06;
  PublishEncryptedMessage_Create("salt", "topic", (const void *) mock_push_fn);
  memcpy(body, "23456789A", MESSAGE_BODY_LENGTH);
  PublishEncryptedMessage_Invoke(0x05, 0x06, body);
  PublishEncryptedMessage_Destroy();
  CHECK_EQUAL(1, push_fn_spy.calledCount);
  CHECK_EQUAL(spy_expected[1], spy_pushed_content[1]);
  CHECK_EQUAL(spy_expected[2], spy_pushed_content[2]);
  CHECK_FALSE(spy_expected[3] == spy_pushed_content[3]);
}

TEST(PublishEncryptedMessage, PublishAndRead) {
  unsigned char expected_body[MESSAGE_BODY_LENGTH];
  Result result;
  unsigned char result_port;
  unsigned char result_id;
  unsigned char result_body[MESSAGE_BODY_LENGTH];
  unsigned char expected_port = 0x07;
  unsigned char expected_id = 0x08;

  PublishEncryptedMessage_Create("salt", "topic", (const void *) mock_push_fn);
  memcpy(expected_body, "23456789A", MESSAGE_BODY_LENGTH);
  PublishEncryptedMessage_Invoke(expected_port, expected_id, expected_body);
  PublishEncryptedMessage_Destroy();

  ReadEncryptedMessage_Create("salt", "topic", (void *)mock_pull_fn);
  result = ReadEncryptedMessage_Invoke(&result_port, &result_id, result_body);
  ReadEncryptedMessage_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(expected_port, result_port);
  CHECK_EQUAL(expected_id, result_id);
  MEMCMP_EQUAL(expected_body, result_body, MESSAGE_BODY_LENGTH);
}