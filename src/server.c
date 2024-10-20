#include "server.h"

int main(int argc, char* argv[]){
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int server_fd, client_fd; //fd denotes socket descriptor
    char *bufferReadIn = (char*) malloc(BUF_SIZE * sizeof(char));
    printf("Searching for connection...\n");
    server_fd = socket(AF_INET, SOCK_STREAM, 0); //Socket uses domain AF_INET(IPV4), we use TCP for communication as more reliable, and final parameter indicates usage of IP protocol.
    if (server_fd < 0){
        perror("Failed connection!\n");
        return -1;
    }
    int opVal = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opVal, sizeof(opVal)) < 0) {
        perror("Failed to set SO_REUSEADDR option");
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NO);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Let OS assign IP.
    //Binds socket to address/port number in server_addr
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Failed to bind.\n");
        return -1;
    }
    printf("Binded to port.\n");
    if (listen((server_fd), 10) < 0){ //Waits for connection from client, builds a queue of connections up to a maximum capacity of 10.
        perror("Failed to listen.\n");
        return -1;
    }
    while (true){
        //Accepts a connection from the aforementioned queue
        if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen) < 0)){
            perror("Couldn't accept connection.\n");
            continue;
        }
        ssize_t receiver;
        pthread_t newServerThread;
        int* clientPointer = malloc(sizeof(int));
        *clientPointer = client_fd;
        pthread_create(&newServerThread, NULL, handleResponse, (void*) clientPointer);
        pthread_detach(newServerThread);      
    }
    close(server_fd);
    return 0;
}
