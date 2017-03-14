
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <curses.h>
#include <term.h>
#include <string.h>
#include <sys/stat.h>
#define LINELEN 514
static unsigned long filesize = 0;
static unsigned long input_filesize = 0; 
static FILE *out_stream = (FILE *) 0;
static int filesum = 0; 

void clear_more(int, int, FILE *);   
int cols_more(FILE *fp);  
int lines_more(FILE *);  
int char_to_terminal(int ); 
void do_more(FILE *, char *filename);
int see_more(FILE *,int, int, char *filename);
unsigned long get_filesize(FILE *);
int main(int ac, char *av[])
{
    FILE *fp;int i,j;
    filesum = ac - 1;
    
    if(ac==1)
    {
        printf("Usage: more filename...\n");
        return;
    }
    else
    {
        while(--ac)
          if( (fp = fopen(* ++av, "r")) != NULL)
          {
              
              filesize = input_filesize = 0; 
              filesize = get_filesize(fp);
              do_more(fp, *av);
              fclose(fp);
          }
          else
            exit(1);
    }
    return 0;
}

void do_more(FILE *fp, char *filename)
{
    char line[LINELEN];
    int reply;
    int number_line = 0;
    FILE *fp_tty_in, *fp_tty_out;
    fp_tty_in = fopen("/dev/tty", "r");
    fp_tty_out = fopen("/dev/tty", "w");
    struct termios initial_settings, new_settings;
    tcgetattr(fileno(fp_tty_in), &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO; 
    
    new_settings.c_cc[VMIN] = 1; 
    new_settings.c_cc[VTIME] = 0;
    if(tcsetattr(fileno(fp_tty_in), TCSANOW, &new_settings) != 0) { 
        fprintf(stderr, "could not set attributes\n");
    }
    int Pagelen = lines_more(fp_tty_in) - 1; 
    int PageCol = cols_more(fp_tty_in);  
    int add_line;
    if(filesum > 1) 
    {
        fprintf(fp_tty_out, "-------> %s <-------\n",filename);
        number_line = 1;
    }
    while(fgets(line, LINELEN, fp) != NULL)
    {
        if(number_line >= Pagelen) 
        {

            reply = see_more(fp_tty_in,Pagelen, add_line, filename);
            int prePage = Pagelen;
            Pagelen = lines_more(fp_tty_in) - 1;  
            PageCol = cols_more(fp_tty_in);   
            if(prePage < Pagelen)
                clear_more(Pagelen-1, 0, fp_tty_out);   
            else
                clear_more(Pagelen, 0, fp_tty_out);
            if(reply == 0) 
              break;
            if(number_line != Pagelen && reply == 1)  
                number_line = Pagelen -1;
            else
                number_line -= reply;

        }
        if( fputs(line, fp_tty_out) == EOF)
        {
            tcsetattr(fileno(fp_tty_in), TCSANOW, &initial_settings); 
            exit(1);
        }
        int line_len = strlen(line);
        input_filesize += (unsigned long)line_len;
        add_line = (line_len + PageCol - 1)/PageCol;
        number_line += add_line;
    }
    tcsetattr(fileno(fp_tty_in), TCSANOW, &initial_settings);
}
void clear_more(int posx,int posy,FILE *fp)
{
    char *el;
    char *cursor;
    out_stream = fp;
    cursor = tigetstr("cup"); 
    el = tigetstr("el");
    tputs(tparm(cursor, posx, posy), 1, char_to_terminal); 
    tputs(el, 1,  char_to_terminal);

}
int see_more(FILE *cmd,int Pagelen, int add_line, char *filename)
{
    int c;
    standout();
    if(filesize > 0 ) 
        printf("\033[7m%s more %ld\% \033[m",filename,
                input_filesize*100/filesize);	
        
    else
        printf("\033[7m%s more %ld\% \033[m",filename,
                input_filesize*100/filesize);	
        
        
    do {
        c = fgetc(cmd);
        if(c == 'q')
          return 0;
        if(c == ' ')
        {
            return Pagelen;
        }
        if(c == '\n' || c == '\r') 
          return add_line;
    }while(1);
    standend();
    return 0;
}
int char_to_terminal(int char_to_write)
{
    if(out_stream) putc(char_to_write,out_stream);
    return 0;
}
int lines_more(FILE *fp)
{
    int nrows;
    setupterm(NULL, fileno(fp), (int *)0);
    nrows = tigetnum("lines");
    return nrows;
}
int cols_more(FILE *fp)
{
    int ncols;
    setupterm(NULL, fileno(fp), (int *)0);
    ncols = tigetnum("cols");
    return ncols;
}
unsigned long get_filesize(FILE *fp)
{
    struct stat buf;
    if(fstat(fileno(fp), &buf) < 0)
        return (unsigned long) 0;
    return (unsigned long) buf.st_size;
}
