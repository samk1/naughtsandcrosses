#include "state.h"

const Board State::blankBoard = { {
	{ BoardPosition::Nothing, BoardPosition::Nothing, BoardPosition::Nothing },
	{ BoardPosition::Nothing, BoardPosition::Nothing, BoardPosition::Nothing },
	{ BoardPosition::Nothing, BoardPosition::Nothing, BoardPosition::Nothing }
} };

State::State()
{
}


State::~State()
{
}
