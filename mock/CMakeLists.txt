project (CommoTalkie)
include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/sleepable)

add_library(StubSleepable
        sleepable/StubSleepable.c
        )