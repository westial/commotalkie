#include "Driver.h"
#include "CppUTest/TestHarness.h"
#include <cstring>
#include <ebyte/EByte.h>

// -----------------------------------------------------------------------------

#define MAX_TEST_INDEX 100

static char sample_address[2];
static Driver create_sample(const char *, char, int, int);

static int stub_read_pin_return;
static int stub_read_pin_call_count;
static int stub_read_pin_toggle_at;
static int stub_read_pin(int pin);

static void spy_write_pin(int pin, int value);
static int spy_write_pin_args[MAX_TEST_INDEX][2];
static int spy_write_pin_args_index;

static void spy_write_to_serial(void *, int);
static char spy_write_to_serial_arg_1[MAX_TEST_INDEX][MAX_TEST_INDEX];
static int spy_write_to_serial_arg_2[MAX_TEST_INDEX];
static int spy_write_to_serial_call_count;

static unsigned long default_timeout;
static unsigned long progressive_ms;
static unsigned long stub_progressive_epoch_ms_fn();

// -----------------------------------------------------------------------------

int stub_read_pin(int pin) {
  return (stub_read_pin_toggle_at == stub_read_pin_call_count++)
             ? !stub_read_pin_return
             : stub_read_pin_return;
}

void spy_write_pin(int pin, int value) {
  spy_write_pin_args[spy_write_pin_args_index][0] = pin;
  spy_write_pin_args[spy_write_pin_args_index][1] = value;
  ++spy_write_pin_args_index;
}

void spy_write_to_serial(void *content, int size) {
  memcpy(spy_write_to_serial_arg_1[spy_write_to_serial_call_count], content,
         size);
  spy_write_to_serial_arg_2[spy_write_to_serial_call_count] = size;
  ++spy_write_to_serial_call_count;
}

unsigned long stub_progressive_epoch_ms_fn() {
  return progressive_ms += 1;
}

Driver create_sample(const char *topic, const char air_data_rate,
                     const int is_fixed, const int full_power) {
  PinMap pins = {1, 2, 3};
  RadioParams params;
  params.address[0] = topic[0];
  params.address[1] = topic[1];
  params.channel = topic[2];
  params.air_data_rate = air_data_rate;
  params.is_fixed_transmission = is_fixed;
  params.full_transmission_power = full_power;
  Timer timer = Timer_Create((const void *)stub_progressive_epoch_ms_fn);
  return Driver_Create(pins, params, stub_read_pin, spy_write_pin,
                       spy_write_to_serial, timer, default_timeout);
}

// -----------------------------------------------------------------------------

TEST_GROUP(IntegratingDriver){void setup() override{default_timeout = 20000;
progressive_ms = 1;
stub_read_pin_return = 1; // ready by default
spy_write_pin_args_index = 0;
stub_read_pin_call_count = 0;
stub_read_pin_toggle_at = 1000;
spy_write_to_serial_call_count = 0;
memset(sample_address, '\0', 2);
for (auto &arg : spy_write_pin_args) {
  arg[0] = -1;
  arg[1] = -1;
}
for (auto &content_arg : spy_write_to_serial_arg_1) {
  memset(content_arg, '\0', MAX_TEST_INDEX);
}
for (auto &size_arg : spy_write_to_serial_arg_2) {
  size_arg = 0;
}
}
}
;

TEST(IntegratingDriver, CreateADriver) {
  PinMap pins = {1, 2, 3};
  RadioParams params;
  params.address[0] = '\x08';
  params.address[1] = '\x09';
  params.channel = '\x06';
  params.air_data_rate = 0;
  Timer timer = Timer_Create((const void *)stub_progressive_epoch_ms_fn);
  Driver instance = Driver_Create(pins, params, stub_read_pin, spy_write_pin,
                                  spy_write_to_serial, timer, default_timeout);
  CHECK_EQUAL(1, instance.pins.m0);
  CHECK_EQUAL(2, instance.pins.m1);
  CHECK_EQUAL(3, instance.pins.aux);
  CHECK_EQUAL(0x08, instance.address[0]);
  CHECK_EQUAL(0x09, instance.address[1]);
  CHECK_EQUAL(0x06, instance.channel);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
}

TEST(IntegratingDriver, CreateADriverAtSleepMode) {
  Driver sample_driver = create_sample("\x08\x09\x06", 0, 0, 0);
  CHECK_EQUAL(SLEEP, sample_driver.state);
  CHECK_EQUAL(spy_write_pin_args[0][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[0][1], HIGH_VALUE);
  CHECK_EQUAL(spy_write_pin_args[1][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[1][1], HIGH_VALUE);
}

TEST(IntegratingDriver, SetAddressAndChannel) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL('\xA1', spy_write_to_serial_arg_1[0][ADDRESS_LOW]);
  CHECK_EQUAL('\xA2', spy_write_to_serial_arg_1[0][ADDRESS_HIGH]);
  CHECK_EQUAL('\xA3', spy_write_to_serial_arg_1[0][CHANNEL]);
}

TEST(IntegratingDriver, SetDefaultParityBit) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(PARITY_BIT_8N1,
              (spy_write_to_serial_arg_1[0][SPEED] & ALL_BITS_LOW_BUT_PARITY) >>
                  PARITY_INDEX);
}

TEST(IntegratingDriver, SetDefaultUartBaudRate) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(BAUD_RATE_9600, (spy_write_to_serial_arg_1[0][SPEED] &
                               ALL_BITS_LOW_BUT_BAUD_RATE) >>
                                  BAUD_RATE_INDEX);
}

TEST(IntegratingDriver, SetParameterizedAirDataRate) {
  create_sample("\xA1\xA2\xA3", AIR_RATE_19200, 0, 0);
  CHECK_EQUAL(AIR_RATE_19200, (spy_write_to_serial_arg_1[0][SPEED] &
                               ALL_BITS_LOW_BUT_AIR_RATE) >>
                                  AIR_RATE_INDEX);
}

TEST(IntegratingDriver, SetParameterizedTransmissionMode) {
  create_sample("\xA1\xA2\xA3", 0, 1, 0);
  CHECK_EQUAL(1, (spy_write_to_serial_arg_1[0][OPTIONS] &
                  ALL_BITS_LOW_BUT_TRANSMISSION_MODE) >>
                     TRANSMIT_MODE_INDEX);
}

TEST(IntegratingDriver, SetDefaultPullUpState) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_PULL_UP_ON, (spy_write_to_serial_arg_1[0][OPTIONS] &
                               ALL_BITS_LOW_BUT_PULL_UP) >>
                                  PULL_UP_INDEX);
}

TEST(IntegratingDriver, SetDefaultWakeUpTime) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_WAKEUP_250, (spy_write_to_serial_arg_1[0][OPTIONS] &
                               ALL_BITS_LOW_BUT_WAKE_UP_TIME) >>
                                  WAKE_UP_TIME_INDEX);
}

TEST(IntegratingDriver, SetDefaultFECSwitch) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_FEC_ON, (spy_write_to_serial_arg_1[0][OPTIONS] &
                           ALL_BITS_LOW_BUT_FEC_SWITCH) >>
                              FEC_SWITCH_INDEX);
}

TEST(IntegratingDriver, SetFullTransmissionPower) {
  create_sample("\xA1\xA2\xA3", 0, 0, 1);
  CHECK_EQUAL(OPT_MAX_POWER, (spy_write_to_serial_arg_1[0][OPTIONS] &
                              ALL_BITS_LOW_BUT_TRANSMIT_POWER) >>
                                 TRANSMIT_POWER_INDEX);
}

TEST(IntegratingDriver, SetLowestTransmissionPower) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(OPT_MIN_POWER, (spy_write_to_serial_arg_1[0][OPTIONS] &
                              ALL_BITS_LOW_BUT_TRANSMIT_POWER) >>
                                 TRANSMIT_POWER_INDEX);
}

TEST(IntegratingDriver, ConfiguresADriverOnCreating) {
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL('\xA1', spy_write_to_serial_arg_1[0][ADDRESS_LOW]);
  CHECK_EQUAL('\xA2', spy_write_to_serial_arg_1[0][ADDRESS_HIGH]);
  CHECK_EQUAL('\xA3', spy_write_to_serial_arg_1[0][CHANNEL]);
}

TEST(IntegratingDriver, WaitUntilAuxGetsHigh) {
  stub_read_pin_return = 0;
  create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(1, spy_write_to_serial_call_count);
  CHECK_EQUAL(stub_read_pin_toggle_at, stub_read_pin_call_count - 1);
}

TEST(IntegratingDriver, TimeoutWaitingForHighOnAux) {
  stub_read_pin_return = 0;
  default_timeout = progressive_ms + 1;
  Driver driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  CHECK_EQUAL(ERROR, driver.state);
}

TEST(IntegratingDriver, Sends) {
  Driver sample_driver = create_sample("\xA1\xA2\xA3", 0, 0, 0);
  const char raw_message[] = "abcdefghi";
  int result = Driver_Send(&sample_driver, raw_message, sizeof(raw_message));
  CHECK_EQUAL(1, result);
}
