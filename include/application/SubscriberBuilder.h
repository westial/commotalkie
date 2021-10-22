//
// Created by jaume on 9/30/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_SUBSCRIBERBUILDER_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_SUBSCRIBERBUILDER_H_

#ifdef __cplusplus
extern "C" {
#endif

void SubscriberBuilder_Create();

void SubscriberBuilder_SetSalt(const char *salt);

void SubscriberBuilder_SetListenCallback(
    int (*)(const char *, char *, const unsigned long));

void SubscriberBuilder_SetReceiverStateCallback(void(*)(), void(*)());

void SubscriberBuilder_SetTimeService(unsigned long (*)());

void SubscriberBuilder_SetTimeout(const unsigned long *);

void SubscriberBuilder_SetId(const unsigned char *);

int SubscriberBuilder_Build();

void SubscriberBuilder_Destroy();

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_APPLICATION_SUBSCRIBERBUILDER_H_
