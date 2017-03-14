#include<stdio.h>
#include<curses.h>
#include<stdlib.h>

#define BLANK ' '
#define BALL 'o'
#define Fence '*'
#define Baffle '='
#define WALL "X"

#define TopRow 5
#define BotRow 20
#define LEFT_EDGE 5
#define RIGHT_EDGE 70

#define X_INIT 10
#define Y_INIT 10

#define startX_ 21
#define startY_ 10
#define BaffleLen 7
/* baffle start position */

#define String1 "Name:Drow&Destroy"
#define String2 "Developer:Ruby Lyu"
#define String3 "Stu ID:10122130102"
#define String4 "ScoreC:"
#define String5 "ScoreB:"
#define String6 "Game State:"
#define Tip1 "GameOn"
#define Tip2 "GameOver"
#define Tip3 "YourScore:"

#define TICKS_PER_SEC 50

#define X_Time 5
#define Y_Time 8

#define LEFT KEY_LEFT
#define RIGHT KEY_RIGHT

#define Min 1
#define Max 12



#define DELTA 5



struct BallInfo{
int x_pos,y_pos;
int x_Time1,y_Time1;
int x_Time2,y_Time2;
int x_dir,y_dir;
char ball_symbol;
};

struct BaffleInfo{
int x,y,len;
char baffle_symbol;
};


struct WallInfo{
int x,y;
bool flag;
};