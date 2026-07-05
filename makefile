#
# Compiler and flags
CC = g++
CFLAGS = -Wall -Werror -std=c++17

#
# Default target
all: load_balancer

#
# Link object files to create executable
load_balancer: main.o loadbalancer.o
	$(CC) $(CFLAGS) -o load_balancer main.o loadbalancer.o

#
# Compile main.cpp into main.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

#
# Compile LoadBalancer.cpp into loadbalancer.o
loadbalancer.o: LoadBalancer.cpp
	$(CC) $(CFLAGS) -c LoadBalancer.cpp

# 
# Clean up generated files
clean:
	rm -f load_balancer *.o