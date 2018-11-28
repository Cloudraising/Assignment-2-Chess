#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

// Global constants
const int MAX_ARRAY_SIZE = 18;
const int MIN_ARRAY_SIZE = 8;
const int MAX_PIECES = 72;
const int NOPLAYER = 0;
const int WHITEWINS = 1;
const int REDWINS = 2;
const int NOONEWINS = 0;
const int EMPTY = 0;
const int WHITESOLDIER = 1;
const int WHITEMULE = 2;
const int WHITEKING = 3;
const int REDSOLDIER = 4;
const int REDMULE = 5;
const int REDKING = 6;
const int WHITEPLAYER = 1;
const int REDPLAYER = 2;

void DisplayBoard(int CMCheckersBoard[10][10], int numRowsInBoard);

int CountJumps(int CMCheckersBoard[10][10],
	int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex);
bool IsJump(int CMCheckersBoard[10][10],
	int numRowsInBoard, int player, int xLoc, int yLoc);

int main()
{
	// Declare arrays and initialize to 0
	//int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = { 0, 0 };
	int xIndicesMove[MAX_PIECES] = { 0 };
	int yIndicesMove[MAX_PIECES] = { 0 };
	int xIndicesJump[MAX_PIECES] = { 0 };
	int yIndicesJump[MAX_PIECES] = { 0 };

	// Integer variables
	int numRowsInBoard = 0; // Holds the number of rows and columns that user inputs.
	int player = 1; // Default is white player first
	int jumpPossible = 0; // Holds the return value of countJump function. It is the count of possible jumps in the current board.
	int movePossible = 0; // Holds the return value of countMove1Square. It is the count of possible moves in the current board.
	int moveFrom = 0; // User inputs the value he/she wishes to move.
	int moveTo = 0; // User inputs the value he/she wishes to move to.
	int xMoveFrom = 0; // x coordinate for the square user choses to move.
	int yMoveFrom = 0; // y coordinate for the square user choses to move.
	int xMoveTo = 0; // x coordinate for the square user wants to move to.
	int yMoveTo = 0; // y coordinate for the square user wants to move to.
	int counter = 0; // Counts how many values are in the jump indices.
	int canJump = 0; // Variable for squares that can jump
	int xCanJump = 0; // x coordinate for the square that can jump.
	int yCanJump = 0; // y coordinate for the square that can jump.
	int xDiagCheck = 0; // Difference of x value of xMoveFrom and xMoveTo.
	int yDiagCheck = 0; // Difference of y values of yMoveFrom and yMoveTo.

	// Character variable to end the game.
	char endGame = '\0';

	// Booleans for flags 
	bool validEntry = false; // Flags if user entry is valid.
	bool diagCheck = false; // Flags if user wants to move to a diagonal square.
	bool jumped = false; // Flags if the checker jumped
	bool moved = false; // Boolean to save the return value of MakeMove function.

	// Prompting and reading the the size of the board from the user. 
	for (int counter = 1; counter <= 3; counter++)
	{
		cout << "Enter the number of squares along each edge of the board" << endl; // Prompt the user to input the board size
		cin >> numRowsInBoard;

		if (cin.fail()) // Check if the input is integer. If not, print error message
		{
			cin.clear();
			cin.ignore();
			cerr << "ERROR: Board size is not an integer." << endl;
		}
		else if (numRowsInBoard % 2 == 1) // User input is odd
		{
			cerr << "ERROR: Board size odd." << endl;
		}
		else if (numRowsInBoard > MAX_ARRAY_SIZE) // User input is too big
		{
			cerr << "ERROR: Board size too large." << endl;
		}
		else if (numRowsInBoard < MIN_ARRAY_SIZE) // User input is too small
		{
			cerr << "ERROR: Board size too small." << endl;
		}
		else // Good condition
		{
			break;
		}

		cerr << MIN_ARRAY_SIZE << " <= number of squares <= " << MAX_ARRAY_SIZE << endl;

		if (counter == 3) // If user reaches maximum of three tries, program terminates
		{
			cerr << "ERROR: Too many errors entering the size of the board.\n";
			exit(1);
		}
	}

	//InitializeBoard(myCMCheckersBoard, numRowsInBoard); // Initializing the board.
	int myCMCheckersBoard[10][10] = { 0, 2, 0, 2, 0, 2, 0, 2, 0, 2,
									  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
									  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
									  1, 0, 1, 0, 0, 0, 1, 0, 1, 0,
									  0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
									  0, 0, 0, 0, 0, 0, 4, 0, 0, 0,
									  0, 4, 0, 4, 0, 4, 0, 0, 0, 4,
									  4, 0, 4, 0, 4, 0, 4, 0, 4, 0,
									  0, 4, 0, 4, 0, 4, 0, 4, 0, 4,
									  5, 0, 5, 0, 5, 0, 5, 0, 5, 0 };
	DisplayBoard(myCMCheckersBoard, 10); // Displaying the board.

	jumpPossible = CountJumps(myCMCheckersBoard, numRowsInBoard, WHITEPLAYER, xIndicesJump, yIndicesJump);
	cout << jumpPossible << endl;

	return 0;
}

void DisplayBoard(int CMCheckersBoard[10][10], int numRowsInBoard)
{
	int elementValueTemp = 0;
	int count = 0;
	for (int i = 0; i < numRowsInBoard; i++)
	{
		for (int j = 0; j < numRowsInBoard; j++)
		{
			elementValueTemp = CMCheckersBoard[i][j];
			switch (elementValueTemp)
			{
			case 1: cout << setw(4) << "WS";
				break;
			case 2: cout << setw(4) << "WM";
				break;
			case 3: cout << setw(4) << "WK";
				break;
			case 4: cout << setw(4) << "RS";
				break;
			case 5: cout << setw(4) << "RM";
				break;
			case 6: cout << setw(4) << "RK";
				break;
			default: cout << setw(4) << count;
			}
			count++;
		}
		cout << endl;
	}
	cout << endl << endl;
}

// Function to
// Returns an integer containing the number of the player's checkers that can jump
// Call with 1 for player using white and 2 for player using red
int CountJumps(int CMCheckersBoard[10][10], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
	int counter = 0; // Counter for the number of the player's checker that can jump an opponent's checker.

	// Initialize to -1
	for (int index = 0; index < MAX_PIECES; index++)
	{
		yLocArray[index] = -1;
		xLocArray[index] = -1;
	}

	// Check each squares and if there is a player's checker, call IsJump function to check 
	// if it can jump. If IsJump is true, the counter recording the number of the player's 
	// checkers that can jump is ++. The value of xIndex is placed in the next unused location 
	// of xLocArray and the value of yIndex is placed in the next unused element of array yLocArray.

	// If it is white player's turn
	if (player == WHITEPLAYER)
	{
		for (int rows = 0; rows < numRowsInBoard; rows++) // Check all rows
		{
			for (int columns = 0; columns < numRowsInBoard; columns++) // Check all column
			{
 				if ((CMCheckersBoard[rows][columns] == WHITESOLDIER) || (CMCheckersBoard[rows][columns] == WHITEMULE) || (CMCheckersBoard[rows][columns] == WHITEKING))
				{
					cout << rows << endl;
					if (IsJump(CMCheckersBoard, numRowsInBoard, player, columns, rows))
					{
						xLocArray[counter] = columns;
						yLocArray[counter] = rows;
						counter++;
					}
				}
			}
		}
	}

	// If it is red player's turn
	if (player == REDPLAYER)
	{
		for (int rows = 0; rows < numRowsInBoard; rows++) // Check all rows
		{
			for (int columns = 0; columns < numRowsInBoard; columns++) // Check all column
			{
				if ((CMCheckersBoard[rows][columns] == REDSOLDIER) || (CMCheckersBoard[rows][columns] == REDMULE) || (CMCheckersBoard[rows][columns] == REDKING))
				{
					if (IsJump(CMCheckersBoard, numRowsInBoard, player, columns, rows))
					{
						xLocArray[counter] = columns;
						yLocArray[counter] = rows;
						counter++;
					}
				}
			}
		}
	}
	return counter;
}

// Function checks 
bool CheckList(int intArray1[], int intArray2[], int xIndex, int yIndex)
{
	for (int count = 0; count < MAX_PIECES; count++)
	{
		if (intArray1[count] == yIndex)
		{
			if (intArray2[count] == xIndex)
			{
				return true;
			}
		}
	}
	return false;
}

bool IsJump(int CMCheckersBoard[10][10], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER)
	{

		cout << xLoc << endl;
		// If checker is a mule or soldier:
		// Depending on which column checker is in, perform right and left checks differently
 		if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE)
		{
			if (xLoc = 0) // At leftmost column
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == 1) // At second to leftmost column
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 1) // At rightmost column
			{
				// Right Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][0] == REDSOLDIER || CMCheckersBoard[yLoc + 1][0] == REDMULE || CMCheckersBoard[yLoc + 1][0] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][1] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 2) // At second to rightmost column
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][0] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			// Not in the rightmost 2 columns nor the leftmost 2 columns
			else
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
		}

		// If the checker is a king
		// also test diagnally backwards left and right in addition to diagnally forwards left and right
		else if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
		{
			if (xLoc = 0)
			{
				// Forward Diagnal checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal checks
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == 1)
			{
				// Forward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				// Forward Diagnal Checks
					// Right Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][0] == REDSOLDIER || CMCheckersBoard[yLoc + 1][0] == REDMULE || CMCheckersBoard[yLoc + 1][0] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][1] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
					// Right Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][0] == REDSOLDIER || CMCheckersBoard[yLoc - 1][0] == REDMULE || CMCheckersBoard[yLoc - 1][0] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][1] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 2)
			{
				// Foward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][0] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][0] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			// Not in the rightmost 2 columns nor the leftmost 2 columns
			else
			{
				// Forward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backwards Diagnal Checks
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
		}
		else
			return false;
	}
	else if (player == REDPLAYER)
	{
		// If checker is a mule or soldier:
		// Depending on which column checker is in, perform right and left checks differently
		if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE)
		{
			if (xLoc = 0)
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == 1)
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				// Right Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][0] == WHITEMULE || CMCheckersBoard[yLoc - 1][0] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][1] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 2)
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][0] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			// Not in the rightmost 2 columns nor the leftmost 2 columns
			else
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}

		}
		else if (CMCheckersBoard[yLoc][xLoc] == REDKING)
		{
			if (xLoc = 0)
			{
				// Forward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal checks
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == 1)
			{
				// Forward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				// Forward Checks
					// Right Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][0] == WHITEMULE || CMCheckersBoard[yLoc - 1][0] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][1] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][0] == WHITEMULE || CMCheckersBoard[yLoc + 1][0] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][1] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 2)
			{
				// Forward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][0] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Checks
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][0] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			// Not in the rightmost 2 columns nor the leftmost 2 columns
			else
			{
				// Forward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
		}
		else
			return false;
	}
	else
		return false;
}