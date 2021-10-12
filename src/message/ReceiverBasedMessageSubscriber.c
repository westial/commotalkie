//
// Created by jaume on 1/20/21.
//
#include "Message.h"
#include "MessageSubscriber.h"
#include "Receiver.h"
#include "Timer.h"
#include <stddef.h>

#define TIMEOUT_OFF 0

#define NO_BUFFER 0

static unsigned long timeout_at;
static Timer timer;

void MessageSubscriber_Create(
    const void *pull_function,
    const void *epoch_function) {
  Receiver_Create(pull_function, MESSAGE_LENGTH);
  timer = Timer_Create(epoch_function);
  timeout_at = TIMEOUT_OFF;
}

void MessageSubscriber_CountDown(const unsigned long timeout_millis) {
  timeout_at = timeout_millis;
  Timer_Start(&timer);
}

Result MessageSubscriber_Pull(const char* topic, const Message *message) {
  int available = NO_BUFFER;
  while (NO_BUFFER == available) {
    available = Receiver_listen(topic);
    if (Timer_IsRunning(&timer) && Timer_GetMillis(&timer) > timeout_at) return Timeout;
  }
  if (NO_BUFFER > available) return IOError;
  Receiver_read((const char *) message);
  return Success;
}

void MessageSubscriber_Destroy() {
  Receiver_Destroy();
  Timer_Destroy();
}