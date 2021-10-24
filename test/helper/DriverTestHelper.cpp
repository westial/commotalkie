#include "Driver.h"
#include <EByte.h>
#include <cstring>

// -----------------------------------------------------------------------------

#define MAX_TEST_INDEX 100

// Epoch service stub changes progressive_ms at:
//  * 2 times to start and get millis to get sleep state.
//  * 1 time to start and MS_DELAY_AFTER_AUX_HIGH to get delayed as required.
//  * 5 for waiting after setting configuration.
#define TIMER_CALLS_ON_CREATING_DRIVER (MS_DELAY_AFTER_READY_CHECK + 3 + 5)

static void driverHelperSetup();

static char sample_address[DRIVER_ADDRESS_SIZE];
static Driver create_sample(const char *, char, int, int);

static int (*dynamic_read_pin)(unsigned char pin);

static int stub_read_pin_return;
static int stub_read_pin_call_count;
static int stub_read_pin_toggle_at;
static int stub_read_pin(unsigned char pin);

static int sequence_return[MAX_TEST_INDEX];
static int stub_read_pin_sequence(unsigned char pin);
static int stub_read_pin_sequence_end_by_permanent_zero(unsigned char pin);

static void reset_write_pin();
static void spy_write_pin(unsigned char pin, unsigned char value);
static int spy_write_pin_args[MAX_TEST_INDEX][2];
static int spy_write_pin_args_index;

static void reset_write_to_serial();
static unsigned long spy_write_to_serial(void *content, unsigned long size);
static char spy_write_to_serial_arg_1[MAX_TEST_INDEX][MAX_TEST_INDEX];
static unsigned long spy_write_to_serial_arg_2[MAX_TEST_INDEX];
static int spy_write_to_serial_call_count;

static unsigned long (*dynamic_from_serial)(char *buffer, unsigned long size,
                                            unsigned long position);
static unsigned long fake_read_from_serial(char *buffer, unsigned long size,
                                           unsigned long position);
static void reset_read_from_serial();
static unsigned long spy_read_from_serial(char *buffer, unsigned long size,
                                          unsigned long position);

static char stub_read_from_serial_buffer_4_char_chunks[10][4];
static int buffer_chunks_index;
static unsigned long spy_read_from_serial_by_chunks(char *buffer,
                                                    unsigned long size,
                                                    unsigned long position);
static char stub_read_from_serial_buffer[MAX_TEST_INDEX];

static int (*dynamic_serial_is_available)();
static unsigned long spy_availability_check_count;
static int serial_is_available_by_value_stub();
static unsigned short stub_availability_value;

static unsigned long stub_read_nothing_from_serial(char *buffer,
                                                   unsigned long size,
                                                   unsigned long position);

static unsigned long default_timeouts[MAX_TIMEOUTS];
static unsigned long progressive_ms;
static unsigned long stub_progressive_epoch_ms_fn();

// -----------------------------------------------------------------------------

void driverHelperSetup() {
  default_timeouts[MODE_TIMEOUT_INDEX] = 5 * 1000;
  default_timeouts[SERIAL_TIMEOUT_INDEX] = 2 * 1000;
  progressive_ms = 1;
  memset(sequence_return, 1, sizeof(sequence_return));
  stub_read_pin_return = 1; // ready by default
  stub_read_pin_call_count = 0;
  stub_read_pin_toggle_at = 1000;
  dynamic_read_pin = stub_read_pin;
  reset_write_pin();
  reset_write_to_serial();
  dynamic_from_serial = fake_read_from_serial;
  reset_read_from_serial();
  stub_availability_value = 1;
  dynamic_serial_is_available = serial_is_available_by_value_stub;
  spy_availability_check_count = 0;
}

int stub_read_pin(unsigned char pin) {
  return (stub_read_pin_toggle_at == stub_read_pin_call_count++)
             ? !stub_read_pin_return
             : stub_read_pin_return;
}

int stub_read_pin_sequence(unsigned char pin) {
  return sequence_return[stub_read_pin_call_count++];
}

int stub_read_pin_sequence_end_by_permanent_zero(unsigned char pin) {
  if (sizeof(sequence_return)/sizeof(sequence_return[0]) > stub_read_pin_call_count)
    return stub_read_pin_sequence(pin);
  return 0;
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

unsigned long stub_read_nothing_from_serial(char *buffer, unsigned long size,
                                            unsigned long position) {
  memset(buffer, '\x00', size);
  return 0;
}

unsigned long spy_read_from_serial(char *buffer, unsigned long size,
                                   unsigned long position) {
  memcpy(buffer, stub_read_from_serial_buffer, size);
  stub_availability_value = 0;
  return size;
}

unsigned long spy_read_from_serial_by_chunks(char *buffer, unsigned long size,
                                             unsigned long position) {
  memcpy(
      buffer + position, stub_read_from_serial_buffer_4_char_chunks[buffer_chunks_index],
      sizeof(stub_read_from_serial_buffer_4_char_chunks[buffer_chunks_index]));
  buffer_chunks_index++;
  position += sizeof(stub_read_from_serial_buffer_4_char_chunks[buffer_chunks_index]);
  return position;
}

unsigned long fake_read_from_serial(char *buffer, unsigned long size,
                                    unsigned long position) {
  return size;
}

int serial_is_available_by_value_stub() {
  ++spy_availability_check_count;
  return stub_availability_value == 1;
}

unsigned long stub_progressive_epoch_ms_fn() { return progressive_ms += 1; }

Driver create_sample(const char *topic, const char air_data_rate,
                     const int is_fixed, const int full_power) {
  PinMap pins = {1, 2, 3};
  RadioParams params = {
      {topic[DRIVER_ADDRESS_HIGH_INDEX], topic[DRIVER_ADDRESS_LOW_INDEX]},
      topic[2],
      air_data_rate,
      is_fixed,
      full_power};
  Timer timer = Timer_Create((const void *)stub_progressive_epoch_ms_fn);
  IOCallback io = {dynamic_read_pin, spy_write_pin, spy_write_to_serial,
                   dynamic_from_serial};
  return Driver_Create(pins, &params, &io, &timer, default_timeouts);
}

void reset_write_to_serial() {
  spy_write_to_serial_call_count = 0;
  for (auto &content_arg : spy_write_to_serial_arg_1) {
    memset(content_arg, '\0', sizeof(content_arg));
  }
  for (auto &size_arg : spy_write_to_serial_arg_2) {
    size_arg = 0;
  }
}

void reset_read_from_serial() {
  memset(stub_read_from_serial_buffer, '\x00',
         sizeof(stub_read_from_serial_buffer));
  buffer_chunks_index = 0;
  for (auto &chunk : stub_read_from_serial_buffer_4_char_chunks) {
    memset(chunk, '\x00', sizeof(chunk));
  }
}

void reset_write_pin() {
  spy_write_pin_args_index = 0;
  for (auto &arg : spy_write_pin_args) {
    arg[0] = -1;
    arg[1] = -1;
  }
}