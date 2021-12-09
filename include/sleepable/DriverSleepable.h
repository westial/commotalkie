#ifndef COMMOTALKIE_INCLUDE_SLEEPABLE_DRIVERSLEEPABLE_H_
#define COMMOTALKIE_INCLUDE_SLEEPABLE_DRIVERSLEEPABLE_H_

#include "Sleepable.h"
#include <Driver.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DriverSleepableStruct *DriverSleepable;

Sleepable DriverSleepable_Create(
    unsigned char power_threshold,
    int (*sleep)(Driver *driver),
    int (*wake_up)(Driver *driver),
    Driver *driver
);

#ifdef __cplusplus
}
#endif

#endif // COMMOTALKIE_INCLUDE_SLEEPABLE_DRIVERSLEEPABLE_H_
