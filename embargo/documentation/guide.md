# Compute usage guide
## Command Submission
### Deadlock avoidance
According to Level Zero specification it is application responsibility to make sure there are no deadlocks.

[Deadlock definition](https://spec.oneapi.io/level-zero/latest/core/PROG.html?highlight=deadlock#events)

`*There are no protections against events causing deadlocks, such as circular waits scenarios.*`

`*These problems are left to the application to avoid.* `

Let's look at some scenarios that may introduce deadlocks.


