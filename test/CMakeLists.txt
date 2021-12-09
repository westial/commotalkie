project (CommoTalkie)

# include project source
include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/application)
include_directories(${PROJECT_SOURCE_DIR}/../include/message)
include_directories(${PROJECT_SOURCE_DIR}/../include/device)
include_directories(${PROJECT_SOURCE_DIR}/../include/driver)
include_directories(${PROJECT_SOURCE_DIR}/../include/sleepable)

# include mocks only for test code
include_directories(${PROJECT_SOURCE_DIR}/../mock)

# include CppUTest headers
include_directories(SYSTEM $ENV{CPPUTEST_HOME})

# add cpputest library
add_library(imp_cpputest STATIC IMPORTED)
set_property(TARGET imp_cpputest PROPERTY IMPORTED_LOCATION $ENV{CPPUTEST_PARENT_DIR}/libCppUTest.a)

# build test library for Packing
add_library(PackingTest ./PackingTest.cpp)
add_library(ValidatingTest ./ValidatingTest.cpp)
add_library(EncryptingTest ./EncryptingTest.cpp)
add_library(PublishingTest ./PublishingTest.cpp)
add_library(SubscriptionTest ./SubscriptionTest.cpp)
add_library(PublishEncryptedMessageTest PublishAppTest.cpp)
add_library(ReadEncryptedMessageTest PullAppTest.cpp)
add_library(PublisherBuilderTest PublisherBuilderTest.cpp)
add_library(SubscriberBuilderTest SubscriberBuilderTest.cpp)
add_library(DriverConfiguringTest DriverConfiguringTest.cpp)
add_library(DriverSendingTest DriverSendingTest.cpp)
add_library(DriverReceivingTest DriverReceivingTest.cpp)
add_library(TimerTest TimerTest.cpp)
add_library(SleepableTest SleepableTest.cpp)

# helper
add_library(DriverTestHelper helper/DriverTestHelper.cpp)
add_library(TimerTestHelper helper/TimerTestHelper.cpp)
add_library(PublishTestHelper helper/PublishTestHelper.cpp)
add_library(PullTestHelper helper/PullTestHelper.cpp)

# add mocking library
add_library(Spy STATIC Spy.h)
add_library(PublishPullShared STATIC helper/PublishPullShared.h)
set_target_properties(Spy PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PublishPullShared PROPERTIES LINKER_LANGUAGE CXX)

add_executable(RunAllTests RunAllTests.cpp)
target_link_libraries(RunAllTests
        Spy
        PublishPullShared
        DriverTestHelper
        TimerTestHelper
        PublishTestHelper
        PullTestHelper
        imp_cpputest
        PackingTest
        ValidatingTest
        EncryptingTest
        PublishingTest
        SubscriptionTest
        SleepableTest
        PublishEncryptedMessageTest
        ReadEncryptedMessageTest
        PublisherBuilderTest
        SubscriberBuilderTest
        DriverConfiguringTest
        DriverSendingTest
        DriverReceivingTest
        TimerTest
        PlainMessageFormatter
        SumMessageValidator
        XorMessageCrypter
        SenderBasedMessagePublisher
        ReceiverBasedMessageSubscriber
        SimpleSender
        SimpleReceiver
        SimpleTimer
        PublishEncryptedMessage
        ReadEncryptedMessage
        PublisherBuilder
        SubscriberBuilder
        EByteDriver
        DriverSleepable
        StubSleepable
)
