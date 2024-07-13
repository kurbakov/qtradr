# Timer

## Clock

3 approaches available:

1. time()
2. clock_gettime(CLOCK_TYPE, ..)
3. gettimeofday()
4. std::chrono::<CLOCK_TYPE>::now()

### time()

Source: [man time()](https://www.man7.org/linux/man-pages/man2/time.2.html)

The fastest way to get time in seconds resolution.

time() returns the time as the number of seconds since the Epoch,
`1970-01-01 00:00:00 +0000 (UTC)`.

### clock_gettime()

Source: [man clock_gettime()](https://www.man7.org/linux/man-pages/man3/clock_gettime.3.html)

The function clock_getres() finds the resolution (precision) of the
specified clock clockid, and, if res is non-NULL, stores it in the
struct timespec pointed to by res.  The resolution of clocks depends
on the implementation and cannot be configured by a particular
process.  If the time value pointed to by the argument tp of
clock_settime() is not a multiple of res, then it is truncated to a
multiple of res.

Used to monitor NANOSECOND resolution.

### gettimeofday()

Source [man gettimeofday()](https://www.man7.org/linux/man-pages/man2/gettimeofday.2.html)

The functions gettimeofday() and settimeofday() can get and set
the time as well as a timezone.

The tv argument is a struct timeval (as specified in
<sys/time.h>):

       struct timeval {
           time_t      tv_sec;     /* seconds */
           suseconds_t tv_usec;    /* microseconds */
       };

Used to monitor MICROSECOND resolution.


## std::chrono

Very powerful library with nanosecond resolution.
But seems like it is designed to work in all cases and optimised for usage and not speed.
In bechmark tests chrono showed 50% longer runtime then clock if we add duration cast to
access the count value of te time point. Just the call of the now() in chrono and clock
is the same.


Useful links to get more about chrono library:

- [cppreference](https://en.cppreference.com/w/cpp/chrono)
- [blog post: chrono api](https://www.freecodecamp.org/news/cpp-std-chrono-api/)
- [The Three Clocks](https://www.modernescpp.com/index.php/the-three-clocks/)
- [C++20: Basic Chrono Terminology with Time Duration and Time Point](https://www.modernescpp.com/index.php/c20-basic-chrono-terminology-with-time-duration-and-time-point/)