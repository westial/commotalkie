project (CommoTalkie)

# include project source
include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/message)
include_directories(${PROJECT_SOURCE_DIR}/../include/device)
include_directories(${PROJECT_SOURCE_DIR}/../include/application)

# include mocks only for test code
include_directories(${PROJECT_SOURCE_DIR}/../mock)

# include CppUTest headers
include_directories(SYSTEM $ENV{CPPUTEST_HOME})

# add cpputest library
add_library(imp_cpputest STATIC IMPORTED)
set_property(TARGET imp_cpputest PROPERTY IMPORTED_LOCATION /usr/local/lib/libCppUTest.a)

# build test library for Packing
add_library(PackingTest ./PackingTest.cpp)
add_library(ValidatingTest ./ValidatingTest.cpp)
add_library(EncryptingTest ./EncryptingTest.cpp)
add_library(PublishingTest ./PublishingTest.cpp)
add_library(SubscriptionTest ./SubscriptionTest.cpp)
add_library(PublishEncryptedMessageTest ./PublishEncryptedMessageTest.cpp)

add_executable(RunAllTests RunAllTests.cpp)
target_link_libraries(RunAllTests
        imp_cpputest
        PackingTest
        ValidatingTest
        EncryptingTest
        PublishingTest
        SubscriptionTest
        PublishEncryptedMessageTest
        PlainMessageFormatter
        SumMessageValidator
        XorMessageCrypter
        SenderBasedMessagePublisher
        ReceiverBasedMessageSubscriber
        SimpleSender
        SimpleReceiver
        PublishEncryptedMessage
)