//
// Created by jaume on 9/28/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHERBUILDER_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHERBUILDER_H_

void PublisherBuilder_Create();

void PublisherBuilder_SetSalt(const char *salt);
void PublisherBuilder_SetSendCallback(unsigned long (*send)(const char*, const char*, const unsigned long));
int PublisherBuilder_Build();

void PublisherBuilder_Destroy();

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHERBUILDER_H_
