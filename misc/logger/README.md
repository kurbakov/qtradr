# Logger

Async logger with SPSC queue for each thread and log processor is pinned to a dedicated CPU ID.

Each thread has own log client and each client register itself during the thread creation and unregister on exit
of the thread.

## Key components:

1. Collection of macros LOGR_(LEVEL): read the log from the code
2. Client (thread local object): responsible for taking the log and pushing to the queue
3. Processor (singleton): responsible for reading for the queue and pass it to the streamed
4. Streamer (owned by Processor): stream log to STD OUT (potentially can write any streamer and push logs to file, DB,
   network)

