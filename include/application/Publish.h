//
// Created by jaume on 1/26/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_

#ifdef __cplusplus
extern "C" {
#endif

void Publish_Create(
    const char *salt,
    const void *push_fn);

void Publish_Invoke(
    const char *topic,
    const unsigned char port,
    const unsigned char id,
    const char *body);

void Publish_Destroy();

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_
