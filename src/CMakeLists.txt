project (CommoTalkie)

include_directories(${PROJECT_SOURCE_DIR}/../include)
include_directories(${PROJECT_SOURCE_DIR}/../include/message)

# Add here all libraries in src
# And link them in the runnable CMakeLists file
add_library(PlainMessageFormatter message/PlainMessageFormatter.c)
add_library(SumMessageValidator message/SumMessageValidator.c)

#add_executable(Tutorial ./Tutorial/tutorial.cxx)
#add_library(LedDriver ./LedDriver/LedDriver.c)