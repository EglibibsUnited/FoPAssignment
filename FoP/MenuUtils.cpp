#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <fstream>
#include<vector>
using namespace std;

#include "ConsoleUtils.h"
#include "TimeUtils.h"


void displayStartScreen()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	ifstream titleArt;								// Open the 'title.txt' file for displaying the 'Spot and Zombies' title screen art //
	titleArt.open("title.txt", ios::in);
	string value;
	for (int i = 18; titleArt; i++)
	{
		getline(titleArt, value);
		showMessage(clRed, clWhite, 6, i, value);
	}
	titleArt.close();

	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "--------------------------");

	showMessage(clDarkGrey, clYellow, 5, 6, "    Group SE1_5 - 2018    ");
	showMessage(clDarkGrey, clYellow, 5, 7, " ------------------------ ");
	showMessage(clDarkGrey, clYellow, 5, 8, " > Michael Elsom 27035059 ");
	showMessage(clDarkGrey, clYellow, 5, 9, " > James Nelhams 27021413 ");
	showMessage(clDarkGrey, clYellow, 5, 10, " > Jake Stringer 27003087 ");

	showMessage(clDarkGrey, clYellow, 40, 2, "Date: " + GetDate());
	showMessage(clDarkGrey, clYellow, 40, 3, "Time: " + GetTime());

	showMessage(clDarkGrey, clYellow, 40, 6, "        Controls       ");
	showMessage(clDarkGrey, clYellow, 40, 7, "-----------------------");
	showMessage(clDarkGrey, clYellow, 40, 8, "| Movement: Arrows    |");
	showMessage(clDarkGrey, clYellow, 40, 9, "| Kill Zombles: X     |");
	showMessage(clDarkGrey, clYellow, 40, 10, "| Freeze: F           |");
	showMessage(clDarkGrey, clYellow, 40, 11, "| Eat Pills: E        |");
	showMessage(clDarkGrey, clYellow, 40, 12, "| Quit: Q             |");
	showMessage(clDarkGrey, clYellow, 40, 13, "| Save Game: S        |");
	showMessage(clDarkGrey, clYellow, 40, 14, "| Load Game: L        |");
	showMessage(clDarkGrey, clYellow, 40, 15, "-----------------------");

	showMessage(clDarkGrey, clYellow, 5, 16, "Enter your name to start:");
	showMessage(clBlack, clRed, 31, 16, " ");
}

bool menuScreen(string playerName)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void scoreScreen(string playerName);
	void rulesScreen();
	void chooseLevel(string playerName);
	void endProgram();

	bool quit = false;
	Clrscr();


	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 6, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 7, "| > Play Game (P)        |");
	showMessage(clDarkGrey, clYellow, 5, 8, "| > See Score (S)        |");
	showMessage(clDarkGrey, clYellow, 5, 9, "| > See Rules (R)        |");
	showMessage(clDarkGrey, clYellow, 5, 10, "| > Quit (Q)             |");
	showMessage(clDarkGrey, clYellow, 5, 11, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 13, "Please enter answer: ");

	char answer;								// Get user input regarding what menu options to select //
	showMessage(clBlack, clRed, 28, 13, " ");
	cin >> answer;
	switch (toupper(answer))
	{
	case 'P':	// Play the game //
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		chooseLevel(playerName);
		break;
	case 'S':	// See the scores //
		scoreScreen(playerName);
		break;
	case 'R':	// See the rules //
		rulesScreen();
		break;
	case 'Q':	// Quit the game //
		quit = true;
		break;
	default:
		showMessage(clBlack, clRed, 5, 15, "Invalid Key!!");	// Otherwise, say an invalid key has been pressed //
		Sleep(1000);
		break;
	}

	cin.get();	// So the rules / score screen doesn't suddenly disappear //
	return quit;
}
void chooseLevel(string playerName)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void scoreScreen(string playerName);
	void runGame(string playerName, int levelNumber);
	void rulesScreen();
	void endProgram();

	bool quit = false;
	Clrscr();


	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 6, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 7, "| > Level 1 (Easy)       |");
	showMessage(clDarkGrey, clYellow, 5, 8, "| > Level 2 (Medium)     |");
	showMessage(clDarkGrey, clYellow, 5, 9, "| > Level 3 (Hard)       |");
	showMessage(clDarkGrey, clYellow, 5, 10, "| > Quit (Q)             |");
	showMessage(clDarkGrey, clYellow, 5, 11, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 13, "Please enter answer: ");

	char answer;								// Get user input regarding what menu options to select //
	bool gameHasRun = false;
	showMessage(clBlack, clRed, 28, 13, " ");
	do {
		cin >> answer;
		switch (toupper(answer))
		{
		case '1':
			gameHasRun = true;
			runGame(playerName, 1);
			break;
		case '2':
			gameHasRun = true;
			runGame(playerName, 2);
			break;
		case '3':
			gameHasRun = true;
			runGame(playerName, 3);
		case 'R':
			break;
		default:
			showMessage(clBlack, clRed, 5, 15, "Invalid Key!!");	// Otherwise, say an invalid key has been pressed //
			Sleep(1000);
			break;
		}
	} while (!gameHasRun);
}
void scoreScreen(string playerName)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	int getPlayerScore(string playerName);

	Clrscr();
	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "--------------------------");
	int score = getPlayerScore(playerName);
	if (score >= -1)
	{
		showMessage(clDarkGrey, clYellow, 5, 5, "|> Previous Best Score: " + to_string(score) + "|");
	}
	else
	{
		showMessage(clDarkGrey, clYellow, 5, 5, "|> No Previous Best Score!");
	}
	showMessage(clBlack, clYellow, 5, 7, "Press enter to return...");

	cin.ignore();

}
void rulesScreen()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	Clrscr();
	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "|                        |");
	showMessage(clDarkGrey, clYellow, 5, 5, "|         RULES          |");
	showMessage(clDarkGrey, clYellow, 5, 6, "--------------------------");

	showMessage(clBlack, clGrey, 5, 9, "1: When Spot ( ) eats a Power Pill ( ), he gains a life.");
	showMessage(clBlack, clBlue, 19, 9, "@");
	showMessage(clBlack, clYellow, 41, 9, "*");

	showMessage(clBlack, clGrey, 5, 10, "2: When Spot ( ) falls into a hole ( ), he loses a life.");
	showMessage(clBlack, clBlue, 19, 10, "@");
	showMessage(clBlack, clRed, 41, 10, "0");

	showMessage(clBlack, clGrey, 5, 11, "3: When Spot ( ) touches a zombie ( ), he loses a life.");
	showMessage(clBlack, clGrey, 15, 12, "(The zombie returns to its original position)");
	showMessage(clBlack, clBlue, 19, 11, "@");
	showMessage(clBlack, clGreen, 40, 11, "Z");

	showMessage(clBlack, clGrey, 5, 13, "4: A zombie ( ) cannot eat a Power Pill ( ).");
	showMessage(clBlack, clGreen, 18, 13, "Z");
	showMessage(clBlack, clYellow, 46, 13, "*");

	showMessage(clBlack, clGrey, 5, 14, "5: If Spot ( ) eats a Power Pill ( ) and touches a zombie ( ) at the same time, they cancel out.");
	showMessage(clBlack, clGrey, 56, 15, "(The zombie returns to its original position)");
	showMessage(clBlack, clBlue, 17, 14, "@");
	showMessage(clBlack, clYellow, 39, 14, "*");
	showMessage(clBlack, clGreen, 64, 14, "Z");

	showMessage(clBlack, clGrey, 5, 16, "6: If two zombies ( ) collide, they both return to their original positions.");
	showMessage(clBlack, clGreen, 24, 16, "Z");

	showMessage(clBlack, clGrey, 5, 17, "7: A zombie ( ) that falls into a hole will disappear and not return.");
	showMessage(clBlack, clGreen, 18, 17, "Z");


	cin.ignore();
}