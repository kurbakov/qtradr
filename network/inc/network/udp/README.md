# IP/UDP

## Links

- [About UDP](https://www.man7.org/linux/man-pages/man7/udp.7.html)
- [socket()](https://www.man7.org/linux/man-pages/man2/socket.2.html)
- [bind()](https://www.man7.org/linux/man-pages/man2/bind.2.html)
- [connect()](https://www.man7.org/linux/man-pages/man2/connect.2.html)
- [sendto()](https://www.man7.org/linux/man-pages/man2/sendto.2.html)
- [recvfrom()](https://www.man7.org/linux/man-pages/man2/recvfrom.2.html)
- [close()](https://www.man7.org/linux/man-pages/man2/close.2.html)

## Diagram

```mermaid
sequenceDiagram
    participant S as Server
    participant C as Client
    note over S: socket()
    note over S: bind()
    note over C: socket()
    note over C: connect()
    C->>+S: Setup the connection
    S-->>C: Connection established
    note over C: sendto()
    C->>+S: Send msg
    note over S: recvfrom()
    note over S: sendto()
    S-->>C: Send msg back
    note over C: recvfrom()
    note over C: close()
```
