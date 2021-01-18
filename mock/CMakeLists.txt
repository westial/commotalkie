project (CommoTalkie)

include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/device)
include_directories(${PROJECT_SOURCE_DIR}/../include/message)

add_library(StubSender StubSender.c)
add_library(MockMessageSender MockMessageSender.c)