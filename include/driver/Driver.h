//
// Created by jaume on 10/8/21.
//

#ifndef COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_
#define COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_

#include "Timer.h"
#ifdef __cplusplus
extern "C" {
#endif

#define OFF 0
#define ON 1

#define DRIVER_ADDRESS_SIZE 2
#define DRIVER_ADDRESS_LOW_INDEX 0
#define DRIVER_ADDRESS_HIGH_INDEX 1

#define MAX_TIMEOUTS 2

typedef struct Destination {
  char address_high;
  char address_low;
  char channel;
} Destination;

typedef struct IOCallback {
  int (*read_pin)(unsigned char);
  void (*write_pin)(unsigned char, unsigned char);
  unsigned long (*write_to_serial)(void *, unsigned long);
  unsigned long (*read_from_serial)(char *, unsigned long);
  int (*is_serial_available)();
} IOCallback;

typedef struct RadioParams {
  char address[DRIVER_ADDRESS_SIZE];
  char channel;
  char air_data_rate;
  int is_fixed_transmission;
  int full_transmission_power;
} RadioParams;

typedef struct PinMap {
  int m0;
  int m1;
  int aux;
} PinMap;

typedef enum State { NORMAL, SLEEP, ERROR, WARNING } State;

typedef struct Driver {
  char address[DRIVER_ADDRESS_SIZE];
  char channel;
  char air_data_rate;
  PinMap pins;
  State state;
  int fixed_on;
  int low_power_on;
  Timer timer;
  unsigned long timeouts[MAX_TIMEOUTS];
} Driver;

Driver Driver_Create(PinMap pins, RadioParams *params, IOCallback *io,
                     Timer* timer, unsigned long *timeouts);

unsigned long Driver_Send(Driver *driver, const Destination *destination,
                             const char *content, unsigned long size);

long Driver_Receive(Driver *driver, const char* buffer, unsigned long size);

#ifdef __cplusplus
}
#endif

#endif // COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_
