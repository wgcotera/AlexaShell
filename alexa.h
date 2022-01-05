/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

/****************************** MACROS ******************************/
#define MAXLETTERS      1000
#define MAXCOMMANDS     100

#define COLOR_PURPLE    "\x1b[35m"
#define COLOR_GREEN     "\x1b[32m"
#define COLOR_CYAN      "\x1b[36m"
#define COLOR_YELLOW    "\x1b[33m"
#define COLOR_RED       "\x1b[31m"
#define resetColor()    printf("\x1b[0m")

/*********************** FUNCTION DECLARATIONS **********************/
void    alexa_help();                                               // show the options of alexa
void    alexa_init();                                               // show a start message
void    alexa_printError(char * string);                            // print error messages
void    alexa_dir();                                                // show current directory  
int     alexa_input(char *str);                                     // take input
void    alexa_execute(char **parsed);                               // where the system command is executed
void    alexa_executePiped(char **parsed, char **parsedPiped);      // where the piped system commands is executed
int     alexa_cmdHandler(char **parsed);                            // execute built-in commands
void    alexa(char inputString[MAXLETTERS], char *parsedArgs[MAXCOMMANDS], char *parsedArgsPiped[MAXCOMMANDS]);

int     parse_pipe(char *str, char **strpiped);                     // find pipe
void    parse_space(char *str, char **parsed);                      // parsing command words
int     parse_string(char *str, char **parsed, char **parsedpipe);  // alexa :)