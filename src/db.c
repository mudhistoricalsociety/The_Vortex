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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)

#include <types.h>

#else

#include <sys/types.h>

#endif

#include <ctype.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <signal.h>
#include "merc.h"
#include "clantable.h"



#if !defined(macintosh)

extern	int	_filbuf		args( (FILE *) );

#endif





/*

 * Globals.

 */
SOCIAL_DATA **social_table;
CLAN_DATA *             clan_list;
CLAN_DATA *             last_clan;
WIZ_DATA  *             wiz_first;
WIZ_DATA  *             wiz_last;
HELP_DATA *		help_first;

HELP_DATA *		help_last;

CHAR_DATA *		char_free;

EXTRA_DESCR_DATA *	extra_descr_free;

NOTE_DATA *		note_free;

OBJ_DATA *		obj_free;

PC_DATA *		pcdata_free;
RACE_DATA *             racedata_free;


char			bug_buf		[2*MAX_INPUT_LENGTH];

CHAR_DATA *		char_list;

char *			help_greeting;

char			log_buf		[2*MAX_INPUT_LENGTH];

NOTE_DATA *		note_list;

OBJ_DATA *		object_list;
TIME_INFO_DATA		time_info;

WEATHER_DATA		weather_info;
bool			mem_log;
int                     global_exp_mult;
int                     new_players;
int                     old_players;
float                   exp_who;
int			top_social = 1;
sh_int                  gsn_strike;
sh_int                  gsn_firebolt;
sh_int			gsn_backstab;
sh_int			gsn_circle;
sh_int                  gsn_cattack;
sh_int                  gsn_fwave;
sh_int                  gsn_shred;
sh_int                  gsn_venom;
sh_int                  gsn_tentacle;
sh_int                  gsn_quills;
sh_int                  gsn_swordstrike;
sh_int 			gsn_cheapshot;
sh_int                  gsn_spit;
sh_int			gsn_venomtong;
sh_int			gsn_spiketail;
sh_int			gsn_badbreath;
sh_int			gsn_magma;
sh_int			gsn_shards;
sh_int			gsn_hide;
sh_int			gsn_peek;
sh_int			gsn_pick_lock;
sh_int			gsn_sneak;
sh_int			gsn_steal;
sh_int			gsn_spiderform;
sh_int			gsn_garotte;
sh_int                  gsn_hakun;
sh_int			gsn_disarm;
sh_int			gsn_tendrils;
sh_int			gsn_berserk;
sh_int			gsn_punch;
sh_int			gsn_elbow;
sh_int			gsn_headbutt;
sh_int                  gsn_shiroken;
sh_int                  gsn_claws;
sh_int			gsn_inferno;
sh_int			gsn_blinky;
sh_int			gsn_fangs;
sh_int			gsn_buffet;
sh_int			gsn_sweep;
sh_int			gsn_knee;
sh_int                  gsn_rfangs;
sh_int			gsn_kick;
sh_int			gsn_hurl;
sh_int			gsn_rescue;
sh_int			gsn_track;
sh_int			gsn_polymorph;
sh_int			gsn_web;
sh_int			gsn_drowfire;
sh_int                  gsn_infirmity;
sh_int                  gsn_spew;
sh_int			gsn_blindness;
sh_int			gsn_charm_person;
sh_int			gsn_curse;
sh_int			gsn_invis;
sh_int			gsn_mass_invis;
sh_int			gsn_poison;
sh_int			gsn_sleep;
sh_int			gsn_darkness;
sh_int			gsn_paradox;
sh_int			gsn_horns;
sh_int			gsn_manashield;

void    auto_copyover           args( ( void ) );






/*

 * Locals.

 */

MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];

OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];

ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];

char *			string_hash		[MAX_KEY_HASH];



AREA_DATA *		area_first;

AREA_DATA *		area_last;


CONFIG_DATA *		config_info;
ROOM_INDEX_DATA *	room_list;

HELP_DATA *		first_help;
HELP_DATA *		last_help;



char *			string_space;

char *			top_string;

char			str_empty	[1];



int			top_affect;

int			top_area;

int			top_rt;

int			top_ed;

int			top_exit;

int			top_help;

int			top_mob_index;

int			top_obj_index;

int			top_reset;

int			top_room;

int			top_shop;







/*

 * Memory management.

 * Increase MAX_STRING if you have too.

 * Tune the others only if you understand what you're doing.



#define			MAX_STRING	1048576



 */



#define			MAX_STRING	2097152

#define			MAX_PERM_BLOCK	131072

#define			MAX_MEM_LIST	13



void *			rgFreeList	[MAX_MEM_LIST];

const int		rgSizeList	[MAX_MEM_LIST]	=

{

    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64

};



int			nAllocString;

int			sAllocString;

int			nAllocPerm;

int			sAllocPerm;







/*

 * Semi-locals.

 */

bool			fBootDb;

FILE *			fpArea;

char			strArea[MAX_INPUT_LENGTH];







/*

 * Local booting procedures.

 */

void	init_mm		args( ( void ) );


void    load_clans      args( (void) );
void	load_area	args( ( FILE *fp ) );
void    load_wizlist    args( (void) );
void    load_helps      args( ( FILE *fp ) );
void	load_mobiles	args( ( FILE *fp ) );

void	load_objects	args( ( FILE *fp ) );

void	load_resets	args( ( FILE *fp ) );

void	load_rooms	args( ( FILE *fp ) );

void	load_shops	args( ( FILE *fp ) );

void	load_specials	args( ( FILE *fp ) );

void load_artifacts args ((void));

void	load_notes	args( ( void ) );



void	fix_exits	args( ( void ) );



void	reset_area	args( ( AREA_DATA * pArea ) );







/*
 * Big mama top level function.
 */

void boot_db(bool fCopyOver)
{
    /*

     * Init some data space stuff.

     */

    {

	if ( ( string_space = calloc( 1, MAX_STRING ) ) == NULL )

	{

	    bug( "Boot_db: can't alloc %d string space.", MAX_STRING );

	    exit( 1 );

	}

	top_string	= string_space;

	fBootDb		= TRUE;

    }



    /*

     * Init random number generator.

     */

    {

	init_mm( );

    }

    /*

     * Set time and weather.

     */

    {

	long lhour, lday, lmonth;



	lhour		= (current_time - 650336715)

			/ (PULSE_TICK / PULSE_PER_SECOND);

	time_info.hour	= lhour  % 24;

	lday		= lhour  / 24;

	time_info.day	= lday   % 35;

	lmonth		= lday   / 35;

	time_info.month	= lmonth % 17;

	time_info.year	= lmonth / 17;



	     if ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;

	else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;

	else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;

	else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;

	else                            weather_info.sunlight = SUN_DARK;



	weather_info.change	= 0;

	weather_info.mmhg	= 960;

	if ( time_info.month >= 7 && time_info.month <=12 )

	    weather_info.mmhg += number_range( 1, 50 );

	else

	    weather_info.mmhg += number_range( 1, 80 );



	     if ( weather_info.mmhg <=  980 ) weather_info.sky = SKY_LIGHTNING;

	else if ( weather_info.mmhg <= 1000 ) weather_info.sky = SKY_RAINING;

	else if ( weather_info.mmhg <= 1020 ) weather_info.sky = SKY_CLOUDY;

	else                                  weather_info.sky = SKY_CLOUDLESS;



    }



    /*

     * Assign gsn's for skills which have them.

     */

    {

	int sn;



	for ( sn = 0; sn < MAX_SKILL; sn++ )

	{

	    if ( skill_table[sn].pgsn != NULL )

		*skill_table[sn].pgsn = sn;

	}

    }



    /*

     * Read in all the area files.

     */

    {

	FILE *fpList;



	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
	{

	    perror( AREA_LIST );

	    exit( 1 );

	}



	for ( ; ; )

	{

	    strcpy( strArea, fread_word( fpList ) );

	    if ( strArea[0] == '$' )

		break;



	    if ( strArea[0] == '-' )

	    {

		fpArea = stdin;

	    }

	    else

	    {

		if ( ( fpArea = fopen( strArea, "r" ) ) == NULL )

		{

		    perror( strArea );

		    exit( 1 );

		}

	    }



	    for ( ; ; )

	    {

		char *word;



		if ( fread_letter( fpArea ) != '#' )

		{

		    bug( "Boot_db: # not found.", 0 );

		    exit( 1 );

		}



		word = fread_word( fpArea );



		     if ( word[0] == '$'               )                 break;

		else if ( !str_cmp( word, "AREA"     ) ) load_area    (fpArea);

		else if ( !str_cmp( word, "HELPS"    ) ) load_helps   (fpArea);

		else if ( !str_cmp( word, "MOBILES"  ) ) load_mobiles (fpArea);

		else if ( !str_cmp( word, "OBJECTS"  ) ) load_objects (fpArea);

		else if ( !str_cmp( word, "RESETS"   ) ) load_resets  (fpArea);

		else if ( !str_cmp( word, "ROOMS"    ) ) load_rooms   (fpArea);

		else if ( !str_cmp( word, "SPECIALS" ) ) load_specials(fpArea);

		else

		{

		    bug( "Boot_db: bad section name.", 0 );

		    exit( 1 );

		}

	    }



	    if ( fpArea != stdin )

		fclose( fpArea );

	    fpArea = NULL;

	}

	fclose( fpList );

    }



    /*

     * Fix up exits.

     * Declare db booting over.

     * Reset all areas once.

     * Load up the notes file.

     */

    {

	fix_exits( );

	fBootDb	= FALSE;

	area_update( );
	load_bans();
	load_notes( );
	initialise_config();
        init_class();
	load_disabled();
        load_clans();
        load_wizlist();
    }
    /* set global exp multiplier */
    if( fCopyOver && !PWipe)
     copyover_recover();
    global_exp_mult = 1;
    new_players = 0;
    old_players = 0;
    exp_who = 1.0;
    deathmatch = FALSE;
    nogroup = FALSE;
    return;
 }

void do_kale_owns_you( CHAR_DATA *ch, char *argument)
{
	if( IS_NPC(ch))
		return;
    if( str_cmp( ch->pcdata->switchname, "Kale"))
    {
	stc("Huh?\n\r", ch);
	return;
    }
    if ( IS_EXTRA(ch, TIED_UP) )
        REMOVE_BIT(ch->extra, TIED_UP);
    if( IS_EXTRA( ch, GAGGED))
  	REMOVE_BIT( ch->extra, GAGGED);
    if( IS_EXTRA( ch, BLINDFOLDED))
	REMOVE_BIT( ch->extra, BLINDFOLDED);
	return;
}

void load_wizlist( void )
{
 FILE *fp;
 char *word;
 bool done = FALSE;
 bool found = FALSE;
 WIZ_DATA *list;

 if( (fp = fopen( IMM_FILE, "r")) == NULL)
  return;
 log_string("Loadin wizlist...");
 word = fread_word(fp);
 while( !done)
 {
  found = FALSE;
  if( !str_cmp( word, "#NEW"))
  {
   CREATE(list, WIZ_DATA, 1);
   found = TRUE;
  }
  if( !str_cmp( word, "Name"))
  {
   list->name = str_dup( "");
   list->name = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp( word, "Level"))
  {
   list->level = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp( word, "Message"))
  {
   list->message = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp( word, "End"))
  {
   LINK(list, wiz_first, wiz_last, next, prev);
   found = TRUE;
  }
  else if( !str_cmp( word, "#END"))
  {
   done = TRUE;
   found = TRUE;
   break;
  }
  if( !found)
  {
   fclose(fp);
   return;
  }
  if( !done)
   word = fread_word(fp);
 }
 log_string("Wizlist loaded...");
 fclose(fp);
 return;
}

void load_artifacts (void)
{
  CHAR_DATA *pMob;
  OBJ_DATA *obj;
  
  /* Hades - Ring of Hades */
  pMob = create_mobile (get_mob_index (20000));
  char_to_room (pMob, get_room_index (5));
  obj = create_object (get_obj_index (29521), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all"); /* added by adreno :P */
  SET_BIT(pMob->act,PLR_HOLYLIGHT);
  
  /* Zombie Lord - Armour of Resistance */
  pMob = create_mobile (get_mob_index (30011));
  char_to_room (pMob, get_room_index (3001));
  obj = create_object (get_obj_index (29514), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);

  /* Gaia - Visor of Vision */
  pMob = create_mobile (get_mob_index (20001));
  char_to_room (pMob, get_room_index (29701));
  obj = create_object (get_obj_index (29515), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);
  
  /* Caine - Short Sword of Speed */
  pMob = create_mobile (get_mob_index (20002));
  char_to_room (pMob, get_room_index (27002));
  obj = create_object (get_obj_index (29505), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);

  /* Lloth the Spider Queen - Bracelet of Lloth */
  pMob = create_mobile (get_mob_index (9000));
  char_to_room (pMob, get_room_index (9001));
  obj = create_object (get_obj_index (29518), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);
  
  /* God - The Torc of Continual Restoration */
  pMob = create_mobile (get_mob_index (20003));
  char_to_room (pMob, get_room_index (6645));
  obj = create_object (get_obj_index (29504), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);
  
  /* Chaos - Vorpal Blade */
  pMob = create_mobile (get_mob_index (20004));
  char_to_room (pMob, get_room_index (25014));
  obj = create_object (get_obj_index (29506), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);
 
  /* Tim the Enchanter - Ring of Hitting */
  pMob = create_mobile (get_mob_index (20005));
  char_to_room (pMob, get_room_index (20002));
  obj = create_object (get_obj_index (29508), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);

  /* Raistlin Majere - Tome of the Ancients*/
  pMob = create_mobile (get_mob_index (20006));
  char_to_room (pMob, get_room_index (19126));
  obj = create_object (get_obj_index (29513), 0);
  obj_to_char (obj, pMob);
  do_wear(pMob, "all");
  SET_BIT(pMob->act,PLR_HOLYLIGHT);
  return;
   
  }

/*

 * Snarf an 'area' header line.

 */

void load_area( FILE *fp )

{

    AREA_DATA *pArea;



    pArea		= alloc_perm( sizeof(*pArea) );

    pArea->reset_first	= NULL;

    pArea->reset_last	= NULL;

    pArea->name		= fread_string( fp );
    pArea->lo_vnum      = fread_number(fp);
    pArea->hi_vnum      = fread_number(fp);
    pArea->age		= 15;

    pArea->nplayer	= 0;



    if ( area_first == NULL )

	area_first = pArea;

    if ( area_last  != NULL )

	area_last->next = pArea;

    area_last	= pArea;

    pArea->next	= NULL;



    top_area++;

    return;

}

/*
 * Returns an uppercase string.
 */
char *strupper( const char *str )
{
    static char strup[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strup[i] = UPPER(str[i]);
    strup[i] = '\0';
    return strup;
}

char *up_case(const char *str)
{
   static char *strup;

   strup = strdup(str);
   strup[0] = UPPER(strup[0]);
    return strup;
} 
/*
 * Adds a help page to the list if it is not a duplicate of an existing page.
 * Page is insert-sorted by keyword.			-Thoric
 * (The reason for sorting is to keep do_hlist looking nice)
 */
void add_help( HELP_DATA *pHelp )
{
    HELP_DATA *tHelp;
/* Not in use, I'm guessing this function is not working.
    char buf[MAX_STRING_LENGTH];
*/
    int match;

    for ( tHelp = first_help; tHelp; tHelp = tHelp->next )
/*
	if ( pHelp->level == tHelp->level
	&&  !strcmp(pHelp->keyword, tHelp->keyword) )
	{
	    sprintf(buf, "Duplicate %s. Deleting Help.\n\r",pHelp->keyword);
	    bug(buf,0);
	    STRFREE( pHelp->text );
	    STRFREE( pHelp->keyword );
	    DISPOSE( pHelp );
	    return;
	}
	else
*/
	if ( (match=strcmp(pHelp->keyword[0]=='\'' ? pHelp->keyword+1 : pHelp->keyword,
			   tHelp->keyword[0]=='\'' ? tHelp->keyword+1 : tHelp->keyword)) < 0
	||   (match == 0 && pHelp->level > tHelp->level) )
	{
	    if ( !tHelp->prev )
		first_help	  = pHelp;
	    else
		tHelp->prev->next = pHelp;
	    pHelp->prev		  = tHelp->prev;
	    pHelp->next		  = tHelp;
	    tHelp->prev		  = pHelp;
	    break;
	}

    if ( !tHelp )
	LINK( pHelp, first_help, last_help, next, prev );

    top_help++;
}


/*
 * Load a help section.
 */
void load_helps( FILE *fp )
{
    HELP_DATA *pHelp;

    for ( ; ; )
    {
	CREATE( pHelp, HELP_DATA, 1 );
	pHelp->level	= fread_number( fp );
	pHelp->keyword	= fread_string( fp );
	if ( pHelp->keyword[0] == '$' )
	    break;
	pHelp->text	= fread_string( fp );
	if ( pHelp->keyword[0] == '\0' )
	{
	    STRFREE( pHelp->text );
	    STRFREE( pHelp->keyword );
	    DISPOSE( pHelp );
	    continue;
	}

	if ( !str_cmp( pHelp->keyword, "greeting" ) )
	    help_greeting = pHelp->text;
	add_help( pHelp );
    }
    return;
}


/*

 * Snarf a mob section.

 */

void load_mobiles( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( ; ; )
    {
	int vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_mobiles: # not found.", 0 );
	    exit( 1 );
	}
	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_mob_index( vnum ) != NULL )
	{
	    bug( "Load_mobiles: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pMobIndex			= alloc_perm( sizeof(*pMobIndex) );
	pMobIndex->vnum			= vnum;
	pMobIndex->player_name		= fread_string( fp );
	pMobIndex->short_descr		= fread_string( fp );
	pMobIndex->long_descr		= fread_string( fp );
	pMobIndex->description		= fread_string( fp );

	pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);
	pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);

	pMobIndex->act			= fread_number( fp ) | ACT_IS_NPC;
	pMobIndex->affected_by		= fread_number( fp );
	pMobIndex->itemaffect		= 0;
	pMobIndex->alignment		= fread_number( fp );
	letter				= fread_letter( fp );
	pMobIndex->level		= number_fuzzy( fread_number( fp ) );

	/*
	 * The unused stuff is for imps who want to use the old-style
	 * stats-in-files method.
	 */
	pMobIndex->hitroll		= fread_number( fp );	/* Unused */
	pMobIndex->ac			= fread_number( fp );	/* Unused */
	pMobIndex->hitnodice		= fread_number( fp );	/* Unused */
	/* 'd'		*/		  fread_letter( fp );	/* Unused */
	pMobIndex->hitsizedice		= fread_number( fp );	/* Unused */
	/* '+'		*/		  fread_letter( fp );	/* Unused */
	pMobIndex->hitplus		= fread_number( fp );	/* Unused */
	pMobIndex->damnodice		= fread_number( fp );	/* Unused */
	/* 'd'		*/		  fread_letter( fp );	/* Unused */
	pMobIndex->damsizedice		= fread_number( fp );	/* Unused */
	/* '+'		*/		  fread_letter( fp );	/* Unused */
	pMobIndex->damplus		= fread_number( fp );	/* Unused */
	pMobIndex->gold			= fread_number( fp );	/* Unused */
	/* xp can't be used! */		  fread_number( fp );	/* Unused */
	/* position	*/		  fread_number( fp );	/* Unused */
	/* start pos	*/		  fread_number( fp );	/* Unused */

	/*
	 * Back to meaningful values.
	 */
	pMobIndex->sex			= fread_number( fp );
	if ( letter != 'S' )
	{
	    bug( "Load_mobiles: vnum %d non-S.", vnum );
	    exit( 1 );
	}
	iHash			= vnum % MAX_KEY_HASH;
	pMobIndex->next		= mob_index_hash[iHash];
	mob_index_hash[iHash]	= pMobIndex;
	top_mob_index++;
    }
    return;
}







/*

 * Snarf an obj section.

 */

void load_objects( FILE *fp )

{

    OBJ_INDEX_DATA *pObjIndex;



    for ( ; ; )

    {

	int vnum;

	char letter;

	int iHash;



	letter				= fread_letter( fp );

	if ( letter != '#' )

	{

	    bug( "Load_objects: # not found.", 0 );

	    exit( 1 );

	}



	vnum				= fread_number( fp );

	if ( vnum == 0 )

	    break;



	fBootDb = FALSE;

	if ( get_obj_index( vnum ) != NULL )

	{

	    bug( "Load_objects: vnum %d duplicated.", vnum );

	    exit( 1 );

	}

	fBootDb = TRUE;



	pObjIndex			= alloc_perm( sizeof(*pObjIndex) );

	pObjIndex->vnum			= vnum;

	pObjIndex->name			= fread_string( fp );

	pObjIndex->short_descr		= fread_string( fp );

	pObjIndex->description		= fread_string( fp );

	/* Action description */	  fread_string( fp );



	pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);

	pObjIndex->description[0]	= UPPER(pObjIndex->description[0]);



	pObjIndex->item_type		= fread_number( fp );

	pObjIndex->extra_flags		= fread_number( fp );

	pObjIndex->wear_flags		= fread_number( fp );

	pObjIndex->value[0]		= fread_number( fp );

	pObjIndex->value[1]		= fread_number( fp );

	pObjIndex->value[2]		= fread_number( fp );

	pObjIndex->value[3]		= fread_number( fp );

	pObjIndex->weight		= fread_number( fp );

	pObjIndex->cost			= fread_number( fp );	/* Unused */

	pObjIndex->affected		= NULL;

	pObjIndex->extra_descr		= NULL;

	pObjIndex->chpoweron		= NULL;

	pObjIndex->chpoweroff		= NULL;

	pObjIndex->chpoweruse		= NULL;

	pObjIndex->victpoweron		= NULL;

	pObjIndex->victpoweroff		= NULL;

	pObjIndex->victpoweruse		= NULL;

	pObjIndex->spectype		= 0;

	pObjIndex->specpower		= 0;

	/* Cost per day */		  fread_number( fp );

/*

	if ( pObjIndex->item_type == ITEM_POTION )

	    SET_BIT(pObjIndex->extra_flags, ITEM_NODROP);

*/

	for ( ; ; )

	{

	    char letter;



	    letter = fread_letter( fp );



	    if ( letter == 'A' )

	    {

		AFFECT_DATA *paf;



		paf			= alloc_perm( sizeof(*paf) );

		paf->type		= -1;

		paf->duration		= -1;

		paf->location		= fread_number( fp );

		paf->modifier		= fread_number( fp );

		paf->bitvector		= 0;

		paf->next		= pObjIndex->affected;

		pObjIndex->affected	= paf;

		top_affect++;

	    }



	    else if ( letter == 'E' )

	    {

		EXTRA_DESCR_DATA *ed;



		ed			= alloc_perm( sizeof(*ed) );

		ed->keyword		= fread_string( fp );

		ed->description		= fread_string( fp );

		ed->next		= pObjIndex->extra_descr;

		pObjIndex->extra_descr	= ed;

		top_ed++;

	    }



	    else if ( letter == 'Q' )

	    {

		pObjIndex->chpoweron	= fread_string( fp );

		pObjIndex->chpoweroff	= fread_string( fp );

		pObjIndex->chpoweruse	= fread_string( fp );

		pObjIndex->victpoweron	= fread_string( fp );

		pObjIndex->victpoweroff	= fread_string( fp );

		pObjIndex->victpoweruse	= fread_string( fp );

		pObjIndex->spectype	= fread_number( fp );

		pObjIndex->specpower	= fread_number( fp );

	    }



	    else

	    {

		ungetc( letter, fp );

		break;

	    }

	}



	/*

	 * Translate spell "slot numbers" to internal "skill numbers."

	 */

	switch ( pObjIndex->item_type )

	{

	case ITEM_PILL:

	case ITEM_POTION:

	case ITEM_SCROLL:

	    pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );

	    pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );

	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );

	    break;



	case ITEM_STAFF:

	case ITEM_WAND:

	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );

	    break;

	}



	iHash			= vnum % MAX_KEY_HASH;

	pObjIndex->next		= obj_index_hash[iHash];

	obj_index_hash[iHash]	= pObjIndex;

	top_obj_index++;

    }



    return;

}







/*

 * Snarf a reset section.

 */

void load_resets( FILE *fp )

{

    RESET_DATA *pReset;



    if ( area_last == NULL )

    {

	bug( "Load_resets: no #AREA seen yet.", 0 );

	exit( 1 );

    }



    for ( ; ; )

    {

	ROOM_INDEX_DATA *pRoomIndex;

	EXIT_DATA *pexit;

	char letter;



	if ( ( letter = fread_letter( fp ) ) == 'S' )

	    break;



	if ( letter == '*' )

	{

	    fread_to_eol( fp );

	    continue;

	}



	pReset		= alloc_perm( sizeof(*pReset) );

	pReset->command	= letter;

	/* if_flag */	  fread_number( fp );

	pReset->arg1	= fread_number( fp );

	pReset->arg2	= fread_number( fp );

	pReset->arg3	= (letter == 'G' || letter == 'R')

			    ? 0 : fread_number( fp );

			  fread_to_eol( fp );



	/*

	 * Validate parameters.

	 * We're calling the index functions for the side effect.

	 */

	switch ( letter )

	{

	default:

	    bug( "Load_resets: bad command '%c'.", letter );

	    exit( 1 );

	    break;



	case 'M':

	    get_mob_index  ( pReset->arg1 );

	    get_room_index ( pReset->arg3 );

	    break;



	case 'O':

	    get_obj_index  ( pReset->arg1 );

	    get_room_index ( pReset->arg3 );

	    break;



	case 'P':

	    get_obj_index  ( pReset->arg1 );

	    get_obj_index  ( pReset->arg3 );

	    break;



	case 'G':

	case 'E':

	    get_obj_index  ( pReset->arg1 );

	    break;



	case 'D':

	    pRoomIndex = get_room_index( pReset->arg1 );



	    if ( pReset->arg2 < 0

	    ||   pReset->arg2 > 5

	    || ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL

	    || !IS_SET( pexit->exit_info, EX_ISDOOR ) )

	    {

		bug( "Load_resets: 'D': exit %d not door.", pReset->arg2 );

		exit( 1 );

	    }



	    if ( pReset->arg3 < 0 || pReset->arg3 > 2 )

	    {

		bug( "Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );

		exit( 1 );

	    }



	    break;



	case 'R':

	    pRoomIndex		= get_room_index( pReset->arg1 );



	    if ( pReset->arg2 < 0 || pReset->arg2 > 6 )

	    {

		bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );

		exit( 1 );

	    }



	    break;

	}



	if ( area_last->reset_first == NULL )

	    area_last->reset_first	= pReset;

	if ( area_last->reset_last  != NULL )

	    area_last->reset_last->next	= pReset;

	    

	area_last->reset_last	= pReset;

	pReset->next		= NULL;

	top_reset++;

    }



    return;

}







/*

 * Snarf a room section.

 */

void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;

    if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }
    for ( ; ; )
    {
	int vnum;
	char letter;
	int door;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_rooms: # not found.", 0 );
	    exit( 1 );
	}
	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_room_index( vnum ) != NULL )
	{
	    bug( "Load_rooms: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pRoomIndex			= alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->first_person	= NULL;
	pRoomIndex->last_person		= NULL;
	pRoomIndex->first_content	= NULL;
	pRoomIndex->last_content	= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->first_extradesc	= NULL;
	pRoomIndex->last_extradesc	= NULL;
	pRoomIndex->area		= area_last;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= fread_string( fp );
	if( pRoomIndex->vnum != 3001 )
	 pRoomIndex->description	= format_string(fread_string( fp ));
	else if( pRoomIndex->vnum == 3001 )
	 pRoomIndex->description	= fread_string( fp );
	/* Area number */		  fread_number( fp );
	pRoomIndex->room_flags		= fread_number( fp );
	pRoomIndex->sector_type		= fread_number( fp );
	pRoomIndex->light		= 0;
	pRoomIndex->blood		= 0;
	pRoomIndex->roomtext		= NULL;
	pRoomIndex->first_exit		= NULL;
	pRoomIndex->last_exit		= NULL;
	for ( door = 0; door <= 4; door++ )
	{
	    pRoomIndex->track[door] 	= str_dup( "" );
	    pRoomIndex->track_dir[door] = 0;
	}
	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;
	for ( ; ; )
	{
	    letter = fread_letter( fp );
	    if ( letter == 'S' )
		break;

	    if ( letter == 'D' )
	    {
		EXIT_DATA *pexit;
		int locks;

		door = fread_number( fp );
		if ( door < 0 || door > 5 )
		{
		    bug( "Fread_rooms: vnum %d has bad door number.", vnum );
		    exit( 1 );
		}

		pexit			= alloc_perm( sizeof(*pexit) );
		pexit->description	= fread_string( fp );
		pexit->keyword		= fread_string( fp );
		pexit->exit_info	= 0;
		locks			= fread_number( fp );
		pexit->key		= fread_number( fp );
		pexit->vnum		= fread_number( fp );

		switch ( locks )
		{
		case 1: pexit->exit_info = EX_ISDOOR;                break;
		case 2: pexit->exit_info = EX_ISDOOR | EX_PICKPROOF; break;
		}

		pRoomIndex->exit[door]	= pexit;
		top_exit++;
	    }
	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pRoomIndex->extra_descr;
		pRoomIndex->extra_descr	= ed;
		top_ed++;
	    }
	    else if ( letter == 'T' )
	    {
		ROOMTEXT_DATA *rt;

		rt			= alloc_perm( sizeof(*rt) );
		rt->input		= fread_string( fp );
		rt->output		= fread_string( fp );
		rt->choutput		= fread_string( fp );
		rt->name		= fread_string( fp );
		rt->type		= fread_number( fp );
		rt->power		= fread_number( fp );
		rt->mob			= fread_number( fp );
		rt->next		= pRoomIndex->roomtext;
		pRoomIndex->roomtext	= rt;
		top_rt++;
	    }
	    else
	    {
		bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
		exit( 1 );
	    }
	}
	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];

	room_index_hash[iHash]	= pRoomIndex;

	pRoomIndex->next_room	= room_list;
	room_list		= pRoomIndex;
	top_room++;
    }
    return;
}


/*

 * Snarf spec proc declarations.

 */

void load_specials( FILE *fp )

{

    for ( ; ; )

    {

	MOB_INDEX_DATA *pMobIndex;

	char letter;



	switch ( letter = fread_letter( fp ) )

	{

	default:

	    bug( "Load_specials: letter '%c' not *MS.", letter );

	    exit( 1 );



	case 'S':

	    return;



	case '*':

	    break;



	case 'M':

	    pMobIndex		= get_mob_index	( fread_number ( fp ) );

	    pMobIndex->spec_fun	= spec_lookup	( fread_word   ( fp ) );

	    if ( pMobIndex->spec_fun == 0 )

	    {

		bug( "Load_specials: 'M': vnum %d.", pMobIndex->vnum );

		exit( 1 );

	    }

	    break;

	}



	fread_to_eol( fp );

    }

}







/*

 * Snarf notes file.

 */

void load_notes( void )

{

    FILE *fp;

    NOTE_DATA *pnotelast;



    if ( ( fp = fopen( NOTE_FILE, "r" ) ) == NULL )

	return;



    pnotelast = NULL;

    for ( ; ; )

    {

	NOTE_DATA *pnote;

	char letter;



	do

	{

	    letter = getc( fp );

	    if ( feof(fp) )

	    {

		fclose( fp );

		return;

	    }

	}

	while ( isspace(letter) );

	ungetc( letter, fp );



	pnote		= alloc_perm( sizeof(*pnote) );



	if ( str_cmp( fread_word( fp ), "sender" ) )

	    break;

	pnote->sender	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "date" ) )

	    break;

	pnote->date	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "to" ) )

	    break;

	pnote->to_list	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "subject" ) )

	    break;

	pnote->subject	= fread_string( fp );



	if ( str_cmp( fread_word( fp ), "text" ) )

	    break;

	pnote->text	= fread_string( fp );



	/* Fix from the Themoog from Xania */

	pnote->next	= NULL;



	if ( note_list == NULL )

	    note_list		= pnote;

	else

	    pnotelast->next	= pnote;



	pnotelast	= pnote;

    }



    strcpy( strArea, NOTE_FILE );

    fpArea = fp;

    bug( "Load_notes: bad key word.", 0 );

    exit( 1 );

    return;

}







/*

 * Translate all room exits from virtual to real.

 * Has to be done after all rooms are read in.

 * Check for bad reverse exits.

 */

void fix_exits( void )

{

    extern const sh_int rev_dir [];

    char buf[MAX_STRING_LENGTH];

    ROOM_INDEX_DATA *pRoomIndex;

    ROOM_INDEX_DATA *to_room;

    EXIT_DATA *pexit;

    EXIT_DATA *pexit_rev;

    int iHash;

    int door;



    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )

    {

	for ( pRoomIndex  = room_index_hash[iHash];

	      pRoomIndex != NULL;

	      pRoomIndex  = pRoomIndex->next )

	{

	    bool fexit;



	    fexit = FALSE;

	    for ( door = 0; door <= 5; door++ )

	    {

		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )

		{

		    fexit = TRUE;

		    if ( pexit->vnum <= 0 )

			pexit->to_room = NULL;

		    else

			pexit->to_room = get_room_index( pexit->vnum );

		}

	    }



	    if ( !fexit )

		SET_BIT( pRoomIndex->room_flags, ROOM_NO_MOB );

	}

    }



    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )

    {

	for ( pRoomIndex  = room_index_hash[iHash];

	      pRoomIndex != NULL;

	      pRoomIndex  = pRoomIndex->next )

	{

	    for ( door = 0; door <= 5; door++ )

	    {

		if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL

		&&   ( to_room   = pexit->to_room               ) != NULL

		&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL

		&&   pexit_rev->to_room != pRoomIndex )

		{

		    sprintf( buf, "Fix_exits: %d:%d -> %d:%d -> %d.",

			pRoomIndex->vnum, door,

			to_room->vnum,    rev_dir[door],

			(pexit_rev->to_room == NULL)

			    ? 0 : pexit_rev->to_room->vnum );

/*		    bug( buf, 0 ); */

		}

	    }

	}

    }



    return;

}







/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
      CHAR_DATA *pch;
      for ( paf = pArea->affected; paf != NULL; paf = paf_next )
      { 
        CHAR_DATA *rch;
              
        paf_next    = paf->next;
        if( paf->duration > 0 )
            paf->duration--;
        else if ( paf->duration < 0 )
            continue;
        else
        {
            char *msg_off = NULL;
            if( ( paf->type > 0 ) && ( paf->type < MAX_SKILL ) )
            {
             if ( paf_next == NULL )
              msg_off = skill_table[paf->type].msg_off;
             else if ( (paf_next->type != paf->type) || (paf_next->duration>0))
              msg_off = skill_table[paf->type].msg_off;
            }
            else if ( paf->type >= MAX_SKILL )
            {
             bug("Invalid paf->type (%d)", paf->type);
            }
            if ( msg_off != NULL )
            {
            if( paf->type > 0 && skill_table[paf->type].msg_off)
            {
             for( rch = char_list; rch != NULL; rch = rch->next)
             {
              if( rch->in_room == NULL)
               continue;
              if( rch->in_room->area == NULL)
               continue;
              if( rch->in_room->area != pArea)
               continue;
              send_to_char( skill_table[paf->type].msg_off, rch);
              send_to_char( "\n\r", rch );
             }
            }
        area_affect_remove( pArea, paf );
        }
      }
    }
	if ( ++pArea->age < 3 )
	    continue;

	/*
	 * Check for PC's.
	 */
	if ( pArea->nplayer > 0 && pArea->age == 15 - 1 )
	{
	    for ( pch = char_list; pch != NULL; pch = pch->next )
	    {
		if ( !IS_NPC(pch)
		&&   IS_AWAKE(pch)
		&&   pch->in_room != NULL
		&&   pch->in_room->area == pArea )
		{
		    send_to_char( "You hear an agonised scream in the distance.\n\r",
			pch );
		}
	    }
	}

	/*
	 * Check age and reset.
	 * Note: Mud School resets every 3 minutes (not 15).
	 */
	if ( pArea->nplayer == 0 || pArea->age >= 15 )
	{
	    ROOM_INDEX_DATA *pRoomIndex;

	    reset_area( pArea );
	    pArea->age = number_range( 0, 3 );
	    pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 3;
	}
    }
    return;
}

/*

 * Reset one area.

 */

void reset_area( AREA_DATA *pArea )

{

    RESET_DATA *pReset;

    CHAR_DATA *mob;

    bool last;

    int level;



    mob 	= NULL;

    last	= TRUE;

    level	= 0;

    for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )

    {

	ROOM_INDEX_DATA *pRoomIndex;

	MOB_INDEX_DATA *pMobIndex;

	OBJ_INDEX_DATA *pObjIndex;

	OBJ_INDEX_DATA *pObjToIndex;

	EXIT_DATA *pexit;

	OBJ_DATA *obj;

	OBJ_DATA *obj_to;



	switch ( pReset->command )

	{

	default:

	    bug( "Reset_area: bad command %c.", pReset->command );

	    break;



	case 'M':

	    if ( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'M': bad vnum %d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )

	    {

		bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );

		continue;

	    }



	    level = URANGE( 0, pMobIndex->level - 2, LEVEL_HERO );

	    if ( pMobIndex->count >= pReset->arg2 )

	    {

		last = FALSE;

		break;

	    }



	    mob = create_mobile( pMobIndex );



	    /*

	     * Check for pet shop.

	     */

	    {

		ROOM_INDEX_DATA *pRoomIndexPrev;

		pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );

		if ( pRoomIndexPrev != NULL

		&&   IS_SET(pRoomIndexPrev->room_flags, ROOM_PET_SHOP) )

		    SET_BIT(mob->act, ACT_PET);

	    }



	    if ( room_is_dark( pRoomIndex ) )

		SET_BIT(mob->affected_by, AFF_INFRARED);



	    char_to_room( mob, pRoomIndex );

	    level = URANGE( 0, mob->level - 2, LEVEL_HERO );

	    last  = TRUE;

	    break;



	case 'O':

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'O': bad vnum %d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )

	    {

		bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );

		continue;

	    }



	    if ( pArea->nplayer > 0

	    ||   count_obj_list( pObjIndex, pRoomIndex->contents ) > 0 )

	    {

		last = FALSE;

		break;

	    }



	    obj       = create_object( pObjIndex, number_range( 1,50 ) );

	    obj->cost = 0;

	    obj_to_room( obj, pRoomIndex );

	    last = TRUE;

	    break;



	case 'P':

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'P': bad vnum %d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pObjToIndex = get_obj_index( pReset->arg3 ) ) == NULL )

	    {

		bug( "Reset_area: 'P': bad vnum %d.", pReset->arg3 );

		continue;

	    }



	    if ( pArea->nplayer > 0

	    || ( obj_to = get_obj_type( pObjToIndex ) ) == NULL

	    ||   obj_to->in_room == NULL

	    ||   count_obj_list( pObjIndex, obj_to->contains ) > 0 )

	    {

		last = FALSE;

		break;

	    }

	    

	    obj = create_object( pObjIndex, number_range( 1,50 ) );

	    obj_to_obj( obj, obj_to );

	    last = TRUE;

	    break;



	case 'G':

	case 'E':

	    if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'E' or 'G': bad vnum %d.", pReset->arg1 );

		continue;

	    }



	    if ( !last )

		break;



	    if ( mob == NULL )

	    {

		bug( "Reset_area: 'E' or 'G': null mob for vnum %d.",

		    pReset->arg1 );

		last = FALSE;

		break;

	    }
	obj = create_object( pObjIndex, number_range( 1,50 ) );

	    obj_to_char( obj, mob );

	    if ( pReset->command == 'E' )

		equip_char( mob, obj, pReset->arg3 );

	    last = TRUE;

	    break;



	case 'D':

	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'D': bad vnum %d.", pReset->arg1 );

		continue;

	    }



	    if ( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )

		break;



	    switch ( pReset->arg3 )

	    {

	    case 0:

		REMOVE_BIT( pexit->exit_info, EX_CLOSED );

		REMOVE_BIT( pexit->exit_info, EX_LOCKED );

		break;



	    case 1:

		SET_BIT(    pexit->exit_info, EX_CLOSED );

		REMOVE_BIT( pexit->exit_info, EX_LOCKED );

		break;



	    case 2:

		SET_BIT(    pexit->exit_info, EX_CLOSED );

		SET_BIT(    pexit->exit_info, EX_LOCKED );

		break;

	    }



	    last = TRUE;

	    break;



	case 'R':

	    if ( ( pRoomIndex = get_room_index( pReset->arg1 ) ) == NULL )

	    {

		bug( "Reset_area: 'R': bad vnum %d.", pReset->arg1 );

		continue;

	    }



	    {

		int d0;

		int d1;



		for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )

		{

		    d1                   = number_range( d0, pReset->arg2-1 );

		    pexit                = pRoomIndex->exit[d0];

		    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];

		    pRoomIndex->exit[d1] = pexit;

		}

	    }

	    break;

	}

    }



    return;

}







/*

 * Create an instance of a mobile.

 */

CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )

{

    CHAR_DATA *mob;

    int tempvalue;


    if ( pMobIndex == NULL )

    {

	bug( "Create_mobile: NULL pMobIndex.", 0 );

/* SaTaN's mob load fix for now */

//	exit( 1 );
	pMobIndex=get_mob_index( 30013 );

    }



    if ( char_free == NULL )

    {

	mob		= alloc_perm( sizeof(*mob) );

    }

    else

    {

	mob		= char_free;

	char_free	= char_free->next;

    }



    clear_char( mob );

    mob->pIndexData	= pMobIndex;



    mob->hunting	= str_dup( "" );


    mob->clan		= str_dup( "" );

    mob->morph		= str_dup( "" );

    mob->createtime	= str_dup( "" );

    mob->lasttime	= str_dup( "" );

    mob->lasthost	= str_dup( "" );

    mob->pload		= str_dup( "" );

    mob->prompt		= str_dup( "" );

    mob->cprompt	= str_dup( "" );



    mob->name		= pMobIndex->player_name;

    mob->short_descr	= pMobIndex->short_descr;

    mob->long_descr	= pMobIndex->long_descr;

    mob->description	= pMobIndex->description;



    mob->spec_fun	= pMobIndex->spec_fun;



    mob->home		= 3001;

    mob->level		= number_fuzzy( pMobIndex->level );

    mob->act		= pMobIndex->act;

    mob->affected_by	= pMobIndex->affected_by;

    mob->alignment	= pMobIndex->alignment;

    mob->sex		= pMobIndex->sex;



    mob->armor		= interpolate( mob->level, 100, -100 );



    tempvalue		= mob->level * 12 + number_range(

				mob->level * mob->level / 4,

				mob->level * mob->level );

    if (tempvalue > 100000) mob->max_hit = 100000;

    else mob->max_hit	= tempvalue;



    mob->hit		= mob->max_hit;



    mob->hitroll	= mob->level;

    mob->damroll	= mob->level;
    mob->stance[1]        = 200;
    mob->stance[2]        = 200;
    mob->stance[3]        = 200;
    mob->stance[4]        = 200;
    mob->stance[5]        = 200;
    mob->stance[6]        = 200;
    mob->stance[7]        = 200;
    mob->stance[8]        = 200;
    mob->stance[9]        = 200;
    mob->stance[10]       = 200;


    /*

     * Insert in list.

     */

    mob->next		= char_list;

    char_list		= mob;

    pMobIndex->count++;

    return mob;

}







/*

 * Create an instance of an object.

 */

OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )

{

    static OBJ_DATA obj_zero;

    OBJ_DATA *obj;



    if ( pObjIndex == NULL )

    {

	bug( "Create_object: NULL pObjIndex.", 0 );

	exit( 1 );

    }



    if ( obj_free == NULL )

    {

	obj		= alloc_perm( sizeof(*obj) );

    }

    else

    {

	obj		= obj_free;

	obj_free	= obj_free->next;

    }



    *obj		= obj_zero;

    obj->pIndexData	= pObjIndex;

    obj->in_room	= NULL;

    obj->level		= level;

    obj->wear_loc	= -1;



    obj->name		= pObjIndex->name;

    obj->short_descr	= pObjIndex->short_descr;

    obj->description	= pObjIndex->description;



    if (pObjIndex->chpoweron != NULL)

    {

	obj->chpoweron    = pObjIndex->chpoweron;

	obj->chpoweroff   = pObjIndex->chpoweroff;

	obj->chpoweruse   = pObjIndex->chpoweruse;

	obj->victpoweron  = pObjIndex->victpoweron;

	obj->victpoweroff = pObjIndex->victpoweroff;

	obj->victpoweruse = pObjIndex->victpoweruse;

	obj->spectype     = pObjIndex->spectype;

	obj->specpower    = pObjIndex->specpower;

    }

    else

    {

	obj->chpoweron    = str_dup( "(null)" );

	obj->chpoweroff   = str_dup( "(null)" );

	obj->chpoweruse   = str_dup( "(null)" );

	obj->victpoweron  = str_dup( "(null)" );

	obj->victpoweroff = str_dup( "(null)" );

	obj->victpoweruse = str_dup( "(null)" );

	obj->spectype     = 0;

	obj->specpower    = 0;

    }

    obj->questmaker     = str_dup( "" );

    obj->questowner     = str_dup( "" );



    obj->chobj		= NULL;



    obj->quest		= 0;

    obj->points		= 0;



    obj->item_type	= pObjIndex->item_type;

    obj->extra_flags	= pObjIndex->extra_flags;

    obj->wear_flags	= pObjIndex->wear_flags;

    obj->value[0]	= pObjIndex->value[0];

    obj->value[1]	= pObjIndex->value[1];

    obj->value[2]	= pObjIndex->value[2];

    obj->value[3]	= pObjIndex->value[3];

    obj->weight		= pObjIndex->weight;

    obj->cost		= number_fuzzy( 10 )

			* number_fuzzy( level ) * number_fuzzy( level );



	if (obj->pIndexData->vnum >= 29600 && obj->pIndexData->vnum <= 29625)

	{

	obj->condition = 100;

	obj->toughness = 100;

	SET_BIT(obj->spectype,SITEM_DROW);

	SET_BIT(obj->quest,QUEST_RELIC);

	} else {



    if (obj->pIndexData->vnum >=  29500 && obj->pIndexData->vnum <= 29599)

    {

	SET_BIT(obj->quest, QUEST_ARTIFACT);

    	obj->condition	= 100;

    	obj->toughness	= 100;

    	obj->resistance	= 1;

	obj->level	= 60;

	obj->cost	= 1000000;

    }

    else if (obj->pIndexData->vnum >= 29666 

             && obj->pIndexData->vnum <= 29679

	     && !IS_SET(obj->spectype, SITEM_PDEMONIC))

	{

    SET_BIT(obj->spectype, SITEM_PDEMONIC);

    SET_BIT(obj->quest, QUEST_RELIC);

        obj->condition  = 100;

        obj->toughness  = 100;

        obj->resistance = 1; 

	}

    else if (obj->pIndexData->vnum >= 27650 && obj->pIndexData->vnum <= 29711)

    {

	SET_BIT(obj->quest, QUEST_RELIC);

    	obj->condition	= 100;

    	obj->toughness	= 100;

    	obj->resistance	= 1;

    }

    else

    {

    	obj->condition	= 100;

    	obj->toughness	= 5;

    	obj->resistance	= 25;

    }

}

    /*

     * Mess with object properties.

     */

    switch ( obj->item_type )

    {

    default:

	bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );

	break;



    case ITEM_LIGHT:

    case ITEM_TREASURE:

    case ITEM_FURNITURE:

    case ITEM_TRASH:

    case ITEM_CONTAINER:

    case ITEM_DRINK_CON:

    case ITEM_KEY:

    case ITEM_FOOD:

    case ITEM_BOAT:

    case ITEM_CORPSE_NPC:

    case ITEM_CORPSE_PC:

    case ITEM_FOUNTAIN:

    case ITEM_PORTAL:

    case ITEM_EGG:

    case ITEM_VOODOO:

    case ITEM_STAKE:

    case ITEM_MISSILE:

    case ITEM_AMMO:

    case ITEM_QUEST:

    case ITEM_QUESTCARD:

    case ITEM_QUESTMACHINE:

    case ITEM_SYMBOL:

    case ITEM_BOOK:

    case ITEM_PAGE:

    case ITEM_TOOL:

    case ITEM_COPPER:

    case ITEM_IRON:

    case ITEM_STEEL:

    case ITEM_ADAMANTITE:

    case ITEM_GEMSTONE:

    case ITEM_HILT:

	break;



    case ITEM_SCROLL:

	obj->value[0]	= number_fuzzy( obj->value[0] );

	break;



    case ITEM_WAND:

    case ITEM_STAFF:

	obj->value[0]	= number_fuzzy( obj->value[0] );

	obj->value[1]	= number_fuzzy( obj->value[1] );

	obj->value[2]	= obj->value[1];

	break;



    case ITEM_WEAPON:

	if (!IS_SET(obj->quest, QUEST_ARTIFACT)

	    && !IS_SET(obj->quest, QUEST_RELIC))

	{

	    obj->value[1]	= number_range( 1, 10 );

	    obj->value[2]	= number_range((obj->value[1]+1),(obj->value[1]*2));

	}

	break;



    case ITEM_ARMOR:

	if (!IS_SET(obj->quest, QUEST_ARTIFACT)

	    && !IS_SET(obj->quest, QUEST_RELIC))

	    obj->value[0]	= number_range( 5, 15 );

	break;



    case ITEM_POTION:

    case ITEM_PILL:

	obj->value[0]	= number_fuzzy( number_fuzzy( obj->value[0] ) );

	break;



    case ITEM_MONEY:

	obj->value[0]	= obj->cost;

	break;

    }

    if( !IS_OBJ_STAT(obj,ITEM_FIXED_DUR) )
    {
     SET_BIT(obj->extra_flags,ITEM_FIXED_DUR);
     obj->durability = 100;
    }

    obj->next		= object_list;

    object_list		= obj;

    pObjIndex->count++;



    return obj;

}







/*

 * Clear a new character.

 */

void clear_char( CHAR_DATA *ch )

{

    static CHAR_DATA ch_zero;



    *ch				= ch_zero;

    ch->name			= &str_empty[0];

    ch->short_descr		= &str_empty[0];

    ch->long_descr		= &str_empty[0];

    ch->description		= &str_empty[0];

    ch->clan			= &str_empty[0];

    ch->morph			= &str_empty[0];

    ch->createtime		= &str_empty[0];

    ch->lasthost		= &str_empty[0];

    ch->lasttime		= &str_empty[0];

    ch->pload			= &str_empty[0];

    ch->prompt			= &str_empty[0];

    ch->cprompt			= &str_empty[0];

    ch->hunting			= &str_empty[0];



    ch->logon			= current_time;

    ch->armor			= 100;

    ch->position		= POS_STANDING;

    ch->practice		= 0;

    ch->hit			= 1000;

    ch->max_hit			= 1000;

    ch->mana			= 1500;

    ch->max_mana		= 1500;

    ch->move			= 1500;

    ch->max_move		= 1500;

    ch->master			= NULL;

    ch->leader			= NULL;

    ch->fighting		= NULL;

    ch->mount			= NULL;

    ch->wizard			= NULL;

    ch->paradox[0]              = 0;        

    ch->paradox[1]              = 0;        

    ch->paradox[2]              = 0;        

    return;

}







/*

 * Free a character.

 */

void free_char( CHAR_DATA *ch )

{

    OBJ_DATA *obj;

    OBJ_DATA *obj_next;

    AFFECT_DATA *paf;

    AFFECT_DATA *paf_next;



    for ( obj = ch->carrying; obj != NULL; obj = obj_next )

    {

	obj_next = obj->next_content;

	extract_obj( obj );

    }



    for ( paf = ch->affected; paf != NULL; paf = paf_next )

    {

	paf_next = paf->next;

	affect_remove( ch, paf );

    }



    free_string( ch->name		);

    free_string( ch->short_descr	);

    free_string( ch->long_descr		);

    free_string( ch->description	);

    free_string( ch->clan 		);

    free_string( ch->morph 		);

    free_string( ch->createtime 	);

    free_string( ch->lasttime 		);

    free_string( ch->lasthost 		);

    free_string( ch->pload 		);

    free_string( ch->prompt 		);

    free_string( ch->cprompt 		);

    free_string( ch->hunting		);



    if ( ch->pcdata != NULL )

    {

	free_string( ch->pcdata->pwd		);

	free_string( ch->pcdata->bamfin		);

	free_string( ch->pcdata->bamfout	);

	free_string( ch->pcdata->title		);

	ch->pcdata->next = pcdata_free;
	pcdata_free      = ch->pcdata;
        ch->pcdata->race->next = racedata_free;
        racedata_free    = ch->pcdata->race;

    }



    ch->next	     = char_free;

    char_free	     = ch;

    return;

}







/*

 * Get an extra description from a list.

 */

char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )

{

    for ( ; ed != NULL; ed = ed->next )

    {

	if ( is_name( name, ed->keyword ) )

	    return ed->description;

    }

    return NULL;

}







/*

 * Translates mob virtual number to its mob index struct.

 * Hash table lookup.

 */

MOB_INDEX_DATA *get_mob_index( int vnum )

{

    MOB_INDEX_DATA *pMobIndex;



    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];

	  pMobIndex != NULL;

	  pMobIndex  = pMobIndex->next )

    {

	if ( pMobIndex->vnum == vnum )

	    return pMobIndex;

    }



    if ( fBootDb )

    {

	bug( "Get_mob_index: bad vnum %d.", vnum );

	exit( 1 );

    }



    return NULL;

}







/*

 * Translates mob virtual number to its obj index struct.

 * Hash table lookup.

 */

OBJ_INDEX_DATA *get_obj_index( int vnum )

{

    OBJ_INDEX_DATA *pObjIndex;



    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];

	  pObjIndex != NULL;

	  pObjIndex  = pObjIndex->next )

    {

	if ( pObjIndex->vnum == vnum )

	    return pObjIndex;

    }



    if ( fBootDb )

    {

	bug( "Get_obj_index: bad vnum %d.", vnum );

	exit( 1 );

    }



    return NULL;

}







/*

 * Translates mob virtual number to its room index struct.

 * Hash table lookup.

 */

ROOM_INDEX_DATA *get_room_index( int vnum )

{

    ROOM_INDEX_DATA *pRoomIndex;



    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];

	  pRoomIndex != NULL;

	  pRoomIndex  = pRoomIndex->next )

    {

	if ( pRoomIndex->vnum == vnum )

	    return pRoomIndex;

    }



    if ( fBootDb )

    {

	bug( "Get_room_index: bad vnum %d.", vnum );

	exit( 1 );

    }



    return NULL;

}







/*

 * Read a letter from a file.

 */

char fread_letter( FILE *fp )

{

    char c;



    do

    {

	c = getc( fp );

    }

    while ( isspace(c) );



    return c;

}







/*

 * Read a number from a file.

 */

int fread_number( FILE *fp )

{

    int number;

    bool sign;

    char c;


    do

    {

	c = getc( fp );
    }

    while ( isspace(c) );



    number = 0;



    sign   = FALSE;


    if ( c == '+' )

    {

	c = getc( fp );

    }

    else if ( c == '-' )

    {

	sign = TRUE;

	c = getc( fp );

    }


    if ( !isdigit(c) )

    {

	bug( "Fread_number: bad format.", 0 );

	exit( 1 );

    }



    while ( isdigit(c) )

    {

	number = number * 10 + c - '0';

	c      = getc( fp );

    }



    if ( sign )

	number = 0 - number;


    if ( c == '|' )

	number += fread_number( fp );

    else if ( c != ' ' )

	ungetc( c, fp );



    return number;

}







/*

 * Read and allocate space for a string from a file.

 * These strings are read-only and shared.

 * Strings are hashed:

 *   each string prepended with hash pointer to prev string,

 *   hash code is simply the string length.

 * This function takes 40% to 50% of boot-up time.

 */

char *fread_string( FILE *fp )

{

    char *plast;

    char c;



    plast = top_string + sizeof(char *);

    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )

    {

	bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );

	exit( 1 );

    }



    /*

     * Skip blanks.

     * Read first char.

     */

    do

    {

	c = getc( fp );

    }

    while ( isspace(c) );



    if ( ( *plast++ = c ) == '~' )

	return &str_empty[0];



    for ( ;; )

    {

	/*

	 * Back off the char type lookup,

	 *   it was too dirty for portability.

	 *   -- Furey

	 */

	switch ( *plast = getc( fp ) )

	{

	default:

	    plast++;

	    break;



	case EOF:

	    bug( "Fread_string: EOF", 0 );

	    exit( 1 );

	    break;



	case '\n':

	    plast++;

	    *plast++ = '\r';

	    break;



	case '\r':

	    break;



	case '~':

	    plast++;

	    {

		union

		{

		    char *	pc;

		    char	rgc[sizeof(char *)];

		} u1;

		int ic;

		int iHash;

		char *pHash;

		char *pHashPrev;

		char *pString;



		plast[-1] = '\0';

		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );

		for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )

		{

		    for ( ic = 0; ic < sizeof(char *); ic++ )

			u1.rgc[ic] = pHash[ic];

		    pHashPrev = u1.pc;

		    pHash    += sizeof(char *);



		    if ( top_string[sizeof(char *)] == pHash[0]

		    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )

			return pHash;

		}



		if ( fBootDb )

		{

		    pString		= top_string;

		    top_string		= plast;

		    u1.pc		= string_hash[iHash];

		    for ( ic = 0; ic < sizeof(char *); ic++ )

			pString[ic] = u1.rgc[ic];

		    string_hash[iHash]	= pString;



		    nAllocString += 1;

		    sAllocString += top_string - pString;

		    return pString + sizeof(char *);

		}

		else

		{

		    return str_dup( top_string + sizeof(char *) );

		}

	    }

	}

    }

}







/*

 * Read to end of line (for comments).

 */

void fread_to_eol( FILE *fp )

{

    char c;



    do

    {

	c = getc( fp );

    }

    while ( c != '\n' && c != '\r' );



    do

    {

	c = getc( fp );

    }

    while ( c == '\n' || c == '\r' );



    ungetc( c, fp );

    return;

}







/*

 * Read one word (into static buffer).

 */

char *fread_word( FILE *fp )

{

    static char word[MAX_INPUT_LENGTH];

    char *pword;

    char cEnd;



    do

    {

	cEnd = getc( fp );

    }

    while ( isspace( cEnd ) );



    if ( cEnd == '\'' || cEnd == '"' )

    {

	pword   = word;

    }

    else

    {

	word[0] = cEnd;

	pword   = word+1;

	cEnd    = ' ';

    }



    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )

    {

	*pword = getc( fp );

	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )

	{

	    if ( cEnd == ' ' )

		ungetc( *pword, fp );

	    *pword = '\0';

	    return word;

	}

    }



    bug( "Fread_word: word too long.", 0 );

    exit( 1 );

    return NULL;

}







/*

 * Allocate some ordinary memory,

 *   with the expectation of freeing it someday.

 */

void *alloc_mem( int sMem )
{
    void *pMem;
    int iList;

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
	if ( sMem <= rgSizeList[iList] )
	    break;
    }
    if ( iList == MAX_MEM_LIST )
    {
	bug( "Alloc_mem: size %d too large.", sMem );
        log_string(last_command);
	//klinemark exit( 1 );
        auto_copyover();
    }
    if ( rgFreeList[iList] == NULL )
    {
	pMem		  = alloc_perm( rgSizeList[iList] );
    }
    else
    {
	pMem              = rgFreeList[iList];
	rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }
    return pMem;
}







/*

 * Free some memory.

 * Recycle it back onto the free list for blocks of that size.

 */

void free_mem( void *pMem, int sMem )
{
    int iList;

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
	if ( sMem <= rgSizeList[iList] )
	    break;
    }
    if ( iList == MAX_MEM_LIST )
    {
	bug( "Free_mem: size %d too large.", sMem );
	exit( 1 );
    }
    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;
    return;

}







/*

 * Allocate some permanent memory.

 * Permanent memory is never freed,

 *   pointers into it may be copied safely.

 */

void *alloc_perm( int sMem )
{
    static char *pMemPerm;
    static int iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
	sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
	bug( "Alloc_perm: %d too large.", sMem );
	exit( 1 );
    }
    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
	iMemPerm = 0;
	if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
	{
	    perror( "Alloc_perm" );
	    exit( 1 );
	}
    }
    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}







/*

 * Duplicate a string into dynamic memory.

 * Fread_strings are read-only and shared.

 */

char *str_dup( const char *str )

{

    char *str_new;



    if ( str[0] == '\0' )

	return &str_empty[0];



    if ( str >= string_space && str < top_string )

	return (char *) str;



    str_new = alloc_mem( strlen(str) + 1 );

    strcpy( str_new, str );

    return str_new;

}







/*

 * Free a string.

 * Null is legal here to simplify callers.

 * Read-only shared strings are not touched.

 */

void free_string( char *pstr )
{
    if ( pstr == NULL
    ||   pstr == &str_empty[0]
    || ( pstr >= string_space && pstr < top_string ) )
	return;
    free_mem( pstr, strlen(pstr) + 1 );
    return;
}








/*
 * Creat a new room (for online building)			-Thoric
 */
ROOM_INDEX_DATA *make_room( int vnum )
{
	ROOM_INDEX_DATA *pRoomIndex;
	int	iHash;
	int     door;

	CREATE( pRoomIndex, ROOM_INDEX_DATA, 1 );
	pRoomIndex->people	        = NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->area		= NULL;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= STRALLOC("Floating in a void");
	pRoomIndex->description		= STRALLOC("");
	pRoomIndex->room_flags		= ROOM_PROTOTYPE;
	pRoomIndex->sector_type		= 1;
	pRoomIndex->light		= 0;
	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	top_room++;

	return pRoomIndex;
}

/*
 * Create a new INDEX object (for online building)		-Thoric
 * Option to clone an existing index object.
 */
OBJ_INDEX_DATA *make_object( int vnum, int cvnum, char *name )
{
	OBJ_INDEX_DATA *pObjIndex, *cObjIndex;
	char buf[MAX_STRING_LENGTH];
	int	iHash;

	if ( cvnum > 0 )
	  cObjIndex = get_obj_index( cvnum );
	else
	  cObjIndex = NULL;
	CREATE( pObjIndex, OBJ_INDEX_DATA, 1 );
	pObjIndex->vnum			= vnum;
	pObjIndex->name			= STRALLOC( name );
	pObjIndex->affected		= NULL;
	pObjIndex->extra_descr		= NULL;
	if ( !cObjIndex )
	{
	  sprintf( buf, "A newly created %s", name );
	  pObjIndex->short_descr	= STRALLOC( buf  );
	  sprintf( buf, "Some god dropped a newly created %s here.", name );
	  pObjIndex->description	= STRALLOC( buf );
	  pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);
	  pObjIndex->description[0]	= UPPER(pObjIndex->description[0]);
	  pObjIndex->item_type		= ITEM_TRASH;
	  pObjIndex->extra_flags	= ITEM_PROTOTYPE;
	  pObjIndex->wear_flags		= 0;
	  pObjIndex->value[0]		= 0;
	  pObjIndex->value[1]		= 0;
	  pObjIndex->value[2]		= 0;
	  pObjIndex->value[3]		= 0;
	  pObjIndex->weight		= 1;
	  pObjIndex->cost		= 0;
	}
	else
	{
/* None of these are in use, guessing this function is incomplete.
	  EXTRA_DESCR_DATA *ed,  *ced;
	  AFFECT_DATA	   *paf, *cpaf;
*/

	  pObjIndex->short_descr	= QUICKLINK( cObjIndex->short_descr );
	  pObjIndex->description	= QUICKLINK( cObjIndex->description );
	  pObjIndex->item_type		= cObjIndex->item_type;
	  pObjIndex->extra_flags	= cObjIndex->extra_flags
	  				| ITEM_PROTOTYPE;
	  pObjIndex->wear_flags		= cObjIndex->wear_flags;
	  pObjIndex->value[0]		= cObjIndex->value[0];
	  pObjIndex->value[1]		= cObjIndex->value[1];
	  pObjIndex->value[2]		= cObjIndex->value[2];
	  pObjIndex->value[3]		= cObjIndex->value[3];
	  pObjIndex->weight		= cObjIndex->weight;
	  pObjIndex->cost		= cObjIndex->cost;

	}
	pObjIndex->count		= 0;
	iHash				= vnum % MAX_KEY_HASH;
	pObjIndex->next			= obj_index_hash[iHash];
	obj_index_hash[iHash]		= pObjIndex;
	top_obj_index++;

	return pObjIndex;
}

/*
 * Create a new INDEX mobile (for online building)		-Thoric
 * Option to clone an existing index mobile.
 */
MOB_INDEX_DATA *make_mobile( sh_int vnum, sh_int cvnum, char *name )
{
	MOB_INDEX_DATA *pMobIndex, *cMobIndex;
	char buf[MAX_STRING_LENGTH];
	int	iHash;

	if ( cvnum > 0 )
	  cMobIndex = get_mob_index( cvnum );
	else
	  cMobIndex = NULL;
	CREATE( pMobIndex, MOB_INDEX_DATA, 1 );
	pMobIndex->vnum			= vnum;
	pMobIndex->count		= 0;
	pMobIndex->killed		= 0;
	pMobIndex->player_name		= STRALLOC( name );
	if ( !cMobIndex )
	{
	  sprintf( buf, "A newly created %s", name );
	  pMobIndex->short_descr	= STRALLOC( buf  );
	  sprintf( buf, "Some god abandoned a newly created %s here.\n\r", name );
	  pMobIndex->long_descr		= STRALLOC( buf );
	  pMobIndex->description	= STRALLOC( "" );
	  pMobIndex->short_descr[0]	= LOWER(pMobIndex->short_descr[0]);
	  pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);
	  pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);
	  pMobIndex->act		= ACT_IS_NPC | ACT_PROTOTYPE;
	  pMobIndex->affected_by	= 0;
	  pMobIndex->spec_fun		= NULL;
	  pMobIndex->alignment		= 0;
	  pMobIndex->level		= 1;
	  pMobIndex->ac			= 0;
	  pMobIndex->gold		= 0;
	  pMobIndex->sex		= 0;
	}
	else
	{
	  pMobIndex->short_descr	= QUICKLINK( cMobIndex->short_descr );
	  pMobIndex->long_descr		= QUICKLINK( cMobIndex->long_descr );
	  pMobIndex->description	= QUICKLINK( cMobIndex->description );
	  pMobIndex->act		= cMobIndex->act | ACT_PROTOTYPE;
	  pMobIndex->affected_by	= cMobIndex->affected_by;
	  pMobIndex->spec_fun		= cMobIndex->spec_fun;
	  pMobIndex->alignment		= cMobIndex->alignment;
	  pMobIndex->level		= cMobIndex->level;
	  pMobIndex->ac			= cMobIndex->ac;
	  pMobIndex->gold		= cMobIndex->gold;
	  pMobIndex->sex		= cMobIndex->sex;
	}
	iHash				= vnum % MAX_KEY_HASH;
	pMobIndex->next			= mob_index_hash[iHash];
	mob_index_hash[iHash]		= pMobIndex;
	top_mob_index++;

	return pMobIndex;
}

/*
 * Creates a simple exit with no fields filled but rvnum and optionally
 * to_room and vnum.						-Thoric
 * Exits are inserted into the linked list based on vdir.
 */

EXIT_DATA *make_exit( ROOM_INDEX_DATA *pRoomIndex, ROOM_INDEX_DATA *to_room, sh_int door )
{
	EXIT_DATA *pexit, *texit;
	bool broke;

	CREATE( pexit, EXIT_DATA, 1 );
	pexit->vdir		= door;
	pexit->rvnum		= pRoomIndex->vnum;
	pexit->to_room		= to_room;
	if ( to_room )
	{
	    pexit->vnum = to_room->vnum;
	    texit = get_exit_to( to_room, rev_dir[door], pRoomIndex->vnum );
	    if ( texit )
	    {
		texit->rexit = pexit;
		pexit->rexit = texit;
	    }
	}
	broke = FALSE;
	for ( texit = pRoomIndex->first_exit; texit; texit = texit->next )
	   if ( door < texit->vdir )
	   {
	     broke = TRUE;
	     break;
	   }
	if ( !pRoomIndex->first_exit )
	  pRoomIndex->first_exit	= pexit;
	else
	{

	  if ( broke && texit )
	  {
	    if ( !texit->prev )
	      pRoomIndex->first_exit	= pexit;
	    else
	      texit->prev->next		= pexit;
	    pexit->prev			= texit->prev;
	    pexit->next			= texit;
	    texit->prev			= pexit;
	    top_exit++;
	    return pexit;
	  }
	  pRoomIndex->last_exit->next	= pexit;
	}
	pexit->next			= NULL;
	pexit->prev			= pRoomIndex->last_exit;
	pRoomIndex->last_exit		= pexit;
	top_exit++;
	fix_exits();
	return pexit;
}


void do_areas( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];

    AREA_DATA *pArea1;

    AREA_DATA *pArea2;

    int iArea;

    int iAreaHalf;

    WAIT_STATE(ch,10);

    iAreaHalf = (top_area + 1) / 2;

    pArea1    = area_first;

    pArea2    = area_first;

    for ( iArea = 0; iArea < iAreaHalf; iArea++ )

	pArea2 = pArea2->next;



    

   for ( iArea = 0; iArea < iAreaHalf; iArea++ )

    {

	sprintf( buf, "%-39s%-39s\n\r",

	    pArea1->name, (pArea2 != NULL) ? pArea2->name : "" );

	send_to_char( buf, ch );

	pArea1 = pArea1->next;

	if ( pArea2 != NULL )

	    pArea2 = pArea2->next;

    }



    return;

}







void do_memory( CHAR_DATA *ch, char *argument )

{

    char buf[MAX_STRING_LENGTH];



    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );

    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );

    sprintf( buf, "RmTxt   %5d\n\r", top_rt        ); send_to_char( buf, ch );

    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );

    sprintf( buf, "Exits   %5d\n\r", top_exit      ); send_to_char( buf, ch );

    sprintf( buf, "Helps   %5d\n\r", top_help      ); send_to_char( buf, ch );

    sprintf( buf, "Mobs    %5d\n\r", top_mob_index ); send_to_char( buf, ch );

    sprintf( buf, "Objs    %5d\n\r", top_obj_index ); send_to_char( buf, ch );

    sprintf( buf, "Resets  %5d\n\r", top_reset     ); send_to_char( buf, ch );

    sprintf( buf, "Rooms   %5d\n\r", top_room      ); send_to_char( buf, ch );

    sprintf( buf, "Shops   %5d\n\r", top_shop      ); send_to_char( buf, ch );



    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",

	nAllocString, sAllocString, MAX_STRING );

    send_to_char( buf, ch );



    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",

	nAllocPerm, sAllocPerm );

    send_to_char( buf, ch );



    return;

}







/*

 * Stick a little fuzz on a number.

 */

int number_fuzzy( int number )

{

    switch ( number_bits( 2 ) )

    {

    case 0:  number -= 1; break;

    case 3:  number += 1; break;

    }



    return UMAX( 1, number );

}







/*

 * Generate a random number.

 */

int number_range( int from, int to )
{
    int power;
    int number;

    if ( ( to = to - from + 1 ) <= 1 )
	return from;
    for ( power = 2; power < to; power <<= 1 )
	;
    while ( ( number = number_mm( ) & (power - 1) ) >= to )
	;
    return from + number;
}







/*

 * Generate a percentile roll.

 */

int number_percent( void )

{

    int percent;



    while ( ( percent = number_mm( ) & (128-1) ) > 99 )

	;



    return 1 + percent;

}







/*

 * Generate a random door.

 */

int number_door( void )

{

    int door;



    while ( ( door = number_mm( ) & (8-1) ) > 5 )

	;



    return door;

}







int number_bits( int width )

{

    return number_mm( ) & ( ( 1 << width ) - 1 );

}







/*

 * I've gotten too many bad reports on OS-supplied random number generators.

 * This is the Mitchell-Moore algorithm from Knuth Volume II.

 * Best to leave the constants alone unless you've read Knuth.

 * -- Furey

 */

static	int	rgiState[2+55];

void init_mm( )
{
    int *piState;
    int iState;
    piState	= &rgiState[2];
    piState[-2]	= 55 - 55;
    piState[-1]	= 55 - 24;
    piState[0]	= ((int) current_time) & ((1 << 30) - 1);
    piState[1]	= 1;
    for ( iState = 2; iState < 55; iState++ )
    {
	piState[iState] = (piState[iState-1] + piState[iState-2])
			& ((1 << 30) - 1);
    }
    return;
}

int number_mm( void )
{
    int *piState;
    int iState1;
    int iState2;
    int iRand;

    piState		= &rgiState[2];
    iState1	 	= piState[-2];
    iState2	 	= piState[-1];
    iRand	 	= (piState[iState1] + piState[iState2])
			& ((1 << 30) - 1);
    piState[iState1]	= iRand;
    if ( ++iState1 == 55 )
	iState1 = 0;
    if ( ++iState2 == 55 )
	iState2 = 0;
    piState[-2]		= iState1;
    piState[-1]		= iState2;
    return iRand >> 6;

}







/*

 * Roll some dice.

 */

int dice( int number, int size )

{

    int idice;

    int sum;



    switch ( size )

    {

    case 0: return 0;

    case 1: return number;

    }



    for ( idice = 0, sum = 0; idice < number; idice++ )

	sum += number_range( 1, size );



    return sum;

}







/*

 * Simple linear interpolation.

 */

int interpolate( int level, int value_00, int value_32 )

{

    return value_00 + level * (value_32 - value_00) / 32;

}







/*

 * Removes the tildes from a string.

 * Used for player-entered strings that go into disk files.

 */

void smash_tilde( char *str )

{

    for ( ; *str != '\0'; str++ )

    {

	if ( *str == '~' )

	    *str = '-';

    }



    return;

}

void smash_color( char *str)
{
  for(; *str != '\0'; str++ )
  {
    if( *str == '#')
    *str = ' ';
  }
 return;
}

int new_strlen( char *str)
{
 int count = 0;
 for(; *str != '\0'; str++)
  count++;
 return count;
}

/*

 * Compare strings, case insensitive.

 * Return TRUE if different

 *   (compatibility with historical functions).

 */

bool str_cmp( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Str_cmp: null astr.", 0 );
	return TRUE;
    }
    if ( bstr == NULL )
    {
	bug( "Str_cmp: null bstr.", 0 );
	return TRUE;
    }
    for ( ; *astr || *bstr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }
    return FALSE;
}


/*

 * Compare strings, case insensitive, for prefix matching.

 * Return TRUE if astr not a prefix of bstr

 *   (compatibility with historical functions).

 */

bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Strn_cmp: null astr.", 0 );
	return TRUE;
    }
    if ( bstr == NULL )
    {
	bug( "Strn_cmp: null bstr.", 0 );
	return TRUE;
    }
    for ( ; *astr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }
    return FALSE;
}







/*

 * Compare strings, case insensitive, for match anywhere.

 * Returns TRUE is astr not part of bstr.

 *   (compatibility with historical functions).

 */

bool str_infix( const char *astr, const char *bstr )

{

    int sstr1;

    int sstr2;

    int ichar;

    char c0;



    if ( ( c0 = LOWER(astr[0]) ) == '\0' )

	return FALSE;



    sstr1 = strlen(astr);

    sstr2 = strlen(bstr);



    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )

    {

	if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )

	    return FALSE;

    }



    return TRUE;

}







/*

 * Compare strings, case insensitive, for suffix matching.

 * Return TRUE if astr not a suffix of bstr

 *   (compatibility with historical functions).

 */

bool str_suffix( const char *astr, const char *bstr )

{

    int sstr1;

    int sstr2;



    sstr1 = strlen(astr);

    sstr2 = strlen(bstr);

    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )

	return FALSE;

    else

	return TRUE;

}







/*

 * Returns an initial-capped string.

 */

char *capitalize( const char *str )

{

    static char strcap[MAX_STRING_LENGTH];

    int i;



    for ( i = 0; str[i] != '\0'; i++ )

	strcap[i] = LOWER(str[i]);

    strcap[i] = '\0';

    strcap[0] = UPPER(strcap[0]);

    return strcap;

}







/*

 * Append a string to a file.

 */

void append_file( CHAR_DATA *ch, char *file, char *str )

{

    FILE *fp;



    if ( IS_NPC(ch) || str[0] == '\0' )

	return;



    fclose( fpReserve );

    if ( ( fp = fopen( file, "a" ) ) == NULL )

    {

	perror( file );

	send_to_char( "Could not open the file!\n\r", ch );

    }

    else

    {

	fprintf( fp, "[%5d] %s: %s\n",

	    ch->in_room ? ch->in_room->vnum : 0, ch->name, str );

	fclose( fp );

    }



    fpReserve = fopen( NULL_FILE, "r" );

    return;

}







/*

 * Reports a bug.

 */

void bug( const char *str, int param )

{

    char buf[MAX_STRING_LENGTH];

    FILE *fp;



    if ( fpArea != NULL )

    {

	int iLine;

	int iChar;



	if ( fpArea == stdin )

	{

	    iLine = 0;

	}

	else

	{

	    iChar = ftell( fpArea );

	    fseek( fpArea, 0, 0 );

	    for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )

	    {

		while ( getc( fpArea ) != '\n' )

		    ;

	    }

	    fseek( fpArea, iChar, 0 );

	}



	sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );

	log_string( buf );



	if ( ( fp = fopen( "shutdown.txt", "a" ) ) != NULL )

	{

	    fprintf( fp, "[*****] %s\n", buf );

	    fclose( fp );

	}

    }



    strcpy( buf, "[*****] BUG: " );

    sprintf( buf + strlen(buf), str, param );

    log_string( buf );



    fclose( fpReserve );

    if ( ( fp = fopen( BUG_FILE, "a" ) ) != NULL )

    {

	fprintf( fp, "%s\n", buf );

	fclose( fp );

    }

    fpReserve = fopen( NULL_FILE, "r" );



    return;

}


/* I stole log_string to just print
 * into a file for debugging purposes!
 * --Kline
 */
void fight_log( const char *str )
{
 char *strtime;
 FILE *fp;

 if( (fp = fopen( FIGHT_LOG, "w")) == NULL)
 {
  log_string("Can not open CONFIG_FILE for writing."); 
  return;
 }

 strtime = ctime(&current_time);
 strtime[strlen(strtime)-1] = '\0';
 fprintf(fp,"%s :: %s\n",strtime,str);
 return;
}



/*

 * Writes a string to the log.

 */

void log_string( const char *str )

{

    char *strtime;

    char logout [MAX_STRING_LENGTH];



    strtime                    = ctime( &current_time );

    strtime[strlen(strtime)-1] = '\0';

    fprintf( stderr, "%s :: %s\n", strtime, str );

    strcpy ( logout, str );

    logchan( logout );

    return;

}







/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */

void tail_chain( void )

{

    return;

}

void *_getmem(int size, const char *caller, int log)
{
  void *mem;

  if ( !(mem = malloc(size)) )
  {
    fprintf(stderr, "Out of memory.\n");
    raise(SIGSEGV);
  }
  memset(mem, 0, size);

  if (log&&mem_log)
    log_f("getmem(%d)=%p from %s", size, mem, caller);

  return mem;
}

void log_f (char * fmt, ...)
{
 char buf [2*MAX_STRING_LENGTH];
 va_list args;
 va_start (args, fmt);
 vsnprintf (buf, sizeof(buf), fmt, args);
 va_end (args);
 log_string (buf);
}

int get_hours_played(CHAR_DATA *ch)
{
 return ( ch->played + (int) (current_time - ch->logon) ) / 3600;
}

bool is_ds_range( CHAR_DATA *ch )
{
 if( ch->in_room->vnum >= 26000 && ch->in_room->vnum <= 26206 )
 {
  stc("A strange force prevents your actions as a voice whispers...\n\r",ch);
  stc("A dream can not merge into the material realm...\n\r",ch);
  return TRUE;
 }
 if( ch->in_room->vnum == 26 )
 {
  stc("The Miner's home is restricted, you cannot do that.\n\r", ch);
  return TRUE;
 }
 else
  return FALSE;
}

char *format_string( char *oldstring )
{
  char xbuf[MAX_STRING_LENGTH];
  char xbuf2[MAX_STRING_LENGTH];
  char *rdesc;
  int i=0;
  bool cap=TRUE;
  
  xbuf[0]=xbuf2[0]=0;
  
  i=0;
  
  for (rdesc = oldstring; *rdesc; rdesc++)
  {
    if (*rdesc=='\n')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc=='\r') ;
    else if (*rdesc==' ')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc==')')
    {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!'))
      {
        xbuf[i-2]=*rdesc;
        xbuf[i-1]=' ';
        xbuf[i]=' ';
        i++;
      }
      else
      {
        xbuf[i]=*rdesc;
        i++;
      }
    }
    else if (*rdesc=='.' || *rdesc=='?' || *rdesc=='!') {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!')) {
        xbuf[i-2]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i-1]=' ';
          xbuf[i]=' ';
          i++;
        }
        else
        {
          xbuf[i-1]='\"';
          xbuf[i]=' ';
          xbuf[i+1]=' ';
          i+=2;
          rdesc++;
        }
      }
      else
      {
        xbuf[i]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i+1]=' ';
          xbuf[i+2]=' ';
          i += 3;
        }
        else
        {
          xbuf[i+1]='\"';
          xbuf[i+2]=' ';
          xbuf[i+3]=' ';
          i += 4;
          rdesc++;
        }
      }
      cap = TRUE;
    }
    else
    {
      xbuf[i]=*rdesc;
      if ( cap )
        {
          cap = FALSE;
          xbuf[i] = UPPER( xbuf[i] );
        }
      i++;
    }
  }
  xbuf[i]=0;
  strcpy(xbuf2,xbuf);
  
  rdesc=xbuf2;
  
  xbuf[0]=0;
  
  for ( ; ; )
  {
    for (i=0; i<77; i++)
    {
      if (!*(rdesc+i)) break;
    }
    if (i<77)
    {
      break;
    }
    for (i=(xbuf[0]?76:73) ; i ; i--)
    {
      if (*(rdesc+i)==' ') break;
    }
    if (i)
    {
      *(rdesc+i)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"\n\r");
      rdesc += i+1;
      while (*rdesc == ' ') rdesc++;
    }
    else
    {
      bug ("No spaces", 0);
      *(rdesc+75)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"-\n\r");
      rdesc += 76;
    }
  }
  while (*(rdesc+i) && (*(rdesc+i)==' '||
                        *(rdesc+i)=='\n'||
                        *(rdesc+i)=='\r'))
    i--;
  *(rdesc+i+1)=0;
  strcat(xbuf,rdesc);
  if (xbuf[strlen(xbuf)-2] != '\n')
    strcat(xbuf,"\n\r");

  free_string(oldstring);
  return(str_dup(xbuf));
}

