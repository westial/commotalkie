#include "CppUTest/TestHarness.h"
#include "Driver.h"
#include "EByte.h"
#include "helper/DriverTestHelper.cpp"

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

TEST_GROUP(DriverReceiving){void setup() override{helperSetup();
}
};

TEST(DriverReceiving, ReceiveNothing) {
  dynamic_from_serial = stub_read_nothing_from_serial;
  Driver sample_driver = create_sample("\xA1\xA2\xA3", AIR_RATE_2400, 1, 1);
  char buffer[MAX_TEST_INDEX];
  long result = Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  CHECK_EQUAL(0, result);
}

TEST(DriverReceiving, SetStateToNormalBeforeReceiving) {
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  CHECK_EQUAL(spy_write_pin_args[2][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[2][1], OFF);
  CHECK_EQUAL(spy_write_pin_args[3][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[3][1], OFF);
}

TEST(DriverReceiving, SetStateToSleepAfterReceiving) {
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  CHECK_EQUAL(spy_write_pin_args[4][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[4][1], ON);
  CHECK_EQUAL(spy_write_pin_args[5][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[5][1], ON);
}

TEST(DriverReceiving, ReceiveSomething) {
  char sample[] = "hello baby";
  dynamic_from_serial = spy_read_from_serial;
  memcpy(stub_read_from_serial_buffer, sample, sizeof(sample));
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  MEMCMP_EQUAL(sample, buffer, sizeof(sample));
  CHECK_EQUAL(1, spy_availability_check_count);
}

TEST(DriverReceiving, ExpireReceivingTimeout) {
  char expected[MAX_TEST_INDEX];
  char sample[] = "hello baby";
  dynamic_from_serial = spy_read_from_serial;
  memcpy(stub_read_from_serial_buffer, sample, sizeof(sample));
  dynamic_serial_is_available = serial_is_never_available;
  memset(expected, '\x00', sizeof(expected));
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  MEMCMP_EQUAL(expected, buffer, MAX_TEST_INDEX);
  CHECK_EQUAL(default_timeout[RECEIVING_TIMEOUT_INDEX], spy_availability_check_count);
}