#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "md5sum.h"
#include "image_container.h"
#include "upload_download.h"
#include "CSV.h"

int main(int argc, char *argv[])
{
   FILE* csv;                               // Handle for csv file
   FILE* uploadHandle;                      // Handle for file being sent to client
   int sockfd, newsockfd, portno;
   socklen_t clilen;
	char* reply1 = "1";                      // Used to tell client that request was received and can be fulfilled
   char* reply0 = "0";                      // Used to tell client that request was received but cannot be fulfilled
   char* csvPath = "./catalog.csv";         // Path to server side csv
   struct sockaddr_in serv_addr, cli_addr;
   int n = 0;                               // Used for error reporting
   int code = 0;                            // Holds download code
   char buffer[256];                        // Holds requests from client

   if (argc < 3) // Check for correct number of arguments
   {
      fprintf(stderr,"Usage: %s <port> <directory>\n",argv[0]);
      exit(1);
   }

  // Create CSV file
  /****************************************************************************/
   fileList imageList; // List of all valid images in directory
   if((csv = fopen(csvPath,"w")) == NULL)
   {
     fprintf(stderr,"Failed to open catalog.csv Exiting...\n");
     exit(1);
   }
   fprintf(csv,"filename, size, checksum\n");
   init_fileList(&imageList,4); // initialize list
   create_CSV(csv,argv[2],&imageList);
   if (fclose(csv))
   {
     fprintf(stderr,"Failed to close catalog.csv Exiting...\n");
     exit(1);
   }
  /****************************************************************************/

  // Set up server connection
  /****************************************************************************/
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
   {
      error("ERROR opening socket");
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
   {
      error("ERROR on binding");
   }
   if (listen(sockfd,5) == -1)
   {
      error("ERROR on listen");
   }
   clilen = sizeof(cli_addr);
   printf("Server Waiting for new client\n");
   newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
   if (newsockfd < 0)
   {
      error("ERROR on accept");
   }
 /*****************************************************************************/

 // Send CSV to client
 /*****************************************************************************/
  printf("Connection established, sending CSV...\n");
  if((csv = fopen(csvPath,"r")) == NULL)
  {
    fprintf(stderr,"Failed to open catalog.csv for client Exiting...\n");
    exit(1);
  }
  upload(newsockfd,csv);
  if (fclose(csv))
  {
    fprintf(stderr,"Failed to close catalog.csv Exiting...\n");
    exit(1);
  }
/******************************************************************************/

  while (1) // Wait for image requests from client
  {
    printf("Waiting for request from client\n");
    bzero(buffer,255);
    n = recv(newsockfd,buffer,255,0); // Wait for client to request image
    if (n < 0) {error("ERROR reading from socket");}
    printf("New image requested\n");
    if(!atoi(buffer)) // Check is client sent exit command
    {
      printf("Recieved disconnect command, Exiting...\n");
 	   close(sockfd);
      destroy_fileList(&imageList);
 	   return 0;
    }

    code = atoi(buffer) - 1; // Adjust integer sent to server so it can be used to index list of files

    if(code >= imageList.nextFree || code < 0) // if index sent by client is invalid
    {
      n = send(newsockfd, reply0, strlen(reply0),0);
      if (n < 0) {error("ERROR writing to socket");}
      continue;
    }

    else // Send Client requested image
    {
      if((uploadHandle = fopen(getFile(&imageList,code)->filePath,"r")) == NULL)
      {
        fprintf(stderr,"Failed to open %s for client\n",getFile(&imageList,code)->filePath);
        n = send(newsockfd, reply0, strlen(reply0),0); // alert client that the file could not be found
        if (n < 0) {error("ERROR writing to socket");}
        continue;
      }

      n = send(newsockfd, reply1, strlen(reply1),0); // send client confirmation that it can send requested image
      if (n < 0) {error("ERROR writing to socket");}

      upload(newsockfd,uploadHandle); // Send image to client

      if (fclose(uploadHandle))
      {
        fprintf(stderr,"Failed to close %s\n",getFile(&imageList,code)->filePath);
      }
    }
  }
  // Should never get here
  close(sockfd);
  destroy_fileList(&imageList);
  return 0;
}
