#include "CppUTest/TestHarness.h"
#include "Timer.h"

#include "helper/TimerTestHelper.cpp"


// -----------------------------------------------------------------------------

TEST_GROUP(Timer) {
};

TEST(Timer, TimerIsNotRunning) {
  Timer timer = Timer_Create((const void*)fake_epoch_ms_fn);
  CHECK_FALSE(Timer_IsRunning(&timer));
}

TEST(Timer, StartCountingTimeFromZero) {
  unsigned long result;
  Timer timer = Timer_Create((const void*)fake_epoch_ms_fn);
  Timer_Start(&timer);
  result = Timer_GetMillis(&timer);
  CHECK_EQUAL(0, result);
}

TEST(Timer, GetCurrentTimestamp) {
  unsigned long result;
  Timer timer = Timer_Create((const void*)stub_progressive_epoch_ms_fn);
  Timer_Start(&timer);
  result = Timer_GetMillis(&timer);
  CHECK_EQUAL(progressive_ms - STUB_PROGRESSIVE_INTERVAL, result);
}

TEST(Timer, TimerDoesNotStartTwice) {
  unsigned long result;
  Timer timer = Timer_Create((const void*)fake_epoch_ms_fn);
  Timer_Start(&timer);
  Timer_Start(&timer);
  CHECK_FALSE(Timer_IsRunning(&timer));
}