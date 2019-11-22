#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

void instructions();
char askYesNo(string question);
int askNumber(string question, int high, int low = 1);
char humanPiece();
char opponent(char piece);
void displayBoard(const vector<char> &board);
char winner(const vector<char> &board);
bool isLegal(const vector<char> &board, int move);
int humanMove(const vector<char> &board, char human);
int computerMove(vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);
bool replayOrQuit();

int main()
{
    bool quit = false;

    while (!quit)
    {
        int move;
        const int NUM_SQUARES = 9;
        vector<char> board(NUM_SQUARES, EMPTY);

        instructions();
        char human = humanPiece();
        char computer = opponent(human);
        char turn = X;
        displayBoard(board);

        while (winner(board) == NO_ONE)
        {
            if (turn == human)
            {
                move = humanMove(board, human);
                board[move] = human;
            }
            else
            {
                move = computerMove(board, computer);
                board[move] = computer;
            }
            displayBoard(board);
            turn = opponent(turn);
        }
        announceWinner(winner(board), computer, human);
        quit = replayOrQuit();
    }
    
    return 0;
}

void instructions()
{
    cout << "Make your move by entering a number from 1-9.\n";
    cout << " 1 | 2 | 3\n";
    cout << " ---------\n";
    cout << " 4 | 5 | 6\n";
    cout << " ---------\n";
    cout << " 7 | 8 | 9\n";
}

char askYesNo(string question)
{
    char response;
    do
    {
        cout << question << " (y/n): ";
        cin >> response;
    } while (response != 'y' && response != 'n');

    return response;
}

int askNumber(string question, int high, int low)
{
    int number;
    do
    {
        cout << question << " (" << low << " - " << high << "): ";
        cin >> number;
    } while (number > high || number < low);

    return number;
}

char humanPiece()
{
    char go_first = askYesNo("Do you require the first move?");
    if (go_first == 'y')
    {
        cout << "Take the first move, yours will be X.\n";

        return X;
    }
    else if (go_first == 'n')
    {
        cout << "Computer will take the first move (X), yours will be O.\n";

        return O;
    }

    return 'E';
}

char opponent(char piece)
{
    if (piece == X)
    {
        return O;
    }
    else if (piece == O)
    {
        return X;
    }

    return 'E';
}

void displayBoard(const vector<char> &board)
{
    cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
    cout << "\n\t---------";
    cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
    cout << "\n\t---------";
    cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
    cout << "\n\n";
}

char winner(const vector<char> &board)
{
    const int WINNING_ROWS[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}};
    const int TOTAL_ROWS = 8;
    for (int row = 0; row < TOTAL_ROWS; ++row)
    {
        if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
            (board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
            (board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
        {
            return board[WINNING_ROWS[row][0]];
        }
    }
    if (count(board.begin(), board.end(), EMPTY) == 0)
    {
        return TIE;
    }

    return NO_ONE;
}

inline bool isLegal(int move, const vector<char> &board)
{
    return (board[move] == EMPTY);
}

int humanMove(const vector<char> &board, char human)
{
    int move = askNumber("Where will you move?", board.size()) - 1;
    while (!isLegal(move, board))
    {
        cout << "This square is already occupied.\n";
        move = askNumber("Where will you move?", board.size()) - 1;
    }

    return move;
}

int computerMove(vector<char> board, char computer)
{
    unsigned int move = 0;
    bool found = false;
    while (!found && move < board.size())
    {
        if (isLegal(move, board))
        {
            board[move] = computer;
            found = winner(board) == computer;
            board[move] = EMPTY;
        }
        if (!found)
        {
            ++move;
        }
    }
    if (!found)
    {
        move = 0;
        char human = opponent(computer);
        while (!found && move < board.size())
        {
            if (isLegal(move, board))
            {
                board[move] = human;
                found = winner(board) == human;
                board[move] = EMPTY;
            }
            if (!found)
            {
                ++move;
            }
        }
    }
    if (!found)
    {
        move = 0;
        if (board[4] == computer && board[0] == computer)
        {
            if (isLegal(1, board))
            {
                move = 1;
                found = true;
            }
            else if (isLegal(3, board))
            {
                move = 3;
                found = true;
            }
        }
        if (board[4] == computer && board[2] == computer)
        {
            if (isLegal(1, board))
            {
                move = 1;
                found = true;
            }
            else if (isLegal(5, board))
            {
                move = 5;
                found = true;
            }
        }
        if (board[4] == computer && board[6] == computer)
        {
            if (isLegal(3, board))
            {
                move = 3;
                found = true;
            }
            else if (isLegal(7, board))
            {
                move = 7;
                found = true;
            }
        }
        if (board[4] == computer && board[8] == computer)
        {
            if (isLegal(5, board))
            {
                move = 5;
                found = true;
            }
            else if (isLegal(7, board))
            {
                move = 7;
                found = true;
            }
        }
    }
    if (!found)
    {
        move = 0;
        unsigned int i = 0;
        const int BEST_MOVES[] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
        while (!found && i < board.size())
        {
            move = BEST_MOVES[i];
            if (isLegal(move, board))
            {
                found = true;
            }
            ++i;
        }
    }
    cout << "Computer will take square number " << move + 1 << "\n";

    return move;
}

void announceWinner(char winner, char computer, char human)
{
    if (winner == computer)
    {
        cout << "Computer win\n";
    }
    else if (winner == human)
    {
        cout << "Human win\n";
    }
    else if (winner == TIE)
    {
        cout << "Tie\n";
    }
    else
    {
        cout << "There is no winner yet.\n";
    }
}

bool replayOrQuit()
{
    char flag = askYesNo("Do you want to play again?");
    if (flag == 'y')
    {
        return false;
    }
    else
    {
        return true;
    }
}