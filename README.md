CommoTalkie
===========

This project is the abstraction of use cases for sending and receiving 
lightweight encrypted messages. The main purpose of these features is a safe
communication link between low power resource devices.

"Keep it tiny" is one of the most important requirements of this project.

## Message ##

The message structure is the most important point of this project. This structure
allows the use cases to validate, encrypt and transport information.

The structure of the message is as follows:

![message structure](doc/messageschema.png)

MESSAGE_LENGTH value is a configuration parameter of "messageconfig.h" and is
the total length of the message, META and BODY together.

### Signature ###

The most significant byte is the sign to validate the next message bytes. This 
byte is automatically set during the publication of a message.

### Port ###

The second most significant byte is for feature segregation in the receiver.

### Id ###

The third most significant byte purpose is an Id.

## Use cases ##

The use cases of this project are located at [./include/application](./include/application)
They are the ones that have to be created and invoked to get work this library.

## Testing ##

Testing directory [./test](./test) is the best documentation stuff of this 
project. There are test scenarios for all components separately and together 
injected into a use case.

It is created from TDD based on the book "Test-Driven Development for Embedded C"
written by the great [James W. Grenning](https://wingman-sw.com/).

The tests are handled by the library [CppUTest](https://cpputest.github.io/),
also following the guidelines from the book mentioned above and the documentation
in the official page of the library.

## Arduino ##

There is an Arduino library based implementation in [./arduino](./arduino).

To build the files for this Arduino library, go to this directory and execute
`make`. It creates a subdirectory in [./arduino/prebuild](./arduino/prebuild)
with the directory of the library itself [./arduino/prebuild/CommoTalkie](./arduino/prebuild/CommoTalkie)
and a configurable parameters file [./arduino/prebuild/messageconfig.h](./arduino/prebuild/messageconfig.h)

To install the library move the [./arduino/prebuild/CommoTalkie](./arduino/prebuild/CommoTalkie)
to your OS Arduino Library and do not forget to move the configuration file
[./arduino/prebuild/messageconfig.h](./arduino/prebuild/messageconfig.h) to every 
project you want to use this library. This configuration file can either be left
into the CommoTalkie Arduino Library directory as well, and then, the same
configuration is taken for every project using this library.