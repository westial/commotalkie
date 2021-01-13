#include "CppUTest/TestHarness.h"

extern "C" {
#include "MessageFormatter.h"
}

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
  Message message = MessageFormatter_Pack("01234567890A");
  MEMCMP_EQUAL("234567890A", message.body, 10);
  MEMCMP_EQUAL("01", message.meta, 2);
}

TEST(Packing, PackStruct) {
  const void* input = &sampleValue;
  Message message = MessageFormatter_Pack(input);
  MEMCMP_EQUAL(input, &message, 12);
}

TEST(Packing, UnpackStruct) {
  const void* input = &sampleValue;
  Message message = MessageFormatter_Pack(input);
  struct StubMsg output = {};
  MessageFormatter_Unpack(message, &output);
  CHECK_EQUAL(sampleValue.a2, output.a2);
  CHECK_EQUAL(sampleValue.b4, output.b4);
  MEMCMP_EQUAL(sampleValue.c3, output.c3, 3);
  CHECK_EQUAL(sampleValue.d1, output.d1);
  CHECK_EQUAL(sampleValue.e2, output.e2);
}