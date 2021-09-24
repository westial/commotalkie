CommoTalkie
===========

Communication interface for low power consumption devices.

"Keep it tiny" is one of the most important requirements of this project.

## SDK ##

Two sides of a communication, publisher and subscriber. 

See [./include/application](./include/application) for the concrete interface
and the [./test](./test) scenarios for implementation details.

### Publish ###

This client sends a message towards a "topic". A topic is an address, event name, 
just a message destination.

```c
// My Device Id
const unsigned char my_id = 0x06;
unsigned char body[MESSAGE_BODY_LENGTH];

// Initialize publisher for one topic only
Publish_Create("salt", "destination::address", (const void *) push_fn);

// A port for the commotalkie context backend is as an endpoint for an HTTP API
const unsigned char port_for_temperature = 0x05;

// Send a message
Publish_Invoke(port_for_temperature, my_id, (const unsigned char*)"a message");

// Send other message to different port
Publish_Invoke(0xAD, my_id, (const unsigned char*)"other message");

// Destroy
Publish_Destroy();
```

The `push_fn` function is a function with an implementation of the message 
submission using the kind of communication as your choice: LoRa, Wi-Fi, 
Bluetooth,...

Following that's the signature of the submission function. The maximum allowed 
size of the content is always the value of the constant `MESSAGE_LENGTH`. 
Address is just the place for the mentioned message destination (topic).

```c
push_fn(const char* address, const char* content, unsigned long size);
```

### Pull ###

This client listens for a message in its "topic" only, until a valid message is
received or timeout expires.

```c
// Initialize the content destination variables
const unsigned long timeout_milliseconds = 10 * 1000;

// Initialize the pulling service
Pull_Create(
    "salt",
    "destination::address",
    (void *) listen_fn,
    (void *) now_fn,
    timeout_milliseconds
);

// Start listening for a message
unsigned char body[MESSAGE_BODY_LENGTH];
unsigned char port, id;
result = Pull_Invoke(&port, &id, body);

// Do some stuff with the message contents
something(body);

// Destroy after first message
Pull_Destroy();
```

The dependencies for the pulling service are two functions, the function to
listen for a message and the time service.

The listening function listens for a message input from whatever communication 
device is provided in the integration and fills up the input content and 
returns one of the following integer values:

* The size of the received content on success.
* 0 when it did receive nothing.
* -1 on error.

The listening function has the following signature:

```c
int stub_message_fn(const char* address, const char* content, const int size);
```

The time service function signature is pretty simple, it's just a call to a 
clock to get an increasing number of milliseconds:

```c
unsigned long now_fn();
```

## Message ##

The message pipelines validate, encrypt and transport the information as well.

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

### Content ###

The information of the message.

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

## License ##

GNU General Public License (GPLv3). Read the attached [license file](LICENSE.txt).

## Author ##

Jaume Mila Bea <jaume@westial.com>