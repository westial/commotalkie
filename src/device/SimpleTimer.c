//
// Created by jaume on 2/3/21.
//

#include "Timer.h"

Timer Timer_Create(const void *epoch_millis_fn) {
  Timer mark;
  mark.started_at = 0;
  mark.epoch_millis = epoch_millis_fn;
  return mark;
}

void Timer_Start(Timer *timer) {
  if (Timer_IsRunning(timer))
    Timer_Stop(timer);
  else
    timer->started_at = timer->epoch_millis();
}

void Timer_Stop(Timer *timer) {
  timer->started_at = 0;
}

int Timer_IsRunning(Timer *timer) {
  return 0 != timer->started_at;
}

unsigned long Timer_GetMillis(Timer *timer) {
  if (0 == timer->started_at) return 0;
  return timer->epoch_millis() - timer->started_at;
}

void Timer_Destroy(Timer *timer) {
  Timer_Stop(timer);
}
