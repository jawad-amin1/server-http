#include "server.h"
char* determineType(char* ext){  
    if (strcmp(ext, "html") == 0 || strcmp(ext, "htm") == 0)
        return "text/html";
    else if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
        return "image/jpeg";
    else if (strcmp(ext, "css") == 0)
        return "text/css";
    else if (strcmp(ext, "png") == 0)
        return "image/png";
    else if (strcmp(ext, "js") == 0)
        return "application/javascript";
    else
        return "application/octet-stream"; //Indicates data trasmitted is binary stream of bytes (defaulted)
}

void* handleResponse(void* arg){
    int client_fd = *(int*) arg;
    char *bufferReadIn = (char*) malloc(BUF_SIZE * sizeof(char));
    ssize_t receiver;
    free(arg);
    if ((receiver = recv(client_fd, bufferReadIn, BUF_SIZE, 0)) <= 0){
        free(bufferReadIn);
        close(client_fd);
        return NULL;
    }
    FILE *fileToSend;
    long fileSize;
    char* bufPreserved = strdup(bufferReadIn);
    ssize_t bytesSent;
    char* reply = (char*)malloc(BUF_SIZE * sizeof(char));
    char* reply_error = "HTTP/1.1 404 Not Found\r\n\r\n";

    char *http_method = strtok(bufferReadIn, " "); //"GET /filename.ext HTTP/1.1", retrieves HTTP method.

    char *link = strtok(NULL, "/"); //NULL allows us to continue tokenizing buffer. We now have a path, and method.
    if ((strcmp(link, "/")) == 0){
        sprintf(reply,"HTTP/1.1 200 OK\r\n\r\n");
        bytesSent = send(client_fd, reply, strlen(reply),0);
    }
    else if((strncmp(link, "echo", 4))==0){
        link = strtok(NULL, " ");
        sprintf(reply, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(link), link);
        printf("Response sent.\n");
        bytesSent = send(client_fd, reply, strlen(reply),0);
    }
    else if((strncmp(link, "file",4)==0) &&(strcmp(http_method,"POST") ==0)){ 
        link = strtok(link, " "); //name of file stored in link
        fileToSend = fopen(link, "wb+");
        if (!fileToSend){
            printf("Error when opening file.");
            bytesSent = send(client_fd, reply_error, strlen(reply_error),0);
        }
        else{
           
            char *contentLengthStr  = strstr(bufPreserved, "Content-Length:"); //searches for first occurrence of "Content-Length:" within the buffer.
            contentLengthStr = strtok(contentLengthStr + 16,"\r\n"); //"7"
            int contentLength = atoi(contentLengthStr);
            char *postInfo  = strtok(NULL,"\r\n");
            postInfo  = strtok(NULL,"\r\n");
                /* LINES PARSED THROUGH:
                Host: 127.0.0.1:8080
                User-Agent:
                Accept:
                Content-Length:
                Content-Type:
                Information the client wishes to POST */
            // 

            fwrite(postInfo, 1, contentLength, fileToSend);
            fclose(fileToSend);
            char* fileExtensionP = strtok(link, "."); //parses through link name to find file extension
            fileExtensionP = strtok(NULL, " ");
            char* contentType = determineType(fileExtensionP);
            sprintf(reply ,"HTTP/1.1 201 Created\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n%s",contentType, (int) contentLength, postInfo);
            printf("Constructed response: %s\n",reply);
            bytesSent = send(client_fd, reply, strlen(reply),0);
        }
    }
    else if (strncmp(link, "file", 4)==0 && strcmp(http_method, "GET")==0){
        link = strtok(NULL, " ");
        fileToSend = fopen(link, "rb");
        if (!fileToSend){
            printf("Error when opening file.\n");
            bytesSent = send(client_fd, reply_error, strlen(reply_error),0);
        }
        else{
            fseek(fileToSend, 0, SEEK_END); //read file to end
            fileSize = ftell(fileToSend); //obtain file size
            rewind(fileToSend); //rewind the pointer back in order to perform fread() function.
            char* filedata = malloc(fileSize);
            if (fread(filedata, 1, fileSize, fileToSend) != fileSize){
                printf("Could not read file.");
                bytesSent = send(client_fd, reply_error, strlen(reply_error), 0);
            }
            else{
                char* fileExtensionP = strtok(link, ".");
                fileExtensionP = strtok(NULL, " ");
                char* contentType = determineType(fileExtensionP);
                char* headers = malloc(sizeof(char) * 100);
                int headerLength = sprintf(headers,
                                    "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: %s\r\n"
                                    "Content-Length: %ld\r\n"
                                    "\r\n", contentType, fileSize);
                memcpy(reply, headers, headerLength);   
                memcpy(reply + headerLength, filedata, fileSize);                
                bytesSent = send(client_fd, reply, headerLength + fileSize,0);
                free(headers);
            }
            fclose(fileToSend);
            free(filedata);
        }
    }
    else{
        bytesSent = send(client_fd, reply_error, strlen(reply_error),0);
    }
    free(bufferReadIn);
    close(client_fd);
    free(reply);  
    return NULL;
}
