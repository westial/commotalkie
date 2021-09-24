//
// Created by jaume on 1/20/21.
//
#include "Timer.h"
#include "Message.h"
#include "Receiver.h"
#include "MessageSubscriber.h"

static const char *address;
unsigned long timeout_at;

void MessageSubscriber_Create(
    const void *pull_function,
    const void *epoch_function,
    const char *topic) {
  Receiver_Create(pull_function, MESSAGE_LENGTH);
  Timer_Create(epoch_function);
  address = topic;
  timeout_at = 0;
}

void MessageSubscriber_CountDown(const unsigned long timeout_millis) {
  timeout_at = timeout_millis;
  Timer_Start();
}

Result MessageSubscriber_Pull(const Message *message) {
  int available = 0;
  while (0 == available) {
    available = Receiver_listen(address);
    if (Timer_IsRunning() && Timer_GetMillis() > timeout_at) return Timeout;
  }
  if (0 > available) return IOError;
  Receiver_read((const char *) message);
  return Success;
}

void MessageSubscriber_Destroy() {
  Receiver_Destroy();
}