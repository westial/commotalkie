//
// Created by jaume on 2/3/21.
//

#ifndef COMMOTALKIE_INCLUDE_DEVICE_TIMER_H_
#define COMMOTALKIE_INCLUDE_DEVICE_TIMER_H_

void Timer_Create(const void *epoch_ms_fn);

void Timer_Start(void);

int Timer_IsRunning(void);

unsigned long Timer_GetMillis(void);

void Timer_Destroy(void);

#endif //COMMOTALKIE_INCLUDE_DEVICE_TIMER_H_
