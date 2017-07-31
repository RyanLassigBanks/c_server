/* Functions for uploading and downloading files*/

int upload(int clientSocket,FILE* file);

/* int upload(int clientSocket,FILE* file);
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

int download(int socket,FILE* file);

/* int download(int socket,FILE* file);
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

void error(char* msg); // WORKS CITED: error was from recittion 13

/* void error(char* msg);
 *
 * error takes a string as an argument and calls perror and causes
 * the calling process to terminate.
 */
