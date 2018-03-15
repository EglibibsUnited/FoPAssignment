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
#include <vector>
using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for Seed, Random
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"
#include "MenuUtils.h"

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

struct GameData {
	int holes;
	int powerPills;
	int lives;
	int numProtectedMoves;
	int level;
};

int main()
{
	void displayStartScreen();
	bool menuScreen(string playerName);
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

	do
	{
		menuScreen(playerName);
		Clrscr();
	} while (!menuScreen(playerName));


	endProgram();						//display final message
	return 0;
}

//---------------------------------------------------------------------------
//----- Run game
//---------------------------------------------------------------------------

void runGame(string playerName, int levelNumber)
{
	// TODO: Replace all instances of 'lives' with GameData's lives attribute //

	//function declarations (prototypes)
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot, Item zombies[], GameData level);
	void paintGame(const char g[][SIZEX], string mess, string playerName, char m[][SIZEX], int zombieCount, GameData level);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	bool isCheatCode(const int k);
	int  getKeyPress();
	void changeCursorVisibility(bool);
	void runCheatCode(const int k, int& powerPills, Item zombies[], bool& zombFreeze, int& zombieCount);

	void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, char maze[][SIZEX], Item zombies[], int& powerpillTouch, int moveCounter, bool zombiesMove, int& zombieCount, bool& powerpillTouched, vector<Item>& gameReplay, GameData& level);


	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot, Item zombies[]);
	void powerpillProtection(int moveCounter, int& powerpillTouch, Item& spot, bool& powerpillTouched);
	int getPlayerScore(string playerName);
	void playerData(string playerName, int lives);
	bool hasWon(Item zombies[], GameData level);
	void checkPlayerScore(string playerName);
	void saveGame(const char grid[][SIZEX], string playerName ,int lives ,int powerPills, int moveCounter ,int zombieCount, Item spot, Item zombies[]);
	void loadGame(char maze[][SIZEX], string playerName, int& lives, int& powerPills, int& moveCounter, int& zombieCount, Item& spot, Item zombies[], bool& powerpillTouched);
	void showReplay(char g[][SIZEX], char m[][SIZEX], Item spot, Item zombies[], vector<Item> gameReplay, GameData level, string playerName, int zombieCount);

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	Item zombies[4];					// Array of zombies // 
	vector<Item> gameReplay;			// A constantly updated list of Items to recreate a 'replay' if needs be //
	string message("LET'S START...");	//current message to player

	int zombieCount = 4;				//zombie count for checking game finish
	bool zombiesMove(true);				//For zombie freeze cheat code
	bool hasCheated(false);
	bool powerpillTouched(false);

	int moveCounter(0), powerpillTouch(0);

	changeCursorVisibility(false);
	checkPlayerScore(playerName);		// Check for previous high scores, handle save files etc. //

	// Create a blank level //
	GameData level;
	switch (levelNumber)
	{
	case 1:
		level.holes = 12;
		level.lives = 8;
		level.powerPills = 8;
		level.numProtectedMoves = 10;	
		break;
	case 2:
		level.holes = 5;
		level.lives = 5;
		level.powerPills = 5;
		level.numProtectedMoves = 8;
		break;
	case 3:
		level.holes = 2;
		level.lives = 3;
		level.powerPills = 2;
		level.numProtectedMoves = 5;
		break;
	}
	level.level = levelNumber;

	initialiseGame(grid, maze, spot, zombies, level);	// Initialise grid (incl. walls and spot) //

	paintGame(grid, message, playerName, maze, zombieCount, level);			//display game info, modified grid and messages
	
	int key;							//current key selected by player
	do {
		key = getKeyPress(); 	//read in selected key: arrow or letter command
		key = toupper(key);
		if (isArrowKey(key))
		{
			updateGameData(grid, spot, key, message, maze, zombies, powerpillTouch, moveCounter, zombiesMove, zombieCount, powerpillTouched, gameReplay, level);		//move spot in that direction
			updateGrid(grid, maze, spot, zombies);					//update grid information
			moveCounter++;
		}
		if (isCheatCode(key))
		{
			runCheatCode(key, level.powerPills, zombies, zombiesMove, zombieCount);
			hasCheated = true;
			updateGameData(grid, spot, key, message, maze, zombies, powerpillTouch, moveCounter, zombiesMove, zombieCount, powerpillTouched, gameReplay, level);
			updateGrid(grid, maze, spot, zombies);
		}
		if (key == 'S')
		{
			saveGame(grid, playerName, level.lives, level.powerPills, moveCounter, zombieCount, spot, zombies);
		}
		if (key == 'L')
		{
			loadGame(maze, playerName, level.lives, level.powerPills, moveCounter, zombieCount, spot, zombies, powerpillTouched);
			updateGrid(grid, maze, spot, zombies);
		}
		if (key == 'R')
		{
			showReplay(maze, grid, spot, zombies, gameReplay, level, playerName, zombieCount);
		}
		paintGame(grid, message, playerName, maze, zombieCount, level);		//display game info, modified grid and messages
	} while (!wantsToQuit(key) && level.lives >= 0 && hasWon(zombies, level) == false); // Game quits if user presses Q, Spot has no lives or wins the game //
	
	if (level.lives < 0 && !wantsToQuit(key))
	{
		showMessage(clRed, clYellow, 40, 24, "Unlucky, try again next time!");
	}
	else if (!wantsToQuit(key))
	{
		showMessage(clYellow, clRed, 40, 24, "Congratulations!! You Win!!");
		levelNumber++; // TODO: Make levels
		Sleep(1000);
	}

	if (!hasCheated)
	{
		playerData(playerName, level.lives);
	}

	changeCursorVisibility(true);
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


void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, Item zombies[], GameData level)
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX], Item zombies[], GameData level);
	void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX]);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], Item b, Item zombies[]);

	setInitialMazeStructure(maze, zombies, level);		//initialise maze
	setSpotInitialCoordinates(spot, maze);
	updateGrid(grid, maze, spot, zombies);		//prepare grid
}

void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX])
{ //set spot coordinates inside the grid at random at beginning of game
	int y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
	int x = Random(SIZEX - 2);      //horizontal coordinate in range [1..(SIZEX - 2)]
	while (maze[y][x] == WALL || maze[y][x] == ZOMBIE || maze[y][x] == HOLE || maze[y][x] == POWERPILL)
	{
		y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		x = Random(SIZEX - 2);		//horizontal coordinate in range [1..(SIZEX - 2)]
	}
	spot.y = y;
	spot.x = x;
}

void setInitialMazeStructure(char maze[][SIZEX], Item zombies[], GameData level)
{ //set the position of the walls in the maze
  //initialise maze configuration
	void loadInitialLevel(char initialMaze[][SIZEX], GameData level);
	char initialMaze[SIZEY][SIZEX];

	loadInitialLevel(initialMaze, level);

	// Create level.zombies - set their default X and Y positions and initialise their X and Y coords to the same //
	zombies[0].defaultY = 1; zombies[0].defaultX = 1; zombies[0].y = 1; zombies[0].x = 1;
	zombies[1].defaultY = 1; zombies[1].defaultX = SIZEX - 2; zombies[1].y = 1; zombies[1].x = SIZEX - 2;
	zombies[2].defaultY = SIZEY - 2; zombies[2].defaultX = 1; zombies[2].y = SIZEY - 2; zombies[2].x = 1;
	zombies[3].defaultY = SIZEY - 2; zombies[3].defaultX = SIZEX - 2; zombies[3].y = SIZEY - 2; zombies[3].x = SIZEX - 2;
	for (int zomb = 0; zomb < 4; zomb++)
	{
		zombies[zomb].symbol = ZOMBIE;
		//initialMaze[zombies[zomb].y][zombies[zomb].x] = zombies[zomb].symbol;
	}

	for (int holesCount = level.holes; holesCount > 0; holesCount--) // Add holes //
	{
		int x = Random(SIZEX - 2);
		int y = Random(SIZEY - 2);
		while (initialMaze[y][x] == WALL || initialMaze[y][x] == ZOMBIE)
		{
			x = Random(SIZEX - 2);
			y = Random(SIZEY - 2);
		}
		initialMaze[y][x] = HOLE;
	}

	for (int powerpills = level.powerPills; powerpills > 0; powerpills--) // Add power pills //
	{
		int x = Random(SIZEX - 2);
		int y = Random(SIZEY - 2);
		while (initialMaze[y][x] == WALL || initialMaze[y][x] == ZOMBIE || initialMaze[y][x] == HOLE)
		{
			x = Random(SIZEX - 2);
			y = Random(SIZEY - 2);
		}
		initialMaze[y][x] = POWERPILL;
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


void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, char maze[][SIZEX], Item zombies[], int& powerpillTouch, int moveCounter, bool zombiesMove, int& zombieCount, bool& powerpillTouched, vector<Item>& gameReplay, GameData& level)

{ //move spot in required direction
	bool isArrowKey(const int k);
	bool isCheatCode(const int k);
	void setKeyDirection(int k, int& dx, int& dy);

	if (isArrowKey(key))
	{
		void powerpillProtection(int moveCounter, int& powerpillTouch, Item& spot, bool& powerpillTouched, GameData level);
		assert(isArrowKey(key));

		//reset message to blank
		mess = "                                         ";		//reset message to blank

		int dx(0), dy(0);					//calculate direction of movement for given key
		setKeyDirection(key, dx, dy);

		//check new target position in grid and update game data (incl. spot coordinates) if move is possible
		switch (g[spot.y + dy][spot.x + dx])
		{			//...depending on what's on the target position in grid...
		case TUNNEL:		//can move
			spot.y += dy;	//go in that Y direction
			spot.x += dx;	//go in that X direction
			if (powerpillTouched == true)
				powerpillProtection(moveCounter, powerpillTouch, spot, powerpillTouched, level);
				
			break;
		case WALL:  		//hit a wall and stay there
			mess = "CANNOT GO THERE!";
			break;
		case HOLE:		// Fall into a hole //
			spot.y += dy;
			spot.x += dx;
			level.lives--;
			break;
		case POWERPILL:		// Eat power pill // Task 2a Function Call
			spot.y += dy;
			spot.x += dx;
			maze[spot.y][spot.x] = ' ';
			powerpillTouch = moveCounter;
			powerpillTouched = true;
			powerpillProtection(moveCounter, powerpillTouch, spot, powerpillTouched, level);
			level.lives++;
			level.powerPills--;
			break;
		case ZOMBIE:

			spot.y += dy;
			spot.x += dx;
			if (powerpillTouched)
			{
				for (int i = 0; i < 4; i++)
				{
					if (maze[zombies[i].y][zombies[i].x] == SPOT || maze[spot.y][spot.x] == ZOMBIE)
					{
						zombies[i].canMove = false;
						zombies[i].symbol = ' ';
						zombies[i].x = -1;
						zombies[i].y = -1;
						zombieCount--;
					}
				}

			}
		}

		gameReplay.push_back(spot); // Add spot to the replay //

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

									level.lives--;

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

										level.lives--;
									
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
						zombieCount--;
					}
					else
					{
						zombies[zomb].y = zombies[zomb].defaultY;
						zombies[zomb].x = zombies[zomb].defaultX;
						
						level.lives--;
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

			gameReplay.push_back(zombies[zomb]); // Add each zombie to the replay //
		}

		if (level.lives < 0)
		{
			mess = "YOU LOSE!";
			cin.get();
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

void paintGame(const char g[][SIZEX], string mess, string playerName, char m[][SIZEX], int zombieCount, GameData level)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void paintGrid(const char g[][SIZEX], char m[][SIZEX]);
	int getPlayerScore(string playerName);
	//display game title
	showMessage(clBlack, clYellow, ((SIZEX - 2) / 2 - 7), 1, "SPOT AND ZOMBIES");
	SelectBackColour(clDarkGrey);
	SelectTextColour(clYellow);

	// Display lives left //
	stringstream ss;
	if (level.lives < 0)
	{
		ss << "Lives: 0H NO";
	}
	else
	{
		ss << "Lives:         ";
		showMessage(clBlack, clGreen, 40, 17, ss.str());
		ss.str("");
		ss << "Lives: " << level.lives;
	}
	stringstream pps;
	if (level.powerPills < 0)
	{
		pps << "Power Pills Remaining: 0";
	}
	else
	{
		pps << "Power Pills Remaining: " << level.powerPills;
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

	string levelString = to_string(level.level);
	showMessage(clBlack, clGreen, 40, 24, "DIFFICULTY " + levelString);

	string score = to_string(getPlayerScore(playerName));
	showMessage(clBlack, clGreen, 40, 20, playerName);

	if (stoi(score) <= -1)
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

void powerpillProtection(int moveCounter, int& powerpillTouch, Item& spot, bool& powerpillTouched, GameData level)
{
	if (moveCounter - level.numProtectedMoves < powerpillTouch && powerpillTouched)
	{
		spot.symbol = '$';
	}
	else if (moveCounter - level.numProtectedMoves > powerpillTouch && powerpillTouched)
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

bool hasWon(Item zombies[], GameData level)
{
	int zombiesRemaining = 0;
	for (int zomb = 0; zomb < 4; zomb++)
	{
		if (zombies[zomb].canMove)
		{
			zombiesRemaining++;
		}
	}
	return (zombiesRemaining == 0 && level.powerPills == 0);
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


// SAVE AND LOAD - REQUIRES FOLDER CALLED SAVES //
void saveGame(const char g[][SIZEX], string playerName, int lives, int powerPills, int powerMovesRemain, int zombieCount, Item spot, Item zombies[]) {
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
	writeGrid << powerMovesRemain;
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

// LOAD //
void loadGame(char m[][SIZEX], string playerName, int& lives, int& powerPills, int& powerMovesRemain, int& zombieCount, Item& spot, Item zombies[], bool& powerpillTouched) {
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
				if (m[row][col] == '$')
				{
					powerpillTouched = true;
					spot.symbol = '$';
				}
				if (m[row][col] == SPOT || m[row][col] == ZOMBIE || m[row][col] == '$')
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
		powerMovesRemain = value;
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
			zombies[i].symbol = ZOMBIE;   // Resets the zombies symbol for example if the zombie has fallen down a hole prior to load //
		}
	}
}

void showReplay(char g[][SIZEX], char m[][SIZEX], Item spot, Item zombies[], vector<Item> gameReplay, GameData level, string playerName, int zombieCount)
{
	void paintGame(const char g[][SIZEX], string mess, string playerName, char m[][SIZEX], int zombieCount, GameData level);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot, Item zombies[]);

	char tempGrid[SIZEY][SIZEX];

	for (int replay = 0; replay < gameReplay.size(); replay += 5)
	{
			for (int row = 0; row < SIZEY; row++)
			{
				for (int col = 0; col < SIZEX; col++)
				{
					tempGrid[row][col] = m[row][col];
					if (m[row][col] == SPOT || m[row][col] == ZOMBIE || m[row][col] == '$')
					{
						m[row][col] = TUNNEL;
					}

				}
			}
		
		

		// i = Spot, i+1, i+2, i+3 & i+4 = Zombies //
		spot.x = gameReplay[replay].x;
		spot.y = gameReplay[replay].y;

		zombies[0].x = gameReplay[replay + 1].x; zombies[0].y = gameReplay[replay + 1].y; zombies[0].symbol = ZOMBIE;
		zombies[1].x = gameReplay[replay + 2].x; zombies[1].y = gameReplay[replay + 2].y; zombies[1].symbol = ZOMBIE;
		zombies[2].x = gameReplay[replay + 3].x; zombies[2].y = gameReplay[replay + 3].y; zombies[2].symbol = ZOMBIE;
		zombies[3].x = gameReplay[replay + 4].x; zombies[3].y = gameReplay[replay + 4].y; zombies[3].symbol = ZOMBIE;

		
		updateGrid(tempGrid, m, spot, zombies);
		paintGame(tempGrid, "", playerName, m, zombieCount, level);

		Sleep(50);
	}
	

	for (int i = 0; i < 4; i++)
	{
		if (!zombies[i].canMove)
		{
			zombies[i].symbol = TUNNEL;
		}
		
	}
}

void loadInitialLevel(char initialMaze[][SIZEX], GameData level)
{
	ifstream loadMap;
	string levelNumber = to_string(level.level);
	loadMap.open(".//Levels/level" + levelNumber + ".spot", ios::in);
	for (int row(0); row < SIZEY; row++)
	{
		for (int col(0); col < SIZEX; col++)
		{
			loadMap.get(initialMaze[row][col]);	// Get each row and column into the initialMaze array //
		}
	}
}