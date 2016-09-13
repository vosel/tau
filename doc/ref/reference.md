# Library reference

The `tau` library classes are divided into 4 namespaces:
 - `tau::communications_handling`
 - `tau::layout_generation`
 - `tau::util`
 - `tau::common`

##tau::communications_handling
This namespace contains the classes that are used to handle the information sent and received between client and server. They parse the packets from client, generate and send the packets to it. This part of the library is also responsible for triggering the callbacks into the user code. For each packet received from client, there is a callback mechanism, which allows the user handler to be executed for this packet. 
[tau::communications_handling reference](communications_management.md)
##tau::layout_generation
This namespace contains classes used for generation of the json objects for layout presentation. These json objects are sent to the client, which uses them to draw the UI for the user.

##tau::util
The util namespace contains all the helper classes that can be useful, but not essential for the library user. There are classes that implement socket communication logic for different platforms (currently Boost::asio and C++/CLI implementations are available), also it has some classes built on top of the tau::communications_handling classes to facilitate the library usage (getting rid of some of the boilerplate code).

##tau::common
This namespace contains classes, which are used across the whole library. The purpose of this namespace is to remove dependencies between `communications_handling` and `layout_generation`.