CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu11 -O0 -ggdb -no-pie
LIBS:=-lm

all: cachesim

cachesim: cachesim.o ArgumentParser.o Cache.o CacheSet.o CacheLine.o CacheOperationInfo.o TraceValues.o SimulationSummary.o String.o FileProcess.o HandleMemoryFail.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

cachesim.o: cachesim.c
	$(CC) $(CFLAGS) -c $^ -o $@

ArgumentParser.o: ArgumentParser/ArgumentParser.c
	$(CC) $(CFLAGS) -c $^ -o $@

Cache.o : Cache/Cache.c
	$(CC) $(CFLAGS) -c $^ -o $@

CacheSet.o: Cache/CacheSet/CacheSet.c
	$(CC) $(CFLAGS) -c $^ -o $@

CacheLine.o: Cache/CacheLine/CacheLine.c
	$(CC) $(CFLAGS) -c $^ -o $@

CacheOperationInfo.o: CacheOperationInfo/CacheOperationInfo.c
	$(CC) $(CFLAGS) -c $^ -o $@

TraceValues.o: InputParser/TraceValues.c
	$(CC) $(CFLAGS) -c $^ -o $@

SimulationSummary.o: SimulationSummary/SimulationSummary.c
	$(CC) $(CFLAGS) -c $^ -o $@

HandleMemoryFail.o: MemoryFails/HandleMemoryFail.c
	$(CC) $(CFLAGS) -c $^ -o $@

String.o: String/String.c
	$(CC) $(CFLAGS) -c $^ -o $@

FileProcess.o: FileProcess/FileProcess.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o
	rm -f cachesim
