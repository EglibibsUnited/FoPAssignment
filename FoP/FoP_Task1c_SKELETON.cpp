//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Jake Stinger / Michael Elsom / James Nelhams
//Last updated: 1 March 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <fstream>
#include<vector>
using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for Seed, Random
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(38);    	//horizontal dimension
const int  SIZEY(25);		//vertical dimension
							//defining symbols used for display of the grid and content
const char SPOT('@');			 	//spot
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char HOLE('0');    	//hole
const char POWERPILL('*');  //power pill
const char ZOMBIE('Z');  //zombie
						 //defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
							//defining the other command letters
const char QUIT('Q');		//to end the game

struct Item {
	int x, y;
	char symbol;
	bool canMove = true;
	int defaultX, defaultY;
};

int main()
{
	void displayStartScreen();
	bool menuScreen(string playerName, int level);
	void runGame();
	void changeCursorVisibility(bool);
	void endProgram();

	Seed();								//seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2018");

	displayStartScreen();
	string playerName;
	cin >> playerName;
	changeCursorVisibility(false);
	Clrscr();

	int level = 1;

	do
	{
		menuScreen(playerName, level);
		Clrscr();
	} while (!menuScreen(playerName, level));


	endProgram();						//display final message
	return 0;
}

//---------------------------------------------------------------------------
//----- Run game
//---------------------------------------------------------------------------

void runGame(string playerName, int level)
{
	//function declarations (prototypes)
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot, Item zombies[]);
	void paintGame(const char g[][SIZEX], string mess, int lives, string playerName, int powerPills, char m[][SIZEX], int zombieCount, int level);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	bool isCheatCode(const int k);
	int  getKeyPress();
	void changeCursorVisibility(bool);
	void runCheatCode(const int k, int& powerPills, Item zombies[], bool& zombFreeze, int& zombieCount);
	void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, int& lives, char m[][SIZEX], int& powerPills, Item zombies[], int& powerpillTouch, int moveCounter, bool zombMove, int& zombieCount, bool& powerpillTouched, vector<Item>, vector<Item>);

	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot, Item zombies[]);
	void powerpillProtection(int moveCounter, int& powerpillTouch, Item& spot, bool& powerpillTouched);
	int getPlayerScore(string playerName);
	void playerData(string playerName, int lives);
	bool hasWon(Item zombies[], int powerPills);
	void checkPlayerScore(string playerName);
	void saveGame(const char grid[][SIZEX], string playerName ,int lives ,int powerPills ,int zombieCount, Item spot, Item zombies[]);
	void loadGame(char maze[][SIZEX], string playerName, int& lives, int& powerPills, int& zombieCount, Item& spot, Item zombies[]);
	void showReplay(const char m[][SIZEX], char g[][SIZEX], Item spot, Item zombies[], vector<Item> spotReplay, vector<Item> zombiesReplay);

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	Item zombies[4];					// Array of zombies // 
	vector<Item> spotReplay;
	vector<Item> zombieReplay;
	string message("LET'S START...");	//current message to player
	int lives = 3;						// Initialise Spot with 3 lives //
	int powerPills = 8;					// Initialise the game with 8 power pills //
	int zombieCount = 4;				//zombie count for checking game finish
	bool zombiesMove(true);				//For zombie freeze cheat code
	bool hasCheated(false);
	bool powerpillTouched(false);

	int moveCounter(0), powerpillTouch(0);

	changeCursorVisibility(false);
	checkPlayerScore(playerName);		// Check for previous high scores, handle save files etc. //

	initialiseGame(grid, maze, spot, zombies);	// Initialise grid (incl. walls and spot) //

	paintGame(grid, message, lives, playerName, powerPills, maze, zombieCount, level);			//display game info, modified grid and messages
	
	int key;							//current key selected by player
	do {
		key = getKeyPress(); 	//read in selected key: arrow or letter command
		key = toupper(key);
		if (isArrowKey(key))
		{
			updateGameData(grid, spot, key, message, lives, maze, powerPills, zombies, powerpillTouch, moveCounter, zombiesMove, zombieCount, powerpillTouched, spotReplay, zombieReplay);		//move spot in that direction
			updateGrid(grid, maze, spot, zombies);					//update grid information
			moveCounter++;
		}
		if (isCheatCode(key))
		{
			runCheatCode(key, powerPills, zombies, zombiesMove, zombieCount);
			hasCheated = true;
			updateGameData(grid, spot, key, message, lives, maze, powerPills, zombies, powerpillTouch, moveCounter, zombiesMove, zombieCount, powerpillTouched, spotReplay, zombieReplay);
			updateGrid(grid, maze, spot, zombies);
		}
		if (key == 'S')
		{
			saveGame(grid, playerName, lives, powerPills, zombieCount, spot, zombies);
		}
		if (key == 'L')
		{
			loadGame(maze, playerName, lives, powerPills, zombieCount, spot, zombies);
			updateGrid(grid, maze, spot, zombies);
		}
		if (key == 'R')
		{
			showReplay(maze, grid, spot, zombies, spotReplay, zombieReplay);
		}
		else
		{
			message = "INVALID KEY!";	//set 'Invalid key' message
		}
		paintGame(grid, message, lives, playerName, powerPills, maze, zombieCount, level);		//display game info, modified grid and messages
	} while (!wantsToQuit(key) && lives >= 0 && hasWon(zombies, powerPills) == false); // Game quits if user presses Q, Spot has no lives or wins the game //
	
	if (lives < 0 && !wantsToQuit(key))
	{
		showMessage(clRed, clYellow, 40, 24, "Unlucky, try again next time!");
	}
	else if (!wantsToQuit(key))
	{
		showMessage(clYellow, clRed, 40, 24, "Congratulations!! You Win!!");
		level++; // TODO: Make levels
		bool action = false;
		do {
			Clrscr();
			showMessage(clBlack, clBlack, 1, 1, "");
			showMessage(clBlack, clRed, 5, 5, "Continue to next level(N) or return(R) to menu?");
			key = getKeyPress();
			key = toupper(key);
			switch (key)
			{
			case 'N':
				// TODO Next level loads here
				action = true;
				break;
			case 'R':
				action = true;
				break;
			default:
				break;
			}
		} while (action == false);
	}

	if (!hasCheated)
	{
		playerData(playerName, lives);
	}

	changeCursorVisibility(true);
	Sleep(2000);
	showMessage(clBlack, clYellow, 40, 24, "");
}

void checkPlayerScore(string p)
{
	ifstream getScore;
	getScore.open(".\\Players\\" + p + ".txt", ios::in);
	int value, sum(0);
	getScore >> value;
	if (value < -1)
	{
		// Nothing is written in the file //
		ofstream writeScore;
		writeScore.open(".\\Players\\" + p + ".txt", ios::out);
		writeScore << "-1";
	}
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------
void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, Item zombies[])
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX], Item zombies[]);
	void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX]);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], Item b, Item zombies[]);

	setInitialMazeStructure(maze, zombies);		//initialise maze
	setSpotInitialCoordinates(spot, maze);
	updateGrid(grid, maze, spot, zombies);		//prepare grid
}

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

bool menuScreen(string playerName, int level)
{

	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void scoreScreen(string playerName);
	void rulesScreen();
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
		runGame(playerName, level);
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

	cin.get();

	return quit;
}
void scoreScreen(string playerName)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	int getPlayerScore(string playerName);

	Clrscr();
	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "--------------------------");
	int score =  getPlayerScore(playerName);
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
	showMessage(clBlack, clGreen , 64, 14, "Z");

	showMessage(clBlack, clGrey, 5, 16, "6: If two zombies ( ) collide, they both return to their original positions.");
	showMessage(clBlack, clGreen, 24, 16, "Z");

	showMessage(clBlack, clGrey, 5, 17, "7: A zombie ( ) that falls into a hole will disappear and not return.");
	showMessage(clBlack, clGreen, 18, 17, "Z");


	cin.ignore();
}

void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX])
{ //set spot coordinates inside the grid at random at beginning of game
	int y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
	int x = Random(SIZEX - 2);      //horizontal coordinate in range [1..(SIZEX - 2)]
	while (maze[y][x] == WALL)
	{
		y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		x = Random(SIZEX - 2);		//horizontal coordinate in range [1..(SIZEX - 2)]
	}
	spot.y = y;
	spot.x = x;
}

void setInitialMazeStructure(char maze[][SIZEX], Item zombies[])
{ //set the position of the walls in the maze
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX];

	for (int column = 0; column < SIZEY; column++)
	{
		for (int row = 0; row < SIZEX; row++)
		{
			if (column == 0 || column == SIZEY - 1 || row == 0 || row == SIZEX - 1)
			{
				// Just a row of walls //
				initialMaze[column][row] = '#';
			}
			else
			{
				// Make it a tunnel //
				initialMaze[column][row] = ' ';
			}
		}
	}

	// Create Zombies - set their default X and Y positions and initialise their X and Y coords to the same //
	zombies[0].defaultY = 1; zombies[0].defaultX = 1; zombies[0].y = 1; zombies[0].x = 1;
	zombies[1].defaultY = 1; zombies[1].defaultX = SIZEX - 2; zombies[1].y = 1; zombies[1].x = SIZEX - 2;
	zombies[2].defaultY = SIZEY - 2; zombies[2].defaultX = 1; zombies[2].y = SIZEY - 2; zombies[2].x = 1;
	zombies[3].defaultY = SIZEY - 2; zombies[3].defaultX = SIZEX - 2; zombies[3].y = SIZEY - 2; zombies[3].x = SIZEX - 2;
	for (int zomb = 0; zomb < 4; zomb++)
	{
		zombies[zomb].symbol = 'Z';
		//initialMaze[zombies[zomb].y][zombies[zomb].x] = zombies[zomb].symbol;
	}

	for (int holesCount = 12; holesCount >= 0; holesCount--) // Add holes //
	{
		int x = Random(SIZEX - 2);
		int y = Random(SIZEY - 2);
		while (initialMaze[y][x] == WALL || initialMaze[y][x] == ZOMBIE)
		{
			x = Random(SIZEX - 2);
			y = Random(SIZEY - 2);
		}
		initialMaze[y][x] = '0';
	}

	for (int powerpills = 8; powerpills > 0; powerpills--) // Add power pills //
	{
		int x = Random(SIZEX - 2);
		int y = Random(SIZEY - 2);
		while (initialMaze[y][x] == WALL || initialMaze[y][x] == ZOMBIE || initialMaze[y][x] == HOLE)
		{
			x = Random(SIZEX - 2);
			y = Random(SIZEY - 2);
		}
		initialMaze[y][x] = '*';
	}

	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used change
			case '#': maze[row][col] = WALL; break;
			case ' ': maze[row][col] = TUNNEL; break;
			case '0': maze[row][col] = HOLE; break;
			case '*': maze[row][col] = POWERPILL; break;
			case 'Z': maze[row][col] = ZOMBIE; break;
			}
}

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item spot, Item zombies[])
{ //update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX], Item zombies[]);
	void placeItem(char g[][SIZEX], const Item spot);

	setMaze(grid, maze, zombies);	//reset the empty maze configuration into grid
	placeItem(grid, spot);	//set spot in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX], Item zombies[])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];

	// ZOMBIES placement on grid //
	for (int zomb = 0; zomb < 4; zomb++)
	{



		grid[zombies[zomb].y][zombies[zomb].x] = zombies[zomb].symbol;
	}
}

void placeItem(char g[][SIZEX], const Item item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

//---------------------------------------------------------------------------
//----- move items on the grid
//---------------------------------------------------------------------------

void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, int& lives, char maze[][SIZEX], int& powerPills, Item zombies[], int& powerpillTouch, int moveCounter, bool zombiesMove, int& zombieCount, bool& powerpillTouched, vector<Item> spotReplay, vector<Item> zombiesReplay)
{ //move spot in required direction
	bool isArrowKey(const int k);
	bool isCheatCode(const int k);
	void setKeyDirection(int k, int& dx, int& dy);

	if (isArrowKey(key))
	{
		void powerpillProtection(int moveCounter, int& powerpillTouch, Item& spot, bool& powerpillTouched);
		assert(isArrowKey(key));

		//reset message to blank
		mess = "                                         ";		//reset message to blank

																//calculate direction of movement for given key
		int dx(0), dy(0);
		setKeyDirection(key, dx, dy);

		//check new target position in grid and update game data (incl. spot coordinates) if move is possible
		switch (g[spot.y + dy][spot.x + dx])
		{			//...depending on what's on the target position in grid...
		case TUNNEL:		//can move
			spot.y += dy;	//go in that Y direction
			spot.x += dx;	//go in that X direction
			if (powerpillTouched == true)
				powerpillProtection(moveCounter, powerpillTouch, spot, powerpillTouched);
				spotReplay.push_back(spot);
			break;
		case WALL:  		//hit a wall and stay there
			mess = "CANNOT GO THERE!";
			spotReplay.push_back(spot);
			break;
		case HOLE:		// Fall into a hole //
			spot.y += dy;
			spot.x += dx;
			lives--;
			spotReplay.push_back(spot);
			break;
		case POWERPILL:		// Eat power pill // Task 2a Function Call
			spot.y += dy;
			spot.x += dx;
			maze[spot.y][spot.x] = ' ';
			powerpillTouch = moveCounter;
			powerpillTouched = true;
			powerpillProtection(moveCounter, powerpillTouch, spot, powerpillTouched);
			lives++;
			powerPills--;
			spotReplay.push_back(spot);
			break;
		}

		// Move Zombies //
		if (zombiesMove)
		{
			for (int zomb = 0; zomb < 4; zomb++)
			{
				if (zombies[zomb].canMove)
				{
					// Move the zombie //
					if ((maze[zombies[zomb].y][zombies[zomb].x + 1] != WALL) && (spot.x >= zombies[zomb].x)) // Right //
					{
						if (powerpillTouched && maze[zombies[zomb].y][zombies[zomb].x - 1] != WALL)
						{
							zombies[zomb].x--;
						}
						else if (!powerpillTouched)
						{
							
								zombies[zomb].x++;
								if ((zombies[zomb].y == spot.y) && (zombies[zomb].x == spot.x))
								{

									zombies[zomb].y = zombies[zomb].defaultY;
									zombies[zomb].x = zombies[zomb].defaultX;

									lives--;

								}
						}
					}

					// TODO: I changed these to four if's not two if's/elseif's cos it seems to fix the zigzag issue but maybe caused other bits to fuck up now... //
					
					if ((maze[zombies[zomb].y][zombies[zomb].x - 1] != WALL) && (spot.x <= zombies[zomb].x)) // Left //
					{
						if (powerpillTouched && maze[zombies[zomb].y][zombies[zomb].x + 1] != WALL)
						{
							zombies[zomb].x++;
						}
						else if (!powerpillTouched)
						{
							
								zombies[zomb].x--;
							
						}
					}

					if ((maze[zombies[zomb].y - 1][zombies[zomb].x] != WALL) && (spot.y <= zombies[zomb].y)) // Up //
					{
						if (powerpillTouched && maze[zombies[zomb].y + 1][zombies[zomb].x] != WALL)
						{
							zombies[zomb].y++;
						}
						else if (!powerpillTouched)
						{
							
								zombies[zomb].y--;
								if ((zombies[zomb].y == spot.y) && (zombies[zomb].x == spot.x))
								{
								
										zombies[zomb].y = zombies[zomb].defaultY;
										zombies[zomb].x = zombies[zomb].defaultX;

										lives--;
									
								}
						}
					}
					
					if ((maze[zombies[zomb].y + 1][zombies[zomb].x] != WALL) && (spot.y >= zombies[zomb].y)) // Downs //
					{
						if (powerpillTouched && maze[zombies[zomb].y - 1][zombies[zomb].x] != WALL)
						{
							zombies[zomb].y--;
						}
						else if (!powerpillTouched)
						{
							
								zombies[zomb].y++;
							
						}
					}
					
					
				}

				// See if a zombie is touching spot //
				// TODO: Having powerpillTouched being true seems to make the next IF statement not fire for some reason? //
				// TODO: Also seems that this if statement only fires if two zombies are both above and below spot >:( //

				if ((zombies[zomb].y == spot.y) && (zombies[zomb].x == spot.x))
				{
					if (powerpillTouched) // Can spot kill the zombie? //
					{
						zombies[zomb].canMove = false;
						zombies[zomb].symbol = ' ';
						zombies[zomb].x = -1;
						zombies[zomb].y = -1;
						zombies--;
					}
					else
					{
						zombies[zomb].y = zombies[zomb].defaultY;
						zombies[zomb].x = zombies[zomb].defaultX;
						
						lives--;
					}
					
				}

				// Check for zombies touching a hole //
				if (maze[zombies[zomb].y][zombies[zomb].x] == HOLE)
				{
					zombies[zomb].y = -1; zombies[zomb].x = -1;
					zombies[zomb].symbol = ' ';
					zombieCount--;
					zombies[zomb].canMove = false;
				}
			}
		}

		// Check if two zombies are touching //

		for (int zomb = 0; zomb < 4; zomb++)
		{
			Item z = zombies[zomb];
			for (int zomb2 = 0; zomb2 < 4; zomb2++)
			{
				Item z2 = zombies[zomb2];
				if (zomb != zomb2 && z.x == z2.x && z.y == z2.y)
				{
					zombies[zomb].x = zombies[zomb].defaultX;
					zombies[zomb].y = zombies[zomb].defaultY;

					zombies[zomb2].x = zombies[zomb2].defaultX;
					zombies[zomb2].y = zombies[zomb2].defaultY;
				}
			}
		}

		if (lives < 0)
		{
			mess = "YOU LOSE!";
		}
	}

	//Remove pills from game on E press
	if (isCheatCode(key)) {
		if (key == 'E')
		{
			for (int row(0); row < SIZEY; row++)
			{
				for (int col(0); col < SIZEX; col++)
				{
					if (maze[row][col] == POWERPILL)
					{
						maze[row][col] = TUNNEL;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = +1;
		break;
	}
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

bool isCheatCode(const int key)
{
	return (key == 'E') || (key == 'X') || (key == 'F');
}

//Runs a cheat on right button press
void runCheatCode(const int key, int& powerPills, Item zombs[4], bool& zombieMove, int& zombieCount) {
	switch (key)
	{
	case 'E': powerPills = 0;
		break;
	case 'X':
		for (int i = 0; i < 4; i++)
		{
			if (zombs[i].symbol == ' ')
			{
				zombs[i].symbol = ZOMBIE;
				
				zombieCount = 4;
			}
			else
			{
				zombs[i].symbol = ' ';
				zombs[i].x = zombs[i].defaultX;
				zombs[i].y = zombs[i].defaultY;
				
				zombieCount = 0;
				zombs[i].canMove = false;
			}
		}
		break;
	case 'F': zombieMove = !zombieMove;
		break;
	default:
		break;
	}
}

bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x)
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{	//display a string using specified colour at a given position 
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}
void paintGame(const char g[][SIZEX], string mess, int lives, string playerName, int powerPills, char m[][SIZEX], int zombieCount, int level)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX], char m[][SIZEX]);
	int getPlayerScore(string playerName);
	//display game title
	showMessage(clBlack, clYellow, ((SIZEX - 2) / 2 - 8), 1, "SPOT AND ZOMBIES");
	SelectBackColour(clDarkGrey);
	SelectTextColour(clYellow);

	// Display lives left //
	stringstream ss;
	if (lives < 0)
	{
		ss << "Lives: 0H NO";
	}
	else
	{
		ss << "Lives: " << lives;
	}
	stringstream pps;
	if (powerPills < 0)
	{
		pps << "Power Pills Remaining: 0";
	}
	else
	{
		pps << "Power Pills Remaining: " << powerPills;
	}


	// Display date and time etc. //
	showMessage(clDarkGrey, clYellow, 40, 1, "FoP Task 1c: February 2018");
	showMessage(clDarkGrey, clYellow, 40, 2, "Date: " + GetDate());
	showMessage(clDarkGrey, clYellow, 40, 3, "Time: " + GetTime());

	//display menu options available
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

	showMessage(clBlack, clGreen, 40, 17, ss.str());
	showMessage(clBlack, clGreen, 40, 18, pps.str());

	showMessage(clBlack, clGreen, 40, 19, "Zombs remaining: " + to_string(zombieCount));

	string lss = to_string(level);
	showMessage(clBlack, clGreen, 40, 24, "LEVEL " + lss);

	string score = to_string(getPlayerScore(playerName));
	showMessage(clBlack, clGreen, 40, 20, playerName);
	if (stoi(score) < -1)
	{
		showMessage(clBlack, clGreen, 40, 21, playerName + " has no previous best score!");
	}
	else
	{
		showMessage(clBlack, clGreen, 40, 21, playerName + "'s previous best score is: " + score);
	}

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 26, mess);


	paintGrid(g, m);
}

void paintGrid(const char g[][SIZEX], char m[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
		{
			if (g[row][col] == SPOT)
			{
				SelectTextColour(clBlue);
				cout << g[row][col];
				SelectTextColour(clWhite);
			}
			else if (g[row][col] == HOLE)
			{
				SelectTextColour(clRed);
				cout << g[row][col];
				SelectTextColour(clWhite);
			}
			else if (g[row][col] == ZOMBIE)
			{
				SelectTextColour(clGreen);
				cout << g[row][col];
				SelectTextColour(clWhite);
			}
			else if (g[row][col] == POWERPILL)
			{
				SelectTextColour(clYellow);
				cout << g[row][col];
				SelectTextColour(clWhite);
			}
			else
			{
				cout << g[row][col];	//output cell content
			}
		}
		cout << endl;
	}
}

void powerpillProtection(int moveCounter, int& powerpillTouch, Item& spot, bool& powerpillTouched)
{
	if (moveCounter - 10 < powerpillTouch && powerpillTouched == true)
	{
		spot.symbol = '$';
	}
	else if (moveCounter - 10 > powerpillTouch && powerpillTouched == true)
	{
		spot.symbol = SPOT;
		powerpillTouched = false;
	}
}

int getPlayerScore(string playerName)
{
	ifstream getScore;
	getScore.open(".\\Players\\" + playerName + ".txt", ios::in);
	int value, sum(0);
	getScore >> value;
	while (getScore)
	{
		getScore >> value;
		sum++;
	}
	getScore.close();
	return value;
}

void playerData(string playerName, int lives)
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	ifstream getScore;
	ofstream writeScore;
	getScore.open(".\\Players\\" + playerName + ".txt", ios::in);
	int value, sum(0);
	getScore >> value;
	while (getScore)
	{
		getScore >> value;
		sum++;
	}
	getScore.close();
	writeScore.open(".\\Players\\" + playerName + ".txt", ios::out);
	if (sum > 0)
	{
		if (lives > value)
		{
			showMessage(clRed, clYellow, 40, 22, "NEW SCORE!");
			writeScore << lives;
		}
	}
	else if (sum < 0)
	{
		writeScore << lives;
	}
}
bool hasWon(Item zombies[], int powerPills)
{
	int zombiesRemaining = 0;
	for (int zomb = 0; zomb < 4; zomb++)
	{
		if (zombies[zomb].canMove)
		{
			zombiesRemaining++;
		}
	}
	return (zombiesRemaining == 0 && powerPills == 0);
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clRed, clYellow, 5, 15, "");
	system("pause");	//hold output screen until a keyboard key is hit
}

void changeCursorVisibility(bool v)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = v; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}


//SAVE AND LOAD - REQUIRES FOLDER CALLED SAVES
void saveGame(const char g[][SIZEX], string playerName, int lives, int powerPills, int zombieCount, Item spot, Item zombies[]) {
	ofstream writeGrid;
	writeGrid.open(".//Saves//" + playerName + ".txt.", ios::out);
	
	for (int row(0); row < SIZEY; row++)
	{
		for  (int col(0);col < SIZEX; col++)
		{
			writeGrid.put(g[row][col]);
			
		}
	}
	writeGrid.put(' ');
	writeGrid << lives;
	writeGrid.put(' ');
	writeGrid << powerPills;
	writeGrid.put(' ');
	writeGrid << zombieCount;
	writeGrid.put(' ');
	writeGrid << spot.x;
	writeGrid.put(' ');
	writeGrid << spot.y;
	for (int i = 0; i < 4; i++)
	{
		writeGrid.put(' ');
		writeGrid << zombies[i].x;
		writeGrid.put(' ');
		writeGrid << zombies[i].y;
		writeGrid.put(' ');
		writeGrid << zombies[i].canMove;
	}
}

//LOAD
void loadGame(char m[][SIZEX], string playerName, int& lives, int& powerPills, int& zombieCount, Item& spot, Item zombies[]) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	ifstream readGrid;
	int value;
	readGrid.open(".//Saves//" + playerName + ".txt", ios::in);
	if (readGrid.fail())
	{
		
		showMessage(clRed, clYellow, 40, 25, "No save file present");
	}
	else 
	{
		for (int row(0); row < SIZEY; row++)
		{
			for (int col(0); col < SIZEX; col++)
			{
				readGrid.get(m[row][col]);
				if (m[row][col] == SPOT || m[row][col] == ZOMBIE)
				{
					m[row][col] = TUNNEL;
				}
			}
		}
		readGrid >> value;
		lives = value;
		readGrid >> value;
		powerPills = value;
		readGrid >> value;
		zombieCount = value;
		readGrid >> value;
		spot.x = value;
		readGrid >> value;
		spot.y = value;
		for (int i = 0; i < 4; i++)
		{
			readGrid >> value;
			zombies[i].x = value;
			readGrid >> value;
			zombies[i].y = value;
			readGrid >> value;
			zombies[i].canMove = value;
		}
	}
}

void showReplay(const char m[][SIZEX], char g[][SIZEX], Item spot, Item zombies[], vector<Item> spotReplay, vector<Item> zombiesReplay) {
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item, Item[]);
	for (int row = 0; row < SIZEY; row++)
	{
		for (int col = 0; col < SIZEX; col++)
		{
			g[row][col] = m[row][col];
		}
	}
	for (Item n : spotReplay)
	{
		spot.x = n.x;
		spot.y = n.y;
		updateGrid(g, m, spot, zombies);
	}
}