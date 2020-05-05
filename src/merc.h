/**************************************************************************
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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#ifndef MERC_H
#define MERC_H

#include <mysql/mysql.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <sys/time.h>  
#include <dlfcn.h>
/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#endif

#define                 MAX_MEM_LIST    13
void pwipe();
/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef int 				sh_int;
typedef unsigned char			bool;
#endif
#include "player.h"
#include "old.h"
#include "buffer.h"
#include "libs.h"

struct ansi_type
{
   char *	name;
   char	*	value;		/* escape sequence, or whatever */
   int 		index;
   char         letter;
   int          stlen;
};

#define MAX_ANSI	28
extern  const   struct  ansi_type	    ansi_table	    [MAX_ANSI];
void    log_f (char * fmt, ...) __attribute__ ((format(printf,1,2)));
extern bool mem_log;
#define STR(x) #x
#define SX(x) STR(x)
#define getmem(size) _getmem(size,_caller,1)
#define qgetmem(size) _getmem(size,_caller,0)
#define _caller __FILE__ ":" SX(__LINE__)
#define _dispose(mem,size,log) \
do { \
  if ( ! (mem) ) \
  { \
    bug("Disposing NULL memory",0); \
    return; \
  } \
  if (log&&mem_log) log_f("dispose(%p) from %s:%d", (mem), __FILE__, __LINE__); \
  free((mem)); \
  (mem) = NULL; \
} while(0)
#define dispose(mem,size) _dispose(mem,size,1)
#define qdispose(mem,size) _dispose(mem,size,0)

/*
 * Structure types.
 */
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct	char_data		CHAR_DATA;

typedef struct  config_data		CONFIG_DATA;
typedef	struct	editor_data		EDITOR_DATA;
typedef struct  board_data		BOARD_DATA;

typedef struct	class_data		CLASS_DATA;

typedef struct  balance_data		BALANCE_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
typedef struct  disabled_data           DISABLED_DATA;
typedef struct  race_data               RACE_DATA;
typedef struct  clan_data               CLAN_DATA;
typedef struct  fortress_data           FORTRESS_DATA;
typedef struct  wiz_data                WIZ_DATA;

extern AREA_DATA *area_first;
extern CLASS_DATA *alie;
extern CLASS_DATA *apoc;
extern CLASS_DATA *drag;
extern CLASS_DATA *drow;
extern CLASS_DATA *jedi;
extern CLASS_DATA *monk;
extern CLASS_DATA *vamp;
extern CLASS_DATA *were;


/* one disabled command */
struct disabled_data
{
      DISABLED_DATA *next; /* pointer to next node */
      struct cmd_type const *command; /* pointer to the command struct*/
      char *disabled_by; /* name of disabler */
      sh_int level; /* level of disabler */
};
extern                        DISABLED_DATA     *disabled_first;

struct race_data
{
   RACE_DATA *next;
   char *name;
   int group1;
   int group2;
   bool complete;
};

/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch ) );
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
/* sound defines for MSP */
#define SOUND_SWORD1 "vortexmsp/sword.wav"
#define SOUND_BITE   "vortexmsp/bite.wav"
#define SOUND_SCREAM "vortexmsp/diescream.wav"
#define SOUND_GUN    "vortexmsp/goldengun.wav"
#define SOUND_WOO    "vortexmsp/gyscream.wav" /*dont use*/
#include "globals.h"
/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 4096
#define MAX_INPUT_LENGTH	  400
#define ALIGN_NONE		0
#define ALIGN_LEFT		1
#define ALIGN_RIGHT             2
#define ALIGN_CENTER		3
#define CURRENT_PFILE_VERSION   6

int jedi_eq_counter( CHAR_DATA *ch, bool exempt );
int blood_max( CHAR_DATA *ch );
int  count_mobs( CHAR_DATA *ch);
bool wearing_one_ring(CHAR_DATA *ch);
bool is_naked(CHAR_DATA *ch);
bool is_ds_range( CHAR_DATA *ch );
bool is_vorpal( CHAR_DATA *ch );
void play_sound( CHAR_DATA *ch, char *sound, int type);
void *_getmem( int size, const char *caller, int log);
void send_to_room args(( char *message, ROOM_INDEX_DATA *room ));
void conceal_count args((CHAR_DATA *ch));
void strip_class_bits args((CHAR_DATA *ch));
bool has_metal	args((CHAR_DATA *ch, int count, int type));
void remove_bars(CHAR_DATA *ch, int count, int type);
bool in_arena(CHAR_DATA *ch);
bool has_power_level(CHAR_DATA *ch, int power, int level);
bool has_power(CHAR_DATA *ch, int power);
bool standard_calls(CHAR_DATA *ch, int power, int level);
int random_block(CHAR_DATA *ch);
int get_warp(CHAR_DATA *ch);
bool is_valid_warp(CHAR_DATA *ch, int warp);
void update_players_logged(void);
void set_stun( CHAR_DATA * victim, int stuntime );
void display_class_stats( CHAR_DATA *ch);
void improve_class(CHAR_DATA *ch, int field);
int class_parry(CHAR_DATA *ch);
int class_dodge(CHAR_DATA *ch);
int class_num_attack(CHAR_DATA *ch);
int class_mod_damage(CHAR_DATA *ch, CHAR_DATA *victim, int dam);
void get_points(CHAR_DATA *ch, CHAR_DATA *victim);
void class_attributes(CHAR_DATA *ch);
int class_hitroll(CHAR_DATA *ch);
int class_damroll(CHAR_DATA *ch);
int class_ac(CHAR_DATA *ch);
void class_regen(CHAR_DATA *ch);
bool has_weapons(CHAR_DATA *ch);
WIZ_DATA *get_immortal( char *argument);

/* 
 * Rotains Gobal Procedures
 */
void put_wall       args( (ROOM_INDEX_DATA *room, int dir, int wall));
void make_wall      args( (ROOM_INDEX_DATA *room, int dir, int wall));

void shootdirection args( (CHAR_DATA *ch, CHAR_DATA *victim,int range,char arg[MAX_STRING_LENGTH]));
void snipedirection args( (CHAR_DATA *ch, CHAR_DATA *victim,int range,char arg[MAX_STRING_LENGTH]));
bool safe_room      args( (CHAR_DATA *ch));
void clear_stats    args( (CHAR_DATA *ch) );
void room_is_total_darkness args( (ROOM_INDEX_DATA *pRoomIndex) );
void improve_wpn    args( (CHAR_DATA *ch,int dtype, int right_hand) );
void improve_stance args( (CHAR_DATA *ch) );
void skillstance    args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void show_spell     args( (CHAR_DATA *ch, int dtype) );
void fightaction    args( (CHAR_DATA *ch, CHAR_DATA *victim, int actype , \
int dtype, int wpntype) ); 
void crack_head     args( (CHAR_DATA *ch, OBJ_DATA *obj, char *argument) );
void critical_hit   args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam) );

void take_item      args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void newbie_trans   args( (OBJ_DATA *obj) );
void do_damn	    args( (CHAR_DATA *ch, char *argument) );
void raw_kill       args( (CHAR_DATA *victim) );
void trip           args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void disarm         args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void make_corpse    args( (CHAR_DATA *ch) );
void one_hit        args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt, \
				int handtype) );
void make_part      args( (CHAR_DATA *ch, char *argument) );
void do_reward	    args( (CHAR_DATA *ch, char *argument) );
void    group_gain              args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void behead                 args( (CHAR_DATA *victim) );
void paradox                args( (CHAR_DATA *ch) );

void load_bans	args( (void) );
void save_bans	args( (void) );
void load_class_stats args( (void) );
void save_class_stats args( (void) );
void save_commands args( (void) );
void config_who_exp args( (void) );
int count_techniques(CHAR_DATA *ch);
int get_hours_played	    args( (CHAR_DATA *ch) );

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */


#define SKILL_ADEPT             100
#define MAX_FORGET              10 
#define DONATION_ROOM_WEAPON    3207
#define DONATION_ROOM_ARMOR     3207
#define DONATION_ROOM_REST      3207 

#define PARADOX_TICK               30
#define MAX_SKILL		  250
#define MAX_SPELL		  120
#define MAX_LEVEL		   12
#define NO_WATCH		    10
#define NO_GODLESS		   (MAX_LEVEL - 2)
#define LEVEL_HERO		   (MAX_LEVEL - 9)
#define LEVEL_IMMORTAL	   (MAX_LEVEL - 5)

#define LEVEL_MORTAL	   (MAX_LEVEL - 10)
#define LEVEL_AVATAR	   (MAX_LEVEL - 9)
#define LEVEL_APPRENTICE   (MAX_LEVEL - 8)
#define LEVEL_MAGE		   (MAX_LEVEL - 7)
#define LEVEL_ARCHMAGE	   (MAX_LEVEL - 6)
#define LEVEL_NINJA        (MAX_LEVEL - 6)
#define LEVEL_MONK         (MAX_LEVEL - 6)
#define LEVEL_BUILDER	   (MAX_LEVEL - 5)
#define LEVEL_QUESTMAKER   (MAX_LEVEL - 4)
#define LEVEL_ENFORCER	   (MAX_LEVEL - 3)
#define LEVEL_JUDGE		   (MAX_LEVEL - 2)
#define LEVEL_HIGHJUDGE	   (MAX_LEVEL - 1)
#define LEVEL_IMPLEMENTOR  (MAX_LEVEL)

#define PULSE_PER_SECOND	    4
#define PULSE_VIOLENCE		  ( 3 * PULSE_PER_SECOND)
#define OLOAD_30333		    1

#define PULSE_EMBRACE             ( 5 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  ( 4 * PULSE_PER_SECOND)
#define PULSE_TICK		  (30 * PULSE_PER_SECOND)
#define PULSE_AREA		  (60 * PULSE_PER_SECOND)
#define PULSE_WW                  ( 4 * PULSE_PER_SECOND)

struct wiz_data
{
 WIZ_DATA *next;
 WIZ_DATA *prev;
 char *name;
 char *message;
 int  level;
};

struct clan_data
{
 CLAN_DATA *next;
 CLAN_DATA *prev;
 FORTRESS_DATA *fort;
 char *name;
 char *leader;
 char *coleader;
 int fight_timer;
 int kills;
 int deaths;
 int points;
 int id_num;
 char *channel;
 char *rank_name[10];
};

struct fortress_data
{
 char *name;
 int level;
 int max_tanks;
 int max_turrets;
 int tanks;
 int turrets;
 int spies;
 int wall_str[4];
 int wall_cond[4];
 int upper_vnum;
 int lower_vnum;
 int recall_vnum;
};

/*
 * Site ban structure.
 */
struct	ban_data
{
    BAN_DATA *	next;
    char *	name;
};

/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};



/*
 * Directions.
 * Used in #ROOMS.
 */
typedef enum
{
  DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_UP, DIR_DOWN,
  DIR_NORTHEAST, DIR_NORTHWEST, DIR_SOUTHEAST, DIR_SOUTHWEST,
DIR_SOMEWHERE
} dir_types;

#define MAX_DIR			DIR_SOUTHWEST	/* max for normal walking */

#define SUB_NORTH DIR_NORTH
#define SUB_EAST  DIR_EAST
#define SUB_SOUTH DIR_SOUTH
#define SUB_WEST  DIR_WEST
#define SUB_UP    DIR_UP
#define SUB_DOWN  DIR_DOWN
#define SUB_NE    DIR_NORTHEAST
#define SUB_NW    DIR_NORTHWEST
#define SUB_SE    DIR_SOUTHEAST
#define SUB_SW    DIR_SOUTHWEST


/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_SEX			 6
#define CON_GET_NEW_CLASS		 7
#define CON_GET_NEW_VT102                8
#define CON_GET_NEW_ANSI                 9 
#define CON_READ_MOTD			 10
#define CON_NOT_PLAYING			 11
#define CON_EDITING			 12
#define CON_COPYOVER_RECOVER             13
#define CON_ANSI                         14
/*
 * Character substates
 */
typedef enum
{
  SUB_NONE, SUB_PAUSE, SUB_PERSONAL_DESC, SUB_OBJ_SHORT, SUB_OBJ_LONG,
  SUB_OBJ_EXTRA, SUB_MOB_LONG, SUB_MOB_DESC, SUB_ROOM_DESC, SUB_ROOM_EXTRA,
  SUB_ROOM_EXIT_DESC, SUB_WRITING_NOTE, SUB_MPROG_EDIT, SUB_HELP_EDIT,
  SUB_WRITING_MAP, SUB_PERSONAL_BIO, SUB_REPEATCMD, SUB_RESTRICTED,
  SUB_DEITYDESC,
  /* timer types ONLY below this point */
  SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;

/*
 * Timer macros.
 */

#define TIMER(ch, tmr)		((ch)->tick_timer[(tmr)])
#define SET_TIMER(ch, tmr, tm)	((ch)->tick_timer[(tmr)] = (tm))
#define ADD_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] += (tm))
#define SUB_TIMER(ch, tmr, tm)  ((ch)->tick_timer[(tmr)] -= (tm))
#define TIME_UP(ch, tmr)	((ch)->tick_timer[(tmr)] == 0 ? TRUE : FALSE)

#define RTIMER(room, rtmr)	 ((room)->tick_timer[(rtmr)])
#define SET_RTIMER(room, rtmr, rtm) ((room)->tick_timer[(rtmr)] = (rtm))
#define ADD_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] += (rtm))
#define SUB_RTIMER(room, rtmr, rtm)  ((room)->tick_timer[(rtmr)] -= (rtm))
#define RTIME_UP(room, rtmr)	 ((room)->tick_timer[(rtmr)] == 0 ? TRUE : FALSE)

#define TIMER_FIGHT_LAG    	23
#define TIMER_CAN_GUST		26
#define TIMER_STASIS            29
#define TIMER_SHADOWPLANE	58
#define TIMER_DROW_DARK         59
#define TIMER_TASER             60
#define TIMER_CAN_SUMMON        61
#define MAX_TIMER               62

#define PULSE_HEALTOUCH        0
#define PULSE_DEATHTOUCH       1
#define PULSE_STASIS           2
#define PULSE_SHADOWPLANE      3
#define PULSE_HEATMETAL        4
#define PULSE_BEAR             5
#define PULSE_GRAB	       6
#define PULSE_DARKNESS	       7
#define MAX_PULSE_TIMER	       8

#define RTIMER_WALL_NORTH	14
#define RTIMER_WALL_EAST	15
#define RTIMER_WALL_SOUTH	16
#define RTIMER_WALL_WEST	17
#define RTIMER_WALL_UP		18
#define RTIMER_WALL_DOWN	19
#define RTIMER_DARK_ROOM	21
#define RTIMER_SILENCE		22
#define MAX_RTIMER		30



/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    DESCRIPTOR_DATA *   infect;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    char *		host;
    char *              mac;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    bool                vt102;
    bool                ansi;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    char *              showstr_head;  /* From ENVY code to compile */
    char *              showstr_point; /* From ENVY code to compile */
    char *		outbuf;
    long		outsize;
    long		outtop; 
    void *              pEdit;          /* OLC */
    char **             pString;        /* OLC */
    int                 editor;         /* OLC */	
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_ALL		    4
#define GOCIAL		    5

/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    HELP_DATA * prev;
    sh_int	level;
    char *	keyword;
    char *	text;
};

/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA *	next;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
};



/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    sh_int		type;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
    int                 level;
};



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/


/* ASCII conversions -- used so we can have letters in this file */

#define BIT_1                 1
#define BIT_2                 2
#define BIT_3                 4
#define BIT_4                 8
#define BIT_5                 16
#define BIT_6                 32
#define BIT_7                 64
#define BIT_8                 128
#define BIT_9                 256
#define BIT_10                512
#define BIT_11                1024
#define BIT_12                2048
#define BIT_13                4096
#define BIT_14                8192
#define BIT_15                16384
#define BIT_16                32768
#define BIT_17                65536
#define BIT_18                131072
#define BIT_19                262144
#define BIT_20                524288
#define BIT_21                1048576
#define BIT_22                2097152
#define BIT_23                4194304
#define BIT_24                8388608
#define BIT_25                16777216
#define BIT_26                33554432
#define BIT_27                67108864      
#define BIT_28                134217728
#define BIT_29                268435456
#define BIT_30                536870912
#define BIT_31                1073741824
#define BIT_32                2147483648
#define BIT_33                4294967256


/*
 * Bits for 'affected_by'.
 * Used in #MOBILES. flags2
 */

#define AFF_REDONE              BIT_26
#define AFF_HOLD                BIT_1

#define AFF_ALLOW_VAMP          BIT_16
#define AFF_ALLOW_WERE          BIT_17
#define AFF_ALLOW_ELAD          BIT_18
#define AFF_ALLOW_DEMON         BIT_19
#define AFF_ALLOW_WARLOCK       BIT_20

#define AFF_DDONE	        BIT_8
#define AFF_DEMONDONE 		BIT_9
#define AFF_FUCK    		BIT_10


#define AFF_BOUNTY		BIT_5
#define AFF_IMPROVED_INVIS      BIT_11
#define AFF_CLAW                BIT_12
#define AFF_BITE                BIT_13
#define AFF_TAIL                BIT_14
#define AFF_WING                BIT_15
#define AFF_MAGIC_DEAF          BIT_21
#define AFF_SHIELD              BIT_22
#define AFF_SHADOW_PLANE        BIT_23
#define AFF_WRENCHED            BIT_24
#define AFF_SHADOW_SIGHT        BIT_25
#define AFF_LAUGHTER            BIT_26
#define AFF_WATER_SHIELD        BIT_27
#define AFF_MASKED              BIT_28
#define AFF_SHAPE_SHIFTED       BIT_29
#define AFF_FAMILIAR            BIT_30

#define AFF3_BLINK_1ST_RD	BIT_1
#define AFF3_BLINK_2ND_RD	BIT_2


/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	   3060
#define MOB_VNUM_VAMPIRE	   3404


/*
 * Immunities, for players.  KaVir.
 */
#define IMM_SLASH	      1	/* Resistance to slash, slice. 		*/
#define IMM_STAB	      2	/* Resistance to stab, pierce. 		*/
#define IMM_SMASH	      4	/* Resistance to blast, pound, crush. 	*/
#define IMM_ANIMAL	      8	/* Resistance to bite, claw. 		*/
#define IMM_MISC	     16	/* Resistance to grep, suck, whip. 	*/
#define IMM_CHARM	     32	/* Immune to charm spell. 		*/
#define IMM_HEAT	     64	/* Immune to fire/heat spells. 		*/
#define IMM_COLD	    128	/* Immune to frost/cold spells.		*/
#define IMM_LIGHTNING	    256	/* Immune to lightning spells.		*/
#define IMM_ACID	    512	/* Immune to acid spells.		*/
#define IMM_SUMMON	   1024	/* Immune to being summoned.		*/
#define IMM_VOODOO	   2048	/* Immune to voodoo magic.		*/
#define IMM_VAMPIRE	   4096	/* Allow yourself to become a vampire.	*/
#define IMM_STAKE	   8192	/* Immune to being staked (vamps only). */
#define IMM_SUNLIGHT	  16384	/* Immune to sunlight (vamps only).	*/
#define IMM_SHIELDED	  32768 /* For Obfuscate. Block scry, etc.	*/
#define IMM_HURL	  65536 /* Cannot be hurled.			*/
#define IMM_BACKSTAB	 131072 /* Cannot be backstabbed.		*/
#define IMM_KICK	 262144 /* Cannot be kicked.			*/
#define IMM_DISARM	 524288 /* Cannot be disarmed.			*/
#define IMM_STEAL	1048576 /* Cannot have stuff stolen.		*/
#define IMM_SLEEP	2097152 /* Immune to sleep spell.		*/
#define IMM_DRAIN       4194304 /* Immune to energy drain.		*/
#define IMM_DEMON	8388608 /* Allow yourself to become a demon.	*/
#define IMM_TRANSPORT  16777216 /* Objects can't be transported to you.	*/
#define IMM_TRAVEL     33554432

/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		    BIT_1	/* Auto set for mobs	*/
#define ACT_SENTINEL		    BIT_2	/* Stays in one room	*/
#define ACT_SCAVENGER		    BIT_3	/* Picks up objects	*/
#define ACT_AGGRESSIVE		    BIT_4	/* Attacks PC's		*/
#define ACT_STAY_AREA		    BIT_5	/* Won't leave area	*/
#define ACT_WIMPY		    BIT_6	/* Flees when hurt	*/
#define ACT_PET			    BIT_7	/* Auto set for pets	*/
#define ACT_TRAIN		    BIT_8	/* Can train PC's	*/
#define ACT_PRACTICE		    BIT_9	/* Can practice PC's	*/
#define ACT_MOUNT		    BIT_10	/* Can be mounted	*/
#define ACT_NOPARTS		    BIT_11	/* Dead = no body parts	*/
#define ACT_NOEXP		    BIT_12	/* No exp for killing   */
#define ACT_PROTOTYPE		    BIT_13

#define POWER_LIFE 1

/* Warps */
#define MAX_WARP 26

#define WARP_CRYSTAL 1 /*crystaline body*/
#define WARP_STEEL   2 /*steel body*/
#define WARP_VENOM   4 /*coated in venom */
#define WARP_SPIKE   8 /*covered in spikes */
#define WARP_QUICK   16 /* extra quick */
#define WARP_SLOW    32 /* extra slow */
#define WARP_TERROR  64 /* opponents randomly flee */
#define WARP_REGEN   128 /* double healing rate */
#define WARP_WEAPON  256 /* weapon master */
#define WARP_INEPT   512 /* lower weapon levels */
#define WARP_WEAK    1024 /* do lower damage */
#define WARP_VULN    2048 /* take extra damage */
#define WARP_SHARDS  4096 /* throw glass shards */
#define WARP_CLUMSY  8192 /* lower dodge/parry */
#define WARP_TENT    16384 /* tentacles */
#define WARP_SLOWHEAL 32768 /* half regen rate */
#define WARP_ARCANE   65536 /* higher magic levels */
#define WARP_NOMAGE   131072 /* low magic level */
#define WARP_STRONG   262144 /* do extra damage */
#define WARP_SMELLY   524288 /* does nothing */
#define WARP_WART     1048576  /* nothing */
#define WARP_FEATHER  2097152 /* nothing */
#define WARP_HAIRY    4194304  /* nothing */
#define WARP_BIGFEET  8388608 /*nohting again */
#define WARP_BALD     16777216 /* nothing */
#define WARP_TAIL     33554432 /*nothing */


/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		      1
#define AFF_INVISIBLE		      2
#define AFF_DETECT_EVIL		      4
#define AFF_DETECT_INVIS	      8
#define AFF_DETECT_MAGIC	     16
#define AFF_DETECT_HIDDEN	     32
#define AFF_SHADOWPLANE		     64 /* Creatures in shadow plane - KaVir */
#define AFF_SANCTUARY		    128
#define AFF_FAERIE_FIRE		    256
#define AFF_INFRARED		    512
#define AFF_CURSE		   1024
#define AFF_FLAMING		   2048 /* For burning creatures - KaVir */
#define AFF_POISON		   4096
#define AFF_PROTECT		   8192
#define AFF_ETHEREAL		  16384 /* For ethereal creatures - KaVir */
#define AFF_SNEAK		  32768
#define AFF_HIDE		  65536
#define AFF_SLEEP		 131072
#define AFF_CHARM		 262144
#define AFF_FLYING		 524288
#define AFF_PASS_DOOR		1048576
#define AFF_POLYMORPH		2097152 /* For polymorphed creatures -KaVir */
#define AFF_SHADOWSIGHT		4194304 /* Can see between planes - KaVir */
#define AFF_WEBBED		8388608 /* Cannot move - KaVir */
#define AFF_TENDRILS           16777216 /* Cannot get pregnant - KaVir */
#define AFF_DROWFIRE           33554432 /* Drow Darkness - Rotain */   
#define AFF_ZULOFORM	       67108864
#define AFF_SHIFT             134217728
#define AFF_HASTE             268435456
#define AFF_INFIRMITY         536870912
#define AFF_GODBLESS         1073741824 /* Monk and Paladin Bless */
#define AFF_DARKNESS       2147483648 /* Total Blind, nothing can penerate */
#define AFF_STEEL_SHIELD            4294967296 
#define MONKFLAMING          8589934592

#define MONK_HASTE          1
#define MONK_SLOW           2
#define MONK_BLIND          4
#define MONK_DIAMONDSKIN    8
#define MONK_IRONSKIN       16
/*
 * Bits for 'itemaffect'.
 * Used in #MOBILES.
 */
#define ITEMA_SHOCKSHIELD	      1
#define ITEMA_FIRESHIELD	      2
#define ITEMA_ICESHIELD		      4
#define ITEMA_ACIDSHIELD	      8
#define ITEMA_DBPASS                 16
#define ITEMA_CHAOSSHIELD            32
#define ITEMA_ARTIFACT               64
#define ITEMA_REGENERATE            128
#define ITEMA_SPEED                 256
#define ITEMA_VORPAL                512
#define ITEMA_PEACE                1024
#define ITEMA_RIGHT_SILVER         2048
#define ITEMA_LEFT_SILVER          4096
#define ITEMA_REFLECT              8192
#define ITEMA_RESISTANCE          16384
#define ITEMA_VISION              32768
#define ITEMA_STALKER             65536
#define ITEMA_VANISH             131072
#define ITEMA_RAGER              262144
#define ITEMA_HIGHLANDER         524288


/* Colour scale macros - added 12th Aug 1995 by Calamar */

#define NO_COLOUR	""		/* Blank */
#define GREY		"[0;1;30m"	/* Dark Grey */
#define D_RED		"[0;0;31m"	/* Dark Red */
#define L_RED		"[0;1;31m"	/* Light Red */
#define D_GREEN		"[0;0;32m"	/* Dark Green */
#define L_GREEN		"[0;1;32m"	/* Light Green */
#define BROWN		"[0;0;33m"	/* Brown */
#define YELLOW		"[0;1;33m"	/* Yellow */
#define D_BLUE		"[0;0;34m"	/* Dark Blue */
#define L_BLUE		"[0;1;34m"	/* Light Blue */
#define MAGENTA		"[0;0;35m"	/* Magenta */
#define PINK		"[0;1;35m"	/* Pink */
#define D_CYAN		"[0;0;36m"	/* Dark Cyan */
#define L_CYAN		"[0;1;36m"	/* Light Cyan */
#define NORMAL		"[0;0;37m"	/* Light Grey */
#define WHITE		"[0;0;38m"	/* White */

#define ADD_COLOUR(_player,_str,_col) {char swh_temp[255]; \
if (!IS_NPC(_player) && IS_SET(_player->act, PLR_ANSI)) {swh_temp[0] = '\0'; \
strcpy(swh_temp, _col); strcat(swh_temp, _str); strcat(swh_temp, NORMAL); \
strcpy(_str, swh_temp);}} 

#define SCALE_COLS 4

#define COL_SCALE(_swh_str,_swh_ch,_swh_curr,_swh_max) \
	ADD_COLOUR(_swh_ch, _swh_str, \
	(_swh_curr < 1) ? L_RED : \
	(_swh_curr < _swh_max) ? \
	scale[(SCALE_COLS * _swh_curr) / ((_swh_max > 0) \
	? _swh_max : 1)] : L_CYAN)

extern char *scale[SCALE_COLS];


/*
 * Zombie Lord.
 */
#define ZOMBIE_NOTHING		      0
#define ZOMBIE_TRACKING		      1
#define ZOMBIE_ANIMATE		      2
#define ZOMBIE_CAST		      3
#define ZOMBIE_REST		      4


/* return values for check_imm */
#define IS_NORMAL               0
#define IS_DIMMUNE              1
#define IS_RESISTANT            2
#define IS_VULNERABLE           3

/* damage classes */
#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4
#define DAM_COLD                5
#define DAM_LIGHTNING           6
#define DAM_ACID                7
#define DAM_POISON              8
#define DAM_NEGATIVE            9
#define DAM_HOLY                10
#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT               15
#define DAM_OTHER               16
#define DAM_HARM                17
#define DAM_CHARM               18
#define DAM_SOUND               19

/* IMM bits for mobs */
#define DIMM_SUMMON              BIT_1
#define DIMM_CHARM               BIT_2
#define DIMM_MAGIC               BIT_3
#define DIMM_WEAPON              BIT_4
#define DIMM_BASH                BIT_5
#define DIMM_PIERCE              BIT_6
#define DIMM_SLASH               BIT_7
#define DIMM_FIRE                BIT_8
#define DIMM_COLD                BIT_9
#define DIMM_LIGHTNING           BIT_10
#define DIMM_ACID                BIT_11
#define DIMM_POISON              BIT_12
#define DIMM_NEGATIVE            BIT_13
#define DIMM_HOLY                BIT_14
#define DIMM_ENERGY              BIT_15
#define DIMM_MENTAL              BIT_16
#define DIMM_DISEASE             BIT_17
#define DIMM_DROWNING            BIT_18
#define DIMM_LIGHT               BIT_19
#define DIMM_SOUND               BIT_20
#define DIMM_WOOD                BIT_24
#define DIMM_SILVER              BIT_25
#define DIMM_IRON                BIT_26
 
/* RES bits for mobs */
#define RES_SUMMON              BIT_1
#define RES_CHARM               BIT_2
#define RES_MAGIC               BIT_3
#define RES_WEAPON              BIT_4
#define RES_BASH                BIT_5
#define RES_PIERCE              BIT_6
#define RES_SLASH               BIT_7
#define RES_FIRE                BIT_8
#define RES_COLD                BIT_9
#define RES_LIGHTNING           BIT_10
#define RES_ACID                BIT_11
#define RES_POISON              BIT_12
#define RES_NEGATIVE            BIT_13
#define RES_HOLY                BIT_14
#define RES_ENERGY              BIT_15
#define RES_MENTAL              BIT_16
#define RES_DISEASE             BIT_17
#define RES_DROWNING            BIT_18
#define RES_LIGHT               BIT_19
#define RES_SOUND               BIT_20
#define RES_WOOD                BIT_24
#define RES_SILVER              BIT_25
#define RES_IRON                BIT_26
 
/* VULN bits for mobs */
#define VULN_SUMMON             BIT_1
#define VULN_CHARM              BIT_2
#define VULN_MAGIC              BIT_3
#define VULN_WEAPON             BIT_4
#define VULN_BASH               BIT_5
#define VULN_PIERCE             BIT_6
#define VULN_SLASH              BIT_7
#define VULN_FIRE               BIT_8
#define VULN_COLD               BIT_9
#define VULN_LIGHTNING          BIT_10
#define VULN_ACID               BIT_11
#define VULN_POISON             BIT_12
#define VULN_NEGATIVE           BIT_13
#define VULN_HOLY               BIT_14
#define VULN_ENERGY             BIT_15
#define VULN_MENTAL             BIT_16
#define VULN_DISEASE            BIT_17
#define VULN_DROWNING           BIT_18
#define VULN_LIGHT              BIT_19
#define VULN_SOUND              BIT_20
#define VULN_WOOD               BIT_24
#define VULN_SILVER             BIT_25
#define VULN_IRON               BIT_26


/*
 * Bits for Demonic Champions.
 *
 * new demon fields in player.h and old ones in old.h
 */


/*
 * Mounts
 */
#define IS_ON_FOOT		      0
#define IS_MOUNT		      1
#define IS_RIDING		      2
#define IS_CARRIED		      4
#define IS_CARRYING		      8




/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2



/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3

#define OBJ_VNUM_BROD		     30073
#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_FINAL_TURD	     16

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_BLOOD_SPRING	     23

#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716

/* For KaVir's stuff */
#define OBJ_VNUM_SOULBLADE	  30000
#define OBJ_VNUM_GATE             30042
#define OBJ_VNUM_GATE2		  30072
#define OBJ_VNUM_PORTAL		  30001
#define OBJ_VNUM_EGG		  30002
#define OBJ_VNUM_EMPTY_EGG	  30003
#define OBJ_VNUM_SPILLED_ENTRAILS 30004
#define OBJ_VNUM_QUIVERING_BRAIN  30005
#define OBJ_VNUM_SQUIDGY_EYEBALL  30006
#define OBJ_VNUM_SPILT_BLOOD      30007
#define OBJ_VNUM_VOODOO_DOLL      30010
#define OBJ_VNUM_RIPPED_FACE      30012
#define OBJ_VNUM_TORN_WINDPIPE    30013
#define OBJ_VNUM_CRACKED_HEAD     30014
#define OBJ_VNUM_SLICED_EAR	  30025
#define OBJ_VNUM_SLICED_NOSE	  30026
#define OBJ_VNUM_KNOCKED_TOOTH	  30027
#define OBJ_VNUM_TORN_TONGUE	  30028
#define OBJ_VNUM_SEVERED_HAND	  30029
#define OBJ_VNUM_SEVERED_FOOT	  30030
#define OBJ_VNUM_SEVERED_THUMB	  30031
#define OBJ_VNUM_SEVERED_INDEX	  30032
#define OBJ_VNUM_SEVERED_MIDDLE	  30033
#define OBJ_VNUM_SEVERED_RING	  30034
#define OBJ_VNUM_SEVERED_LITTLE	  30035
#define OBJ_VNUM_SEVERED_TOE	  30036
#define OBJ_VNUM_PROTOPLASM	  30037
#define OBJ_VNUM_QUESTCARD	  30039
#define OBJ_VNUM_QUESTMACHINE	  30040
#define OBJ_VNUM_COPPER           30049
#define OBJ_VNUM_IRON             30050
#define OBJ_VNUM_STEEL            30051
#define OBJ_VNUM_ADAMANTITE       30052
#define OBJ_VNUM_BOMB             4
#define OBJ_VNUM_MINE             5
#define OBJ_VNUM_MISSLE           6

#define MOB_VNUM_GUARDIAN	  30001
#define MOB_VNUM_SERVANT          30002
#define MOB_VNUM_MOUNT		  6
#define MOB_VNUM_FROG		  7
#define MOB_VNUM_RAVEN		  8
#define MOB_VNUM_CAT		  9
#define MOB_VNUM_DOG		  10
#define MOB_VNUM_EYE		  12
#define MOB_VNUM_SATAN            30003
#define MOB_VNUM_DEMON		  30005
#define MOB_VNUM_SERPENT	  30006

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PORTAL		     27
#define ITEM_EGG		     28
#define ITEM_VOODOO		     29
#define ITEM_STAKE		     30
#define ITEM_MISSILE		     31 /* Ammo vnum, cur, max, type */
#define ITEM_AMMO		     32 /* ???, dam min, dam max, type */
#define ITEM_QUEST		     33
#define ITEM_QUESTCARD		     34
#define ITEM_QUESTMACHINE	     35
#define ITEM_SYMBOL		     36
#define ITEM_BOOK		     37
#define ITEM_PAGE		     38
#define ITEM_TOOL		     39
#define ITEM_WALL                    40
#define ITEM_COPPER                  41
#define ITEM_IRON		     42
#define ITEM_STEEL		     43
#define ITEM_ADAMANTITE		     44
#define ITEM_GEMSTONE                45
#define ITEM_HILT                    46
#define ITEM_ARTIFACT                47
#define ITEM_BOMB                    48
#define ITEM_GEM                     49
#define ITEM_ORE                     50
#define ITEM_METAL                   51

/*
 * Weapon Stats
 */

#define WEAPON_FLAMING          BIT_1
#define WEAPON_FROST            BIT_2
#define WEAPON_VAMPIRIC         BIT_3
#define WEAPON_SHARP            BIT_4
#define WEAPON_VORPAL           BIT_5
#define WEAPON_TWO_HANDS        BIT_6
#define WEAPON_SHOCKING         BIT_7
#define WEAPON_POISON           BIT_8
#define WEAPON_SUNBLADE         BIT_9
#define WEAPON_DRAGONLANCE      BIT_10
#define WEAPON_SILVER           BIT_11
#define WEAPON_RUNE_FORCE_BOLT  BIT_12
#define WEAPON_RUNE_SMITE_EVIL  BIT_13
#define WEAPON_RUNE_BLAZE       BIT_14
#define WEAPON_RUNE_LIGHTNING   BIT_15
#define WEAPON_RUNE_DANCING     BIT_16
#define WEAPON_ELE_FLAME        BIT_17
#define WEAPON_ELE_WATER        BIT_18
#define WEAPON_ELE_EARTH        BIT_19
#define WEAPON_ELE_AIR          BIT_20


/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		      1
#define ITEM_HUM		      2
#define ITEM_THROWN		      4
#define ITEM_KEEP		      8
#define ITEM_VANISH		     16
#define ITEM_INVIS		     32
#define ITEM_MAGIC		     64
#define ITEM_NODROP		    128
#define ITEM_BLESS		    256
#define ITEM_ANTI_GOOD		    512
#define ITEM_ANTI_EVIL		   1024
#define ITEM_ANTI_NEUTRAL	   2048
#define ITEM_NOREMOVE		   4096
#define ITEM_INVENTORY		   8192
#define ITEM_LOYAL		  16384
#define ITEM_SHADOWPLANE	  32768
#define ITEM_FORGED               65536
#define ITEM_PREPARED            131072
#define FORGE_COPPER             262144
#define  FORGE_BRONZE            524288
#define FORGE_IRON              1048576
#define FORGE_STEEL             2097152
#define ITEM_ONE_RING		4194304
#define ITEM_LIGHT_SABER	8388608
#define ITEM_PROTOTYPE		 BIT_12
#define ITEM_FIXED_DUR		 BIT_25

/* artifact and relic flags */

#define ITEM_TELEPORTS		   BIT_23  /* teleports upon owner death */
#define ITEM_DESTROYED		   BIT_24  /* destroyed upon owner death */
#define ITEM_UNIQUE		   BIT_25  /* only 1 in game ever */
#define ITEM_DESIRED		   BIT_26  /* you can't let go of it ever */
#define ITEM_INDESTRUCTABLE	   BIT_27  /* guess */
#define ITEM_TELEPORT_PROTECTION   BIT_28  /* teleports when attempts to destroy it */
#define ITEM_KNOW_OWNER 	   BIT_29  /* owner displayed on artifact command */

#define ITEM_FLYING	           BIT_30
#define ITEM_FORM_MELDED	   BIT_31



/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		      1
#define ITEM_WEAR_FINGER	      2
#define ITEM_WEAR_NECK		      4
#define ITEM_WEAR_BODY		      8
#define ITEM_WEAR_HEAD		     16
#define ITEM_WEAR_LEGS		     32
#define ITEM_WEAR_FEET		     64
#define ITEM_WEAR_HANDS		    128 
#define ITEM_WEAR_ARMS		    256
#define ITEM_WEAR_SHIELD	    512
#define ITEM_WEAR_ABOUT		   1024 
#define ITEM_WEAR_WAIST		   2048
#define ITEM_WEAR_WRIST		   4096
#define ITEM_WIELD		   8192
#define ITEM_HOLD		  16384
#define ITEM_WEAR_FACE		  32768



/*
 * Special types.
 * Used in #OBJECTS for special items - KaVir.
 */
#define SITEM_ACTIVATE		      1
#define SITEM_TWIST		      2
#define SITEM_PRESS		      4
#define SITEM_PULL		      8
#define SITEM_TARGET		      16
#define SITEM_SPELL		      32
#define SITEM_TRANSPORTER	      64
#define SITEM_TELEPORTER	      128
#define SITEM_DELAY1		      256
#define SITEM_DELAY2		      512
#define SITEM_OBJECT		     1024
#define SITEM_MOBILE		     2048
#define SITEM_ACTION		     4096
#define SITEM_MORPH		     8192
#define SITEM_SILVER		    16384
#define SITEM_WOLFWEAPON	    32768
#define SITEM_DROW		    65536
#define SITEM_CHAMPWEAPON	   131072
#define SITEM_DEMONIC		   262144
#define SITEM_HIGHLANDER	   524288
#define SITEM_COPPER              1048576
#define SITEM_IRON		  2097152
#define SITEM_STEEL		  4194304
#define SITEM_ADAMANTITE	  8388608
#define SITEM_GEMSTONE           16777216
#define SITEM_HILT               33554432
#define SITEM_PDEMONIC		 67108864
#define SITEM_MONK		134217728
#define SITEM_NINJA		268435456
/*
 * Apply types (for quest affects).
 * Used in #OBJECTS.
 */
#define QUEST_STR		      1
#define QUEST_DEX		      2
#define QUEST_INT		      4
#define QUEST_WIS		      8
#define QUEST_CON		     16
#define QUEST_HITROLL		     32
#define QUEST_DAMROLL		     64
#define QUEST_HIT		    128
#define QUEST_MANA		    256
#define QUEST_MOVE		    512
#define QUEST_AC		   1024

#define QUEST_NAME		   2048
#define QUEST_SHORT		   4096
#define QUEST_LONG		   8192
#define QUEST_FREENAME		  16384

#define QUEST_ENCHANTED		  32768
#define QUEST_SPELLPROOF	  65536
#define QUEST_ARTIFACT		 131072
#define QUEST_IMPROVED		 262144
#define QUEST_MASTER_RUNE	 524288
#define QUEST_RELIC             1048576
#define QUEST_BLOODA            2097152
#define QUEST_CLONED            4194304
#define QUEST_ZOMBIE            8388608
#define QUEST_FORGE            16777216


/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_POLY		     25



/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8



/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		   1200
#define ROOM_VNUM_TEMPLE	   3001
#define ROOM_VNUM_ALTAR		   3054
#define ROOM_VNUM_SCHOOL	   3700
#define ROOM_VNUM_HELL		      6
#define ROOM_VNUM_CRYPT		  30001
#define ROOM_VNUM_DISCONNECTION	      2
#define ROOM_VNUM_IN_OBJECT	  30008
#define ROOM_VNUM_AWINNER            70
#define ROOM_VNUM_ALOSER             69
#define ROOM_VNUM_CAINE		  27000
#define ROOM_VNUM_DEVOUR	  30006

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		      1
#define ROOM_NO_OTRANS		      2
#define ROOM_NO_MOB		      4
#define ROOM_INDOORS		      8
#define ROOM_PRIVATE		    512
#define ROOM_SAFE		   1024
#define ROOM_SOLITARY		   2048
#define ROOM_PET_SHOP		   4096
#define ROOM_NO_RECALL		   8192
#define ROOM_NO_TELEPORT	  16384
#define ROOM_TOTAL_DARKNESS       32768
#define ROOM_BLADE_BARRIER        65536
#define ROOM_ARENA               131072
#define ROOM_FLAMING             262144
#define ROOM_SILENCE             524288
#define ROOM_EXPLODED           1048576
#define ROOM_PROTOTYPE		 BIT_16
/*
 * Room text flags (KaVir).
 * Used in #ROOMS.
 */
#define RT_LIGHTS		      1 /* Toggles lights on/off */
#define RT_SAY			      2 /* Use this if no others powers */
#define RT_ENTER		      4
#define RT_CAST			      8
#define RT_THROWOUT		     16 /* Erm...can't remember ;) */
#define RT_OBJECT		     32 /* Creates an object */
#define RT_MOBILE		     64 /* Creates a mobile */
#define RT_LIGHT		    128 /* Lights on ONLY */
#define RT_DARK			    256 /* Lights off ONLY */
#define RT_OPEN_LIFT		    512 /* Open lift */
#define RT_CLOSE_LIFT		   1024 /* Close lift */
#define RT_MOVE_LIFT		   2048 /* Move lift */
#define RT_SPELL		   4096 /* Cast a spell */
#define RT_PORTAL		   8192 /* Creates a one-way portal */
#define RT_TELEPORT		  16384 /* Teleport player to room */

#define RT_ACTION		  32768
#define RT_BLANK_1		  65536
#define RT_BLANK_2		 131072

#define RT_RETURN		1048576 /* Perform once */
#define RT_PERSONAL		2097152 /* Only shows message to char */
#define RT_TIMER		4194304 /* Sets object timer to 1 tick */


/* area flags */
#define UNDER_WATER                   1

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      BIT_1
#define EX_CLOSED		      BIT_2
#define EX_LOCKED		      BIT_3
#define EX_PICKPROOF		      BIT_6
#define EX_NOPASS                     BIT_7
#define EX_EASY                       BIT_8
#define EX_HARD                       BIT_9
#define EX_INFURIATING                BIT_10
#define EX_NOCLOSE                    BIT_11
#define EX_NOLOCK                     BIT_12
#define EX_ICE_WALL                   BIT_13
#define EX_FIRE_WALL                  BIT_14
#define EX_SWORD_WALL                 BIT_15
#define EX_PRISMATIC_WALL             BIT_16
#define EX_IRON_WALL                  BIT_17
#define EX_MUSHROOM_WALL              BIT_18
#define EX_CALTROP_WALL               BIT_19
#define EX_ASH_WALL                   BIT_20
#define EX_WARDING                    BIT_21
#define EX_ENERGYNET_WALL	      BIT_22

#define MAX_EXFLAG		      20
#define MAX_WALL		      10



/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNUSED		      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_MAX		     11



/*
 * Equipment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FINGER_L		      1
#define WEAR_FINGER_R		      2
#define WEAR_NECK_1		      3
#define WEAR_NECK_2		      4
#define WEAR_BODY		      5
#define WEAR_HEAD		      6
#define WEAR_LEGS		      7
#define WEAR_FEET		      8
#define WEAR_HANDS		      9
#define WEAR_ARMS		     10
#define WEAR_SHIELD		     11
#define WEAR_ABOUT		     12
#define WEAR_WAIST		     13
#define WEAR_WRIST_L		     14
#define WEAR_WRIST_R		     15
#define WEAR_WIELD		     16
#define WEAR_HOLD		     17
#define WEAR_THIRD		     18
#define WEAR_FOURTH		     19
#define WEAR_FACE		     20
#define WEAR_SCABBARD_L		     21
#define WEAR_SCABBARD_R		     22
#define MAX_WEAR		     23




/*
 * Locations for damage.
 */
#define LOC_HEAD		      0
#define LOC_BODY		      1
#define LOC_ARM_L		      2
#define LOC_ARM_R		      3
#define LOC_LEG_L		      4
#define LOC_LEG_R		      5

/*
 * For Head
 */
#define LOST_EYE_L		       1
#define LOST_EYE_R		       2
#define LOST_EAR_L		       4
#define LOST_EAR_R		       8
#define LOST_NOSE		      16
#define BROKEN_NOSE		      32
#define BROKEN_JAW		      64
#define BROKEN_SKULL		     128
#define LOST_HEAD		     256
#define LOST_TOOTH_1		     512 /* These should be added..... */
#define LOST_TOOTH_2		    1024 /* ...together to caculate... */
#define LOST_TOOTH_4		    2048 /* ...the total number of.... */
#define LOST_TOOTH_8		    4096 /* ...teeth lost.  Total..... */
#define LOST_TOOTH_16		    8192 /* ...possible is 31 teeth.   */
#define LOST_TONGUE		   16384

/*
 * For Body
 */
#define BROKEN_RIBS_1		       1 /* Remember there are a total */
#define BROKEN_RIBS_2		       2 /* of 12 pairs of ribs in the */
#define BROKEN_RIBS_4		       4 /* human body, so not all of  */
#define BROKEN_RIBS_8		       8 /* these bits should be set   */
#define BROKEN_RIBS_16		      16 /* at the same time.          */
#define BROKEN_SPINE		      32
#define BROKEN_NECK		      64
#define CUT_THROAT		     128
#define CUT_STOMACH		     256
#define CUT_CHEST		     512

/*
 * For Arms
 */
#define BROKEN_ARM		       1
#define LOST_ARM		       2
#define LOST_HAND		       4
#define LOST_FINGER_I		       8 /* Index finger */
#define LOST_FINGER_M		      16 /* Middle finger */
#define LOST_FINGER_R		      32 /* Ring finger */
#define LOST_FINGER_L		      64 /* Little finger */
#define LOST_THUMB		     128
#define BROKEN_FINGER_I		     256 /* Index finger */
#define BROKEN_FINGER_M		     512 /* Middle finger */
#define BROKEN_FINGER_R		    1024 /* Ring finger */
#define BROKEN_FINGER_L		    2048 /* Little finger */
#define BROKEN_THUMB		    4096

/*
 * For Legs
 */
#define BROKEN_LEG		       1
#define LOST_LEG		       2
#define LOST_FOOT		       4
#define LOST_TOE_A		       8
#define LOST_TOE_B		      16
#define LOST_TOE_C		      32
#define LOST_TOE_D		      64 /* Smallest toe */
#define LOST_TOE_BIG		     128
#define BROKEN_TOE_A		     256
#define BROKEN_TOE_B		     512
#define BROKEN_TOE_C		    1024
#define BROKEN_TOE_D		    2048 /* Smallest toe */
#define BROKEN_TOE_BIG		    4096

/*
 * For Bleeding
 */
#define BLEEDING_HEAD		       1
#define BLEEDING_THROAT		       2
#define BLEEDING_ARM_L		       4
#define BLEEDING_ARM_R		       8
#define BLEEDING_HAND_L		      16
#define BLEEDING_HAND_R		      32
#define BLEEDING_LEG_L		      64
#define BLEEDING_LEG_R		     128
#define BLEEDING_FOOT_L		     256
#define BLEEDING_FOOT_R		     512





/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2


/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_MEDITATING		      5
#define POS_SITTING		      6
#define POS_RESTING		      7
#define POS_FIGHTING                  8
#define POS_STANDING                  9
/* Status of Arena */

#define FIGHT_OPEN                    0
#define FIGHT_START                   1
#define FIGHT_BUSY                    2
#define FIGHT_LOCK                    3
#define ARENA_NORMAL                  0
#define ARENA_CLAN                    1
#define ARENA_STATUS                  2

/*
 * ACT bits for players.
 */
/*
#define PLR_EMBRACING		      4	
#define PLR_EMBRACED		    256	
Seriosly bugged bits for some reason dont enable them they arent used*/
#define PLR_IS_NPC		      1		/* Don't EVER set.	*/
#define PLR_MXP			      2
#define PLR_NEW_HELP		      4
#define PLR_AUTOEXIT		      8
#define PLR_AUTOLOOT		     16
#define PLR_AUTOSAC                  32
#define PLR_BLANK		     64
#define PLR_BRIEF		    128
#define PLR_COL_SCALE		    256
#define PLR_COMBINE		    512
#define PLR_PROMPT		   1024
#define PLR_TELNET_GA		   2048

#define PLR_HOLYLIGHT		   4096
#define PLR_WIZINVIS		   8192
#define PLR_ANSI		  16384

#define	PLR_SILENCE		  32768
#define PLR_VT102       	  65536
#define PLR_INCOG                131072
#define PLR_NO_TELL		 262144
#define PLR_LOG			 524288
#define PLR_DENY		1048576
#define PLR_FREEZE		2097152

#define PLR_SHADOWSIGHT		4194304
#define PLR_MSP                 8388608 

#define PLR_WATCHER	       33554432
#define PLR_NO_GROUP           67108864
#define PLR_CHALLENGER        134217728
#define PLR_CHALLENGED        268435456
#define PLR_SACREDINVIS	      536870912
#define PLR_WAS_AVATAR       1073741824

/*New bits for playrs (Infidel)*/

#define NEW_CLOAK     32768
#define NEW_DARKNESS  131072
#define THIRD_HAND    262144
#define FOURTH_HAND   524288
#define NEW_SETLOG    2097152
#define NEW_SETEXIT   4194304
#define NEW_SETDECAP  8388608
#define HAS_HADES    16777216

/* Clan bits */
#define CLAN_LDR      1
#define CLAN_CO       2
#define CLAN_TRUSTED  4

/*
 * EXTRA bits for players. (KaVir)
 */
#define EXTRA_ZOMBIE		      2
#define EXTRA_TRUSTED		      4
#define EXTRA_NEWPASS		      8
#define EXTRA_OSWITCH		     16
#define EXTRA_SWITCH		     32
#define TIED_UP			    128
#define GAGGED			    256
#define BLINDFOLDED		    512
#define EXTRA_PROMPT		  65536
#define EXTRA_CALL_ALL		1048576
#define EXTRA_NOTE_TRUST        4194304
#define EXTRA_ROT		BIT_28

/*
 * Stances for combat
 */
#define STANCE_NONE		     -1
#define STANCE_NORMAL		      0
#define STANCE_VIPER		      1
#define STANCE_CRANE		      2
#define STANCE_CRAB		      3
#define STANCE_MONGOOSE		      4
#define STANCE_BULL		      5
#define STANCE_MANTIS		      6
#define STANCE_DRAGON		      7
#define STANCE_TIGER		      8
#define STANCE_MONKEY		      9
#define STANCE_SWALLOW		     10
#define STANCE_WOLF                  11


/*
 * Channel bits.
 */
#define	CHANNEL_CLAN3		  1
#define	CHANNEL_CHAT		      2
#define CHANNEL_CLAN1			  4
#define	CHANNEL_IMMTALK		      8
#define	CHANNEL_MUSIC		     16
#define	CHANNEL_QUESTION	     32
#define	CHANNEL_SHOUT		     64
#define	CHANNEL_YELL		    128
#define	CHANNEL_CLAN2    	    256
#define	CHANNEL_CLAN5		    512
#define	CHANNEL_LOG                1024
#define	CHANNEL_PRAY		   2048
#define	CHANNEL_INFO		   4096
#define CHANNEL_NEWTALK	           8192
#define	CHANNEL_TELL		  16384
#define	CHANNEL_MAGETALK          32768
#define CHANNEL_HIGHTALK	  65536
#define CHANNEL_CLAN		 131072
#define CHANNEL_SIGN		 262144
#define	CHANNEL_MONKTALK         524288
#define CHANNEL_FORCETALK	1048576
#define CHANNEL_BELLOW          BIT_22
#define CHANNEL_GODTALK          BIT_23
#define CHANNEL_CLAN4            BIT_24 
#define CHANNEL_CLAN6		BIT_25
#define CHANNEL_CLAN7		BIT_26
/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    char *		hunting;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		lord;
    char *		clan;
    char *		morph;
    char *		createtime;
    char *		pload;
    char *		lasttime;
    char *		lasthost;
    char *		powertype;
    char *		poweraction;
    char *		prompt;
    char *		cprompt;
    sh_int		spectype;
    sh_int		specpower;
    int		        loc_hp	[7];
    int			vnum;
    sh_int		count;
    sh_int		killed;
    sh_int		sex;
    sh_int		mounted;
    sh_int		home;
    sh_int              recall2;
    int		        level;
    int			immune;
    int			polyaff;
    int			vampaff;
    int			itemaffect;
    int			vamppass;
    int			form;
    int			act;
    int			extra;
    unsigned long	affected_by;
    unsigned long       aff2;
    sh_int		alignment;
    sh_int		hitroll;		/* Unused */
    sh_int		ac;			/* Unused */
    sh_int		hitnodice;		/* Unused */
    sh_int		hitsizedice;		/* Unused */
    sh_int		hitplus;		/* Unused */
    sh_int		damnodice;		/* Unused */
    sh_int		damsizedice;		/* Unused */
    sh_int		damplus;		/* Unused */
    int			gold;			/* Unused */
   /*int                 special;
    int                 class; */
};

struct balance_data
{
    int			pkills;
    int			pkilled;
    int			members;
    int			total_score;
    int			position;
    int			total_gen;
    int			num_tertiary;
    int			total_alignment;
};

struct	editor_data
{
    sh_int		numlines;
    sh_int		on_line;
    sh_int		size;
    char		line[49][81];
};

struct class_data
{
 int	armo;
 int	atta;
 int	damr;
 int	dodg;
 int	hitr;
 int	parr;
 float	powe;
 float	rege;
 float	toug;
};

struct config_data
{
    float               ap_exp_mult;
    int                 ap_exp_points;
    int			mob_hp_mult;
    int			mob_hp_div;
    int			mob_level_mult;
    int			mob_level_div;
    int			mob_dam_mult;
    int			mob_dam_div;
    int			mob_ac_mult;
    int			mob_ac_div;
    int			prac_exp_mult;
    int			prac_exp_div;
    int			research_exp_mult;
    int			research_exp_div;
    int			research_time_mult;
    int			research_time_div;
    int			train_exp_mult;
    int			train_exp_div;
    int			max_players;
    char		*autohelp;
    char		*title_none;
    char		*title00;
    char		*title05;
    char		*title10;
    char		*title15;
    char		*title20;
    char		*title25;
    char		*title30;
    char		*title35;
    char		*title40;
    char                *title45;
    char		*title50;
    char                *imm07;
    char                *imm08;
    char                *imm09;
    char                *imm10;
    char                *imm11;
    char                *imm12;
};



/*
 * One character (PC or NPC).
 */
struct	char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		prev_in_room;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *         embracing;
    CHAR_DATA *         embraced;
    CHAR_DATA *		blinkykill;
    CHAR_DATA *		reply;
    CHAR_DATA *		mount;
    CHAR_DATA *		wizard;
    CHAR_DATA *         challenger; /*  person who challenged you */
    CHAR_DATA *         challenged; /*  person who you challenged */
    CHAR_DATA *         gladiator;  /*  ARENA player wagered on */
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;
    NOTE_DATA *		pnote;
    OBJ_DATA *		carrying;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    PC_DATA *		pcdata;
    DO_FUN *		last_cmd;
    DO_FUN *		prev_cmd;   /* mapping */
    int                 clan_rank;
    sh_int              ABlock[3];
    sh_int              BBlock[3];
    sh_int              CBlock[3];
    bool                stasis;
    char *		hunting;
    char *		name;
    char *	        pload;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		clan;
    char *		createtime;
    char *		setlog;
    char *		setexit;
    char *		lasttime;
    char *		lasthost;
    char *		prompt;
    char *		cprompt;
    sh_int		sex;
    sh_int		race;
    sh_int		class;
    int			immune;
    int			fight_timer;
    int                 login_timer;
    int			itemaffect;
    int			warp;
    int			explevel;
    int			expgained;
    int			levelexp;
    int                 paintball[3];
    int                 smelt[4];
    int			stuntimer;
/* SMAUUUUUUUUUUUG */
    void *		dest_buf;
    void *		spare_ptr;
    int			tempnum;
    EDITOR_DATA *	editor;
    sh_int		substate;
    int			pagelen;                        /* BUILD INTERFACE */
    sh_int		inter_page;                     /* BUILD INTERFACE */
    sh_int		inter_type;                     /* BUILD INTERFACE */
    char  		*inter_editing;                 /* BUILD INTERFACE */
    int			inter_editing_vnum;             /* BUILD INTERFACE */
    sh_int		inter_substate;                 /* BUILD INTERFACE */
/* End O' Smaug */
    sh_int 	        generation;
    sh_int		rage;
    sh_int		tick_timer[MAX_TIMER];
    sh_int		warpcount;
    sh_int		loc_hp	[7];
    sh_int		wpn	[13];
    sh_int		spl	[5];
    sh_int		cmbt	[8];
    sh_int		stance	[11];
    sh_int		mounted;
    sh_int		home;
    sh_int              recall2;
    sh_int		level;
    sh_int		trust;
    int			played;
    time_t		logon;
    time_t		save_time;
    sh_int		timer;
    sh_int		wait;
    int			pkill;
    int			pdeath;
    int			mkill;
    int			mdeath;
    int		        hit;
    int		        max_hit;
    int			mana;
    int			max_mana;
    int			move;
    int			max_move;
    int			exp;
    int			act;
    int			extra;
    int                 newbits;
    int                 clanbits;
    int                 special;
    int			affected_by;
    int                 aff2;
    sh_int		position;
    sh_int		practice;
    sh_int		carry_weight;
    sh_int		carry_number;
    sh_int		saving_throw;
    sh_int		alignment;
    sh_int		hitroll;
    sh_int		damroll;
    sh_int		armor;
    sh_int		wimpy;
    sh_int		deaf;
    sh_int		paradox		[3];
    char		*morph;
};



/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    OBJ_DATA *		familiar;
    OBJ_DATA *		chobj;
    RACE_DATA *         race;
    int                 radio;
    int                 version;
    char *              held;
    char *              held_by;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
    char *              who;
    char *	 	setlog;
    char *		setexit;
    char *		forget[MAX_FORGET];
    char *		switchname;
    char *        	custom_enter_room;
    char *        	enter_room;
    char *		subprompt;	/* Substate prompt */
    sh_int		souls;
    sh_int		perm_str;
    sh_int		perm_int;
    sh_int		perm_wis;
    sh_int		perm_dex;
    sh_int		perm_con;
    sh_int		mod_str;
    sh_int		mod_int;
    sh_int		mod_wis;
    sh_int		mod_dex;
    sh_int		mod_con;
    sh_int              remort;
    int			quest;
    int                 gentick;
    int			powers[35];
    int			askills[20];
    int 		stats[12];
    int			pulse_timer[MAX_PULSE_TIMER];
    bool                lwrgen;
    sh_int		wolf;
    sh_int		rank;
    int			obj_vnum;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int              followers;
    int                 plr_wager; /*  ARENA amount wagered     */
    int                 awins;     /*  ARENA number of wins     */
    int                 alosses;   /*  ARENA number of losses  */
    int			comm;
    int			exp_multiplyer; /* for newbie exp multiplyer */
    sh_int              bounty[2];
    char *              decap_mess;
    int                 c_chal;
    int			apoints;
    char *		target;
};



/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		16

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    EXTRA_DESCR_DATA *prev;	/* Previous			    */
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};



/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect; 
    AFFECT_DATA *	affected;
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    int			vnum;
    sh_int		item_type;
    sh_int		extra_flags;
    sh_int		extra_flags2;
    sh_int		wear_flags;
    sh_int		count;
    sh_int		weight;
    sh_int		weapflags;
    int			spectype;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int			quest;
    sh_int		points;
    int			cost;			/* Unused */
    int			value	[4];
    int			durability;
};



/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    CHAR_DATA *		carried_by;
    CHAR_DATA *		chobj;
    CHAR_DATA *         armed_by;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AFFECT_DATA *	first_affect;
    AFFECT_DATA *	last_affect;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    sh_int              gem_count;
    char *		name;
    char *		short_descr;
    char *		description;
    char *		chpoweron;
    char *		chpoweroff;
    char *		chpoweruse;
    char *		victpoweron;
    char *		victpoweroff;
    char *		victpoweruse;
    char *		questmaker;
    char *		questowner;
    sh_int		item_type;
    int	         	extra_flags;
    sh_int		extra_flags2;
    sh_int		wear_flags;
    sh_int		wear_loc;
    sh_int		weight;
    sh_int		weapflags;
    int			spectype;
    int			specpower;
    sh_int		condition;
    sh_int		toughness;
    sh_int		resistance;
    int			quest;
    sh_int		points;
    int			cost;
    sh_int		level;
    sh_int		timer;
    int			value	[4];
    int			durability;
};
#define BOMB_MINE 1


/*
 * Exit data.
 */
struct	exit_data
{
    EXIT_DATA *		prev;		/* previous exit in linked list	*/
    EXIT_DATA *		next;		/* next exit in linked list	*/
    EXIT_DATA *		rexit;		/* Reverse exit pointer		*/
    ROOM_INDEX_DATA *	to_room;	/* Pointer to destination room	*/
    char *		keyword;	/* Keywords for exit or door	*/
    char *		description;	/* Description of exit		*/
    int			vnum;		/* Vnum of room exit leads to	*/
    int			rvnum;		/* Vnum of room in opposite dir	*/
    int			exit_info;	/* door states & other flags	*/
    int			key;		/* Key vnum			*/
    sh_int		vdir;		/* 0,5 N\E\S\W\U\D shit		*/
};



/*
 * Room text checking data.
 */
typedef struct roomtext_data
{
    int				type;
    int				power;
    int				mob;
    char *			input;
    char *			output;
    char *			choutput;
    char *			name;
    struct roomtext_data	*next;
} ROOMTEXT_DATA;



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    sh_int		arg1;
    sh_int		arg2;
    sh_int		arg3;
};



/*
 * Area definition.
 */
struct  area_data
{
    AREA_DATA *		next;
    RESET_DATA *	reset_first;
    RESET_DATA *	reset_last;
    int                 hi_vnum;
    int                 lo_vnum;
    char *		name;
    sh_int		age;
    sh_int		nplayer;
    int                 flags;
    char *		filename;
    AFFECT_DATA *       affected;
    int                 affected_by;
};



/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    ROOM_INDEX_DATA *   next_room;
    char *		trapped_by;
    char *		rune_by;
    CHAR_DATA *		people;
    CHAR_DATA *		first_person;
    CHAR_DATA *		last_person;
    OBJ_DATA *		contents;
    OBJ_DATA *		first_content;
    OBJ_DATA *		last_content;
    EXTRA_DESCR_DATA *	extra_descr;
    EXTRA_DESCR_DATA *	first_extradesc;
    EXTRA_DESCR_DATA *	last_extradesc;
    AREA_DATA *		area;
    EXIT_DATA *		exit	  [6];
    EXIT_DATA *		first_exit;
    EXIT_DATA *		last_exit;
    ROOMTEXT_DATA *	roomtext;
    AFFECT_DATA *       affected;
    char *		track     [5];
    char *		name;
    char *		description;
    int			vnum;
    int		        room_flags;
    sh_int		light;
    sh_int		blood;
    sh_int		track_dir [5];
    sh_int		sector_type;
    sh_int		tick_timer[MAX_RTIMER];
    int                 affected_by;
};



/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000



/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4

#define TAR_OBJ_CHAR_DEF            5
#define TAR_OBJ_CHAR_OFF            6
#define TAR_OBJ_ROOM		    7
#define TAR_EXIT		    8
#define TAR_CHAR_WORLD		    9

#define TARGET_CHAR                 0
#define TARGET_OBJ                  1
#define TARGET_ROOM                 2
#define TARGET_NONE                 3

#define PURPLE_MAGIC		    0
#define RED_MAGIC		    1
#define BLUE_MAGIC		    2
#define GREEN_MAGIC		    3
#define YELLOW_MAGIC		    4



/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    sh_int	skill_level;	        /* Level needed by class	*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    int	        slot;			/* Slot for #OBJECT loading	*/
    sh_int	min_mana;		/* Minimum mana used		*/
    sh_int	beats;			/* Waiting time after use	*/
    char *	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
};

struct skill_data
{
 char *name;
 sh_int level;
 sh_int pos;
 sh_int beats;
 sh_int min_move;
 sh_int min_mana;
 char *noun;
};



/*
 * These are skill_lookup return values for common skills and spells.
 */
extern  sh_int  gsn_firebolt;
extern  sh_int  gsn_strike;
extern  sh_int    gsn_quills;
extern  sh_int    gsn_cheapshot;
extern  sh_int    gsn_shred;
extern  sh_int  gsn_swordstrike;
extern  sh_int  gsn_fwave;
extern  sh_int  gsn_cattack;
extern  sh_int  gsn_venom;
extern  sh_int  gsn_tentacle;
extern  sh_int  gsn_venomtong;
extern  sh_int  gsn_spiketail;
extern  sh_int  gsn_badbreath;
extern  sh_int  gsn_magma;
extern  sh_int  gsn_shards;
extern  sh_int  gsn_spiderform;
extern  sh_int  gsn_hakun;
extern  sh_int  gsn_garotte;
extern  sh_int  gsn_backstab;
/* drow circle */
extern  sh_int gsn_circle;
extern sh_int  gsn_shiroken;
extern sh_int  gsn_claws;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern  sh_int	gsn_godbless;  /* Vic - Monks */
extern  sh_int	gsn_totalblind; /* Vic - Monks */
extern	sh_int	gsn_tendrils;
extern	sh_int	gsn_berserk;
extern	sh_int	gsn_punch;
extern	sh_int	gsn_elbow;
extern	sh_int	gsn_headbutt;
extern  sh_int  gsn_shiroken;
extern  sh_int  gsn_blinky;
extern  sh_int  gsn_inferno;
extern  sh_int  gsn_fangs;
extern  sh_int  gsn_buffet;
extern  sh_int  gsn_rfangs;
extern	sh_int	gsn_sweep;
extern	sh_int	gsn_knee;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_hurl;
extern	sh_int	gsn_kick;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_track;
extern	sh_int	gsn_polymorph;
extern	sh_int	gsn_web;
extern  sh_int  gsn_infirmity;
extern sh_int	gsn_drowfire;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_mass_invis;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;
extern	sh_int	gsn_paradox;
extern sh_int gsn_spew;
extern	sh_int	gsn_darkness;
extern sh_int gsn_circle;
extern sh_int gsn_horns;
extern sh_int gsn_manashield;

/*
 * Utility macros.
 */
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define TOGGLE_BIT(var, bit)	((var) ^= (bit))

#define COLOUR(ch,color,bit)    (ADD_COLOUR((ch),(bit),(color));)


/*
 * Memory allocation macros.
 */

#define CREATE(result, type, number)				\
do								\
{								\
   if (!((result) = (type *) calloc ((number), sizeof(type))))	\
	{ perror("malloc failure"); abort(); }			\
} while(0)

#define RECREATE(result,type,number)				\
do								\
{								\
  if (!((result) = (type *) realloc ((result), sizeof(type) * (number))))\
	{ perror("realloc failure"); abort(); }			\
} while(0)


#define DISPOSE(point) 						\
do								\
{								\
  if (!(point))							\
  {								\
	bug( "Freeing null pointer",0 ); \
	fprintf( stderr, "DISPOSEing NULL in %s, line %d\n", __FILE__, __LINE__ ); \
  }								\
  else free(point);						\
  point = NULL;							\
} while(0)

#ifdef HASHSTR
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	quick_link((point))
#define QUICKMATCH(p1, p2)	(int) (p1) == (int) (p2)
#define STRFREE(point)						\
do								\
{								\
  if (!(point))							\
  {								\
	bug( "Freeing null pointer",0 );	 		\
  }								\
  else if (str_free((point))==-1) 				\
    fprintf( stderr, "STRFREEing bad pointer in %s, line %d\n", __FILE__, __LINE__ ); \
} while(0)
#else
#define STRALLOC(point)		str_dup((point))
#define QUICKLINK(point)	str_dup((point))
#define QUICKMATCH(p1, p2)	strcmp((p1), (p2)) == 0
#define STRFREE(point)						\
do								\
{								\
  if (point == NULL)						\
  {								\
	bug( "Freeing null pointer",0 );		        \
  }								\
  else free_string((point));					\
} while(0)
#endif

/* double-linked list handling macros -Thoric */

#define LINK(link, first, last, next, prev)			\
do								\
{								\
    if ( !(first) )						\
      (first)			= (link);			\
    else							\
      (last)->next		= (link);			\
    (link)->next		= NULL;				\
    (link)->prev		= (last);			\
    (last)			= (link);			\
} while(0)

#define INSERT(link, insert, first, next, prev)			\
do								\
{								\
    (link)->prev		= (insert)->prev;		\
    if ( !(insert)->prev )					\
      (first)			= (link);			\
    else							\
      (insert)->prev->next	= (link);			\
    (insert)->prev		= (link);			\
    (link)->next		= (insert);			\
} while(0)

#define UNLINK(link, first, last, next, prev)			\
do								\
{								\
    if ( !(link)->prev )					\
      (first)			= (link)->next;			\
    else							\
      (link)->prev->next	= (link)->next;			\
    if ( !(link)->next )					\
      (last)			= (link)->prev;			\
    else							\
      (link)->next->prev	= (link)->prev;			\
} while(0)


#define CHECK_LINKS(first, last, next, prev, type)		\
do {								\
  type *ptr, *pptr = NULL;					\
  if ( !(first) && !(last) )					\
    break;							\
  if ( !(first) )						\
  {								\
    bug( "CHECK_LINKS: last with NULL first!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (last); ptr->prev; ptr = ptr->prev );		\
    (first) = ptr;						\
  }								\
  else if ( !(last) )						\
  {								\
    bug( "CHECK_LINKS: first with NULL last!  %s.",		\
        __STRING(first) );					\
    for ( ptr = (first); ptr->next; ptr = ptr->next );		\
    (last) = ptr;						\
  }								\
  if ( (first) )						\
  {								\
    for ( ptr = (first); ptr; ptr = ptr->next )			\
    {								\
      if ( ptr->prev != pptr )					\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->prev = pptr;					\
      }								\
      if ( ptr->prev && ptr->prev->next != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->prev->next != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->prev->next = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
    pptr = NULL;						\
  }								\
  if ( (last) )							\
  {								\
    for ( ptr = (last); ptr; ptr = ptr->prev )			\
    {								\
      if ( ptr->next != pptr )					\
      {								\
        bug( "CHECK_LINKS (%s): %p:->next != %p.  Fixing.",	\
            __STRING(first), ptr, pptr );			\
        ptr->next = pptr;					\
      }								\
      if ( ptr->next && ptr->next->prev != ptr )		\
      {								\
        bug( "CHECK_LINKS(%s): %p:->next->prev != %p.  Fixing.",\
            __STRING(first), ptr, ptr );			\
        ptr->next->prev = ptr;					\
      }								\
      pptr = ptr;						\
    }								\
  }								\
} while(0)


#define ASSIGN_GSN(gsn, skill)					\
do								\
{								\
    if ( ((gsn) = skill_lookup((skill))) == -1 )		\
	fprintf( stderr, "ASSIGN_GSN: Skill %s not found.\n",	\
		(skill) );					\
} while(0)

#define CHECK_SUBRESTRICTED(ch)					\
do								\
{								\
    if ( (ch)->substate == SUB_RESTRICTED )			\
    {								\
	send_to_char( "You cannot use this command from within another command.\n\r", ch );	\
	return;							\
    }								\
} while(0)


/*
 * Character macros.
 */

#define IS_NEWFLAG(ch, sn)      (IS_SET((ch)->flag2, (sn)))
#define IS_CREATOR(ch)		(!str_cmp(GET_PC_NAME(ch), "SaTaN" ))
#define GET_PROPER_NAME(ch)     (IS_NPC((ch)) ? (ch)->short_descr : (ch)->pcdata->switchname)
#define GET_PC_NAME(ch)     	(IS_NPC((ch)) ? "<npc>" : (ch)->pcdata->switchname)

#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_JUDGE(ch)		(get_trust(ch) >= LEVEL_JUDGE)
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)		(get_trust(ch) >= LEVEL_HERO)

#define IS_DAY()	    	((weather_info.sunlight == SUN_RISE || weather_info.sunlight == SUN_LIGHT))
#define IS_NIGHT()	    	((!IS_DAY()))

#define CAN_PK(ch)		(get_trust(ch)>= 3 && get_trust(ch)<= 12)
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
#define IS_SPEAKING(ch, sn)	(IS_SET((ch)->pcdata->language[0], (sn)))
#define CAN_SPEAK(ch, sn)	(IS_SET((ch)->pcdata->language[1], (sn)))
#define IS_ITEMAFF(ch, sn)	(IS_SET((ch)->itemaffect, (sn)))
#define IS_IMMUNE(ch, sn)	(IS_SET((ch)->immune, (sn)))
#define IS_FORM(ch, sn)		(IS_SET((ch)->form, (sn)))
#define IS_POLYAFF(ch, sn)	(IS_SET((ch)->polyaff, (sn)))
#define IS_EXTRA(ch, sn)	(IS_SET((ch)->extra, (sn)))
#define IS_STANCE(ch, sn)	(ch->stance[0] == sn)

#define IS_HEAD(ch, sn)		(IS_SET((ch)->loc_hp[0], (sn)))
#define IS_BODY(ch, sn)		(IS_SET((ch)->loc_hp[1], (sn)))
#define IS_ARM_L(ch, sn)	(IS_SET((ch)->loc_hp[2], (sn)))
#define IS_ARM_R(ch, sn)	(IS_SET((ch)->loc_hp[3], (sn)))
#define IS_LEG_L(ch, sn)	(IS_SET((ch)->loc_hp[4], (sn)))
#define IS_LEG_R(ch, sn)	(IS_SET((ch)->loc_hp[5], (sn)))
#define IS_BLEEDING(ch, sn)	(IS_SET((ch)->loc_hp[6], (sn)))

#define IN_CLAN( ch )           (IS_NPC(ch)?0:ch->clan)
#define IS_PLAYING( d )         (d->connected==CON_PLAYING)
 
#define IS_GOOD(ch)		(ch->alignment >= 350)
#define IS_EVIL(ch)		(ch->alignment <= -350)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))

#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->room_flags,		    \
				    ROOM_INDOORS))

#define WAIT_STATE(ch, npulse)	((ch)->wait = UMAX((ch)->wait, (npulse)))



/*
 * Object Macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))
#define IS_OBJ_STAT2(obj, stat) (IS_SET((obj)->extra_flags2,(stat)))
#define IS_WEAP(obj, stat)      (IS_SET((obj)->weapflags,  (stat)))


/*
 * Description macros.
 */
#define PERS(ch, looker) ( can_see(looker,(ch)) ? (IS_NPC(ch) ? (ch)->short_descr : \
			 ( strlen((ch)->morph) > 0 ? (ch)->morph : (ch)->name)) : "someone" )


/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    //char *              code;
    sh_int		position;
    sh_int		level;
    sh_int		log;
    sh_int              race; /* 0 = all, other = specific race */
    sh_int		discipline; /* USE THE DISC_VAMP_???? etc.... */
    sh_int	       	disclevel;  /* level in disc the command is granted */
};

/*
 * Global constants.
 */
extern	const	struct	str_app_type	str_app		[26];
extern	const	struct	int_app_type	int_app		[26];
extern	const	struct	wis_app_type	wis_app		[26];
extern	const	struct	dex_app_type	dex_app		[26];
extern	const	struct	con_app_type	con_app		[26];

extern	const	struct	cmd_type	cmd_table	[];
extern	const	struct	liq_type	liq_table	[LIQ_MAX];
extern	const	struct	skill_type	skill_table	[MAX_SKILL];
extern  const   struct  race_type       race_table      [];
extern  const   char    * colour_name                   [8];
extern  const   char	* circlesymbol			[7];
extern  const   struct	clanrank_type	clanrank	[9][14];
extern  const   sh_int			rev_dir		[];

/*
 * Global variables.
 */
extern          WIZ_DATA          *     wiz_first;
extern          WIZ_DATA          *     wiz_last;
extern          CLAN_DATA         *     clan_list;
extern          CLAN_DATA         *     last_clan;
extern		HELP_DATA	  *	help_first;

extern		HELP_DATA	  *	first_help;
extern		HELP_DATA	  *	last_help;

extern		BAN_DATA	  *	ban_list;
extern		CHAR_DATA	  *	char_list;
extern		DESCRIPTOR_DATA   *	descriptor_list;
extern		NOTE_DATA	  *	note_list;
extern		OBJ_DATA	  *	object_list;

extern		BALANCE_DATA	  	balance_info[7];
extern		CONFIG_DATA	  *	config_info;
extern          ROOM_INDEX_DATA   *     room_list;
extern		AFFECT_DATA	  *	affect_free;
extern		BAN_DATA	  *	ban_free;
extern		CHAR_DATA	  *	char_free;
extern		DESCRIPTOR_DATA	  *	descriptor_free;
extern		EXTRA_DESCR_DATA  *	extra_descr_free;
extern		ROOMTEXT_DATA     *	roomtext_free;
extern		NOTE_DATA	  *	note_free;
extern		OBJ_DATA	  *	obj_free;
extern		PC_DATA		  *	pcdata_free;
extern          RACE_DATA         *     racedata_free;

extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern		FILE *			fpReserve;
extern		char			log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA		weather_info;
extern          char                    last_command[MAX_STRING_LENGTH];
extern          int                     arena;
extern          int                     arena_type;
extern          int                     global_exp_mult;
extern          int                     new_players;
extern          int                     old_players;
extern          float                   exp_who;
extern          bool                    PWipe;
extern		bool			deathmatch;
extern		bool			nogroup;
extern  bool            db_connect;       /* tells us if we have an open connection to the DB */
extern  sh_int          db_last_action;   /* How long since we last accessed the DB */
extern  MYSQL           *db;              /* the database object */

extern		int			top_social;
typedef struct  social_data   SOCIAL_DATA;
extern SOCIAL_DATA **social_table;

void init_social_table   args((void));
void resize_social_table args((int amt));

enum { SOC_COL_NAME, SOC_COL_CNA, SOC_COL_CF, SOC_COL_CA, SOC_COL_ONA, SOC_COL_VF, SOC_COL_OF, SOC_COL_OA };
struct social_data
{
        char *name;
        char *char_no_arg;
        char *others_no_arg;
        char *char_found;
        char *others_found;
        char *vict_found;
        char *char_auto;
        char *others_auto;
        bool changed;
        int times_used;
};

/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(_AIX)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(apollo)
int	atoi		args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(interactive)
#endif

#if	defined(linux)
char *	crypt		args( ( const char *key, const char *salt
) );
#endif

#if	defined(macintosh)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *	crypt		args( ( const char *key, const char *salt
) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
#if 	defined(SYSV)
size_t 	fread		args( ( void *ptr, size_t size, size_t n, 
				FILE *stream ) );
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif



/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)

#define PLAYER_DIR	""		/* Player files			*/
#define NULL_FILE	"proto.are"	/* To reserve one stream	*/
#define BACKUP_DIR      "" /*reimburse shit -Infidel*/
#endif

#if defined(MSDOS)
#define PLAYER_DIR	""		/* Player files                 */
#define NULL_FILE	"nul"		/* To reserve one stream	*/
#endif

#if defined(unix)
#define COMMAND_FILE    "commands.dat"        
#define PLAYER_DIR	"../player/"           /* Player files   	*/
#define BACKUP_DIR      "../player/store/"     /*reimb shit*/
#define BACK2_DIR       "../player/backup/"    /* need to delete this dir...*/
#define LAST_COMMAND    "../last_command.txt"  /*For the signal handler.*/
#define NULL_FILE	"/dev/null"	       /* To reserve one stream	*/
#endif

#define CLAN_FILE       "clans.dat"
#define AREA_LIST	"area.lst"	/* List of areas		*/
#define CLAN_LIST	"clan1.txt"	/* List of clans		*/
#define ART_LIST	"art1.txt"	/* List of artifacts            */
#define HOME_AREA	"../area/homes.are" /* Mages towers, etc        */
#define BAN_LIST	"../area/ban.txt"	/* baaan. */
#define BUG_FILE	"bugs.txt"      /* For 'bug' and bug( )		*/
#define IDEA_FILE	"ideas.txt"	/* For 'idea'			*/
#define TYPO_FILE	"typos.txt"     /* For 'typo'			*/
#define NOTE_FILE	"notes.txt"	/* For 'notes'			*/
#define SHUTDOWN_FILE	"shutdown.txt"	/* For 'shutdown'		*/
#define COPYOVER_FILE   "copyover"
#define EXE_FILE       "../src/merc"   
#define IMM_FILE	"wizl.dat"	/* For relevel/dynamic wizlist  */
#define CONFIG_FILE     "config.dat"    /* Various configuration things */
#define FIGHT_LOG	"fight_log.log" /* Fight logging for debugging  */
#define CLASS_FILE	"class.dat"	/* Class configuration	--Kline */

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define ED	EXIT_DATA
/* who.c */
char* get_status_name( CHAR_DATA *ch);
char* get_class_name( CHAR_DATA *ch);
char* get_class_rank( CHAR_DATA *ch);
char* get_player_name( CHAR_DATA *ch);
char* get_title( CHAR_DATA *ch);
char* get_clan_rank( CHAR_DATA *ch);
char* act_who( CHAR_DATA *ch, const char *format);
CLAN_DATA *find_clan_name(char *argument);
/* act_comm.c */
bool    write_to_descriptor     args( ( int desc, char *txt, int length ) );
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void	room_text	args( ( CHAR_DATA *ch, char *argument ) );
char    *strlower       args( ( char * ip ) );
void	excessive_cpu	args( ( int blx ) );
bool	check_parse_name args( ( char *name ) );

void    room_message    args( ( ROOM_INDEX_DATA *room, char *message ) );

/* act_info.c */
void	set_title	args( ( CHAR_DATA *ch, char *title ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
int	char_hitroll	args( ( CHAR_DATA *ch ) );
int	char_damroll	args( ( CHAR_DATA *ch ) );
int	char_ac		args( ( CHAR_DATA *ch ) );
void    set_switchname  args( ( CHAR_DATA *ch, char *title ) );
void    set_pc_name     args( ( CHAR_DATA *ch, char *title ) );

ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );

/* act_move.c */
void	move_char	args( ( CHAR_DATA *ch, int door ) );
void	open_lift	args( ( CHAR_DATA *ch ) );
void	close_lift	args( ( CHAR_DATA *ch ) );
void	move_lift	args( ( CHAR_DATA *ch, int to_room ) );
void	move_door	args( ( CHAR_DATA *ch ) );
void	thru_door	args( ( CHAR_DATA *ch, int doorexit ) );
void	open_door	args( ( CHAR_DATA *ch, bool be_open ) );
bool	is_open		args( ( CHAR_DATA *ch ) );
bool	same_floor	args( ( CHAR_DATA *ch, int cmp_room ) );
void	check_hunt	args( ( CHAR_DATA *ch ) );
void    check_trap      args( ( CHAR_DATA *ch));

int   disc_points_needed args( (CHAR_DATA *ch) );
void  gain_disc_points  args( (CHAR_DATA *ch, int points ) );
RID  *get_random_room   args ( (CHAR_DATA *ch) );
RID  *get_rand_room   args ( ( ) );
RID  *get_treemeld_room   args ( ( ) );
RID  *get_rand_room_by_sect   args ( (int sect ) );


/* act_obj.c */
bool	is_ok_to_wear	args( ( CHAR_DATA *ch, bool wolf_ok, char *argument ) );
void	quest_object	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	remove_obj	args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );

/* act_wiz.c */
void	bind_char	args( ( CHAR_DATA *ch ) );
void	logchan		args( ( char *argument ) );


/* build.c */
ED *	get_exit	args( ( ROOM_INDEX_DATA *room, sh_int dir ) );
ED *	get_exit_to	args( ( ROOM_INDEX_DATA *room, sh_int dir, int vnum ) );
ED *	get_exit_num	args( ( ROOM_INDEX_DATA *room, sh_int count ) );

char *	copy_buffer	args( ( CHAR_DATA *ch ) );
void	edit_buffer	args( ( CHAR_DATA *ch, char *argument ) );
char *	strip_cr	args( ( char *str  ) );
void    start_editing   args( ( CHAR_DATA *ch, char *data ) );  
void    stop_editing    args( ( CHAR_DATA *ch ) );

/* comm.c */
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	close_socket2	args( ( DESCRIPTOR_DATA *dclose, bool kickoff ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void    send_to_desc    args( ( const char *txt, DESCRIPTOR_DATA *d) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	act2		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type, int dam ) );
void	kavitem		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void ch_output		args( ( CHAR_DATA *ch, char *txt, ...) );

void    banner_to_char  args( ( char *txt, CHAR_DATA *ch ) );
void    banner2_to_char args( ( char *txt, CHAR_DATA *ch ) );
void    divide_to_char  args( ( CHAR_DATA *ch ) );
void    divide2_to_char args( ( CHAR_DATA *ch ) );
void    divide3_to_char args( ( CHAR_DATA *ch ) );
void    divide4_to_char args( ( CHAR_DATA *ch ) );
void    divide5_to_char args( ( CHAR_DATA *ch ) );
void    divide6_to_char args( ( CHAR_DATA *ch ) );
void    stc	        args( ( const char *txt, CHAR_DATA *ch ) );
void    cent_to_char    args( ( char *txt, CHAR_DATA *ch ) );
int     new_strlen      args( ( char *str) );
#define str_len new_strlen
#define DISABLED_FILE "disabled.txt"  /* disabled commands */

/* prototypes from db.c */
void  load_disabled   args( ( void ) );
void  save_disabled   args( ( void ) );

/* db.c */
void init_descriptor (DESCRIPTOR_DATA *dnew, int desc);
void    copyover_recover();
void	boot_db		args( ( bool fCopyOver) );
void	area_update	args( ( void ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
void	free_char	args( ( CHAR_DATA *ch ) );
char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
char *	get_roomtext	args( ( const char *name, ROOMTEXT_DATA    *rt ) );
MID *	get_mob_index	args( ( int vnum ) );
OID *	get_obj_index	args( ( int vnum ) );
RID *	get_room_index	args( ( int vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
void	free_mem	args( ( void *pMem, int sMem ) );
char *	str_dup		args( ( const char *str ) );
void	free_string	args( ( char *pstr ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
int	number_mm	args( ( void ) );
int	dice		args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
void	smash_tilde	args( ( char *str ) );
void    smash_color     args( ( char *str ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void	bug		args( ( const char *str, int param ) );
void    fight_log       args( ( const char *str ) );
void	log_string	args( ( const char *str ) );
void	tail_chain	args( ( void ) );

void	add_help	args( ( HELP_DATA *pHelp ) );
char * up_case          args( ( const char *str ) );
char *	strupper	args( ( const char *str ) );
RID *	make_room	args( ( int vnum ) );
OID *	make_object	args( ( int vnum, int cvnum, char *name ) );
MID *	make_mobile	args( ( sh_int vnum, sh_int cvnum, char *name ) );
ED  *	make_exit	args( ( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, sh_int door ) );

/* daemon.c */

ROOM_INDEX_DATA * locate_obj	args ( ( OBJ_DATA *obj ) );
void shock_effect  args( ( void *vo,int level, int dam, int target ) );
void cold_effect   args( ( void *vo,int level, int dam, int target ) );
void acid_effect   args( ( void *vo,int level, int dam, int target ) );
void fire_effect   args( ( void *vo,int level, int dam, int target ) );
void make_wall     args( ( ROOM_INDEX_DATA *room, int dir, int wall ) );

/* fight.c */

int  cap_dam		args( ( CD *ch, CD *victim, int dam));
int  randomize_damage	args( ( CD *ch, int dam ) );
void bounty_reward      args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
bool has_bounty         args( ( CHAR_DATA *ch) );
void	violence_update	args( ( void ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
bool    damage_old      args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int class, bool show ) );
void	adv_damage	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void    stop_embrace    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool	no_attack	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	MAGIC_CAN_AFFECT args(( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_stun	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    can_heal        args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	hurt_person	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool    has_timer	args( ( CHAR_DATA *ch ) );
bool 	IS_DAMNED	args( ( CHAR_DATA *ch ) );
bool	is_clan_leader  args( ( CHAR_DATA *ch ) );
bool   is_clan_coleader args( ( CHAR_DATA *ch ) );

/* handler.c */

void	extract_exit	args( ( ROOM_INDEX_DATA *room, EXIT_DATA *pexit ) );

int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_age		args( ( CHAR_DATA *ch ) );
int	get_curr_str	args( ( CHAR_DATA *ch ) );
int	get_curr_int	args( ( CHAR_DATA *ch ) );
int	get_curr_wis	args( ( CHAR_DATA *ch ) );
int	get_curr_dex	args( ( CHAR_DATA *ch ) );
int	get_curr_con	args( ( CHAR_DATA *ch ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( const char *str, char *namelist ) );
// void	assign_class_name  args( ( CHAR_DATA *ch ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf) );
void	total_affects	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf) );
void    affect_to_char2 args( ( CHAR_DATA *ch, AFFECT_DATA *paf, int level));
void    affect_remove2  args( ( CHAR_DATA *ch, AFFECT_DATA *paf, int level));
void    affect_modify2  args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, int level));
void    affect_strip2   args( ( CHAR_DATA *ch, int sn, int level));
void    room_affect_remove args(( ROOM_INDEX_DATA *room, AFFECT_DATA *paf));
void    area_affect_remove args(( AREA_DATA *area, AFFECT_DATA *paf));
void    affect_to_room  args( ( ROOM_INDEX_DATA *room, AFFECT_DATA *paf) );
void    affect_to_area  args( ( AREA_DATA *area, AFFECT_DATA *paf) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
CD *    get_char_area   args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_in_obj	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_room	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world2	args( ( CHAR_DATA *ch, char *argument ) );
OD *	create_money	args( ( int amount ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *	item_type_name	args( ( OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *  affect_bit_name2 args( (int vector) );
char *	extra_bit_name	args( ( int extra_flags ) );
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd) );
void    room_affect_modify args(( ROOM_INDEX_DATA *room, AFFECT_DATA *paf,
                                  bool fAdd));
void    area_affect_modify args(( AREA_DATA *area, AFFECT_DATA *paf, bool fAdd));

void    set_learnable_disciplines args( ( CHAR_DATA *ch ) );
void    update_disc	args( ( CHAR_DATA *ch) );
void    make_gem        args( ( OBJ_DATA *obj) );
void    make_ore        args( ( OBJ_DATA *obj) );
void    set_gem_name    args( (OBJ_DATA *obj) );
bool    standard_calls  args( (CHAR_DATA *ch, int power, int level) );

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
char *format_string	args( ( char *oldstring ) );
void	stage_update	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int stage ) );
void    make_preg       args( ( CHAR_DATA *mother, CHAR_DATA *father ) );

/* magic.c */
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );
void	enhance_stat	args( ( int sn, int level, CHAR_DATA *ch, 
				    CHAR_DATA *victim, int apply_bit,
				    int bonuses, int affect_bit ) );
void    do_summon_wear_loc args( (CHAR_DATA *ch, char *argument ) );

/* stat_sav.c */

void    initialise_config	args( ( void ) );

/* class_config.c */
void	init_class		args( ( void ) );

/* save.c */
void	save_char_obj		args( ( CHAR_DATA *ch ) );
void	save_char_obj_backup	args( ( CHAR_DATA *ch ) );
bool	load_char_obj		args( ( DESCRIPTOR_DATA *d, char *name ) );
bool	load_char_short		args( ( DESCRIPTOR_DATA *d, char *name ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );

/* update.c */
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );

void    mobile_update   args( ( void ) );
void    weather_update  args( ( void ) );
void    char_update     args( ( void ) );
void    obj_update      args( ( void ) );
void    aggr_update     args( ( void ) );
void    ww_update       args( ( void ) );
void    room_update	args( ( void ) );

/* kav_fight.c */
void	special_move	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* kav_info.c */
void	birth_date	args( ( CHAR_DATA *ch, bool is_self ) );
void	other_age	args( ( CHAR_DATA *ch, int extra, bool is_preg, 
				char *argument ) );
int	years_old	args( ( CHAR_DATA *ch ) );

/* kav_wiz.c */
void oset_affect args( ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest) );
void mob_oset_affect ( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest );

/* clan.c */
void	werewolf_regen	args( ( CHAR_DATA *ch ) );
void	mortal_regen	args( ( CHAR_DATA *ch ) );
void	reg_mend	args( ( CHAR_DATA *ch ) );
void	vamp_rage	args( ( CHAR_DATA *ch ) );
bool	char_exists	args( ( bool backup, char *argument ) );
OD *	get_page	args( ( OBJ_DATA *book, int page_num ) );

/* vic.c */
DECLARE_DO_FUN( do_relevel2 );
void reset_weapon   args( (CHAR_DATA *ch, int dtype) );
void reset_spell    args( (CHAR_DATA *ch, int dtype) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef  ED

#endif

/*
 * sql_handler.c
 */
MYSQL_RES *query_db(char *query);
