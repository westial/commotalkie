//
// Created by jaume on 2/3/21.
//

#ifndef COMMOTALKIE_INCLUDE_DEVICE_TIMER_H_
#define COMMOTALKIE_INCLUDE_DEVICE_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Timer {
  unsigned long started_at;
  unsigned long (*epoch_millis)();
} Timer;

Timer Timer_Create(const void *epoch_ms_fn);

void Timer_Start(Timer *timer);

void Timer_Stop(Timer *timer);

int Timer_IsRunning(Timer *timer);

unsigned long Timer_GetMillis(Timer *timer);

void Timer_Destroy(Timer *timer);

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_DEVICE_TIMER_H_
