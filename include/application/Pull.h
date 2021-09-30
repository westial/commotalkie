//
// Created by jaume on 1/28/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PULL_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PULL_H_

#include "Result.h"

void Pull_Create(const char *salt,
                 const char *topic,
                 const void *pull_function,
                 const void *epoch_function,
                 const unsigned long timeout_millis,
                 const unsigned char *to_id);

Result Pull_Invoke(
    const char *topic,
    unsigned char *port,
    unsigned char *id,
    unsigned char *body);

void Pull_Destroy();

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PULL_H_
