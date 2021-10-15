#include "CppUTest/TestHarness.h"
#include "Driver.h"
#include "EByte.h"
#include "helper/DriverTestHelper.cpp"
#include <cstring>

// -----------------------------------------------------------------------------

TEST_GROUP(DriverConfiguring){void setup() override{default_timeout = 20000;
progressive_ms = 1;
stub_read_pin_return = 1; // ready by default
stub_read_pin_call_count = 0;
stub_read_pin_toggle_at = 1000;
memset(sample_address, '\0', 2);
reset_write_pin();
reset_write_to_serial();
}
}
;

TEST(DriverConfiguring, CreateADriver) {
  PinMap pins = {1, 2, 3};
  RadioParams params;
  params.address[0] = '\x08';
  params.address[1] = '\x09';
  params.channel = '\x06';
  params.air_data_rate = 0;
  Timer timer = Timer_Create((const void *)stub_progressive_epoch_ms_fn);
  IOCallback io;
  io.read_pin = stub_read_pin;
  io.write_pin = spy_write_pin;
  io.write_to_serial = spy_write_to_serial;
  Driver instance = Driver_Create(pins, &params, &io, timer, default_timeout);
  CHECK_EQUAL(1, instance.pins.m0);
  CHECK_EQUAL(2, instance.pins.m1);
  CHECK_EQUAL(3, instance.pins.aux);
  CHECK_EQUAL(0x08, instance.address[0]);
  CHECK_EQUAL(0x09, instance.address[1]);
  CHECK_EQUAL(0x06, instance.channel);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
}

TEST(DriverConfiguring, SetStateAsSleepAfterInitialization) {
  Driver sample_driver = create_sample("\x08\x09\x06", 0, 0, 0);
  CHECK_EQUAL(SLEEP, sample_driver.state);
  CHECK_EQUAL(spy_write_pin_args[0][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[0][1], ON);
  CHECK_EQUAL(spy_write_pin_args[1][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[1][1], ON);
}

TEST(DriverConfiguring, SetAddressAndChannel) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL('\xA2', spy_write_to_serial_arg_1[0][ADDRESS_LOW]);
  CHECK_EQUAL('\xA1', spy_write_to_serial_arg_1[0][ADDRESS_HIGH]);
  CHECK_EQUAL('\xA3', spy_write_to_serial_arg_1[0][CHANNEL]);
}

TEST(DriverConfiguring, SetDefaultParityBit) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(PARITY_BIT_8N1,
              (spy_write_to_serial_arg_1[0][SPEED] & ALL_BITS_LOW_BUT_PARITY) >>
                  PARITY_INDEX);
}

TEST(DriverConfiguring, SetDefaultUartBaudRate) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(BAUD_RATE_9600, (spy_write_to_serial_arg_1[0][SPEED] &
                               ALL_BITS_LOW_BUT_BAUD_RATE) >>
                                  BAUD_RATE_INDEX);
}

TEST(DriverConfiguring, SetParameterizedAirDataRate) {
  create_sample("\xA1\xA2\xA3", AIR_RATE_19200, 0, 0);
  CHECK_EQUAL(AIR_RATE_19200, (spy_write_to_serial_arg_1[0][SPEED] &
                               ALL_BITS_LOW_BUT_AIR_RATE) >>
                                  AIR_RATE_INDEX);
}

TEST(DriverConfiguring, SetParameterizedTransmissionMode) {
  create_sample("\xA1\xA2\xA3", 0, 1, 0);
  CHECK_EQUAL(1, (spy_write_to_serial_arg_1[0][OPTIONS] &
                  ALL_BITS_LOW_BUT_TRANSMISSION_MODE) >>
                     TRANSMIT_MODE_INDEX);
}

TEST(DriverConfiguring, SetDefaultPullUpState) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_PULL_UP_ON, (spy_write_to_serial_arg_1[0][OPTIONS] &
                               ALL_BITS_LOW_BUT_PULL_UP) >>
                                  PULL_UP_INDEX);
}

TEST(DriverConfiguring, SetDefaultWakeUpTime) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_WAKEUP_250, (spy_write_to_serial_arg_1[0][OPTIONS] &
                               ALL_BITS_LOW_BUT_WAKE_UP_TIME) >>
                                  WAKE_UP_TIME_INDEX);
}

TEST(DriverConfiguring, SetDefaultFECSwitch) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_FEC_ON, (spy_write_to_serial_arg_1[0][OPTIONS] &
                           ALL_BITS_LOW_BUT_FEC_SWITCH) >>
                              FEC_SWITCH_INDEX);
}

TEST(DriverConfiguring, SetFullTransmissionPower) {
  create_sample("\xA1\xA2\xA3", 0, 0, 1);
  CHECK_EQUAL(OPT_MAX_POWER, (spy_write_to_serial_arg_1[0][OPTIONS] &
                              ALL_BITS_LOW_BUT_TRANSMIT_POWER) >>
                                 TRANSMIT_POWER_INDEX);
}

TEST(DriverConfiguring, SetLowestTransmissionPower) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_MIN_POWER, (spy_write_to_serial_arg_1[0][OPTIONS] &
                              ALL_BITS_LOW_BUT_TRANSMIT_POWER) >>
                                 TRANSMIT_POWER_INDEX);
}

TEST(DriverConfiguring, ConfiguresADriverOnCreating) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL('\xA2', spy_write_to_serial_arg_1[0][ADDRESS_LOW]);
  CHECK_EQUAL('\xA1', spy_write_to_serial_arg_1[0][ADDRESS_HIGH]);
  CHECK_EQUAL('\xA3', spy_write_to_serial_arg_1[0][CHANNEL]);
}

TEST(DriverConfiguring, WaitUntilAuxGetsHigh) {
  stub_read_pin_return = 0;
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL(stub_read_pin_toggle_at, stub_read_pin_call_count - 1);
}

TEST(DriverConfiguring, TimeoutWaitingForHighOnAux) {
  stub_read_pin_return = 0;
  default_timeout = progressive_ms + 1;
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(ERROR, sample_driver.state);
}

TEST(DriverConfiguring, DelayAfterAuxGetsHigh) {
  progressive_ms = 0;
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  // Epoch service stub changes progressive_ms at:
  //  * 2 times to start and get millis to get sleep state.
  //  * 1 time to start and MS_DELAY_AFTER_AUX_HIGH to get delayed as required.
  //  * 2 times passes through the mode switch delay.of
  //  MS_DELAY_AFTER_MODE_SWITCH.
  CHECK_EQUAL(MS_DELAY_AFTER_AUX_HIGH + 5, progressive_ms);
}