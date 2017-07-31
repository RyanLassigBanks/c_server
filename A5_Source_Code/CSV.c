#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "image_container.h"
#include "md5sum.h"
#include "CSV.h"
#include <errno.h>

void create_CSV(FILE* csv_handle,char* directory,fileList* imageList)
{
	struct dirent* dirHandle;
	DIR* direct;
	char* fileExtensions;
	struct stat size;
	char* filepath;
	if ((direct = opendir(directory))==NULL)
	{
		fprintf(stderr, "Failed to open directory\n");
		exit(1);
	}

	while ((dirHandle = readdir(direct))!= NULL)
	{
      if (!strcmp(dirHandle->d_name,".") || !strcmp(dirHandle->d_name,".."))
		{
			continue;
		}

		unsigned char sum[MD5_DIGEST_LENGTH];
		fileExtensions = strrchr((char*) dirHandle->d_name,'.');

      /******************** Build File Path ***********************************/
      filepath=malloc(strlen((const char*) dirHandle->d_name)+strlen(directory)+3);
      sprintf(filepath,"%s/%s",directory,(const char*) dirHandle->d_name);
      /************************************************************************/

      if (dirHandle->d_type == DT_DIR)
      {
         create_CSV(csv_handle,filepath,imageList);
      }

		else if (!strcmp(".gif",fileExtensions)||!strcmp(".tiff",fileExtensions)||!strcmp(".png",fileExtensions))
		{
			if (stat(filepath,&size) == -1)
			{
				perror("\n");
				fprintf(stderr,"stat failed to get file info\n");
				exit(1);
			}

         addFile(imageList,filepath,++fileExtensions,size.st_size);
         /******************** Update csv **********************************/
			md5sum(filepath, sum);
			int i;
			fprintf(csv_handle,"%s,%zd,",(const char*) dirHandle->d_name, size.st_size);
			for (i = 0; i < MD5_DIGEST_LENGTH; i++)
			{
				fprintf(csv_handle,"%02x", sum[i]);
			}
			fprintf(csv_handle,"\n");
         /******************************************************************/
		}

      free(filepath);
	}
}

char* getChecksum(FILE* csv,int line) // return value needs to be freed when done being used
{
   char* linePtr = NULL;
   char* ret;
   char* temp;
   size_t n = 0;

   getline(&linePtr,&n,csv); // remove header (filename,size,checksum)

   for(int i = 1;i <= line;i++)
   {
      getline(&linePtr,&n,csv);
   }
   temp = strrchr(linePtr,',');
	temp[strlen(temp)-1] = '\0'; // remove new line character
   temp++;
   ret = malloc(strlen(temp)+1);
   strcpy(ret,temp);
   free(linePtr);
   fseek(csv,0,SEEK_SET); //reset file stream to begining of file
   return ret;
}

char* getFileName(FILE* csv,int line) // return value needs to freed when done being used
{
   char* linePtr = NULL;
	char* ret;
	char* temp;
   size_t n = 0;

   getline(&linePtr,&n,csv); // remove header (filename,size,checksum)

   for(int i = 1;i < line;i++)
   {
      getline(&linePtr,&n,csv);
   }

   getdelim(&linePtr,&n,',',csv);

   linePtr[strlen(linePtr)-1] = '\0';
	temp = linePtr;
	ret = malloc(strlen(temp)+1);
	strcpy(ret,temp);
   fseek(csv,0,SEEK_SET); //reset file stream to begining of file
	free(linePtr);
   return ret;
}

void displayCSV(FILE* csv)
{
   char* linePtr = NULL;
   size_t n = 0;
   int count = 1;

   getline(&linePtr,&n,csv); // remove header (filename,size,checksum)

   printf("\n==============================\n");
   printf("Dumping contents of catalog.csv\n");

   while(getdelim(&linePtr,&n,',',csv) != -1)
   {
      linePtr[strlen(linePtr)-1] = '\0';
      printf("[%d] %s\n",count,linePtr);
      count++;
      getline(&linePtr,&n,csv);
   }
   free(linePtr);
   printf("\n==============================\n");
   fseek(csv,0,SEEK_SET); //reset file stream to begining of file
}

int getFileIndex(FILE* csv,char* fileType)
{
	char* linePtr = NULL;
	char* extension;
	size_t n = 0;
	static int count = 0;

	getline(&linePtr,&n,csv); // remove header (filename,size,checksum)

	while(getdelim(&linePtr,&n,',',csv) != -1)
	{
		count ++;
	   linePtr[strlen(linePtr)-1] = '\0';
		extension = strrchr(linePtr,'.');
		if (!strcmp(fileType,(extension+1)))
		{
			free(linePtr);
			return count;
		}
		getline(&linePtr,&n,csv);
	}
	free(linePtr);
	return 0;
}
