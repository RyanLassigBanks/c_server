#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include "upload_download.h"

void error(char *msg) // WORKS CITED: error was from recittion 13
{
    perror(msg);
    exit(1);
}

int upload(int clientSocket,FILE* file)
{
   char buff[520];
   char response[2];
   int bytesread;
   while((bytesread = fread(buff,sizeof(char),512,file)))
   {
      if (recv(clientSocket,response,2,0) < 0) // block until client requests packet
      {
         error("upload: ERROR reading from socket");
      }
      if (send(clientSocket,buff,bytesread,0) < 0)
      {
         error("upload: ERROR writing to socket");
      }
      bzero(buff,520);
      bzero(response,2);
   }
   strcpy(buff,"DONE");
   if (recv(clientSocket,response,2,0) < 0)
   {
      error("upload: ERROR reading from socket");
   }
   if (send(clientSocket,buff,strlen("DONE"),0) < 0)
   {
      error("upload: ERROR writing to socket");
   }
   return 0;
}

int download(int socket,FILE* file)
{
   char buff[520];
   char* confirm = "y";
   int bytesRec;
   while(1)
   {
      if (send(socket,confirm,2,0) < 0) // Send message to server requesting data
      {
         error("download: ERROR writing to socket");
      }
      if((bytesRec = recv(socket,buff,512,0)) < 0)
      {
         error("download: ERROR reading from socket");
      }
      if(strstr(buff,"DONE"))
      {
         return 0;
      }
      fwrite(buff,sizeof(char),bytesRec,file);
      bzero(buff,520);
   }
}
