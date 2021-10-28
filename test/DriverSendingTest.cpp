#include "CppUTest/TestHarness.h"
#include "Driver.h"
#include "EByte.h"
#include "helper/DriverTestHelper.cpp"

// -----------------------------------------------------------------------------

TEST_GROUP(DriverSending){void setup() override{driverHelperSetup();
}
}
;

TEST(DriverSending, SendAString) {
  const unsigned char topic[] = {0xA1, 0xA2, 0xA3};
  Driver sample_driver = create_sample(topic, AIR_RATE_2400, 1, 1);
  reset_write_to_serial();
  const char raw_message[] = "abcdefghi";
  const Destination target = {0xA1, 0xA2, 0xA3};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  MEMCMP_EQUAL("\xA1\xA2\xA3", spy_write_to_serial_arg_1[0], 3);
  MEMCMP_EQUAL("abcdefghi", spy_write_to_serial_arg_1[0] + 3,
               sizeof(raw_message));
}

TEST(DriverSending, SetStateToNormalBeforeSending) {
  const unsigned char topic[] = {0xA1, 0xA2, 0xA3};
  Driver sample_driver = create_sample(topic, 0, 0, 0);
  const char raw_message[] = "abcdefghi";
  const Destination target = {0xA1, 0xA2, 0xA3};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  CHECK_EQUAL(spy_write_pin_args[2][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[2][1], OFF);
  CHECK_EQUAL(spy_write_pin_args[3][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[3][1], OFF);
}

TEST(DriverSending, SetStateToSleepAfterSending) {
  const unsigned char topic[] = {0xA1, 0xA2, 0xA3};
  Driver sample_driver = create_sample(topic, 0, 0, 0);
  const char raw_message[] = "abcdefghi";
  const Destination target = {0xA1, 0xA2, 0xA3};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  CHECK_EQUAL(spy_write_pin_args[4][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[4][1], ON);
  CHECK_EQUAL(spy_write_pin_args[5][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[5][1], ON);
}

TEST(DriverSending, WaitUntilAuxIsHighAfterSending) {
  const unsigned char topic[] = {0xA1, 0xA2, 0xA3};
  Driver sample_driver = create_sample(topic, AIR_RATE_2400, 1, 1);
  read_pin_sequence[2] = 0; // Let read with AUX at 0 for two times
  read_pin_sequence[3] = 0;
  dynamic_read_pin = stub_read_pin_sequence;
  const char raw_message[] = "abcdefghi";
  const Destination target = {0xA1, 0xA2, 0xA3};
  unsigned long result =
      Driver_Send(&sample_driver, &target, raw_message, sizeof(raw_message));
  CHECK_EQUAL(sizeof(raw_message), result);
  CHECK_EQUAL(TIMER_CALLS_ON_CREATING_DRIVER + (TIMER_CALLS_ON_SENDING * 3) + 7,
              progressive_ms);
}