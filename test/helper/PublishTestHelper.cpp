#include "../Spy.h"
#include "PublishPullShared.h"

#define MAX_MESSAGES 3
#define TOPIC_LENGTH 16

static unsigned long mock_push_fn(const unsigned char*, const char*, unsigned long);
static unsigned long mock_push_fn_keep_topic(const unsigned char*, const char*, unsigned long);
static unsigned long fake_push_fn(const unsigned char *, const char *, unsigned long);
static unsigned long stub_push_fn(const unsigned char *, const char *, unsigned long);

static struct Spy push_fn_spy;
static char spy_expected[MESSAGE_LENGTH];
static char spy_pushed_contents[MAX_MESSAGES][MESSAGE_LENGTH];
static int spy_pushed_topics_index;
static char spy_pushed_topics[MAX_MESSAGES][TOPIC_LENGTH];
static int spy_pushed_contents_index;

// -----------------------------------------------------------------------------

unsigned long mock_push_fn(const unsigned char* address, const char* content, unsigned long size) {
  push_fn_spy.calledCount ++;
  memcpy(spy_pushed_contents[spy_pushed_contents_index++], content, MESSAGE_LENGTH);
  return size;
}

unsigned long mock_push_fn_keep_topic(const unsigned char* address, const char* content, unsigned long size) {
  mock_push_fn(address, content, size);
  memcpy(spy_pushed_topics[spy_pushed_topics_index++], address, TOPIC_LENGTH);
  return size;
}

unsigned long fake_push_fn(
    const unsigned char *address, const char *content, unsigned long size) {
  push_fn_spy.calledCount++;
  return 0;
}

unsigned long stub_push_fn(const unsigned char *address, const char *content,
                           unsigned long size) {
  memcpy(stub_message_content, content, size);
  return size;
}