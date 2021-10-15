#include "CppUTest/TestHarness.h"
#include "Driver.h"
#include "EByte.h"
#include "helper/DriverTestHelper.cpp"

// -----------------------------------------------------------------------------

TEST_GROUP(DriverSending){void setup() override{default_timeout = 20000;
progressive_ms = 1;
stub_read_pin_return = 1; // ready by default
stub_read_pin_call_count = 0;
stub_read_pin_toggle_at = 1000;
reset_write_pin();
reset_write_to_serial();
}
}
;

TEST(DriverSending, SendAString) {
  Driver sample_driver = create_sample("\xA1\xA2\xA3", AIR_RATE_2400, 1, 1);
  reset_write_to_serial();
  const char raw_message[] = "abcdefghi";
  const Destination target = {'\xA1', '\xA2', '\xA3'};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  MEMCMP_EQUAL("\xA1\xA2\xA3", spy_write_to_serial_arg_1[0], 3);
  MEMCMP_EQUAL("abcdefghi", spy_write_to_serial_arg_1[0] + 3,
               sizeof(raw_message));
}

TEST(DriverSending, SetStateToNormalBeforeSending) {
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  const char raw_message[] = "abcdefghi";
  const Destination target = {'\xA1', '\xA2', '\xA3'};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  CHECK_EQUAL(spy_write_pin_args[2][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[2][1], OFF);
  CHECK_EQUAL(spy_write_pin_args[3][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[3][1], OFF);
}

TEST(DriverSending, SetStateToSleepAfterSending) {
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  const char raw_message[] = "abcdefghi";
  const Destination target = {'\xA1', '\xA2', '\xA3'};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  CHECK_EQUAL(spy_write_pin_args[4][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[4][1], ON);
  CHECK_EQUAL(spy_write_pin_args[5][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[5][1], ON);
}