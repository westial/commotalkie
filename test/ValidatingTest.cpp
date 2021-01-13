#include "CppUTest/TestHarness.h"

extern "C" {
#include "MessageFormatter.h"
#include "MessageValidator.h"
}

// -----------------------------------------------------------------------------

TEST_GROUP(Validating) {
};

TEST(Validating, ValidateFalse) {
  Message message;
  MessageFormatter_Pack("01234567890A", &message);
  CHECK_FALSE(MessageValidator_Check(&message));
}

TEST(Validating, ValidateTrue) {
  Message message;
  MessageFormatter_Pack("01234567890A", &message);
  MessageValidator_Sign(&message);
  CHECK(MessageValidator_Check(&message));
}