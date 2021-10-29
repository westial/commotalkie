#include "PublishPullShared.h"
#include <MessageFormatter.h>
#include <MessageValidator.h>

static int stub_message_fn(const unsigned char *address, const char *content,
                           unsigned long size);
static int stub_not_valid_fn(const unsigned char *address, const char *content,
                             unsigned long size);
static int stub_message_after_not_valid_failure_fn(const unsigned char *address,
                                                   const char *content,
                                                   unsigned long size);
static int stub_io_error_fn(const unsigned char *address, const char *content, int size);
static int stub_pull_nothing_yet_fn(const unsigned char *address, const char *content,
                                    int size);
static int stub_empty_message_fn(const unsigned char *address, const char *content,
                          unsigned long size);

static int mock_address_fn(const unsigned char *, const char *, int);
static int stub_pull_fn(const char *, const char *, int);
static int stub_force_error_pull_fn(const char *, const char *, int);

static char body[MESSAGE_BODY_LENGTH];
static unsigned char port, id;
static struct Spy pull_fn_spy;
static unsigned long nothing_until_zero;

static int spy_receiver_state;
static void spy_turn_on_receiver_fn();
static void spy_turn_off_receiver_fn();

static unsigned int spy_state_on_counter;
static unsigned int spy_state_off_counter;

// -----------------------------------------------------------------------------

int stub_message_fn(const unsigned char *address, const char *content,
                    const unsigned long size) {
  pull_fn_spy.calledCount++;
  Message message;
  MessageFormatter_Pack("0123456789AB", &message);
  MessageValidator_Sign(&message);
  memcpy((void *)content, (void *)&message, MESSAGE_LENGTH);
  return 0 < size;
}

int stub_not_valid_fn(const unsigned char *address, const char *content,
                      const unsigned long size) {
  pull_fn_spy.calledCount++;
  memcpy((void *)content, "XXXXXXXXXXXX", MESSAGE_LENGTH);
  return size;
}

int stub_empty_message_fn(const unsigned char *address, const char *content,
                          const unsigned long size) {
  return 0;
}

int stub_message_after_not_valid_failure_fn(const unsigned char *address,
                                            const char *content,
                                            unsigned long size) {
  if (0 == pull_fn_spy.calledCount) {
    stub_not_valid_fn((const unsigned char *)address, content, size);
    return size;
  }
  return stub_message_fn((const unsigned char *)address, content, size);
}

int stub_io_error_fn(const unsigned char *address, const char *content, int size) {
  pull_fn_spy.calledCount++;
  return -1;
}
int mock_address_fn(const unsigned char *address, const char *content, const int size) {
  return 7;
}

int stub_pull_fn(const char *address, const char *content, const int size) {
  memcpy((void *)content, stub_message_content, size);
  return size;
}

int stub_force_error_pull_fn(const char *address, const char *content,
                             const int size) {
  return -1;
}

int stub_pull_nothing_yet_fn(const unsigned char *address, const char *content,
                             const int size) {
  if (--nothing_until_zero) {
    pull_fn_spy.calledCount++;
    return 0;
  } else
    return stub_message_fn((const unsigned char *)address, content, size);
}

void spy_turn_on_receiver_fn() {
  spy_state_on_counter++;
  spy_receiver_state = 1;
}
void spy_turn_off_receiver_fn() {
  spy_state_off_counter++;
  spy_receiver_state = 0;
}