//
// Created by jaume on 1/20/21.
//
#include "Message.h"
#include "MessageSubscriber.h"
#include "Receiver.h"
#include "Timer.h"

#define TIMEOUT_OFF 0

#define NO_BUFFER 0

static Result receive(const char* topic, void *message);
static unsigned long timeout_at;
static Timer timer;

void MessageSubscriber_Create(
    const void *pull,
    const void *epoch,
    const void *turn_on,
    const void *turn_off) {
  Receiver_Create(pull, turn_on, turn_off, MESSAGE_LENGTH);
  Receiver_TurnOff();
  timer = Timer_Create(epoch);
  timeout_at = TIMEOUT_OFF;
}

void MessageSubscriber_StartCountDown(const unsigned long timeout_at_) {
  timeout_at = timeout_at_;
  Timer_Start(&timer);
}

void MessageSubscriber_StopCountDown() {
  Timer_Stop(&timer);
}

Result receive(const char* topic, void *message) {
  int available = NO_BUFFER;
  while (NO_BUFFER == available) {
    available = Receiver_Listen(topic);
    if (Timer_IsRunning(&timer) && Timer_GetMillis(&timer) > timeout_at) {
      return Timeout;
    }
  }
  if (NO_BUFFER > available) return IOError;
  Receiver_Read((void*)message);
  return Success;
}

Result MessageSubscriber_Pull(const char* topic, const Message *message) {
  Result result;
  Receiver_TurnOn();
  result = receive(topic, (void *)message);
  Receiver_TurnOff();
  return result;
}

void MessageSubscriber_Destroy() {
  Receiver_Destroy();
  Timer_Destroy(&timer);
}