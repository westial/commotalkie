set(HEADER_FILES
        Pull.h
        Publish.h
        Config.h
        PublisherBuilder.h
        )

add_library(application STATIC ${HEADER_FILES})
