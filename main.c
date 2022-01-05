/*************************** HEADER FILES ***************************/
#include <getopt.h>
#include "alexa.h"

/********************** FUNCTION IMPLEMENTATIONS ********************/
/*-----------------------------------------------------
-----------------------------------------------------*/

void print_help(char *command) {
    printf(COLOR_YELLOW"\n-------------------ALEXA'S SHELL-------------------\n");
    
    printf("\nALEXA'S SHELL ");
    resetColor();
    printf("allows the execution of any program \nor command on the machine, including arguments.\n");
    printf("\nUse:\n \t%s [-h]\n\n", command);
	printf("Options:\n");
	printf("\t-h\tHelp, display this message.\n\n");
    printf("Build-In Commands:\n");
    printf("\tHELP\tDisplay a help message.\n");
	printf("\tQUIT\tEnds the execution.\n\n");
}

/*-----------------------------------------------------
-----------------------------------------------------*/

int main(int argc, char **argv) {

    int opt, index;

	while ((opt = getopt (argc, argv, "h")) != -1){
		switch(opt)
		{
			case 'h':

				print_help(argv[0]);
				return 0;

            default:

				fprintf(stderr, COLOR_RED"\nUse:\n");
				fprintf(stderr, "     %s -h\n", argv[0]);
				fprintf(stderr, "     %s \n\n", argv[0]);
                resetColor();
				return -1;

		}
	}

    if(argc == 1) {   
        char inputString[MAXLETTERS];     
        char *parsedArgs[MAXCOMMANDS], *parsedArgsPiped[MAXCOMMANDS];
        alexa(inputString, parsedArgs, parsedArgsPiped);
    }
}