#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include "game01.h"
 
bool set_up( bool );
void wrap_up();
void clear_up();


void show_Fence();

void show_Wall( int);
void cover_Wall( int ,int );

void show_score_gameon();
void show_score_gameoff();

void ball_move();
void in_bound( struct BallInfo * );
void in_bound2( struct BallInfo * );


void moving_baffle( int );
void show_my_moving_baffle();
void cover_my_moving_baffle();

void solve_sigint();
void solve_sigquit();

void on_input(int);
void enable_kbd_signals();

 

struct BallInfo Ball;
struct BallInfo Second_Ball;
struct BaffleInfo my_moving_Baffle;
struct WallInfo Wall[150];

int scoreC;
int scoreB;
int w;
int done=0;
int text=0;
//清空屏幕
void clear_up(){

       int i,j;

       for( i=0;i<24;i++ )
       {

              for( j=0;j<80;j++ )
                     mvaddch( i,j,BLANK);
       
       }

       return ;

}

bool set_up( bool flag ){

       int user_input;
       scoreC = 0;
       scoreB=0;
       
       //初始化小球1
       Ball.x_pos =(RIGHT_EDGE-LEFT_EDGE)/5;
       Ball.y_pos = 19;
       Ball.x_Time1 = Ball.x_Time2 = X_Time;
       Ball.y_Time1 = Ball.y_Time2 = Y_Time;
       Ball.x_dir = 1;
       Ball.y_dir = -1;
       Ball.ball_symbol = BALL;

    //初始化小球2

       Second_Ball.x_pos = 6;
       Second_Ball.y_pos = 6;
       Second_Ball.x_Time1 = Second_Ball.x_Time2 = X_Time;
       Second_Ball.y_Time1 = Second_Ball.y_Time2 = Y_Time;
       Second_Ball.x_dir = 1;
       Second_Ball.y_dir = -1;
       Second_Ball.ball_symbol = BALL;
 
    //初始化挡板
       my_moving_Baffle.len = BaffleLen;
       my_moving_Baffle.baffle_symbol = Baffle;
       my_moving_Baffle.x = startX_;
       my_moving_Baffle.y =startY_;

 

    //初始化障碍物   
          for(w=0;w<150;w++)
          {
       
                     if(w<50)
                     {
                Wall[w].x =8;
                            Wall[w].y = w+10;
                     }
                     else if(w>=50&&w<100)
                     {
                            Wall[w].x =9;
                            Wall[w].y = w-40;
                     }
                     else
                     {
                            Wall[w].x =10;
                            Wall[w].y = w-90;
                     }
                     Wall[w].flag = true;
        }
 


 

              initscr();
              noecho();
              crmode();
              keypad( stdscr,true );

    //画出墙壁    
              show_Fence();
        
      
     //游戏初始界面
              if( flag==true )
              {
                
                //显示游戏信息
              mvaddstr(2,LEFT_EDGE+1,String1);
                mvaddstr(2,LEFT_EDGE+23,String2);
                 mvaddstr(2,LEFT_EDGE+45,String3);

              mvaddstr( X_INIT-3,Y_INIT,"Are you ready for the game(Y/N)?" );
              mvaddstr( X_INIT-2,Y_INIT,"Press 'Left' to move the Baffle to the Left.");
              mvaddstr( X_INIT-1,Y_INIT,"Press 'Rignt' to move the Baffle to the Right.");
              mvaddstr( X_INIT,Y_INIT,"(:-D) See The Document for more details !!");
              
              move( LINES-1,COLS-1 );
              refresh();

              user_input = getch();
              if( user_input=='y'||user_input=='Y' ){ clear_up(); }
              else {

                     wrap_up();
                     return false;
              }

              }
    
    //游戏界面

      
              show_Fence();
        mvaddstr(2,LEFT_EDGE+1,String1);
        mvaddstr(2,LEFT_EDGE+23,String2);
              mvaddstr(2,LEFT_EDGE+45,String3);
              mvaddstr(3,LEFT_EDGE+1,String4);
        mvaddstr(3,LEFT_EDGE+23,String5);
              mvaddstr(3,LEFT_EDGE+45,String6);
        show_score_gameon();
        for(w=0;w<150;w++)
           show_Wall(w);
        mvaddch( Ball.y_pos, Ball.x_pos, Ball.ball_symbol );
        if(scoreB==1500)
          mvaddch( Second_Ball.y_pos, Second_Ball.x_pos, Second_Ball.ball_symbol );
        show_my_moving_baffle();
        refresh();

    //信号处理
        signal( SIGINT,solve_sigint );
        signal( SIGQUIT,solve_sigquit );
        signal( SIGALRM, ball_move );


       set_ticker( 1000 / TICKS_PER_SEC );

       return true;

}

//退出游戏
void wrap_up(){

       set_ticker( 0 );

       beep();

       endwin();

}

//选择让CTRL+C 退出游戏
void solve_sigint(){

      wrap_up();
      exit(0);

      return ;

}



 

void show_Wall( int i ){

       if( Wall[i].flag==true )
              mvaddstr( Wall[i].x,Wall[i].y,WALL);

       return ;

}

void solve_sigquit(){

       clear_up();
       set_up( false );
       return ;

}

void show_score_gameon(){


       mvaddstr(3,LEFT_EDGE+56,Tip1);

       char str1[105],str2[105],str3[105],str4[105];

       int cnt1 = 0,cnt2 = 0,cnt3=0,cnt4=0;

       int tp = scoreC/8,tp2=scoreB;

       int i;

       while( tp ){

              str1[ cnt1++ ] = tp%10 + '0';

              tp /= 10;

       }
       
       
       while( tp2 ){

              str3[ cnt3++ ] = tp2%10 + '0';

              tp2 /= 10;

       }
       
       for( i=cnt1-1;i>=0;i-- ){

              str2[ cnt2++ ] = str1[ i ];

       }
       
       for( i=cnt3-1;i>=0;i-- ){

              str4[ cnt4++ ] = str3[ i ];

       }
       if( cnt2==0 ) str2[ cnt2++ ] = '0';
          if(cnt4==0)str4[cnt4++]='0';

       str2[ cnt2 ] = '\0';
       str4[ cnt4 ]='\0';
       mvaddstr(3,LEFT_EDGE+10,str2);
       mvaddstr(3,LEFT_EDGE+30,str4);


       move( LINES-1,COLS-1 );
 

}

void show_score_gameoff(){


       mvaddstr(3,LEFT_EDGE+56,Tip2);

       char str1[ 105 ],str2[ 105 ];

       int cnt1 = 0,cnt2 = 0;

       int tp = scoreC;

       int i;

       while( tp ){

              str1[ cnt1++ ] = tp%10 + '0';

              tp /= 10;

       }

       for( i=cnt1-1;i>=0;i-- ){

              str2[ cnt2++ ] = str1[ i ];

       }

       if( cnt2==0 ) str2[ cnt2++ ] = '0';

       str2[ cnt2 ] = '\0';


       
       mvaddstr(LEFT_EDGE+7,TopRow+7,"what a pity!!");
       
       mvaddstr( LEFT_EDGE+7,TopRow+22,"Press the 'ctrl + \\' to play again." );

       move( LINES-1,COLS-1 );

}

void ball_move(){

       signal( SIGALRM,SIG_IGN );//这个符号表示忽略该信号，执行了相应的signal()调用后，进程会忽略类型为sig的信号。

      

       bool moved2 = false;
       bool moved = true;

       int x_cur = Ball.x_pos;
       int y_cur = Ball.y_pos;

       int sec_x_cur=Second_Ball.x_pos;
       int sec_y_cur=Second_Ball.y_pos;


       bool crash_ed ;



       if( Ball.x_Time1>0 && ((Ball.x_Time2--) == 1) ){

              moved = true;

              Ball.x_pos += Ball.x_dir;

              Ball.x_Time2 = Ball.x_Time1;

       }

 

       if( Ball.y_Time1>0 && ((Ball.y_Time2--) == 1) ){

              moved = true;

              Ball.y_pos += Ball.y_dir;

              Ball.y_Time2 = Ball.y_Time1;

       }

 

       if( moved == true ){

           
              mvaddch( y_cur,x_cur,BLANK );

              mvaddch( Ball.y_pos,Ball.x_pos,Ball.ball_symbol );

              in_bound( &Ball );
       
              
              move( LINES-1,COLS-1 );
              
              refresh();

       }


//////////////////////////////////////////////////////////////////////////////////////////////
     if(scoreB==1500)
       {

           if( Second_Ball.x_Time1>0 && ((Second_Ball.x_Time2--) == 1) ){

              moved2 = true;

              Second_Ball.x_pos += Second_Ball.x_dir;

              Second_Ball.x_Time2 =Second_Ball.x_Time1;

        }

 

       if( Second_Ball.y_Time1>0 && ((Second_Ball.y_Time2--) == 1) ){

              moved2 = true;

              Second_Ball.y_pos += Second_Ball.y_dir;

              Second_Ball.y_Time2 = Second_Ball.y_Time1;

        }

 

       if( moved2 == true ){

              

              mvaddch( sec_y_cur,sec_x_cur,BLANK );

              mvaddch( Second_Ball.y_pos,Second_Ball.x_pos,Second_Ball.ball_symbol );

              in_bound2( &Second_Ball );
       
              
              move( LINES-1,COLS-1 );
              
              refresh();

       }
    }


       signal( SIGALRM,ball_move );

       return ;

}


 
void in_bound( struct BallInfo *myballPoint ){

       
       int i;    

    //对于小球1需要先处理撞上WALL的情况
       for(i=0;i<150;i++)
       {
            if( Ball.y_pos==Wall[i].x && Ball.x_pos==Wall[i].y &&Wall[i].flag ==true){
          
                     mvaddstr(Wall[i].x,Wall[i].y," ");
                     Wall[i].flag = false;
                           mvaddch( Wall[i].x,Wall[i].y,Ball.ball_symbol );
              
                           scoreB+=10;
                           show_score_gameon();
                        myballPoint->y_dir *= -1;

                     }


       }

       if( myballPoint->x_pos==LEFT_EDGE ){

              myballPoint->x_dir = 1;

       }
       else if( myballPoint->x_pos==RIGHT_EDGE ){

              myballPoint->x_dir = -1;


       }

       if( myballPoint->y_pos==TopRow ){

              myballPoint->y_dir = 1;


       }

       else if( myballPoint->y_pos==BotRow ){

              if( ((myballPoint->x_pos)>=my_moving_Baffle.y) && ((myballPoint->x_pos)<=my_moving_Baffle.y+my_moving_Baffle.len) ){

                     myballPoint->y_dir = -1;

                     scoreC ++;

                     show_score_gameon();



              }

              else{

                     show_score_gameoff();

                     wrap_up();

              }

       }

}


void in_bound2( struct BallInfo *myballPoint )
{

bool return_dir = false;
 if( myballPoint->x_pos==LEFT_EDGE ){

              myballPoint->x_dir = 1;

              

       }//left

       else if( myballPoint->x_pos==RIGHT_EDGE ){

              myballPoint->x_dir = -1;

            

       }//right

       if( myballPoint->y_pos==TopRow ){

              myballPoint->y_dir = 1;

              

       }//up

       else if( myballPoint->y_pos==BotRow ){

              if( ((myballPoint->x_pos)>=my_moving_Baffle.y) && ((myballPoint->x_pos)<=my_moving_Baffle.y+my_moving_Baffle.len) ){

                     myballPoint->y_dir = -1;

                     scoreC ++;

                     show_score_gameon();

                
              }

              else{

                     show_score_gameoff();

                     wrap_up();

              }

       }//down

}
 

void show_Fence(){

       int i;

       for( i=1;i<=BotRow+1;i++ ){

              mvaddch( i,LEFT_EDGE-1,Fence );
              mvaddch( i,RIGHT_EDGE+1,Fence);

       }

       for( i=LEFT_EDGE-1;i<=RIGHT_EDGE+1;i++ ){
           
             mvaddch(1,i,Fence);
          mvaddch( TopRow-1,i,Fence );
             mvaddch(BotRow+2,i,Fence);

       }

       return ;

}



void moving_baffle( int user_input ){

       if( user_input==LEFT ){

              if( my_moving_Baffle.y-1 >= LEFT_EDGE ){

                     cover_my_moving_baffle();

                     my_moving_Baffle.y --;

                     show_my_moving_baffle();

              }

       }

  

       if( user_input==RIGHT ){

              if( my_moving_Baffle.y+my_moving_Baffle.len<=RIGHT_EDGE ){

                     cover_my_moving_baffle();

                     my_moving_Baffle.y ++;

                     show_my_moving_baffle();

              }

       }



}



void cover_my_moving_baffle(){

       int i;

       for( i=0;i<my_moving_Baffle.len;i++ )

              mvaddch( my_moving_Baffle.x,my_moving_Baffle.y+i,BLANK );

       return ;

}

void show_my_moving_baffle(){

       int i;

       for( i=0;i<my_moving_Baffle.len;i++ )

              mvaddch( my_moving_Baffle.x,my_moving_Baffle.y+i,my_moving_Baffle.baffle_symbol );

       return ;

}


int main(){

       
      
       bool flag = set_up( true );
       signal(SIGIO,on_input);
       enable_kbd_signals();

       if( flag==false )
              return 0;

       while(! done)
             pause();
       
     
       wrap_up();
       //exit（0）；

       return 0;

}

void on_input(int signum)
{
       int user_input=getch();

       keypad(stdscr,true);

        if( user_input=='s'&&Ball.x_Time1>Min ) Ball.x_Time1 --;

        else if( user_input=='f'&&Ball.x_Time1<Max ) Ball.x_Time1 ++;

        if( user_input=='S'&&Ball.y_Time1>Min ) Ball.y_Time1 --;

        else if( user_input=='F'&&Ball.y_Time1<Max ) Ball.y_Time1 ++;


        if( user_input=='K'&&my_moving_Baffle.len>=2 ) {

                     cover_my_moving_baffle();

                     my_moving_Baffle.len --;

                     show_my_moving_baffle();

              }

        else if( user_input=='L'&&my_moving_Baffle.y+my_moving_Baffle.len<RIGHT_EDGE ){

                     cover_my_moving_baffle();

                     my_moving_Baffle.len ++;

                     show_my_moving_baffle();

              }

   
        if( user_input=='P'&& text%2==0 ) {
                     
            set_ticker( 0 );
                  text++;
         }
           else if(user_input=='P'&& text%2==1 ){
       
                set_ticker( 1000 / TICKS_PER_SEC );
                text++;
        }

        if( user_input==LEFT||user_input==RIGHT ){

              moving_baffle(user_input);

         }
              
}
void enable_kbd_signals()
{
       int fd_flags;
       
       fcntl(0,F_SETOWN,getpid());
       fd_flags=fcntl(0,F_GETFL);
       fcntl(0,F_SETFL,(fd_flags|O_ASYNC));
}