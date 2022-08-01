#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;

}

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;
	
	
	while (1)
	{
		//获取鼠标点击的消息
		msg = GetMouseMsg();

		//通过chess对象，来调用 判断落子是否有效， 以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos))
			break;

	}

	printf("%d,%d\n", pos.row, pos .col);

	chess->chessDown(&pos, CHESS_BLACK);

}
