set(HEADER_FILES
        ReadEncryptedMessage.h
        PublishEncryptedMessage.h
        ApplicationConfig.h
        )

add_library(application STATIC ${HEADER_FILES})
