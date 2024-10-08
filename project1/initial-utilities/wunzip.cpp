/*      wunzip // wzip
    The next tools you will build come in a pair, because one (wzip) is a file compression tool, and the other (wunzip) is a file decompression tool.
    The type of compression used here is a simple form of compression called run-length encoding (RLE). 
        RLE is quite simple: when you encounter n characters of the same type in a row, 
        the compression tool (wzip) will turn that into the number n and a single instance of the character.
    Thus, if we had a file with the following contents:
        aaaaaaaaaabbbb
    the tool would turn it (logically) into:
        10a4b
    However, the exact format of the compressed file is quite important; here, you will write out a 4-byte integer in binary format followed by the single character in ASCII. 
    Thus, a compressed file will consist of some number of 5-byte entries, each of which is comprised of a 4-byte integer (the run length) and the single character.

    To write out an integer in binary format (not ASCII), you should use write(). Read the man page for more details. 
        For wzip, all output should be written to standard output (the STDOUT_FILENO file descriptor, which, is already open when the program starts running).

    Note that typical usage of the wzip tool would thus use shell redirection in order to write the compressed output to a file. 
        For example, to compress the file file.txt into a (hopefully smaller) file.z, you would type:
            prompt> ./wzip file.txt > file.z
    The "greater than" sign is a UNIX shell redirection; in this case, it ensures that the output from wzip is written to the file file.z (instead of being printed to the screen). 

    The wunzip tool simply does the reverse of the wzip tool, taking in a compressed file and writing (to standard output again) the uncompressed results. 
        For example, to see the contents of file.txt, you would type:
            prompt> ./wunzip file.z
    wunzip should read in the compressed file (using read()) and print out the uncompressed output to standard output using write().

    * Correct invocation should pass one or more files via the command line to the program; 
        if no files are specified, the program should exit with return code 1 and print "wzip: file1 [file2 ...]" (followed by a newline) 
            or "wunzip: file1 [file2 ...]" (followed by a newline) for wzip and wunzip respectively.
    * The format of the compressed file must match the description above exactly (a 4-byte integer followed by a character for each run).
    * Do note that if multiple files are passed to *wzip, they are compressed into a single compressed output, 
        and when unzipped, will turn into a single uncompressed stream of text (thus, the information that multiple files were originally input into wzip is lost). 
        The same thing holds for wunzip.
*/

/*  wunzip - file decompression
10a4b [BINARY] -> aaaaaaaaaabbbb
*/
#include <iostream>     // input/output stream

#include <stdlib.h>     // std library

#include <unistd.h>     // read, write, close
#include <fcntl.h>      // open

#include <sys/types.h>  // open
#include <sys/stat.h>   // open

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {            // no cmd line arguments -- print correct input and return 1
        string error = "wunzip: file1 [file2 ...]\n";
        write(STDOUT_FILENO, error.c_str(), error.length());    // write error to std output
        exit(1);  
    }
    else if (argc >= 2) {       // one or more files specified
        int fileDescriptor;
        int ret;
        
        // read one file at a time, going through all the files listed
        for (int i = 1; i < argc; i ++){
            // open file 
            fileDescriptor = open(argv[i], O_RDONLY); 
            
            // error - bad fileDescriptor
            if (fileDescriptor == -1) {
                string error = "wunzip: cannot open file\n";
                write(STDOUT_FILENO, error.c_str(), error.length());    // write error to std output
                exit(1);
            }
            int32_t num;    // 4 bytes = 32 bits, int is not always 4 bytes on every OS
            char c;

            // read file -- 5 bytes at a time
            while ((ret = read(fileDescriptor, &num, sizeof(num))) > 0) {   // if there is a number, there will be a char to go with it
                read(fileDescriptor, &c, 1);
                for (int i = 0; i < num; i++)
                    write(STDOUT_FILENO, &c, 1);
            }

            // close file when done reading/printing
            close(fileDescriptor);
        }
        return 0;   // successful after outputting all files
    }
    exit(1);
}

