/* Contains functions for building HTML file */

/* int htmlStart(char* html)
 *
 * htmlStart takes the path to an HTML file  as an argument and writes the code
 * for the first part of a simple html file to the file at the file path.
 *
 * htmlStart returns 0 is successful and 1 if unsuccessful.
 */

int htmlStart(char* html);

/* int htmlEnd(html)
 *
 * htmlEnd takes the path to an HTML file as an argument and writes the code for
 * the end of a simple html file to the file at the file path.
 *
 * htmlStart returns 0 is successful and 1 if unsuccessful.
 */

int htmlEnd(char* html);

/* int updateHTML(FILE* html,char* filePath,fileName,int flag)
 *
 * updateHTML takes the path to an HTML file, a path to an image file, a file
 * Name and an integer flag as arguments.
 *
 * If the flag is set to a non-zero integer, updateHTML will add a hyperlink to
 * the specififed path to the HTML file. If the flag is set to zero, updateHTML
 * will simply add text with the file name to the HTML file.
 *
 * htmlStart returns 0 is successful and 1 if unsuccessful.
 */

 int updateHTML(char* html,char* filePath,char* fileName,int flag);

 /* char* buildHASH(char* filePath)
  *
  * buildHASH takes  a file path as an argument and returns an MD5 hash code
  * for the file at the file path.
  */

  char* buildHASH(char* filePath);
