#include "DriverSleepable.h"
#include <malloc.h>
#include "../mock/sleepable/StubSleepable.c"

typedef struct DriverSleepableStruct
{
  SleepableStruct base;
  int (*Sleep)(Driver *driver);
  int (*WakeUp)(Driver *driver);
  Driver *driver;
} DriverSleepableStruct;


// Fulfill the interface methods -----------------------------------------------

static int turn_off(Sleepable super) {
  DriverSleepable self = (DriverSleepable) super;
  self->Sleep(self->driver);
  return 1;
}

static int turn_on(Sleepable super) {
  DriverSleepable self = (DriverSleepable) super;
  self->WakeUp(self->driver);
  return 1;
}

static void destroy(Sleepable super) {
  DriverSleepable self = (DriverSleepable) super;
  free(self);
}

static SleepableInterfaceStruct interface = {
    .TurnOff = turn_off,
    .TurnOn = turn_on,
    .Destroy = destroy
};

// -----------------------------------------------------------------------------


Sleepable DriverSleepable_Create(
    unsigned char power_threshold,
    int (*sleep)(Driver *driver),
    int (*wake_up)(Driver *driver),
    Driver *driver
) {
  DriverSleepable self = calloc(1, sizeof(DriverSleepableStruct));
  Sleepable_Init((Sleepable)self, &interface, power_threshold);
  self->Sleep = sleep;
  self->WakeUp = wake_up;
  self->driver = driver;
  return (Sleepable)self;
}