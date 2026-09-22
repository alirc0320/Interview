---
topic: RTOS concepts and concurrency
date_written: <YYYY-MM-DD>
---

# Topic: RTOS Concepts and Concurrency

Maps to GFG Q26-30 (RTOS vs general-purpose OS, task priorities, priority
inversion, preemptive vs cooperative multitasking, semaphores) and Q106
(RTOS advantages/disadvantages). Explain these aloud and draw the
diagrams (task states, priority inversion timeline) the way you would for
the OS study routine — this is conceptual, not something to force into a
coding problem; a real RTOS scheduler isn't meaningfully testable with a
5-assert C program.

## 1. Concept explanation — RTOS vs. general-purpose OS
Q: What does "real-time" actually mean here — is it about being fast, or about something else? Define hard real-time vs. soft real-time in terms of what happens when a deadline is missed. Then name at least three concrete differences between how an RTOS (e.g. FreeRTOS, Zephyr, VxWorks) schedules and manages tasks versus how a general-purpose OS (Linux, Windows) schedules processes — think about scheduling determinism/worst-case latency, memory footprint, and how much of the "kernel" is even present.
A:

## 2. Concept explanation — task priorities and preemption
Q: Explain what a task priority controls in a typical priority-based preemptive RTOS scheduler: when does a higher-priority task actually interrupt a lower-priority one that's currently running? Then explain the difference between preemptive and cooperative multitasking — in a cooperative scheduler, what specifically has to happen for control to move to another task, and what's the practical risk of a single misbehaving task in that model that doesn't exist (in the same way) under preemptive scheduling?
A:

## 3. Concept explanation — priority inversion
Q: Walk through the classic three-task priority inversion scenario step by step: a low-priority task holds a mutex/resource, a high-priority task needs that same resource and blocks, and a medium-priority task (which needs neither) preempts the low-priority task and runs indefinitely — explain concretely why the high-priority task ends up waiting on the medium-priority one even though it never touches the medium task's work. Then explain priority inheritance (the low-priority task temporarily inherits the high task's priority while it holds the contested resource) as the standard fix, and why that fix specifically breaks the chain you just described.
A:

## 4. Concept explanation — semaphores and mutexes
Q: Explain the difference between a binary semaphore, a counting semaphore, and a mutex — they're often confused, but they solve different problems. Specifically: a mutex has the concept of "ownership" (only the task that locked it can unlock it) and supports priority inheritance; a semaphore typically doesn't have an owner and is commonly used for signaling between tasks/ISRs (e.g. an ISR "gives" a semaphore that a task is blocked "taking"), not just mutual exclusion. Give one concrete embedded scenario each: one where a mutex is the right tool, one where a counting semaphore is the right tool (bounding access to a pool of N identical resources), and one where a plain binary semaphore used purely for ISR-to-task signaling is the right tool (and would be the wrong use case for a mutex, since ISRs generally can't "own"/block on a mutex the way a task can).
A:

## 5. Debugging / design — sharing data between an ISR and a task
Q: A sensor ISR fires frequently and updates a multi-byte struct (say, a 3-axis reading: three `int16_t` fields) that the main task reads periodically. Explain what can go wrong if the main task just reads the struct fields directly while the ISR can fire and update them mid-read (a torn/inconsistent read — not a single-word atomicity problem, since it's multiple fields), and describe at least one correct pattern to fix it: e.g. disabling that specific interrupt briefly while copying the struct out, using a double-buffer with an atomic "which buffer is current" flag, or a lock-free single-producer/single-consumer technique if the RTOS/hardware supports one. Explain why "just make the struct `volatile`" does NOT fully solve this (tie back to `03_volatile_qualifier.md` question 3).
A:

## 6. Concept explanation — RTOS tradeoffs (advantages/disadvantages)
Q: You're deciding whether a new embedded project needs an RTOS at all, versus a simple bare-metal superloop (`while(1) { checkSensor(); updateActuator(); ... }`). Give at least two concrete advantages an RTOS brings (e.g. easier reasoning about independently-timed concurrent activities, priority-based responsiveness for hard deadlines, built-in primitives for the kind of ISR-to-task handoff from question 5) and at least two concrete disadvantages/costs (memory/flash footprint, added complexity and a whole new class of bugs like priority inversion and race conditions that a superloop doesn't have, context-switch overhead). What's the actual decision factor — is it about how many independent timing-sensitive activities the system has, or something else?
A:

---

## Corrections
Reopen the material. Fix anything wrong above (don't erase the original
wrong answer — strike it through or note what was wrong, so the mistake
stays visible on review).

-

## Missed on first pass?
- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
