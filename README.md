# `TAU` - Thin Asynchronous/Attached UI

`TAU` is a client-server library for instrumenting your c++ program to work with a touchscreen devices, turning them into an attached UI for the program. Currently, android devices are supported (a special app should be installed on the device).

It can be used in the situations when we need to interact with a program that has a limited number of ways of communication with the user:
 - programs on a devices with no physical access to
 - monitoring of the status of the program execution on the phone or tablet
 - situations when it is preferable not to use keyboard or mouse for interaction with the program (e.g. keyboard/mouse emulation software)
 - programs on embedded or IoT devices, which may not have any UI to interact with, but can be accessed via network
 - addition of the secondary interfaces to the programs (as a plugin or an additional feature)
 
## Requirements
### C++ server project:
The `TAU` library can be used in both header-only and compiled modes.
The core of this library does not depend on any non-standard libraries. Some of the not-essential utility classes contain boost and C++/CLI dependencies.
The minimum set of requirements for using `tau` are:
 - C++03 compatible compiler
 - socket I/O support (ability to create sockets, listen on them, send and receive data)
 
[configuring your C++ project to work with the library](doc/installation.md)

### Client device
 - Android 3.0 or newer with [tau client](https://play.google.com/store/apps/details?id=com.tau.client) app installed.

## Reference, examples, code samples
[introduction](doc/introduction.md)
