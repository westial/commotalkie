
#include <Publish.h>
static const char* publisher_salt = 0;
static const char* publisher_topic = 0;
static unsigned long (*publisher_send)(const char*, const char*, const unsigned long) = 0;

void PublisherBuilder_Create() {
}

void PublisherBuilder_SetSalt(const char *salt) {
  publisher_salt = salt;
}

void PublisherBuilder_SetTopic(const char *topic) {
  publisher_topic = topic;
}

void PublisherBuilder_SetSendCallback(
    unsigned long (*send_fn)(const char*, const char*, const unsigned long)) {
  publisher_send = send_fn;
}

int PublisherBuilder_Build() {
  if (0 == publisher_salt || 0 == publisher_topic || 0 == publisher_send)
    return -1;
  Publish_Create(publisher_salt, publisher_topic, (const void *) publisher_send);
  return 1;
}

void PublisherBuilder_Destroy() {
  publisher_salt = 0;
  publisher_topic = 0;
  publisher_send = 0;
}