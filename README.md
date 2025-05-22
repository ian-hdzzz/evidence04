# Evidence 4 Demonstration of a Programming Paradigm
Ian Hernández Hernández - A01276755

## Description
This project simulates an application that receives and processes images concurrently. Multiple threads (producers) generate image data, and multiple threads (consumers) process them. A shared queue is used for communication between producers and consumers, ensuring synchronization with mutexes and condition variables.

This pattern is widely used in real-time data processing systems, web servers, multimedia, streaming applications, and concurrent back-end systems.


##  Models
Paradigm: Concurrent Programming in C++

### Why Concurrent Programming?
This problem involves multiple producers (simulating image uploads) and multiple consumers (simulating image processing), all operating simultaneously. To handle these parallel activities efficiently, we need a model where threads can work independently but share resources safely.

Concurrent programming allows us to:
1. Perform multiple tasks at the same time, improving responsiveness and throughput.
2. Share a common resource (the image queue) between threads in a controlled way.
3. Coordinate producer and consumer activity without data loss or race conditions.

This model is common in real-world systems like web servers, background processors, or live data streams, where different tasks must run simultaneously and communicate safely.

### Key Concepts Used:
	• thread – Used to launch multiple producer and consumer threads.
	• mutex – Ensures only one thread accesses the shared queue at a time (prevents race conditions).
	• condition_variable – Lets threads wait and notify each other efficiently (e.g., consumers wait until the queue has images).

## Implementation
	•	Programa la solución en un lenguaje que soporte el paradigma elegido:
	•	Ej. Python para programación funcional o imperativa.
	•	Prolog para programación lógica.
	•	Java/C++/Python para POO.

 ## Test
	•	Implementa y documenta pruebas:
	•	Casos base, normales y extremos.
	•	Usa asserts, frameworks o pruebas manuales documentadas.
	•	Muestra que tu programa funciona correctamente.

## Analysis

### Time Complexity
	• Production and consumption: O(1) per operation (insertion or removal from the queue).
	• However, the total execution time depends on the simulated (random) delays.
	• Thanks to thread synchronization using mutexes and condition variables, the shared queue is accessed in a safe and efficient manner, even with multiple threads running concurrently.

The O(1) complexity comes from the underlying queue data structure (std::queue in C++), which allows constant-time insertion and deletion. The use of threads does not change this complexity—it simply enables operations to occur concurrently. Proper synchronization ensures that these O(1) operations remain correct and do not result in race conditions.

### Other possible paradigms tha could solve the problem

* Logic (Prolog)
Viable here: No
Not suitable for concurrency or dynamic I/O handling.

* Scripting (Shell)
Viable here: No
Difficult to coordinate multiple tasks with precise synchronization.

* Functional (Scheme)
Viable here: Partially
Possible with functional threads, but complex to manage shared resources.

* Parallel (OpenMP)
Viable here: Yes
Suitable if processing were purely parallel without shared queues.

 • Logic Programming (Prolog): Great for rule-based problems, but not designed for concurrent operations or real-time data flow.
 • Scripting (Shell): Useful for automation and simple task pipelines, but lacks the tools for fine-grained thread control or shared data protection.
 • Functional Programming (Scheme): Supports concurrency through immutability and pure functions, but sharing mutable state (like a queue) is tricky and error-prone.
 • Parallel Programming (OpenMP): Effective for data-parallel tasks (e.g., matrix operations), but less ideal for producer-consumer patterns that require shared state coordination.
