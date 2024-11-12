# Philosophers - 42 Berlin
This repository contains my solution  **Philosophers** project for 42 Berlin school, implementing the **Arbitrator Solution** approach.

## The Dining Philosophers Problem
The Dining Philosophers problem is a classic example in computer science that shows the challenges of concurrent programming and dealing with potential deadlocks.

In this problem, philosophers sit around a table, each needing access to two resources (forks) to eat. The challenge lies in coordinating resource access to avoid deadlock, where philosophers are stuck waiting indefinitely.

There are three well-known solutions to this problem:
1. Resource Hierarchy
2. Arbitrator Solution
3. Chandy-Misra Solution

For this project, I chose the **Arbitrator Solution** with a twist.

## Arbitrator with (N-1) Seats
In my implementation, I limit the number of philosophers allowed at the table to `N-1`, where `N` is the total number of philosophers. This restriction prevents deadlock by ensuring that there is always at least one philosopher who can pick up both forks and eat, thus allowing the others to eventually do the same.

The solution uses an arbitrator (or controller) to manage the philosophers’ access to the forks, avoiding situations where philosophers indefinitely wait for each other.

