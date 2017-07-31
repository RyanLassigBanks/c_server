## Information
CSci 4061 Spring 2015 Assignment 5
Name1 = Ryan Lassig Banks
Name2 = Andrea Gunderson
StudentID1 = 4019816
StudentID2 = 4749993
Lab Machines: KH2170-05 and KH2170-06

SYNTAX AND USAGE POINTERS:

To Compile both server and client go into the 4019816_4749993 directory and
enter the following command:

<somePath>/<4019816_4749993>$ make

To run server enter the following command:

<somePath>/<4019816_4749993>$ ./server <port> <directory>

To run client enter the following command:

<somePath>/<4019816_4749993>$ ./client <ip_address> <port> <optional_extension_arguments>

FUNCTION DESCRIPTIONS:

The following are descriptions for the user created functions used
in the main functions for the server and client executables
(server.c and client.c).

/* void create_CSV(FILE* csv_handle,char* directory,fileList* imageList)
 *
 * Source code found in: CSV.c
 *
 * DESCRIPTION:
 *
 * create_CSV takes a FILE structure, directory path, and a fileList structure
 * as arguments. create_CSV parses the directory for all png, gif,and tiff files
 * and adds them to the csv file that csv_handle points to.
 *
 * Each line of the CSV file will contain a file's name, size, and checksum
 * value.
 *
 * create_CSV also adds the file path, file size, and file type
 * information for each png, gif, and tiff file found to the fileList struct
 * that imageList points to.
 */

/* getChecksum(FILE* csv,int line)
 *
 * Source code found in: CSV.c
 *
 * DESCRIPTION:
 *
 * getChecksum takes a FILE structure that points to a csv file created by
 * create_CSV and a line number and returns the checksum value for the image
 * at the specified line number.
 */

/* char* getFileName(FILE* csv,int line)
 *
 * Source code found in: CSV.c
 *
 * DESCRIPTION:
 *
 * getFileName takes a FILE structure that points to a csv file created by
 * create_CSV and a line number and returns the name of the image file at the
 * specified line number.
 */

/* int getFileIndex(FILE* csv,char* fileType)
 *
 * Source code found in: CSV.c
 *
 * DESCRIPTION:
 *
 * getFile takes a FILE structure that points to a CSV file created by
 * create_CSV and a file extension as arguments. getFile iterates through
 * the CSV file until it finds an image file that has the specified extension
 * and returns the line number at which the file is located.
 *
 * If no file with the specified extension is found, getFile returns 0.
 *
 * Note: getFile does not reset the file stream.
 */

/* void displayCSV(FILE* csv)
 *
 * Source code found in: CSV.c
 *
 * DESCRIPTION:
 *
 * displayCSV takes a FILE structure that points to a csv file created by
 * create_CSV and prints its contents to stdout.
 */

/* int htmlStart(char* html)
 *
 * Source code found in: HTML.c
 *
 * DESCRIPTION:
 *
 * htmlStart takes the path to an HTML file  as an argument and writes the code
 * for the first part of a simple html file to the file at the file path.
 *
 * htmlStart returns 0 is successful and 1 if unsuccessful.
 */

/* int htmlEnd(html)
 *
 * Source code found in: HTML.c
 *
 * DESCRIPTION:
 *
 * htmlEnd takes the path to an HTML file as an argument and writes the code for
 * the end of a simple html file to the file at the file path.
 *
 * htmlStart returns 0 is successful and 1 if unsuccessful.
 */

/* int updateHTML(FILE* html,char* filePath,fileName,int flag)
 *
 * Source code found in: HTML.c
 *
 * DESCRIPTION:
 *
 * updateHTML takes the path to an HTML file, a path to an image file, a file
 * Name and an integer flag as arguments.
 *
 * If the flag is set to a non-zero integer, updateHTML will add a hyperlink to
 * the specified path to the HTML file. If the flag is set to zero, updateHTML
 * will simply add text with the file name to the HTML file.
 *
 * htmlStart returns 0 is successful and 1 if unsuccessful.
 */

/* char* buildHASH(char* filePath)
 *
 * Source code found in: HTML.c
 *
 * DESCRIPTION:
 *
 * buildHASH takes  a file path as an argument and returns an MD5 hash code
 * for the file at the file path.
 */

/* fileInfo* addFile(fileList* fileInfoList,char* newFileName,char* newType,int newFileSize);
 *
 * Source code found in: image_container.c
 *
 * DESCRIPTION:
 *
 * addFile takes a pointer to a fileList structure, a file path, the type of the file
 * and the files size as arguments. addFile then creates a fileInfo struct and
 * adds it to the specified fileList structure.
 *
 * If the fileList structure passed to addFile is full, addFile will allocate more
 * space to accommodate it.
 *
 * addFile returns a pointer to the newly created fileInfo structure.
 */

/* fileInfo* getFile(fileList* fileInfoList,int idx)
 *
 * Source code found in: image_container.c
 *
 * DESCRIPTION:
 *
 * getFile takes a pointer to a fileList structure and an index as arguments
 * and returns a pointer to the fileInfo structure at index idx.
 */

/* int upload(int clientSocket,FILE* file);
 *
 * Source code found in: upload_download.c
 *
 * DESCRIPTION:
 *
 * upload takes a file descriptor for a socket and a FILE structure
 * pointing to an open file as arguments.
 *
 * The process that calls upload (the server) waits for a response from
 * a client proccess and once the response is received, it will send
 * 512 bytes from the open file to the client and then wait for
 * another response from the client. When the client sends another
 * response, the next 512 bytes from the open file will be sent to
 * the client. This process continues until the entire file has been
 * sent to the client process.
 *
 * upload returns 0 when successful. If upload fails it terminates the
 * server process.
 */

/* int download(int socket,FILE* file);
 *
 * Source code found in: upload_download.c
 *
 * DESCRIPTION:
 *
 * download takes a file descriptor for a socket and a FILE structure
 * pointing to an open file as arguments.
 *
 * The process that calls download (the client) sends a message to a
 * server process. When the server receives the message it sends
 * a 512 byte packet of data to the client, which the client writes
 * to an open file. This process continues until the server no longer
 * has data to send to the client. When this occurs the server sends
 * the client a termination message causing download to return 0.
 *
 * download returns 0 if all of the data from the server is downloaded
 * successfully. If download fails it terminates the client process.
 */

/* void error(char* msg);
 *
 * Source code found in: upload_download.c
 *
 * DESCRIPTION
 *
 * error takes a string as an argument and calls perror and causes
 * the calling process to terminate.
 */
