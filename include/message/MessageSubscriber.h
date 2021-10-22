//
// Created by jaume on 1/19/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_

#include "Message.h"
#include "Result.h"

#ifdef __cplusplus
extern "C" {
#endif

void MessageSubscriber_Create(
    const void *pull,
    const void *epoch,
    const void *turn_on,
    const void *turn_off);

/**
 * Start timeout countdown
 * @param timeout_at
 */
void MessageSubscriber_CountDown(unsigned long timeout_at);

/**
 * Listen from a topic
 * @param char*
 * @param Message*
 * @return
 */
Result MessageSubscriber_Pull(const char* topic, const Message *message);

void MessageSubscriber_Destroy();

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
