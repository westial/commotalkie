set(HEADER_FILES
        Message.h
        MessageFormatter.h
        MessageValidator.h
        )

add_library(message STATIC ${HEADER_FILES})
