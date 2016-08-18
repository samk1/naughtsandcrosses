#pragma once
#include "state.h"

// Move: place what piece where?
typedef struct Move {
	int x;
	int y;
	PieceType piece;
};

// abstract class representing how to play the game
class Strategy
{
public:
	// you must know which player you are to have a strategy.
	Strategy(PieceType player);
	~Strategy();

	virtual Move* makeMove(Board board) = 0;

protected:
	PieceType pieceType;
};

// dumb strategy where we just place a move on the first empty position
class DumbStrategy: public Strategy
{
	DumbStrategy(PieceType player);
	~DumbStrategy();

	Move* makeMove(Board board);
};