
#include <stdlib.h>
#include <string.h>

static unsigned char *buffer = 0;
static int size = 0;
/**
 * Listen and get an event from the address.
 * @param address listening from
 * @param content recipient for the content
 * @param size maximum allowed buffer size
 * @return int number of bytes received or -1 in case of error. Returns 0 if
 * nothing.
 */
static int (*listen)(const unsigned char *address, unsigned char *content,
                     const unsigned long size) = 0;
static void (*turn_on)() = 0;
static void (*turn_off)() = 0;

void Receiver_Create(const void *listen_, const void *turn_on_,
                     const void *turn_off_, int buffer_size) {
  listen =
      (int(*)(const unsigned char *, unsigned char *, const unsigned long))listen_;
  turn_on = (void(*)())turn_on_;
  turn_off = (void(*)())turn_off_;
  buffer = malloc(buffer_size);
  size = buffer_size;
}

void Receiver_TurnOn() { turn_on(); }

void Receiver_TurnOff() { turn_off(); }

void Receiver_Read(unsigned char *content) { memcpy(content, buffer, size); }

int Receiver_Listen(const unsigned char *address) {
  return listen(address, buffer, size);
}

void Receiver_Destroy() {
  if (0 != buffer)
    free(buffer);
  listen = 0;
}