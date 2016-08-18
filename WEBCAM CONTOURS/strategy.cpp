#include "strategy.h"



Strategy::Strategy(PieceType player)
{
	pieceType = player;
}


Strategy::~Strategy()
{
}

Move* DumbStrategy::makeMove(Board board)
{
	Move *move = NULL;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board.positions[i][j] == BoardPosition::Nothing) {
				Move *move = new Move();
				move->piece = pieceType;
				move->x = i;
				move->y = j;

				// use goto to break out of both loops
				goto FOUND_MOVE;
			}
		}
	}

FOUND_MOVE:
	// we might get here if we have not found a move.
	// in that case we'll just return NULL (no move)
	return move;
}