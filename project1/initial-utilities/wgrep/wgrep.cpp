/* wgrep: similar to grep, **wgrep**, looks through a file, line by line, trying to find a
user-specified search term in the line. If a line has the word within it, the
line is printed out, otherwise it is not. 

    * Your program **wgrep** is always passed a search term and zero or
    more files to grep through (thus, more than one is possible). It should go
    through each line and see if the search term is in it; if so, the line
    should be printed, and if not, the line should be skipped.

    * The matching is case sensitive. Thus, if searching for **foo**, lines
    with **Foo** will *not* match.

    * Lines can be arbitrarily long (that is, you may see many many characters
    before you encounter a newline character, \\n). **wgrep** should work
    as expected even with very long lines. For this, you might want to write a
    function that reads from a file descriptor and buffers data until you reach
    a newline character.

    * If **wgrep** is passed no command-line arguments, it should print
    "wgrep: searchterm [file ...]" (followed by a newline) and exit with
    status 1.  

    * If **wgrep** encounters a file that it cannot open, it should print
    "wgrep: cannot open file" (followed by a newline) and exit with status 1. 

    * In all other cases, **wgrep** should exit with return code 0.

    * If a search term, but no file, is specified, **wgrep** should work,
    but instead of reading from a file, **wgrep** should read from
    *standard input*. Doing so is easy, because the file descriptor **STDIN_FILENO**
    is already open; you can use **read()** to read from it.

    * For simplicity, if passed the empty string as a search string, **wgrep**
    can either match NO lines or match ALL lines, both are acceptable.

*/

#include <iostream>     // input/output stream

#include <stdlib.h>     // std library

#include <unistd.h>     // read, write, close
#include <fcntl.h>      // open

#include <sys/types.h>  // open
#include <sys/stat.h>   // open

#include <vector>
#include <algorithm>    // find

using namespace std;

vector<string> bufferLines(int fileDescriptor); 
vector<string> splitLine(string line);

int main(int argc, char *argv[]) {
    if (argc == 1) {            // no cmd line arguments -- print correct input and return 1
        string error = "wgrep: searchterm [file ...]\n";
        write(STDOUT_FILENO, error.c_str(), error.length());    // write error to std output
        exit(1);  
    } else if (argc == 2) {     // search term provided but no files -- read from standard input
        int fileDescriptor = STDIN_FILENO;                      // standard input file descriptor

        vector<string> lines = bufferLines(fileDescriptor);

        for (auto line : lines) {
            //if (find(words.begin(), words.end(), argv[1]) != words.end()) {
            if (line.find(argv[1]) != string::npos) {
                write(STDOUT_FILENO, line.c_str(), line.length()); 
                write(STDOUT_FILENO, "\n", 1); 
            }
        }

        return 0;
    } else if (argc > 2) {     // at least one argument -- can be invoked on multiple files
        int fileDescriptor;
        
        // read one file at a time, going through all the files listed
        for (int i = 2; i < argc; i ++){
            // open file 
            fileDescriptor = open(argv[i], O_RDONLY); 
            
            // error - bad fileDescriptor
            if (fileDescriptor == -1) {
                string error = "wgrep: cannot open file\n";
                write(STDOUT_FILENO, error.c_str(), error.length());    // write error to std output
                exit(1);
            }

            vector<string> lines = bufferLines(fileDescriptor);
            for (auto line : lines) {
                if (line.find(argv[1]) != string::npos) {
                    write(STDOUT_FILENO, line.c_str(), line.length()); 
                    write(STDOUT_FILENO, "\n", 1);  // output was missing newlines
                }
            }

            // close file when done reading/printing
            close(fileDescriptor);
        }
        return 0; // successful after searching all files
        
    }
    exit(1); // error if it reaches this point
}

/*  bufferLines reads from a file descriptor and buffers data until you reach a newline character.*/ 
// input: fileDescriptor
// output: vector of strings containing each line
vector<string> bufferLines(int fileDescriptor)
{   
    char buffer[4096];
    int ret;
    vector<string> lines;
    string line;

    // read file via buffer
    while ((ret = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
        // loop through buffer
        for (int i = 0; i < ret; i++) {
            // if buffer[i] contains newline,
            if (buffer[i] == '\n') {    
                lines.push_back(line);  // store line in vector of lines
                line.clear();           // reset line for next time
            } else {
                line += buffer[i];      // keep reading chars into line
            }               
        }  
    } 

    // if there is anything left over, push back final line to vector of lines
    if (!line.empty())
        lines.push_back(line);
    return lines; 
 }
