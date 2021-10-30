#include "CppUTest/TestHarness.h"
#include "Driver.h"
#include "EByte.h"
#include "helper/DriverTestHelper.cpp"

// -----------------------------------------------------------------------------

TEST_GROUP(DriverConfiguring){void setup() override{driverHelperSetup();
}
}
;

TEST(DriverConfiguring, CreateADriver) {
  PinMap pins = {1, 2, 3};
  RadioParams params;
  params.address[0] = 0x08;
  params.address[1] = 0x09;
  params.channel = 0x06;
  params.air_data_rate = 0;
  Timer timer = Timer_Create((const void *)stub_progressive_epoch_ms_fn);
  IOCallback io;
  io.read_pin = stub_read_pin;
  io.write_pin = spy_write_pin;
  io.write_to_serial = spy_write_to_serial;
  io.read_from_serial = spy_read_from_serial;
  io.clear_serial = spy_clear_serial;
  Driver instance = Driver_Create(pins, &params, &io, &timer, default_timeouts);
  CHECK_EQUAL(1, instance.pins.m0);
  CHECK_EQUAL(2, instance.pins.m1);
  CHECK_EQUAL(3, instance.pins.aux);
  CHECK_EQUAL(0x08, instance.address[0]);
  CHECK_EQUAL(0x09, instance.address[1]);
  CHECK_EQUAL(0x06, instance.channel);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
}

TEST(DriverConfiguring, SetStateAsSleepAfterInitialization) {
  Driver sample_driver =
      create_sample((const unsigned char *)"\x08\x09\x06", 0, 0, 0);
  CHECK_EQUAL(SLEEP, sample_driver.state);
  CHECK_EQUAL(spy_write_pin_args[0][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[0][1], ON);
  CHECK_EQUAL(spy_write_pin_args[1][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[1][1], ON);
}

TEST(DriverConfiguring, SetAddressAndChannel) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL('\xA2', spy_write_to_serial_arg_1[0][ADDRESS_LOW]);
  CHECK_EQUAL('\xA1', spy_write_to_serial_arg_1[0][ADDRESS_HIGH]);
  CHECK_EQUAL('\xA3', spy_write_to_serial_arg_1[0][CHANNEL]);
}

TEST(DriverConfiguring, SetDefaultParityBit) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(PARITY_BIT_8N1,
              (spy_write_to_serial_arg_1[0][SPEED] & ALL_BITS_LOW_BUT_PARITY) >>
                  PARITY_INDEX);
}

TEST(DriverConfiguring, SetDefaultUartBaudRate) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(BAUD_RATE_9600, (spy_write_to_serial_arg_1[0][SPEED] &
                               ALL_BITS_LOW_BUT_BAUD_RATE) >>
                                  BAUD_RATE_INDEX);
}

TEST(DriverConfiguring, SetParameterizedAirDataRate) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", AIR_RATE_19200, 0, 0);
  CHECK_EQUAL(AIR_RATE_19200, (spy_write_to_serial_arg_1[0][SPEED] &
                               ALL_BITS_LOW_BUT_AIR_RATE) >>
                                  AIR_RATE_INDEX);
}

TEST(DriverConfiguring, SetParameterizedTransmissionMode) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 1, 0);
  CHECK_EQUAL(1, (spy_write_to_serial_arg_1[0][OPTIONS] &
                  ALL_BITS_LOW_BUT_TRANSMISSION_MODE) >>
                     TRANSMIT_MODE_INDEX);
}

TEST(DriverConfiguring, SetDefaultPullUpState) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_PULL_UP_ON, (spy_write_to_serial_arg_1[0][OPTIONS] &
                               ALL_BITS_LOW_BUT_PULL_UP) >>
                                  PULL_UP_INDEX);
}

TEST(DriverConfiguring, SetDefaultWakeUpTime) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_WAKEUP_250, (spy_write_to_serial_arg_1[0][OPTIONS] &
                               ALL_BITS_LOW_BUT_WAKE_UP_TIME) >>
                                  WAKE_UP_TIME_INDEX);
}

TEST(DriverConfiguring, SetDefaultFECSwitch) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_FEC_ON, (spy_write_to_serial_arg_1[0][OPTIONS] &
                           ALL_BITS_LOW_BUT_FEC_SWITCH) >>
                              FEC_SWITCH_INDEX);
}

TEST(DriverConfiguring, SetFullTransmissionPower) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 1);
  CHECK_EQUAL(OPT_MAX_POWER, (spy_write_to_serial_arg_1[0][OPTIONS] &
                              ALL_BITS_LOW_BUT_TRANSMIT_POWER) >>
                                 TRANSMIT_POWER_INDEX);
}

TEST(DriverConfiguring, SetLowestTransmissionPower) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_MIN_POWER, (spy_write_to_serial_arg_1[0][OPTIONS] &
                              ALL_BITS_LOW_BUT_TRANSMIT_POWER) >>
                                 TRANSMIT_POWER_INDEX);
}

TEST(DriverConfiguring, ConfiguresADriverOnCreating) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL('\xA2', spy_write_to_serial_arg_1[0][ADDRESS_LOW]);
  CHECK_EQUAL('\xA1', spy_write_to_serial_arg_1[0][ADDRESS_HIGH]);
  CHECK_EQUAL('\xA3', spy_write_to_serial_arg_1[0][CHANNEL]);
}

TEST(DriverConfiguring, WaitUntilAuxGetsHigh) {
  stub_read_pin_return = 0;
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  // Read pin call count + setting configuration timeout
  CHECK_EQUAL(stub_read_pin_toggle_at + default_timeouts[MODE_TIMEOUT_INDEX], stub_read_pin_call_count - 1);
}

TEST(DriverConfiguring, TimeoutWaitingForHighOnAux) {
  stub_read_pin_return = 0;
  default_timeouts[MODE_TIMEOUT_INDEX] = progressive_ms + 1;
  Driver sample_driver =
      create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(WARNING, sample_driver.state);
}

TEST(DriverConfiguring, DelayAfterAuxGetsHigh) {
  progressive_ms = 0;
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL(TIMER_CALLS_ON_CREATING_DRIVER, progressive_ms);
}

TEST(DriverConfiguring, ClearBufferAfterSettingConfiguration) {
  create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_clear_serial_call_count);
}