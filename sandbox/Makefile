
all : build exec


build : main.cpp
	@echo Build
	@g++ -g -Wall -std=c++14 main.cpp -o main.o

exec : main.o
	@echo Exec
	@./main.o
