#include<stdio.h>
#include"wrap.h"
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"pub.h"
#define PORT 8889

void send_header(int cfd, int code, char* info, char* filetype, int length) //send HTTP header
{
	//send state line, 
	char buf[1024] = "";
	int len = 0;
	len = sprintf(buf, "HTTP/1.1 %d %s\r\n", code, info);
	send(cfd, buf, len, 0); //be careful that code might be 0
	
	//info header,
	len = sprintf(buf, "Content-Type:%s\r\n", filetype);
	send(cfd, buf, len, 0);
	if(length > 0)
	{
		len = sprintf(buf, "Content-Length:%d\r\n", length);
		send(cfd, buf, len, 0);
	}
	// line break
	send(cfd, "\r\n", 2, 0);
}

void send_file(int cfd, char* pathname, struct epoll_event* ev, int epfd)  //read file's content and send the filestream to cfd
{
	int fd = open(pathname, O_RDONLY);
	if(fd < 0)
	{
		perror("");
		return ;
	}
	char buf[1024] = "";
	int len = 0;
	while(1)
	{
		len = read(fd, buf, sizeof(buf));
		if(len < 0)
		{
			perror("");
			break;
		}
		else if(len == 0) //read the file over
		{
			break;
		}
		else
		{
			send(cfd, buf ,len, 0);
		}
	}
	close(fd);
	close(cfd);  //when the filestream is transferred over, close the cfd and drop it from the epoll tree
	epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, ev); // solve the loading problem in browers
}

void read_client_request(int epfd, struct epoll_event* ev)
{
	//read the first line of a HTTP message and drop the rest of the message
	char buf[1024] = "";
	char tmp[1024] = "";
	int n = Readline(ev->data.fd, buf, sizeof(buf)); //read the HTTP's first line
	if(n <= 0)
	{
		perror("The client is closed or has some errors");
		epoll_ctl(epfd, EPOLL_CTL_DEL, ev->data.fd, ev);
		close(ev->data.fd);
		return ;
	}
	printf("[%s]\n", buf);
	int ret = 0;
	while( (ret = Readline(ev->data.fd, tmp, sizeof(tmp))) > 0 ); //read the rest of the message
	
	//parse the request line : GET /a.txt HTTP/1.1\r\n
	char method[256] ="";
	char file[256] ="";
	char protocol[256] ="";
	sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", method, file, protocol); //jump over blank space
	printf("[%s] [%s] [%s]\n", method, file, protocol);
	
	//judge whether it is a GET request, ignoring capital letters
	if(strcasecmp(method, "get") == 0)
	{
		char* strfile = file +1; //jump over '/'
		if(*strfile == 0) //no content in 'file', then go to current catalog
		{
			strfile = "./";
		}
		
		//judge whether there is such a file
		struct stat s;
		if( stat(strfile, &s) < 0 )  //no such a file
		{
			printf("no such a file\n"); 
			//send message head(state line, info header, line break)
			send_header(ev->data.fd, 404, "NOT FOUND", get_mime_type("*.html"), 0);
			
			//send the err.html to clients' browers
			send_file(ev->data.fd, "err.html", ev, epfd);
		}
		else
		{
			//is it a regular file?
			if(S_ISREG(s.st_mode))
			{
				printf("It is a normal file\n");
				
				//send message head(state line, info header, line break)
				send_header(ev->data.fd, 200, "OK", get_mime_type(strfile), s.st_size);
				
				//send the file to clients
				send_file(ev->data.fd, strfile, ev, epfd);
				
			}
			else if(S_ISDIR(s.st_mode))   //is it a directory?
			{
				printf("It is a directory so cannot be sent\n");
			}
			
		}
	}
	
}

int main(int argc, char* argv[])
{
	//switch working directory and get pwd
	char pwd_path[256] = "";
	char* path = getenv("PWD"); //get pwd
	strcpy(pwd_path, path);
	strcat(pwd_path, "/web-development");  //form an entire path contains the html projects
	chdir(pwd_path);
	
	//create socket and bind it
	int lfd = tcp4bind(PORT, NULL);
	
	//listen new request
	Listen(lfd, 128);
	
	//create epoll tree
	int epfd = epoll_create(1);
	
	//put lfd on the epoll tree
	struct epoll_event ev, evs[1024];
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	
	//listen circularly
	while(1)
	{
		int nready = epoll_wait(epfd, evs, 1024, -1);
		if(nready <= 0)
		{
			perror("");
			break;
		}
		else
		{
			for(int i = 0; i< nready; i++)
			{
				//whether it is a lfd that changes its state
				if(evs[i].data.fd == lfd && evs[i].events & EPOLLIN)
				{
					struct sockaddr_in cliaddr;
					char ip[16] = "";
					socklen_t len = sizeof(cliaddr);
					int cfd = Accept(lfd, (struct sockaddr*) &cliaddr, &len);
					printf("A new client ip = %s port = %d\n", 
					inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, 16),
					ntohs(cliaddr.sin_port));  //print the info of the client
					
					//set cfd as non-block state
					int flag = fcntl(cfd, F_GETFL);
					flag |= O_NONBLOCK;
					fcntl(cfd, F_SETFL, flag);
					
					//put cfd on the epoll tree
					ev.data.fd = cfd;
					ev.events = EPOLLIN;
					epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				}
				else if(evs[i].events & EPOLLIN) //if a cfd has changed
				{
					read_client_request(epfd, &evs[i]);
				}
			}
		}
	}
	
	
	
	return 0;
}
