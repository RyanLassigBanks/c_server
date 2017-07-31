#include <stdlib.h>
#include <string.h>
#include "image_container.h"

/* fileInfo implentation
 */

void init_fileInfo(fileInfo* new,char* fileName,char* type,int size)
{
   new->filePath = malloc(strlen(fileName)+1);
   strcpy(new->filePath,fileName);

   new->type = malloc(4);
   strcpy(new->type,type);

   new->size = size;
}

void destroy_fileInfo(fileInfo* old)
{
   free(old->filePath);
   free(old->type);
   old->size = 0;
}

/* fileList Implementation
 */

void init_fileList(fileList* newFile,int length)
{
   newFile->list = malloc(sizeof(fileInfo)*length);
   newFile->length = length;
   newFile->nextFree = 0;
}

void destroy_fileList(fileList* fileInfoList)
{
   for (int i = 0;i < fileInfoList->nextFree;i++)
   {
      destroy_fileInfo(&(fileInfoList->list[i]));
   }
   free(fileInfoList->list);
   fileInfoList->length = 0;
   fileInfoList->nextFree = 0;
}

fileInfo* addFile(fileList* fileInfoList,char* newFileName,char* newType,int newFileSize)
{
   if (fileInfoList->nextFree >= fileInfoList->length)
   {
      fileInfoList->length = 2*(fileInfoList->length) + 1;
      fileInfoList->list = realloc(fileInfoList->list,sizeof(fileInfo)*fileInfoList->length);
   }
   init_fileInfo(&(fileInfoList->list[fileInfoList->nextFree]),newFileName,newType,newFileSize);
   return &(fileInfoList->list[fileInfoList->nextFree++]);
}

fileInfo* getFile(fileList* fileInfoList,int idx)
{
   return &(fileInfoList->list[idx]);
}
