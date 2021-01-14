//
// Created by jaume on 1/13/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGECRYPTER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGECRYPTER_H_

#include "Message.h"

void MessageCrypter_Create(const char* originalSalt);

void MessageCrypter_Destroy(void);

void MessageCrypter_Encrypt(const Message* message, char* encrypted);

void MessageCrypter_Decrypt(const char* raw, Message* decrypted);

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGECRYPTER_H_
