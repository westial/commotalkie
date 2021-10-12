//
// Created by jaume on 10/8/21.
//

#ifndef COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_
#define COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_

#include "Timer.h"
#ifdef __cplusplus
extern "C" {
#endif

#define LOW_VALUE 0
#define HIGH_VALUE 1

typedef struct IOCallback {
  int (*read_pin)(int);
  void (*write_pin)(int, int);
  void (*write_to_serial)(void *, int);
} IOCallback;

typedef struct RadioParams {
  char address[2];
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

typedef enum State { NORMAL, SLEEP, ERROR } State;

typedef struct Driver {
  char address[2];
  char channel;
  char air_data_rate;
  PinMap pins;
  State state;
  int fixed_on;
  int low_power_on;
  Timer timer;
  unsigned long timeout_at;
} Driver;

Driver Driver_Create(PinMap pins, RadioParams *params, IOCallback *io,
                        Timer timer, unsigned long timeout_ms);

int Driver_Send(Driver *, const char *, unsigned long size);

#ifdef __cplusplus
}
#endif

#endif // COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_
