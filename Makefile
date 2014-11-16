all: main

main: main.c
	${CC} -g -Wall main.c -lcurl -o htstress
