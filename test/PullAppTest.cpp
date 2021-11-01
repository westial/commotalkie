#include "Spy.h"
#include <CppUTest/TestHarness.h>

#include "Message.h"
#include "Pull.h"
#include <cstring>

#include "helper/TimerTestHelper.cpp"
#include "helper/PullTestHelper.cpp"

// -----------------------------------------------------------------------------

TEST_GROUP(PullApp){void setup() override{nothing_until_zero = 10000;
pull_fn_spy.calledCount = 0;
memset(body, 0, MESSAGE_BODY_LENGTH);
port = 0;
id = 0;
timerHelperSetup();
spy_receiver_state = -1;
}
}
;

TEST(PullApp, PullingSuccess) {
  Result result;
  Pull_Create("", (const void *)stub_message_fn, (const void *)fake_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 999, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
}

TEST(PullApp, PullingSuccessAfterFailure) {
  Result result;
  Pull_Create("", (const void *)stub_message_after_not_valid_failure_fn,
              (const void *)fake_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 999, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 2);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
}

TEST(PullApp, NoTimeoutPulling) {
  Result result;
  Pull_Create("", (const void *)stub_pull_nothing_yet_fn,
              (const void *)fake_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 0, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(10000, pull_fn_spy.calledCount);
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
  CHECK_EQUAL(0, nothing_until_zero);
}

TEST(PullApp, NotValidEndsByTimeout) {
  Result result;
  Pull_Create("", (const void *)stub_not_valid_fn,
              (const void *)stub_progressive_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 10000, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_FALSE(1 == pull_fn_spy.calledCount);
  CHECK_EQUAL(Timeout, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}

TEST(PullApp, EmptyEndsByTimeout) {
  Result result;
  Pull_Create("", (const void *)stub_empty_message_fn,
              (const void *)stub_progressive_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 10000, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_FALSE(1 == pull_fn_spy.calledCount);
  CHECK_EQUAL(Timeout, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}

TEST(PullApp, MultipleTimeouts) {
  Result result;
  int timeout_at = 10000;
  Pull_Create("", (const void *)stub_empty_message_fn,
              (const void *)stub_progressive_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, timeout_at, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  CHECK_EQUAL(Timeout, result);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  CHECK_EQUAL(Timeout, result);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  CHECK_EQUAL(Timeout, result);
  Pull_Destroy();
  CHECK_FALSE(3 == pull_fn_spy.calledCount);
  /**
   * `3 * ` Three times pulling.
   * `(STUB_PROGRESSIVE_INTERVAL * 2)` The progressive incremental method is
   *    visited on starting and for the last checking.
   * `+ 1` Progressive ms starts at 1.
   */
  CHECK_EQUAL(3 * (timeout_at + (STUB_PROGRESSIVE_INTERVAL * 2)) + 1, progressive_ms);
}

TEST(PullApp, UnexpectedIdEndsWithTimeout) {
  Result result;
  const unsigned char strict_id = 0xA1;
  Pull_Create("", (const void *)stub_message_fn,
              (const void *)stub_progressive_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 10000, &strict_id);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
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
  Pull_Create("", (const void *)stub_message_fn,
              (const void *)stub_progressive_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 10000, &strict_id);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(Success, result);
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL('1', port);
  CHECK_EQUAL('2', id);
  MEMCMP_EQUAL("3456789AB", body, MESSAGE_BODY_LENGTH);
}

TEST(PullApp, IOErrorFailure) {
  Result result;
  Pull_Create("", (const void *)stub_io_error_fn,
              (const void *)fake_epoch_ms_fn,
              (const void *)spy_turn_on_receiver_fn,
              (const void *)spy_turn_off_receiver_fn, 999, nullptr);
  result = Pull_Invoke((const unsigned char *)"address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(pull_fn_spy.calledCount, 1);
  CHECK_EQUAL(IOError, result);
  CHECK_EQUAL('\0', port);
  CHECK_EQUAL('\0', id);
  CHECK_EQUAL('\0', body[0]);
}