

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHERBUILDER_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHERBUILDER_H_

#ifdef __cplusplus
extern "C" {
#endif

void PublisherBuilder_Create();

void PublisherBuilder_SetSalt(const char *);

void PublisherBuilder_SetSendCallback(
    unsigned long (*)(const unsigned char*, const unsigned char*, const unsigned long));

int PublisherBuilder_Build();

void PublisherBuilder_Destroy();

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PUBLISHERBUILDER_H_
