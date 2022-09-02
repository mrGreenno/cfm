#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AC_RED     "\x1b[31;1m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34;1m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

#define APP_VERSION "1.0"

void helpCommand (void);
void versionCommand (void);

const char * const allowedFlags[] = { "--help", "--version", "--set-numbers" };
void (*commandsStore [])(void) = { helpCommand, versionCommand };

const unsigned int allowedFlagsLength = sizeof allowedFlags / sizeof *allowedFlags;

unsigned int validateFlags (const unsigned int length, const char * const flags []) {

        int incorrectFlagIndex = -1;
        unsigned int rtrn = 0;

        for (int i = 1; i < length; i++) {

                for (int j = 0; j < allowedFlagsLength; j++) {

                        if (strcmp(*(flags + i), *(allowedFlags + j)) == 0) {

                                if (strcmp (*(flags + i), "--set-numbers") == 0) rtrn = 1; 

                                incorrectFlagIndex = -1;
                                break;
                        }

                        else incorrectFlagIndex = i;
		}

                if (incorrectFlagIndex != -1) {

                        printf (AC_RED "\n\n[-] Error: %s incorrect flag was given!%s\n\n", flags [incorrectFlagIndex], AC_RESET);
                        exit (EXIT_FAILURE);
		}

                else incorrectFlagIndex = -1; 
        }

        return rtrn;
}

void invokeCommands (const unsigned int count, const char * const commands []) {

        for (int i = 1; i < count; i++) {
                
                for (int j = 0; j < allowedFlagsLength; j++) {
        
                        if (strcmp(*(commands + i), *(allowedFlags + j)) == 0 && strcmp (*(commands + i), "--set-numbers") != 0) commandsStore [j] ();
                }
        }
    
    return;
}

void helpCommand (void) {

        printf (AC_BLUE "\n\n Help Command Invoked.\n\n" AC_RESET);
    
        printf (" CFM is a custom file manager.\n\n");
        printf (" Flags:\n\n");

        printf (" --help   %50s\n", "displays a description of the program.");
        printf (" --version%41s\n\n", "displays the program version.");
        printf (" --set-numbers%37s\n\n", "activates line numbering.");

        printf (" Control:\n\n");

        printf (" \'q\'%23s\n\n", "quit.");

        printf (" \'k\', \'j\'%23s\n\n", "up & down.");

        printf (" \'e\'%24s\n\n", "enter.");

        printf (" \'r\'%26s\n\n", "refresh.");

        printf (" \'1\'%32s\n\n", "input address.");

        printf (" \'2\'%33s\n\n", "make directory.");

        printf (" \'3\'%39s\n\n", "search files/folders.");

        printf (" \'4\'%38s\n\n", "displacement change.");

        printf (" \'5\'%38s\n\n", "move directory/file.");

        exit (EXIT_SUCCESS);

        return;
}

void versionCommand (void) {

        printf (AC_BLUE "\n\n App Version: %s%s\n\n", APP_VERSION, AC_RESET);
    
        exit (EXIT_SUCCESS);

        return;
}
