/* Container for image files found in directory */

typedef struct
{
   char* filePath;
   char* type;
   int size;
} fileInfo;

/* fileInfo:
 *
 * A fileInfo structure is used to hold the file path of an image file, the type
 * of the file (png, gif, or tif), and the size of the  image file
 */

void init_fileInfo(fileInfo* new,char* fileName,char* type,int size); // Constructor of fileInfo structure

void destroy_fileInfo(fileInfo* old); //Deconstructor for image file structure

typedef struct
{
   fileInfo* list; // Array of structs
   int length; // Total size of array
   int nextFree;
} fileList;

/* fileList:
 *
 * A fileList is a structure that acts as an array of fileInfo structs that can
 * change its size dynamically and is mechanically similar to Vectors in C++.
 *
 * When a new fileInfo structure is added to a fileList, the array will have
 * its size doubled if the array is already full.
 */

void init_fileList(fileList* newFile,int length); // Constructor

void destroy_fileList(fileList* fileInfoList); // Deconstructor

fileInfo* addFile(fileList* fileInfoList,char* newFileName,char* newType,int newFileSize);

/* fileInfo* addFile(fileList* fileInfoList,char* newFileName,char* newType,int newFileSize);
 *
 * addFile takes a pointer to a fileList structure, a file path, the type of the file
 * and the files size as arguments. addFile then creates a fileInfo struct and
 * adds it to the specified fileList structure.
 *
 * If the fileList structure passed to addFile is full, addFile will allocate more
 * space to accommodate it.
 *
 * addFile returns a poiner to the newly created fileInfo structure.
 */

fileInfo* getFile(fileList* fileInfoList,int idx);

/* fileInfo* getFile(fileList* fileInfoList,int idx);
 *
 * getFile takes a pointer to a fileList structure and an index as arguments
 * and returns a pointer to the fileInfo structure at index idx.
 */
