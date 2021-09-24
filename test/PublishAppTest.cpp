#include <CppUTest/TestHarness.h>
#include "Spy.h"

extern "C" {
#include <cstring>
#include "Message.h"
#include "Publish.h"
#include "Result.h"
#include "Pull.h"
}

// -----------------------------------------------------------------------------

static unsigned long mock_push_fn(const char*, const char*, unsigned long);
static int mock_pull_fn(const char*, const char*, int);
static unsigned long fake_epoch_ms_fn();

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

static unsigned long fake_epoch_ms_fn() {
  return 100;
}

TEST_GROUP(PublishApp) {
  void setup() override {
    push_fn_spy.calledCount = 0;
    memset(spy_expected, '\0', MESSAGE_LENGTH);
    memset(spy_pushed_content, '\0', MESSAGE_LENGTH);
  }
};

TEST(PublishApp, Publish) {
  unsigned char body[MESSAGE_BODY_LENGTH];
  spy_expected[1] = 0x05;
  spy_expected[2] = 0x06;
  Publish_Create("salt", "topic", (const void *) mock_push_fn);
  memcpy(body, "23456789A", MESSAGE_BODY_LENGTH);
  Publish_Invoke(0x05, 0x06, body);
  Publish_Destroy();
  CHECK_EQUAL(1, push_fn_spy.calledCount);
  CHECK_EQUAL(spy_expected[1], spy_pushed_content[1]);
  CHECK_EQUAL(spy_expected[2], spy_pushed_content[2]);
  CHECK_FALSE(spy_expected[3] == spy_pushed_content[3]);
}

TEST(PublishApp, PublishAndRead) {
  unsigned char expected_body[MESSAGE_BODY_LENGTH];
  Result result;
  unsigned char result_port;
  unsigned char result_id;
  unsigned char result_body[MESSAGE_BODY_LENGTH];
  unsigned char expected_port = 0x07;
  unsigned char expected_id = 0x08;

  Publish_Create("salt", "topic", (const void *) mock_push_fn);
  memcpy(expected_body, "23456789A", MESSAGE_BODY_LENGTH);
  Publish_Invoke(expected_port, expected_id, expected_body);
  Publish_Destroy();

  Pull_Create(
      "salt",
      "topic",
      (void *) mock_pull_fn,
      (void *) fake_epoch_ms_fn,
      999);
  result = Pull_Invoke(&result_port, &result_id, result_body);
  Pull_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(expected_port, result_port);
  CHECK_EQUAL(expected_id, result_id);
  MEMCMP_EQUAL(expected_body, result_body, MESSAGE_BODY_LENGTH);
}