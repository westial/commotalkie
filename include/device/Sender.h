//
// Created by jaume on 1/15/21.
//

#ifndef COMMOTALKIE_INCLUDE_DEVICE_SENDER_H_
#define COMMOTALKIE_INCLUDE_DEVICE_SENDER_H_

#ifdef __cplusplus
extern "C" {
#endif

void Sender_Create(const void *send_function);

unsigned long Sender_Send(const char* address, const char* content, unsigned long size);

void Sender_Destroy(void);

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_DEVICE_SENDER_H_
