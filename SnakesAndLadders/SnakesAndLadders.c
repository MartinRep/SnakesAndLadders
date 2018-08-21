#define _CRT_SECURE_NO_WARNINGS //command for Visual Studio to ignore security warnings
#define KEY_ESC '\033'  //creating easy to remember sign for esc key
#define KEY_SPACE '\032' ////creating easy to remember sign for space key
#define cls() system("cls"); //creating my shortcut command for clearing the screen;



#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
#include<Mmsystem.h>
#include<string.h>



int RandomNumberGenerator(int startRange, int endRange);
void GenerateBoard();
void DisplayBoard();
void position(int x, int y);
void hidecursor();
void NewGame();
void Play();
void MaximizeWindow();
int Saves();
void LoadGame();
int readSavedFile(char* Gamefile);
int writeSaveFile(char* GameFile);
void SaveConfFile();
void SaveGame();
void DeleteGame();


int board[107];
int snakes[16];
int ladders[16];
char savedGame[9][20];
char playersNames[5][10];

void main()
{
	int i, startRange, endRange;
	char choice;
	
	hidecursor();
	MaximizeWindow();
	
	do {
		cls();
		position(27,1);			//Display the Main Menu
		printf("MENU");
		if (board[0] != 0)			//Display Play option only if any game is Loaded or Started new one
		{
			position(25, 5);
			printf("1.Play");
		}
		position(25, 6);
		printf("2.New Game");
		position(25, 7);
		printf("3.Load Game");
		position(25, 8);
		printf("4.Save Game");
		position(25, 9);
		printf("5.Delete Game");
		position(25, 10);
		printf("Esc - Quit");
		choice = _getch();
		switch (choice)
		{
		case '1':
			if (board[0] != 0) Play();
			break;
		case '2':
			NewGame();
			break;
		case '3':
			LoadGame();
			break;
		case '4':
			SaveGame();
			break;
		case '5':
			DeleteGame();
			break;
		case KEY_ESC:
			cls();
			position(1,1);
			printf("...Thank you for using Snakes&Ladders from bigrepo...");
			Sleep(2000);
			break;
		default:
			break;
		}
	} while (choice != KEY_ESC);
}

int RandomNumberGenerator(int startRange, int endRange)
{
	srand( time(NULL)*rand() ); //seeds the random function with time and previous random number.
	return(rand() % (endRange - startRange) + startRange);  //generates number between startRange and endRange and return it 
}

void GenerateBoard()
{
	int i,j, error = 0;
	for (i = 0;i < 16;i=i+2)
	{
		do
		{
			error = 0;
			snakes[i] = RandomNumberGenerator(11, 99); //generates random position of snakes head (no bottom line)
			snakes[i+1] = RandomNumberGenerator(1, snakes[i]); //generates random position of snakes tail between 1 and snakes head.
			if (snakes[i] == snakes[i + 1]) error = 1;
			else for (j = i-1;j >= 0;j--) if (snakes[j] == snakes[i] || snakes[j] == snakes[i+1]) error = 1; //check if generated position isn't already ocupied
		} while (error != 0);	
	}
	for (i = 0; i < 16;i=i+2)
	{
		do
		{
			error = 0;
			ladders[i] = RandomNumberGenerator(1,89); //generates random position for ladder bottom between 1 and 89
			ladders[i+1] = RandomNumberGenerator(ladders[i], 99); // generates random position for ladder top between ladder bottom and 99
			if (ladders[i] == ladders[i + 1]) error = 1;
			else
			{
				for (j = i - 1;j >= 0;j--) if (ladders[i] == ladders[j] || ladders[i + 1] == ladders[j]) error = 1; //checks if randomly generated position of ladder  isn't already occupied with other ladder top and bottom
				for (j = 0;j < 16;j = j + 2) if (ladders[i] == snakes[j] || ladders[i + 1] == snakes[j] || ladders[i] == snakes[j + 1] || ladders[i + 1] == snakes[j + 1]) error = 1; //also checks if ladders top isn't snakes head
			}
		} while (error != 0);
	}
	j = 1;
	for (i = 1;i <= 16;i=i+2)
	{
		board[snakes[i-1]] = j*10+1; //assaign snakes mouth to the board eg. 11
		board[snakes[i]] = j*10+2;	//assaign snakes tail to the board eg. 12
		board[ladders[i-1]] = j*10+3; //assaign ladder bottom to the board eg. 13
		board[ladders[i]] = j*10+4;		//assaign ladder top to the board eg. 14
		j++;
	}
}

void DisplayBoard()
{
	int i, j,p;
	cls();
	position(32, 1);
	printf("Snakes and Ladders");
	position(6,2);
	for (i = 0;i < 10;i++)	printf("______ ");				//Display the table...
	for (j = 0;j < 30;j=j+3)
	{
		position(5, 3 + j);
		for (i = 0;i < 10;i++)	printf("|      ");
		printf("|");
		position(5, 4 + j);
		for (i = 0;i < 10;i++)	printf("|      ");
		printf("|");
		position(5, 5 + j);
		for (i = 0;i < 10;i++)	printf("|______");
		printf("|");
	}
	for (i = 9;i > 0;i = i - 2)
	{
		for (int j = 10; j > 0; j--)
		{
			position(8+7*(10-j),3*(10-i)); //display the number of field
			printf("%d", i * 10 + j);
			for (p = 1;p <= board[0];p++)
			{
				if (i * 10 + j == board[100 + p])
				{
					position(6 + 7 * (10 - j) + (p - 1), 3 * (10 - i) + 2);
					printf("%d", p);					//print out number of player to the field in the table
				}
			}
			if (board[i * 10 + j] != 0)
			{
				
				switch (board[i * 10 + j]%10)
				{
				case 1:
					position(6 + 7 * (10 - j), 3 * (10 - i)+1);
					printf("sHead%d", board[i * 10 + j]/10);
					snakes[((board[i * 10 + j] / 10)-1)*2] = i * 10 + j; // populate the snakes[] array mouth
					break;
				case 2:
					position(6 + 7 * (10 - j), 3 * (10 - i) + 1);
					printf("sTail%d", board[i * 10 + j] / 10);
					snakes[((board[i * 10 + j] / 10) - 1) * 2+1] = i * 10 + j;  // populate the snakes[] array tail
					break;
				case 3:
					position(6 + 7 * (10 - j), 3 * (10 - i) + 1);
					printf("lBott%d", board[i * 10 + j] / 10);
					ladders[((board[i * 10 + j] / 10) - 1) * 2] = i * 10 + j;  // populate the ladders[] array bottom
					break;
				case 4:
					position(6 + 7 * (10 - j), 3 * (10 - i) + 1);
					printf("lTop%d", board[i * 10 + j] / 10);
					ladders[((board[i * 10 + j] / 10) - 1) * 2+1] = i * 10 + j; // // populate the ladders[] array top
					break;
				default:
					break;
				}
			}
		}
		for (int j = 1; j <= 10; j++)
		{
			position(8 + 7 * (j-1), 3+3*(10-i));
			printf("%d",(i-1)*10+j);
			for (p = 1;p <= board[0];p++)
			{
				if ((i - 1) * 10 + j == board[100 + p])
				{
					position(6 + 7 * (j - 1)+(p-1), 3 + 3 * (10 - i) + 2);
					printf("%d", p);			//print out number of player to the field in the table
				}
			}
			if (board[(i-1) * 10 + j] != 0)
			{
				switch (board[(i-1) * 10 + j] % 10)
				{
				case 1:
					position(6 + 7 * (j-1), 3+3 * (10 - i)+1);
					printf("sHead%d", board[(i-1) * 10 + j] / 10);
					snakes[((board[(i-1) * 10 + j] / 10) - 1) * 2] = (i - 1) * 10 + j; // populate the snakes[] array mouth
					break;
				case 2:
					position(6 + 7 * (j-1), 3+3 * (10 - i)+1);
					printf("sTail%d", board[(i-1) * 10 + j] / 10);
					snakes[((board[(i - 1) * 10 + j] / 10) - 1) * 2+1] = (i - 1) * 10 + j; // populate the snakes[] array tail
					break;
				case 3:
					position(6 + 7 * (j-1), 3+3 * (10 - i)+1);
					printf("lBott%d", board[(i-1) * 10 + j] / 10);
					ladders[((board[(i - 1) * 10 + j] / 10) - 1) * 2] = (i - 1) * 10 + j; // populate the ladders[] array bottom
					break;
				case 4:
					position(6 + 7 * (j-1), 3+3 * (10 - i)+1);
					printf("lTop%d", board[(i-1) * 10 + j] / 10);
					ladders[((board[(i - 1) * 10 + j] / 10) - 1) * 2] = (i - 1) * 10 + j; // populate the ladders[] array top
					break;
				default:
					break;
				}
			}
		}
	}
	for (i = 1; i <= board[0];i++)
	{
		position(85, 5 + i);
		printf("%d. Player: %s",i, playersNames + (i-1)); //display list of players next to table
	}
}

void position(int x, int y)
{
	COORD coord = { 0,0 };
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void Play()
{
	char choice;
	int dice,i;
	do
	{
		DisplayBoard();
		position(0,38);
		printf("It's %s turn...\n", playersNames[(board[107]-1)]);
		printf("..press any key to roll the dice... or Esc for MENU..");
		choice = _getch();
		position(0, 38);
		printf("                                                      \n", playersNames[(board[107] - 1)]);
		printf("                                                     ");
		if (choice != KEY_ESC)
		{
			dice = RandomNumberGenerator(1, 6);
			position(0,35);
			printf("%s rolled number %d on the dice.\n", playersNames[board[107] - 1],dice);
			printf("%s is moving from position %d to position %d", playersNames[(board[107] - 1)],board[100+board[107]], board[100 + board[107]] + dice);
			board[100 + board[107]] += dice;
			for (i = 0;i < 16;i = i + 2) if (snakes[i] == board[100 + board[107]])
			{
				printf("\nYou hit the Snakes mouth and slided to position %d", snakes[i + 1]);
				board[100 + board[107]] = snakes[i + 1];
			}
			for (i = 0;i < 16;i = i + 2) if (ladders[i] == board[100 + board[107]])
			{
				printf("\nYou hit the Ladder bottom and climbed to position %d", ladders[i + 1]);
				board[100 + board[107]] = ladders[i + 1];
			}
			if (board[100 + board[107]] > 99)
			{
				printf("\n%s REACHED the TOP and WON!!!! CONGRATULATIONS", playersNames[board[107] - 1]);
				board[0] = 0;
				choice = KEY_ESC;
			}
			board[107]++;
			if (board[107] > board[0]) board[107] = 1;
			position(1, 40);
			printf("Press any key to continue...\n                                                                                              ");
			_getch();
		}
	} while (choice != KEY_ESC);
}

void NewGame()
{
	int i;
	char choice;
	do
	{
		do
		{
			cls();
			position(30, 1);
			printf("NEW GAME");
			position(1, 20);
			printf("Enter the number of players(2-6):");
			scanf("%d", &board[0]);  // board[0] number of players
			position(1, 20);
			printf("\r                                                                        ");
		} while (board[0] <2 || board[0] >6);
		for (i = 0;i < board[0];i++)
		{
			position(1, 20);
			printf("Enter the name of %d. player: ", (i + 1));
			scanf("%s", playersNames[i]);
			position(1, 20);
			printf("\r                                                                        ");
			board[101 + i] = 0;
			position(1, 10 + i);
			printf("%d. Player: %s", i + 1, playersNames[i]);
			board[101+i] = 0;
		}
		position(1, 11+i);
		printf("Are these informations correct? Y?");
		choice = _getch();
		choice = tolower(choice);
	} while (choice != 'y');
	GenerateBoard();
	board[107] = 1;
}

void MaximizeWindow() //function to maximize the console window (source internet)
{
	HWND hWnd;
	SetConsoleTitle("  ");
	hWnd = FindWindow(NULL, "  ");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	SetConsoleScreenBufferSize(hOut, NewSBSize);
	DisplayArea.Right = NewSBSize.X - 1;
	DisplayArea.Bottom = NewSBSize.Y - 1;
	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
	ShowWindow(hWnd, SW_MAXIMIZE);
}

int Saves()
{
	FILE *savesFile;
	int i;
	savesFile = fopen("saves.dat","r");
	i = 0;
	if (savesFile != NULL)
	{
		//cls();
		while (!feof(savesFile) && i < 9)
		{
			fscanf(savesFile,"%s",savedGame[i]);
			i++;
		}
		fclose(savesFile);
	}
	else return -1;
	return 0;

}

void LoadGame()
{
	char choice;
	int choicePosition;
	char fileName[15] = "NoPick";
	if (Saves() != -1)  //call Saves to populate the array with saved games names (if no error)
	{
		cls();
		do {
			position(24,1);
			printf("Load saved Game");
			for (int i = 0;i < 9;i++)			//Display names of saves (1..9)
			{
				position(20, 4 + i);
				printf("%d: %s",i+1, savedGame[i]);
			}
			position(20, 14);
			printf("Esc - Quit");
			choice = _getch();
			choicePosition = choice - '0';
			if (choicePosition < 10 && choicePosition > 0)			//Checks if key pressed is 1 to 9
			{
				strcpy(fileName, "SavedGame");			//Create the name for SaveFile choosen
				fileName[9] = choice;					//Add the number of save
				strcat(fileName, ".dat");
			}
			if (strcmp(fileName, "NoPick") != 0 && strcmp(savedGame[choicePosition - 1], "...empty...") != 0)
			{
				
				if (strcmp(fileName,"NoPick") != 0 && readSavedFile(fileName) != 0)
				{
					position(0, 30);
					printf("...Error Loading the Game!!!...");
					Sleep(1000);
				}
				else
				{
					cls();
					position(0, 30);
					printf("...Games loaded...");
					Sleep(1000);
					return;
				}
			}
		} while (choice != KEY_ESC);
	}
	else		//disply error message method Saved() returns error reading 'saved.dat' file
	{
		position(0, 30);
		printf("...Error reading saved Game!!!...");
		Sleep(1000);
	}
}

int readSavedFile(char* Gamefile)
{
	FILE *savedGameFile;
	
	int i;
	savedGameFile = fopen(Gamefile, "r");		//Open "save(player choice)+.dat" file for read(r)
	if (savedGameFile != NULL)
	{
		for (i = 0; i <= 107; i++) fscanf(savedGameFile, " %d", &board[i]);
		for (i = 0; i < board[0]; i++) fscanf(savedGameFile, "%s", &playersNames[i]);
		fclose(savedGameFile);
		return 0;
	}
	else return -1;
}

int writeSaveFile(char* Gamefile)
{
	FILE *savedGameFile;

	int i;
	savedGameFile = fopen(Gamefile, "w");		//Open "save(player choice)+.dat" file for write(w)
	if (savedGameFile != NULL)
	{
		for (i = 0; i <= 107; i++) fprintf(savedGameFile, " %d", board[i]);
		for (i = 0; i < board[0]; i++) fprintf(savedGameFile, "%s\n", playersNames[i]);
		fclose(savedGameFile);
		return 0;
	}
	else return -1;
}

void SaveConfFile()
{
	FILE *confFile;

	int i;
	confFile = fopen("saves.dat","w");
	for (i = 0;i < 9;i++) fprintf(confFile, "%s\n", savedGame[i]);
	fclose(confFile);
}

void SaveGame()
{
	int choicePosition;
	char choice, overwrite;
	char fileName[15]="NoPick";
	char gameName[10];
	if (board[0] != 0) return;
	cls();
	if (Saves() == -1)  //call Saves to populate the array with saved games names (if no error)
	{
		for (int i = 0; i < 10;i++) strcpy(savedGame[i],"...empty...");			//Populate the Game Saves name array with "...empty..."
		SaveConfFile();															//Creates a new config file
		if (Saves() == -1)														//Checks if config file reads with no problem.
		{
			position(0, 30);
			printf("...Error writing the config file!!!...");
			Sleep(1000);
			return;
		}
	}
	do {
		position(24, 1);
		printf("Save current Game");
		for (int i = 0;i < 9;i++)
		{
			position(20, 4 + i);
			printf("%d: %s", i + 1, savedGame[i]);					//Prints out the names of Saved games
		}
		position(20, 14);
		printf("Esc - Quit");
		choice = _getch();
		choicePosition = choice - '0';			//converting char to int 
		if (choicePosition < 10 && choicePosition > 0)			//Checks if key pressed is 1 to 9
		{
			strcpy(fileName, "SavedGame");			//Create the name for SaveFile choosen
			fileName[9] = choice;					//Add the number of save
			strcat(fileName, ".dat");
		}
		if (strcmp(fileName, "NoPick") != 0)
		{
			position(0, 28);
			printf("Enter Saves Name(max 10 char):");
			scanf("%s", &gameName);
			if (strcmp(savedGame[choicePosition - 1], "...empty...") == 0)
			{
				if (writeSaveFile(fileName) != 0)
				{
					position(0, 30);
					printf("...Error saving the Game!!!...");
					Sleep(1000);
				}
				else
				{
					cls();
					position(0, 30);
					printf("...Game saved...");
					strcpy(savedGame[choicePosition - 1], gameName);
					SaveConfFile();
					Sleep(1000);
					return;
				}
			}
			else				//if save position selected is occupied
			{
				position(0, 28);
				printf("Position already occupied! Owerwrite? Y?");
				overwrite = _getch();
				overwrite = tolower(overwrite);
				if (overwrite == 'y')
				{
					if (writeSaveFile(fileName) != 0)
					{
						position(0, 30);
						printf("...Error saving the Game!!!...");
						Sleep(1000);
						break;
					}
				}
				else strcpy(fileName,"NoPick");
			}
		}
	} while (choice != KEY_ESC);
}

void DeleteGame()
{
	char choice, overwrite;
	int choicePosition;
	char* fileName = "No Pick";
	if (Saves() != -1)  //call Saves to populate the array with saved games names (if no error)
	{
		cls();
		do {
			position(24, 1);
			printf("Load saved Game");
			for (int i = 0;i < 9;i++)			//Display names of saves (1..9)
			{
				position(20, 4 + i);
				printf("%d: %s", i + 1, savedGame[i]);
			}
			position(20, 14);
			printf("Esc - Quit");
			choice = _getch();
			choicePosition = choice - '0';
			if (choice != KEY_ESC && strcmp(savedGame[choicePosition - 1], "...empty...") != 0)
			{
				position(0,38);
				printf("Are you sure you want to erase the Saved Game? Y?");
				overwrite = _getch();
				overwrite = tolower(overwrite);
				if (overwrite == 'y')
				{
			/*		switch (choice)
					{
					case '1':
						remove("SavedGame1.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '2':
						remove("SavedGame2.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '3':
						remove("SavedGame3.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '4':
						remove("SavedGame4.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '5':
						remove("SavedGame5.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '6':
						remove("SavedGame6.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '7':
						remove("SavedGame7.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '8':
						remove("SavedGame8.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					case '9':
						remove("SavedGame9.dat");
						strcpy(savedGame[choicePosition - 1], "...empty...");
						break;
					default:
						break;
					}*/
					remove("SavedGame9.dat");
					strcpy(savedGame[choicePosition - 1], "...empty...");
					SaveConfFile();
					cls();
				}
				else return;
			}
		} while (choice != KEY_ESC);
	}
	else		//disply error message method Saved() returns error reading 'saved.dat' file
	{
		position(0, 30);
		printf("...Error reading saved Game!!!...");
		Sleep(1000);
	}
}
