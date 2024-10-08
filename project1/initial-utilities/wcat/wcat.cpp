/* wcat: similar to cat, **wcat** reads the file **main.cpp** and prints out its contents. 
    
    * Your program **wcat** can be invoked with one or more files on the command
    line; it should just print out each file in turn. 

    * In all non-error cases, **wcat** should exit with status code 0, usually by
    returning a 0 from **main()** (or by calling **exit(0)**).

    * If *no files* are specified on the command line, **wcat** should just exit
    and return 0. Note that this is slightly different than the behavior of 
    normal UNIX **cat** (if you'd like to, figure out the difference).

    * If the program tries to **open()** a file and fails, it should print the
    exact message "wcat: cannot open file" (followed by a newline) and exit
    with status code 1.  If multiple files are specified on the command line,
    the files should be printed out in order until the end of the file list is
    reached or an error opening a file is reached (at which point the error
    message is printed and **wcat** exits)
    
*/

#include <iostream>     // input/output stream

#include <stdlib.h>     // std library

#include <unistd.h>     // read, write, close
#include <fcntl.h>      // open

#include <sys/types.h>  // open
#include <sys/stat.h>   // open

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {            // no arguments -- no files to cat & output to screen
        return 0;                   // usually cat waits for standard input if there are no arguments
    } else if (argc >= 2) {     // at least one argument -- can be invoked on multiple files
        int fileDescriptor;
        char buffer[4096];
        int ret;
        //stringstream stringOut;
        
        // read one file at a time, going through all the files listed
        for (int i = 1; i < argc; i ++){
            // open file 
            fileDescriptor = open(argv[i], O_RDONLY); 
            
            // error - bad fileDescriptor
            if (fileDescriptor == -1) {
                //cerr << "wcat: cannot open file" << endl;
                string error = "wcat: cannot open file\n";
                write(STDOUT_FILENO, error.c_str(), error.length());    // write error to std output
                exit(1);
            }

            // read file via buffer
            while ((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
                // write buffer to standard output
                write(STDOUT_FILENO, buffer, ret);
            }

            // close file when done reading/printing
            close(fileDescriptor);
        }
        return 0;   // successful after outputting all files
    }
    // else
    exit(1);    // error if it reaches this point
}
