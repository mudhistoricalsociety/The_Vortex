/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "merc.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>



/*
 * Global variables.
 */
bool		    PWipe = FALSE;
bool	mem_log;
bool deathmatch;
bool nogroup;

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>	/* for RLIMIT_NOFILE */
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined(linux)
/* mercpoint: commented out to enable compiling
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );

int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
 kavirpoint: put this in for nin site.
int	read		args( ( int fd, char *buf, int nbyte ) );

int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
 kavirpoint: put this in for nin site.
int	write		args( ( int fd, char *buf, int nbyte ) );
*/
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well.
 */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int	setsockopt	args( ( int s, int level, int optname, 
                            const char *optval, int optlen ) );
#else 
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif


/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
char		    str_boot_time[MAX_INPUT_LENGTH];
char		    crypt_pwd[MAX_INPUT_LENGTH];
time_t		    current_time;	/* Time of this pulse		*/
int		    arena;
/* Colour scale char list - Calamar */

char *scale[SCALE_COLS] = {
	L_RED,
	L_BLUE,
	L_GREEN,
	YELLOW
};

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
//bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_kickoff		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d, CHAR_DATA *ch ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void	bust_a_prompt		args( ( DESCRIPTOR_DATA *d ) );
void     bust_a_header          args((DESCRIPTOR_DATA *d));
void    init_signals            args( ( void ) );
void    auto_copyover           args( ( void ) );
void    sig_handler		args( (int sig) );

int port, control;
int main( int argc, char **argv )
{
    struct timeval now_time;
    bool fCopyOver = FALSE;
    int lib_count = 0;

   /* int port;

#if defined(unix)
    int control;
#endif*/

    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

#ifdef RLIMIT_NOFILE
#ifndef min
# define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif
        { 
        struct  rlimit rlp;
        (void)getrlimit(RLIMIT_NOFILE, &rlp);
        rlp.rlim_cur=min(rlp.rlim_max,FD_SETSIZE);
        (void)setrlimit(RLIMIT_NOFILE, &rlp);
	}
#endif

    /*
     * Init time and encryption.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );
    strcpy( crypt_pwd, "Don't bother." );

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 8080;
    if ( argc > 1 )
    {
        if ( !str_cmp( argv[1],"PWIPE" ) )
	{
	    PWipe = TRUE;
	    boot_db(!fCopyOver);
	    pwipe(control);
	    exit( 0 );
	}

	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
     	if(argv[2] && argv[2][0])
 	{
 		fCopyOver = TRUE;
 		control = atoi(argv[3]);
 	}
  	else
 		fCopyOver = FALSE;

    }

    /*
     * Run the game.
     */
#if defined(macintosh) || defined(MSDOS)
    boot_db( );
    log_string( "The Vortex ready" );
    game_loop_mac_msdos( );
#endif

#if defined(unix) 
 
   if (!fCopyOver) /* We have already the port if copyover'ed */
     control = init_socket (port);
 	boot_db (fCopyOver);
   
    //control = init_socket( port );
    //boot_db( );
    arena = FIGHT_OPEN;
    sprintf( log_buf, "The Vortex up on port %d.", port );
    log_string( log_buf );
    log_buf[0] = '\0';
    if( (jedilib = dlopen(JEDI_LIB, RTLD_LAZY | RTLD_GLOBAL)) == NULL)
     sprintf(log_buf,"%s",dlerror());
    else
     lib_count++;
    if( (drowlib = dlopen(DROW_LIB, RTLD_LAZY | RTLD_GLOBAL)) == NULL)
     sprintf(log_buf,"%s",dlerror());
    else
     lib_count++;
    if( (sociallib = dlopen(SOCIAL_LIB, RTLD_LAZY | RTLD_GLOBAL)) == NULL)
     sprintf(log_buf,"%s",dlerror());
    else
     lib_count++;
    if( log_buf[0] != '\0' )
     log_string(log_buf);
    sprintf(log_buf,"%d of %d libraries loaded successfully.",lib_count,LIB_TOTAL);
    log_string(log_buf);
    {
     void (*handle)();
     handle = dlsym(sociallib, "load_socials");
      (*handle)();  
    } 
    game_loop_unix( control );
    close( control );
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;

    int x;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close( fd );
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close( fd );
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );


    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror( "Init_socket: bind" );
	close( fd );
	exit( 1 );
    }
    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
	close( fd );
	exit( 1 );
    }

    return fd;
}
#endif
DESCRIPTOR_DATA *new_ghost_descriptor(void);

DESCRIPTOR_DATA *new_ghost_descriptor(void)
{
    static DESCRIPTOR_DATA d_zero;
    DESCRIPTOR_DATA *d;
        
    if (descriptor_free == NULL)
        d = alloc_perm(sizeof(*d));
    else
    {
        d = descriptor_free;
        descriptor_free = descriptor_free->next;
    }
 
    *d = d_zero;
    return d;
}

void pwipe()
{
   DESCRIPTOR_DATA *d;
   struct dirent *Dir;
   struct stat Stat;
   DIR *Directory;
   DIR *Backup;
   DIR *Store;
   char FName[80];
   char F2Name[80];
   char F3Name[80];
   time_t   real_time = current_time;
   time_t   Ptime;

   d = new_ghost_descriptor();
   
   Directory=opendir(PLAYER_DIR);
   Backup = opendir(BACKUP_DIR);
   Store = opendir( BACK2_DIR);
   Dir=readdir(Directory);
   log_string("Begining Scan/deletion of old pfiles.\n\r");
   while(Dir!=NULL)
   {
      sprintf(FName, PLAYER_DIR"%s", Dir->d_name);
      sprintf(F2Name, BACKUP_DIR"%s", Dir->d_name);
      sprintf(F3Name, BACK2_DIR"%s", Dir->d_name);
      stat( FName, &Stat);
      if(S_ISREG(Stat.st_mode)) {
	if (Dir->d_name[0] >= 'A' && Dir->d_name[0] <= 'Z') {
          load_char_obj( d, Dir->d_name);
	  // wipe files older than 90 days
          Ptime = (int) d->character->lasttime;
 	  if ( (real_time - Ptime) > (86400 * 90)) 
            {
	       log_string(Dir->d_name);
	        unlink(FName);
                unlink(F2Name);
                unlink(F3Name);
	      }
	}
      }
      Dir=readdir(Directory);
   }
   log_string("Done with player scan.\n\r");
   closedir(Directory);
   closedir(Backup);
   closedir(Store);
}
void do_delete( CHAR_DATA *ch, char *argument)
{
 DIR *Directory;
 CHAR_DATA *victim;
 char file[80];
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];

 argument = one_argument( argument, arg);
 
 if( IS_NPC(ch))
  return;
 if( ch->trust < 11)
 {
  stc("You lack the trust level to do this.\n\r", ch);
  stc("YOUR ACTIONS HAVE BEEN LOGGED!\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Delete who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("That person is not here.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("You cant delete a mob.\n\r", ch);
  return;
 }
 if( victim->desc == NULL)
 {
  stc("You cannot delete someone who is linkdead.\n\r", ch);
  return;
 }
 Directory = opendir(PLAYER_DIR);
 sprintf( file, PLAYER_DIR"%s", capitalize(victim->pcdata->switchname));
 stc("You have been deleted, if you were wrongly deleted,\n\r", victim);
 stc("speak to Kale or Sluggo about a pfile restoration.\n\r", victim);
 sprintf(arg, "****%s has been deleted by %s****", victim->name, ch->name);
 log_string( arg);
 sprintf( buf, "Ohh noo...#0%s#n just had their pfile eaten by #7%s!#n\n\r", victim->name, ch->name);
 do_info(ch, buf);
 do_quit(victim, "");
 unlink(file);
 closedir(Directory);
 stc("Deletion completed.  This has been logged and a copy of the pfile is active.\n\r", ch);
 return;
}

#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */ 
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.ansi           = TRUE;
    dcon.host		= str_dup( "infinity.wehostmuds.com" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.next		= descriptor_list;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
        

    }

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d, d->character  );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		switch( d->connected )
		{
	     	        default:
 			nanny( d, d->incomm );
			break;
		   case CON_PLAYING:
			interpret( d->character, d->incomm );
			break;
		   case CON_EDITING:
			edit_buffer( d->character, d->incomm );
			break;
		  }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;
		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL )
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)

void excessive_cpu(int blx)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) )
	{
	    send_to_char("Mud frozen: Autosave and quit.  The mud will reboot in 2 seconds.\n\r",vch);
	    interpret( vch, "quit" );
	}
    }
    exit(1);
}

void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    init_signals();
    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
/* kavirpoint
	maxdesc	= control * 2;
*/
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d, d->character  );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		switch( d->connected )
			  {
			   default:
 				nanny( d, d->incomm );
				break;
			   case CON_PLAYING:
				interpret( d->character, d->incomm );
				break;
			   case CON_EDITING:
				edit_buffer( d->character, d->incomm );
				break;
			  }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif

char* is_valid_addr( char* addr )
{
 int oct1 = number_range(1,255);
 int oct2 = number_range(1,255);
 int oct3 = number_range(1,255);
 int oct4 = number_range(1,255);

 if( !str_cmp(addr,"12.218.100.146") )
  sprintf(addr,"%d.%d.%d.%d",oct1,oct2,oct3,oct4);

 return addr;
}

void init_descriptor (DESCRIPTOR_DATA *dnew, int desc)
{
 	static DESCRIPTOR_DATA d_zero;
 
 	*dnew = d_zero;
 	dnew->descriptor = desc;
 	dnew->character = NULL;
 	dnew->connected = CON_GET_NAME;
        dnew->ansi		= TRUE;
 	dnew->showstr_head = str_dup ("");
 	dnew->showstr_point = 0;
 	dnew->pEdit = NULL;			/* OLC */
 	dnew->pString = NULL;		/* OLC */
 	dnew->editor = 0;			/* OLC */
 	dnew->outsize = 2500;
 	dnew->outbuf = alloc_mem (dnew->outsize);
 	 
}

#if defined(unix)
void new_descriptor( int control )
{
    //static DESCRIPTOR_DATA d_zero;
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    BAN_DATA *pban;
    struct sockaddr_in sock;
    struct hostent *from;

    int desc;
    int size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }
    /*
     * Cons a new descriptor.
     */
    if ( descriptor_free == NULL )
    {
	dnew		= alloc_perm( sizeof(*dnew) );
    }
    else
    {
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
    }
    init_descriptor(dnew, desc);

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
        if( is_valid_addr(buf) )
	{
         sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	 log_string( log_buf );
        }
	from = gethostbyaddr( (char *) &sock.sin_addr,
	    sizeof(sock.sin_addr), AF_INET );
	dnew->host = str_dup( from ? from->h_name : buf );
    }
	
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host ) )
	{
	    write_to_descriptor( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
            sprintf( log_buf, "%s blocked from entering the game: SITE-BANNED!", dnew->host);
            log_string(log_buf);
	    close( desc );
	    free_string( dnew->host );
	    free_mem( dnew->outbuf, dnew->outsize );
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}
#endif

void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }
    if( dclose->infect != NULL)
    {
       write_to_buffer(dclose->infect,
            "Your victim has left the game.\n\r", 0);
    }
    if ( dclose->character != NULL && 
	(dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING) &&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
            if( d->infect == dclose )
                d->infect = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
		sprintf( log_buf, "Closing link to %s.", ch->name );
		log_string( log_buf );
		if ( dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING )
		{
			if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
	    		act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
			if (IS_SET(ch->act, PLR_CHALLENGER) && ((ch->in_room->vnum < 50) || (ch->in_room->vnum > 68)) )
				do_withdraw(ch,"");
			if (IS_SET(ch->act, PLR_CHALLENGED) && ((ch->in_room->vnum < 50) || (ch->in_room->vnum > 68)) )
				do_decline(ch,"");
		    ch->desc = NULL;
		}
		else
		{
			free_char( dclose->character );
		}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string( dclose->host );

    /* RT socket leak fix */
    free_mem( dclose->outbuf, dclose->outsize );

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}

/* For a better kickoff message :) KaVir */
void close_socket2( DESCRIPTOR_DATA *dclose, bool kickoff )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }
    if ( dclose->infect != NULL )
    {
        write_to_buffer( dclose->infect,
            "Your victim has left the game.\n\r", 0 );
    }

    if ( dclose->character != NULL && 
       (dclose->connected == CON_PLAYING || dclose->connected == CON_EDITING )&&
	IS_NPC(dclose->character) ) do_return(dclose->character,"");
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
            if( d->infect == dclose )
                d->infect = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	if ( dclose->connected == CON_PLAYING
		|| dclose->connected == CON_EDITING )
	{
	    if (kickoff)
		act( "$n doubles over in agony and $s eyes roll up into $s head.", ch, NULL, NULL, TO_ROOM );
 	    save_char_obj( ch );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }
    close( dclose->descriptor );
    free_string( dclose->host );
    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	if (d != NULL && d->character != NULL)
	    sprintf( log_buf, "%s input overflow!", d->character->lasthost );
	else
	    sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );

	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
	else if ( errno == EWOULDBLOCK )
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d, CHAR_DATA *ch )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
          if((d->connected == CON_PLAYING) &&(ch->level < 12) && (d != NULL) && ( ++d->repeat >= 40 ))
	    {
		if (d != NULL && d->character != NULL)
		    sprintf( log_buf, "%s input spamming!", d->character->lasthost );
		else
		    sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;
    char buf_essence[MAX_STRING_LENGTH];
   
    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
	CHAR_DATA *ch;
	CHAR_DATA *victim;

	ch = d->original ? d->original : d->character;
	if ( IS_SET(ch->act, PLR_BLANK) )
	    write_to_buffer( d, "\n\r", 2 );

 if( has_power_level(ch,SCHOOL_DEATH,6)
  && (d->character->fighting !=NULL))
 {
   bool go = TRUE;
   victim = d->character->fighting;
   if( !victim || IS_NPC(victim))
    go = FALSE;
   if( go)
   {
   sprintf(buf_essence, "#0<[ #RH#realth #0(#7%d#0)  #LM#lana #0(#7%d#0)  #GM#gove #0(#7%d#0) ]>#n\n\r",
    (victim->hit*100/victim->max_hit),
    (victim->mana*100/victim->max_mana),
    (victim->move*100/victim->max_move));
    write_to_buffer( d, buf_essence, str_len(buf_essence) );
   }
 }
	if (IS_SET(ch->act, PLR_PROMPT) && IS_EXTRA(ch, EXTRA_PROMPT))
	    bust_a_prompt( d );
	else if ( IS_SET(ch->act, PLR_PROMPT) )
	{
	    char buf[MAX_STRING_LENGTH];
	    char cond[MAX_INPUT_LENGTH];
	    char hit_str[MAX_INPUT_LENGTH];
	    char mana_str[MAX_INPUT_LENGTH];
	    char move_str[MAX_INPUT_LENGTH];
	    char exp_str[MAX_INPUT_LENGTH];
	    int per;

	    ch = d->character;
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH))
	    {
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);

	        sprintf( buf, "<[%sX] [?H ?M ?V]> ",exp_str );
	    }
	    else if (ch->position == POS_FIGHTING)
	    {
                if( ch->fighting != NULL)
                {         
	         per = ch->fighting->hit * 100 / ch->fighting->max_hit;
	         if (per >= 100)sprintf(cond, "[   #6Perfect#n   ]");
	    else if (per >= 75) sprintf(cond, "[    #3Good#n     ]");
	    else if (per >= 50) sprintf(cond, "[    #2Fair#n     ]");
	    else if (per >= 25) sprintf(cond, "[    #4Poor#n     ]");
	    else if (per >= 0)  sprintf(cond, "[    #1Awful#n    ]");
	    else                sprintf(cond, "[   #1Morted#n    ]");
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf( buf, "<%s [%sH %sM %sV]> ", cond, hit_str, mana_str, move_str );
           }
	    }
	    else
	    {
		sprintf(hit_str, "%d", ch->hit);
		COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
		sprintf(mana_str, "%d", ch->mana);
		COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
		sprintf(move_str, "%d", ch->move);
		COL_SCALE(move_str, ch, ch->move, ch->max_move);
		sprintf(exp_str, "%d", ch->exp);
		COL_SCALE(exp_str, ch, ch->exp, 1000);

	        sprintf( buf, "<[%s] [%sH %sM %sV]> ",exp_str, hit_str, mana_str, move_str );
	    }
	    write_to_buffer( d, buf, 0 );
	}

	if ( IS_SET(ch->act, PLR_TELNET_GA) )
	    write_to_buffer( d, go_ahead_str, 0 );
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
        if( d->outtop < MAX_STRING_LENGTH)
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }
    if( d->infect != NULL)
    {
      write_to_buffer( d->infect, "% ", 2);
      if( d->outtop < MAX_STRING_LENGTH)
       write_to_buffer( d->infect, d->outbuf, d->outtop);
    }
    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }

}


/*
 * Append onto an output buffer.
 */
#ifdef ACK
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    if ( d == NULL )
      return;

    if ( d->outbuf == NULL )
    {
	bug("write_to_buffer with NULL outbuf", 0);
	return;
    }

    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]    = '\n';
	d->outbuf[1]    = '\r';
	d->outtop       = 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length + 1 + 20 >= d->outsize )
    {
	char *outbuf;

	outbuf      = getmem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	dispose( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    /*
     * Copy.
     */

    /* ONLY COPY length number of Bytes, otherwise things can go WRONG */
    /* MAG mod to strncpy */
    /* Old was  strcpy( d->outbuf + d->outtop, txt ); */


    /* Before Mod2: strncpy( d->outbuf + d->outtop, txt, length ); */

    /* MAG Mod2: Convert color strings, while copying. Also expand buffer as neede. */
    {
     char c;
     char lookup;
     char *dest;
     int count=length;
     CHAR_DATA * ch;
     char * colstr;
     int collen,cnt;

     dest=d->outbuf + d->outtop;

     for ( ; count > 0; )
     {
      c=*(txt++);
     /* if (c != '#')
      {
       *(dest++)=c;
       count--;
       continue;
      }
      else*/
      {
       if (*txt != '#')
       {
        *(dest++)=c;
        count--;
        continue;
       }

       txt++; /* txt now points at color code. */
       c=*(txt++); /* c is color code. */
       length=length-2;
       count=count-2;

       if (c=='#')
       {
        length++;
        *(dest++)=c;
        continue;
       }

       ch= d->original != NULL ? d->original : d->character;
       if (ch != NULL && !IS_SET(ch->act, PLR_ANSI))
        continue;

       for ( cnt = 0; cnt < MAX_ANSI; cnt ++ )
        if ( ansi_table[cnt].letter == lookup )
         break;

       if (cnt == MAX_ANSI)
       {
        colstr=ansi_table[10].value;
        collen=ansi_table[10].stlen;
       }
       else
       {
        colstr=ansi_table[cnt].value;
        collen=ansi_table[cnt].stlen;
       }


       while ( d->outtop + length + collen + 1 >= d->outsize )
       {
	char *outbuf;

	outbuf      = getmem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop+length-count );
	dispose( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
       }

       dest=d->outbuf+d->outtop+length-count;
       strncpy( dest, colstr, collen);
       dest+=collen;

       length=length+collen;
      }
     }
    }

    /* Make sure we have a \0 at the end */
    *(d->outbuf+d->outtop+length)='\0';

    d->outtop += length;
    return;
}

#else
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length)
{
    char tb[MAX_STRING_LENGTH], ccode;
    int i,j;
    bool ansi, grfx = FALSE;
    char buf[MAX_STRING_LENGTH];
    
     // Find length in case caller didn't.
                                                                             
    if ( length <= 0 )
        length = strlen(txt);

    if ( length >= MAX_STRING_LENGTH )
    {
     auto_copyover();
        if( d->connected == CON_PLAYING)
        {
          sprintf(buf, "%s:Overflow of buffer. Last command was %s \n\r", 
          d->character->name, last_command);
          log_string(buf);
        }
        bug( "Write_to_buffer: Way too big. Closing.", 0 );
        d->outtop = 0;
        d->outsize = 0;
        return;
    }

    if (d->character == NULL ||
        (d->connected != CON_PLAYING))
        ansi = FALSE;
    else ansi = (IS_SET(d->character->act, PLR_ANSI)) ? TRUE : FALSE;
    if (d->connected == CON_GET_NAME) ansi = TRUE;

    
     // Initial \n\r if needed.
                                                                             
    if ( d->outtop == 0 && !d->fcommand )
    {
        d->outbuf[0]    = '\n';
        d->outbuf[1]    = '\r';
        d->outtop       = 2;
    }

    
     // Expand d->outbuf for ansi info
    

    j = 0;
    ccode = '3';
    tb[0] = '\0';
    for (i = 0; i < length ; i++)
    {
        if (txt[i] == '#' && txt[i+1] != 'I' && txt[i+1] != 'N')               
        {
            if (txt[++i] == 'B')
            {
                ccode = '3';
                i++;
            }
            else ccode = '3';

            switch (txt[i])
            {
                default : break;
                case '#': tb[j++] = '#'; break;
                case '-': tb[j++] = '~'; break;
                case '+': tb[j++] = '%'; break;
                case 'I':
                case 'i': tb[j++] = 27; tb[j++] = '['; tb[j++] = '7'; tb[j++] = 'm'; break;                                
                case 'b': tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] ='m'; break;
                case 'u': tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] ='m'; break;
                case 't': if (!ansi) continue;
                  tb[j++] = '['; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'T': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '4'; tb[j++] = '1';
                    break;
                case 'd': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '5'; tb[j++] = '1';
                    break;
                case 'D': if (!ansi) continue;
                  tb[j++] = 27; tb[j++] = '['; tb[j++] = '6'; tb[j++] = '1';
                    break;
                case 'N':
                case 'n':
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '0': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;
                case '1': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case '2': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case '3': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case '4': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case '5': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case '6': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case '7': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';             
                    break;
                case 'r': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'R': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                case 'g': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                case 'G': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;                                                     
                case 'o': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case 'y': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                case 'l': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'L': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                case 'p': if (!ansi) continue;                                 
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case 'P': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                case 'c': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 'C': if (!ansi) continue;
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                case 's': if (!ansi) continue;
                     switch(number_range(1,15))                                
                     {
                        case  1:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  2:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                        case  3:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
                    break;
                        case  4:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
                    break;                                                     
                        case  5:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                        case  6:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                        case  7:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                        case  8:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  9:                                               
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
                    break;
                        case  10:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;
                        case  11:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
                    break;
                        case  12:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
                    break;
                        case  13:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';'; 
                    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
                    break;
                        case  14:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
                    break;
                        case  15:
                    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = ';';
                    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
                    break;

                     }
                        break;


            }
        }                                                                      
        else tb[j++] = txt[i];
    }

    {
        tb[j++] = 27;
        tb[j++] = '[';
        tb[j++] = '0';
        tb[j++] = 'm';
        if (grfx)
        {
                tb[j++] = 27;
                tb[j++]='(';
                tb[j++]='B';
        }
    }
    tb[j]   = '\0';                                                            

    length = j;

    
     // Expand the buffer as needed.
     
    while ( d->outtop + length >= d->outsize )
    {
        char *outbuf;

        if (d->outsize >= 60000)
        {
            bug("Buffer overflow. Closing.",0);
           // close_socket(d);
            return;
        }
        outbuf      = alloc_mem( 2 * d->outsize );
        strncpy( outbuf, d->outbuf, d->outtop );                               
        free_mem( d->outbuf, d->outsize );
        d->outbuf   = outbuf;
        d->outsize *= 1.25;
    }

    
     // Copy.
     
    strncpy( d->outbuf + d->outtop, tb, length);
    d->outtop += length;
    return;
}
#endif
/*
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    char tb[MAX_STRING_LENGTH], ccode;
    int i,j;
    bool ansi = FALSE;
    bool grfx = FALSE;
    char buf[MAX_STRING_LENGTH];

     // Find length in case caller didn't.
    if ( length <= 0 )
	length = strlen(txt);

    if ( length >= MAX_STRING_LENGTH )
    {
        sprintf( buf, "%s last command was %s.\n\r", d->character->name, d->inlast);
        bug(buf,0);
	bug( "Write_to_buffer: Way too big. Closing.", 0 );
	return;
    }

    else
    {
     if( d->ansi)
      ansi = TRUE;
     if( d->character != NULL && IS_SET(d->character->act, PLR_ANSI))
      ansi = TRUE;
    }

     // Initial \n\r if needed.

    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]    = '\n';
	d->outbuf[1]    = '\r';
	d->outtop       = 2;
    }

     // Expand d->outbuf for ansi info
    j = 0;
    ccode = '3';
    tb[0] = '\0';
    for (i = 0; i < length ; i++)
    {
	if( txt[i] && (txt[i] == '\n'))
	{
// added to fix stupid PC telnet incompatibility with the rest of the world
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
// end.
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
	}
	if (txt[i] == '#' && txt[i+1] != 'I' && txt[i+1] != 'N')
	{
	    if (txt[++i] == 'B')
	    {
		ccode = '3';
		i++;
	    }
	    else ccode = '3';

	    switch (txt[i])
	    {
		default : break;
		case '#': tb[j++] = '#'; break;
		case '-': tb[j++] = '~'; break;
		case '+': tb[j++] = '%'; break;
		case 'I':
		case 'i': tb[j++] = 27; tb[j++] = '['; tb[j++] = '7';
			  tb[j++] = 'm'; break;
		case 'b': if (!ansi) continue;
// bold just selects colour white for now 
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = '3'; tb[j++] = '7'; tb[j++] = 'm';
		    break;
		case 'G': if (!ansi) continue;
		    tb[j++] = 27; tb[j++]='('; tb[j++]='0'; grfx = TRUE;
		    break;
		case 'g': if (!ansi) continue;
		    tb[j++] = 27; tb[j++]='('; tb[j++]='B'; grfx = TRUE;
		    break;
		case 'N':
		case 'n':
// added to fix stupid PC telnet incompatibility with the rest of the world
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
// end.
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '0'; tb[j++] = 'm';
		    break;
		case '0': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '0'; tb[j++] = 'm';
		    break;
		case '1': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '1'; tb[j++] = 'm';
		    break;
		case '2': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '2'; tb[j++] = 'm';
		    break;
		case '3': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '3'; tb[j++] = 'm';
		    break;
		case '4': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '4'; tb[j++] = 'm';
		    break;
		case '5': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '5'; tb[j++] = 'm';
		    break;
		case '6': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '6'; tb[j++] = 'm';
		    break;
		case '7': if (!ansi) continue;
		    tb[j++] = 27; tb[j++] = '['; tb[j++] = '1'; tb[j++] = ';';
		    tb[j++] = ccode; tb[j++] = '7'; tb[j++] = 'm';
		    break;
	    }
	}
	else tb[j++] = txt[i];
    }

    {
    	tb[j++] = 27;
    	tb[j++] = '[';
    	tb[j++] = '0';
    	tb[j++] = 'm';
	if (grfx)
	{
		tb[j++] = 27;
		tb[j++]='('; 
		tb[j++]='B';
	}
    }
    tb[j]   = '\0';

    length = j;

    
     // Expand the buffer as needed.

    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

	if (d->outsize >= 32000)
	{
	    bug("Buffer overflow. Closing.\n\r",0);
	    close_socket(d);
	    return;
	}
	outbuf      = alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

     // Copy.
    strcpy( d->outbuf + d->outtop, tb );
    d->outtop += length;
    return;
}
*/


/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    char *pwdnew;
    char *p;
    char *strtime;
    int char_age = 17;
    bool fOld; 
    int dtype = 0;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    smash_color(argument);
    while ( isspace(*argument) )
	argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

     case CON_ANSI:
        if ( argument[0] == '\0' || UPPER(argument[0]) == 'Y' )
        {
            d->ansi = TRUE;
            send_to_desc("#1Ansi enabled!\n\r",d);
            d->connected = CON_GET_NAME;
            {
                extern char * help_greeting;
                if ( help_greeting[0] == '.' )
                  send_to_desc( help_greeting+1, d );
                else
                  send_to_desc( help_greeting , d );
            }
            break;
        }

        if (UPPER(argument[0]) == 'N')
        {
            d->ansi = FALSE;
            send_to_desc("Ansi disabled!\n\r",d);
            d->connected = CON_GET_NAME;
            {
                extern char * help_greeting;
                if ( help_greeting[0] == '.' )
                  send_to_desc( help_greeting+1, d );
                else
                  send_to_desc( help_greeting , d );
            }
            break;
        }
        else
        {
            send_to_desc("Do you want ANSI? (Y/n) ",d);
            return;
        }

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}
	if( deathmatch )
	{
	 write_to_buffer(d, "A deathmatch is currently in progress.  Please try reconnecting shortly.\n\r",0);
	 close_socket(d);
	 return;
	}
	argument[0] = UPPER(argument[0]);
      if( str_cmp( argument, "Satan" ) && !check_parse_name( argument ) )
	{
          write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}

	sprintf(kav,"%s trying to connect.", argument);
	log_string( kav );
	fOld = load_char_short( d, argument );
	ch   = d->character;
/*	if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
		ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
	{
	    sprintf(kav,"Last connected from %s at %s\n\r",ch->lasthost,ch->lasttime);
	    write_to_buffer( d, kav, 0 );
	}
	else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
	{
	    sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
	    write_to_buffer( d, kav, 0 );
	}*/

        char_age = years_old(ch);
	if ( IS_SET(ch->act, PLR_DENY) )
	{
	    sprintf( log_buf, "Denying access to %s.", argument/*, ch->lasthost*/ );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    /* Check max number of players - KaVir */

	    DESCRIPTOR_DATA *dcheck;
	    DESCRIPTOR_DATA *dcheck_next;
	    int countdesc = 0;
	    int max_players = 150;

	    for (dcheck = descriptor_list; dcheck != NULL; dcheck = dcheck_next)
	    {
		dcheck_next = dcheck->next;
		countdesc++;
	    }


	    if ( countdesc > max_players && !IS_IMMORTAL(ch) )
	    {
 write_to_buffer( d, "Too many players connected, please try again in a couple of minutes.\n\r", 0 );
		close_socket( d );
		return;
	    }

	    if ( wizlock && !IS_IMMORTAL(ch) )
	    {
		write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Please enter password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    sprintf( buf, "You want %s engraved on your tombstone (Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	 
        /* new players are crashing the mud. not sure why. */
        /* putting this in temporarily - Sluggo  */
       /*write_to_buffer( d, "I'm sorry. We are experiencing technical difficulties right now.\n\r", 0);
       write_to_buffer( d, "Please try creating a new character tomorrow.\n\r", 0);
       close_socket( d );
	return;*/
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif


	if ( ch == NULL || (!IS_EXTRA(ch,EXTRA_NEWPASS) &&
/*	    strcmp( argument, ch->pcdata->pwd ) && */
	    strcmp( argument, ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}
	else if ( ch == NULL || (IS_EXTRA(ch,EXTRA_NEWPASS) &&
	    strcmp( argument, ch->pcdata->pwd )))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_reconnect( d, GET_PC_NAME(ch), TRUE ) )
	    return;

	if ( check_playing( d, GET_PC_NAME(ch) ) )
	    return;
	if (ch->level > 1)
	{
	    sprintf(kav,ch->pcdata->switchname);
	    free_char(d->character);
	    d->character = NULL;
	    fOld = load_char_obj( d, kav );
	    ch   = d->character;
	}

	if ( !IS_EXTRA(ch,EXTRA_NEWPASS) && strlen(argument) > 1) {
		sprintf(kav,"%s %s",argument,argument);
		do_password(ch,kav);}

        if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
                ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
        {
            sprintf(kav,"Last connected from %s at %s\n\r",ch->lasthost,ch->lasttime);
            write_to_buffer( d, kav, 0 );
        }   
        else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
        {
            sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
            write_to_buffer( d, kav, 0 );
        }
        
	if (ch->lasthost != NULL)
		free_string(ch->lasthost);
	if (ch->desc != NULL && ch->desc->host != NULL)
	    ch->lasthost = str_dup(ch->desc->host);
	else
	    ch->lasthost = str_dup("(unknown)");
	strtime = ctime( &current_time );
	strtime[strlen(strtime)-1] = '\0';
	free_string(ch->lasttime);
	ch->lasttime = str_dup( strtime );
	sprintf( log_buf, "%s@%s has connected.",ch->name, ch->lasthost );
	log_string( log_buf );
       if (ch->desc->ansi)
         SET_BIT(ch->act, PLR_ANSI);
       else REMOVE_BIT(ch->act, PLR_ANSI);


	/* In case we have level 4+ players from another merc mud, or 
	 * players who have somehow got file access and changed their pfiles.
	 */
	if ( ch->level > 3 && ch->trust == 0)
	    ch->level = 3;
	else
	{
	    if ( ch->level > MAX_LEVEL )
		ch->level = MAX_LEVEL;
	    if ( ch->trust > MAX_LEVEL)
		ch->trust = MAX_LEVEL;
	    /* To temporarily grant higher powers...
	    if ( ch->trust > ch->level)
		ch->trust = ch->level;
	    */
	    if ( ch->trust < ch->level );
		ch->trust = ch->level;
	}
/*	if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 &&
		ch->lasttime != NULL && strlen(ch->lasttime) > 1 )
	{
	    sprintf(kav,"Last connected from %s at %s\n\r",ch->lasthost,ch->lasttime);
	    write_to_buffer( d, kav, 0 );
	}
	else if ( fOld && ch->lasthost != NULL && strlen(ch->lasthost) > 1 )
	{
	    sprintf(kav,"Last connected from %s.\n\r",ch->lasthost);
	    write_to_buffer( d, kav, 0 );
	}

*/
	if( IS_MXP(ch) )
         do_help(ch,"MXP_DEF");
	if ( IS_HERO(ch) )
	    do_help( ch, "imotd" );
	do_help( ch, "motd" );
        old_players++;
	d->connected = CON_READ_MOTD;
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
           if (ch->desc->ansi)
             SET_BIT(ch->act, PLR_ANSI);
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

    	pwdnew = argument;

	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );

	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( argument, ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "What is your sex (M/F)? ", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}
	

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "Does your terminal support ANSI (y/n)? ", 0);
	d->connected = CON_GET_NEW_ANSI;
	break;

    case CON_GET_NEW_ANSI:
	switch ( argument[0] )
	{
	case 'y': case 'Y': SET_BIT(ch->act,PLR_ANSI);    break;
	case 'n': case 'N': break;
	default:
	    write_to_buffer( d, "Does your terminal support ANSI? (y/n)? ", 0 );
	    return;
	}

	ch->pcdata->perm_str=number_range(10,16);
        ch->pcdata->perm_int=number_range(10,16);
        ch->pcdata->perm_wis=number_range(10,16);
        ch->pcdata->perm_dex=number_range(10,16);
	ch->pcdata->perm_con=number_range(10,16);
	sprintf( log_buf, "%s@%s new player.",ch->name, d->host );
        new_players++;
	log_string( log_buf );
	/* NEWBIE LOGIN CHECKS GO HERE */
        SET_BIT(ch->special, SPC_NEWBIEPASS);
	SET_BIT(ch->act, PLR_COL_SCALE);
	SET_BIT(ch->act, PLR_NEW_HELP);
        do_pack(ch, "self");
	write_to_buffer( d, "\n\r", 2 );
	do_help( ch, "motd" );
	d->connected = CON_READ_MOTD;
	break;

    case CON_READ_MOTD:
	write_to_buffer( d, "\n\r** Be weary young traveller, for you are about to enter the Vortex **\n\r", 0 );
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
   

	/* OTHER LOGIN CHECKS GO HERE */
        do_fixpoly(ch,"");
        if( str_cmp(ch->clan,"DAMNED") )
	 ch->clan = strdup("");
        if( IS_SET(ch->act, PLR_CHALLENGED))
         REMOVE_BIT(ch->act, PLR_CHALLENGED);
        if( IS_SET(ch->act, PLR_CHALLENGER))
         REMOVE_BIT(ch->act, PLR_CHALLENGER);
        ch->stasis = FALSE;
        if( IS_CLASS(ch,CLASS_DRAGON) )
         class_attributes(ch);
        if( IS_CLASS(ch,CLASS_SHADOW) )
        {
         if( IS_SET(GET_POWERS(ch,SHADOW_BITS),SHADOW_FOLLOW) )
          REM_BIT_POWERS(ch,SHADOW_BITS,SHADOW_FOLLOW);
         if( IS_SET(GET_POWERS(ch,SHADOW_BITS),SHADOW_FOLLGO) )
          REM_BIT_POWERS(ch,SHADOW_BITS,SHADOW_FOLLGO);
        }
        if( IS_CLASS(ch,CLASS_VAMPIRE) )
        {
         if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_POTENCE) )
         {
          REM_BIT_POWERS(ch,VAMP_BITS,BIT_POTENCE);
          ch->generation++;
         }
         if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLASHING) )
         {
          REM_BIT_POWERS(ch,VAMP_BITS,BIT_FLASHING);
          GET_POWERS(ch,DISC_CELERITY)--;
         }
         if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BAAL) )
         {
          REM_BIT_POWERS(ch,VAMP_BITS,BIT_BAAL);
          GET_POWERS(ch,DISC_CELERITY)--;
          GET_POWERS(ch,DISC_POTENCE)--;
          GET_POWERS(ch,DISC_FORTITUDE)--;
         }
         GET_POWERS(ch,VAMP_FORM) = FORM_NORMAL;
         if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DISGUISE) )
          REM_BIT_POWERS(ch,VAMP_BITS,BIT_DISGUISE);
         if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_TIDE) )
          REM_BIT_POWERS(ch,VAMP_BITS,BIT_TIDE);
        }
        if( ch->pcdata->version < 3 )
        {
         ch->race += (ch->generation / 2);
         ch->race += (ch->pcdata->souls / 3);
         ch->generation = 4;
         ch->pcdata->souls = 0;
         stc("#7Your generation and souls have been reset in accordance to the latest changes.#n\n\r",ch);
         stc("#7You have been reimbursed with status to compensate.#n\n\r",ch);
        }
        if( ch->pcdata->version >= CURRENT_PFILE_VERSION )
        {
         if( ch->race >= 60 && ch->generation > 1 )
         {
          ch->generation = 1;
          stc("#7Your generation has advanced from your 60 status!#n\n\r",ch);
         }
         else if( (ch->race >= 35 && ch->race < 60) && ch->generation > 2 )
         {
          ch->generation = 2;
          stc("#7Your generation has advanced from your 35 status!#n\n\r",ch);
         }
         else if( (ch->race >= 15 && ch->race < 35) && ch->generation > 3 )
         {
          ch->generation = 3;
          stc("#7Your generation has advanced from your 15 status!#n\n\r",ch);
         }
        }
        if( ch->generation > 4 )
         ch->generation = 4;
	if( !IS_SET(ch->pcdata->askills[AS_BITS],ABIT_TRUESIGHT) )
	{
	 if( !IS_IMMORTAL(ch) )
	 {
	  if( IS_SET(ch->act, PLR_HOLYLIGHT) )
	   REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	 }
	}
	if( IS_CLASS(ch, CLASS_SHADOW) )
	{
	 GET_POWERS(ch, SHADOW_COUNTER) = 0;
	 if( IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_FOLLOW) )
	  REM_BIT_POWERS(ch, SHADOW_BITS, SHADOW_FOLLOW);
	}
        if( ch->trust == 0)
          ch->level = 1;
        if( (ch->ABlock[POWER_TYPE] != 0) && (ch->class == 0))
          ch->class = CLASS_APOCRYPHIC;
      if (!IS_NPC (ch)) 
      {
         for (obj= ch->carrying; obj != NULL; obj = obj_next )
         {
            obj_next = obj->next_content;
            if (IS_SET (obj->quest, QUEST_ARTIFACT) || obj->item_type == ITEM_CORPSE_NPC)
            {    
               extract_obj (obj);
            }     
            } 
            }
	
	while ( dtype < 13 )
	{
	
	dtype = dtype + 1;
	}
/* Arena Stuff */
	if (ch->level > 0)
	{
	    if (ch->gladiator  != NULL)
			ch->gladiator       = NULL; /* set player to bet on to NULL */
	    if (ch->challenger != NULL)
			ch->challenger = NULL;
	    if (ch->challenged != NULL)
			ch->challenged = NULL;
	    if (IS_SET(ch->act,PLR_CHALLENGER))
			REMOVE_BIT(ch->act,PLR_CHALLENGER);
	    if (IS_SET(ch->act,PLR_CHALLENGED))
			REMOVE_BIT(ch->act,PLR_CHALLENGED);
		if (ch->level > 0 && (ch->in_room != NULL) && 
                       (ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
		{
			char_to_room( ch, get_room_index(3054) );
			char_from_room( ch );
			do_look( ch, "auto" );
		}
		if (ch->race < 0)
			ch->race = 0;
		if (!IS_SET(ch->act, PLR_AUTOEXIT))
			do_config(ch,"+autoexit");
	}
	
/* end of arena stuff */
	if ( !IS_SET(ch->extra, EXTRA_TRUSTED) )
		SET_BIT(ch->extra, EXTRA_TRUSTED);
	/*wibble*/
	//if ((ch->level > 2) && (!IS_SET(ch->special, SPC_NEWBIEPASS)))
	//	SET_BIT(ch->special, SPC_NEWBIEPASS);
	if ( ch->level == 0 )
	{

	    ch->pcdata->plr_wager    = 0; /* arena betting amount */
            ch->pcdata->awins        = 0; /* arena wins           */
            ch->pcdata->alosses      = 0; /* arena losses         */
            ch->gladiator       = NULL; /* set player to bet on to NULL */
	    ch->challenger = NULL;
	    ch->challenged = NULL;
	    ch->level	= 1;
	    ch->exp	= 10000;
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
	    ch->class   = 0;
		do_config(ch,"+autoexit");
            ch->special = 0;
	    set_switchname(ch, ch->name);
	    set_title( ch, "the mortal" );
		do_pack(ch, "self");
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    do_look( ch, "auto" );
	}
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	{
	    if (ch->in_room != NULL) char_to_room( ch, ch->in_room );
	    else char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    bind_char(ch);
	    break;
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	    do_look( ch, "auto" );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	    do_look( ch, "auto" );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	    do_look( ch, "auto" );
	}
/*klinemark logins*/
	if IS_SET(ch->newbits, NEW_SETLOG)
	sprintf(buf,"%s",ch->setlog);
	else
	sprintf(buf,"#3%s #2has been sucked into the Vortex.#n", ch->name );
        if( !IS_SET(ch->act,PLR_WIZINVIS) && !IS_SET(ch->act,PLR_INCOG) )  
	 do_info(ch,buf);

	ch->fight_timer = 0;
        ch->login_timer = 5;
	if( !str_cmp(ch->clan, "DAMNED") )
	{
	   ch->level = 4;
	   ch->trust = 4;
	}
	act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );
	room_text(ch,">ENTER<");

	ch->embraced=NULL; 
        ch->embracing=NULL;


    /* START CLEAR */

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	if ( obj->wear_loc != WEAR_NONE ) 
	    {obj_from_char(obj); obj_to_char(obj,ch);}
	}
	while ( ch->affected )
	    affect_remove( ch, ch->affected );
	if (IS_AFFECTED(ch,AFF_POLYMORPH) && IS_AFFECTED(ch,AFF_ETHEREAL))
	{
    	    ch->affected_by	 = AFF_POLYMORPH + AFF_ETHEREAL;
	}
	else if (IS_AFFECTED(ch,AFF_POLYMORPH))
	    ch->affected_by	 = AFF_POLYMORPH;
	else if (IS_AFFECTED(ch,AFF_ETHEREAL))
	    ch->affected_by	 = AFF_ETHEREAL;
	else
	    ch->affected_by	 = 0;
	ch->armor		 = 100;
	ch->hit		 = UMAX( 1, ch->hit  );
	ch->mana		 = UMAX( 1, ch->mana );
	ch->move		 = UMAX( 1, ch->move );
	ch->hitroll		 = 0;
	ch->damroll		 = 0;
	ch->saving_throw	 = 0;
	ch->pcdata->mod_str	 = 0;
	ch->pcdata->mod_int	 = 0;
	ch->pcdata->mod_wis	 = 0;
	ch->pcdata->mod_dex	 = 0;
	ch->pcdata->mod_con	 = 0;
	save_char_obj( ch );

	if (ch->level == 1)
	{
/*                ch->exp += 10000;
		do_claim(ch, "dagger");
		do_claim(ch, "mask");
		do_claim(ch, "lantern");
		do_claim(ch, "belt");
		do_claim(ch, "cloak");
		do_claim(ch, "shield");
		do_claim(ch, "sleeves");
		do_claim(ch, "gloves");
		do_claim(ch, "boots");
		do_claim(ch, "leggings");
		do_claim(ch, "helmet");
		do_claim(ch, "plate");
		do_claim(ch, "collar");
		do_claim(ch, "2.collar");
		do_claim(ch, "bracer");
		do_claim(ch, "2.bracer");
		do_claim(ch, "ring");
		do_claim(ch, "2.ring");
		do_claim(ch, "sword");
		do_claim(ch, "bag");*/
	}
	/* Attempt to re-wear */
        do_wear(ch, "all");
	send_to_char("Your stats have been cleared.  Please check your equipment.\n\r",ch);
	update_players_logged();

	/* END */
	if (ch->level < 2)
	{
		do_help( ch, "newbie" );
	    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	    send_to_char("We suggest that if you require assistance that you enquire to the spirit of the\n\r",ch);
		send_to_char("MUD School, and read help newbie. Thank you.\n\r", ch);
	    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
		do_look( ch, "auto" );
	}
	break;
    }
    return;
}


/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto immortal self someone gaia quit hitler" ) )
	return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen(name) <  3 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch) && !IS_EXTRA(ch, EXTRA_SWITCH)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character), GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "Reconnecting.\n\r", ch );
	    	if (IS_NPC(ch) || ch->pcdata->obj_vnum == 0)
		    act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s reconnected.",ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Kick off old connection.  KaVir.
 */
bool check_kickoff( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&& ( !fConn || ch->desc == NULL )
	&&   !str_cmp( GET_PC_NAME(d->character),GET_PC_NAME(ch) ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "You take over your body, which was already in use.\n\r", ch );
		act( "...$n's body has been taken over by another spirit!", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s kicking off old link.", ch->name, ch->lasthost );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing - KaVir.
 * Using kickoff code from Malice, as mine is v. dodgy.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold != NULL; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? GET_PC_NAME(dold->original) : GET_PC_NAME(dold->character) ) )
	{
	    char	buf [MAX_STRING_LENGTH];
	    if ( d->character != NULL )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    send_to_char("This body has been taken over!\n\r",dold->character);
	    d->connected = CON_PLAYING;
	    d->character = dold->character;
	    d->character->desc = d;
	    send_to_char( "You take over your body, which was already in use.\n\r", d->character );
	    act( "$n doubles over in agony and $s eyes roll up into $s head.", d->character, NULL, NULL, TO_ROOM );
	    act( "...$n's body has been taken over by another spirit!", d->character, NULL, NULL, TO_ROOM );
	    dold->character=NULL;
	    
	    sprintf(buf,"Kicking off old connection %s@%s",d->character->name,d->host);
	    log_string(buf);
	    close_socket(dold);	/*Slam the old connection into the ether*/
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   (ch->desc->connected != CON_PLAYING && ch->desc->connected != CON_EDITING)
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}


void stc( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

int col_str_len(char *txt)
{
    int pos, len;

    len = 0;
    for (pos = 0; txt[pos] != '\0'; pos++)
    {
	if (txt[pos] != '#')
	{
	    len++;
	    continue;
	}

	pos++;
	if (txt[pos] == '\0') return len;
	if (txt[pos] == '#' || txt[pos] == '-') len++;
    }

    return len;
}

void cent_to_char(char *txt, CHAR_DATA *ch)
{
    int len,pos;
    char buf[MAX_STRING_LENGTH];

    len = (80 - col_str_len(txt)) / 2;
    for (pos = 0; pos < len; pos++)
    {
	buf[pos] = ' ';
    }
    buf[pos]= '\0';
    send_to_char(buf, ch);
    send_to_char(txt, ch);
    send_to_char("\n\r",ch);
}

void divide_to_char(CHAR_DATA *ch)
{
send_to_char("-------------------------------------------------------------------------------\r\n",
ch); }

void divide2_to_char(CHAR_DATA *ch)
{
send_to_char("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\r\n",
ch); }

void divide3_to_char(CHAR_DATA *ch)
{
send_to_char("===============================================================================\r\n",ch);
}

void divide4_to_char(CHAR_DATA *ch)
{
send_to_char("#4-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6***#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-=[#6**#4]=-\r\n",ch);
}

void divide5_to_char(CHAR_DATA *ch)
{
cent_to_char("#4-=[#6***********#4]=-------------=[#6***********#4]=-#n",ch);
}

void divide6_to_char(CHAR_DATA *ch)
{
cent_to_char("#4-    -   -  - - -#6- ---====#7*#6====--- -#4- - -  -   --",ch); }

void banner_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#2===#2==#2=====#2==#2=====#2==#2====#n[                               ]#2====#2==#2=====#2==#2=====#2==#2===#n"); 
	wdth = (31 - ln) / 2 + 20;
}
else
{
	sprintf(buf,
"#2===#2==#2=====#2==#2=====#2==#2=====#2==#2====#n[                ]#2====#2==#2=====#2==#2=====#2==#2=====#2==#2===#n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 22] = txt[loop];
cent_to_char(buf,ch);
}

void banner2_to_char(char *txt, CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int loop,wdth,ln;
ln = strlen(txt);
if (ln > 16)
{
	sprintf(buf, "#4    -   -  - - -#6- ---===#7                               #6===--- -#4- - -  -   -\r\n"); 
	wdth = (31 - ln) / 2 + 24;
}
else
{
	sprintf(buf, "#4     -    -   -  - - -#6- ---====#7                #6====--- -#4- - -  -   -    -\r\n");
	wdth = (16 - ln) / 2 + 32;
}
for (loop = 0; loop < ln; loop++)
    buf[loop + wdth + 6] = txt[loop];
send_to_char(buf,ch);
}


/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
/*    CHAR_DATA *wizard;
    CHAR_DATA *familiar;

    if ( ch->desc == NULL && IS_NPC(ch) && (wizard = ch->wizard) != NULL )
    {
	if (!IS_NPC(wizard) && (familiar = wizard->pcdata->familiar) != NULL 
	    && familiar == ch && ch->in_room != wizard->in_room)
	{
	    send_to_char("[ ",wizard);
	    if ( txt != NULL && wizard->desc != NULL )
		write_to_buffer( wizard->desc, txt, strlen(txt) );
	}
    }
*/
    if ( txt != NULL && ch->desc != NULL )
	write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

void send_to_desc( const char *txt, DESCRIPTOR_DATA *d)
{
        
    if ( txt && d )
        write_to_buffer( d, txt, strlen(txt) );
    return;
}

/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    BUFFER *buf2 = buffer_new(MAX_STRING_LENGTH);
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    DESCRIPTOR_DATA *d;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char GOCIAL_TAG[] = "#4[#6>>#7-#1GOCIAL#7-#6<<#4]#n  ";
    const char *str;
    const char *i;
    char *point;

    bool is_fam;

    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;
    if( !str_cmp(format,"NOT SET") || !str_cmp(format,"(null)") )
     return;
    if( ch != NULL && ch->in_room != NULL)
     if( (to = ch->in_room->people) == NULL)
      return;
    if ( type == TO_VICT )
    {
	if ( vch == NULL || vch->in_room == NULL)
	{
/*            sprintf(bug_buf, "Act: null vch with TO_VICT.  Calling character: %s, string: %s",
                  ch->name, format);
	    log_string(bug_buf);
*/	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {

	is_fam = FALSE;
	to_old = to;

	if ( type == TO_CHAR && to != ch ) continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
	if ( type == TO_ROOM && to == ch ) continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;
	if ( to->desc == NULL || !IS_AWAKE(to) )
	{

	    if (is_fam) to = to_old;

	    continue;
	}

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok)
	    {

		if (is_fam) to = to_old;

		continue;
	    }
    	}

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
	    {
		/* bug( "Act: missing arg2 for code %d.", *str );*/
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS( ch,  to  );				break;
/*KlineMark*/	case 'N': i = PERS( vch, to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'M': i = him_her [URANGE(0, vch ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'S': i = his_her [URANGE(0, vch ->sex, 2)];	break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? ( (obj2->chobj != NULL && obj2->chobj == to)
			    ? "you" : obj2->short_descr)
			    : "something";
		    break;

		case 'd':
		    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';

	buf[0]   = UPPER(buf[0]);
	if( type != GOCIAL )
	   write_to_buffer( to->desc, buf, point - buf );
	else
	{
	   for( d = descriptor_list; d != NULL; d = d->next )
	   {
		if( d->connected == CON_PLAYING
		&&  d->character != ch
		&&  (d->character != vch || vch == NULL) ) 
		{
		   stc("\n\r", d->character);
		   stc(GOCIAL_TAG, d->character);
		   write_to_buffer(d->character->desc, buf, point - buf );
		}

	   }
	   if( is_fam ) to = to_old;
	   buffer_free(buf2);
	   return;
	}

       /* if( to->desc->infect != NULL)
        {
         buffer_clear(buf2);
         bprintf( buf2, "#4%s: %s\n\r", to->name, buf);
         write_to_buffer(to->desc->infect,buf2->data, 0);
        }*/
	if (is_fam) to = to_old;

    }
    buffer_free(buf2);
    return;
}

void act2( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type, int dam)
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    BUFFER *buf2 = buffer_new(MAX_STRING_LENGTH);
    char fname[MAX_INPUT_LENGTH];
    char number[20];
    CHAR_DATA *to;

    CHAR_DATA *to_old;

    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;   
        
    bool is_fam;
 
    bool is_ok;
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;
    if( ch != NULL && ch->in_room != NULL)
     if( (to = ch->in_room->people) == NULL)
      return;
    if ( type == TO_VICT )
    {
        if ( vch == NULL || vch->in_room == NULL)
        {
            bug( "Act2: null vch with TO_VICT.", 0 );
            return;
        }
        to = vch->in_room->people;
    }
  
    for ( ; to != NULL; to = to->next_in_room )
    {
  
        is_fam = FALSE;
        to_old = to;
    
        if ( type == TO_CHAR && to != ch ) continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) ) continue;
        if ( type == TO_ROOM && to == ch ) continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) ) continue;
        if ( to->desc == NULL || !IS_AWAKE(to) )
        {
         
            if (is_fam) to = to_old;
     
            continue;
        }
     
        if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
        {
            is_ok = FALSE;
    
            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                ch->pcdata->chobj->in_room != NULL &&
                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                to->pcdata->chobj->in_room != NULL &&
                ch->in_room == to->in_room)
                    is_ok = TRUE; else is_ok = FALSE;
            
            if (!IS_NPC(ch) && ch->pcdata->chobj != NULL &&
                ch->pcdata->chobj->in_obj != NULL &&
                !IS_NPC(to) && to->pcdata->chobj != NULL &&
                to->pcdata->chobj->in_obj != NULL &&
                ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
                    is_ok = TRUE; else is_ok = FALSE;
                    
            if (!is_ok)
            {
         
                if (is_fam) to = to_old;
        
                continue;
            }
        }
     
        point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            ++str;
                
            {
                switch ( *str )
                {
                default:  bug( "Act2: bad code %d.", *str );
                          i = " <@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 'D': sprintf(number, "%d", dam);                   
                          i = (char *) number; break;
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;
                case 'n': i = PERS( ch,  to  );                         break;
                case 'N': i = PERS( vch, to  );                         break;
                case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];        break;
                case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];        break;
                case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];        break;
                case 'M': i = him_her [URANGE(0, vch ->sex, 2)];        break;
                case 's': i = his_her [URANGE(0, ch  ->sex, 2)];        break;
                case 'S': i = his_her [URANGE(0, vch ->sex, 2)];        break;
         
                case 'p':
                    i = can_see_obj( to, obj1 )
                            ? ( (obj1->chobj != NULL && obj1->chobj == to)
                            ? "you" : obj1->short_descr)
                            : "something";
                    break;
                
                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? ( (obj2->chobj != NULL && obj2->chobj == to)
                            ? "you" : obj2->short_descr)
                            : "something";
                    break;
             
                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
            }
                
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }
                            
        *point++ = '\n';
        *point++ = '\r';
        buf[0]   = UPPER(buf[0]);
        write_to_buffer( to->desc, buf, point - buf );
        if (is_fam) to = to_old;
               
    }
    buffer_free(buf2);
    return;
}

void kavitem( const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type )
{
    static char * const he_she	[] = { "it",  "he",  "she" };
    static char * const him_her	[] = { "it",  "him", "her" };
    static char * const his_her	[] = { "its", "his", "her" };

    char buf[MAX_STRING_LENGTH];
    char kav[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    const char *str;
    const char *i;
    char *point;
    bool is_ok;

    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( vch == NULL )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to != NULL; to = to->next_in_room )
    {
	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

    	if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    	{
	    is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_room != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_room != NULL &&
	    	ch->in_room == to->in_room)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && 
	    	ch->pcdata->chobj->in_obj != NULL &&
	    	!IS_NPC(to) && to->pcdata->chobj != NULL && 
	    	to->pcdata->chobj->in_obj != NULL &&
	    	ch->pcdata->chobj->in_obj == to->pcdata->chobj->in_obj)
		    is_ok = TRUE; else is_ok = FALSE;

	    if (!is_ok) continue;
    	}
	if ( type == TO_CHAR && to != ch )
	    continue;
	if ( type == TO_VICT && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
		i = "";
	    else
	    {
		switch ( *str )
		{
		default:  i = "";					break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];	break;
		case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];	break;
		case 's': i = his_her [URANGE(0, ch  ->sex, 2)];	break;
		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "you" : obj1->short_descr)
			    : "something";
		    break;

		case 'o':
		    if (obj1 != NULL) sprintf(kav,"%s's",obj1->short_descr);
		    i = can_see_obj( to, obj1 )
			    ? ( (obj1->chobj != NULL && obj1->chobj == to)
			    ? "your" : kav)
			    : "something's";
		    break;

		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER(buf[0]);
	write_to_buffer( to->desc, buf, point - buf );
    }

    return;
}
void bust_a_header(DESCRIPTOR_DATA *d)
{
char class[MAX_STRING_LENGTH];
char class2[MAX_STRING_LENGTH];
char header[MAX_STRING_LENGTH];
char header1[MAX_STRING_LENGTH];
char blanklin[MAX_STRING_LENGTH];
CHAR_DATA *ch;
char cls[MAX_STRING_LENGTH];
sprintf(cls," ");

ch=d->character;
if (ch==NULL) return;
ADD_COLOUR(ch,cls,NORMAL);
sprintf(class,"Classless"); 
sprintf(class2,"%s the %s",ch->name,class);
sprintf(blanklin," ");
sprintf(header1,"%-30s Align:%-4d",class2,ch->alignment);
sprintf(header,
"\0337\033[1;1H\033[1;44m\033[1;37m%-79s%s\0338",header1,cls);
send_to_char(header,ch);
return;
}
/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
         CHAR_DATA *ch;
         CHAR_DATA *victim;
         CHAR_DATA *tank;
   const char      *str;
   const char      *i;
         char      *point;
         char       buf  [ MAX_STRING_LENGTH ];
         char       buf2 [ MAX_STRING_LENGTH ];
	 bool       is_fighting = TRUE;

   if ( ( ch = d->character ) == NULL ) return;
   if ( ch->pcdata == NULL )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }
   if ( ch->position == POS_FIGHTING && ch->cprompt[0] == '\0' )
   {
      if ( ch->prompt[0] == '\0' )
      {
         send_to_char( "\n\r\n\r", ch );
         return;
      }
      is_fighting = FALSE;
   }
   else if ( ch->position != POS_FIGHTING && ch->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }

   point = buf;
   if ( ch->position == POS_FIGHTING && is_fighting )
      str = d->original ? d->original->cprompt : d->character->cprompt;
   else
      str = d->original ? d->original->prompt : d->character->prompt;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit );
	    if( IS_SET(ch->act, PLR_COL_SCALE) )
            COL_SCALE(buf2, ch, ch->hit, ch->max_hit);
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana                              );
            if( IS_SET(ch->act, PLR_COL_SCALE) )
            COL_SCALE(buf2, ch, ch->mana, ch->max_mana);
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move                              );
            if( IS_SET(ch->act, PLR_COL_SCALE) ) 
            COL_SCALE(buf2, ch, ch->move, ch->max_move);
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move                          );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp                               );
            if( IS_SET(ch->act, PLR_COL_SCALE) )
            COL_SCALE(buf2, ch, ch->exp, 1000);
            i = buf2; break;
         case 'T':
            sprintf( buf2, "%d", ch->fight_timer                       );
            ADD_COLOUR(ch, buf2, L_RED);
            i = buf2; break;
         case 'f' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               if ((victim->hit*100/victim->max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((victim->hit*100/victim->max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((victim->hit*100/victim->max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((victim->hit*100/victim->max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((victim->hit*100/victim->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'F' :
            if ( ( victim = ch->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else if ( ( tank = victim->fighting ) == NULL )
            {
	       strcpy( buf2, "N/A" );
               ADD_COLOUR(ch, buf2, L_CYAN);
            }
            else
            {
               if ((tank->hit*100/tank->max_hit) < 25)
               {strcpy(buf2, "Awful");ADD_COLOUR(ch, buf2, L_RED);}
               else if ((tank->hit*100/tank->max_hit) < 50)
               {strcpy(buf2, "Poor");ADD_COLOUR(ch, buf2, L_BLUE);}
               else if ((tank->hit*100/tank->max_hit) < 75)
               {strcpy(buf2, "Fair");ADD_COLOUR(ch, buf2, L_GREEN);}
               else if ((tank->hit*100/tank->max_hit) < 100)
               {strcpy(buf2, "Good");ADD_COLOUR(ch, buf2, YELLOW);}
               else if ((tank->hit*100/tank->max_hit) >= 100)
               {strcpy(buf2, "Perfect");ADD_COLOUR(ch, buf2, L_CYAN);}
            }
            i = buf2; break;
         case 'n' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( IS_AFFECTED(victim, AFF_POLYMORPH) )
                  strcpy(buf2, victim->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, victim->short_descr);
               else
                  strcpy(buf2, victim->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'N' :
            if ( ( victim = ch->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else if ( ( tank = victim->fighting ) == NULL )
	       strcpy( buf2, "N/A" );
            else
            {
               if ( ch == tank )
                  strcpy(buf2, "You");
               else if ( IS_AFFECTED(tank, AFF_POLYMORPH) )
                  strcpy(buf2, tank->morph);
               else if ( IS_NPC(victim) )
                  strcpy(buf2, tank->short_descr);
               else
                  strcpy(buf2, tank->name);
               buf2[0] = UPPER(buf2[0]);
            }
            i = buf2; break;
         case 'a' :
            sprintf( buf2, "%s", IS_GOOD( ch ) ? "good"
		                  : IS_EVIL( ch ) ? "evil" : "neutral" );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'A' :
            sprintf( buf2, "%d", ch->alignment                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'r' :
            if( ch->in_room )
               sprintf( buf2, "%s", ch->in_room->name                  );
            else
               sprintf( buf2, " "                                      );
            ADD_COLOUR(ch, buf2, L_CYAN);
            i = buf2; break;
         case 'R' :
               sprintf( buf2, "%d", ch->rage);
               ADD_COLOUR(ch, buf2, L_RED);
            i = buf2; break;
         case 'c' :
            sprintf( buf2, "%d", class_ac(ch) );
            i = buf2; break;
         case 'p' :
            sprintf( buf2, "%d", class_hitroll(ch) );
            if( IS_SET(ch->act, PLR_COL_SCALE) )
            COL_SCALE(buf2, ch, class_hitroll(ch), 200);
            i = buf2; break;
         case 'P' :
            sprintf( buf2, "%d", class_damroll(ch) );
            if( IS_SET(ch->act, PLR_COL_SCALE) )
            COL_SCALE(buf2, ch, class_damroll(ch), 200);
            i = buf2; break;
	 case 't' :
            if( weather_info.sunlight == SUN_DARK || weather_info.sunlight == SUN_SET )
	     sprintf( buf2, "Night" );
            else
             sprintf( buf2, "Day"   );
            i = buf2; break;
	case 'i' :
	    if( !IS_CLASS(ch,CLASS_MONK) )
	     sprintf( buf2, "NA"    );
	    else
	     sprintf( buf2, "%d", GET_POWERS(ch,MONK_CURR_CHI) );
	    i = buf2; break;
	case 'I' :
	    if( !IS_CLASS(ch,CLASS_MONK) )
	     sprintf( buf2, "NA"    );
	    else
	     sprintf( buf2, "%d", GET_POWERS(ch,MONK_MAX_CHI) );
	    i = buf2; break;
	case 'u' :
	    if( !IS_CLASS(ch,CLASS_MONK) )
	     sprintf( buf2, "NA"    );
	    else
	     sprintf( buf2, "%d", GET_POWERS(ch,MONK_CURR_FOC) );
	    i = buf2; break;
	case 'U' :
	    if( !IS_CLASS(ch,CLASS_MONK) )
	     sprintf( buf2, "NA"    );
	    else
	     sprintf( buf2, "%d", GET_POWERS(ch,MONK_MAX_FOC) );
	    i = buf2; break;
        case 'b':
            if( !IS_CLASS( ch, CLASS_DRAGON))
             sprintf( buf2, "NA");
            else
             sprintf( buf2, "%d", GET_POWERS(ch, DRAGON_STIME) );
            i = buf2; break;
        case 'B':
            if( !IS_CLASS( ch, CLASS_DRAGON))
             sprintf( buf2, "NA");
            else
             sprintf( buf2, "%d", GET_POWERS(ch, DRAGON_BTIME) );
            i = buf2; break;     
        case '%' :
            sprintf( buf2, "%%"                                        );
            i = buf2; break;
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, buf, point - buf );
   return;
}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

void init_signals()
{
  signal(SIGBUS,sig_handler);
  signal(SIGTERM,sig_handler);
  signal(SIGABRT,sig_handler);
  signal(SIGSEGV,sig_handler);
}

void sig_handler(int sig)
{
  switch(sig)
  {
    case SIGBUS:
      bug("Sig handler SIGBUS.",0);
      auto_copyover();
      break;
    case SIGTERM:
      bug("Sig handler SIGTERM.",0);
      auto_copyover();
      break;
    case SIGABRT:
      bug("Sig handler SIGABRT",0);
      auto_copyover();             
     case SIGSEGV:
      bug("Sig handler SIGSEGV",0);
      auto_copyover();
      break;
  }
}

void ch_output( CHAR_DATA *ch, char *txt, ...)
{
  va_list args;
  char buf[MAX_STRING_LENGTH];

  va_start( args, txt);
  vsprintf( buf, txt, args);
  va_end(args);

  stc(buf,ch);
  stc("\n\r",ch);
  return;
}
