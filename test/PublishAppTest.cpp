#include <CppUTest/TestHarness.h>
#include "Spy.h"

#include <cstring>
#include "Message.h"
#include "Publish.h"
#include "Result.h"
#include "Pull.h"


// -----------------------------------------------------------------------------

#define MAX_MESSAGES 3
#define TOPIC_LENGTH 16

static unsigned long mock_push_fn(const char*, const char*, unsigned long);
static unsigned long mock_push_fn_keep_topic(const char*, const char*, unsigned long);
static int mock_pull_fn(const char*, const char*, int);
static unsigned long mock_address_fn(const char*, const char*, unsigned long);
static unsigned long fake_epoch_ms_fn();

static struct Spy push_fn_spy;
static char spy_expected[MESSAGE_LENGTH];
static char spy_pushed_contents[MAX_MESSAGES][MESSAGE_LENGTH];
static int spy_pushed_topics_index;
static char spy_pushed_topics[MAX_MESSAGES][TOPIC_LENGTH];
static int spy_pushed_contents_index;

static int spy_receiver_state;
static void spy_turn_on_receiver_fn();
static void spy_turn_off_receiver_fn();

// -----------------------------------------------------------------------------

unsigned long mock_push_fn(const char* address, const char* content, unsigned long size) {
  push_fn_spy.calledCount ++;
  memcpy(spy_pushed_contents[spy_pushed_contents_index++], content, MESSAGE_LENGTH);
  return size;
}

unsigned long mock_push_fn_keep_topic(const char* address, const char* content, unsigned long size) {
  mock_push_fn(address, content, size);
  memcpy(spy_pushed_topics[spy_pushed_topics_index++], address, TOPIC_LENGTH);
  return size;
}

int mock_pull_fn(const char* address, const char* content, const int size) {
  memcpy((void *)content, spy_pushed_contents[0], MESSAGE_LENGTH);
  return size;
}

unsigned long fake_epoch_ms_fn() {
  return 100;
}

void spy_turn_on_receiver_fn() {
  spy_receiver_state = 1;
}
void spy_turn_off_receiver_fn() {
  spy_receiver_state = 0;
}

// -----------------------------------------------------------------------------

TEST_GROUP(PublishApp) {
  void setup() override {
    push_fn_spy.calledCount = 0;
    spy_pushed_contents_index = 0;
    spy_pushed_topics_index = 0;
    memset(spy_expected, '\0', MESSAGE_LENGTH);
    for (auto & spy_pushed_content : spy_pushed_contents) {
      memset(spy_pushed_content, '\0', MESSAGE_LENGTH);
    }
    for (auto & spy_pushed_topic : spy_pushed_topics) {
      memset(spy_pushed_topic, '\0', TOPIC_LENGTH);
    }
    spy_receiver_state = -1;
  }
};

TEST(PublishApp, Publish) {
  char body[MESSAGE_BODY_LENGTH];
  spy_expected[1] = 0x05;
  spy_expected[2] = 0x06;
  Publish_Create("salt", (const void *) mock_push_fn);
  memcpy(body, "23456789A", MESSAGE_BODY_LENGTH);
  Publish_Invoke("topic", 0x05, 0x06, body);
  Publish_Destroy();
  CHECK_EQUAL(1, push_fn_spy.calledCount);
  CHECK_EQUAL(spy_expected[1], spy_pushed_contents[0][1]);
  CHECK_EQUAL(spy_expected[2], spy_pushed_contents[0][2]);
  CHECK_TRUE(spy_expected[3] != spy_pushed_contents[0][3]);
}

TEST(PublishApp, PublishMultipleMessages) {
  char body[MESSAGE_BODY_LENGTH];
  Publish_Create("", (const void *) mock_push_fn);
  memcpy(body, "23456789A", MESSAGE_BODY_LENGTH);
  Publish_Invoke("topic", 0x05, 0x06, body);
  Publish_Invoke("topic", 0x08, 0x09, body);
  memcpy(body, "xxxdifxxx", MESSAGE_BODY_LENGTH);
  Publish_Invoke("topic", 0x05, 0x06, body);
  Publish_Destroy();
  CHECK_EQUAL(3, push_fn_spy.calledCount);
  CHECK_EQUAL(0x05, spy_pushed_contents[0][1]);
  CHECK_EQUAL(0x06, spy_pushed_contents[0][2]);
  CHECK_TRUE(spy_expected[3] != spy_pushed_contents[0][3]);
  CHECK_EQUAL(0x08, spy_pushed_contents[1][1]);
  CHECK_EQUAL(0x09, spy_pushed_contents[1][2]);
  CHECK_TRUE(spy_expected[3] != spy_pushed_contents[1][3]);
  CHECK_TRUE(spy_pushed_contents[0][MESSAGE_BODY_LENGTH-1] == spy_pushed_contents[1][MESSAGE_BODY_LENGTH-1]);
  CHECK_FALSE(spy_pushed_contents[0][MESSAGE_BODY_LENGTH-1] == spy_pushed_contents[2][MESSAGE_BODY_LENGTH-1]);
}

TEST(PublishApp, PublishToTwoTopics) {
  char body[MESSAGE_BODY_LENGTH];
  Publish_Create("salt", (const void *) mock_push_fn_keep_topic);
  memcpy(body, "same body", MESSAGE_BODY_LENGTH);
  Publish_Invoke("topic1", 0x05, 0x06, body);
  Publish_Invoke("topic2", 0x05, 0x06, body);
  Publish_Destroy();
  MEMCMP_EQUAL(
      spy_pushed_contents[0] + MESSAGE_META_LENGTH,
      spy_pushed_contents[1] + MESSAGE_META_LENGTH,
      MESSAGE_BODY_LENGTH
      );
  MEMCMP_EQUAL(spy_pushed_topics[0], "topic1", 6);
  MEMCMP_EQUAL(spy_pushed_topics[1], "topic2", 6);
}

TEST(PublishApp, PublishAndRead) {
  char expected_body[MESSAGE_BODY_LENGTH];
  Result result;
  unsigned char result_port;
  unsigned char result_id;
  char result_body[MESSAGE_BODY_LENGTH];
  unsigned char expected_port = 0x07;
  unsigned char expected_id = 0x08;
  Publish_Create("salt", (const void *) mock_push_fn);
  memcpy(expected_body, "23456789A", MESSAGE_BODY_LENGTH);
  Publish_Invoke("topic", expected_port, expected_id, expected_body);
  Publish_Destroy();
  Pull_Create("salt", (void *)mock_pull_fn, (void *)fake_epoch_ms_fn,
              (void *)spy_turn_on_receiver_fn, (void *)spy_turn_off_receiver_fn,
              999, 0);
  result = Pull_Invoke("topic", &result_port, &result_id, result_body);
  Pull_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(expected_port, result_port);
  CHECK_EQUAL(expected_id, result_id);
  MEMCMP_EQUAL(expected_body, result_body, MESSAGE_BODY_LENGTH);
}