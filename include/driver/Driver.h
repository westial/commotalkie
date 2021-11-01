

#ifndef COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_
#define COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_

#include "Timer.h"
#ifdef __cplusplus
extern "C" {
#endif

#define OFF 0
#define ON 1

#define DRIVER_ADDRESS_SIZE 2

#define DRIVER_ADDRESS_HIGH_INDEX 0
#define DRIVER_ADDRESS_LOW_INDEX 1

#define MAX_TIMEOUTS 2

typedef struct Destination {
  unsigned char address_high;
  unsigned char address_low;
  unsigned char channel;
} Destination;

typedef struct IOCallback {
  int (*read_pin)(unsigned char);
  void (*write_pin)(unsigned char, unsigned char);
  unsigned long (*write_to_serial)(unsigned char *, unsigned long);
  unsigned long (*read_from_serial)(unsigned char *, unsigned long, unsigned long);
  void (*clear_serial)();
} IOCallback;

typedef struct RadioParams {
  unsigned char address[DRIVER_ADDRESS_SIZE];
  unsigned char channel;
  unsigned char air_data_rate;
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
  unsigned char address[DRIVER_ADDRESS_SIZE];
  unsigned char channel;
  unsigned char air_data_rate;
  PinMap pins;
  State state;
  int fixed_on;
  int low_power_on;
  Timer timer;
  unsigned long timeouts[MAX_TIMEOUTS];
} Driver;

Driver Driver_Create(PinMap pins, RadioParams *params, IOCallback *io,
                     Timer *timer, unsigned long *timeouts);

unsigned long Driver_Send(Driver *driver, const Destination *destination,
                          const void *content, unsigned long size);

int Driver_Receive(Driver *driver, unsigned char *buffer, unsigned long size);

int Driver_TurnOn(Driver *driver);
int Driver_TurnOff(Driver *driver);

#ifdef __cplusplus
}
#endif

#endif // COMMOTALKINO_LIB_EBYTE_EBYTEDRIVER_H_
