# Multiplexing

## select

[select man page](https://www.man7.org/linux/man-pages/man2/select.2.html)

> **_NOTE 1:_** Available: POSIX.1-2001, 4.4BSD (first appeared in 4.2BSD).

> **_NOTE 2:_** Fairly saying, this should never be used in time critical applicaions.

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

## EPoll

[epoll man page](https://www.man7.org/linux/man-pages/man7/epoll.7.html)

The epoll API performs monitoring multiple file descriptors to
see if I/O is possible on any of them.  The epoll API can be
used either as an edge-triggered or a level-triggered interface
and scales well to large numbers of watched file descriptors.


### epoll_create() vs epoll_create1()

### epoll_wait() vs epoll_pwait() vs epoll_pwait2()

### Stop epoll

## Select vs. Poll vs. EPoll

## Useful links

