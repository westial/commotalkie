//
// Created by jaume on 1/28/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_READENCRYPTEDMESSAGE_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_READENCRYPTEDMESSAGE_H_

#include "Result.h"

void ReadEncryptedMessage_Create(
    const char *salt,
    const char *topic,
    const void *pull_function,
    const void *epoch_function,
    unsigned long timeout_millis
    );

Result ReadEncryptedMessage_Invoke(
    unsigned char *port,
    unsigned char *id,
    unsigned char *body);

void ReadEncryptedMessage_Destroy();

#endif //COMMOTALKIE_INCLUDE_APPLICATION_READENCRYPTEDMESSAGE_H_
