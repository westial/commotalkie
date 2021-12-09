#include "Sleepable.h"
#include "CppUTest/TestHarness.h"
#include "DriverSleepable.h"
#include "helper/DriverTestHelper.cpp"

// -----------------------------------------------------------------------------

Driver sample_driver;

TEST_GROUP(Sleepable){void setup() override{driverHelperSetup();
sample_driver =
    create_sample((const unsigned char *)"\xA1\xA2\xA3", 0, 0, 0);
}
}
;

TEST(Sleepable, CreateADriverSleepable) {
  Sleepable sleepable_driver = DriverSleepable_Create(
      50,
      Driver_TurnOff,
      Driver_TurnOn,
      &sample_driver
      );
  CHECK_EQUAL(0, sleepable_driver->is_sleeping);
  Sleepable_Destroy(sleepable_driver);
}

TEST(Sleepable, TurnOffADriverSleepable) {
  Sleepable sleepable_driver = DriverSleepable_Create(
      50,
      Driver_TurnOff,
      Driver_TurnOn,
      &sample_driver
      );
  Sleepable_TurnOff(sleepable_driver);
  CHECK_EQUAL(1, sleepable_driver->is_sleeping);
  CHECK_EQUAL(spy_write_pin_args[2][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[2][1], ON);
  CHECK_EQUAL(spy_write_pin_args[3][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[3][1], ON);
  Sleepable_Destroy(sleepable_driver);
}

TEST(Sleepable, TurnOnADriverSleepable) {
  Sleepable sleepable_driver = DriverSleepable_Create(
      50,
      Driver_TurnOff,
      Driver_TurnOn,
      &sample_driver
      );
  Sleepable_TurnOn(sleepable_driver);
  CHECK_EQUAL(spy_write_pin_args[2][0], sample_driver.pins.m0);
  CHECK_EQUAL(spy_write_pin_args[2][1], OFF);
  CHECK_EQUAL(spy_write_pin_args[3][0], sample_driver.pins.m1);
  CHECK_EQUAL(spy_write_pin_args[3][1], OFF);
  CHECK_EQUAL(0, sleepable_driver->is_sleeping);
  Sleepable_Destroy(sleepable_driver);
}