#include "CppUTest/TestHarness.h"

#include "MessageFormatter.h"
#include "MessageValidator.h"


// -----------------------------------------------------------------------------

TEST_GROUP(Validating) {
};

TEST(Validating, ValidateFalse) {
  Message message;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &message);
  CHECK_FALSE(MessageValidator_Check(&message));
}

TEST(Validating, ValidateTrue) {
  Message message;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &message);
  MessageValidator_Sign(&message);
  CHECK(MessageValidator_Check(&message));
}