set(HEADER_FILES
        Sender.h
        Receiver.h
        Timer.h
        )

add_library(device STATIC ${HEADER_FILES})
