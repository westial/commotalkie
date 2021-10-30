

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEVALIDATOR_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEVALIDATOR_H_

#include "Message.h"

#ifdef __cplusplus
extern "C" {
#endif

int MessageValidator_Check(const Message* message);

void MessageValidator_Sign(Message* message);

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEVALIDATOR_H_
