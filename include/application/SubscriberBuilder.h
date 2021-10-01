//
// Created by jaume on 9/30/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_SUBSCRIBERBUILDER_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_SUBSCRIBERBUILDER_H_

void SubscriberBuilder_Create();

void SubscriberBuilder_SetSalt(const char *salt);

void SubscriberBuilder_SetListenCallback(
    int (*)(const char *, const char *, const int));

void SubscriberBuilder_SetTimeService(unsigned long (*)(void));

void SubscriberBuilder_SetTimeout(const unsigned long *);

void SubscriberBuilder_SetId(const unsigned char *);

int SubscriberBuilder_Build();

void SubscriberBuilder_Destroy();

#endif //COMMOTALKIE_INCLUDE_APPLICATION_SUBSCRIBERBUILDER_H_
