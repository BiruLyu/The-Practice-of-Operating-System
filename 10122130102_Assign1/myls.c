#include	<stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>
#include    <unistd.h>
#include    <sys/ioctl.h> 
#include	<pwd.h>
#include	<grp.h> 

void ls_print(char [][1000],int n);
void do_ls(char []);
void do_ls_l(char[]);
void total_print(char []);
void do_ls_R(char []);
void dostat(char*,char *);
void show_file_info( char *, struct stat *);
void mode_to_letters( int , char [] );
char *uid_to_name( uid_t );
char *gid_to_name( gid_t );
void sort(char array[][1000],int n ){
	char temp[1000];
    int i,j,k;
    for(i=1;i<n-1;i++)

    {
        k=i;
        for(j=i+1;j<n;j++)
            if(strcmp(array[k],array[j])>0)
            k=j;
        if(k!=i)
        {
            strcpy(temp,array[i]);   //字符串交换顺序
            strcpy(array[i],array[k]);
            strcpy(array[k],temp);
        }
    }
}

char sort_name[1000][1000];
int sign=0;//是否支持-a的全局参数。


int main(int ac, char *av[])
{
	if ( ac == 1 )
		do_ls( "." );
	else if ( ac == 2)
	    {
	    	if(strcmp(* ++av, "-l")==0)
	    		do_ls_l(".");
	    	else if (strcmp(* av, "-a")==0)
	    	{
	    		sign= 1;
	    		do_ls(".");
	    	}
	    	else if (strcmp(* av, "-R")==0)
	    	{
	    		do_ls(".");
	    		do_ls_R(".");
	    	}
	    	else
	    		do_ls(* av);

	    }
	else if ( ac == 3)
	{
		if(strcmp(* ++av, "-a")==0)
		{
			if(strcmp(* ++av, "-l")==0)
			{
	    		sign= 1;
	    		do_ls_l(".");
	    	}
	    }
	    else if(strcmp(* av, "-l")==0)
	    {
	    	do_ls_l( * ++av );
	    }
	    else if (strcmp(* av, "-R")==0)
	    {
            do_ls(* ++av);
	    	do_ls_R( * av );
	    }

	}

	else		

		while ( --ac ){
			
			{
				printf("%s:\n", *++av );
			    do_ls( *av );
			}
		}
}

void do_ls( char dirname[] )
/*
 *	list files in directory called dirname
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
    int i=0;


	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls1: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
			            if(sign==0)
			            {
			            	if(direntp->d_name[0]!='.')
			            	{
                               strcpy(sort_name[i], direntp->d_name);
                               i++;
                            }

                        }
                        else
                        {
                            strcpy(sort_name[i], direntp->d_name);
                            i++;
                        }
                        }
                ls_print(sort_name,i);
                
    }
    closedir(dir_ptr);
}

void ls_print(char name[][1000],int n)
{
	int max_name=0,i;
	struct winsize size;
	int cols;
	int col,row,col_len;

    
	
	for(i=0;i<n;i++)
	   max_name=(strlen(name[i])>max_name)?strlen(name[i]):max_name;
    
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
    cols=size.ws_col;
    cols/=(max_name+1);

    col_len = n / cols ;
	if ( n % cols )		/* for uneven cols, put the */
		col_len++;		/* extra on the left	    */

	sort(name,n);

	for( row = 0 ; row < col_len ; row++ )
		for( col = 0 ; col < cols ; col++ ){
			i = row + col*col_len ;
			if ( i < n ){
				if ( i + col_len < n )
					printf("%-*s",max_name+1,name[i]);
				else
					printf("%s\n",name[i]);
			}
		}
	printf("\n");

}
    
	             	 	

void do_ls_R(char dirname[])
{
 
    struct stat info;
    DIR		*dir_ptr;		/* the directory */
    DIR     *next_dir_ptr;
    struct dirent	*direntp;
    int i;
    char next_dirname[1000];
    char *fullpath;

    fullpath = (char *)malloc(strlen(dirname) + 1 + MAXNAMLEN + 1);
    dir_ptr = opendir( dirname );
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
		{
               	if(direntp->d_name[0] !='.'){
                        sprintf(fullpath,"%s/%s",dirname,direntp->d_name);
               		    if((next_dir_ptr = opendir(fullpath)) !=NULL)
               		    {
               		    	printf("%s:\n",fullpath);
							do_ls(fullpath);/*using ls(0) to output*/
							do_ls_R(fullpath);/*using ls(3) to recursion*/
               		    }
	            }
	        }
	    closedir(dir_ptr);
    }

}


void do_ls_l( char dirname[] )
/*
 *	list files in directory called dirname
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
    char    *fullpath;

	
	fullpath = (char *)malloc(strlen(dirname) + 1 + MAXNAMLEN + 1);
	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls_l: cannot open %s\n", dirname);
	else
	{
		total_print(dirname);
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
			if(sign==0)
			{
			   if(direntp->d_name[0]!='.')
			   {
			   	  sprintf(fullpath,"%s/%s",dirname,direntp->d_name);
			      dostat(fullpath, direntp->d_name );
			   }
			}
			else
			{
				 sprintf(fullpath,"%s/%s",dirname,direntp->d_name);
				 dostat( fullpath,direntp->d_name );
			}

		closedir(dir_ptr);
	}
}

void dostat( char *fullpath, char *filename )
{
	struct stat info;

	if ( stat(fullpath, &info) == -1 )		/* cannot stat	 */
		perror(filename);			/* say why	 */
	else					/* else show info	 */
		show_file_info(filename, &info);
}




void show_file_info( char *filename, struct stat *info_p )
/*
 * display the info about 'filename'.  The info is stored in struct at *info_p
 */
{
	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    char    modestr[11];

	mode_to_letters( info_p->st_mode, modestr );
    

	printf( "%s"    , modestr );
	printf( "%4d "  , (int) info_p->st_nlink);	
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime(&info_p->st_mtime));
	printf( "%s\n"  , filename );

}

/*
 * utility functions
 */

/*
 * This function takes a mode value and a char array
 * and puts into the char array the file type and the 
 * nine letters that correspond to the bits in mode.
 * NOTE: It does not code setuid, setgid, and sticky
 * codes
 */
void mode_to_letters( int mode, char str[] )
{
    strcpy( str, "----------" );           /* default=no perms */

    if ( S_ISDIR(mode) )  str[0] = 'd';    /* directory?       */
    if ( S_ISCHR(mode) )  str[0] = 'c';    /* char devices     */
    if ( S_ISBLK(mode) )  str[0] = 'b';    /* block device     */

    if ( mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';
}



char *uid_to_name( uid_t uid )
/* 
 *	returns pointer to username associated with uid, uses getpw()
 */	
{
	struct	passwd *getpwuid(), *pw_ptr;
	static  char numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name ;
}



char *gid_to_name( gid_t gid )
/*
 *	returns pointer to group number gid. used getgrgid(3)
 */
{
	struct group *getgrgid(), *grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

void total_print(char dirname[])
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
	struct stat info;
    struct stat *info_p;
    char    modestr[11];
    int count=0;

	dir_ptr = opendir( dirname );
    while ( ( direntp = readdir( dir_ptr ) ) != NULL )
	{ 
		stat(direntp->d_name, &info);
	    if(sign==0)
	       {
	           if(direntp->d_name[0]!='.')
	             {
		            mode_to_letters( info.st_mode, modestr );
		         	if((long)info.st_size<4096)
		               count+=4;
		            else
		            {
                        if((long)info.st_size%4096==0)
		            	   count+=((long)info.st_size/4096)*4;
		            	else
		            		count+=((long)info.st_size/4096)*4+4;
		            }
		           }
		       }
		 else
		       {
		         	mode_to_letters( info.st_mode, modestr );
		         	if((long)info.st_size<4096)
		               count+=4;
		            else
		            {
                        if((long)info.st_size%4096==0)
		            	   count+=((long)info.st_size/4096)*4;
		            	else
		            		count+=((long)info.st_size/4096)*4+4;
		            }
		       }
	    }
			
    closedir(dir_ptr);
	printf("total:%d\n",count);

}
