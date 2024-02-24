#ifndef WRAP_H
#define WRAP_H
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
void perr_exit(const char *s);
int tcp4bind(short port, const char* IP);
int Accept(int fd,struct sockaddr *sa,socklen_t *salenptr);
void Bind(int fd,const struct sockaddr* sa,socklen_t salen);
void Connect(int fd,const struct sockaddr* sa,socklen_t salen);
void Listen(int fd,int backlog);
int Socket(int family,int type,int protocol);
ssize_t Read(int fd,void *ptr,size_t nbytes);
ssize_t Write(int fd,const void *ptr,size_t nbytes);
ssize_t Readn(int fd,void *vptr,size_t n);
ssize_t Writen(int fd,const void*vptr,size_t n);
static ssize_t my_read(int fd,char *ptr);
ssize_t Readline(int fd,void *vptr,size_t maxlen);
void Close(int fd);
 
#endif
