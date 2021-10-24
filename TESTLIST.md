Test list
=========

The following test scenarios are done when the line begins with a plus sign (+) 
and pending when it begins with a minus sign (-).

## Application ##

+ Pack / Unpack a message.
+ Sign / Validate a message.
+ Encrypt / Decrypt a message.
+ Publish a message.
+ Receive a message.

+ Publish an encrypted message.
+ Pull and read an encrypted message.

+ Encrypt first byte of the body.
+ Throw a listening Timeout error.

+ Permanently listen for pulling as an option.
+ Ignore an invalid message.
+ Ignore a message for a different ID as an option.

+ Create a publisher.
+ Publish to two topics.
+ Create a subscriber.

## Infrastructure ##

### EByte ###

+ Create a Driver with the appropriate speed.
+ Create a Driver with the appropriate options.
+ Wait for Aux getting high.
+ Send a string.
+ Wait an absolute interval after Aux got high.
+ Receive nothing.
+ Change to normal state before receiving.
+ Change to sleep state after receiving.
+ Receive a string.
+ Expire receiving timeout.
+ Auxiliary wait and receiving window have a different timeout.
+ Listen callback returns a boolean value.
+ passing argument 1 of 'read_from_serial_callback' discards 'const' qualifier from pointer target type
+ Subscriber turns on the receiver when it starts pulling in.
+ Subscriber turns off the receiver when the input is read.
+ Receiver is turned off by default.
+ Driver does not cut receiving while aux input is still low.
+ Driver gets out from receiving when aux is permanently low.
+ Do not start a Timer if it is already running.
- Do not change the state if the new state is the current one.