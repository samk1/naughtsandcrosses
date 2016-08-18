#pragma once
#include <vector>
#include <opencv2\core.hpp>

using namespace cv;
using namespace std;

// Each position can be either a nought a cross or have nothing
typedef enum { Nought, Cross, Nothing } BoardPosition;

// noughts and crosses board is 3x3
typedef struct Board { BoardPosition positions[3][3]; } Board;

// it can be either human or robots turn
typedef enum { Humans, Robots } Turn;

// types of pieces
typedef enum { Nought, Cross } PieceType;

// a piece
typedef struct Piece {
	Point2f location; // where in 2-space it is
	Scalar speed; // how fast it's moving (pixels / second?)
	Scalar direction; // what direction its moving in (radians)
	PieceType type; // what type of piece it is - nought or cross
};

class State
{
public:
	State(Board initialBoard, Turn initialTurn);
	~State();

	// update state using the position of perceived noughts and crosses
	void update(const vector<Piece> &pieces);

	// robot can finish it's turn
	void finishTurn();

	// access current state
	Board getBoard();
	Turn getTurn();

	// blank board - not sure where to put this really
	static const Board blankBoard;

private:
	// current state of board
	Board board;

	// whose turn it is
	Turn turn;

	// how long the everything has been still for in milliseconds
	int stillTimeMs;

	// how long since last update in milliseconds
	int sinceUpdateMs;
};

