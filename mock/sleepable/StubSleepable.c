#include "Sleepable.h"

void Sleepable_Init(
    Sleepable instance,
    SleepableInterface interface,
    unsigned char power_threshold) {
  instance->vtable = interface;
  instance->power_threshold = power_threshold;
  instance->is_sleeping = 0;
}

int Sleepable_TurnOff(Sleepable self) {
  self->vtable->TurnOff(self);
  self->is_sleeping = 1;
  return 1;
}

int Sleepable_TurnOn(Sleepable self) {
  self->vtable->TurnOn(self);
  self->is_sleeping = 0;
  return 1;
}

void Sleepable_Destroy(Sleepable self) {
  self->vtable->Destroy(self);
}