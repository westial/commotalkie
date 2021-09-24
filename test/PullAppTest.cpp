#include <CppUTest/TestHarness.h>
#include "Spy.h"

extern "C" {
#include <string.h>
#include "Message.h"
#include "MessageFormatter.h"
#include "MessageValidator.h"
#include "Pull.h"
}

// -----------------------------------------------------------------------------

unsigned char body[MESSAGE_BODY_LENGTH];
unsigned char port, id;
static struct Spy pull_fn_spy;

static int stub_message_fn(const char* address, const char* content, const int size) {
  pull_fn_spy.calledCount ++;
  Message message;
  MessageFormatter_Pack("0123456789AB", &message);
  MessageValidator_Sign(&message);
  memcpy((void *)content, (void *)&message, MESSAGE_LENGTH);
  return size;
}

static int stub_not_valid_fn(const char* address, const char* content, const int size) {
  pull_fn_spy.calledCount ++;
  memcpy((void *)content, "0123456789AB", MESSAGE_LENGTH);
  return size;
}

static int stub_io_error_fn(const char* address, const char* content, const int size) {
  pull_fn_spy.calledCount ++;
  return -1;
}

static unsigned long fake_epoch_ms_fn() {
  return 100;
}

static unsigned long nothing_until_zero;

static int stub_pull_nothing_yet_fn(const char* address, const char* content, const int size) {
  if (--nothing_until_zero) return 0;
  else return stub_message_fn(address, content, size);
}

TEST_GROUP(PullApp) {
  void setup() {
    nothing_until_zero = 10000;
    pull_fn_spy.calledCount = 0;
    memset(body, '\0', MESSAGE_BODY_LENGTH);
    port = '\0';
    id = '\0';
  }
};

TEST(PullApp, PullingSuccess) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_message_fn,
      (void *) fake_epoch_ms_fn,
      999
  );
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
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
      0
  );
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
  CHECK_EQUAL(0, nothing_until_zero);
}

TEST(PullApp, NotValidFailure) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_not_valid_fn,
      (void *) fake_epoch_ms_fn,
      999
  );
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(NotValid, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}

TEST(PullApp, IOErrorFailure) {
  Result result;
  Pull_Create(
      "",
      "address",
      (void *) stub_io_error_fn,
      (void *) fake_epoch_ms_fn,
      999
  );
  result = Pull_Invoke(&port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(IOError, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}