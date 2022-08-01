#pragma once
#include <graphics.h>  //图形库文件
#include <vector>
using namespace std;

typedef enum
{
	CHESS_WHITE = -1, //白棋
	CHESS_BLACK = 1, //黑棋

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
	
	//棋盘初始化
	void init();
	bool clickBoard(int x, int y, ChessPos*pos);
	void chessDown(ChessPos *pos, chess_kind_t kind);
	int getGradeSize();//获取棋盘的大小（13，15，19）
	int getChessDate(ChessPos* pos);
	int getChessDate(int row, int col);
	bool checkOver();//检查棋局是否结束
private:
	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	int gradeSize;
	int margin_x; 
	int margin_y;
	float chessSize;
	//存储当前器具的棋子分布0 空白 1黑子 -1 白子  
	vector <vector<int>> chessMap;

	//便是现在该谁落子
	bool playerFlag; //true 该黑   false 该白





};


