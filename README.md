# HTTP Server

A multithreaded HTTP server built entirely in C.

## Description

A server that sets up client-server connections using TCP/IP connections, and sets up an initial backlog of 10 clients. Uses pthreading to incorporate 
multithreaded features within the server. Capable of handling GET and POST requests, it can load .jpg, .js, .png, .css and .html files within the browser.
Currently, the server only responds with 3 codes: 200 OK, 201 Created, and 404 Not Found

This is a project that is a rather simple implementation of HTTP, where more requests and response codes are planned to be incorporated and built on with further version updates.

## Dependencies

* The project requires you to be using an operating system compliant with the POSIX standard due to using the <sys/socket.h> header file.
* Unfortunately, this means the project will not work with Windows operating systems, and it is therefore highly recommended to run this project on a Unix based OS.
* For best performance, Mozilla Firefox is recommended as the browser for displaying/receiving files.
## Executing program

* Using the gcc compiler, enter this command in the terminal to produce a compiled server file.
```
gcc server.c server.h handleResponse.c -o server
```
* To run the file, enter the following command:
```
./server
```
<!-- ROADMAP -->
## Roadmap

- [x] Initial Multithreaded Implementation
- [x] Add GET requests
  - [x] Add capability to display images properly by handling binary data correctly 
  - [ ] Investigate the issue of dynamic webpages not loading correctly all the time  
- [x] Add basic POST requests
- [x] Add example sites/requests users can use to test code
- [ ] Add DELETE requests
- [ ] Add more response codes that allow for more specificity e.g. server code 500
- [ ] Add a simple LRU Cache implementation
- [ ] Investigate potential race conditions between threads e.g. incorporating a mutex.


## Requests
* To perform GET requests, go to your preferred browser and type the following in the address bar:
```
localhost:8080/file/<your_file_path_here>
```
* To perform POST requests, the following is an example instruction one would perform in a terminal:
```
#To post "hi there" to a file called filetext.txt
curl -X POST -d "hi there" http://127.0.0.1:8080/file/filetest.txt
```
## Issues
* When performing requests, you may need to repeatedly send a request 1 or 2 times before receiving the resource.
* There are cases where dynamic web pages are not loading correctly, only loading the HTML and JS, without the CSS file. 
  

## Version History
* 0.2
    * Fixed image handling issues, allowing clients to receive the requested images by reading binary data correctly
      * Caused by using sprintf to update a server response with the read data, which cut off the raw binary data at null bytes.
    * Fixed issue where running the server code after just using it resulted in delays, by using the following, using SO_REUSEADDR to allow the reuse of local addresses:
      ```
      setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opVal, sizeof(opVal)) < 0
      ```
    * Added server repository to GitHub profile 

* 0.1
    * Initial Creation

