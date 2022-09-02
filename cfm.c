#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ncurses.h>
#include "validator.h"

#define MAX_NUMBER_ELEMENTS 888
#define MAX_LENGTH_NAME 256
#define MAX_LENGTH_ADDRESS_BAR 1000 

char addressBar [MAX_LENGTH_ADDRESS_BAR] = "/home";

int j_2 = 1;

unsigned int findString (const char array [] [MAX_LENGTH_NAME], const unsigned int sizeArray, const char * const name) {
	
	for (int i = 0; i < sizeArray; i++) if (strcmp (*(array + i), name) == 0) return 0;

	return 1;
}

unsigned int search (const char OBjects [] [MAX_LENGTH_NAME], const unsigned int sizeOBjects, int* chc ) {

        clear ();

        echo ();
        curs_set (1);

        char obt [MAX_LENGTH_NAME] = "\0";
        attron (A_BOLD);
        
        printw ("\n%s/", addressBar);       
        scanw ("%s", obt);

        for (int i = 0; i < sizeOBjects; i++) {

                if (strcmp (*(OBjects + i), obt) == 0) {

                        attroff (A_BOLD);
                   
                        noecho ();
                        curs_set (0);

                        *chc = i;

                        return 0;
                }
        }

        noecho ();
        curs_set (0);

        attroff (A_BOLD);

        return 1;
}

int moveDir (const char name1 [], const char name2 []) {

	DIR* dr = NULL;
	if ((dr = opendir (name2)) == NULL) {

		clear ();
		addch ('\n');

		printw ("[*] Error");

		attroff (A_BOLD);

		closedir (dr);
		getch ();

		return 1;
	}

	closedir (dr);

	char mv [MAX_LENGTH_ADDRESS_BAR] = { 'm', 'v', ' ' };

	strncat (mv, name1, 256);
	strncat (mv, " ", 256);
	strncat (mv, name2, 256);

	system (mv);

	return 0;
}

void addressBarUpadte (const char * const folder) {

        unsigned int sizeAddressBar = strlen (addressBar);
        unsigned int slashIndex = 0; 

        if (strcmp (folder, "..") == 0) {
                        
		for (int i = 1; *(addressBar + (sizeAddressBar - i)) != '/'; i++) {
                                
                        *(addressBar + (sizeAddressBar - i)) = '\0';
                        slashIndex = i;
                }

                if (sizeAddressBar - (slashIndex + 1) != 0) *(addressBar + (sizeAddressBar - (slashIndex + 1))) = '\0';
                else *(addressBar + (sizeAddressBar - slashIndex)) = '\0';
	}

	else {
                
		if (strcmp (folder, ".") == 0) return;
		else if (strcmp (addressBar, "/") != 0) strncat (addressBar, "/", 2);
		
		strncat (addressBar, folder, 256);
        }

	return;
}

void makeDir (void) {

        clear ();

        attron (A_BOLD);
        
        echo ();
        curs_set (1);

        addch ('\n');

        char directory [MAX_LENGTH_NAME] = "\0";

        printw ("%s/", addressBar);
        scanw ("%s", &*directory);

        const unsigned int lengthNameDirectory = strlen (directory);

        for (int i = 0; i < lengthNameDirectory; i++) {

                if (*(directory + i) == '/') {

                        clear ();

                        printw ("\n[!] Error");
                        getch ();
                
                        return;
                }
        }

        noecho ();
        curs_set (0);

        addressBarUpadte ("/");
        addressBarUpadte (directory);

        char makedir [MAX_LENGTH_NAME] = "mkdir -p "; 
        strncat (makedir, addressBar, MAX_LENGTH_NAME);

        system (makedir);

        addressBarUpadte ("..");

        *(addressBar + (strlen (addressBar) - 1)) = '\0';
        *(addressBar + (strlen (addressBar) - 1)) = '\0';

        attroff (A_BOLD);
 
        return;
}


void callEditor (const char * const file) {

	char editor [MAX_LENGTH_NAME] = { 'c', 'o', 'd', 'e', ' ' }; 
	strncat (editor, file, MAX_LENGTH_NAME);

	system (editor);

	return;
}

void sortingDirs (char dirs [] [MAX_LENGTH_NAME], const unsigned int sizeDirs) {

        for (int i = 0; i < sizeDirs; i++) {

                if (strcmp (*(dirs + i), ".") == 0) {

                        strcpy (*(dirs + i), *dirs);
                        strcpy (*dirs, ".");
                }

                else if (strcmp (*(dirs + i), "..") == 0) {

                        strcpy (*(dirs + i), *(dirs + 1));
                        strcpy (*(dirs + 1), "..");
                }
	}

	unsigned int step = 0;

                for (int i = 0; i < sizeDirs; i++) {

                        if (dirs [i] [0] == '.') {

                		strcpy (*(dirs + (MAX_NUMBER_ELEMENTS - 1)), *(dirs + step));
				strcpy (*(dirs + step), *(dirs + i));
				strcpy (*(dirs + i), *(dirs + (MAX_NUMBER_ELEMENTS - 1)));

				step++;
                        }
                }

	return;
}

void sortingFiles (char fls [] [MAX_LENGTH_NAME], const unsigned int sizeFls) {

        unsigned int step = 0;

                for (int i = 0; i < sizeFls; i++) {

                        if (fls [i] [0] == '.') {

                                strcpy (*(fls + (MAX_NUMBER_ELEMENTS - 1)), *(fls + step));
				strcpy (*(fls + step), *(fls + i));
				strcpy (*(fls + i), *(fls + (MAX_NUMBER_ELEMENTS - 1)));

                                step++;
                        }
                }

	return;
}

void initCustomFileManager (const unsigned int mode) {

	DIR* dir = NULL;
	struct dirent* entry = NULL;

	if ((dir = opendir (addressBar)) == NULL) {

		closedir (dir);
		endwin ();
		exit (EXIT_FAILURE);
	}

	char directories [MAX_NUMBER_ELEMENTS] [MAX_LENGTH_NAME] = { '\0' };
	char files [MAX_NUMBER_ELEMENTS] [MAX_LENGTH_NAME] = { '\0' };

	unsigned int indexI = 0;
	unsigned int indexJ = 0;

	while ((entry = readdir (dir)) != NULL) {

		if (entry->d_type == DT_DIR) { 
			
			strcpy (*(directories + indexI), entry->d_name);
			indexI++;
		}
		
		else if (entry->d_type == DT_REG) {
			
			strcpy (*(files + indexJ), entry->d_name);
			indexJ++;
		}
	}

	closedir (dir);

	sortingDirs (&*directories, indexI);
	sortingFiles (&*files, indexJ);

	char objects [MAX_NUMBER_ELEMENTS] [MAX_LENGTH_NAME] = { '\0' };

	for (int i = 0; i < indexI; i++) strcpy (*(objects + i), *(directories + i));
	for (int i = 0; i < indexJ; i++) strcpy (*(objects + (i + indexI)), *(files + i));

	const unsigned int numberObjects = indexI + indexJ; 
	int choice = 0; 

	unsigned int row = 0, col = 0;
	getmaxyx (stdscr, row, col); 

	unsigned int saveIndex = 0;

	unsigned int var1 = 0;
	unsigned int var2 = 0;

	if (numberObjects > row) {

		var1 = numberObjects / row;
		var2 = var1;

	}

	else var2 = 1;

	char newAddress [MAX_LENGTH_ADDRESS_BAR] = "/";
        char j_1 = 0;

	while ( true ) {

		clear ();

		if (choice < (row * var2)) {

			for (int i = 1; i <= var2; i++) {

				if (choice < (row * i)) {

					saveIndex = ((row * i) - row);
					break;
				}

			}

			for (int i = 0; i < row; i++, saveIndex++) {


                                if (saveIndex < numberObjects && mode == 1) printw ("%d ", saveIndex);

				if (saveIndex == choice) { 

					addch ('>');
					attron (A_BOLD);

					addch (' ');
				}

				else {

					attroff (A_BOLD);		
					addch (' ');
				}

				if (findString (directories, indexI, *(objects + saveIndex)) == 0) addch ('/');
				else addch (' ');

				printw ("%s\n", *(objects + saveIndex));
                                
                                attroff (A_BOLD);
                        }
		}

		else {

			for (int i = 0; i < (numberObjects - (row * var2)); i++) {

                                if (mode == 1) printw ("%d ", ((row * var2) + i));

				if (((row * var2) + i) == choice) { 

					addch ('>');
					attron (A_BOLD);

					addch (' ');
				}

				else {

					attroff (A_BOLD);		
					addch (' ');
				}

				if (findString (directories, indexI, *(objects + saveIndex)) == 0) addch ('/');
				else addch (' ');


				printw ("%s\n", *(objects + ((row * var2) + i)));

                                attroff (A_BOLD);
                	}
		}

		attron (A_BOLD | A_UNDERLINE);

		mvwprintw (stdscr, (row - row), (col - 10), "total: %d", numberObjects);
		mvwprintw (stdscr, (row - (row - 1)), (col - (strlen (addressBar) + 1)), "%s", addressBar);

		attroff (A_BOLD | A_UNDERLINE);

		switch ( getch () ) {

                        case 'q':
                                endwin ();
                                exit (EXIT_FAILURE);

			case 'r':
				return;

                        case 'k': 
                                if ((choice - j_2) >= 0) choice -= j_2;
                                else choice = 0;

                                break; 

                        case 'j': 
                                if ((choice + j_2) <= (numberObjects - 1)) choice += j_2;
                                else choice = (numberObjects - 1);

                                break;
                        
			case 'e':
				if (findString (files, indexJ, *(objects + choice)) == 0) callEditor (*(objects + choice));
				else { 
					addressBarUpadte (*(objects + choice));
					return;
				}

				break;

			case 'c':
				switch ( getch () ) {

					case '1':
						clear ();

						echo ();
						curs_set (1);

						attron (A_BOLD);

						addch ('\n');

						addch ('/');
						scanw ("%s", &*(newAddress + 1));

						noecho ();
						curs_set (0);

						dir = opendir (newAddress);				
						if (dir != NULL) {
							
							strcpy (addressBar, newAddress);
							return;
						}

						else {
							
							clear ();
							printw ("\n[!] Error");

							getch ();
						}

						attroff (A_BOLD);
						closedir (dir);

						break;

		                        case '2':						
		                                makeDir ();
						return;

		                        case '3':		
		                                if (search (objects, numberObjects, &choice) != 0) {
							
		                                        clear ();

		                                        attron (A_BOLD);
		                                        printw ("\n[!] Error");

		                                        getch ();

		                                        attroff (A_BOLD);
		                                }

						break;

		                        case '4':
		                                j_1 = getch ();

		                                if (j_1 < '0' || j_1 > '9') {
							
		                                        clear ();

		                                        attron (A_BOLD);
		                                        printw ("\n[!] Error");

		                                        attroff (A_BOLD);
		                                        getch ();

		                                        break;
		                                }

		                                else j_2 = j_1 - 48;

		                                break;

					case '5':
						clear ();

						echo ();
						curs_set (1);

						attron (A_BOLD);
						addch ('\n');

						char name1 [MAX_LENGTH_NAME] = "\0"; 
						
						addch ('>');
						scanw ("%s", &*name1);


						addch ('#');
						for (int i = 0; i < strlen (name1); i++) addch ('#');


						addch ('\n');

						char name2 [MAX_LENGTH_NAME] = "\0";
						
						addch ('>');
						scanw ("%s", &*name2);

						if (findString (objects, numberObjects, name1) == 0 && findString (directories, indexI, name2) == 0) {

							if (moveDir (name1, name2) == 1) return;						
						}
	
						else {

							clear ();
							closedir (dir);

							addch ('\n');

							printw ("[!] Error");

							noecho ();
							curs_set (0);

							attroff (A_BOLD);

							noecho ();
							curs_set (0);

							return;
						}

						noecho ();
						curs_set (0);

						return;
				
					default:
						break;
				}

			default:
                		break;
		}	
	}

	return;
}

int main (const int argc, const char * const argv []) {

        unsigned int setNumbers = 0;

	if (argc > 1) {

		setNumbers = validateFlags (argc, argv);
		invokeCommands (argc, argv);
	}

        initscr ();

        if (has_colors () == false) {

        	endwin ();
        	exit (EXIT_FAILURE);
        }

        noecho ();
        curs_set (0);

        while ( true ) initCustomFileManager (setNumbers);

        endwin ();

	return EXIT_SUCCESS;
}
