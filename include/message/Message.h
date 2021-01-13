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
 *
 * It is limited to exactly 12 bytes because of this limitation in a possible
 * implementation for Sigfox.
 */

#ifndef COMMOTALKIE_SRC_MESSAGE_MESSAGE_H_
#define COMMOTALKIE_SRC_MESSAGE_MESSAGE_H_

#pragma pack(push)
#pragma pack(2)

#define MESSAGE_LENGTH 12

typedef struct Message {
  unsigned char meta[2];
  unsigned char body[10];
} Message;

#pragma pack(pop)

#endif //COMMOTALKIE_SRC_MESSAGE_MESSAGE_H_
