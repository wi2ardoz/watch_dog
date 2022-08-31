# Watch Dog
SW Watch Dog library for Linux distros (POSIX only)

This library provides API for forlking new process companing the original process running, in order to keep it alive.
The method it uses is via IPC (specifically via POSIX Signals).
It supports concurrency (thread-safe).
