#include "Chess.h"
#include <mmsystem.h>
#include <math.h>
#pragma comment(lib,"winmm.lib")


void putimagePNG (int x,int y, IMAGE * picture)//xΪ����ͼƬ��x���꣬yΪy����
{
	//������ʼ��
	DWORD * dst = GetImageBuffer();                 //���ڻ�ȡ��ͼ�豸���Դ�ָ�룬easyx �Դ�
	DWORD* draw = GetImageBuffer();                 
	DWORD* src = GetImageBuffer(picture);       //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth();        //��ȡpicture�Ŀ�� easyx�Դ�
	int picture_height = picture->getheight();      //��ȡpicture�ĸ߶� easyx�Դ�  
	int graphWidth = getwidth();                    //��ȡ��ͼ���Ŀ�ȣ�easyx�Դ�
	int graphHeight = getheight();				    //��ȡ��ͼ���Ŀ�ȣ�easyx�Դ�
	int dstX = 0;                                  //���ִ������صĽǱ�
	
	//ʵ��͸����ͼ ��ʽ��Cp = a p*FP+(1-a p)*BP, ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for(int iy = 0; iy < picture_height;iy++ )
	{
		for(int ix = 0 ;ix <picture_width ; ix++)
		{
			int srcX = ix + iy * picture_width;
			int sa = ((src[srcX] & 0xff000000) >> 24);  //0xAArrggbb  AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16);    //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);       //G
			int sb = src[srcX] & 0xff;                  //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth;   //�����Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ��Cp = a p *FP+(1-a p)*BP ;
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)           //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);                   //��p=sa/255 , FP=sb , BP=db
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
	//������Ϸ����
	initgraph(960, 960,EW_SHOWCONSOLE);
	//��ʾ����
	loadimage(0, "res/����.jpg");
	//��������
	mciSendString("play res/start.wav", 0, 0, 0);
	//��������
	loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

	//��������

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
		//���Ͻǵ��ж�
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

		//���½ǵ��ж�
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

		//���Ͻǵ��ж�
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

		//���½ǵ��ж�
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
