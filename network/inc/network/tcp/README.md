# IP/TCP

The Transmission Control Protocol (TCP) is intended for use as a highly
reliable host-to-host protocol between hosts in packet-switched computer
communication networks, and in interconnected systems of such networks.

Basic definition [rfc793](https://www.rfc-editor.org/rfc/rfc793).

Latest version of the protocol is defined in [rfc9293](https://www.ietf.org/rfc/rfc9293.html).

## Header example

```
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Data |       |C|E|U|A|P|R|S|F|                               |
| Offset| Rsrvd |W|C|R|C|S|S|Y|I|            Window             |
|       |       |R|E|G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum            |         Urgent Pointer        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           [Options]                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               :
:                             Data                              :
:                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      Note that one tick mark represents one bit position.
```

## Diagram

```mermaid
sequenceDiagram
    participant S as Server
    participant C as Client
    note over S: socket()
    note over S: bind()
    note over S: listen()
    note over C: socket()
    note over C: connect()
    C->>+S: Setup the connection
    note over S: accept()
    S-->>C: Connection established
    note over C: sendto()
    C->>+S: Send msg
    S-->>C: Msg delivery ack
    note over S: recvfrom()
    note over S: sendto()
    S->>+C: Send msg back
    note over C: recvfrom()
    note over C: close()
```

## Links

See links from [UDP](../udp/README.md)

- [About TCP](https://www.man7.org/linux/man-pages/man7/tcp.7.html)
- [listen](https://www.man7.org/linux/man-pages/man2/listen.2.html)
- [accept](https://www.man7.org/linux/man-pages/man2/accept.2.html)
