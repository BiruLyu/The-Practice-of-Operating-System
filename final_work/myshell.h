#define	YES	1
#define	NO	0
#define	DFL_PROMPT	"> "
#define	MAXVARS	200	
#define	is_delim(x) ((x)==' '||(x)=='\t')


char	*next_cmd();
char	**splitline(char *);
void	freelist(char **);
void	*emalloc(size_t);
void	*erealloc(void *, size_t);
int	    execute(char **);
void	fatal(char *, char *, int );
int	    process();
int	    VLenviron2table(char **);
int	    VLexport(char *);
char	*VLlookup(char *);
void	VLlist();
int	    VLstore( char *, char * );
char	**VLtable2environ();
int	    VLenviron2table(char **);
int     is_control_command(char *);
int     do_control_command(char **);
int     ok_to_execute();
int     builtin_command(char **, int *);
int	    syn_err(char *);
int     assign(char *);
int     okname(char *);
int     numeric(char *);

enum states   { NEUTRAL, WANT_THEN, THEN_BLOCK, ELSE_BLOCK };
enum results  { SUCCESS, FAIL };

static int if_state  = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;