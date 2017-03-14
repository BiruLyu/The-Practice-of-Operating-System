/*************************************************
 *  正常运行:       hanoi -n <步数>              *
 *  示例:           hanoi -a <步数>              *
 *  帮助:           hanoi -h                     *
 *************************************************/
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define EXIT_SUCCESS        0
#define EXIT_FAILURE        1
#define NPEGS               3
#define MINTILES            3
#define MAXTILES            6
#define DEFAULTTILES        7
#define TOPLINE             6
#define BASELINE            16
#define STATUSLINE      (LINES-3)
#define LEFTPEG         19
#define MIDPEG          39
#define RIGHTPEG        59
#define LENTOIND(x)     (((x)-1)/2)
#define OTHER(a,b)      (3-((a)+(b)))

//柱的数据结构
struct Peg 
{
    size_t Length[MAXTILES];  //自底而上，存放盘的大小，盘的大小应该是各不同的   
    int Count; //记录柱上的盘的总数
};
static struct Peg Pegs[NPEGS];  //为三条柱定义三个结构体
static int PegPos[] ={LEFTPEG, MIDPEG, RIGHTPEG};
static int TileColour[] ={COLOR_RED,COLOR_GREEN,COLOR_YELLOW,COLOR_BLUE,COLOR_MAGENTA,COLOR_CYAN};
static int NMoves = 0;
static void InitTiles(int NTiles);
static void DisplayTiles(void);
static void MakeMove(int From, int To);
static void AutoMove(int From, int To, int Num);
static void Usage(void);
static int Solved(int NumTiles);
static int GetMove(int *From, int *To);
static int InvalidMove(int From, int To);

int main(int argc, char **argv)
{
    int NTiles, FromCol, ToCol,opt;
    unsigned char AutoFlag = 0;
    if(argc<=2 || argc>3)
    { 
        Usage();
        exit(0);
    }
        opt=*argv[1];
        switch (opt)
        {
            case 'n':
                NTiles = atoi(argv[2]);
                if (NTiles > MAXTILES || NTiles < MINTILES)
                {   
                    fprintf(stderr, "步数从 %d 自 %d之间\n", MINTILES, MAXTILES);
                    return EXIT_FAILURE;
                }
                AutoFlag = FALSE;
                break;
            case 'a':
                NTiles = atoi(argv[2]);
                if (NTiles > MAXTILES || NTiles < MINTILES)
                {       
                    fprintf(stderr, "步数从 %d 自 %d之间\n", MINTILES, MAXTILES);
                    return EXIT_FAILURE;
                }
                AutoFlag = TRUE;
                break;
            case 'h':
            case '?':
            case ':':
                Usage();
                exit(0);
        }
        initscr();
        clear();
        if (has_colors())  //判断终端是否可以使用颜色
        {
            int i;
            int bg = COLOR_BLACK;   //黑色为0
            start_color();  //开启颜色设置
            if (use_default_colors() == OK)  //把终端设置为前景：白色  背景：黑色
                bg = -1;
            for (i = 0; i < 9; i++)
                init_pair(i + 1, bg, TileColour[i]);   //初始化颜色对
        }
        cbreak();
        /*LINES和COLS是Curses内部变量,用于存储当前终端的行数和列数*/
        if (LINES < 24 || COLS < 80)          /*标准的终端为24行80列*/
        {
            endwin();
            fprintf(stderr, "当前小于 24x80 \n");
            return EXIT_FAILURE;
        }
        if (AutoFlag)
        {
            curs_set(0);
            leaveok(stdscr, TRUE);
        }
        InitTiles(NTiles);
        DisplayTiles();
        if (AutoFlag) 
        {
            do 
            {
                noecho();
                AutoMove(0, 2, NTiles);
            } 
            while (!Solved(NTiles));
            sleep(2);
        } 
        else 
        {
            echo();
            for (;;) 
            {
                if (GetMove(&FromCol, &ToCol))
                    break;
                if (InvalidMove(FromCol, ToCol)) 
                {
                    mvaddstr(STATUSLINE, 0, "The moving is invalid !!");
                    refresh();
                    beep();
                    sleep(2);
                    continue;
                }
                MakeMove(FromCol, ToCol);
                if (Solved(NTiles)) 
                {
                    mvprintw(STATUSLINE, 0, "Congratulation!! You win with %d steps. ", NMoves);
                    refresh();
                    sleep(5);
                    break;
                }
            }
        }
        endwin();
        return EXIT_SUCCESS; 
}

static int InvalidMove(int From, int To)
{   
    //输入应该是有效的，包括From和To都是柱的编号，在１至３之间；From不等于To
    if (From >= NPEGS)
        return TRUE;
    if (From < 0)
        return TRUE;
    if (To >= NPEGS)
        return TRUE;
    if (To < 0)
        return TRUE;
    if (From == To)
        return TRUE;
    if (!Pegs[From].Count)
        return TRUE;
    if (Pegs[To].Count && Pegs[From].Length[Pegs[From].Count - 1] > Pegs[To].Length[Pegs[To].Count - 1])
    /*任何盘都不得放在比它小的盘上，即其长度小于终点柱最顶层的盘的长度。*/
        return TRUE;
    return FALSE;
}

//初始化每个柱上的盘数
//NTiles是柱上盘总数
//盘的长度为1,3,5,7等
static void InitTiles(int NTiles)  
{
    int Size, SlotNo;
    for (Size = NTiles * 2 + 1, SlotNo = 0; Size >= 3; Size -= 2)
        Pegs[0].Length[SlotNo++] = Size;  //自底向上
    Pegs[0].Count = NTiles;  //0柱上的盘数
    Pegs[1].Count = 0;
    Pegs[2].Count = 0;

}

//显示柱和盘
static void DisplayTiles(void)
{
    int Line, peg, SlotNo;
    char TileBuf[BUFSIZ];
    erase();
    init_pair(20,COLOR_MAGENTA,COLOR_BLACK);   //设置20号索引为[洋红，黑色]
    attrset(COLOR_PAIR(20)|A_BOLD);   //高亮
    mvaddstr(1, 27,"Tower of Hanoi Game");
    attrset(A_NORMAL);   //恢复正常显示
    mvprintw(18, 35, "Current steps : ");
    init_pair(21,COLOR_RED,COLOR_BLACK);   //设置21号索引为[红色，黑色]
    attrset(COLOR_PAIR(21)|A_BOLD);   //高亮
    mvprintw(18,45,"%d",NMoves);    
    attrset(A_NORMAL);  //恢复正常显示
    attrset(A_REVERSE);  //反白  背景色为白色，前景色为黑色
    mvaddstr(BASELINE, 8,"                                                               ");
    
    //三条柱的实现
    //LEFTPEG，MIDPEG，RIGHTPEG分别是三条柱的位置
    //垂直长度为从TOPLINE 到BASELINE 
    for (Line = TOPLINE; Line < BASELINE; Line++)
    {
        mvaddch(Line, LEFTPEG, ' ');
        mvaddch(Line, MIDPEG, ' ');
        mvaddch(Line, RIGHTPEG, ' ');
    }
    mvaddch(BASELINE, LEFTPEG, '1');
    mvaddch(BASELINE, MIDPEG, '2');
    mvaddch(BASELINE, RIGHTPEG, '3');
    attrset(A_NORMAL);  //恢复正常显示
    
    for (peg = 0; peg < NPEGS; peg++) 
    {
        for (SlotNo = 0; SlotNo < Pegs[peg].Count; SlotNo++) 
        /*分别画出三条柱上的盘，NPEGS为3*/
        {
            memset(TileBuf, ' ', Pegs[peg].Length[SlotNo]); //将TileBuf前几位设置为空格
            TileBuf[Pegs[peg].Length[SlotNo]] = '\0';  //设置了TileBuff的长度为盘的长度
            /*设置盘的长度，即空格串长度*/
            if (has_colors())
                attrset(COLOR_PAIR(LENTOIND(Pegs[peg].Length[SlotNo]))); 
            else
                attrset(A_REVERSE);
            /*设置彩色*/
            mvaddstr(BASELINE - (SlotNo + 1),(int) (PegPos[peg] - Pegs[peg].Length[SlotNo] / 2),TileBuf);
            /*画出盘*/
        }
    }
    attrset(A_NORMAL);
    refresh();
}

static int GetMove(int *From, int *To)
{
    attrset(A_REVERSE);
    mvaddstr(LINES-1, 20,"<Q>/<q> QUIT       <1>-<3> MOVE");
    attrset(A_NORMAL);
    mvaddstr(STATUSLINE, 0, "Next Step: From ");
    clrtoeol();  //删除当前行中从光标到行末的的所有字符
    refresh();
    *From = getch();
    if ((*From == 'q')||(*From == 'Q') )
        return TRUE;
    *From -= ('0' + 1);
    addstr(" to ");
    clrtoeol();
    refresh();
    *To = getch();
    if ((*To == 'q') || (*To == 'Q'))
        return TRUE;
    *To -= ('0' + 1);
    refresh();
    napms(500);
    move(STATUSLINE, 0);
    clrtoeol();
    refresh();
    return FALSE;
}

static void MakeMove(int From, int To)
{
    /*Pegs[From].Count减一，因Length数组是有底而上设置的，
    当Count减一时柱最顶层的盘就自动丢弃；
    然后，把目标盘加入到终点柱上，*/
    Pegs[From].Count--;
    Pegs[To].Length[Pegs[To].Count] = Pegs[From].Length[Pegs[From].Count];
    Pegs[To].Count++;
    NMoves++;
    DisplayTiles();
}

static void AutoMove(int From, int To, int Num)
{
    if (Num == 1) 
    {
        MakeMove(From, To);
        napms(500);
        return;
    }
    AutoMove(From, OTHER(From, To), Num - 1);
    MakeMove(From, To);
    napms(500);
    AutoMove(OTHER(From, To), To, Num - 1);
}

static int Solved(int NumTiles)
{
    int i;
    for (i = 1; i < NPEGS; i++)
    if (Pegs[i].Count == NumTiles)
        return TRUE;
    return FALSE;
}

static void Usage()
{
    fprintf(stderr, "\nhanoi\t[n]步数 -- 玩游戏\n\t[a]步数 -- 示范\n\t[h] \t-- 帮助 \n");
    printf("\e[0;33m其中步数在%d-%d之间\e[0m\n", MINTILES, MAXTILES);
}
/*----------------the end-----------------*/
