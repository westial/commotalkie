//
// Created by jaume on 1/26/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_

void Publish_Create(
    const char *salt,
    const void *push_fn);

void Publish_Invoke(
    const char *topic,
    const unsigned char port,
    const unsigned char id,
    const unsigned char *body);

void Publish_Destroy();

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_
