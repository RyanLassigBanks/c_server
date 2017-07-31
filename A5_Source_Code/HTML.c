#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HTML.h"
#include "md5sum.h"

int htmlStart(char* html)
{
   FILE* handle;
   if ((handle = fopen(html,"w")) == NULL)
   {
      return 1;
   }

   fprintf(handle,"<html>\n");
   fprintf(handle,"<head>\n");
   fprintf(handle,"<title>Downloaded images</title>\n");
   fprintf(handle,"</head>\n");
   fprintf(handle,"<body>\n");
   fprintf(handle,"<h1> Downloaded images</h1>\n");
   fprintf(handle,"<pre>\n");

   if (fclose(handle))
   {
      return 1;
   }

   return 0;
}

int htmlEnd(char* html)
{
   FILE* handle;
   if ((handle = fopen(html,"a")) == NULL)
   {
      return 1;
   }

   fprintf(handle,"</pre>\n");
   fprintf(handle,"</body>\n");
   fprintf(handle,"</html>\n");

   if (fclose(handle))
   {
      return 1;
   }

   return 0;
}

int updateHTML(char* html,char* filePath,char* fileName,int flag)
{
   FILE* handle;
   if ((handle = fopen(html,"a")) == NULL)
   {
      return 1;
   }

   if (flag)
   {
      fprintf(handle,"(Checksum match!) <a href='%s'>%s</a><br />",filePath,fileName);
   }
   else
   {
      fprintf(handle,"(Checksum mismatch!) %s</a><br />",fileName);
   }

   if (fclose(handle))
   {
      return 1;
   }

   return 0;
}

// return value needs to be freed when done being used

char* buildHASH(char* filePath)
{
   char* ret = malloc((MD5_DIGEST_LENGTH*2)+1);
   unsigned char sum[MD5_DIGEST_LENGTH];
   md5sum(filePath,sum);

   for (int i = 0;i < MD5_DIGEST_LENGTH;i++)
   {
      sprintf(ret+(i*2),"%02x",sum[i]);
   }
   return ret;
}
