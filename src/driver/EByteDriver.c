#include "ebyte/EByte.h"

Driver Driver_Create(
    PinMap pins,
    RadioParams params,
    int (*read_pin)(int),
    void (*write_pin)(int, int),
    void (*write_to_serial)(void *, int)
) {
  read_pin_callback = read_pin;
  write_pin_callback = write_pin;
  write_to_serial_callback = write_to_serial;
  Driver self = create_driver(pins,
                              params.address,
                              &params.channel,
                              &params.air_data_rate);
  set_mode_sleep(&self);
  set_configuration(&self);
  return self;
}

int Driver_Send(Driver *driver, const char *content, unsigned long size) {
  // TODO
  return 1;
}

int value_speed(const char parity, const char baud_rate, const char air_rate) {
  return ((parity & 0xFF) << PARITY_INDEX)
      | ((baud_rate & 0xFF) << BAUD_RATE_INDEX)
      | ((air_rate & 0xFF) << AIR_RATE_INDEX);
}

void set_configuration(Driver *driver) {
  char config[6];
  config[PERSISTENT] = PERSISTENT_CONF;
  config[ADDRESS_HIGH] = driver->address[ADDRESS_HIGH_INDEX];
  config[ADDRESS_LOW] = driver->address[ADDRESS_LOW_INDEX];
  config[SPEED] =
      (char) value_speed(PARITY_BIT_8N1, BAUD_RATE_9600, driver->air_data_rate);
  config[CHANNEL] = driver->channel;
  config[OPTIONS] = 0;
  write_to_serial_callback((void *) config, sizeof(config));
}

int wait_until_ready(Driver *driver) {
  while (LOW_VALUE == read_pin_callback(driver->pins.aux));
  return 1;
}

void set_mode_sleep(Driver *driver) {
  write_pin_callback(driver->pins.m0, HIGH_VALUE);
  write_pin_callback(driver->pins.m1, HIGH_VALUE);
  wait_until_ready(driver);
  driver->mode = SLEEP;
}

Driver create_driver(
    PinMap pins,
    const char *address,
    const char *channel,
    const char *air_data_rate
) {
  Driver result;
  result.address[0] = address[ADDRESS_LOW_INDEX];
  result.address[1] = address[ADDRESS_HIGH_INDEX];
  result.channel = *channel;
  result.air_data_rate = *air_data_rate;
  result.pins = pins;
  return result;
}