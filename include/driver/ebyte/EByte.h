//
// Created by jaume on 10/12/21.
//

#ifndef COMMOTALKIE_INCLUDE_DRIVER_EBYTE_H_
#define COMMOTALKIE_INCLUDE_DRIVER_EBYTE_H_

#include "Driver.h"

// -----------------------------------------------------------------------------

#define AIR_RATE_300 0b000		// 300 baud
#define AIR_RATE_1200 0b001		// 1200 baud
#define AIR_RATE_2400 0b010		// 2400 baud
#define AIR_RATE_4800 0b011		// 4800 baud
#define AIR_RATE_9600 0b100		// 9600 baud
#define AIR_RATE_19200 0b101	// 19200 baud

// -----------------------------------------------------------------------------

#define PERSISTENT_CONF 0xC0

// Best configuration for full feature availability.
#define PARITY_BIT_8N1 0b00
#define BAUD_RATE_9600 0b011

#define ALL_BITS_LOW_BUT_PARITY 0xc0
#define ALL_BITS_LOW_BUT_BAUD_RATE 0x38
#define ALL_BITS_LOW_BUT_AIR_RATE 0x07

// -----------------------------------------------------------------------------

#define ADDRESS_LOW_INDEX 0
#define ADDRESS_HIGH_INDEX 1

// -----------------------------------------------------------------------------

typedef enum LowerBit {
  PARITY_INDEX = 6,
  BAUD_RATE_INDEX = 3,
  AIR_RATE_INDEX = 0
} LowerBit;

typedef enum ConfigIndex {
  PERSISTENT = 0,
  ADDRESS_HIGH = 1,
  ADDRESS_LOW = 2,
  SPEED = 3,
  CHANNEL = 4,
  OPTIONS = 5
} ConfigIndex;

static void set_mode_sleep(Driver*);
static void set_mode_normal(Driver*);
static void set_configuration(Driver *driver);
static int value_speed(char parity, char baud_rate, char air_rate);
static int wait_until_ready(Driver *driver);

static int (*read_pin_callback)(int);
static void (*write_pin_callback)(int, int);
static void (*write_to_serial_callback)(void*, int);

static Driver create_driver(PinMap, const char*, const char*, const char*);

#endif //COMMOTALKIE_INCLUDE_DRIVER_EBYTE_H_
