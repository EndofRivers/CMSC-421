#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>

#include "utils.h"

void shell_loop(bool file, FILE *fp);
char *shell_read_line(bool file, FILE *fp, bool *exitp);
char **shell_split(char*);
int shell_exe(char **args, char *finalLine, char *line, FILE *fp);

// Define buffer sizes/token delims
#define BUFFER_SIZE 1024
#define TOKEN_BUFSIZE 64
#define TOKEN_DELIM " "

int main(int argc, char *argv[]) {

  FILE *fp; // file pointer

  if (argc == 1) {
    // promt user to run commands
    fp = fopen(argv[0], "r");
    shell_loop(false, fp);
    fclose(fp);
  }// if
  else if (argc == 2) {
    // read in file, run the commands in that order
    fp = fopen(argv[1], "r");
    shell_loop(true, fp);
    fclose(fp);
  }// else if
  else if (argc > 2) {
    fprintf(stderr, "Must be run with 0 or 1 arguments\n");
    exit(EXIT_FAILURE);
  }// else if
  else {}
  
  // exit(EXIT_SUCCESS);
  return 0;
}// main method

/**
 * Function Name: shell_loop
 * Reads in lines, parses them, executes commands and loops
 * 
 **/
void shell_loop(bool file, FILE *fp) {
  char *line; // line read in by user
  char *finalLine; // line after escape sequences
  char **args; // list of args
  int status; // return of the execute
  bool loopCheck = false;; // boolean to control loop
  bool fileExit = false; // boolean to control loop
  bool *fileExitP = &fileExit; // ptr to fileExit, passing by Ref

  do{
    // If asking for commands or running script
    if(!file) {
      printf("simple_shell>> ");
      line = shell_read_line(file, fp, fileExitP);
    }// if
    else {
      line = shell_read_line(file, fp, fileExitP);
    }// else

    // exit checking
    if(strcmp(line, "exit\0") == 0 || fileExit == true)
      loopCheck = 1;
    else {
      // escape sequences
      finalLine = unescape(line, fp);
      // split into tokens
      args = shell_split(finalLine);
      // execute and return success/fail
      status = shell_exe(args, finalLine, line, fp);
      // free finalLine given from unescape
      free(finalLine);
	    

      if(status == 1) {
	if(file) {
	  printf("Fix Error and Run again. Exiting...\n");
	  free(args);
	  free(line);
	  fclose(fp);
	  exit(EXIT_FAILURE);
	}
	else
	  printf("Incorrect Command. ls /bin for commands.\n");
      } 
    }// else

    free(line);
    // if we allocated args, free
    if(loopCheck == 0)
      free(args);
  }while(loopCheck == 0);
}// shell_loop


/**
 * Function name: shell_read_line
 * Reads in line from the user until EOF or newline character
 *
 **/
char *shell_read_line(bool file, FILE *fp, bool *fileExitP) {
  int bufsize = BUFFER_SIZE; // size of current buffer
  int pos = 0; // pos in array
  char *buffer = malloc(sizeof(char) * bufsize); // allocated buffer
  int c; // current character

  while(1) {

    // IF NOT FILE, So if no file/script was passed read input
    if(!file) {
      c = getchar();
      if(c == EOF || c == '\n') {
	buffer[pos] = '\0';
	return buffer;
      }// if
      else {
	buffer[pos] = c;
      }// else
    }// if
    // IF FILE, So if a file/script was passed
    else {
      c = fgetc(fp);
      if(c == EOF || c == '\n') {
	buffer[pos] = '\0';
	if(c == EOF)
	  *fileExitP = true;
	return buffer;
      }
      else {
	buffer[pos] = c;
      }
    }
    pos++;

    // Increase buffer size if needed
    if(pos >= bufsize) {
      bufsize += BUFFER_SIZE;
      buffer = realloc(buffer, bufsize);
    }// if
  }// while
}// shell_read_line

/**
 * Function name: shell_split
 * Splits the line given from user into tokens ready for execution
 * 
 **/
char **shell_split(char *line) {
  int bufsize = TOKEN_BUFSIZE; // local variable of token buffer size
  int pos = 0; // position in token array
  char **tokens = malloc(bufsize * sizeof(char*)); // array of tokens
  char *token; // current token

  // get a token from line, delimited by defined delimiters
  token = strtok(line, TOKEN_DELIM);

  // while we have tokens to get
  while(token != NULL) {
    // insert token into the token array and increment pos
    tokens[pos] = token;
    pos++;

    // if we have exceeded the buffer size, create more space
    if(pos >= bufsize) {
      bufsize += TOKEN_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
    }// if

    //get next token
    token = strtok(NULL, TOKEN_DELIM);
  }//while

  //put null character at end of array and return
  tokens[pos] = NULL;
  return tokens;
}//shell_split

/**
 * Function name: shell_exe
 * Executes args created from shell_splot
 *
 **/
int shell_exe(char **args, char *finalLine, char *line, FILE *fp) {
  if(args[0] == NULL) {
    printf("Empty Args.\n");
    return 1;
  }

  pid_t pid = fork();
  int status = 0;

  if(pid == -1) {
    printf("Failed to Fork.\n");
    return 1;
  }// if
  else if(pid > 0) {
    // Parent
    waitpid(pid, &status, 0);
    // IF ABORTED OR NOT, Abort returns 6
    if(status == 6)
      return 1;
    else
      return 0;
  }// else if
  else {
    execvp(args[0], args);
    printf("Child failed to exe.\n");
    free(args);
    free(finalLine);
    free(line);
    fclose(fp);
    abort();
  }// else
}// shell_exe
 
