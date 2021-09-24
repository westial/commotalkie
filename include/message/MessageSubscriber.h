//
// Created by jaume on 1/19/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_

#include "Message.h"
#include "Result.h"

void MessageSubscriber_Create(
    const void *pull_function,
    const void *epoch_function,
    unsigned long timeout_millis,
    const char *topic);

/**
 * Start timeout countdown
 */
void MessageSubscriber_CountDown(void);

/**
 * Get the
 * @param buffer
 * @return
 */
Result MessageSubscriber_Pull(const Message *message);

void MessageSubscriber_Destroy();

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
