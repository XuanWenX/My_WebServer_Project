#include"pub.h"

char* get_mime_type(char* name)
{
	char* dot;
	dot = strrchr(name, '.'); //find the clause from the last .
	
	if(dot == (char*)0) //default, no content
	{
		return "text/plain; charset = utf-8";
	}
	if(strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0) 
	{
		return "text/html; charset = utf-8";
	}
	if(strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0) 
	{
		return "image/jpeg";
	}
	if(strcmp(dot, ".gif") == 0) 
	{
		return "image/gif";
	}
	if(strcmp(dot, ".png") == 0) 
	{
		return "image/png";
	}
	if(strcmp(dot, ".css") == 0) 
	{
		return "text/css";
	}
	if(strcmp(dot, ".js") == 0) 
	{
		return "application/x-javascript";
	}
	if(strcmp(dot, ".mp3") == 0) 
	{
		return "audio/mp3";
	}
	if(strcmp(dot, ".mp4") == 0) 
	{
		return "video/mpeg4";
	}
	
	return "text/plain; charset=utf-8";
}