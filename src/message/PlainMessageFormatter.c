
#include <string.h>
#include "MessageFormatter.h"

void MessageFormatter_Pack(const void* input, Message* message) {
  memcpy(message->meta, input, MESSAGE_META_LENGTH);
  memcpy(message->body, input + MESSAGE_META_LENGTH, MESSAGE_BODY_LENGTH);
}

void MessageFormatter_Unpack(const Message* message, void* output) {
  memcpy(output, message, MESSAGE_LENGTH);
}
