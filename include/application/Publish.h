

#ifndef COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_
#define COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_

#ifdef __cplusplus
extern "C" {
#endif

void Publish_Create(
    const char *salt,
    const void *push_fn);

void Publish_Invoke(
    const unsigned char *topic,
    unsigned char port,
    unsigned char id,
    const unsigned char *body);

void Publish_Destroy();

#ifdef __cplusplus
}
#endif

#endif //COMMOTALKIE_INCLUDE_APPLICATION_PUBLISH_H_
