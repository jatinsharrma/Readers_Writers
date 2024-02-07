# Reader Writer Problem

This repository demonstrates the Reader-Writer problem with two scenarios:

**Reader priority**: Readers have priority over writers, meaning they can access the shared resource concurrently as long as no writer is present.
**Writer priority**: Writers have exclusive access to the shared resource, meaning only one writer can access it at a time, and readers must wait until the writer finishes.
Each scenario is implemented in a separate file:

**read_priority.c**: Demonstrates the Reader-Writer problem with reader priority.
**writer_priority.c**: Demonstrates the Reader-Writer problem with writer priority.

Both files utilize the following elements:
**Semaphores**: To control access to the shared resource and ensure mutual exclusion.
**Shared resource**: A file used as the shared resource that readers read and writers write to.
**Threads**: To simulate concurrent readers and writers.

## To run the code:

>Make sure you have a C compiler installed.
>Clone this repository to your local machine.
>Open a terminal in the repository directory.
>Compile each file using your compiler (e.g., gcc writer_priority.c -o wp -pthread  and gcc read_priority.c -o wp -pthread ).
>Run each executable file (e.g., ./rp and ./wp).

## Observations:

Observe how the code behaves in each scenario, paying attention to the order in which readers and writers access the shared resource.
Modify the number of readers and writers to see how it affects the behavior.

## Additional Notes:
This is a basic implementation of the Reader-Writer problem.
There are more complex and efficient solutions available.
This repository is intended for educational purposes only.
