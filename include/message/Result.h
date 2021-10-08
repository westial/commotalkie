//
// Created by jaume on 1/29/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_RESULT_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_RESULT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  Timeout,
  IOError,
  Success,
  NotValid
} Result;

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_MESSAGE_RESULT_H_
