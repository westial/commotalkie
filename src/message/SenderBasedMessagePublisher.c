
#include "Message.h"
#include "Sender.h"

void MessagePublisher_Create(const void *push_function) {
  Sender_Create(push_function);
}

int MessagePublisher_Push(const unsigned char* topic, Message *message) {
  if (MESSAGE_LENGTH ==
      Sender_Send(topic, (const unsigned char *)message, MESSAGE_LENGTH))
    return 1;
  return 0;
}

void MessagePublisher_Destroy() {
  Sender_Destroy();
}