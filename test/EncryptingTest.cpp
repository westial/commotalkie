#include "CppUTest/TestHarness.h"

extern "C" {
#include "MessageFormatter.h"
#include "MessageCrypter.h"
}

// -----------------------------------------------------------------------------

TEST_GROUP(Encrypting) {
};

TEST(Encrypting, EncryptNoSalt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Encrypt(&expected, "", encrypted);
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  MEMCMP_EQUAL(expected.body, result.body, 10);
}

TEST(Encrypting, EncryptShortSalt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Encrypt(&expected, "01234567890", encrypted);
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  CHECK_EQUAL(expected.body[9], result.body[9]);
  CHECK_FALSE(expected.body[8] == result.body[8]);
}

TEST(Encrypting, EncryptSameLenghtSalt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Encrypt(&expected, "01234567890A", encrypted);
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == ((unsigned char*)encrypted)[i]) {
      equalsCount++;
    }
  }
  CHECK_FALSE(equalsCount == MESSAGE_LENGTH);
}

TEST(Encrypting, EncryptLongerSalt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Encrypt(&expected, "01234567890AZSWD", encrypted);
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == ((unsigned char*)encrypted)[i]) {
      equalsCount++;
    }
  }
  CHECK_FALSE(equalsCount == MESSAGE_LENGTH);
}

TEST(Encrypting, Decrypt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Encrypt(&expected, "abcdefghijkl", encrypted);
  Message decrypted;
  MessageCrypter_Decrypt(encrypted, "abcdefghijkl", &decrypted);
  MEMCMP_EQUAL((const char*)expected.meta, (const char*)decrypted.meta, 2);
  MEMCMP_EQUAL((const char*)expected.body, (const char*)decrypted.body, 10);
}