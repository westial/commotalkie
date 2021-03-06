

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

void MessageSubscriber_StartCountDown(unsigned long timeout_at);

void MessageSubscriber_StopCountDown();

/**
 * Listen from a topic
 * @param char*
 * @param Message*
 * @return
 */
Result MessageSubscriber_Pull(const unsigned char* topic, const Message *message);

void MessageSubscriber_Destroy();

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
