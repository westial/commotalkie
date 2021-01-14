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
  MessageCrypter_Create("");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  MEMCMP_EQUAL(expected.body, result.body, MESSAGE_BODY_LENGTH);
}

TEST(Encrypting, EncryptShortSalt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("01234567890");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  CHECK_EQUAL(expected.body[MESSAGE_BODY_LENGTH -1], result.body[MESSAGE_BODY_LENGTH -1]);
  CHECK_FALSE(expected.body[MESSAGE_BODY_LENGTH -2] == result.body[MESSAGE_BODY_LENGTH -2]);
}

TEST(Encrypting, EncryptSameLenghtSalt) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("01234567890A");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
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
  MessageCrypter_Create("01234567890AZSWD");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == ((unsigned char*)encrypted)[i]) {
      equalsCount++;
    }
  }
  CHECK_FALSE(equalsCount == MESSAGE_LENGTH);
}

TEST(Encrypting, DecryptMatch) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("abcdefghijkl");
  MessageCrypter_Encrypt(&expected, encrypted);
  Message decrypted;
  MessageCrypter_Decrypt(encrypted, &decrypted);
  MessageCrypter_Destroy();
  MEMCMP_EQUAL((const char*)expected.meta, (const char*)decrypted.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL((const char*)expected.body, (const char*)decrypted.body, MESSAGE_BODY_LENGTH);
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == ((unsigned char*)&decrypted)[i]) {
      equalsCount++;
    }
  }
  CHECK(equalsCount == MESSAGE_LENGTH);
}

TEST(Encrypting, DecryptUnmatch) {
  Message expected;
  MessageFormatter_Pack("01234567890A", &expected);
  char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("abcdefghijkl");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  Message decrypted;
  MessageCrypter_Create("9999999999");
  MessageCrypter_Decrypt(encrypted, &decrypted);
  MessageCrypter_Destroy();
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == ((unsigned char*)&decrypted)[i]) {
      equalsCount++;
    }
  }
  CHECK_FALSE(equalsCount == MESSAGE_LENGTH);
}