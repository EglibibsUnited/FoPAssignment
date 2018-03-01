//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
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
const char SPOT('@');   	//spot
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
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void displayStartScreen();
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot);
	void paintGame(const char g[][SIZEX], string mess, int lives, string playerName, int powerPills, char m[][SIZEX]);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, int& lives, char m[][SIZEX], int& powerPills);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot);
	void playerData(string playerName, int lives);
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	string message("LET'S START...");	//current message to player
	int lives = 3;						// Initialise Spot with 3 lives //
	int powerPills = 8;					// Initialise the game with 8 power pills //

	Seed();								//seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");

	displayStartScreen();
	string playerName;
	cin >> playerName;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	Clrscr();

	ofstream fout;
	string filename = playerName + ".txt";
	fout.open(filename, ios::out);


	initialiseGame(grid, maze, spot);	//initialise grid (incl. walls and spot)
	paintGame(grid, message, lives, playerName, powerPills, maze);			//display game info, modified grid and messages
	int key;							//current key selected by player
	do {
		key = getKeyPress(); 	//read in selected key: arrow or letter command
		key = toupper(key);
		if (isArrowKey(key))
		{
			updateGameData(grid, spot, key, message, lives, maze, powerPills);		//move spot in that direction
			updateGrid(grid, maze, spot);					//update grid information
		}
		else
			message = "INVALID KEY!";	//set 'Invalid key' message
    
		paintGame(grid, message, lives, playerName, powerPills, maze);		//display game info, modified grid and messages
	} while (!wantsToQuit(key) && lives >= 0);		//while user does not want to quit and they still have lives left //
	playerData(playerName, lives);
	endProgram();						//display final message
	return 0;
}


//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void displayStartScreen()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clDarkGrey, clYellow, 5, 2, "--------------------------");
	showMessage(clDarkGrey, clYellow, 5, 3, "|    SPOT AND ZOMBIES    |");
	showMessage(clDarkGrey, clYellow, 5, 4, "--------------------------");

	showMessage(clDarkGrey, clYellow, 5, 6, "    Group SE1_5 - 2018    ");
	showMessage(clDarkGrey, clYellow, 5, 7, " ------------------------ ");
	showMessage(clDarkGrey, clYellow, 5, 8, " * Michael Elsom 27035059 ");
	showMessage(clDarkGrey, clYellow, 5, 9, " * James Nelhams 27021413 ");
	showMessage(clDarkGrey, clYellow, 5, 10, " * Jake Stringer 27003087 ");

	showMessage(clDarkGrey, clYellow, 40, 2, "Date: " + GetDate());
	showMessage(clDarkGrey, clYellow, 40, 3, "Time: " + GetTime());

	showMessage(clDarkGrey, clYellow, 40, 6, "        Controls       ");
	showMessage(clDarkGrey, clYellow, 40, 7, "-----------------------");
	showMessage(clDarkGrey, clYellow, 40, 8, "| Movement: Arrows    |");
	showMessage(clDarkGrey, clYellow, 40, 9, "| Attack: X           |");
	showMessage(clDarkGrey, clYellow, 40, 10, "| Freeze: F           |");
	showMessage(clDarkGrey, clYellow, 40, 11, "| Quit: Q             |");
	showMessage(clDarkGrey, clYellow, 40, 12, "-----------------------");

	showMessage(clDarkGrey, clYellow, 5, 14, "Enter your name to start:");
	showMessage(clBlack, clRed, 31, 14, " ");
}

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot)
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX]);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], Item b);

	setInitialMazeStructure(maze);		//initialise maze
	setSpotInitialCoordinates(spot, maze);
	updateGrid(grid, maze, spot);		//prepare grid
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

void setInitialMazeStructure(char maze[][SIZEX])
{ //set the position of the walls in the maze
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX];

	for (int column = 0; column < SIZEY; column++)
	{
		for (int row = 0; row < SIZEX; row++)
		{
			if (column == 0 || column == SIZEY-1 || row == 0 || row == SIZEX-1)
			{
				// Just a row of walls //
				initialMaze[column][row] = '#';
			} else
			{
				initialMaze[column][row] = ' ';
			}
		}
	}

	initialMaze[1][1] = 'Z';
	initialMaze[1][SIZEX-2] = 'Z';
	initialMaze[SIZEY-2][1] = 'Z';
	initialMaze[SIZEY-2][SIZEX-2] = 'Z';

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

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item spot)
{ //update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item spot);

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, spot);	//set spot in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

//---------------------------------------------------------------------------
//----- move items on the grid
//---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess, int& lives, char maze[][SIZEX], int& powerPills)
{ //move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
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
			break;
		case WALL:  		//hit a wall and stay there
							//TODO: remove alarm when bumping into walls - too annoying
			cout << '\a';	//beep the alarm
			mess = "CANNOT GO THERE!";
			break;
		case HOLE:			// Fall into a hole //
			spot.y += dy;
			spot.x += dx;
			lives--;
			break;
		case POWERPILL:		// Eat power pill //
			spot.y += dy;
			spot.x += dx;
			maze[spot.y][spot.x] = ' ';
			lives++;
			powerPills--;
			break;

	}

	// Move Zombies //
	//for (int column = 0; column < SIZEY; column++)
	//{
	//	for (int row = 0; row < SIZEX; row++)
	//	{
	//		if (maze[row][column] == 'Z')
	//		{
	//			// Move the zombie //
	//			maze[row][column] = ' ';
	//			maze[row + 1][column + 1] = 'Z';
	//		}
	//	}
	//}

	if (lives < 0)
	{
		mess = "YOU LOSE!";
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
void paintGame(const char g[][SIZEX], string mess, int lives, string playerName, int powerPills, char m[][SIZEX])
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX], char m[][SIZEX]);
	//TODO: Change the colour of the messages
	//display game title
	showMessage(clBlack, clYellow, 0, 0, "___GAME___");

	// Display lives left //
	stringstream ss;
	if (lives < 0)
	{
		ss << "Lives: 0H NO";
	} else
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
	showMessage(clDarkGrey, clYellow, 40, 9, "| Attack: X           |");
	showMessage(clDarkGrey, clYellow, 40, 10, "| Freeze: F           |");
	showMessage(clDarkGrey, clYellow, 40, 11, "| Quit: Q             |");
	showMessage(clDarkGrey, clYellow, 40, 12, "-----------------------");

	int score(0);
	showMessage(clDarkGrey, clYellow, 40, 14, "Player: " + playerName + ": " + to_string(score)+ "  ");
	showMessage(clBlack, clGreen, 40, 17, ss.str());
	showMessage(clBlack, clGreen, 40, 18, pps.str());

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 25, mess);


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
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 29);
				cout << g[row][col];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			} else if (g[row][col] == HOLE)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << g[row][col];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			} else if (g[row][col] == ZOMBIE)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				cout << g[row][col];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

				// TODO - Update zombie position //
			}
			else if (g[row][col] == POWERPILL)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				cout << g[row][col];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			} else
			{
				cout << g[row][col];	//output cell content
			}
		}
		cout << endl;
	}
}

void playerData(string playerName, int lives)
{
	ifstream getScore;
	ofstream writeScore;
	getScore.open(playerName + ".txt", ios::in);
	int value, sum(0);
	getScore >> value;
	while (getScore)
	{
		getScore >> value;
		sum++;
	}
	getScore.close();
	writeScore.open(playerName + ".txt", ios::out);
	if (sum > 0)
	{
		if (value > lives) 
		{
			writeScore << lives;
		}		
	}
	writeScore << lives;
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clRed, clYellow, 40, 21, "");
	system("pause");	//hold output screen until a keyboard key is hit
}