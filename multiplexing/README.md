# Multiplexing

## select

[select man page](https://www.man7.org/linux/man-pages/man2/select.2.html)

> **_NOTE 1:_** Available: POSIX.1-2001, 4.4BSD (first appeared in 4.2BSD).

> **_NOTE 2:_** Fairly saying, this should never be used in time critical
applicaions.

`select()` allows a program to monitor multiple file descriptors,
waiting until one or more of the file descriptors become "ready"
for some class of I/O operation (e.g., input possible).  A file
descriptor is considered ready if it is possible to perform a
corresponding I/O operation (e.g., read(2), or a sufficiently
small write(2)) without blocking.

**Pros**:
1. The main advantage of select is the fact that it is very portable – every
unix like OS has it

**Cons**:
1. select() can monitor only file descriptors numbers that
are less than FD_SETSIZE (1024)—an unreasonably low limit for
many modern applications—and this limitation will not change.

2. Select is slow compared to poll/epoll. It will always iterate throug
all monitored FDs.

3. Upon return, each of the file descriptor sets is
modified in place to indicate which file descriptors are
currently "ready".  Thus, if using select() within a loop, the
sets must be reinitialized before each call.

## Poll

[poll man page](https://www.man7.org/linux/man-pages/man2/poll.2.html)

`poll()` waits for one of a set of file descriptors to become ready to
perform I/O.

`poll()` is still slow, like we did with select , we need to check each pollfd
object to see if its file descriptor is ready but we don’t need to build the
set each iteration
[source](https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/).

### Poll vs Select

[Source:](https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/)
- `poll()` does not require that the user calculate the value of the highest
numbered file descriptor +1
- `poll()` is more efficient for large-valued file descriptors.
Imagine watching a single file descriptor with the value 900 via
`select()` the kernel would have to check each bit of each passed-in set,
up to the 900th bit.
- `select( )`’s file descriptor sets are statically sized.
- With `select()`, the file descriptor sets are reconstructed on return,
so each subsequent call must reinitialize them. The `poll()` system call
separates the input (events field) from the output (revents field),
allowing the array to be reused without change.
- The timeout parameter to `select()` is undefined on return. Portable code
needs to reinitialize it. This is not an issue with `pselect()`
- `select()` is more portable, as some Unix systems do not support `poll()`


## EPoll

[epoll man page](https://www.man7.org/linux/man-pages/man7/epoll.7.html)

The epoll API performs monitoring multiple file descriptors to
see if I/O is possible on any of them.  The epoll API can be
used either as an edge-triggered or a level-triggered interface
and scales well to large numbers of watched file descriptors.


### epoll_create() vs epoll_create1()

Both functins create an EPoll descriptor.

```c
int epoll_create(int size);
```

Variable `size` is ignored, but for backwards compatability it is a part of
the function signature.

```c
int epoll_create1(int flags);
```

`flags`: `EPOLL_CLOEXEC` -> Set the close-on-exec (FD_CLOEXEC) flag on the
new file descriptor.

> **Note:** Note that the use of this flag is essential in some
multithreaded programs, because using a separate fcntl(2)
F_SETFD operation to set the FD_CLOEXEC flag does not
suffice to avoid race conditions where one thread opens a
file descriptor and attempts to set its close-on-exec flag
using fcntl(2) at the same time as another thread does a
fork(2) plus execve(2).  Depending on the order of
execution, the race may lead to the file descriptor
returned by open() being unintentionally leaked to the
program executed by the child process created by fork(2).
[Source](https://www.man7.org/linux/man-pages/man2/open.2.html)

### epoll_wait() vs epoll_pwait() vs epoll_pwait2()

The epoll_wait() system call waits for events on the
[epoll](https://www.man7.org/linux/man-pages/man7/epoll.7.html)
instance referred to by the file descriptor epfd.


A call to epoll_wait() will block until either:
- a file descriptor delivers an event;
- the call is interrupted by a signal handler; or
- the timeout expires.

`epoll_pwait()` allows an application to safely wait until either a file
descriptor becomes ready or until a signal is caught.

The `epoll_pwait2()` system call is equivalent to epoll_pwait() except for the
timeout argument.  It takes an argument of type timespec to be able to specify
nanosecond resolution timeout.

### Stop epoll

Sinse the `epoll_wait`/`epoll_pwait`/`epoll_pwait2` are blocking system calls
we need a reliable mechanism to exit the epoll.

1. `Timeout`: set a timeout - if no events happen, epoll will exit.
The problem is that timeout is hard to define on the system when events
happen irregular.
2. Use `signal handler` and epoll_pwait/epoll_pwait2: reliable way to exit epoll
3. Add additional `file destriptor` (let's name it command-and-control)
to epoll and write to descriptor. based on the information you write epoll
can provide a reasonable collection of handlers.

### Epoll vs Select/Poll

- We can add and remove file descriptor while waiting
- epoll_wait returns only the objects with ready file descriptors
- epoll has better performance – O(1) instead of O(n)
- epoll can behave as level triggered or edge triggered (see man page)
- epoll is Linux specific so non portable

## Useful links

- [Select vs Poll vs Epoll](https://devarea.com/linux-io-multiplexing-select-vs-poll-vs-epoll/)
- [Man page: select](https://www.man7.org/linux/man-pages/man2/select.2.html)
- [Man page: poll](https://www.man7.org/linux/man-pages/man2/poll.2.html)
- [Man page: epoll](https://www.man7.org/linux/man-pages/man7/epoll.7.html)
