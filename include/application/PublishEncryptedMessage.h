//
// Created by jaume on 1/26/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHENCRYPTEDMESSAGE_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHENCRYPTEDMESSAGE_H_

void PublishEncryptedMessage_Create(const char* topic, const char* salt, const void* push_fn);

void PublishEncryptedMessage_Invoke(unsigned char port, unsigned char id, unsigned char* body);

void PublishEncryptedMessage_Destroy();

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHENCRYPTEDMESSAGE_H_
