
# Communications management between client and server
## Terminology
#### Server
A program that uses the `tau` library code. It creates a socket and listens on it waiting for the clients to connect. When a client connects, server uses the client's screen to display the UI and interact with the user.
#### Client
A phone or tablet device with the 'tau client' app installed. This app is used to connect to the server and communicate with it via TCP/IP socket.
#### Packet
A smallest complete logical block of information sent between client and server. Packets are sent through the socket as a stream of bytes and parsed on the opposite side. Until a packet is not received entirely, nothing is done on the receiving side. Packets have different types - for different purposes:
 - server request packets
 - client responses to the server requests
 - status updates from client to the server
 - events notifications from client to the server
 - automatic notifications to the server (values of inputs, layout screen changes, e.t.c)



## `tau::communications_handling` introduction
### Overview
The communication between client and server is done through a TCP/IP socket connection. Each command or data is sent as a single unit - packet. The generation of outgoing and parsing of incoming packets is handled by the library. All the code, dedicated to it is located in `tau::communications_handling` namespace.

The main means of communications are done through 3 classes:
 - `IncomingPacketsHandler`
 - `OutgiongPacketsGenerator`
 - `CommunicationIssuesHandler`
These classes make up the interface, through which the user's C++ code on server talks to the client.

Objects of these three classes are owned by a `AbstractEventsDispatcher` class, that contains all the user code.

### Initial handshake protocol at the moment client connects to the server
 - 1) initial state: server is listening on  the socket, waiting for the incoming connection. Client is not connected.
 - 2) client connects to the socket, connection is accepted. At this moment the `AbstractEventsDispatcher::onClientConnected()` callback will be executed.
 - 3) first packet is sent from client to server. It contains the information about the screen resolution of the device, client software version, device id, e.t.c.
 - 4) after that, server sends the packet with the layout information (json format) to the client. The json data is parsed and layout is displayed on client. If the json is malformed, the client sends an error packet to the server and continues waiting for the packet with correct layout json.
 - 5) at this point the normal operations state is reached. From now on, the packets can be sent both ways: server to client and client to server. Server sends commands for reading and setting data in the UI, layout changes. Client sends notifications about user actions or responses to the server requests.

### Setting up your server
In order to get your server code to communicate with the client you will need to set up several things:
 - the ability to build requests to the client in a natural way to your program.
 - the ability to react to the requests from the client (they might be status updates, responses to the server requests, event notifications, e.t.c)
 - the machinery in your code that physically sends and receives data through the socket, handles communication errors.

There are classes dedicated for these tasks in the `tau::communications_handling` namespace of the library. Also, there are some helpful classes in `tau::util` namespace, which make it easier to write your server. They implement some of the common patterns of building the server class.

#### AbstractEventsDispatcher class

All the user code that works directly with the client should be put in a class that is inherited from `tau::communications_handling::AbstractEventsDispatcher` class.
The `AbstractEventsDispatcher` class provides access to 3 objects, which handle the data flow from the user's point of view:
- `IncomingPacketsHandler`
- `tau::communications_handling::CommunicationIssuesHandler`
- `tau::communications_handling::OutgiongPacketsGenerator`

The first two classes (`IncomingPacketsHandler` and `CommunicationIssuesHandler`) are created inside the `AbstractEventsDispatcher` object (usually, during construction). Their purpose is to provide the callback functions, which are executed when a specific packet arrives from the client, or if there is an issue with the socket I/O.

The actual parsing of the byte stream coming from the socket is done in the `IncomingDataStreamParser` class. It makes sense of the byte stream - figures out, where the packets begin and end, extracts all the fields of the received packets. After that it calls the appropriate callback in `IncomingPacketsHandler`, which contains the user's code for handling the given packet.

The `OutgiongPacketsGenerator` class is passed to the `AbstractEventsDispatcher`'s constructor. This class's purpose is to provide a way for the user code inside `AbstractEventsDispatcher` to send packets to the client. This is the point through witch the outgoing data stream is directed. Somewhere inside the `OutgiongPacketsGenerator` there is actual code, which sends a buffer of data through a socket, so the implementation of the `OutgiongPacketsGenerator` may depend on the platform. There are several implementations of the `OutgiongPacketsGenerator` for different platforms in the `tau::util` namespace.

The last thing to address in order to get the server to work is the actual sending and receiving data via sockets. The `tau::communications_handling` part of the library is totally agnostic to the way socket I/O is done. So, it can be implemented by the user in any way he or she seems fit. The 2 places where the library interfaces with the socket I/O code are inside `OutgiongPacketsGenerator` and `IncomingDataStreamParser`, where the data is represented by a buffer (either received from the socket, or about to be sent to the socket). So, the I/O code should work through these 2 points.

Also, there are currently 2 available implementations of the socket I/O logic in the `tau::util` namespace. One of them is using boost::asio to work with sockets, the other uses C++/CLI async I/O machinery (Microsoft's .NET platform).

This concludes the introduction into `tau::communications_handling` part of the library.
