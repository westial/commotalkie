#include "CppUTest/TestHarness.h"

#include "MessageFormatter.h"
#include "MessageCrypter.h"


// -----------------------------------------------------------------------------

TEST_GROUP(Encrypting) {
};

TEST(Encrypting, EncryptNoSalt) {
  Message expected;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  MEMCMP_EQUAL(expected.meta, result.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(expected.body, result.body, MESSAGE_BODY_LENGTH);
}

TEST(Encrypting, EncryptFirstBodyByte) {
  Message expected;
  MessageFormatter_Pack((unsigned char *)"01834567890A", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("X");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  CHECK_FALSE(expected.body[0] == result.body[0]);
}

TEST(Encrypting, EncryptShortSalt) {
  Message expected;
  MessageFormatter_Pack((unsigned char *)"01834567890A", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("12345678");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  Message result;
  MessageFormatter_Pack(encrypted, &result);
  CHECK_EQUAL(expected.body[MESSAGE_BODY_LENGTH -1], result.body[MESSAGE_BODY_LENGTH -1]);
  CHECK_FALSE(expected.body[MESSAGE_BODY_LENGTH -2] == result.body[MESSAGE_BODY_LENGTH -2]);
}

TEST(Encrypting, EncryptSameLenghtSalt) {
  Message expected;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("0123456789");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == encrypted[i]) {
      equalsCount++;
    }
  }
  CHECK_FALSE(equalsCount == MESSAGE_LENGTH);
}

TEST(Encrypting, EncryptLongerSalt) {
  unsigned int leastEncrypted = 4;
  Message expected;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("0123456789ABZSWD");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == encrypted[i]) {
      equalsCount++;
    }
  }
  CHECK_FALSE(equalsCount + leastEncrypted == MESSAGE_LENGTH);
}

TEST(Encrypting, MetaIsNotEncrypted) {
  Message expected;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("0123456789ABZSWD");
  MessageCrypter_Encrypt(&expected, encrypted);
  MessageCrypter_Destroy();
  MEMCMP_EQUAL(expected.meta, encrypted, MESSAGE_META_LENGTH);
}

TEST(Encrypting, DecryptMatch) {
  Message expected;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
  MessageCrypter_Create("abcdefghijkl");
  MessageCrypter_Encrypt(&expected, encrypted);
  Message decrypted;
  MessageCrypter_Decrypt(encrypted, &decrypted);
  MessageCrypter_Destroy();
  MEMCMP_EQUAL(expected.meta, decrypted.meta, MESSAGE_META_LENGTH);
  MEMCMP_EQUAL(expected.body, decrypted.body, MESSAGE_BODY_LENGTH);
  int equalsCount = 0;
  for (unsigned int i = 0; i < MESSAGE_LENGTH; i++) {
    if (((unsigned char*)&expected)[i] == ((unsigned char*)&decrypted)[i]) {
      equalsCount++;
    }
  }
  CHECK(equalsCount == MESSAGE_LENGTH);
}

TEST(Encrypting, DecryptUnmatch) {
  unsigned int leastEncrypted = 4;
  Message expected;
  MessageFormatter_Pack((unsigned char *)"0123456789AB", &expected);
  unsigned char encrypted[MESSAGE_LENGTH];
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
  CHECK_FALSE(equalsCount + leastEncrypted == MESSAGE_LENGTH);
}