# Evidence 4 Demonstration of a Programming Paradigm
Ian Hernández Hernández - A01276755

## Description
This project simulates an application that receives and processes images concurrently. Multiple threads (producers) generate image data, and multiple threads (consumers) process them. A shared queue is used for communication between producers and consumers, ensuring synchronization with mutexes and condition variables.

This pattern is widely used in real-time data processing systems, web servers, multimedia, streaming applications, and concurrent back-end systems.

	•	Describe el contexto del problema:
	•	¿Qué hace el programa?
	•	¿A quién beneficia?
	•	¿Por qué es útil?

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
	•	std::thread – Used to launch multiple producer and consumer threads.
	•	std::mutex – Ensures only one thread accesses the shared queue at a time (prevents race conditions).
	•	std::condition_variable – Lets threads wait and notify each other efficiently (e.g., consumers wait until the queue has images).
    
### Implementation
	•	Programa la solución en un lenguaje que soporte el paradigma elegido:
	•	Ej. Python para programación funcional o imperativa.
	•	Prolog para programación lógica.
	•	Java/C++/Python para POO.

 ### Test
	•	Implementa y documenta pruebas:
	•	Casos base, normales y extremos.
	•	Usa asserts, frameworks o pruebas manuales documentadas.
	•	Muestra que tu programa funciona correctamente.

### Analysis
	•	Complejidad de tiempo:
	•	¿Cuál es la eficiencia del algoritmo? (O(n), O(log n), etc.).
	•	Otros paradigmas posibles:
	•	¿Qué otros paradigmas podrían resolver el problema?
	•	¿Qué diferencias habría en eficiencia, legibilidad o escalabilidad?