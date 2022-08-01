#include <iostream>
#include "ChessGame.h"


int main(void) 
{
	Man man;
	Chess chess(19,58,56,47);
	AI ai;
	ChessGame game(&man, &ai, &chess);
	
	game.play();

	return 0;
}