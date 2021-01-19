set(HEADER_FILES
        Message.h
        MessageFormatter.h
        MessageValidator.h
        MessageCrypter.h
        MessagePublisher.h
        MessageSubscriber.h
        )

add_library(message STATIC ${HEADER_FILES})
