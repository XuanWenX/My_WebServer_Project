# My_WebServer_Project


Environment: 
This project is compiled by GCC in Ubuntu 18.04 virtually working in VMware Workstation Pro.

Learning Diary:
1. Better familiar with C programming language, TCP/IP protocol, HTTP protocol, and I/O multiplexing technique.
2. Must remember to store the snapshots of Ubuntu for fear that the OS collapse.
3. Notice that every time a client finishes its requests then the webserver should drop its cfd from the epoll_event to save storage space.
4. If a function is used many times as a common function, then it can be written as a separate function to be convenient to call the next time.
5. Follow GCC's instructions when revising the program and rely on the 'man' command in Linux OS.
6. Be careful when passing the parameters into a function. Make sure it is used.

Process(Debugging by GCC):
![image](https://github.com/XuanWenX/My_WebServer_Project/blob/main/web_server/debugging.png)

Success Scenes:
![image](https://github.com/XuanWenX/My_WebServer_Project/blob/main/web_server/Successful.png)

![image](https://github.com/XuanWenX/My_WebServer_Project/blob/main/web_server/Successful1.png)
