CommoTalkie
===========

Communication interface for low power consumption devices.

"Keep it tiny" is one of the most important requirements of this project.

## Message ##

This SDK pipelines validate, encrypt and transport the following Message
structure.

![message structure](doc/messageschema.png)

MESSAGE_LENGTH value is a configuration parameter of "messageconfig.h" and is
the total length of the message, META and BODY together. A length change must 
consider the pragma alignment configuration of 4 set in 
[./include/message/Message.h](./include/message/Message.h)

### Signature ###

The most significant byte is the sign to validate the next message bytes. This 
byte is automatically set during the publication of a message.

### Port ###

The second most significant byte is for feature segregation in the receiver.

### Id ###

The third most significant byte purpose is an Id.

### Content ###

The information of the message.

## SDK ##

Publisher and subscriber clients.

See [./include/application](./include/application) for the concrete interface
and [./test](./test) for more implementation details.

### Publish ###

This publisher sends a message towards a "topic". A topic is an address, event 
name, just a message destination.

```c
// My Device Id
const unsigned char my_id = 0x06;
unsigned char body[MESSAGE_BODY_LENGTH];

// Initialize publisher
Publish_Create("salt", (const void *) push_fn);

// A port for the commotalkie context backend is as an endpoint for an HTTP API
const unsigned char port_for_temperature = 0x05;

// Send a message
Publish_Invoke((const unsigned char *)"destination::address", port_for_temperature, my_id, "a message");

// Send other message to a different port
Publish_Invoke((const unsigned char *)"destination::address", 0xAD, my_id, "another message");

// Send other message to another destination
Publish_Invoke((const unsigned char *)"another::destination", 0xAD, my_id, "and another message");

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
unsigned long push_fn(const unsigned char* address, const unsigned char* content, unsigned long size);
```

### Pull ###

This subscriber listens for a message in its "topic" only, until a valid message 
is received or timeout expires. Timeout parameter as 0 disables the expiration.

```c
// Initialize the content destination variables
const unsigned long timeout_milliseconds = 10 * 1000;
const unsigned char to_id = 0xAA;

// Initialize pulling messages to ID 0xAA only
Pull_Create(
    "salt",
    (void *) listen_fn,
    (void *) now_fn,
    timeout_milliseconds,
    &to_id
);

// Start listening for messages sent to "destination::address" only
unsigned char body[MESSAGE_BODY_LENGTH];
unsigned char port, id;
result = Pull_Invoke((const unsigned char *)"destination::address", &port, &id, body);

// Do some stuff with the message contents
something(body);

// Destroy after first message
Pull_Destroy();
```

The option `to_id` validates the messages sent to the given ID only. If this
parameter value is 0x00, the pulling does validate a message sent to any ID.
Initialization example for wildcard ID:

```c
Pull_Create(
    "salt",
    (const void *) listen_fn,
    (const void *) now_fn,
    (const void *) turn_on_fn,
    (const void *) turn_off_fn,
    timeout_at,
    0x00
);
```

There is a salt value in the first argument.

The `timeout_at` argument value is the listening expiration time. The unit for
this value depends on the value returned by the time service `now_fn`. If the 
`timeout_at` value is 0, there is no timeout and the device is listening
permanently.

The last argument is an ID to make the listener selective. If a received message
is not forwarded to this ID, the message is discarded.

The remaining arguments are the following function:

* Listen for a message whatever port or device the implementation uses.
* A time service providing a time in a number type value.
* A command function to turn on the listener.
* A command function to turn off the listener.

The listening function listens for a message input from whatever communication 
device is provided in the integration, fills up the `buffer` with and 
returns one of the following integer values:

* 1 on success.
* 0 when it did receive nothing.
* -1 on error.

The listening function has the following signature. The `size` argument
is exactly the value of MESSAGE_LENGTH. If the transporter driver prepends or 
appends bytes to the message for technical requirements, the `listen_fn` fills
up the `buffer` argument with the content without the transporter additions.

```c
int listen_fn(const unsigned char* address, unsigned char* buffer, const unsigned long size);
```

The time service function signature is pretty simple, it's just a call to a 
clock to get an increasing number of milliseconds:

```c
unsigned long now_fn();
```

The signature for the `turn_on_fn` is as follows:

```c
void turn_on_fn();
```

The signature for the `turn_off_fn` is as follows:

```c
void turn_off_fn();
```

The listener is turned off by default and turned on for listening during the 
window limited by the timeout only.

### Builders make it easier ###

A builder for Pull and Publish instances makes their implementation easier and
safer. Using the following examples to create the instances is highly 
recommended.

More examples here:
* Tests for Publisher Builder [./test/PublisherBuilderTest.cpp](./test/PublisherBuilderTest.cpp)
* Tests for Pull Builder [./test/SubscriberBuilderTest.cpp](./test/SubscriberBuilderTest.cpp)

#### Publish Builder ####

The result of `PublisherBuilder_Build()` is 1 or 0 for success or error
respectively. Salt and sending callback are required.

Create an instance of Publish and use it as follows:
```c
PublisherBuilder_Create();
PublisherBuilder_SetSalt(salt);
PublisherBuilder_SetSendCallback(push_fn);
if (!PublisherBuilder_Build()) exit(-1);
PublisherBuilder_Destroy();

Publish_Invoke((const unsigned char*)"destination::address:1", 0x05, 0x06, body);
Publish_Invoke((const unsigned char*)"destination::address:2", 0x06, 0x06, body);
Publish_Invoke((const unsigned char*)"destination::address:3", 0x07, 0x06, body);

// Destroy if you need it no more.
Publish_Destroy();
```

#### Pull Builder ####

The result of `SubscriberBuilder_Build()` is 1 or 0 for success or error 
respectively. Salt and listening callback are required.

Create an instance of Pull and use it as follows:
```c
SubscriberBuilder_Create();
SubscriberBuilder_SetSalt("salt");
SubscriberBuilder_SetListenCallback(listen_fn);
SubscriberBuilder_SetReceiverStateCallback(turn_on_fn, turn_off_fn);
SubscriberBuilder_SetId(&id);
if (!SubscriberBuilder_Build()) exit(-1);
SubscriberBuilder_Destroy();

Pull_Invoke((const unsigned char*)"destination::address", &port, &id, body);
Pull_Invoke((const unsigned char*)"destination::address", &port, &id, body);
Pull_Invoke((const unsigned char*)"destination::address", &port, &id, body);

// Destroy if you need it no more.
Pull_Destroy();
```

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

To install the library move the [./arduino/prebuild/CommoTalkie](./arduino/prebuild/CommoTalkie)
to your OS Arduino Library 
[./arduino/prebuild/CommoTalkie/messageconfig.h](./arduino/prebuild/CommoTalkie/messageconfig.h) 
to the project you want to use this library in.

### "CommoTalkino" Full integration example ###

[https://github.com/westial/commotalkino](https://github.com/westial/commotalkino)
is an example of integration of this library. The project consists on a communication
between 2 Arduino MCU permanently sending and receiving a number that is getting
incremented every transmission. Something like a stress test to check the 
reliability of CommoTalkie library.

## License ##

GNU General Public License (GPLv3). Read the attached [license file](LICENSE.txt).

## Author ##

Jaume Mila Bea <jaume@westial.com>
