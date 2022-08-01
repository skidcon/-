#pragma once
#include <graphics.h>  //ͼ�ο��ļ�
#include <vector>
using namespace std;

typedef enum
{
	CHESS_WHITE = -1, //����
	CHESS_BLACK = 1, //����

} chess_kind_t;

struct ChessPos 
{
	int row;
	int col;
};



class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	
	//���̳�ʼ��
	void init();
	bool clickBoard(int x, int y, ChessPos*pos);
	void chessDown(ChessPos *pos, chess_kind_t kind);
	int getGradeSize();//��ȡ���̵Ĵ�С��13��15��19��
	int getChessDate(ChessPos* pos);
	int getChessDate(int row, int col);
	bool checkOver();//�������Ƿ����
private:
	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	int gradeSize;
	int margin_x; 
	int margin_y;
	float chessSize;
	//�洢��ǰ���ߵ����ӷֲ�0 �հ� 1���� -1 ����  
	vector <vector<int>> chessMap;

	//�������ڸ�˭����
	bool playerFlag; //true �ú�   false �ð�





};


