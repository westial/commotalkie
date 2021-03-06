#include "EByte.h"
#include <string.h>

static void change_state_to_sleep(Driver *driver);
static void change_state_to_normal(Driver *driver);
static void set_configuration(Driver *driver);
static int value_speed(unsigned char parity, unsigned char baud_rate,
                       unsigned char air_rate);
static unsigned char value_options(unsigned char transmit_mode,
                                   unsigned char pull_up,
                                   unsigned char wake_up_time,
                                   unsigned char fec_switch,
                                   unsigned char transmit_power);
static int wait_until_event(Driver *driver, int (*event)(Driver *));
static int wait_until_mode_is_ready(Driver *driver);
static int wait_until_writing_finished(Driver *driver);
static void delay(Timer *timer, unsigned long timeout);

static int (*read_pin_callback)(unsigned char);
static void (*write_pin_callback)(unsigned char, unsigned char);
static unsigned long (*write_to_serial_callback)(unsigned char *,
                                                 unsigned long);
static unsigned long (*read_from_serial_callback)(unsigned char *,
                                                  unsigned long, unsigned long);
static void (*clear_serial_callback)();

static Driver create_driver(PinMap *pins, RadioParams *params, Timer *timer,
                            const unsigned long *timeout_ms);

static void start_timer(Timer *timer);
static void stop_timer(Timer *timer);
static int is_timer_on_time(Timer *timer, unsigned long timeout_at);

static int is_ebyte_busy(Driver *driver);
static int is_waiting_for_mode_on_time(Driver *driver);

static int is_serial_receiving_on_time(Driver *driver);

Driver Driver_Create(PinMap pins, RadioParams *params, IOCallback *io,
                     Timer *timer, unsigned long *timeouts) {
  read_pin_callback = io->read_pin;
  write_pin_callback = io->write_pin;
  write_to_serial_callback = io->write_to_serial;
  read_from_serial_callback = io->read_from_serial;
  clear_serial_callback = io->clear_serial;
  Driver self = create_driver(&pins, params, timer, timeouts);
  Driver_TurnOff(&self);
  set_configuration(&self);
  clear_serial_callback();
  return self;
}

unsigned long Driver_Send(Driver *driver, const Destination *destination,
                          const void *content, unsigned long size) {
  unsigned char head_size = 3;
  unsigned char data[head_size + size];
  unsigned long written;
  Driver_TurnOn(driver);
  data[0] = destination->address_high;
  data[1] = destination->address_low;
  data[2] = destination->channel;
  memcpy(data + head_size, content, size);
  written = write_to_serial_callback(data, sizeof(data));
  wait_until_writing_finished(driver);
  Driver_TurnOff(driver);
  return (SLEEP == driver->state && sizeof(data) == written) ? size : 0;
}

int Driver_Receive(Driver *driver, unsigned char *buffer, unsigned long size) {
  unsigned long position = 0;
  memset(buffer, 0, size);
  start_timer(&driver->timer);
  while (is_ebyte_busy(driver) && position < size) {
    if (!is_serial_receiving_on_time(driver))
      return -1;
    position = read_from_serial_callback(buffer, size, position);
  }
  stop_timer(&driver->timer);
  delay(&driver->timer, MS_DELAY_AFTER_READY_CHECK);
  return position == size ? 1 : position == 0 ? 0 : -1;
}

int Driver_TurnOn(Driver *driver) {
  change_state_to_normal(driver);
  return driver->state == NORMAL;
}

int Driver_TurnOff(Driver *driver) {
  change_state_to_sleep(driver);
  return driver->state == SLEEP;
}

int is_serial_receiving_on_time(Driver *driver) {
  return is_timer_on_time(&driver->timer,
                          driver->timeouts[SERIAL_TIMEOUT_INDEX]);
}

unsigned char value_options(const unsigned char transmit_mode,
                            const unsigned char pull_up,
                            const unsigned char wake_up_time,
                            const unsigned char fec_switch,
                            const unsigned char transmit_power) {
  return ((transmit_mode & 0xFF) << TRANSMIT_MODE_INDEX) |
         ((pull_up & 0xFF) << PULL_UP_INDEX) |
         ((wake_up_time & 0xFF) << WAKE_UP_TIME_INDEX) |
         ((fec_switch & 0xFF) << FEC_SWITCH_INDEX) |
         ((transmit_power & 0xFF) << TRANSMIT_POWER_INDEX);
}

int value_speed(const unsigned char parity, const unsigned char baud_rate,
                const unsigned char air_rate) {
  return ((parity & 0xFF) << PARITY_INDEX) |
         ((baud_rate & 0xFF) << BAUD_RATE_INDEX) |
         ((air_rate & 0xFF) << AIR_RATE_INDEX);
}

void set_configuration(Driver *driver) {
  unsigned char config[6];
  config[PERSISTENT] = PERSISTENT_CONF;
  config[ADDRESS_HIGH] = driver->address[DRIVER_ADDRESS_HIGH_INDEX];
  config[ADDRESS_LOW] = driver->address[DRIVER_ADDRESS_LOW_INDEX];
  config[SPEED] =
      value_speed(PARITY_BIT_8N1, BAUD_RATE_9600, driver->air_data_rate);
  config[CHANNEL] = driver->channel;
  config[OPTIONS] = value_options(
      driver->fixed_on, OPT_PULL_UP_ON, OPT_WAKEUP_250, OPT_FEC_ON,
      driver->low_power_on ? OPT_MIN_POWER : OPT_MAX_POWER);
  write_to_serial_callback(config, sizeof(config));
  wait_until_mode_is_ready(driver);
}

int is_ebyte_busy(Driver *driver) {
  return OFF == read_pin_callback(driver->pins.aux);
}

int is_waiting_for_mode_on_time(Driver *driver) {
  return is_timer_on_time(&driver->timer, driver->timeouts[MODE_TIMEOUT_INDEX]);
}

int wait_until_event(Driver *driver, int (*event)(Driver *)) {
  int on_time;
  start_timer(&driver->timer);
  do {
    on_time = event(driver);
  } while (is_ebyte_busy(driver) && on_time);
  if (on_time) {
    stop_timer(&driver->timer);
    delay(&driver->timer, MS_DELAY_AFTER_READY_CHECK);
    return 1;
  }
  return 0;
}

int wait_until_mode_is_ready(Driver *driver) {
  return wait_until_event(driver, is_waiting_for_mode_on_time);
}

int wait_until_writing_finished(Driver *driver) {
  return wait_until_event(driver, is_serial_receiving_on_time);
}

void delay(Timer *timer, unsigned long timeout) {
  start_timer(timer);
  while (is_timer_on_time(timer, timeout))
    ;
}

void change_state_to_sleep(Driver *driver) {
  write_pin_callback(driver->pins.m0, ON);
  write_pin_callback(driver->pins.m1, ON);
  driver->state = (wait_until_mode_is_ready(driver)) ? SLEEP : WARNING;
}

void start_timer(Timer *timer) { Timer_Start(timer); }
void stop_timer(Timer *timer) { Timer_Stop(timer); }

int is_timer_on_time(Timer *timer, unsigned long timeout_at) {
  if (timeout_at > Timer_GetMillis(timer))
    return 1;
  else {
    stop_timer(timer);
    return 0;
  }
}

void change_state_to_normal(Driver *driver) {
  write_pin_callback(driver->pins.m0, OFF);
  write_pin_callback(driver->pins.m1, OFF);
  driver->state = (wait_until_mode_is_ready(driver)) ? NORMAL : WARNING;
}

Driver create_driver(PinMap *pins, RadioParams *params, Timer *timer,
                     const unsigned long *timeout_ms) {
  Driver result;
  result.address[0] = params->address[DRIVER_ADDRESS_HIGH_INDEX];
  result.address[1] = params->address[DRIVER_ADDRESS_LOW_INDEX];
  result.channel = params->channel;
  result.air_data_rate = params->air_data_rate;
  result.fixed_on = params->is_fixed_transmission;
  result.low_power_on = !params->full_transmission_power;
  result.pins = *pins;
  result.timer = *timer;
  result.timeouts[MODE_TIMEOUT_INDEX] = timeout_ms[MODE_TIMEOUT_INDEX];
  result.timeouts[SERIAL_TIMEOUT_INDEX] = timeout_ms[SERIAL_TIMEOUT_INDEX];
  return result;
}