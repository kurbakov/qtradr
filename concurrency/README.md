# Concurrencly

## Thread safety

### Mutex
[mutex](https://en.cppreference.com/w/cpp/thread/mutex)

The mutex class is a synchronization primitive that can be used to protect shared data from being simultaneously accessed by multiple threads. 

std::mutex will execute a system call `pthread_mutex_lock` that is a part of [pthred](https://www.man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)

Inctruction genereted by GCC:

![Mutex instruction example](../.img/mutex.png)

### LockGuard

[]()

## Loock free structures


## Wait free structures


## Atomic and memory models


## Ring buffer approach


## Compare and swap
