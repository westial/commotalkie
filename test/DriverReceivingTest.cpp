#include "CppUTest/TestHarness.h"
#include "Driver.h"
#include "EByte.h"
#include "helper/DriverTestHelper.cpp"

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

TEST_GROUP(DriverReceiving){void setup() override{driverHelperSetup();
}
}
;

TEST(DriverReceiving, ReceiveNothing) {
  dynamic_from_serial = stub_read_nothing_from_serial;
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", AIR_RATE_2400, 1, 1);
  char buffer[MAX_TEST_INDEX];
  int result = Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  CHECK_EQUAL(0, result);
}

TEST(DriverReceiving, SetStateToNormal) {
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  Driver_TurnOn(&sample_driver);
  CHECK_EQUAL(spy_write_pin_args[2][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[2][1], OFF);
  CHECK_EQUAL(spy_write_pin_args[3][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[3][1], OFF);
}

TEST(DriverReceiving, ChangeStateToSleep) {
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  Driver_TurnOn(&sample_driver);
  Driver_TurnOff(&sample_driver);
  CHECK_EQUAL(spy_write_pin_args[4][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[4][1], ON);
  CHECK_EQUAL(spy_write_pin_args[5][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[5][1], ON);
}

TEST(DriverReceiving, ReceiveSomething) {
  char sample[] = "hello baby";
  dynamic_from_serial = spy_read_from_serial;
  read_pin_sequence[2] = 0; // Let read with AUX at 0 for one time
  dynamic_read_pin = stub_read_pin_sequence;
  memcpy(stub_read_from_serial_buffer, sample, sizeof(sample));
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  int result = Driver_Receive(&sample_driver, buffer, sizeof(buffer));
  MEMCMP_EQUAL(sample, buffer, sizeof(sample));
  CHECK_EQUAL(1, result);
}

TEST(DriverReceiving, ReceiveSomethingByChunks) {
  char chunk1[] = "1234";
  char chunk2[] = "5678";
  read_pin_sequence[2] = 0; // Let read with AUX at 0 for two times
  read_pin_sequence[3] = 0;
  dynamic_read_pin = stub_read_pin_sequence;
  dynamic_from_serial = spy_read_from_serial_by_chunks;
  memcpy(stub_read_from_serial_buffer_4_char_chunks[0], chunk1, 4);
  memcpy(stub_read_from_serial_buffer_4_char_chunks[1], chunk2, 4);
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  int result = Driver_Receive(&sample_driver, buffer, 8);
  MEMCMP_EQUAL("12345678", buffer, 8);
  CHECK_EQUAL(1, result);
}

TEST(DriverReceiving, ReceiveIncompleteMessage) {
  char chunk1[] = "1234";
  read_pin_sequence[2] = 0; // Let read with AUX at 0 for one time only
  dynamic_read_pin = stub_read_pin_sequence;
  dynamic_from_serial = spy_read_from_serial_by_chunks;
  memcpy(stub_read_from_serial_buffer_4_char_chunks[0], chunk1, 4);
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  int result = Driver_Receive(&sample_driver, buffer, 8);
  CHECK_EQUAL(-1, result);
}

TEST(DriverReceiving, AuxNeverGetsHighBack) {
  progressive_ms = 0;
  read_pin_sequence[2] = 0; // Let read with AUX at 0 for one time only
  memset(read_pin_sequence + 2, 0,
         sizeof(read_pin_sequence) - (2 * sizeof(read_pin_sequence[0])));
  dynamic_read_pin = stub_read_pin_sequence_end_by_permanent_zero;
  dynamic_from_serial = stub_read_nothing_from_serial;
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  char buffer[MAX_TEST_INDEX];
  int result = Driver_Receive(&sample_driver, buffer, 8);
  CHECK_EQUAL(-1, result);
  CHECK_EQUAL(TIMER_CALLS_ON_CREATING_DRIVER +
                  default_timeouts[SERIAL_TIMEOUT_INDEX] + 1,
              progressive_ms);
}