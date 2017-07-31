/* CSV manipulation functions */

/* void create_CSV(FILE* csv_handle,char* directory,fileList* imageList)
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

void create_CSV(FILE* csv_handle,char* directory,fileList* imageList);

/* getChecksum(FILE* csv,int line)
 *
 * getChecksum takes a FILE structure that points to a csv file created by
 * create_CSV and a line number and returns the checksum value for the image
 * at the specified line number.
 */

 char* getChecksum(FILE* csv,int line);

 /* char* getFileName(FILE* csv,int line)
  *
  * getFileName takes a FILE structure that points to a csv file created by
  * create_CSV and a line number and returns the name of the image file at the
  * specified line number.
  */

  char* getFileName(FILE* csv,int line);

 /* void displayCSV(FILE* csv)
  *
  * displayCSV takes a FILE structure that points to a csv file created by
  * create_CSV and prints its contents to stdout.
  */

  void displayCSV(FILE* csv);

  /* int getFileIndex(FILE* csv,char* fileType)
   *
   * getFile takes a FILE structure that points to a CSV file created by
   * create_CSV and a file extension as arguments. getFile interates through
   * the CSV file until it finds an image file that has the specified extension
   * and returns the line number at which the file is located.
   *
   * If no file with the specfied extension is found, getFile returns 0.
   *
   * Note: getFile does not reset the file stream.
   */

   int getFileIndex(FILE* csv,char* fileType);
