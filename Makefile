all: main

main: main.c htstress.h htstress.c
	${CC} -std=c11 -g -Wall main.c htstress.c -lcurl -pthread -o htstress
