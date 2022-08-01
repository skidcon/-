#include "Chess.h"
#include <mmsystem.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")


void putimagePNG (int x,int y, IMAGE * picture)//x为载入图片的x坐标，y为y坐标
{
	//变量初始化
	DWORD * dst = GetImageBuffer();                 //用于获取绘图设备的显存指针，easyx 自带
	DWORD* draw = GetImageBuffer();                 
	DWORD* src = GetImageBuffer(picture);       //获取picture的显存指针
	int picture_width = picture->getwidth();        //获取picture的宽度 easyx自带
	int picture_height = picture->getheight();      //获取picture的高度 easyx自带  
	int graphWidth = getwidth();                    //获取绘图区的宽度，easyx自带
	int graphHeight = getheight();				    //获取绘图区的宽度，easyx自带
	int dstX = 0;                                  //在现存里像素的角标
	
	//实现透明贴图 公式：Cp = a p*FP+(1-a p)*BP, 贝叶斯定理来进行点颜色的概率计算
	for(int iy = 0; iy < picture_height;iy++ )
	{
		for(int ix = 0 ;ix <picture_width ; ix++)
		{
			int srcX = ix + iy * picture_width;
			int sa = ((src[srcX] & 0xff000000) >> 24);  //0xAArrggbb  AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16);    //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);       //G
			int sb = src[srcX] & 0xff;                  //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth;   //存在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式：Cp = a p *FP+(1-a p)*BP ;
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)           //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);                   //αp=sa/255 , FP=sb , BP=db
			}

		}
	}
}

Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
	this->gradeSize = gradeSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;

	for (int i = 0; i < gradeSize; i++)
	{
		vector<int>row;
		for (int j = 0; j < gradeSize; j++)
		{
			row.push_back(0);
		}
		chessMap.push_back(row); 
	}

}

void Chess::init()
{
	//创建游戏窗口
	initgraph(960, 960,EW_SHOWCONSOLE);
	//显示棋盘
	loadimage(0, "res/棋盘.jpg");
	//播放音乐
	mciSendString("play res/start.wav", 0, 0, 0);
	//加载棋子
	loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

	//棋盘清零

	for (int i = 0; i < gradeSize; i++)
	{
		for (int j = 0; j < gradeSize; j++)
		{
			chessMap[i][j] = 0;

		}
	}
	playerFlag = true;
}   


bool Chess::clickBoard(int x, int y, ChessPos*pos)
{
	int col= (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;
	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int offset = chessSize * 0.4;

	int len;
	bool ret = false;

	do
	{
		//左上角的判断
		len = sqrt(((x - leftTopPosX) * (x - leftTopPosX)) + ((y - leftTopPosY) * (y - leftTopPosY)));
		if (len < offset)
		{
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}

		//右下角的判断
		int x2 = leftTopPosX + chessSize;
		int y2 = leftTopPosY ;

		len = sqrt(((x - x2) * (x - x2)) + ((y - y2) * (y - y2)));
		if (len < offset)
		{
			pos->row = row;
			pos->col = col+1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}

		//右上角的判断
		x2 = leftTopPosX;
		y2 = leftTopPosY + chessSize;

		len = sqrt(((x - x2) * (x - x2)) + ((y - y2) * (y - y2)));
		if (len < offset)
		{
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}

		//右下角的判断
		x2 = leftTopPosX + chessSize;
		y2 = leftTopPosY + chessSize;

		len = sqrt(((x - x2) * (x - x2)) + ((y - y2) * (y - y2)));
		if (len < offset)
		{
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}
	} while (0);

	return ret;

}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize;


	if (kind == CHESS_WHITE) 
	{	
		putimagePNG(x, y, &chessWhiteImg);

	}
	else 
	{
		putimagePNG(x, y, &chessBlackImg);
	}

}

int Chess::getGradeSize()
{
	return 0;
}

int Chess::getChessDate(ChessPos* pos)
{
	return 0;
}

int Chess::getChessDate(int row, int col)
{
	return 0;
}

bool Chess::checkOver()
{
	return false;
}
