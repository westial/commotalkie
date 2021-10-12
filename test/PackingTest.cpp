#include "CppUTest/TestHarness.h"

#include "MessageFormatter.h"


// -----------------------------------------------------------------------------

#pragma pack(push)
#pragma pack(2)
struct StubMsg {
  uint16_t a2;
  uint32_t b4;
  char c3[3];
  uint8_t d1;
  uint16_t e2;
};
#pragma pack(pop)

static struct StubMsg sampleValue = {15000, 256000, 'a', 'b', 'c', 100, 10001};

// -----------------------------------------------------------------------------

TEST_GROUP(Packing) {
};

TEST(Packing, PackString) {
  Message message;
  const char sample[] = "0123456789AB";
  MessageFormatter_Pack(sample, &message);
  MEMCMP_EQUAL(sample, message.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(sample + MESSAGE_META_LENGTH, message.body, MESSAGE_BODY_LENGTH);
}

TEST(Packing, PackStruct) {
  const void* input = &sampleValue;
  Message message;
  MessageFormatter_Pack(input, &message);
  MEMCMP_EQUAL(input, &message, MESSAGE_LENGTH);
}

TEST(Packing, UnpackStruct) {
  const void* input = &sampleValue;
  Message message;
  MessageFormatter_Pack(input, &message);
  struct StubMsg output = {};
  MessageFormatter_Unpack(&message, &output);
  CHECK_EQUAL(sampleValue.a2, output.a2);
  CHECK_EQUAL(sampleValue.b4, output.b4);
  MEMCMP_EQUAL(sampleValue.c3, output.c3, 3);
  CHECK_EQUAL(sampleValue.d1, output.d1);
  CHECK_EQUAL(sampleValue.e2, output.e2);
}