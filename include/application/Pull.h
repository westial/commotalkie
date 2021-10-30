//
// Created by jaume on 1/28/21.
//

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PULL_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PULL_H_

#include "Result.h"

#ifdef __cplusplus
extern "C" {
#endif

void Pull_Create(const char *salt, const void *pull, const void *epoch,
                 const void *turn_on, const void *turn_off,
                 unsigned long timeout_at, const unsigned char *to_id);

Result Pull_Invoke(const unsigned char *topic, unsigned char *port,
                   unsigned char *id, unsigned char *body);

void Pull_Destroy();

#ifdef __cplusplus
}
#endif

#endif // COMMOTALKIE_INCLUDE_APPLICATION_PULL_H_
