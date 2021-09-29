Test list
=========

The following test scenarios are done when the line begins with a plus sign (+) 
and pending when it begins with a minus sign (-).

## Single thread only ##

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
- Create a subscriber.

