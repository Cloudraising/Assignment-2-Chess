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

// Function prototypes

void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],
	int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex);

int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],
	int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],
	int numRowsInBoard, int player, int xLoc, int yLoc);

bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],
	int numRowsInBoard, int player, int xLoc, int yLoc);

bool MakeMove(int CMChekersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],
	int numRowsInBoard, int player, int fromSquareNum,
	int toSquareNum, bool &jumped);

bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

int main()
{
	// Declare arrays and initialize to 0
	int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = { 0, 0 };
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

	InitializeBoard(myCMCheckersBoard, numRowsInBoard); // Initializing the board.
	DisplayBoard(myCMCheckersBoard, numRowsInBoard); // Displaying the board.

	while (CheckWin(myCMCheckersBoard, numRowsInBoard) == false)
	{
		jumpPossible = CountJumps(myCMCheckersBoard, numRowsInBoard, player, xIndicesJump, yIndicesJump);
		movePossible = CountMove1Squares(myCMCheckersBoard, numRowsInBoard, player, xIndicesMove, yIndicesMove);
		jumped = false; // Reset the jumped flag

		// There is no possible move, then game is over
		if (jumpPossible == 0 && movePossible == 0)
		{
			if (player == WHITEPLAYER)
			{
				cout << "White is unable to move.\n" << "GAME OVER, Red has won.\n" << "Enter any character to close the game.\n";
				cin >> endGame;
				exit(2);
			}
			else if (player == REDPLAYER)
			{
				cout << "Red is unable to move.\n" << "GAME OVER, White has won.\n";
				cin >> endGame;
				exit(3);
			}
		}
		// There is at least one possible move, then ask user to input the square to move.
		else if (movePossible >= 1 || jumpPossible >= 1)
		{
			if (player == WHITEPLAYER)
			{
				cout << "White takes a turn.\n";
			}
			if (player == REDPLAYER)
			{
				cout << "Red takes a turn.\n";
			}

			// User inputs the square he/she wants to move.
			while (!validEntry)
			{
				cout << "Enter the square number of the checker you want to move\n";
				cin >> moveFrom;

				// Check if the input is integer. If not, print error message
				if (cin.fail())
				{
					cin.clear();
					cin.ignore();
					cerr << "ERROR: You did not enter an integer" << endl << "Try again" << endl;
					continue;
				}
				// Check if the user input is located on the board.
				if (moveFrom >= (numRowsInBoard*numRowsInBoard) || moveFrom < 0)
				{
					cerr << "ERROR: That square is not on the board." << endl << "Try again" << endl;
					continue;
				}

				// To evaluate the square user chose, get x and y coordinate of the square.
				xMoveFrom = moveFrom % numRowsInBoard;
				yMoveFrom = moveFrom / numRowsInBoard;

				// If the chosen square is occupied by an opponent's checker, error message appears
				if (player == WHITEPLAYER)
				{
					if (myCMCheckersBoard[yMoveFrom][xMoveFrom] == REDSOLDIER || myCMCheckersBoard[yMoveFrom][xMoveFrom] == REDMULE || myCMCheckersBoard[yMoveFrom][xMoveFrom] == REDKING)
					{
						cerr << "ERROR: That square contains an opponent's checker." << endl << "Try again" << endl;
						continue;
					}
				}
				if (player == REDPLAYER)
				{
					if (myCMCheckersBoard[yMoveFrom][xMoveFrom] == WHITESOLDIER || myCMCheckersBoard[yMoveFrom][xMoveFrom] == WHITEMULE || myCMCheckersBoard[yMoveFrom][xMoveFrom] == WHITEKING)
					{
						cerr << "ERROR: That square contains an opponent's checker." << endl << "Try again" << endl;
						continue;
					}
				}

				// Checks if the square contains no checker.
				if (myCMCheckersBoard[yMoveFrom][xMoveFrom] == EMPTY)
				{
					cerr << "ERROR: That square is empty." << endl << "Try again" << endl;
					continue;
				}

				// There is a checker that can jump other than the chosen square that can only move one square, then
				// the checker that can jump has to be chosen. First, the xIndicesJump array is examined to determine
				// whether there are any squares that can jump.
				counter = 0; // resetting the counter.

				for (int i = 0; i < MAX_PIECES; i++)
				{
					if (xIndicesJump[i] != -1)
						counter++;
				}

				// If the chosen square is not in the list of squares that can jump, and the counter for jump available
				// square is bigger or equal to 1, then there is another square that can jump.
				if (!CheckList(yIndicesJump, xIndicesJump, xMoveFrom, yMoveFrom) && counter >= 1)
				{
					cerr << "ERROR: You can jump with another checker, you may not move your chosen checker." << endl
						<< "You can jump using checkers on the following squares:";
					// Find what other squares can jump and output them
					for (int i = 0; i < counter; i++)
					{
						xCanJump = xIndicesJump[i];
						yCanJump = yIndicesJump[i];

						canJump = yCanJump * numRowsInBoard + xCanJump;
						cerr << " " << canJump;
					}
					cerr << "\nTry again" << endl;
					continue;
				}

				//If the chosen checker cannot jump or move, then print the error message
				if (!CheckList(yIndicesJump, xIndicesJump, xMoveFrom, yMoveFrom) && !CheckList(yIndicesMove, xIndicesMove, xMoveFrom, yMoveFrom))
				{
					cerr << "ERROR: There is no legal move for this checker" << endl << "Try again" << endl;
					continue;
				}
				validEntry = true;
			}
	
			validEntry = false; // Used to repeat the same procedure with the destination square.

			// User inputs square number that he/she wants to move to.
			label: while (!validEntry)
			{
				if (!jumped)
			    {
				    cout << "Enter the square number of the square you want to move your checker to\n";
			    }
			    else
			    {
				    cout << "You can jump again, Please enter the next square you wish to move your checker to\n";
			    }
			    cin >> moveTo;

			    // Check if the input is integer. If not, print error message
			    if (cin.fail())
			    {
				    cin.clear();
				    cin.ignore();
				    cerr << "ERROR: You did not enter an integer" << endl << "Try again" << endl;
				   continue;
			    }
			    // Check if the user input is located on the board.
			    if (moveTo >= (numRowsInBoard*numRowsInBoard) || moveTo < 0)
			    {
				    cerr << "ERROR: It is not possible to move to a square that is not on the board." << endl << "Try again" << endl;
				    continue;
			    }

			    // To evaluate the square user choses, get x and y coordinate of the square.
			    xMoveTo = moveTo % numRowsInBoard;
			    yMoveTo = moveTo / numRowsInBoard;

			    // Check if the square is already occupied.
			    if (myCMCheckersBoard[yMoveTo][xMoveTo] != EMPTY)
			    {
				    cerr << "ERROR: It is not possible to move to a square that is already occupied." << endl << "Try again" << endl;
				    continue;
			    }

			    // Decide if the user wants to move the square diagonally.
			    xDiagCheck = abs(xMoveFrom - xMoveTo);
			    yDiagCheck = abs(yMoveFrom - yMoveTo);

			    if ((xDiagCheck == 1 && yDiagCheck == 1) || (xDiagCheck == (numRowsInBoard - 1) && yDiagCheck == 1))
			    {
				    diagCheck = true;
			    }
			    else
			    {
				    diagCheck = false;
			    }

			    // Checks if the square can jump when it is requested to move diagonally.
			    if (CheckList(yIndicesJump, xIndicesJump, xMoveFrom, yMoveFrom) && diagCheck)
			    {
				    cerr << "ERROR: You can jump with this checker, you must jump not move 1 space." << endl << "Try again" << endl;
				    continue;
			    }
			    validEntry = true;
		    }
 
			// Reset the flag in case the program have to reprocess the error conditions.
			validEntry = false;

			// Store the return value of the MakeMove function whether the checker is moved.
			moved = MakeMove(myCMCheckersBoard, numRowsInBoard, player, moveFrom, moveTo, jumped);
				   
			// The checker is not moved.
			if (moved == false)
			{
				cerr << "ERROR: Moving to that square is not legal, Try again." << endl;
				goto label;
			}
			// When move is successful, display the changed board.
			DisplayBoard(myCMCheckersBoard, numRowsInBoard);

			// The checker is moved and if it is jumped, the program checks if the checker can jump again by calling IsJump.
			if (jumped)
			{
				if (IsJump(myCMCheckersBoard, numRowsInBoard, player, xMoveTo, yMoveTo))
				{
					xMoveFrom = xMoveTo; // Overwrite because the checker is jumped and it can also jump to a new location.
					yMoveFrom = yMoveTo;
					moveFrom = moveTo;

					jumpPossible = CountJumps(myCMCheckersBoard, numRowsInBoard, player, xIndicesJump, yIndicesJump);
					movePossible = CountMove1Squares(myCMCheckersBoard, numRowsInBoard, player, xIndicesMove, yIndicesMove);

					goto label;
				}
				// IsJump is false, which means the checker cannot jump anymore. The move is completed and the game will continue. Player switches.
				else
				{
					if (player == WHITEPLAYER)
					{
						player = REDPLAYER;
						continue;
					}
					else if (player == REDPLAYER)
					{
						player = WHITEPLAYER;
						continue;
					}
				}
			}
			// If the checker is moved instead of jump, the player switches.
			else
			{
				if (player == WHITEPLAYER)
				{
					player = REDPLAYER;
					continue;
				}
				else if (player == REDPLAYER)
				{
					player = WHITEPLAYER;
					continue;
				}
			}
		}
	}
	cout << "Enter any character to terminate the game then press the enter key\n";
	cin >> endGame;

	return 0;
}

// Function to set up initial board layout
// Algorithm works for all even values of numRowsInBoard
void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	// Populate mules
	for (int j = 0; j < numRowsInBoard; j += 2)
	{
		CMCheckersBoard[0][j + 1] = WHITEMULE; // Populating white mules
		CMCheckersBoard[numRowsInBoard - 1][j] = REDMULE; // populating red mules
	}
	// Populate white soldiers, starting from second row.
	for (int i = 1; i < numRowsInBoard / 2 - 1; i++)
	{
		for (int j = 0; j < numRowsInBoard - 1; j += 2)
		{
			// if row element number is odd
			if (i % 2 == 1)
			{
				CMCheckersBoard[i][j] = WHITESOLDIER;
			}
			else
				CMCheckersBoard[i][j + 1] = WHITESOLDIER;
		}
	}
	// Populate red soldiers starting from second to last row 
	for (int i = numRowsInBoard - 2; i > numRowsInBoard / 2; i--)
	{
		for (int j = 0; j < numRowsInBoard; j += 2)
		{
			// if row element number is odd
			if (i % 2 == 1)
			{
				CMCheckersBoard[i][j] = REDSOLDIER;
			}
			else
				CMCheckersBoard[i][j + 1] = REDSOLDIER;
		}
	}
}

// Function to display board layout to console using text graphics
void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
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
int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
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

	for (int rows = 0; rows < numRowsInBoard; rows++) // Check all rows
	{
		for (int columns = 0; columns < numRowsInBoard; columns++) // Check all column
		{
			
			if (IsJump(CMCheckersBoard, numRowsInBoard, player, columns, rows))
			{
				xLocArray[counter] = columns;
				yLocArray[counter] = rows;
				counter++;
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

int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
	int counter = 0; // Counter for the number of the player's checker that can move.

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

	for (int rows = 0; rows < numRowsInBoard; rows++) // Check all rows
	{
		for (int columns = 0; columns < numRowsInBoard; columns++) // Check all column
		{
				
			if (IsMove1Square(CMCheckersBoard, numRowsInBoard, player, columns, rows))
			{
				xLocArray[counter] = columns;
				yLocArray[counter] = rows;
				counter++;
			}
		}
	}
	return counter;
}

// Function considers one checker
// Call with 1 for player using white and 2 for player using red
bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER)
	{
		// If checker is a mule or soldier:
		// Depending on which column checker is in, perform right and left checks differently
		if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE)
		{
			if (xLoc == 0) // At leftmost column
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc + 2][1] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc + 2][0] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
			if (xLoc == 0)
			{
				// Forward Diagnal checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal checks
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc + 2][1] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
					// Right Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][0] == REDSOLDIER || CMCheckersBoard[yLoc - 1][0] == REDMULE || CMCheckersBoard[yLoc - 1][0] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][1] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc + 2][0] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Backward Diagnal Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][0] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Backwards Diagnal Checks
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
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
			if (xLoc == 0)
			{
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc - 2][1] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc - 2][0] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
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
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				return false;
			}

		}
		else if (CMCheckersBoard[yLoc][xLoc] == REDKING)
		{
			if (xLoc == 0)
			{
				// Forward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard-2)
					{
						return true;
					}
				}
				// Left Diagnal checks
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc - 2][1] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][0] == WHITEMULE || CMCheckersBoard[yLoc + 1][0] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][1] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc - 2][0] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Backward Checks
				// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][0] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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
					if (CMCheckersBoard[yLoc - 2][xLoc + 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 2][xLoc - 2] == EMPTY && yLoc != 1)
					{
						return true;
					}
				}
				// Backward Checks
					// Right Diagnal checks
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc + 2] == EMPTY && yLoc != numRowsInBoard - 2)
					{
						return true;
					}
				}
				// Left Diagnal Checks
				if (CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 2][xLoc - 2] == EMPTY && yLoc != numRowsInBoard - 2)
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

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER)
	{
		// For Soldier, Mule, or King
		// depending on which column checker is in, perform right and left checks differently
		if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING)
		{
			if (xLoc == 0) // At leftmost column
			{
				// Forward diagonal checks. Right check || Left check
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == EMPTY)
				{
					return true;
				}
				// Backward diagonal checks if the checker is king. Right check || Left check
				if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 1) // At rightmost column
			{
				// Forward diagonal checks. Right check || Left check
				if (CMCheckersBoard[yLoc + 1][0] == EMPTY || CMCheckersBoard[yLoc + 1][xLoc - 1] == EMPTY)
				{
					return true;
				}
				// Backwards diagonal checks if the checker is king. Right check || Left check
				if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 1][0] == EMPTY || CMCheckersBoard[yLoc - 1][xLoc - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			// Not in the rightmost or leftmost column
			else
			{
				// Forward diagonal checks. Right check || Left check
				if (CMCheckersBoard[yLoc + 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc + 1][xLoc - 1] == EMPTY)
				{
					return true;
				}
				// Backwards diagonal checks if the checker is king. Right Check || Left Check
				if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc - 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc - 1][xLoc - 1] == EMPTY)
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
		// For Soldier, Mule, or King,
		// depending on which column checker is in, perform right and left checks differently
		if (CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING)
		{
			if (xLoc == 0) // At leftmost column
			{
				// Forward diagonal checks. Right check || Left check
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == EMPTY)
				{
					return true;
				}
				// Backward diagonal checks if the checker is king. Right check || Left check
				if (CMCheckersBoard[yLoc][xLoc] == REDKING)
				{
					if (CMCheckersBoard[yLoc + 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			else if (xLoc == numRowsInBoard - 1) // At rightmost column
			{
				// Forward diagonal checks. Right check || Left check
				if (CMCheckersBoard[yLoc - 1][0] == EMPTY || CMCheckersBoard[yLoc - 1][xLoc - 1] == EMPTY)
				{
					return true;
				}
				// Backwards diagonal checks if the checker is king. Right check || Left check
				if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 1][0] == EMPTY || CMCheckersBoard[yLoc + 1][xLoc - 1] == EMPTY)
					{
						return true;
					}
				}
				return false;
			}
			// Not in the rightmost or leftmost column
			else
			{
				// Forward diagonal checks. Right check || Left check
				if (CMCheckersBoard[yLoc - 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc - 1][xLoc - 1] == EMPTY)
				{
					return true;
				}
				// Backwards diagonal checks if the checker is king. Right Check || Left Check
				if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
				{
					if (CMCheckersBoard[yLoc + 1][xLoc + 1] == EMPTY || CMCheckersBoard[yLoc + 1][xLoc - 1] == EMPTY)
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

// Moves the checker
bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player,
	int fromSquareNum, int toSquareNum, bool &jumped)
{
	// Variables
	int yDistance = 0;
	int xDistance = 0;
	int xLocFrom = 0; // x index for the location of the checker before move.
	int yLocFrom = 0; // y index for the location of the checker before move
	int xLocTo = 0; // x index for the location of the checker after move.
	int yLocTo = 0; // y index for the location of the checker after move.
	int xLocBtw = 0; // x index for the location of the checker between two diagonal move.
	int yLocBtw = 0; // y index for the location of the checker between two diagonal move.
	int move = 0; // Holds the value whether the move is 1 square or 2 squares diagonal. 0 is invalid.
	int betweenNum = 0; // The square between two diagonal move is stored in here.
	int movedTemp = 0; // Holds the temporary value of the moved checker to compare if a mule is changed to a king.
	char endGame = '0'; // Character to end the game when a mule king is created.

	// Constants to determine the movement of the squares.
	const int ONESQUARE = 1;
	const int TWOSQUARES = 2;
	const int INVALID = 0;

	// Determine the locaation of the checker in the board.
	xLocFrom = fromSquareNum % numRowsInBoard;
	yLocFrom = fromSquareNum / numRowsInBoard;
	xLocTo = toSquareNum % numRowsInBoard;
	yLocTo = toSquareNum / numRowsInBoard;

	// Determine the distances of the movement
	yDistance = abs(yLocTo - yLocFrom);
	xDistance = abs(xLocTo - xLocFrom);

	// Determine if the movement is moving one or two squares diagonally. 
	// move = 0 : invalid movement
	// move = 1 : move 1 square diagonally
	// move = 2 : move 2 squares diagonally
	if ((xDistance == 1 && yDistance == 1) || (xDistance == (numRowsInBoard - 1) && yDistance == 1))
	{
		move = ONESQUARE;
	}
	else if ((xDistance == 2 && yDistance == 2) || (xDistance == (numRowsInBoard - 2) && yDistance == 2))
	{
		move = TWOSQUARES;
	}
	else
		move = INVALID;

	// User wants to move more than two squares diagonally.
	if (move == INVALID)
	{
		cerr << "ERROR: Illegal move." << endl;
		return false;
	}
	// If the distance is jumping a checker, including when the checker is moving from one side to the other,
	// check if there is an opponent checker in between.
	else if (move == TWOSQUARES)
	{
		// When the square user wants to move from is located at the rightmost side or second to left, and the square that is moved to is on the other side,
		// the following equation determines the square number of the square between the diagonal move: 
		// square that needs to be checked = (fromSquareNum + toSquareNum)/2 - numRowsInBoard/2
		if ((xLocFrom == 1 || xLocFrom == (numRowsInBoard - 1)) && xDistance == (numRowsInBoard - 2))
		{
			betweenNum = (fromSquareNum + toSquareNum) / 2 - numRowsInBoard / 2;
			xLocBtw = betweenNum % numRowsInBoard;
			yLocBtw = betweenNum / numRowsInBoard;
		}
		// When the square user wants to move from is located at the second to right or leftmost, and the square that is moved to is on the other side,
		// the following equation determines the square number of the square between the diagonal move:
		// square that needs to be checked = (fromSquareNum + toSquareNum)/2 + numRowsInBoard/2
		else if ((xLocFrom == 0 || xLocFrom == (numRowsInBoard - 2)) && xDistance == (numRowsInBoard - 2))
		{
			betweenNum = (fromSquareNum + toSquareNum) / 2 + numRowsInBoard / 2;
			xLocBtw = betweenNum % numRowsInBoard;
			yLocBtw = betweenNum / numRowsInBoard;
		}
		// When the square is not moving from one side to another,
		else if (xLocFrom >= 0 && xLocFrom <= (numRowsInBoard - 1))
		{
			betweenNum = (fromSquareNum + toSquareNum) / 2;
			xLocBtw = betweenNum % numRowsInBoard;
			yLocBtw = betweenNum / numRowsInBoard;
		}

		if (player == WHITEPLAYER)
		{
			// Check the checker located between the two diagonal move
			if (!(CMCheckersBoard[yLocBtw][xLocBtw] == REDSOLDIER || CMCheckersBoard[yLocBtw][xLocBtw] == REDMULE || CMCheckersBoard[yLocBtw][xLocBtw] == REDKING))
			{
				cerr << "ERROR: Illegal move." << endl;
				return false;
			}
		}

		if (player == REDPLAYER)
		{
			// Check the checker located between the two diagonal move
			if (!(CMCheckersBoard[yLocBtw][xLocBtw] == WHITESOLDIER || CMCheckersBoard[yLocBtw][xLocBtw] == WHITEMULE || CMCheckersBoard[yLocBtw][xLocBtw] == WHITEKING))
			{
				cerr << "ERROR: Illegal move." << endl;
				return false;
			}
		}
	}

	// Check if a white soldier or a white mule is moving in the wrong direction. yLocTo is smaller than yLocFrom.
	else if (CMCheckersBoard[yLocFrom][xLocFrom] == WHITESOLDIER || CMCheckersBoard[yLocFrom][xLocFrom] == WHITEMULE)
	{
		if (yLocTo < yLocFrom)
		{
			cerr << "ERROR: Illegal move" << endl;
			return false;
		}
	}
	// Check if a red soldier or a red mule is moving in the wrong direction. yLocTo is bigger than yLocFrom.
	else if (CMCheckersBoard[yLocFrom][xLocFrom] == REDSOLDIER || CMCheckersBoard[yLocFrom][xLocFrom] == REDMULE)
	{
		if (yLocTo > yLocFrom)
		{
			cerr << "ERROR: Illegal move" << endl;
			return false;
		}
	}

	// Making the move. Checking the jump first, then move.
	// For jump, move the checker to the location and destroy the value between the jump.
	// Then empties the value in original square.
	if (move == TWOSQUARES)
	{
		CMCheckersBoard[yLocTo][xLocTo] = CMCheckersBoard[yLocFrom][xLocFrom];
		CMCheckersBoard[yLocBtw][xLocBtw] = EMPTY;
		CMCheckersBoard[yLocFrom][xLocFrom] = EMPTY;
		jumped = true;

		// Check if the jumped checker has reached the end of the board and if so, check if it was a mule. 
		// If it was a mule, print error message and exit the program. Otherwise, return true.
		if (player == WHITEPLAYER)
		{
			if (yLocTo == numRowsInBoard - 1)
			{
				movedTemp = CMCheckersBoard[yLocTo][xLocTo];
				CMCheckersBoard[yLocTo][xLocTo] = WHITEKING;
				if (movedTemp == WHITEMULE)
				{
					cerr << "White has created a Mule King, Red wins the game\n";
					cout << "Enter any character to terminate the game then press the enter key\n";
					cin >> endGame;
					exit(2);
				}
			}
		}
		else if (player == REDPLAYER)
		{
			if (yLocTo == 0)
			{
				movedTemp = CMCheckersBoard[yLocTo][xLocTo];
				CMCheckersBoard[yLocTo][xLocTo] = REDKING;
				if (movedTemp == REDMULE)
				{
					cerr << "Red has created a Mule King, White wins the game\n";
					cout << "Enter any character to terminate the game then press the enter key\n";
					cin >> endGame;
					exit(3);
				}
			}
		}
		return true;
	}
	// For move, change the checker location and empty the value in the original square.
	// Then check if the mule checker has reached the end of the board. If so, print error message and exit the program.
	// Otherwise, return true.
	else if (move == ONESQUARE)
	{
		CMCheckersBoard[yLocTo][xLocTo] = CMCheckersBoard[yLocFrom][xLocFrom];
		CMCheckersBoard[yLocFrom][xLocFrom] = EMPTY;
		jumped = false;

		if (player == WHITEPLAYER)
		{
			if (yLocTo == numRowsInBoard - 1)
			{
				movedTemp = CMCheckersBoard[yLocTo][xLocTo];
				CMCheckersBoard[yLocTo][xLocTo] = WHITEKING;
				if (movedTemp == WHITEMULE)
				{
					cerr << "White has created a Mule King, Red wins the game\n";
					cout << "Enter any character to terminate the game then press the enter key\n";
					cin >> endGame;
					exit(4);
				}
			}
		}
		else if (player == REDPLAYER)
		{
			if (yLocTo == 0)
			{
				movedTemp = CMCheckersBoard[yLocTo][xLocTo];
				CMCheckersBoard[yLocTo][xLocTo] = REDKING;
				if (movedTemp == REDMULE)
				{
					cerr << "Red has created a Mule King, White wins the game\n";
					cout << "Enter any character to terminate the game then press the enter key\n";
					cin >> endGame;
					exit(5);
				}
			}
		}
		return true;
	}
}

bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	// Counter variables for mules and soldiers/kings for each color.
	int redMuleCount = 0;
	int whiteMuleCount = 0;
	int redSKCount = 0;
	int whiteSKCount = 0;

	// Red player no mules
	for (int i = 0; i < numRowsInBoard; i++)
	{
		for (int j = 0; j < numRowsInBoard; j++)
		{
			if (CMCheckersBoard[i][j] == REDMULE)
				redMuleCount++;
		}
	}
	if (redMuleCount == 0)
	{
		cout << "The Red Player has won the game by losing all of the Red Mules\n";
		return true;
	}

	// White player no mules
	for (int i = 0; i < numRowsInBoard; i++)
	{
		for (int j = 0; j < numRowsInBoard; j++)
		{
			if (CMCheckersBoard[i][j] == WHITEMULE)
				whiteMuleCount++;
		}
	}
	if (whiteMuleCount == 0)
	{
		cout << "The White Player has won the game by losing all of the White Mules\n";
		return true;
	}

	// Captured all red soldiers and kings
	for (int i = 0; i < numRowsInBoard; i++)
	{
		for (int j = 0; j < numRowsInBoard; j++)
		{
			if (CMCheckersBoard[i][j] == REDSOLDIER || CMCheckersBoard[i][j] == REDKING)
				redSKCount++;
		}
	}
	if (redSKCount == 0)
	{
		cout << "The White Player has won the game by capturing all of the red players soldiers and kings\n";
		return true;
	}

	// Captured all white soldiers and kings
	for (int i = 0; i < numRowsInBoard; i++)
	{
		for (int j = 0; j < numRowsInBoard; j++)
		{
			if (CMCheckersBoard[i][j] == WHITESOLDIER || CMCheckersBoard[i][j] == WHITEKING)
				whiteSKCount++;
		}
	}
	if (whiteSKCount == 0)
	{
		cout << "The Red Player has won the game by capturing all of the white players soldiers and kings\n";
		return true;
	}
	return false;
}