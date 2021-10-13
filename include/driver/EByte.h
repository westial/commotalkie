//
// Created by jaume on 10/12/21.
//

#ifndef COMMOTALKIE_INCLUDE_DRIVER_EBYTE_H_
#define COMMOTALKIE_INCLUDE_DRIVER_EBYTE_H_

#include "Driver.h"

// -----------------------------------------------------------------------------

#define MS_DELAY_AFTER_AUX_HIGH 2

// -----------------------------------------------------------------------------

#define AIR_RATE_300 0b000		// 300 baud
#define AIR_RATE_1200 0b001		// 1200 baud
#define AIR_RATE_2400 0b010		// 2400 baud
#define AIR_RATE_4800 0b011		// 4800 baud
#define AIR_RATE_9600 0b100		// 9600 baud
#define AIR_RATE_19200 0b101	// 19200 baud

// -----------------------------------------------------------------------------
// Default configuration

#define PERSISTENT_CONF 0xC0

#define OPT_PULL_UP_ON 1
#define OPT_WAKEUP_250 0b000
#define OPT_FEC_ON 1

#define OPT_MIN_POWER 0b11
#define OPT_MAX_POWER 0b00

// Best configuration for full feature availability.
#define PARITY_BIT_8N1 0b00
#define BAUD_RATE_9600 0b011

// -----------------------------------------------------------------------------

#define ALL_BITS_LOW_BUT_PARITY 0xc0
#define ALL_BITS_LOW_BUT_BAUD_RATE 0x38
#define ALL_BITS_LOW_BUT_AIR_RATE 0x07

#define ALL_BITS_LOW_BUT_TRANSMISSION_MODE 0x80
#define ALL_BITS_LOW_BUT_PULL_UP 0x40
#define ALL_BITS_LOW_BUT_WAKE_UP_TIME 0x38
#define ALL_BITS_LOW_BUT_FEC_SWITCH 0x07
#define ALL_BITS_LOW_BUT_TRANSMIT_POWER 0x03

// -----------------------------------------------------------------------------

typedef enum SpeedLowerBit {
  PARITY_INDEX = 6,
  BAUD_RATE_INDEX = 3,
  AIR_RATE_INDEX = 0
} SpeedLowerBit;

typedef enum OptionLowerBit {
  TRANSMIT_MODE_INDEX = 7,
  PULL_UP_INDEX = 6,
  WAKE_UP_TIME_INDEX = 3,
  FEC_SWITCH_INDEX = 2,
  TRANSMIT_POWER_INDEX = 0
} OptionLowerBit;

typedef enum ConfigIndex {
  PERSISTENT = 0,
  ADDRESS_HIGH = 1,
  ADDRESS_LOW = 2,
  SPEED = 3,
  CHANNEL = 4,
  OPTIONS = 5
} ConfigIndex;

static void change_state_to_sleep(Driver* driver);
static void change_state_to_normal(Driver* driver);
static void set_configuration(Driver *driver);
static int value_speed(char parity, char baud_rate, char air_rate);
static int value_options(int transmit_mode,
                         int pull_up, char wake_up_time,
                         int fec_switch, char transmit_power);
static int wait_until_ready(Driver *driver);
static void delay(Driver *driver, unsigned long milliseconds);

static int (*read_pin_callback)(int);
static void (*write_pin_callback)(int, int);
static unsigned long (*write_to_serial_callback)(void*, unsigned long);

static Driver create_driver(PinMap *pins, RadioParams *params, Timer *timer,
                            const unsigned long *timeout_ms);

#endif //COMMOTALKIE_INCLUDE_DRIVER_EBYTE_H_
