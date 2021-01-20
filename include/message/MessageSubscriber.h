//
// Created by jaume on 1/19/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_

#include "Message.h"

typedef enum {
  TimeoutError,
  IOError,
  Success
} Result;

void MessageSubscriber_Create(const void *pull_function, const char *topic);

/**
 * Get the
 * @param buffer
 * @return
 */
Result MessageSubscriber_Pull(const Message *message);

void MessageSubscriber_Destroy();

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
