#include <CppUTest/TestHarness.h>
#include "Spy.h"

extern "C" {
#include <cstring>
#include "Message.h"
#include "MessageFormatter.h"
#include "MessageValidator.h"
#include "Pull.h"
}

// -----------------------------------------------------------------------------

static int stub_message_fn(const char* address, const char*, int);
static int stub_not_valid_fn(const char* address, const char*, int);
static int stub_message_after_not_valid_failure_fn(const char*, const char*, int);
static int stub_io_error_fn(const char*, const char*, int);
static unsigned long fake_epoch_ms_fn();
static int stub_pull_nothing_yet_fn(const char*, const char*, int);

static unsigned char body[MESSAGE_BODY_LENGTH];
static unsigned char port, id;
static struct Spy pull_fn_spy;
static unsigned long nothing_until_zero;
static unsigned long progressive_ms;

// -----------------------------------------------------------------------------

int stub_message_fn(const char* address, const char* content, const int size) {
  pull_fn_spy.calledCount ++;
  Message message;
  MessageFormatter_Pack("0123456789AB", &message);
  MessageValidator_Sign(&message);
  memcpy((void *)content, (void *)&message, MESSAGE_LENGTH);
  return size;
}

int stub_not_valid_fn(const char* address, const char* content, const int size) {
  pull_fn_spy.calledCount ++;
  memcpy((void *)content, "XXXXXXXXXXXX", MESSAGE_LENGTH);
  return size;
}

int stub_message_after_not_valid_failure_fn(const char* address, const char* content, const int size) {
  if (0 == pull_fn_spy.calledCount) {
    stub_not_valid_fn(address, content, size);
    return size;
  }
  return stub_message_fn(address, content, size);
}

int stub_io_error_fn(const char* address, const char* content, const int size) {
  pull_fn_spy.calledCount++;
  return -1;
}

unsigned long fake_epoch_ms_fn() {
  return 100;
}

unsigned long stub_progressive_epoch_ms_fn() {
  // It returns 1 for the Timer_Start and 1001 for the Timer_GetMillis
  return progressive_ms += 1000;
}

int stub_pull_nothing_yet_fn(const char* address, const char* content, const int size) {
  if (--nothing_until_zero) return 0;
  else return stub_message_fn(address, content, size);
}

// -----------------------------------------------------------------------------

TEST_GROUP(PullApp) {
  void setup() override {
    nothing_until_zero = 10000;
    pull_fn_spy.calledCount = 0;
    memset(body, '\0', MESSAGE_BODY_LENGTH);
    port = '\0';
    id = '\0';
    progressive_ms = 1;
  }
};

TEST(PullApp, PullingSuccess) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_message_fn,
      (void *) fake_epoch_ms_fn,
      999, nullptr);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
}

TEST(PullApp, PullingSuccessAfterFailure) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_message_after_not_valid_failure_fn,
      (void *) fake_epoch_ms_fn,
      999, nullptr);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 2);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
}

TEST(PullApp, NoTimeoutPulling) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_pull_nothing_yet_fn,
      (void *) fake_epoch_ms_fn,
      0, nullptr);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
  CHECK_EQUAL(0, nothing_until_zero);
}

TEST(PullApp, NotValidEndsByTimeout) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_not_valid_fn,
      (void *) stub_progressive_epoch_ms_fn,
      10000, nullptr);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_FALSE(1 == pull_fn_spy.calledCount);
  CHECK_EQUAL(Timeout, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}

TEST(PullApp, UnexpectedIdEndsWithTimeout) {
  Result result;
  const unsigned char strict_id = 0xA1;
  Pull_Create(
      "",
      "address",
      (void *) stub_message_fn,
      (void *) stub_progressive_epoch_ms_fn,
      10000, &strict_id);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_FALSE(1 == pull_fn_spy.calledCount);
  CHECK_EQUAL(Timeout, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}

TEST(PullApp, PullFromId) {
  Result result;
  const unsigned char strict_id = '2';
  Pull_Create(
      "",
      "address",
      (void *) stub_message_fn,
      (void *) stub_progressive_epoch_ms_fn,
      10000, &strict_id);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
}

TEST(PullApp, IOErrorFailure) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_io_error_fn,
      (void *) fake_epoch_ms_fn,
      999, nullptr);
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(IOError, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}