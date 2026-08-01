_This project has been created as part of the 42 curriculum by <erzhuo>._

# Philosophers

## Description

Philosophers is a simulation of the classic **dining philosophers problem**, used
to explore the fundamentals of threading and synchronization in C.

A number of philosophers sit around a round table with a large bowl of spaghetti
in the middle. Each philosopher alternates between three states — **eating**,
**sleeping**, and **thinking**. There are as many forks as philosophers, and a
philosopher needs **both** the fork on their left and the fork on their right to
eat. Since neighbours share forks, they cannot all eat at the same time.

The goal is to keep every philosopher alive: if a philosopher does not start
eating within `time_to_die` milliseconds of their last meal, they starve and the
simulation ends. The program must run without **data races** and without
**deadlocks**, and must report every state change with an accurate, ordered log.

This mandatory part represents each philosopher as a **thread**, and protects each
fork with a **mutex**.

## Features

- Each philosopher runs in its own thread.
- Each fork is protected by its own mutex.
- A dedicated **monitor thread** detects starvation and the optional
  "everyone has eaten enough" stop condition.
- Deadlock avoidance through an odd/even fork-picking order plus a small
  start-up offset for even-numbered philosophers.
- Precise timing via a custom `ft_usleep` (loops on short sleeps and re-checks
  the clock) instead of relying on a single imprecise `usleep`.
- Death is announced within the required 10 ms tolerance, and no status message
  is ever printed after a philosopher has died.
- No global variables, no memory leaks, no data races.

## Instructions

### Compilation

```bash
make          # build the philo binary
make clean    # remove object files
make fclean   # remove object files and the binary
make re       # rebuild from scratch
```

The project is compiled with `cc` and the flags `-Wall -Wextra -Werror -pthread`.

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Meaning |
| --- | --- |
| `number_of_philosophers` | Number of philosophers (and forks) |
| `time_to_die` | Milliseconds a philosopher can go without eating before dying |
| `time_to_eat` | Milliseconds a philosopher spends eating (holding two forks) |
| `time_to_sleep` | Milliseconds a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | *(optional)* if every philosopher eats at least this many times, the simulation stops |

All times are in milliseconds.

## Usage examples

```bash
./philo 5 800 200 200      # runs until stopped (no one should die)
./philo 5 800 200 200 3    # stops once every philosopher has eaten 3 times
./philo 1 800 200 200      # a single philosopher cannot get a second fork and dies
./philo 4 310 200 100      # a tight configuration where a philosopher starves
```

The log format for each state change is:

```
timestamp_in_ms  X  has taken a fork
timestamp_in_ms  X  is eating
timestamp_in_ms  X  is sleeping
timestamp_in_ms  X  is thinking
timestamp_in_ms  X  died
```

where `timestamp_in_ms` is the time elapsed since the start of the simulation and
`X` is the philosopher's number.

## Technical choices

- **One thread per philosopher, one mutex per fork.** Forks live in a single
  array; each philosopher holds pointers into that array so that neighbours share
  the exact same mutex object.
- **A separate monitor thread** is the only component with a global view. It
  periodically checks every philosopher's last-meal timestamp and meal count, and
  is responsible for ending the simulation (announcing a death, or stopping once
  the meal quota is reached). Philosophers never decide on their own that the
  simulation is over — they read a shared `stop` flag at the top of their loop.
- **Deadlock avoidance.** If every philosopher grabbed the same-side fork first,
  they could all hold one fork and wait forever for the other (circular wait).
  Odd-numbered philosophers take their right fork first and even-numbered ones
  take their left fork first, which breaks the circular wait. A small start-up
  offset for even philosophers reduces contention and starvation on tight
  configurations.
- **Shared-state protection.** Every piece of state shared between threads is
  guarded by a mutex: forks by their own mutexes, `last_meal` / `meals_eaten` by a
  per-philosopher `meal_lock`, the `stop` flag by a `stop_lock`, and terminal
  output by a `print_lock`. Locks are always acquired in a consistent order
  (`print_lock` before `stop_lock`) to avoid deadlocks between the printing path
  and the death-announcement path. Read-only values set before the threads start
  (the parsed arguments, `start_time`, philosopher ids) need no lock.
- **Timing.** All durations are handled in milliseconds. `ft_usleep` sleeps in
  small increments and re-checks `gettimeofday` so that it does not overshoot the
  requested duration, which keeps starvation detection and the death deadline
  accurate.

## Testing

- **Behaviour** (survival, death timing, stop conditions) is tested by running the
  program directly, without any profiling tool — instrumentation tools slow the
  process down and distort the timing, which can cause false starvation.
- **Memory leaks** are checked separately with `leaks` (macOS) or
  `valgrind --leak-check=full` (Linux), using a configuration that terminates
  cleanly (e.g. `./philo 4 1000 50 50 2`).
- **Data races** are checked with `valgrind --tool=helgrind` or with
  `-fsanitize=thread` (ThreadSanitizer).

## Resources

Classic references on threads and the dining philosophers problem:

- `pthread` manual pages: `pthread_create(3)`, `pthread_join(3)`,
  `pthread_detach(3)`, `pthread_mutex_init(3)`, `pthread_mutex_lock(3)`,
  `pthread_mutex_unlock(3)`, `pthread_mutex_destroy(3)`, `gettimeofday(2)`,
  `usleep(3)`.
- Dijkstra, E. W. — the original formulation of the dining philosophers problem.
- Wikipedia: "Dining philosophers problem" (deadlock, resource starvation, and
  the standard resource-ordering solution).
- General reading on data races, mutual exclusion and memory visibility in
  multithreaded C.

### Use of AI

An AI assistant (Claude) was used throughout this project, mainly as a tutor and
pair-programming partner. Concretely:

- **Understanding concepts.** Explaining threads, mutexes, data races, deadlock
  and starvation, and lower-level C topics that the project relies on (pointers,
  the `void *` thread-function signature, `.` vs `->`, the stack/heap distinction,
  function pointers, and how `pthread_create` / `pthread_join` behave).
- **Design.** Discussing the overall architecture — the `t_table` / `t_philo`
  data structures, which field is written/read by which thread and which lock
  protects it, and the split between the philosopher routine and the monitor
  thread.
- **Implementation and review.** Drafting and reviewing individual functions
  (argument parsing, initialization with staged cleanup, the philosopher loop,
  the monitor, the timing and printing helpers), which were then read, adjusted
  and tested by me.
- **Debugging.** Locating real bugs (an incorrect fork-picking order, a pointer
  passed with a missing index, a race between the death message and other status
  prints) and understanding misleading behaviour (false starvation caused by
  running the program under a profiling tool).

The AI was used to learn and to move faster, not as a substitute for
understanding. Every part of the final code was reviewed, tested and can be
explained.
