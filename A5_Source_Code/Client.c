#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "upload_download.h"
#include "image_container.h"
#include "CSV.h"
#include "HTML.h"

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	long addr;
	char* imageDir = "./images";             // path to images
	char* htmlPath = "./download.html";      // path to download.html
	char* csvPath = "./images/catalog.csv";  // path to catalog.csv
	char* fileName;
	char* filePath;
	FILE* downloadHandle;
	FILE* csvHandle;
	FILE* passiveHandle = NULL;
	int image = 0;
	int interactiveMode = 1;                 // 0 when interactive mode is disabled, 1 when enabled
	int isCorrupted = 0;                     // 1 if image file was NOT corrupted in download, 0 otherwise
	char* downloadHASH;                      // The MD5 hash code for the downloaded image
	char* csvHASH;                           // The MD5 hash for the downloaded image from the CSV file
 	char buffer[256];                        // Holds message to send to server

	// Inital error checking and image directory set up
	/***************************************************************************/
	if (argc < 3)
	{
		fprintf(stderr,"usage %s <ip_address> <port> <optional_extension_arguments>\n", argv[0]);
		exit(1);
	}
	else if (argc == 4) // if 4 arguments are passed to client, enable passive mode
	{
		interactiveMode = 0;
	}

	if (mkdir(imageDir,0740) == 0 && errno != EEXIST)
	{
		printf("%s successfully created!\n\n",imageDir);
	}
	else if (errno == EEXIST) {
		printf("%s found\n\n",imageDir);
	}
	else
	{
		fprintf (stderr,"Output directory could not be found or created\n\n");
	   exit(1);
	}
	/***************************************************************************/

	// Create download.html
	/***************************************************************************/

	if(htmlStart(htmlPath))
	{
		fprintf(stderr,"Failed to create download.html Exiting...\n");
		exit(1);
	}
	/***************************************************************************/

	// Set up connection to server
	/***************************************************************************/
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}
	addr=inet_addr(argv[1]);
	server = gethostbyaddr((char *)&addr,sizeof(addr),AF_INET);
	if (server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	/***************************************************************************/

	// Download CSV
	/***************************************************************************/
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
		error("ERROR connecting");
	}
	if((csvHandle = fopen(csvPath,"w")) == NULL)
	{
		fprintf(stderr,"Failed to download csv file\n");
		exit(1);
	}
	download(sockfd,csvHandle); // download csv file from server
	if (fclose(csvHandle))
	{
		fprintf(stderr,"Failed to download csv file\n");
		exit(1);
	}
  /****************************************************************************/

	// reopen CSV file for manipulation
	/***************************************************************************/
	if ((csvHandle = fopen(csvPath,"r")) == NULL)
	{
		fprintf(stderr,"Failed to access client side csv\n");
		exit(1);
	}

	if(!interactiveMode)
	{
		if ((passiveHandle = fopen(csvPath,"r")) == NULL)
		{
			fprintf(stderr,"Failed to access client side csv\n");
			exit(1);
		}
	}
  /****************************************************************************/

	displayCSV(csvHandle); // Print contents of CSV to screen

	if(!interactiveMode) {printf("Running in passive mode... Downloading '%s' files\n",argv[3]);}

	while(1)
	{
		bzero(buffer,255);
		if(interactiveMode) // if interactive mode is enabled, request information from user.
		{
			printf("Enter ID to download (0 to quit): ");
			fgets(buffer,255,stdin);
		}
		else // else enter passive mode
		{
			sprintf(buffer,"%d",getFileIndex(passiveHandle,argv[3]));
		}
		n = send(sockfd,buffer,strlen(buffer),0); // send request to server
		if (n < 0)
		{
			error("ERROR writing to socket");
		}

		if (!atoi(buffer)) // finish writing to html file and exit if user enters 0 or if passive scan has finished
		{
			if(htmlEnd(htmlPath))
			{
				fprintf(stderr,"Failed to close download.html\n");
			}
			printf("Exiting...\n");
			fclose(csvHandle);
			if (!interactiveMode) {fclose(passiveHandle);}
			close(sockfd);
			return 0;
		}

		image = atoi(buffer);
		bzero(buffer,255);
		n = recv(sockfd,buffer,255,0); // Wait for confirmation from server that request was received
		if (n < 0)
		{
			error("ERROR reading from socket");
		}
		if (atoi(buffer))
		{
			fileName = getFileName(csvHandle,image);
			printf("Downloading %s...\n",fileName);

			filePath = malloc(strlen(fileName)+strlen(imageDir)+2);
			sprintf(filePath,"%s/%s",imageDir,fileName);

			// Download specified file
			/*********************************************************************/
			if ((downloadHandle = fopen(filePath,"w")) == NULL)
			{
				fprintf(stderr,"Download failed critical error\n");
				exit(1);
			}

			download(sockfd,downloadHandle);

			fclose(downloadHandle);
			/*********************************************************************/

			// Check downloaded file for corruption and update HTML
			/*********************************************************************/
			downloadHASH = buildHASH(filePath); // Get MD5 hash for newly downloaded image
			csvHASH = getChecksum(csvHandle,image); // Get MD5 hash from CSV file for newly downloaded image
			isCorrupted = !(strcmp(downloadHASH,csvHASH)); // Compare MD5 hashes
			if (!isCorrupted)
			{
				printf("%s may have been corrupted during download\n",fileName);
			}
			else
			{
				printf("%s was downloaded successfully\n",fileName);
			}
			if(updateHTML(htmlPath,filePath,fileName,isCorrupted))
			{
				fprintf(stderr,"Failed to properly update HTML with %s",fileName);
			}
			/*********************************************************************/

			// iteration clean up

			free(downloadHASH);
			free(csvHASH);
			free(filePath);
			free(fileName);
		}
		else // Server could not find requested file
		{
			printf("Requested file not found\n");
		}
	}

	return 0;
}
