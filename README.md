# Cache Tracer
 A cache tracer to measure the cache hits/misses of a program.

This was a task from the subject Computer Organization.
It consisted of implementing a cache tracer.
The program takes a trace file, the specifications of the cache memory
(size, ways and lines) and the verbose mode flag and range.

The input file should have memory accesses in the next format:
* [instruction-pointer]: [op-type (R/W)] [mem-address] [bytes-written/read] [data-written/read]

It outputs a summary with the next format:
* [E]-way, [S] sets, size = [C]KB
* loads [a] stores [b] total [a + b]
* rmiss [c] wmiss [d] total [c + d]
* dirty rmiss [e] dirty wmiss [f]
* bytes read [g] bytes written [h]
* read time [i] write time [j]
* miss rate [(c + d) / (a + b)]

And verbose operations within the given range with the next format:
* [op-index] [1 (hit) | 2a (miss) | 2b (dirty-miss)] [cache-index] [cache-tag] [cache-line] [previous-tag] [valid]
[dirty] [last-op-index]