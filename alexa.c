
/*************************** HEADER FILES ***************************/
#include "alexa.h"

/********************** FUNCTION IMPLEMENTATIONS ********************/

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa_help() {
    printf(COLOR_YELLOW"\n-------------------WELCOME TO ALEXA'S HELP-------------------\n");
	resetColor();
	printf("\nOptions:\n");
	printf(" \tHELP\t\t\tDisplay this message\n");
	printf(" \tQUIT\t\t\tEnds the execution \n");
    return;
}

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa_init() {

    resetColor();
    printf(COLOR_PURPLE"\n");
    printf("         :::     :::        :::::::::: :::    :::     :::      \n");
    printf("       :+: :+:   :+:        :+:        :+:    :+:   :+: :+:    \n");
    printf("      +:+   +:+  +:+        +:+         +:+  +:+   +:+   +:+   \n");
    printf("     +#++:++#++: +#+        +#++:++#     +#++:+   +#++:++#++:  \n");sleep(1);
    printf("     +#+     +#+ +#+        +#+         +#+  +#+  +#+     +#+  \n");
    printf("     #+#     #+# #+#        #+#        #+#    #+# #+#     #+#  \n");
    printf("     ###     ### ########## ########## ###    ### ###     ###  \n");sleep(1);

    printf(COLOR_YELLOW"\n--------------WELCOME TO ALEXA'S SHELL %s--------------\n", getenv("USER"));sleep(1);

    resetColor();    
}

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa_printError(char * string) {
    fprintf(stderr, COLOR_RED"\n%s\n", string);
    resetColor();
}

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa_dir() {

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf(COLOR_YELLOW"\n%s: " COLOR_CYAN"%s", getenv("USER"), cwd);
    resetColor();
    printf("$ ");

}

/*-----------------------------------------------------
-----------------------------------------------------*/

int alexa_input(char *str) {

    char *buffer;
    buffer = readline(">> ");
    if(strlen(buffer) != 0) {
        add_history(buffer);
        strcpy(str, buffer);
        return 0;
    }    
    else {

        return 1;
    }
}

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa_execute(char **parsed) {

    pid_t pid = fork();

    if(pid == -1) {

        alexa_printError("...Failed forking child...");
        return;
    } 

    else if(pid == 0) {

        if(execvp(parsed[0], parsed) < 0) {

            alexa_printError("...Couldn't execute command...");
        }

        exit(0);
    }

    else {

        wait(NULL);
        return;
    }
}

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa_executePiped(char **parsed, char **parsedPiped) {
    
    int pfd[2];
    pid_t p1, p2;

    if(pipe(pfd) < 0) {
        alexa_printError("...Pipe couldn't be initialized...");
        return;
    }

    p1 = fork();

    if(p1 < 0) {
        alexa_printError("...Couldn't fork...");
        return;
    }

    if(p1 == 0) {

        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[1]);

        if(execvp(parsed[0], parsed) < 0) {
            alexa_printError("...Couldn't execute command 1...");
            exit(0);
        }
    } 
    else {

        p2 = fork();

        if(p2 < 0) {

            alexa_printError("...Couldn't fork...");
            return;
        }

        if(p2 == 0) {

            close(pfd[1]);
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[0]);

            if(execvp(parsedPiped[0], parsedPiped) < 0) {
                alexa_printError("...Couldn't execute command 2...");
                exit(0);
            }
        }
        else {
            wait(NULL);
            wait(NULL);
        }
    }
}

/*-----------------------------------------------------
-----------------------------------------------------*/

int alexa_cmdHandler(char **parsed) {

    int commands = 3, i, builtInArg = 0;
    char *ownComands[commands];
    char *username;

    ownComands[0] = "QUIT";
    ownComands[1] = "HELP";
    ownComands[2] = "cd";
    

    for(i = 0 ; i < commands ; i++) {

        if(strcmp(parsed[0], ownComands[i]) == 0) {
            
            builtInArg = i + 1;
            break;
        }
    }

    switch (builtInArg)
    {

    case 1:
        printf(COLOR_PURPLE"\n");       
        printf("\t\t:::::::::  :::   ::: :::::::::: :::   \n");
        printf("\t\t:+:    :+: :+:   :+: :+:        :+:   \n"); 
        printf("\t\t+:+    +:+  +:+ +:+  +:+        +:+   \n"); 
        printf("\t\t+#++:++#+    +#++:   +#++:++#   +#+   \n");sleep(1); 
        printf("\t\t+#+    +#+    +#+    +#+        +#+   \n"); 
        printf("\t\t#+#    #+#    #+#    #+#              \n"); 
        printf("\t\t#########     ###    ########## ###   \n\n");sleep(1);
        exit(0);
        break;

    case 2:
        alexa_help();
        return 1;

    case 3:
        chdir(parsed[1]);
        return 1;
    
    default:
        break;
    }

    return 0;
}

/*-----------------------------------------------------
-----------------------------------------------------*/

int parse_pipe(char *str, char **strpiped) {

    int i;
    
    for(i = 0 ; i < 2 ; i++) {

        strpiped[i] = strsep(&str, "|");
        if(strpiped[i] == NULL) {
            break;
        }
    }

    if(strpiped[1] == NULL) {
        return 0; 
    }
    else {
        return 1;
    }

}

/*-----------------------------------------------------
-----------------------------------------------------*/

void parse_space(char *str, char **parsed) {
    
    int i;

    for(i = 0 ; i < MAXCOMMANDS ; i++) {

        parsed[i] = strsep(&str, " ");

        if(parsed[i] == NULL) {
            break;
        }

        if(strlen(parsed[i]) == 0) {
            i--;
        }
    }
}

/*-----------------------------------------------------
-----------------------------------------------------*/

int parse_string(char *str, char **parsed, char **parsedpipe) {

    char *strpiped[2];
    int piped = 0;

    piped = parse_pipe(str, strpiped);

    if(piped) {

        parse_space(strpiped[0], parsed);
        parse_space(strpiped[1], parsedpipe);
    } else {

        parse_space(str, parsed);
    }

    if(alexa_cmdHandler(parsed)) {
        return 0;
    } else {
        return 1 + piped;
    }
}

/*-----------------------------------------------------
-----------------------------------------------------*/

void alexa(char inputString[MAXLETTERS], char *parsedArgs[MAXCOMMANDS], char *parsedArgsPiped[MAXCOMMANDS]) {

    int eFlag = 0;
    alexa_init();
    
    while(1) {

        alexa_dir();

        if(alexa_input(inputString)) {
            continue;
        }

        eFlag = parse_string(inputString, parsedArgs, parsedArgsPiped);

        if(eFlag == 1) {
            alexa_execute(parsedArgs);
        }

        if(eFlag == 2) {
            alexa_executePiped(parsedArgs, parsedArgsPiped);
        }
    }
}