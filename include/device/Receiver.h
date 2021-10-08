//
// Created by jaume on 1/19/21.
//

#ifndef COMMOTALKIE_INCLUDE_DEVICE_RECEIVER_H_
#define COMMOTALKIE_INCLUDE_DEVICE_RECEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

void Receiver_Create(const void *listen_function, int buffer_size);

void Receiver_read(const char* content);

int Receiver_listen(const char* address);

void Receiver_Destroy(void);

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_DEVICE_RECEIVER_H_
