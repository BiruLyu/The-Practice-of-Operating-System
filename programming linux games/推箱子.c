/*---------------------------------------------------   
 推箱子游戏:此游戏的算法是十分简明的,将游戏变成一   
 串串的程序代码也不具有难度的.   
 基本思路是利用一个二维数组:数组值用于表示不同的   
 实物与场景.为了直观地处理数据,可以将二维数组设置   
 为一个字符数组.'0'表示什么都没有,'B'表示箱子,'D'   
 表示目的地,'W'表示墙壁,'F'表示箱子被移动到了目的   
 地.'T'表示人走到了目的地上面了.   
 利用这些数值的变化就可以控制整个游戏的运行了.   
  
  
----------------------------------------------------*/    
/*-----------------------变量函数声明部分------------------*/    
#include<stdio.h>     
#include<curses.h>   
#include<string.h> /*库函数与宏定义的常量声明*/   
   
#define GRID 219/*表示方格与推箱者的特殊字符*/    
#define ME 72    
#define MAXL 12 /*地图的最大长度与宽度*/    
#define MAXC 25    
   
char MAP[MAXL][MAXC],ORI[MAXL][MAXC]; /*定义画地图的二维字符数组*/    
int LEVEL=1,SX=5,SY=8,MEX,MEY,TOTAL; /*定义画地图的起始坐标与相关变量*/    
 /*(MEX,MEY)为推箱者相对于窗口(80*25)坐标位置,   
 (SX,SY)为地图的起始坐标(左上角),  
TOTAL为地图 中的总箱子数*/   
    
WINDOW *win;   
   
void initial(); //初始化屏幕   
void InitialMap(int n);    
void DrawMap();    
void StartGame();    
void ClearMap();    
void PutIco(char ch);    
void PutData(); /*观察二维字符数组数值变化的检测函数*/    
void ClearData();    
   
/*-------------------------主函数-------------------------*/    
int main()     
{    
     
  initial(); /* 呼叫 initial(), 启动 curses puts模式 */    
  win=newwin(0,30,0,10);/* 建立一个新视窗, 其中LINES,COLS*/   
  refresh();   
  touchwin(win);           
  wrefresh(win);   
   
//颜色初始化      
  if(!has_colors()||start_color()==ERR){      
  endwin();      
  printf("Terminal   does   not   support   color.\n");      
  exit(1);      
  }      
//添加颜色   
  init_pair(1,   COLOR_GREEN,   COLOR_BLACK);      
  init_pair(2,   COLOR_RED,   COLOR_BLACK);      
  init_pair(3,   COLOR_CYAN,   COLOR_BLACK);      
  init_pair(4,   COLOR_WHITE,   COLOR_BLACK);      
  init_pair(5,   COLOR_MAGENTA,   COLOR_BLACK);      
  init_pair(6,   COLOR_BLUE,   COLOR_BLACK);      
  init_pair(7,   COLOR_YELLOW,   COLOR_BLACK);      
     
 wprintw(win,"\n* ESC to exit GAME *\n");    
 wprintw(win,"* TAB to restart *\n");    
 wprintw(win,"* ENTER to set values *\n");    
 wprintw(win,"*----------------------*\n");    
 InitialMap(1);    
 StartGame();    
 getch();  
  endwin();    
 return 0;    
}    
     
void InitialMap(int n) /*初始化二维数组,即地图的数据设置*/    
{   
 int I,J;     
 for(I=0;I<MAXL;I++)    
 for(J=0;J<MAXC;J++)    
  MAP[I][J]=NULL;    
  switch(n)    
  {   
  case 1: /*第一关地图*/    
    for(I=0;I<MAXL;I++) {   
        if(I==0) strcpy(MAP[I],"0WWWWW00");    
   else if(I==1) strcpy(MAP[I],"0WP0WWW0");    
   else if(I==2) strcpy(MAP[I],"0W0B00W0");    
    else if(I==3) strcpy(MAP[I],"WWW0W0WW");    
    else if(I==4) strcpy(MAP[I],"WDW0W00W");    
    else if(I==5) strcpy(MAP[I],"WDB00W0W");    
    else if(I==6) strcpy(MAP[I],"WD000B0W");    
    else if(I==7) strcpy(MAP[I],"WWWWWWWW");    
  } break;    
 case 2: /*第二关地图*/    
 for(I=0;I<MAXL;I++)    
 {    if(I==0) strcpy(MAP[I],"0WWWW0");    
 else if(I==1) strcpy(MAP[I],"WW00W0");    
 else if(I==2) strcpy(MAP[I],"WPB0W0");    
 else if(I==3) strcpy(MAP[I],"WWB0WW");    
 else if(I==4) strcpy(MAP[I],"WW0B0W");    
 else if(I==5) strcpy(MAP[I],"WDB00W");    
 else if(I==6) strcpy(MAP[I],"WDD0DW");    
 else if(I==7) strcpy(MAP[I],"WWWWWW");    
 } break;    
 case 3: /*第三关地图*/    
 for(I=0;I<MAXL;I++)    
 {    if(I==0) strcpy(MAP[I],"0WWWWWWW00");    
 else if(I==1) strcpy(MAP[I],"0W00000WWW");    
 else if(I==2) strcpy(MAP[I],"WWBWWW000W");    
 else if(I==3) strcpy(MAP[I],"W0P0B00B0W");    
 else if(I==4) strcpy(MAP[I],"W0DDW0B0WW");    
 else if(I==5) strcpy(MAP[I],"WWDDW000W0");    
 else if(I==6) strcpy(MAP[I],"0WWWWWWWW0");    
 } break;    
 case 4: /*第四关地图*/    
 for(I=0;I<MAXL;I++)    
 {    if(I==0) strcpy(MAP[I],"000WWWWWWW");    
 else if(I==1) strcpy(MAP[I],"00WW00W0PW");    
 else if(I==2) strcpy(MAP[I],"00W000W00W");    
 else if(I==3) strcpy(MAP[I],"00WB0B0B0W");    
 else if(I==4) strcpy(MAP[I],"00W0BWW00W");    
 else if(I==5) strcpy(MAP[I],"WWW0B0W0WW");    
 else if(I==6) strcpy(MAP[I],"WDDDDD00W0");    
 else if(I==7) strcpy(MAP[I],"WWWWWWWWW0");    
 } break;     
 case 5: /*第五关地图*/    
 for(I=0;I<MAXL;I++)    
 {    if(I==0) strcpy(MAP[I],"00WWWW00");    
 else if(I==1) strcpy(MAP[I],"00WDDW00");    
 else if(I==2) strcpy(MAP[I],"0WW0DWW0");    
 else if(I==3) strcpy(MAP[I],"0W00BDW0");    
 else if(I==4) strcpy(MAP[I],"WW0B00WW");    
 else if(I==5) strcpy(MAP[I],"W00WBB0W");    
 else if(I==6) strcpy(MAP[I],"W00P000W");    
 else if(I==7) strcpy(MAP[I],"WWWWWWWW");    
 } break;    
 case 6: /*第六关地图*/    
 for(I=0;I<MAXL;I++)    
 { if(I==0) strcpy(MAP[I],"0000WWWWW0000000000");    
 else if(I==1) strcpy(MAP[I],"0000W000W0000000000");    
 else if(I==2) strcpy(MAP[I],"0000WB00W0000000000");    
 else if(I==3) strcpy(MAP[I],"00WWW00BWW000000000");    
 else if(I==4) strcpy(MAP[I],"00W00B0B0W000000000");    
 else if(I==5) strcpy(MAP[I],"WWW0W0WW0W000WWWWWW");    
 else if(I==6) strcpy(MAP[I],"W000W0WW0WWWWW00DDW");    
 else if(I==7) strcpy(MAP[I],"W0B00B0000000000DDW");    
 else if(I==8) strcpy(MAP[I],"WWWWW0WWW0WPWW00DDW");    
 else if(I==9) strcpy(MAP[I],"0000W00000WWWWWWWWW");    
 else if(I==10)strcpy(MAP[I],"0000WWWWWWW00000000");    
 } break;     
 case 7: /*第七关地图*/    
 for(I=0;I<MAXL;I++)    
 { if(I==0) strcpy(MAP[I],"WWWWWWWWWWWW00");    
 else if(I==1) strcpy(MAP[I],"WDD00W00000WWW");    
 else if(I==2) strcpy(MAP[I],"WDD00W0B00B00W");    
 else if(I==3) strcpy(MAP[I],"WDD00WBWWWW00W");    
 else if(I==4) strcpy(MAP[I],"WDD0000P0WW00W");    
 else if(I==5) strcpy(MAP[I],"WDD00W0W00B0WW");    
 else if(I==6) strcpy(MAP[I],"WWWWWW0WWB0B0W");    
 else if(I==7) strcpy(MAP[I],"00W0B00B0B0B0W");    
 else if(I==8) strcpy(MAP[I],"00W0000W00000W");    
 else if(I==9) strcpy(MAP[I],"00WWWWWWWWWWWW");    
 } break;     
 case 8: /*第八关地图*/    
 for(I=0;I<MAXL;I++)    
 { if(I==0) strcpy(MAP[I],"00WWWWWW");    
 else if(I==1) strcpy(MAP[I],"00W0000W");    
 else if(I==2) strcpy(MAP[I],"WWWBBB0W");    
 else if(I==3) strcpy(MAP[I],"WP0BDD0W");    
 else if(I==4) strcpy(MAP[I],"W0BDDDWW");    
 else if(I==5) strcpy(MAP[I],"WWWW00W0");    
 else if(I==6) strcpy(MAP[I],"000WWWW0");    
 } break;     
 case 9: /*第九关地图*/    
 for(I=0;I<MAXL;I++)    
 { if(I==0) strcpy(MAP[I],"WWWWWWW");    
 else if(I==1) strcpy(MAP[I],"WDDBDDW");    
 else if(I==2) strcpy(MAP[I],"WDDWDDW");    
 else if(I==3) strcpy(MAP[I],"W0BBB0W");    
 else if(I==4) strcpy(MAP[I],"W00B00W");    
 else if(I==5) strcpy(MAP[I],"W0BBB0W");    
 else if(I==6) strcpy(MAP[I],"W00WP0W");    
 else if(I==7) strcpy(MAP[I],"WWWWWWW");    
 } break;     
 case 10: /*第十关地图*/    
 for(I=0;I<MAXL;I++)    
 { if(I==0) strcpy(MAP[I],"000WWWWW000");    
 else if(I==1) strcpy(MAP[I],"0WWW000WWW0");    
 else if(I==2) strcpy(MAP[I],"WW00PB0B0W0");    
 else if(I==3) strcpy(MAP[I],"W00WW0WW0WW");    
 else if(I==4) strcpy(MAP[I],"W0BDWDB000W");    
 else if(I==5) strcpy(MAP[I],"W0WDWFW000W");    
 else if(I==6) strcpy(MAP[I],"W0BDDD00WWW");    
 else if(I==7) strcpy(MAP[I],"WWWBW0WWW00");    
 else if(I==8) strcpy(MAP[I],"00W000W0000");    
 else if(I==9) strcpy(MAP[I],"00WWWWW0000");    
 } break;     
 }     
   
 wmove(win,SY-2,SX);   
 wprintw(win,"LEVEL %d \n",n);    
 for(I=0;I<MAXL;I++) /*备份原始地图的部分数据,以备移动时恢复图标使用*/    
 for(J=0;J<MAXC;J++) /*只用备份空白处,目的地和被箱子或推箱者占用的空白处与目的地*/    
 if(MAP[I][J]=='0'||MAP[I][J]=='D'||MAP[I][J]=='W') ORI[I][J]=MAP[I][J];    
 else if((MAP[I][J]=='F')||(MAP[I][J]=='T')) ORI[I][J]='D';    
 else ORI[I][J]='0';    
 DrawMap();     
}     
     
void DrawMap() /*画出第LEVEL关的初始化地图*/     
{ int I,J,temp;    
 char ch;    
 TOTAL=0; /*初始化统计当前所选关的箱子总数*/    
 wmove(win,SY,SX);   
 temp=SY+1;   
 for(I=0;I<MAXL;I++)    
 {    
   for(J=0;J<MAXC;J++) {    
       ch=MAP[I][J];    
       PutIco(ch);   
             /*判定初始化状态下推箱者的位置(MEX,MEY)*/    
      if(ch=='P'){ MEX=J+SX; MEY=I+SY;}    
     else if(ch=='T'||ch=='F'||ch=='D') TOTAL++;    
    }    
  wmove(win,temp++,SX);    
 }    
 wmove(win,MEY,MEX); /*光标总是跟随推箱者下边*/    
}    
   
void StartGame() /*开始游戏及移动处理函数*/    
{   
 int KEY,I=MEY-SY,J=MEX-SX; /*将推箱者的位置(MEX,MEY)换算成在二维数组MAP[][]中的相应位置*/    
 while(1)     
 {    
  wrefresh(win);   
 KEY=getch(); /* 等待自键盘输入字元*/   
 switch(KEY)    
 {   
 case 27 : endwin(); exit(0); /*按ESC键退出游戏*/    
 case '\t': ClearMap(); InitialMap(LEVEL);    
 StartGame(); break; /*按TAB 键重新开始当前游戏*/    
 case '\r': wmove(win,6,1);wprintw(win,"SELECT");  /*按回车键进行重新选关处理*/    
 scanw("%d",&LEVEL); /*注意没有以scanw()函数进行相应处理,所以要正确输入数据*/    
 ClearMap(); InitialMap(LEVEL);     
 StartGame(); break;    
 case 'a'   : if(MAP[I][J-1]=='0') /*推箱者向左移动到空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[I][--J]='P'; --MEX;    
 }     
 else if(MAP[I][J-1]=='B'||MAP[I][J-1]=='F') /*推箱者左边有箱子*/    
 { if(MAP[I][J-2]=='0') /*箱子的左边是空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[I][--J]='P'; --MEX;     
 wmove(win,MEY,MEX-1); PutIco('B');     
 MAP[I][J-1]='B';    
 }     
 else if(MAP[I][J-2]=='D') /*箱子的左边是目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; --MEX;     
 if(MAP[I][J-1]=='F') /*如果此箱子占据目的地*/    
 { MAP[I][--J]='T'; wmove(win,MEY,MEX);   
 wattron(win,COLOR_PAIR(1)); wprintw(win,"T");    
 }     
 else MAP[I][--J]='P'; /*如果此箱子占据空白处*/    
 wmove(win,MEY,MEX-1); wattron(win,COLOR_PAIR(4));  wprintw(win,"F");     
 MAP[I][J-1]='F';     
 }    
 }     
 else if(MAP[I][J-1]=='D') /*推箱者向左移动到目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; MAP[I][--J]='T';     
 wmove(win,MEY,--MEX); wattron(win,COLOR_PAIR(1));  wprintw(win,"T");     
 } break;    
 case 'd': if(MAP[I][J+1]=='0') /*推箱者向右移动到空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[I][++J]='P'; ++MEX;     
 }     
 else if(MAP[I][J+1]=='B'||MAP[I][J+1]=='F') /*推箱者右边有箱子*/    
 { if(MAP[I][J+2]=='0') /*箱子的右边是空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[I][++J]='P'; ++MEX;     
 wmove(win,MEY,MEX+1); PutIco('B');     
 MAP[I][J+1]='B';    
 }    
 else if(MAP[I][J+2]=='D') /*箱子的右边是目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; ++MEX;    
 if(MAP[I][J+1]=='F') /*如果此箱子占据目的地*/    
 { MAP[I][++J]='T';  wmove(win,MEY,MEX);   
  wattron(win,COLOR_PAIR(1));  wprintw(win,"T");    
 }     
 else MAP[I][++J]='P'; /*如果此箱子占据空白处*/    
 wmove(win,MEY,MEX+1);wattron(win,COLOR_PAIR(4));  wprintw(win,"F");     
 MAP[I][J+1]='F';     
  }    
 }     
 else if(MAP[I][J+1]=='D') /*推箱者向右移动到目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; MAP[I][++J]='T';     
 wmove(win,MEY,++MEX);wattron(win,COLOR_PAIR(1));  wprintw(win,"T");     
 } break;    
 case 'w': if(MAP[I-1][J]=='0') /*推箱者向上移动到空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[--I][J]='P'; --MEY;     
 }     
 else if(MAP[I-1][J]=='B'||MAP[I-1][J]=='F') /*推箱者上边有箱子*/    
 { if(MAP[I-2][J]=='0') /*箱子的上边是空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[--I][J]='P'; --MEY;     
 wmove(win,MEY-1,MEX);PutIco('B');     
 MAP[I-1][J]='B';    
 }    
 else if(MAP[I-2][J]=='D') /*箱子的上边是目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; --MEY;     
 if(MAP[I-1][J]=='F') /*如果此箱子占据目的地*/    
 { MAP[--I][J]='T';  wmove(win,MEY,MEX);   
 wattron(win,COLOR_PAIR(1));  wprintw(win,"T");    
 }     
 else MAP[--I][J]='P'; /*如果此箱子占据空白处*/    
 wmove(win,MEY-1,MEX);wattron(win,COLOR_PAIR(4));  wprintw(win,"F");     
 MAP[I-1][J]='F';     
 }    
 }     
 else if(MAP[I-1][J]=='D') /*推箱者向上移动到目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; MAP[--I][J]='T';     
  wmove(win,--MEY,MEX); wattron(win,COLOR_PAIR(1));   wprintw(win,"T");     
 } break;    
 case  's': if(MAP[I+1][J]=='0') /*推箱者向下移动到空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[++I][J]='P'; ++MEY;     
 }     
 else if(MAP[I+1][J]=='B'||MAP[I+1][J]=='F') /*推箱者下边有箱子*/    
 { if(MAP[I+2][J]=='0') /*箱子的下边是空白处*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J];     
 MAP[++I][J]='P'; ++MEY;     
 wmove(win,MEY+1,MEX); PutIco('B');     
 MAP[I+1][J]='B';    
 }    
 else if(MAP[I+2][J]=='D') /*箱子的下边是目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; ++MEY;     
 if(MAP[I+1][J]=='F') /*如果此箱子占据目的地*/    
 { MAP[++I][J]='T';   
  wmove(win,MEY,MEX);   
 wattron(win,COLOR_PAIR(1));   wprintw(win,"T");    
 }     
 else MAP[++I][J]='P'; /*如果此箱子占据空白处*/     
 wmove(win,MEY+1,MEX);wattron(win,COLOR_PAIR(4));  wprintw(win,"F");     
 MAP[I+1][J]='F';     
 }    
 }     
 else if(MAP[I+1][J]=='D') /*推箱者向下移动到目的地*/    
 { PutIco(ORI[I][J]); MAP[I][J]=ORI[I][J]; MAP[++I][J]='T';     
  wmove(win,++MEY,MEX);wattron(win,COLOR_PAIR(1));  wprintw(win,"T");     
 } break;    
 }     
 if(MAP[I][J]=='P') { wmove(win,MEY,MEX); PutIco('P'); } /*标出推箱者的当前位置*/    
     
 //ClearData(); /*清除显示的内存数据*/    
 //PutData(); /*显示内存数据,即二维数组中的数据*/    
   
 wmove(win,MEY,MEX);/*定位光标到推箱者下边*/    
 if(JudgeMap()) /*判定当前闯关是否成功*/     
 {   
  wmove(win,20,5);    
beep();   
  wprintw(win,"SUCCESSFUL,GOOD LUCK!"); /*成功时显示信息并做好进入下一关的准备*/   
  wrefresh(win);   
 getch();  
 wmove(win,20,5);   
 wprintw(win,"                     "); wrefresh(win);   
 ClearMap();    
 InitialMap(++LEVEL);    
 StartGame();    
 }    
 }    
}    
     
void ClearMap()     
{ int I,J;    
 wmove(win,SY,SX);   
 for(I=0;I<MAXL;I++)    
 { for(J=0;J<MAXC;J++)     
 wprintw(win," ");    
 wprintw(win,"\n");    
 }    
}    
   
void PutIco(char ch) /*用固定格式写入图标,即地图中的每个图形单位(字符)*/    
{ if(ch=='0') wprintw(win," ");    
 else if(ch=='W') { wattron(win,COLOR_PAIR(1));  wprintw(win,"W");}    
 else if(ch=='P') { wattron(win,COLOR_PAIR(4));  wprintw(win,"P"); }     
 else if(ch=='B') { wattron(win,COLOR_PAIR(2));  wprintw(win,"B");}    
 else if(ch=='D') { wattron(win,COLOR_PAIR(7));  wprintw(win,"D"); }     
 else if(ch=='F') { wattron(win,COLOR_PAIR(4));  wprintw(win,"F"); }    
 else if(ch=='T') { wattron(win,COLOR_PAIR(1));  wprintw(win,"T"); }   
 //else wprintw(win," ");  
wrefresh(win);   
}    
   
int JudgeMap() /*判定闯关是否成功,此函数可以改成在StartGame()函数中进行数据统计*/    
{ int I,J,N=0;    
 for(I=0;I<MAXL;I++)    
 for(J=0;J<MAXC;J++)    
 if(MAP[I][J]=='F') N++;    
 if(N==TOTAL) return 1;    
 else return 0;    
}    
   
/*------------程序检测部分--------------*/    
   
void PutData()    
{ int I,J;     
 for(I=0;I<MAXL;I++)    
 {  wmove(win,SY+I,30);   
 for(J=0;J<MAXC;J++)    
 wprintw(win,"%c",MAP[I][J]);     
 }    
}    
     
void ClearData()    
{int I,J;     
 for(I=0;I<MAXL;I++)    
 {  wmove(win,SY+I,30);   
 for(J=0;J<MAXC;J++)    
 wprintw(win," ");     
 }    
}     
    
void initial()    
    {    
      initscr();    
      cbreak();    
      nonl();    
      noecho();    
      intrflush(stdscr,FALSE);    
      keypad(stdscr,TRUE);    
      refresh();    
}    