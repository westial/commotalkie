//
// Created by jaume on 1/13/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGECRYPTER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGECRYPTER_H_

#include "Message.h"

void MessageCrypter_Encrypt(
    const Message* message,
    const char* salt,
    char* encrypted);

void MessageCrypter_Decrypt(
    const char* raw,
    const char* salt,
    Message* decrypted);

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGECRYPTER_H_
