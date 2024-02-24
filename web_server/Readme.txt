If a new client(Default from Linux : nc 127.1 8889) connects with the epoll_server, the serer will show the client's ip, port, and 
the first line of the HTTP message -- GET /xx.x HTTP/1.1 -- which is the indispensable information of a HTTP request. 

The server will send a client browser the file it requires and display relevant information to show the types of the required file.
If the file does not exist in the server's repository, then the server will tell the client some information.

The web-development file folder is the project of mine as the course's final project which is a consultant company website.  