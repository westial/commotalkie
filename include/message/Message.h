/**
 *
 * A message is a 12 bytes only package to transmit information between commo
 * nodes.
 *
 * This is the standard package and all information between commo nodes has to
 * use this format only.
 *
 * First 2 bytes of the input are reserved for metadata and the remaining 10
 * bytes contain the information body.
 */

#ifndef COMMOTALKIE_SRC_MESSAGE_MESSAGE_H_
#define COMMOTALKIE_SRC_MESSAGE_MESSAGE_H_

#pragma pack(push)
#pragma pack(2)

#define MESSAGE_LENGTH          12
#define MESSAGE_META_LENGTH     2
#define MESSAGE_BODY_LENGTH     (MESSAGE_LENGTH - MESSAGE_META_LENGTH)

typedef struct Message {
  unsigned char meta[MESSAGE_META_LENGTH];
  unsigned char body[MESSAGE_BODY_LENGTH];
} Message;

#pragma pack(pop)

#endif //COMMOTALKIE_SRC_MESSAGE_MESSAGE_H_
