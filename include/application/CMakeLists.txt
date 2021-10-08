set(HEADER_FILES
        Pull.h
        Publish.h
        PublisherBuilder.h
        SubscriberBuilder.h
        )

add_library(application STATIC ${HEADER_FILES})
