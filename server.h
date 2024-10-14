#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT_NO 8080
#define BUF_SIZE 40960000

void* handleResponse(void* arg);
int main(int argc, char* argv[]);
char* determineType(char* ext);

#endif
