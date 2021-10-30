

#ifndef COMMOTALKIE_INCLUDE_DEVICE_RECEIVER_H_
#define COMMOTALKIE_INCLUDE_DEVICE_RECEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

void Receiver_Create(const void *listen, const void *turn_on,
                        const void *turn_off, int buffer_size);

void Receiver_Read(unsigned char *content);

void Receiver_TurnOn();

void Receiver_TurnOff();

int Receiver_Listen(const unsigned char *address);

void Receiver_Destroy();

#ifdef __cplusplus
}
#endif

#endif // COMMOTALKIE_INCLUDE_DEVICE_RECEIVER_H_
