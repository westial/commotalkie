#include "Driver.h"
#include <cstring>

// -----------------------------------------------------------------------------

#define MAX_TEST_INDEX 100

static char sample_address[DRIVER_ADDRESS_SIZE];
static Driver create_sample(const char *, char, int, int);

static int stub_read_pin_return;
static int stub_read_pin_call_count;
static int stub_read_pin_toggle_at;
static int stub_read_pin(unsigned char pin);

static void reset_write_pin();
static void spy_write_pin(unsigned char pin, unsigned char value);
static int spy_write_pin_args[MAX_TEST_INDEX][2];
static int spy_write_pin_args_index;

static void reset_write_to_serial();
static unsigned long spy_write_to_serial(void *content, unsigned long size);
static char spy_write_to_serial_arg_1[MAX_TEST_INDEX][MAX_TEST_INDEX];
static unsigned long spy_write_to_serial_arg_2[MAX_TEST_INDEX];
static int spy_write_to_serial_call_count;

static unsigned long default_timeout;
static unsigned long progressive_ms;
static unsigned long stub_progressive_epoch_ms_fn();

// -----------------------------------------------------------------------------

int stub_read_pin(unsigned char pin) {
  return (stub_read_pin_toggle_at == stub_read_pin_call_count++)
             ? !stub_read_pin_return
             : stub_read_pin_return;
}

void spy_write_pin(unsigned char pin, unsigned char value) {
  spy_write_pin_args[spy_write_pin_args_index][0] = pin;
  spy_write_pin_args[spy_write_pin_args_index][1] = value;
  ++spy_write_pin_args_index;
}

unsigned long spy_write_to_serial(void *content, unsigned long size) {
  memcpy(spy_write_to_serial_arg_1[spy_write_to_serial_call_count], content,
         size);
  spy_write_to_serial_arg_2[spy_write_to_serial_call_count] = size;
  ++spy_write_to_serial_call_count;
  return size;
}

unsigned long stub_progressive_epoch_ms_fn() {
  return progressive_ms += 1;
}

Driver create_sample(const char *topic, const char air_data_rate,
                     const int is_fixed, const int full_power) {
  PinMap pins = {1, 2, 3};
  RadioParams params = {
      {topic[DRIVER_ADDRESS_HIGH_INDEX], topic[DRIVER_ADDRESS_LOW_INDEX]}, topic[2],
      air_data_rate, is_fixed, full_power};
  Timer timer = Timer_Create((const void *)stub_progressive_epoch_ms_fn);
  IOCallback io = {stub_read_pin, spy_write_pin, spy_write_to_serial};
  return Driver_Create(pins, &params, &io, timer, default_timeout);
}

void reset_write_to_serial() {
  spy_write_to_serial_call_count = 0;
  for (auto &content_arg : spy_write_to_serial_arg_1) {
    memset(content_arg, '\0', MAX_TEST_INDEX);
  }
  for (auto &size_arg : spy_write_to_serial_arg_2) {
    size_arg = 0;
  }
}

void reset_write_pin() {
  spy_write_pin_args_index = 0;
  for (auto &arg : spy_write_pin_args) {
    arg[0] = -1;
    arg[1] = -1;
  }
}