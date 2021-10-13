#include "ebyte/EByte.h"
#include <string.h>

Driver Driver_Create(PinMap pins, RadioParams *params, IOCallback *io,
                        Timer timer, unsigned long timeout_ms) {
  read_pin_callback = io->read_pin;
  write_pin_callback = io->write_pin;
  write_to_serial_callback = io->write_to_serial;
  Driver self = create_driver(&pins, params, &timer, &timeout_ms);
  change_state_to_sleep(&self);
  set_configuration(&self);
  return self;
}

unsigned long Driver_Send(Driver *driver, const char *content, unsigned long size) {
  char data[sizeof(driver->address) + 1 + size];
  unsigned long written;
  change_state_to_normal(driver);
  data[0] = driver->address[DRIVER_ADDRESS_HIGH_INDEX];
  data[1] = driver->address[DRIVER_ADDRESS_LOW_INDEX];
  data[2] = driver->channel;
  memcpy(data + 3, content, size);
  written = write_to_serial_callback(data, sizeof(data));
  change_state_to_sleep(driver);
  return (wait_until_ready(driver) && sizeof(data) == written) ? size : 0;
}

int value_options(int transmit_mode, int pull_up, char wake_up_time,
                  int fec_switch, char transmit_power) {
  return ((transmit_mode & 0xFF) << TRANSMIT_MODE_INDEX) |
         ((pull_up & 0xFF) << PULL_UP_INDEX) |
         ((wake_up_time & 0xFF) << WAKE_UP_TIME_INDEX) |
         ((fec_switch & 0xFF) << FEC_SWITCH_INDEX) |
         ((transmit_power & 0xFF) << TRANSMIT_POWER_INDEX);
}

int value_speed(const char parity, const char baud_rate, const char air_rate) {
  return ((parity & 0xFF) << PARITY_INDEX) |
         ((baud_rate & 0xFF) << BAUD_RATE_INDEX) |
         ((air_rate & 0xFF) << AIR_RATE_INDEX);
}

void set_configuration(Driver *driver) {
  char config[6];
  config[PERSISTENT] = PERSISTENT_CONF;
  config[ADDRESS_HIGH] = driver->address[DRIVER_ADDRESS_HIGH_INDEX];
  config[ADDRESS_LOW] = driver->address[DRIVER_ADDRESS_LOW_INDEX];
  config[SPEED] =
      (char)value_speed(PARITY_BIT_8N1, BAUD_RATE_9600, driver->air_data_rate);
  config[CHANNEL] = driver->channel;
  config[OPTIONS] = (char)value_options(
      driver->fixed_on, OPT_PULL_UP_ON, OPT_WAKEUP_250, OPT_FEC_ON,
      driver->low_power_on ? OPT_MIN_POWER : OPT_MAX_POWER);
  write_to_serial_callback(config, sizeof(config));
}

int wait_until_ready(Driver *driver) {
  int on_time;
  Timer_Start(&driver->timer);
  do {
    on_time = driver->timeout_at > Timer_GetMillis(&driver->timer);
  }while (OFF == read_pin_callback(driver->pins.aux) && on_time);
  return on_time;
}

void change_state_to_sleep(Driver *driver) {
  write_pin_callback(driver->pins.m0, ON);
  write_pin_callback(driver->pins.m1, ON);
  driver->state = (wait_until_ready(driver)) ? SLEEP : ERROR;
}

void change_state_to_normal(Driver *driver) {
  write_pin_callback(driver->pins.m0, OFF);
  write_pin_callback(driver->pins.m1, OFF);
  driver->state = (wait_until_ready(driver)) ? NORMAL : ERROR;
}

Driver create_driver(PinMap *pins, RadioParams *params, Timer *timer,
                     const unsigned long *timeout_ms) {
  Driver result;
  result.address[0] = params->address[DRIVER_ADDRESS_LOW_INDEX];
  result.address[1] = params->address[DRIVER_ADDRESS_HIGH_INDEX];
  result.channel = params->channel;
  result.air_data_rate = params->air_data_rate;
  result.fixed_on = params->is_fixed_transmission;
  result.low_power_on = !params->full_transmission_power;
  result.pins = *pins;
  result.timer = *timer;
  result.timeout_at = *timeout_ms;
  return result;
}