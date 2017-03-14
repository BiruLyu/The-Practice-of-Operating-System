#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include "hurry_up.h"
#include <termios.h>

void InitialMap(int n) /*初始化二维数组,即地图的数据设置*/    
{     int i,j;
  switch(n)    
  {   
  case 1:     
     for(i=0;i<ROW;i++)
      for(j=0;j<COL;j++)
         level1[i][j]=map1[i][j];   
    break;    
  case 2: /*第二关地图*/     
    for(i=0;i<ROW;i++)
      for(j=0;j<COL;j++)
         level1[i][j]=map2[i][j];   
    break;     
  case 3: /*第三关地图*/    
    for(i=0;i<ROW;i++)
      for(j=0;j<COL;j++)
         level1[i][j]=map3[i][j];   
    break;  
}     

}

/////////////////////////////////
void count_time()
{       
        
	char *time="Left Time:", *sec="s", *over="Game Over!!";
        char str1[ 105 ],str2[ 105 ];

        int cnt1 = 0,cnt2 = 0;

        int tp ;

        int i;
        start_color();
        init_pair(20,COLOR_RED,COLOR_BLACK);
        attron(COLOR_PAIR(20)|A_BOLD);
        left_time--;
        tp=left_time;
	move(6,60);
	addstr(time);
       
         while( tp ){

              str1[ cnt1++ ] = tp%10 + '0';

              tp /= 10;

       }
	
       
       for( i=cnt1-1;i>=0;i-- ){

              str2[ cnt2++ ] = str1[ i ];

       }
	
       if( cnt2==0 ) str2[ cnt2++ ] = '0';

       str2[ cnt2 ] = '\0';
 
       mvaddstr(6,70,BLANK) ; 
       mvaddstr(6,71,BLANK) ; 
       mvaddstr(6,70,str2);
	
       move(6,72);
       addstr(sec);
       
       

       if(left_time==0)
       {
            mvaddstr(12,60,"Game Over!!");
            mvaddstr(14,49,"Press the'ctrl + \\'to restart.");
            move(LINES-1,COLS-1);
            crash=1;
            set_ticker(0);
        }
        attroff(COLOR_PAIR(20)|A_BOLD);
         
       refresh();
       
}
/////////////////////////////////////////////////////////////////////////////
 



bool Moving_Ball1(){

  
       my_moving_brick[0].x =6;

       my_moving_brick[0].y =5;

       my_moving_brick[0].speed = speed1;

       my_moving_brick[0].dir = 1;

       my_moving_brick[0].flag = true;

      

       my_moving_brick[1].x =9;

       my_moving_brick[1].y =9;

       my_moving_brick[1].speed = speed1;

       my_moving_brick[1].dir = 1;

       my_moving_brick[1].flag = true;
    


       my_moving_brick[2].x =14;

       my_moving_brick[2].y =25;

       my_moving_brick[2].speed = speed1;

       my_moving_brick[2].dir = 1;

       my_moving_brick[2].flag = true;


       my_moving_brick[3].x =20;

       my_moving_brick[3].y =43;

       my_moving_brick[3].speed = speed1;

       my_moving_brick[3].dir = 1;

       my_moving_brick[3].flag = true;
 



       noecho();

       crmode();

       keypad( stdscr,true );

         if(Game_flag<5&&Game_flag>0)
       {    
       show_my_moving_brick(0,my_moving_brick[0].x,my_moving_brick[0].y );
       show_my_moving_brick(1,my_moving_brick[1].x,my_moving_brick[1].y );
       show_my_moving_brick(2,my_moving_brick[2].x,my_moving_brick[2].y );
       show_my_moving_brick(3,my_moving_brick[3].x,my_moving_brick[3].y );
       refresh();
       signal( SIGALRM, ball_move);
       set_ticker( 1000 / TICKS_PER_SEC );
       }

       return true;

}

///////////////////////////////////////////////////////////////////
void wrap_up(){

       set_ticker( 0 );

       beep();

       endwin();
       exit(0);

}


//////////////////////////////////////////////////////////////////
void show_my_moving_brick(int i, int x,int y ){
       if(has_colors()==TRUE)
        {
                start_color();
	        init_pair(7,COLOR_GREEN,COLOR_CYAN);//ball
        }

       if( my_moving_brick[i].flag==true ){
              attron(COLOR_PAIR(7));

              mvaddstr( x,y,BALL);

              move( LINES-1,COLS-1 );
              
              attroff(COLOR_PAIR(7));

         }
       
       return ;

}
///////////////////////////////////////////////////////////////
void cover_my_moving_brick( int x,int y ){

       if(has_colors()==TRUE)
        {
                start_color();
	        init_pair(8,COLOR_GREEN,COLOR_WHITE);//ball
        }
       attron(COLOR_PAIR(8));
       mvaddstr( x,y,"  " );

       move( LINES-1,COLS-1 );
       attroff(COLOR_PAIR(8));

       return ;

}


/////////////////////////////////////////////////////////////////
void ball_move(){
       

       signal( SIGALRM,SIG_IGN );
	
       int cur[4][2];
       int i;
       for(i=0;i<4;i++)
       {
	   cur[i][0]=my_moving_brick[i].x;
           cur[i][1]=my_moving_brick[i].y;
	   
       }

       if( (my_moving_brick[0].speed--)==1 ){

              my_moving_brick[0].y +=my_moving_brick[0].dir;

              if( my_moving_brick[0].y+2==Position[0][0]||my_moving_brick[0].y==Position[0][1]) my_moving_brick[0].dir *= (-1);
 
              my_moving_brick[0].speed = speed1;

       }
       if( (my_moving_brick[1].speed--)==1 ){

              my_moving_brick[1].y +=my_moving_brick[1].dir;

              if( my_moving_brick[1].y+2==Position[1][0]||my_moving_brick[1].y==Position[1][1]) my_moving_brick[1].dir *= (-1);
 
              my_moving_brick[1].speed = speed1;

       }
	if( (my_moving_brick[2].speed--)==1 ){

              my_moving_brick[2].x +=my_moving_brick[2].dir;

              if( my_moving_brick[2].x+1==Position[2][1]||my_moving_brick[2].x==Position[2][0]) my_moving_brick[2].dir *= (-1);
 
              my_moving_brick[2].speed = speed1;

       }
       	if( (my_moving_brick[3].speed--)==1 ){

              my_moving_brick[3].x -=my_moving_brick[3].dir;

              if( my_moving_brick[3].x+1==Position[3][0]||my_moving_brick[3].x==Position[3][1]) my_moving_brick[3].dir *= (-1);
 
              my_moving_brick[3].speed = speed1;

       }
       cover_my_moving_brick( my_moving_brick[0].x,cur[0][1]);
       cover_my_moving_brick( my_moving_brick[1].x,cur[1][1]);
       cover_my_moving_brick( cur[2][0],my_moving_brick[2].y );
       cover_my_moving_brick( cur[3][0],my_moving_brick[3].y );
 
       for(i=0;i<4;i++)
          show_my_moving_brick( i,my_moving_brick[i].x,my_moving_brick[i].y );
       
        if(++count==25)
       {   count_time();   count=0;   }
 

       move( LINES-1,COLS-1 );
       show_gameover();
       refresh();

       signal( SIGALRM,ball_move );

       return ;

}


/////////////////////////////////////////////////////////////////
int set_ticker( int n_msecs ){

       struct itimerval new_timeset;

       long n_sec, n_usecs;

 

       n_sec = n_msecs / 1000;

       n_usecs = ( n_msecs %1000 ) * 1000L;

      

       new_timeset.it_interval.tv_sec = n_sec;

       new_timeset.it_interval.tv_usec = n_usecs;

       new_timeset.it_value.tv_sec = n_sec;

       new_timeset.it_value.tv_usec = n_usecs;

      

       return setitimer( ITIMER_REAL, &new_timeset, NULL );

}

/////////////////////////////////////////////////////////////
//piant the game map
void show_map(int map[ROW][COL])
{
	int lef,top;
	int i,j;
	lef=LEFT_EDGE;
	top=TOP_EDGE;
        if(has_colors()==TRUE)
        {
                start_color();
	        init_pair(51,COLOR_GREEN,COLOR_GREEN);//map1
                init_pair(52,COLOR_GREEN,COLOR_YELLOW);//map2
                init_pair(53,COLOR_GREEN,COLOR_GREEN);//map3
	        init_pair(3,COLOR_YELLOW,COLOR_RED);//person
	        init_pair(4,COLOR_GREEN,COLOR_BLACK);//ball
	        init_pair(5,COLOR_RED,COLOR_BLUE);//food
         	init_pair(6,COLOR_BLACK,COLOR_WHITE);//road
        }

	for(i=0;i<ROW;i++)
		for(j=0;j<COL;j++)
			switch(map[i][j])
			{
			//ball
			case 0:
				{
					attron(COLOR_PAIR(4)|A_BOLD);
					move(top+i+1,lef+j+1);
					addstr(BALL);
					attroff(COLOR_PAIR(4)|A_BOLD);break;
				}
                        //wall
			case 1:
				{
					attron(COLOR_PAIR(50+Game_flag));
					move(top+i+1,lef+j+1);
					addstr(BLANK);
					attroff(COLOR_PAIR(50+Game_flag));break;
				}
			//person
			case 2:
				{
					attron(COLOR_PAIR(3)|A_BOLD);
					move(top+i+1,lef+j+1);
					addstr(PERSON);
					attroff(COLOR_PAIR(3)|A_BOLD);break;
				}
			//food
			case 3:
				{
					attron(COLOR_PAIR(5)|A_BOLD);
					move(top+i+1,lef+j+1);
					addstr(FOOD);
					attroff(COLOR_PAIR(5)|A_BOLD);break;
				}
                        //road
			case 4:
				{
					attron(COLOR_PAIR(6));
					move(top+i+1,lef+j+1);
					addstr(BLANK);
					attroff(COLOR_PAIR(6));break;
				}
			}




       Role.len = 1;
       Role.x =3;
       Role.y =5;


       	initscr();
       	noecho();
       	crmode();
       	keypad( stdscr,true );

        

        move(LINES-1,COLS-1); 
	refresh();
}

/////////////////////////////////////////////////////////////
void paintwall()
{
        int i,j;
        char *l="LEVEL ",*block1="|",*block2="=";
        char str1[ 105 ],str2[ 105 ];

        int cnt1 = 0,cnt2 = 0;
        int tp;
        tp = Game_flag;

        while( tp ){

              str1[ cnt1++ ] = tp%10 + '0';

              tp /= 10;

       }
	
       
       for( i=cnt1-1;i>=0;i-- ){

              str2[ cnt2++ ] = str1[ i ];

       }
	
       if( cnt2==0 ) str2[ cnt2++ ] = '0';

       str2[ cnt2 ] = '\0';
 
        if(has_colors()==TRUE)
        {
                start_color();
                init_pair(54,COLOR_GREEN,COLOR_MAGENTA);//wall1
                init_pair(55,COLOR_GREEN,COLOR_MAGENTA);//wall2
                init_pair(56,COLOR_GREEN,COLOR_BLUE);//wall3
                init_pair(40,COLOR_BLUE,COLOR_BLACK);
                init_pair(41,COLOR_RED,COLOR_BLACK);
        }
        attron(COLOR_PAIR(53+Game_flag)|A_BOLD);
        for(i=TOP_EDGE,j=LEFT_EDGE;j<=RIGHT_EDGE;j++)
        {        
                move(i,j);
                addstr(BLANK);
        }
        for(i=BUTTOM_EDGE,j=LEFT_EDGE;j<=RIGHT_EDGE;j++)
        {
                move(i,j);
                addstr(BLANK);
        }
        for(j=LEFT_EDGE,i=TOP_EDGE;i<=BUTTOM_EDGE;i++)
        {
                move(i,j);
                addstr(BLANK);
        }
        for(j=LEFT_EDGE-1,i=TOP_EDGE;i<=BUTTOM_EDGE;i++)
        {
                move(i,j);
                addstr(BLANK);
        }
        for(j=RIGHT_EDGE,i=TOP_EDGE;i<BUTTOM_EDGE-1;i++)
        {
                move(i,j);
                addstr(BLANK);
        }
        for(j=RIGHT_EDGE+1,i=TOP_EDGE;i<BUTTOM_EDGE-1;i++)
        {
                move(i,j);
                addstr(BLANK);
        }
        attroff(COLOR_PAIR(53+Game_flag)|A_BOLD);

        attron(COLOR_PAIR(41)|A_BOLD);
        mvaddstr(4,62,l);
        mvaddstr(4,68,str2);
        attroff(COLOR_PAIR(41)|A_BOLD);
        attron(COLOR_PAIR(40)|A_BOLD);

        for(i=4;i<=10;i++)
           mvaddstr(i,55,block1);
        for(i=4;i<=10;i++)
           mvaddstr(i,75,block1);

        for(j=55;j<=75;j++)
           mvaddstr(10,j,block2);
        for(j=55;j<=61;j++)
           mvaddstr(4,j,block2);
        for(j=69;j<=75;j++)
           mvaddstr(4,j,block2);
        attroff(COLOR_PAIR(40)|A_BOLD);
        count_time();
        show_score();

        refresh();
}

////////////////////////////////////////////////////////////////////
void on_input(int signum)
{
	int user_input=getch();
    int i,j;

	keypad(stdscr,true);

    if(has_colors()==TRUE)
    {
        start_color();
	    init_pair(9,COLOR_WHITE,COLOR_WHITE);//ball
        init_pair(10,COLOR_YELLOW,COLOR_RED);
    }
       
    if(crash!=1&&left_time!=0&&Game_flag<5&&pause_digit==0)
    { 
        if( user_input==KEY_LEFT )
        {
    
	        if(level1[Role.x-TOP_EDGE-1][Role.y-LEFT_EDGE-2]!=1&&Role.y-1!=LEFT_EDGE)
            {
		        if(level1[Role.x-TOP_EDGE-1][Role.y-LEFT_EDGE-2]==3)
		        {
		        	Score++;
		        	level1[Role.x-TOP_EDGE-1][Role.y-LEFT_EDGE-2]=4;
		        }    
                show_score();					
                attron(COLOR_PAIR(9));
                mvaddstr( Role.x,Role.y,BLANK );
                attroff(COLOR_PAIR(9));
                Role.y --;
                attron(COLOR_PAIR(10)|A_BOLD);
                mvaddstr( Role.x,Role.y,PERSON );
                attroff(COLOR_PAIR(10)|A_BOLD);
            }

        }

        if( user_input==KEY_RIGHT )
        {
           if(!(Score == 4*Game_flag && Role.x==20&&Role.y==44))
           {
           if(level1[Role.x-TOP_EDGE-1][Role.y-LEFT_EDGE]!=1&&Role.y+1!=RIGHT_EDGE)
            {
	        if(level1[Role.x-TOP_EDGE-1][Role.y-LEFT_EDGE]==3)
	        {
		    Score++;
		    level1[Role.x-TOP_EDGE-1][Role.y-LEFT_EDGE]=4;
		}      
                show_score();
                attron(COLOR_PAIR(9));
                mvaddstr( Role.x,Role.y,BLANK );
                attroff(COLOR_PAIR(9));
                Role.y ++;
                attron(COLOR_PAIR(10)|A_BOLD);
                mvaddstr( Role.x,Role.y,PERSON );
                attroff(COLOR_PAIR(10)|A_BOLD);

            }
           }
           else if(Score == 4*Game_flag && Role.x==20&&Role.y==44)
           { 
                attron(COLOR_PAIR(9));
                mvaddstr( Role.x,Role.y,BLANK );
                attroff(COLOR_PAIR(9));
                Role.y ++;
                attron(COLOR_PAIR(10)|A_BOLD);
                mvaddstr( Role.x,Role.y,PERSON );
                attroff(COLOR_PAIR(10)|A_BOLD);
                Game_flag++;
                set_up(Game_flag);
           }
        }
	    if( user_input==KEY_UP )
	    {
            if(level1[Role.x-TOP_EDGE-2][Role.y-LEFT_EDGE-1]!=1&&Role.x-1!=TOP_EDGE)
            {    
			    if(level1[Role.x-TOP_EDGE-2][Role.y-LEFT_EDGE-1]==3)
				{
		        	Score++;
		        	level1[Role.x-TOP_EDGE-2][Role.y-LEFT_EDGE-1]=4;
		        } 
                show_score();
                attron(COLOR_PAIR(9));
                mvaddstr( Role.x,Role.y,BLANK );
                attroff(COLOR_PAIR(9));
                Role.x --;
                attron(COLOR_PAIR(10)|A_BOLD);
                mvaddstr( Role.x,Role.y,PERSON );
                attroff(COLOR_PAIR(10)|A_BOLD);
              }
        }

       if( user_input==KEY_DOWN )
       {
            if(level1[Role.x-TOP_EDGE][Role.y-LEFT_EDGE-1]!=1&&Role.x+1!=BUTTOM_EDGE)
            {
                if(level1[Role.x-TOP_EDGE][Role.y-LEFT_EDGE-1]==3)
				{
		        	Score++;
		        	level1[Role.x-TOP_EDGE][Role.y-LEFT_EDGE-1]=4;
		        } 
                show_score();
                attron(COLOR_PAIR(9));
                mvaddstr( Role.x,Role.y,BLANK );
                attroff(COLOR_PAIR(9));
                Role.x ++;
                attron(COLOR_PAIR(10)|A_BOLD);
                mvaddstr( Role.x,Role.y,PERSON );
                attroff(COLOR_PAIR(10)|A_BOLD);
            }

       }

    }
    if( (user_input==32)&& text%2==0 ) {
                     
            set_ticker( 0 );
            pause_digit=1;
            text++;
         }
        else if( (user_input==32)&& text%2==1 ){
       
                set_ticker( 1000 / TICKS_PER_SEC );
                pause_digit=0;
                text++;
        }
       
    if(user_input=='r'||user_input=='R')
    {
          crash=0;
          Game_flag=1;
          Score=0;
          count=0;
          text=0;
          clear();
          set_ticker(0);
          //set_ticker( 1000 / TICKS_PER_SEC );
          set_up(0);
         }
     else if(user_input=='q'||user_input=='Q')
     {
                 
                 wrap_up();
                
     }

	
}

///////////////////////////////////////////////////////////////////////
void enable_kbd_signals()
{
	int fd_flags;
	fcntl(0,F_SETOWN,getpid());
	fd_flags=fcntl(0,F_GETFL);
	fcntl(0,F_SETFL,(fd_flags|O_ASYNC));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show_score(){

       char str1[ 105 ],str2[ 105 ];
       
       char *score = "Score:   ";
       int cnt1 = 0,cnt2 = 0;

       int tp = Score;

       int i;
       start_color();
       init_pair(21,COLOR_RED,COLOR_BLACK);
       attron(COLOR_PAIR(21)|A_BOLD);

       mvaddstr(8,60,score);
       while( tp ){

              str1[ cnt1++ ] = tp%10 + '0';

              tp /= 10;

       }
	
       
       for( i=cnt1-1;i>=0;i-- ){

              str2[ cnt2++ ] = str1[ i ];

       }
	
       if( cnt2==0 ) str2[ cnt2++ ] = '0';

       str2[ cnt2 ] = '\0';
       
       mvaddstr(8,66,str2);
      
       move( LINES-1,COLS-1 );
       attroff(COLOR_PAIR(21)|A_BOLD);


}
//////////////////////////////////////////////////////////////////////////////
void show_gameover()
{
	int i; 
        start_color();
        init_pair(22,COLOR_RED,COLOR_BLACK);
        attron(COLOR_PAIR(22)|A_BOLD);
        for(i=0;i<4;i++)
        {
             
	     if(my_moving_brick[i].x==Role.x&&(my_moving_brick[i].y==Role.y||my_moving_brick[i].y==Role.y-1))
	     {
                mvaddstr(12,60,"Game Over!!");
                mvaddstr(14,49,"Press the'ctrl + \\'to restart.");
                move(LINES-1,COLS-1);
                crash=1;
                set_ticker(0);

             }
        }
        attroff(COLOR_PAIR(22)|A_BOLD);
}
//////////////////////////////////////////////////
void welcome()
{
     int row1 = 6,  row2 = 18,
         col1 = 12, col2 = 61,
         word1 = 33, word2 = 41;
     int i,j;
     char od;
     char *name="Hurry Up",
          *star="*",
          *Wel="Welcome to our game!",
          *ready="Are you ready for the game(Y/N)?",
          *guid="Press 'G' to view guidance";
     start_color();
     init_pair(24,COLOR_RED,COLOR_WHITE);
     attron(COLOR_PAIR(24)|A_BOLD);
     
     for(i=row1;i<=row2;i++)
        for(j=col1;j<=col2;j++)
           mvaddstr(i,j,BLANK);

     move(row1,word1);
     addstr(name);
     mvaddstr(9,27,Wel);
     mvaddstr(11,20,ready);
     mvaddstr(13,22,guid);

     tty_mode(0);
     set_nodelay_mode();
   while(1)
   {
     for(i=row1;i<=row2;i++)
        for(j=col1;j<=col2;j++)
           mvaddstr(i,j,BLANK);

     move(row1,word1);
     addstr(name);
     mvaddstr(9,27,Wel);
     mvaddstr(11,20,ready);
     mvaddstr(13,22,guid);

    //top edge
     for(i=row1,j=col1;j<word1;j++)
     {
           move(i,j);
           if(j%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }

      for(i=row1,j=word2;j<=col2;j++)
     {
           move(i,j);
           if(j%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }
     //buttom edge
     for(i=row2,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }
      //left edge
      for(i=row1,j=col1;i<=row2;i++)
     {
           move(i,j);
           if(i%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }

      //right edge
      for(i=row1,j=col2;i<=row2;i++)
     {
           move(i,j);
           if(i%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }
      move( LINES-1,COLS-1 );
      refresh();

      usleep(100000);
   
      //top edge
     for(i=row1,j=col1;j<word1;j++)
     {
           move(i,j);
           if(j%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }

      for(i=row1,j=word2;j<=col2;j++)
     {
           move(i,j);
           if(j%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }
     //buttom edge
     for(i=row2,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }
      //left edge
      for(i=row1,j=col1;i<=row2;i++)
     {
           move(i,j);
           if(i%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }

      //right edge
      for(i=row1,j=col2;i<=row2;i++)
     {
           move(i,j);
           if(i%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }
      move( LINES-1,COLS-1 );
      refresh();
      usleep(100000);
      
      od = getch();
      if( od=='y'||od=='Y' )
         {  attroff(COLOR_PAIR(24)|A_BOLD);Game_flag=1;set_up(1);
            tty_mode(1);break;}
      else if ( od=='n'||od=='N'){
                 tty_mode(1);
                 wrap_up();}
      else if(od=='g'||od=='G')
          {  
               //  top edge
           for(i=row1,j=col1;j<word1;j++)
           {
                move(i,j);
                if(j%2==0)
                addstr(star);
                else
                addstr(BLANK);
           }
    
          for(i=row1,j=word2;j<=col2;j++)
          {
              move(i,j);
              if(j%2==0)
              addstr( star);
              else
              addstr(BLANK);
          }
     //buttom edge
     for(i=row2,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }
      //left edge
      for(i=row1,j=col1;i<=row2;i++)
     {
           move(i,j);
           
              addstr(star);
           
      }

      //right edge
      for(i=row1,j=col2;i<=row2;i++)
     {
           move(i,j);
           
              addstr(star);
           
      }
                 move( LINES-1,COLS-1 );
      

                 mvaddstr(9,27,"                    ");
                 mvaddstr(11,20,"                                ");
                 mvaddstr(13,22,"                          ");

                 mvaddstr(7,14,"When you play the game, you can:");
                 mvaddstr(8,14,"Press 'LEFT' to go left;");
                 mvaddstr(9,14,"Press 'RIGHT' to go right;");
                 mvaddstr(10,14,"Press 'UP' to go up;");
                 mvaddstr(11,14,"Press 'DOWN' to go down;");
                 mvaddstr(12,14,"Press space to pause;");
                 mvaddstr(13,14,"Press 'Ctrl+\\' restart this checkpoint;");
                 mvaddstr(14,14,"Press 'R' to back to the begin;");
                 mvaddstr(15,14,"Press 'Q' to exit;");
                 mvaddstr(16,14,"You must eat all the food before going out.");
                 mvaddstr(17,38,"Now, press 'B' to back.");
                 move(LINES-1,COLS-1);
                 refresh();

                  
               while(1)
                {
                 
                 if(getch()=='b')
                 {clear();
                 
                 break;
                 }
                      
               }
           }
        }
       
   
}


//////////////////////////////////////////////////
bool set_up(int Game_flag)
{
        int row1 = 6,  row2 = 18,
         col1 = 12, col2 = 61;
     int i,j;
     char od;
     char *star="*",
          *con="Congratulation!",
          *pass="You have already passed all the checkpoints!!",
          *play="Press the 'R' to play again.",
          *giveup="Preess the 'Q' to exit.";
    clear();
    
    switch(Game_flag)
    {  
       case 0:
            welcome();
            break;
       case 1:
            left_time=30;
            count_time();
            paintwall();
            InitialMap(1);
            show_map(level1);
            Moving_Ball1();
            show_score();
            break;
       case 2:
            left_time=40;
            count_time();
            paintwall();
            InitialMap(2);
            show_map(level1);
            Moving_Ball1();
            show_score();
            break;
       case 3:
            left_time=50;
            count_time();
            paintwall();
            InitialMap(3);
            show_map(level1);
            Moving_Ball1();
            show_score();
            break;
       default :
            set_ticker(0);

     start_color();
     init_pair(30,COLOR_YELLOW,COLOR_CYAN);
     attron(COLOR_PAIR(30)|A_BOLD);
     for(i=row1;i<=row2;i++)
        for(j=col1;j<=col2;j++)
           mvaddstr(i,j,BLANK);
     mvaddstr(9,29,con);
     mvaddstr(11,15,pass);
     mvaddstr(13,25,play);
     mvaddstr(15,27,giveup);

     tty_mode(0);
     set_nodelay_mode();
   while(1)
   {
        
    //top edge
     for(i=row1,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }

      
     //buttom edge
     for(i=row2,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }
      //left edge
      for(i=row1,j=col1;i<=row2;i++)
     {
           move(i,j);
           if(i%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }

      //right edge
      for(i=row1,j=col2;i<=row2;i++)
     {
           move(i,j);
           if(i%2==0)
              addstr(star);
           else
              addstr(BLANK);
      }
      move( LINES-1,COLS-1 );
      refresh();

      usleep(100000);
   
      //top edge
     for(i=row1,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }

     //buttom edge
     for(i=row2,j=col1;j<=col2;j++)
     {
           move(i,j);
           if(j%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }
      //left edge
      for(i=row1,j=col1;i<=row2;i++)
     {
           move(i,j);
           if(i%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }

      //right edge
      for(i=row1,j=col2;i<=row2;i++)
     {
           move(i,j);
           if(i%2==1)
              addstr(star);
           else
              addstr(BLANK);
      }
      move( LINES-1,COLS-1 );
      refresh();
      usleep(100000);
       od = getch();
         if( od=='r'||od=='R' )
         {   Game_flag=1;Score=0;crash=0;count=0;text=0;set_ticker(0);
             tty_mode(1);set_up(0);break;}
         else if ( od=='q'||od=='Q'){
                 tty_mode(1);
                 wrap_up();
                 
                 //return false;
              }
         
  }
  attroff(COLOR_PAIR(24)|A_BOLD);

           
            break;
            
    }


}

/////////////////////////////////////////////////////////////////
void solve_restart()
{
    crash=0;
    Score=(Game_flag-1)*4;
    count=0;
    text=0;
    set_up(Game_flag);
}

////////////////////////////////////////////////////////////////////////
int main()
{

    initscr();
    clear();
    noecho();
    crmode();
    signal( SIGQUIT,solve_restart);
    keypad( stdscr,true );   
    set_up(0);
    signal(SIGIO,on_input);
    enable_kbd_signals();
    //show_score();
    while(!done) pause();
    wrap_up();
    getch();
    endwin();
}


void set_nodelay_mode()
/*
 * purpose: put file descriptor 0 into no-delay mode
 *  method: use fcntl to set bits
 *   notes: tcsetattr() will do something similar, but it is complicated
 */
{
	int	termflags;

	termflags = fcntl(0, F_GETFL);		/* read curr. settings	*/
	termflags |= O_NDELAY;			/* flip on nodelay bit	*/
	fcntl(0, F_SETFL, termflags);		/* and install 'em	*/
}

/* how == 0 => save current mode,  how == 1 => restore mode */
/* this version handles termios and fcntl flags             */

void tty_mode(int how)
{
	static struct termios original_mode;
	static int            original_flags;
	static int            stored = 0;

	if ( how == 0 ){
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}
	else if ( stored ) {
		tcsetattr(0, TCSANOW, &original_mode); 
		fcntl( 0, F_SETFL, original_flags);	
	}
}
