/****************************************************************************
*		       Online Building and Editing Module		    *
*****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "merc.h"

extern int top_ed;

EXTRA_DESCR_DATA *SetRExtra( ROOM_INDEX_DATA *room, char *keywords )
{
    EXTRA_DESCR_DATA *ed;
    
    for ( ed = room->first_extradesc; ed; ed = ed->next )
    {
	  if ( is_name( keywords, ed->keyword ) )
	    break;
    }
    if ( !ed )
    {
	CREATE( ed, EXTRA_DESCR_DATA, 1 );
	LINK( ed, room->first_extradesc, room->last_extradesc, next, prev
);
	ed->keyword	= STRALLOC( keywords );
	ed->description	= STRALLOC( "" );
	top_ed++;
    }
    return ed;
}

bool DelRExtra( ROOM_INDEX_DATA *room, char *keywords )
{
    EXTRA_DESCR_DATA *rmed;
    
    for ( rmed = room->first_extradesc; rmed; rmed = rmed->next )
    {
	  if ( is_name( keywords, rmed->keyword ) )
	    break;
    }
    if ( !rmed )
      return FALSE;
    UNLINK( rmed, room->first_extradesc, room->last_extradesc, next, prev
);
    STRFREE( rmed->keyword );
    STRFREE( rmed->description );
    DISPOSE( rmed );
    top_ed--;
    return TRUE;
}

EXTRA_DESCR_DATA *SetOExtra( OBJ_DATA *obj, char *keywords )
{
    EXTRA_DESCR_DATA *ed;
    
    for ( ed = obj->first_extradesc; ed; ed = ed->next )
    {
	  if ( is_name( keywords, ed->keyword ) )
	    break;
    }
    if ( !ed )
    {
	CREATE( ed, EXTRA_DESCR_DATA, 1 );
	LINK( ed, obj->first_extradesc, obj->last_extradesc, next, prev );
	ed->keyword	= STRALLOC( keywords );
	ed->description	= STRALLOC( "" );
	top_ed++;
    }
    return ed;
}

bool DelOExtra( OBJ_DATA *obj, char *keywords )
{
    EXTRA_DESCR_DATA *rmed;
    
    for ( rmed = obj->first_extradesc; rmed; rmed = rmed->next )
    {
	  if ( is_name( keywords, rmed->keyword ) )
	    break;
    }
    if ( !rmed )
      return FALSE;
    UNLINK( rmed, obj->first_extradesc, obj->last_extradesc, next, prev );
    STRFREE( rmed->keyword );
    STRFREE( rmed->description );
    DISPOSE( rmed );
    top_ed--;
    return TRUE;
}

EXTRA_DESCR_DATA *SetOExtraProto( OBJ_INDEX_DATA *obj, char *keywords )
{
    EXTRA_DESCR_DATA *ed;
    
    for ( ed = obj->first_extradesc; ed; ed = ed->next )
    {
	  if ( is_name( keywords, ed->keyword ) )
	    break;
    }
    if ( !ed )
    {
	CREATE( ed, EXTRA_DESCR_DATA, 1 );
	LINK( ed, obj->first_extradesc, obj->last_extradesc, next, prev );
	ed->keyword	= STRALLOC( keywords );
	ed->description	= STRALLOC( "" );
	top_ed++;
    }
    return ed;
}

bool DelOExtraProto( OBJ_INDEX_DATA *obj, char *keywords )
{
    EXTRA_DESCR_DATA *rmed;
    
    for ( rmed = obj->first_extradesc; rmed; rmed = rmed->next )
    {
	  if ( is_name( keywords, rmed->keyword ) )
	    break;
    }
    if ( !rmed )
      return FALSE;
    UNLINK( rmed, obj->first_extradesc, obj->last_extradesc, next, prev );
    STRFREE( rmed->keyword );
    STRFREE( rmed->description );
    DISPOSE( rmed );
    top_ed--;
    return TRUE;
}


/*
 * Function to get the equivelant exit of DIR 0-MAXDIR out of linked list.
 * Made to allow old-style diku-merc exit functions to work.	-Thoric
 */
EXIT_DATA *get_exit( ROOM_INDEX_DATA *room, sh_int dir )
{
    EXIT_DATA *xit;

    if ( !room )
    {
	bug( "Get_exit: NULL room", 0 );
	return NULL;
    }

    for (xit = room->first_exit; xit; xit = xit->next )
       if ( xit->vdir == dir )
         return xit;
    return NULL;
}

/*
 * Function to get an exit, leading the the specified room
 */
EXIT_DATA *get_exit_to( ROOM_INDEX_DATA *room, sh_int dir, int vnum )
{
    EXIT_DATA *xit;

    if ( !room )
    {
	bug( "Get_exit: NULL room", 0 );
	return NULL;
    }

    for (xit = room->first_exit; xit; xit = xit->next )
       if ( xit->vdir == dir && xit->vnum == vnum )
         return xit;
    return NULL;
}

/*
 * Function to get the nth exit of a room			-Thoric
 */
EXIT_DATA *get_exit_num( ROOM_INDEX_DATA *room, sh_int count )
{
    EXIT_DATA *xit;
    int cnt;

    if ( !room )
    {
	bug( "Get_exit: NULL room", 0 );
	return NULL;
    }

    for (cnt = 0, xit = room->first_exit; xit; xit = xit->next )
       if ( ++cnt == count )
         return xit;
    return NULL;
}

/*
 * Returns value 0 - 9 based on directional text.
 */
int get_dir( char *txt )
{
    int edir;
    char c1,c2;

    c1 = txt[0];
    if ( c1 == '\0' )
      return 0;
    c2 = txt[1];
    edir = 0;
    switch ( c1 )
    {
	  case 'n':
	  switch ( c2 )
	  {
		    default:   edir = 0; break;	/* north */
	  }
	  break;    case '0':  edir = 0; break; /* north */
	  case 'e': case '1':  edir = 1; break; /* east  */
	  case 's':
	  switch ( c2 )
	  {
		    default:   edir = 2; break; /* south */
	  }
	  break;    case '2':  edir = 2; break; /* south */
	  case 'w': case '3':  edir = 3; break; /* west	 */
	  case 'u': case '4':  edir = 4; break; /* up	 */
	  case 'd': case '5':  edir = 5; break; /* down	 */
    }
    return edir;
}

char *  const   ex_flags [] = 
{ 
"isdoor", "closed", "locked", "pickproof" 
};

char *	const	r_flags	[] =
{
"dark", "no_otrans",
"nomob", "indoors",
"private", "safe",
"solitary", "petshop",
"norecall", "noteleport",
"totaldarkness",
"blade_barrier", "arena",
"flaming", "silence"
};

char *	const	o_flags	[] =
{
"glow", "hum", "thrown",
"keep", "vanish",
"invis", "magic",
"nodrop", "bless",
"antigood", "antievil",
"antineutral", "noremove",
"inventory", "loyal",
"shadowplane"
};


char *	const	w_flags	[] =
{
"take", "finger", "neck", "body", "head", "legs", "feet", "hands", "arms",
"shield", "about", "waist", "wrist", "wield", "hold", "face"
};

char *	const	o_types	[] =
{
"none", "light", "scroll", "wand", "staff", "weapon", 
"treasure", "armor", "potion", "furniture", "trash", "container",
"drinkcon", "key", "food", "money", "boat",
"corpse", "corpse_pc", "fountain", "pill", 
"portal", "egg",
"voodoo", "stake",
"missile", "ammo",
"quest", "questcard",
"symbol", "book",
"page", "tool",
"wall", "copper",
"iron", "steel",
"adamantite", "gemstone",
"hilt"
};

char *	const	a_types	[] =
{
"none", "strength", "dexterity", "intelligence", "wisdom", "constitution",
"sex", "class", "level", "age", "height", "weight", "mana", "hit", "move",
"gold", "experience", "armor", "hitroll", "damroll", "save_para",
"save_rod","save_petri", 
"save_breath",
"save_spell",
"poly"
};

char *	const	a_flags [] =
{
"blind", "invisible", "detect_evil", "detect_invis", "detect_magic",
"detect_hidden", "shadowplane", "sanctuary", "faerie_fire", "infrared",
"curse",
"flaming", "poison", "protect", "ethereal", "sneak", "hide", "sleep",
"charm", "flying", "pass_door", "polymorph", "shadowsight", "webbed",
"tendrils", "drowfire", "zuloform", "shift", "peace", "infirmity", 
"godbless", "satansbless", "totalblind",
"steelshield", "monkflaming" };

char *	const	act_flags [] =
{
"npc", "sentinel", "scavenger", "aggressive", "stayarea", "wimpy", "pet",
"train", "practice", "mount", "noparts", "noexp", "prototype", "r1" };


char *	const	plr_flags [] =
{
"npc", "autoexit", "autoloot", "autosac", "blank", "brief", "combine", 
"prompt", "telnet_ga", "holylight", "wizinvis", "ansi", "silence", 
"vt102", "incog", "notell", "log", "deny", "freeze", "godless", 
"watcher", "acid", "challenger","challendged", 
"r1", "r2"
};

char *	const	wear_locs [] =
{
"light", "finger1", "finger2", "neck1", "neck2", "body", "head", "legs",
"feet", "hands", "arms", "shield", "about", "waist", "wrist1", "wrist2",
"wield", "hold", "third", "fourth", "r1", "r2"
};

char *flag_string( int bitvector, char * const flagarray[] )
{
    static char buf[MAX_STRING_LENGTH];
    int x;

    buf[0] = '\0';
    for ( x = 0; x < 32 ; x++ )
      if ( IS_SET( bitvector, 1 << x ) )
      {
	strcat( buf, flagarray[x] );
	strcat( buf, " " );
      }
    if ( (x=strlen( buf )) > 0 )
      buf[--x] = '\0';
    
    return buf;
}


bool can_rmodify( CHAR_DATA *ch, ROOM_INDEX_DATA *room )
{
	if ( IS_NPC( ch ) )
	  return FALSE;
	if ( get_trust( ch ) >= MAX_LEVEL - 1 )
	  return TRUE;
	if ( !IS_SET( room->room_flags, ROOM_PROTOTYPE) )
	{
	  send_to_char( "You cannot modify this room.\n\r", ch );
	  return FALSE;
	}
	
	send_to_char( "That room is not in your allocated range.\n\r", ch
);
	return FALSE;
}

bool can_oedit( CHAR_DATA *ch, OBJ_INDEX_DATA *obj )
{
	
	if ( IS_NPC( ch ) )
	  return FALSE;
	if ( get_trust( ch ) >= MAX_LEVEL - 1 )
	  return TRUE;
	if ( !IS_OBJ_STAT( obj, ITEM_PROTOTYPE) )
	{
	  send_to_char( "You cannot modify this object.\n\r", ch );
	  return FALSE;
	}

	send_to_char( "That object is not in your allocated range.\n\r",
ch );
	return FALSE;
}


bool can_mmodify( CHAR_DATA *ch, CHAR_DATA *mob )
{

	if ( mob == ch )
	  return TRUE;

	if ( !IS_NPC( mob ) )
	{
	   if ( get_trust( ch ) >= MAX_LEVEL - 1 && get_trust(ch) > 
		get_trust( mob ) )
	     return TRUE;
	   else
	     send_to_char( "You can't do that.\n\r", ch );
	     return FALSE;
	}

	if ( IS_NPC( ch ) )
	  return FALSE;
	if ( get_trust( ch ) >= MAX_LEVEL - 1 )
	  return TRUE;
	if ( !IS_SET( mob->act, ACT_PROTOTYPE) )
	{
	  send_to_char( "You cannot modify this mobile.\n\r", ch );
	  return FALSE;
	}
	
	send_to_char( "That mobile is not in your allocated range.\n\r",
ch );
	return FALSE;
}

bool can_medit( CHAR_DATA *ch, MOB_INDEX_DATA *mob )
{
	if ( IS_NPC( ch ) )
	  return FALSE;
	if ( get_trust( ch ) >= MAX_LEVEL - 1 )
	  return TRUE;
	if ( !IS_SET( mob->act, ACT_PROTOTYPE) )
	{
	  send_to_char( "You cannot modify this mobile.\n\r", ch );
	  return FALSE;
	}

	send_to_char( "That mobile is not in your allocated range.\n\r",
ch );
	return FALSE;
}

int get_otype( char *type )
{
    int x;

    for ( x = 0; x < (sizeof(o_types) / sizeof(o_types[0]) ); x++ )
      if ( !str_cmp( type, o_types[x] ) )
        return x;
    return -1;
}


int get_wearloc( char *type )
{
    int x;
    
    for ( x = 0; x < MAX_WEAR; x++ )
      if ( !str_cmp( type, wear_locs[x] ) )
        return x;
    return -1;
}

int get_exflag( char *flag )
{
    int x;
    
    for ( x = 0; x <= MAX_EXFLAG; x++ )
      if ( !str_cmp( flag, ex_flags[x] ) )
        return x;
    return -1;
}

int get_rflag( char *flag )
{
    int x;
    
    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, r_flags[x] ) )
        return x;
    return -1;
}

int get_oflag( char *flag )
{
    int x;

    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, o_flags[x] ) )
        return x;
    return -1;
}

int get_wflag( char *flag )
{
    int x;

    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, w_flags[x] ) )
        return x;
    return -1;
}

int get_actflag( char *flag )
{
    int x;

    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, act_flags[x] ) )
        return x;
    return -1;
}

int get_plrflag( char *flag )
{
    int x;

    for ( x = 0; x < 32; x++ )
      if ( !str_cmp( flag, plr_flags[x] ) )
        return x;
    return -1;
}


void do_redit( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    ROOM_INDEX_DATA	*location, *tmp;
    EXTRA_DESCR_DATA	*ed;
    char		dir;
    EXIT_DATA		*xit, *texit;
    int			value;
    int			edir, ekey, evnum;
    char		*origarg = argument;

    if ( !ch->desc )
    {
	send_to_char( "You have no descriptor.\n\r", ch );
	return;
    }

    switch( ch->substate )
    {
	default:
	  break;
	case SUB_ROOM_DESC:
	  location = ch->dest_buf;
	  if ( !location )
	  {
		bug( "redit: sub_room_desc: NULL ch->dest_buf", 0 );
		location = ch->in_room;
	  }
	  STRFREE( location->description );
	  location->description = copy_buffer( ch );
	  stop_editing( ch );
	  ch->substate = ch->tempnum;
	  return;
	case SUB_ROOM_EXTRA:
	  ed = ch->dest_buf;
	  if ( !ed )
	  {
		bug( "redit: sub_room_extra: NULL ch->dest_buf", 0 );
		stop_editing( ch );
		return;
	  }
	  STRFREE( ed->description );
	  ed->description = copy_buffer( ch );
	  stop_editing( ch );
	  ch->substate = ch->tempnum;
	  return;
    }

    location = ch->in_room;

    smash_tilde( argument );
    argument = one_argument( argument, arg );
    if ( ch->substate == SUB_REPEATCMD )
    {
	if ( arg[0] == '\0' )
	{
	    do_rstat( ch, "" );
	    return;
	}
	if ( !str_cmp( arg, "done" ) || !str_cmp( arg, "off" ) )
	{
	    send_to_char( "Redit mode off.\n\r", ch );
	    if ( ch->pcdata && ch->pcdata->subprompt )
		STRFREE( ch->pcdata->subprompt );
	    ch->substate = SUB_NONE;
	    return;
	}
    }
    if ( arg[0] == '\0' || !str_cmp( arg, "?" ) )
    {
	if ( ch->substate == SUB_REPEATCMD )
	  send_to_char( "Syntax: <field> value\n\r",			ch);
	else
	  send_to_char( "Syntax: redit <field> value\n\r",		ch);
	send_to_char( "\n\r",						ch);
	send_to_char( "Field being one of:\n\r",			ch);
	send_to_char( "  name desc ed rmed\n\r",			ch);
	send_to_char( "  exit bexit exdesc exflags exname\n\r",	ch );
	send_to_char( "  flags sector rlist exkey\n\r",	ch );
	return;
    }

    if ( !can_rmodify( ch, location ) )
      return;

    if ( !str_cmp( arg, "on" ) )
    {
	send_to_char( "Redit mode on.\n\r", ch );
	ch->substate = SUB_REPEATCMD;
	if ( ch->pcdata )
	{
	   if ( ch->pcdata->subprompt )
		STRFREE( ch->pcdata->subprompt );
	   ch->pcdata->subprompt = STRALLOC( "<&CRedit &W#%r&w> %i" );
	}
	return;
    }
    if ( !str_cmp( arg, "substate" ) )
    {
	  argument = one_argument( argument, arg2);
          if( !str_cmp( arg2, "north" )  )
	  {
               ch->inter_substate = SUB_NORTH; 
	       return;
	  }
          if( !str_cmp( arg2, "east" )  )
	  {
               ch->inter_substate = SUB_EAST; 
	       return;
	  }
          if( !str_cmp( arg2, "south" )  )
	  {
               ch->inter_substate = SUB_SOUTH; 
	       return;
	  }
          if( !str_cmp( arg2, "west" )  )
	  {
               ch->inter_substate = SUB_WEST; 
	       return;
	  }
          if( !str_cmp( arg2, "up" )  )
	  {
               ch->inter_substate = SUB_UP; 
	       return;
	  }
          if( !str_cmp( arg2, "down" )  )
	  {
               ch->inter_substate = SUB_DOWN; 
	       return;
	  }
          send_to_char( " unrecognized substate in redit\n\r", ch);
	  return;
    }


    if ( !str_cmp( arg, "name" ) )
    {
	if ( argument[0] == '\0' )
	{
	   send_to_char( "Set the room name.  A very brief single line room description.\n\r", ch );
	   send_to_char( "Usage: redit name <Room summary>\n\r", ch );
	   return;
	}
	STRFREE( location->name );
	location->name = STRALLOC( argument );
	return;
    }

    if ( !str_cmp( arg, "desc" ) )
    {
	if ( ch->substate == SUB_REPEATCMD )
	  ch->tempnum = SUB_REPEATCMD;
	else
	  ch->tempnum = SUB_NONE;
	ch->substate = SUB_ROOM_DESC;
	ch->dest_buf = location;
	start_editing( ch, location->description );
	return;
    }

    if ( !str_cmp( arg, "ed" ) )
    {
	if ( !argument || argument[0] == '\0' )
	{
	   send_to_char( "Create an extra description.\n\r", ch );
	   send_to_char( "You must supply keyword(s).\n\r", ch );
	   return;
	}
	CHECK_SUBRESTRICTED( ch );
	ed = SetRExtra( location, argument );
	if ( ch->substate == SUB_REPEATCMD )
	  ch->tempnum = SUB_REPEATCMD;
	else
	  ch->tempnum = SUB_NONE;
	ch->substate = SUB_ROOM_EXTRA;
	ch->dest_buf = ed;
	start_editing( ch, ed->description );
	return;
    }

    if ( !str_cmp( arg, "rmed" ) )
    {
	if ( !argument || argument[0] == '\0' )
	{
	   send_to_char( "Remove an extra description.\n\r", ch );
	   send_to_char( "You must supply keyword(s).\n\r", ch );
	   return;
	}
	if ( DelRExtra( location, argument ) )
	  send_to_char( "Deleted.\n\r", ch );
	else
	  send_to_char( "Not found.\n\r", ch );
	return;
    }


    if ( !str_cmp( arg, "flags" ) )
    {
	if ( !argument || argument[0] == '\0' )
	{
	   send_to_char( "Toggle the room flags.\n\r", ch );
	   send_to_char( "Usage: redit flags <flag> [flag]...\n\r", ch );
	   return;
	}
	while ( argument[0] != '\0' )
	{
	   argument = one_argument( argument, arg2 );
	   value = get_rflag( arg2 );
	   if ( value < 0 || value > 31 )
	   {
	     sprintf( buf, "Unknown flag: %s\n\r", arg2 );
	     stc(buf, ch);
	   }
	   else
	   {
	     if ( 1 << value == ROOM_PROTOTYPE
	     &&   get_trust( ch ) < MAX_LEVEL - 1 )
	       send_to_char( "You cannot change the prototype flag.\n\r",ch );
	     else
	       TOGGLE_BIT( location->room_flags, 1 << value );
	   }
	}
	return;
    }

    if ( !str_cmp( arg, "sector" ) )
    {
	if ( !argument || argument[0] == '\0' )
	{
	   send_to_char( "Set the sector type.\n\r", ch );
	   send_to_char( "Usage: redit sector <value>\n\r", ch );
	   return;
	}
	location->sector_type = atoi( argument );
	if ( location->sector_type < 0 || location->sector_type >=
SECT_MAX )
	{
	  location->sector_type = 1;
	  send_to_char( "Out of range\n\r.", ch ); 
	}
	else
	  send_to_char( "Done.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "exkey" ) )
    {
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	if ( arg2[0] == '\0' || arg3[0] == '\0' )
	{
	   send_to_char( "Usage: redit exkey <dir> <key vnum>\n\r", ch );
	   return;
	}
	if ( arg2[0] == '#' )
	{
	   edir = atoi( arg2+1 );
	   xit = get_exit_num( location, edir );
	}
	else
	{
	   edir = get_dir( arg2 );
	   xit = get_exit( location, edir );
	}
	value = atoi( arg3 );
	if ( !xit )
	{
	   send_to_char( "No exit in that direction.  Use 'redit exit ...' first.\n\r", ch );
	   return;
	}
	xit->key = value;
	send_to_char( "Done.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "exname" ) )
    {
	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
	   send_to_char( "Change or clear exit keywords.\n\r", ch );
	   send_to_char( "Usage: redit exname <dir> [keywords]\n\r", ch );
	   return;
	}
	if ( arg2[0] == '#' )
	{
	   edir = atoi( arg2+1 );
	   xit = get_exit_num( location, edir );
	}
	else
	{
	   edir = get_dir( arg2 );
	   xit = get_exit( location, edir );
	}
	if ( !xit )
	{
	   send_to_char( "No exit in that direction.  Use 'redit exit ...' first.\n\r", ch );
	   return;
	}
	STRFREE( xit->keyword );
	xit->keyword = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "exflags" ) )
    {
	if ( !argument || argument[0] == '\0' )
	{
	   send_to_char( "Toggle or display exit flags.\n\r", ch );
	   send_to_char( "Usage: redit exflags <dir> <flag> [flag]...\n\r", ch );
	   return;
	}
	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '#' )
	{
	   edir = atoi( arg2+1 );
	   xit = get_exit_num( location, edir );
	}
	else
	{
	   edir = get_dir( arg2 );
	   xit = get_exit( location, edir );
	}
	if ( !xit )
	{
	   send_to_char( "No exit in that direction.  Use 'redit exit ...' first.\n\r", ch );
	   return;
	}
	if ( argument[0] == '\0' )
	{
	   sprintf( buf, "Flags for exit: direction: %d  Keywords: %s Key: %d\n\r[ ",
	   	xit->vdir, xit->keyword, xit->key );
	   for ( value = 0; value <= MAX_EXFLAG; value++ )
	   {
		if ( IS_SET( xit->exit_info, 1 << value ) )
		{
		    strcat( buf, ex_flags[value] );
		    strcat( buf, " " );
		}
	   }
	   strcat( buf, "]\n\r" );
	   send_to_char( buf, ch );
	   return;
	}
	while ( argument[0] != '\0' )
	{
	   argument = one_argument( argument, arg2 );
	   value = get_exflag( arg2 );
	   if ( value < 0 || value > MAX_EXFLAG )
	   {  
	      sprintf( buf, "Unknown flag: %s\n\r", arg2 );
	      stc(buf, ch);
	   }
	   else
	     TOGGLE_BIT( xit->exit_info, 1 << value );
	}
	return;
    }



    if ( !str_cmp( arg, "ex_flags" ) )
    {
	argument = one_argument( argument, arg2 );
        switch(ch->inter_substate)
	{
           case SUB_EAST : dir = 'e'; edir = 1; break;
           case SUB_WEST : dir = 'w'; edir = 3; break;
           case SUB_SOUTH: dir = 's'; edir = 2; break;
           case SUB_UP   : dir = 'u'; edir = 4; break;
           case SUB_DOWN : dir = 'd'; edir = 5; break;
	   default:
           case SUB_NORTH: dir = 'n'; edir = 0; break;
	}

	value = get_exflag(arg2);
        if ( value < 0 )
	{
           send_to_char("Bad exit flag. \n\r", ch);
	   return;
	}
	if ( (xit = get_exit(location,edir)) == NULL )
	{ 
	   sprintf(buf,"exit %c 1",dir);
	   do_redit(ch,buf);
	   xit = get_exit(location,edir);
	}     
	TOGGLE_BIT( xit->exit_info, value );
	return;
    }


    if ( !str_cmp( arg, "ex_to_room" ) )
    {
	argument = one_argument( argument, arg2 );
        switch(ch->inter_substate)
	{
           case SUB_EAST : dir = 'e'; edir = 1; break;
           case SUB_WEST : dir = 'w'; edir = 3; break;
           case SUB_SOUTH: dir = 's'; edir = 2; break;
           case SUB_UP   : dir = 'u'; edir = 4; break;
           case SUB_DOWN : dir = 'd'; edir = 5; break;
	   default:
           case SUB_NORTH: dir = 'n'; edir = 0; break;
	}
	evnum = atoi(arg2);
	if ( evnum < 1 || evnum > 10032766 )
	{
	    send_to_char( "Invalid room number.\n\r", ch );
	    return;
	}
	if ( (tmp = get_room_index( evnum )) == NULL )
	{
	    send_to_char( "Non-existant room.\n\r", ch );
	    return;
	}
	if ( (xit = get_exit(location,edir)) == NULL )
	{ 
	   sprintf(buf,"exit %c 1",dir);
	   do_redit(ch,buf);
	   xit = get_exit(location,edir);
	}     
	xit->vnum = evnum;
	return;
    }

    if ( !str_cmp( arg, "ex_key" ) )
    {
	argument = one_argument( argument, arg2 );
        switch(ch->inter_substate)
	{
           case SUB_EAST : dir = 'e'; edir = 1; break;
           case SUB_WEST : dir = 'w'; edir = 3; break;
           case SUB_SOUTH: dir = 's'; edir = 2; break;
           case SUB_UP   : dir = 'u'; edir = 4; break;
           case SUB_DOWN : dir = 'd'; edir = 5; break;
	   default:
           case SUB_NORTH: dir = 'n'; edir = 0; break;
	}
	if ( (xit = get_exit(location,edir)) == NULL )
	{ 
	   sprintf(buf,"exit %c 1",dir);
	   do_redit(ch,buf);
	   xit = get_exit(location,edir);
	}     
	xit->key = atoi( arg2 );
	return;
    }

    if ( !str_cmp( arg, "ex_exdesc" ) )  
    {
        switch(ch->inter_substate)
	{
           case SUB_EAST : dir = 'e'; edir = 1; break;
           case SUB_WEST : dir = 'w'; edir = 3; break;
           case SUB_SOUTH: dir = 's'; edir = 2; break;
           case SUB_UP   : dir = 'u'; edir = 4; break;
           case SUB_DOWN : dir = 'd'; edir = 5; break;
	   default:
           case SUB_NORTH: dir = 'n'; edir = 0; break;
	}
	if ( (xit = get_exit(location, edir)) == NULL )
	{ 
	   sprintf(buf,"exit %c 1",dir);
	   do_redit(ch,buf);
	}     
	sprintf(buf,"exdesc %c %s",dir,argument);
	do_redit(ch,buf);
	return;
    }

    if ( !str_cmp( arg, "ex_keywords" ) )  /* not called yet */
    {
        switch(ch->inter_substate)
	{
           case SUB_EAST : dir = 'e'; edir = 1; break;
           case SUB_WEST : dir = 'w'; edir = 3; break;
           case SUB_SOUTH: dir = 's'; edir = 2; break;
           case SUB_UP   : dir = 'u'; edir = 4; break;
           case SUB_DOWN : dir = 'd'; edir = 5; break;
	   default:
           case SUB_NORTH: dir = 'n'; edir = 0; break;
	}
	if ( (xit = get_exit(location, edir)) == NULL )
	{ 
	   sprintf(buf, "exit %c 1", dir);
	   do_redit(ch,buf);
	   if ( (xit = get_exit(location, edir)) == NULL )
	     return;
	}     
	sprintf( buf, "%s %s", xit->keyword, argument );
	STRFREE( xit->keyword );
	xit->keyword = STRALLOC( buf );
	return;
    }

    if ( !str_cmp( arg, "exit" ) )
    {
	bool addexit, numnotdir;

	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	if ( !arg2 || arg2[0] == '\0' )
	{
	    send_to_char( "Create, change or remove an exit.\n\r", ch );
	    send_to_char( "Usage: redit exit <dir> [room] [flags] [key] [keywords]\n\r", ch );
	    return;
	}
	addexit = numnotdir = FALSE;
	switch( arg2[0] )
	{
	    default:	edir = get_dir(arg2);			  break;
	    case '+':	edir = get_dir(arg2+1);	addexit = TRUE;	  break;
	    case '#':	edir = atoi(arg2+1);	numnotdir = TRUE; break;  
	}
	if ( !arg3 || arg3[0] == '\0' )
	    evnum = 0;
	else
	    evnum = atoi( arg3 );
	if ( numnotdir )
	{
	    if ( (xit = get_exit_num(location, edir)) != NULL )
	      edir = xit->vdir;
	}
	else
	    xit = get_exit(location, edir);
	if ( !evnum )
	{
	    if ( xit )
	    {
		extract_exit(location, xit);
		send_to_char( "Exit removed.\n\r", ch );
		return;
	    }
	    send_to_char( "No exit in that direction.\n\r", ch );
	    return;
	}
	if ( evnum < 1 || evnum > 10032766 )
	{
	    send_to_char( "Invalid room number.\n\r", ch );
	    return;
	}
	if ( (tmp = get_room_index( evnum )) == NULL )
	{
	    send_to_char( "Non-existant room.\n\r", ch );
	    return;
	}
	if ( addexit || !xit )
	{
	    if ( numnotdir )
	    {
		send_to_char( "Cannot add an exit by number, sorry.\n\r",
ch );
		return;
	    }
	    if ( addexit && xit && get_exit_to(location, edir, tmp->vnum)
)
	    {
		send_to_char( "There is already an exit in that direction leading to that location.\n\r", ch );
		return;
	    }
	    xit = make_exit( location, tmp, edir );
	    xit->keyword		= STRALLOC( "" );
	    xit->description		= STRALLOC( "" );
	    xit->key			= -1;
	    xit->exit_info		= 0;
	    act( "$n reveals a hidden passage!", ch, NULL, NULL, TO_ROOM
);
	}
	else
	    act( "Something is different...", ch, NULL, NULL, TO_ROOM );
	if ( xit->to_room != tmp )
	{
	    xit->to_room = tmp;
	    xit->vnum = evnum;
	    texit = get_exit_to( xit->to_room, rev_dir[edir],
location->vnum );
	    if ( texit )
	    {
		texit->rexit = xit;
		xit->rexit = texit;
	    }
	}
	argument = one_argument( argument, arg3 );
	if ( arg3 && arg3[0] != '\0' )
	    xit->exit_info = atoi( arg3 );
	if ( argument && argument[0] != '\0' )
	{
	    one_argument( argument, arg3 );
	    ekey = atoi( arg3 );
	    if ( ekey != 0 || arg3[0] == '0' )
	    {
		argument = one_argument( argument, arg3 );
		xit->key = ekey;
	    }
	    if ( argument && argument[0] != '\0' )
	    {
		STRFREE( xit->keyword );
		xit->keyword = STRALLOC( argument );
	    }
	}
	send_to_char( "Done.\n\r", ch );
	return;
    }

    /*
     * Twisted and evil, but works				-Thoric
     * Makes an exit, and the reverse in one shot.
     */
    if ( !str_cmp( arg, "bexit" ) )
    {
	EXIT_DATA *xit, *rxit;
	char tmpcmd[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *tmploc;
	int vnum, exnum;
	char rvnum[MAX_INPUT_LENGTH];
	bool numnotdir;

	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	if ( !arg2 || arg2[0] == '\0' )
	{
	    send_to_char( "Create, change or remove a two-way exit.\n\r",
ch );
	    send_to_char( "Usage: redit bexit <dir> [room] [flags] [key] [keywords]\n\r", ch );
	    return;
	}
	numnotdir = FALSE;
	switch( arg2[0] )
	{
	    default:
		edir = get_dir( arg2 );
		break;
	    case '#':
		numnotdir = TRUE;
		edir = atoi( arg2+1 );
		break;
	    case '+':
		edir = get_dir( arg2+1 );
		break;
	}
	tmploc = location;
	exnum = edir;
	if ( numnotdir )
	{
	    if ( (xit = get_exit_num(tmploc, edir)) != NULL )
	      edir = xit->vdir;
	}
	else
	    xit = get_exit(tmploc, edir);
	rxit = NULL;
	vnum = 0;
	rvnum[0] = '\0';
	if ( xit )
	{
	    vnum = xit->vnum;
	    if ( arg3[0] != '\0' )
	      sprintf( rvnum, "%d", tmploc->vnum );
	    if ( xit->to_room )
	      rxit = get_exit(xit->to_room, rev_dir[edir]);
	    else
	      rxit = NULL;
	}
	sprintf( tmpcmd, "exit %s %s %s", arg2, arg3, argument );
	do_redit( ch, tmpcmd );
	if ( numnotdir )
	  xit = get_exit_num(tmploc, exnum);
	else
	  xit = get_exit(tmploc, edir);
	if ( !rxit && xit )
	{
	    vnum = xit->vnum;
	    if ( arg3[0] != '\0' )
	      sprintf( rvnum, "%d", tmploc->vnum );
	    if ( xit->to_room )
	      rxit = get_exit(xit->to_room, rev_dir[edir]);
	    else
	      rxit = NULL;
	}
	if ( vnum )
	{
	    sprintf( tmpcmd, "%d redit exit %d %s %s",
				vnum,
				rev_dir[edir],
				rvnum,
				argument );
	    do_at( ch, tmpcmd );
	}
	return;
    }

    if ( !str_cmp( arg, "exdesc" ) )
    {
	argument = one_argument( argument, arg2 );
	if ( !arg2 || arg2[0] == '\0' )
	{
	   send_to_char( "Create or clear a description for an exit.\n\r", ch );
	   send_to_char( "Usage: redit exdesc <dir> [description]\n\r", ch );
	   return;
	}
	if ( arg2[0] == '#' )
	{
	   edir = atoi( arg2+1 );
	   xit = get_exit_num( location, edir );
	}
	else
	{
	   edir = get_dir( arg2 );
	   xit = get_exit( location, edir );
	}
	if ( xit )
	{
	   STRFREE( xit->description );
	   if ( !argument || argument[0] == '\0' )
	     xit->description = STRALLOC( "" );
	   else
	   {
	     sprintf( buf, "%s\n\r", argument );
	     xit->description = STRALLOC( buf );
	   }
	   send_to_char( "Done.\n\r", ch );
	   return;
	}
	send_to_char( "No exit in that direction.  Use 'redit exit ...' first.\n\r", ch );
	return;
    }

    /*
     * Generate usage message.
     */
    if ( ch->substate == SUB_REPEATCMD )
    {
	ch->substate = SUB_RESTRICTED;
	interpret( ch, origarg );
	ch->substate = SUB_REPEATCMD;
	ch->last_cmd = do_redit;
    }
    else
	do_redit( ch, "" );
    return;
}

void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf  [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;
    int minattr, maxattr;
    bool lockvictim;
    char *origarg = argument;

    if ( IS_NPC( ch ) )
    {
	send_to_char( "Mob's can't mset\n\r", ch );
	return;    
    }

    if ( !ch->desc )
    {
	send_to_char( "You have no descriptor\n\r", ch );
	return;
    }

    switch( ch->substate )
    {
	default:
	  break;
	case SUB_MOB_DESC:
	  if ( !ch->dest_buf )
	  {
		send_to_char( "Fatal error: report to Thoric.\n\r", ch );
		bug( "do_mset: sub_mob_desc: NULL ch->dest_buf", 0 );
		ch->substate = SUB_NONE;
		return;
	  }
	  victim = ch->dest_buf;
	  if ( victim == NULL )
        {
		stc("Your victim doesn't exist!\n\r",ch);
	      stop_editing(ch);
            return;
	  }

	  STRFREE( victim->description );
	  victim->description = copy_buffer( ch );
	  if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	  {
		STRFREE( victim->pIndexData->description );
		victim->pIndexData->description = QUICKLINK(victim->description );
	  }
	  stop_editing( ch );
	  ch->substate = ch->tempnum;
	  return;
    }

    victim = NULL;
    lockvictim = FALSE;
    smash_tilde( argument );

    if ( ch->substate == SUB_REPEATCMD )
    {
	victim = ch->dest_buf;
	if ( victim == POS_DEAD )
	{
	    send_to_char( "Your victim died!\n\r", ch );
	    victim = NULL;
	    argument = "done";
	}
	if ( argument[0] == '\0' || !str_cmp( argument, " " )
	||   !str_cmp( argument, "stat" ) )
	{
	    if ( victim )
		do_mstat( ch, victim->name );
	    else
	        send_to_char( "No victim selected.  Type '?' for help.\n\r", ch );
	    return;
	}
	if ( !str_cmp( argument, "done" ) || !str_cmp( argument, "off" ) )
	{
	    send_to_char( "Mset mode off.\n\r", ch );
	    ch->substate = SUB_NONE;
	    ch->dest_buf = NULL;
	    if ( ch->pcdata && ch->pcdata->subprompt )
		STRFREE( ch->pcdata->subprompt );
	    return;
	}
    }
    if ( victim )
    {
	lockvictim = TRUE;
	strcpy( arg1, victim->name );
	argument = one_argument( argument, arg2 );
	strcpy( arg3, argument );
    }
    else
    {
	lockvictim = FALSE;
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	strcpy( arg3, argument );
    }

    if ( !str_cmp( arg1, "on" ) )
    {
	send_to_char( "Syntax: mset <victim|vnum> on.\n\r", ch );
	return;
    }

    if ( arg1[0] == '\0' || (arg2[0] == '\0' && ch->substate != SUB_REPEATCMD)
    ||   !str_cmp( arg1, "?" ) )
    {
	if ( ch->substate == SUB_REPEATCMD )
	{
	    if ( victim )
		send_to_char( "Syntax: <field>  <value>\n\r",		ch );
	    else
		send_to_char( "Syntax: <victim> <field>  <value>\n\r",	ch );
	}
	else
	    send_to_char( "Syntax: mset <victim> <field>  <value>\n\r",	ch );
		send_to_char( "\n\r",						ch );
		send_to_char( "Field being one of:\n\r",			ch );
		send_to_char( "drowpoints demonpoints demonsouls\n\r", ch);
		send_to_char( "str int wis dex con sex", ch);
		send_to_char( "hp mana move primal magebits\n\r", ch);
		send_to_char( "drunk thirst hitroll quest\n\r", ch);
		send_to_char( "name short long description title spec\n\r", ch);
		send_to_char( "level\n\r", ch);
		send_to_char( "I got pissed off not being able to do any of the msets.\n\r", ch);
		send_to_char( "whichever other one's work note me and I'll add them.\n\r", ch);
	return;
    }

    if ( !victim && get_trust( ch ) < MAX_LEVEL - 3 )
    {
	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}
    }
    else
    if ( !victim )
    {
	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "No one like that in all the realms.\n\r", ch );
	    return;
	}
    }

    if ( get_trust( ch ) < get_trust( victim ) && !IS_NPC( victim ) )
    {
	send_to_char( "You can't do that!\n\r", ch );
	ch->dest_buf = NULL;
	return;
    }
    if ( lockvictim )
      ch->dest_buf = victim;

    if ( IS_NPC(victim) )
    {
	minattr = 1;
	maxattr = 25;
    }
    else
    {
	minattr = 3;
	maxattr = 18;
    }

    if ( !str_cmp( arg2, "on" ) )
    {
	CHECK_SUBRESTRICTED( ch );
	sprintf( buf, "Mset mode on. (Editing %s).\n\r",
		victim->name );
      stc(buf, ch);
	ch->substate = SUB_REPEATCMD;
	ch->dest_buf = victim;

	return;
    }
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    if ( atoi(arg3) < -1 && value == -1 )
      value = atoi(arg3);

    if ( !str_cmp( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	victim->sex = value;
	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	  victim->pIndexData->sex = value;
	return;
    }

    if ( !str_cmp( arg2, "armor" ) )
    {
		if ( value < -300 || value > 300 )
		{
			send_to_char( "AC range is -300 to 300.\n\r", ch );
			return;
		}
		victim->armor = value;
		if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
			victim->pIndexData->ac = value;
		return;
    }
    if ( !str_cmp( arg2, "level" ) )
    {
		if ( IS_NPC(victim) )
		{
			send_to_char( "Only on PC's.\n\r", ch );
			return;
		}

		if ( value < 0 || value > 12 )
		{
		stc("You can only set player levels from 0 - 12.\n\r",ch);
		return;
		}
		victim->level = value;
		stc("Level set.\n\r",ch);
		return;
    }


    /********* UNAUTHORIZED CODE FROM LOIMOS *********/

    if ( !str_cmp( arg2, "dna" ) )
    {
     if ( value < 0 || value > 1000000 )
     {
      stc("Value must be in range from 0 to 1,000,000.\n\r", ch);
      return;
     }    
     victim->pcdata->powers[ALIEN_DNA] = value;
     stc("DNA set.\n\r", ch);
     return;
    }  
    if ( !str_cmp( arg2, "size" ) )
    {
     if ( value < 0 || value > 10 )
     {
      stc("Value must be in the range of 0 - 10.\n\r", ch);
      return;
     }
     if ( (GET_POWERS(victim, ALIEN_SIZE) < 10 ) )
     {
      stc("Your size has been altered.\n\r", victim);
      stc("Size set.\n\r", ch);
      GET_POWERS(victim, ALIEN_SIZE) = value;
      return;
     }
     else
     {
      stc("They have already reached their maximum size.\n\r", ch);
      return;
     }
    }
  
    /***************** END LOIMOS' CODE **************/

    if ( !str_cmp( arg2, "hitroll" ) )
    {
		victim->hitroll = URANGE(0, value, 85);
		if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
			victim->pIndexData->hitroll = victim->hitroll;
			return;
    }
    if ( !str_cmp( arg2, "hp" ) )
    {
	if ( value < 1 || value > 30000 )
	{
	    send_to_char( "Hp range is 1 to 30,000 hit points.\n\r", ch );
	    return;
	}
	victim->max_hit = value;
	return;
    }
	if (!str_cmp( arg2, "primal") )
	{
		if (value < 1 || value > 5000)
		{
			send_to_char( "Primal can only be set between 1 and 5000 points.\n\r", ch);
			return;
		}
		victim->practice = value;
		return;
	}
	if ( !str_cmp( arg2, "exp") )
	{
		if (value < 1 || value > 99000000)
		{
			send_to_char("Experience can only be set between 1 and 99,000,000.\n\r", ch);
			return;
		}
		victim->exp = value;
		return;
	}
    if ( !str_cmp( arg2, "mana" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Mana range is 0 to 30,000 mana points.\n\r", ch);
	    return;
	}
	victim->max_mana = value;
	return;
    }

    if ( !str_cmp( arg2, "move" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch);
	    return;
	}
	victim->max_move = value;
	return;
    }

    if ( !str_cmp( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = value;
	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	  victim->pIndexData->alignment = value;
	return;
    }

    if ( !str_cmp( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Thirst range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

    if ( !str_cmp( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Drunk range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }

    if ( !str_cmp( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 100 )
	{
	    send_to_char( "Full range is 0 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	return;
    }

    if ( !str_cmp( arg2, "blood" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 2000 )
	{
	    stc("Blood range is 0 to 2000.\n\r",ch);
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	if ( !IS_NPC(victim) && get_trust( ch ) < MAX_LEVEL )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	STRFREE( victim->name );
	victim->name = STRALLOC( arg3 );
	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	{
	   STRFREE( victim->pIndexData->player_name );
	   victim->pIndexData->player_name = STRALLOC( arg3 );
	}
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	STRFREE( victim->short_descr );
	victim->short_descr = STRALLOC( arg3 );
	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	{
	   STRFREE( victim->pIndexData->short_descr );
	   victim->pIndexData->short_descr = QUICKLINK( victim->short_descr );
	}
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	STRFREE( victim->long_descr );
	strcpy( buf, arg3 );
	strcat( buf, "\n\r" );
	victim->long_descr = STRALLOC( buf );
	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	{
	   STRFREE( victim->pIndexData->long_descr );
	   victim->pIndexData->long_descr = QUICKLINK( victim->long_descr
);
	}
	return;
    }

    if ( !str_cmp( arg2, "description" ) )
    {
	if ( arg3[0] )
	{
	   STRFREE( victim->description );
	   victim->description = STRALLOC( arg3 );
	   if ( IS_NPC( victim ) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	   {
	      STRFREE(victim->pIndexData->description );
	      victim->pIndexData->description = QUICKLINK(
victim->description );
	   }
	   return;
	}
	CHECK_SUBRESTRICTED( ch );
	if ( ch->substate == SUB_REPEATCMD )
	  ch->tempnum = SUB_REPEATCMD;
	else
	  ch->tempnum = SUB_NONE;
	ch->substate = SUB_MOB_DESC;
	ch->dest_buf = victim;
	start_editing( ch, victim->description );
	return;
    }

    if ( !str_cmp( arg2, "title" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	set_title( victim, arg3 );
	return;
    }

    if ( !str_cmp( arg2, "spec" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

        if ( !str_cmp( arg3, "none" ) )
        {
          victim->spec_fun = NULL;
	  send_to_char( "Special function removed.\n\r", ch );
	  if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	    victim->pIndexData->spec_fun = victim->spec_fun;
	  return;
        }

	if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	{
	    send_to_char( "No such spec fun.\n\r", ch );
	    return;
	}
	if ( IS_NPC(victim) && IS_SET( victim->act, ACT_PROTOTYPE ) )
	  victim->pIndexData->spec_fun = victim->spec_fun;
	return;
    }

    /*
     * Generate usage message.
     */
    if ( ch->substate == SUB_REPEATCMD )
    {
	ch->substate = SUB_RESTRICTED;
	interpret( ch, origarg );
	ch->substate = SUB_REPEATCMD;
	ch->last_cmd = do_mset;
    }
    else
	do_mset( ch, "" );
    return;
}

void do_ocreate( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA	*pObjIndex;
    OBJ_DATA		*obj;
    int			 vnum, cvnum;

    if ( IS_NPC(ch) )
    {
	send_to_char( "Mobiles cannot create.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    vnum = is_number( arg ) ? atoi( arg ) : -1;

    if ( vnum == -1 || !argument || argument[0] == '\0' )
    {
	send_to_char( "Usage: ocreate <vnum> [copy vnum] <item name>\n\r", ch );
	return;
    }

    if ( vnum < 1 || vnum > 2000000 )
    {
	send_to_char( "Bad number.\n\r", ch );
	return;
    }

    one_argument( argument, arg2 );
    cvnum = atoi( arg2 );
    if ( cvnum != 0 )
      argument = one_argument( argument, arg2 );
    if ( cvnum < 1 )
      cvnum = 0;

    if ( get_obj_index( vnum ) )
    {
	send_to_char( "An object with that number already exists.\n\r", ch );
	return;
    }    

    if ( IS_NPC( ch ) )
      return;
    if ( get_trust( ch ) < LEVEL_IMMORTAL )
    {
	  send_to_char( "Sorry, No thanks..\n\r", ch );
	  return;
    }

    pObjIndex = make_object( vnum, cvnum, argument );
    if ( !pObjIndex )
    {
	send_to_char( "Error.\n\r", ch );
	log_string( "do_ocreate: make_object failed." );
	return;
    }
    obj = create_object( pObjIndex, get_trust(ch) );
    obj_to_char( obj, ch );
    act( "$n makes some ancient arcane gestures, and opens $s hands to reveal $p!", ch, obj, NULL, TO_ROOM );
    act( "You make some ancient arcane gestures, and open your hands to reveal $p!", ch, obj, NULL, TO_CHAR );
}

void do_mcreate( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA	*pMobIndex;
    CHAR_DATA		*mob;
    int			 vnum, cvnum;

    if ( IS_NPC(ch) )
    {
	send_to_char( "Mobiles cannot create.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );
    
    vnum = is_number( arg ) ? atoi( arg ) : -1;

    if ( vnum == -1 || !argument || argument[0] == '\0' )
    {
	send_to_char( "Usage: mcreate <vnum> [cvnum] <mobile name>\n\r", ch );
	return;
    }

    if ( vnum < 1 || vnum > 2000000 )
    {
	send_to_char( "Bad number.\n\r", ch );
	return;
    }

    one_argument( argument, arg2 );
    cvnum = atoi( arg2 );
    if ( cvnum != 0 )
      argument = one_argument( argument, arg2 );
    if ( cvnum < 1 )
      cvnum = 0;

    if ( get_mob_index( vnum ) )
    {
	send_to_char( "A mobile with that number already exists.\n\r", ch );
	return;
    }    

    if ( IS_NPC( ch ) )
      return;
    if ( get_trust( ch ) < LEVEL_IMMORTAL )
    {
	  send_to_char( "No thanks.\n\r", ch );
	  return;
    }

    pMobIndex = make_mobile( vnum, cvnum, argument );
    if ( !pMobIndex )
    {
	send_to_char( "Error.\n\r", ch );
	log_string( "do_mcreate: make_mobile failed." );
	return;
    }
    mob = create_mobile( pMobIndex );
    char_to_room( mob, ch->in_room );
    act( "$n waves $s arms about, and $N appears at $s command!", ch, NULL, mob, TO_ROOM );
    act( "You wave your arms about, and $N appears at your command!", ch, NULL, mob, TO_CHAR );
}

/*
 * Simple but nice and handle line editor.			-Thoric
 */
void edit_buffer( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    EDITOR_DATA *edit;
    char cmd[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    sh_int x, line, max_buf_lines;
    bool save;

    if ( (d = ch->desc) == NULL )
    {
	send_to_char( "You have no descriptor.\n\r", ch );
	return;
    }

   if ( d->connected != CON_EDITING )
   {
	send_to_char( "You can't do that!\n\r", ch );
	bug( "Edit_buffer: d->connected != CON_EDITING", 0 );
	return;
   }
    
   if ( ch->substate <= SUB_PAUSE )
   {
	send_to_char( "You can't do that!\n\r", ch );
      sprintf(buf,"Edit_buffer: illegal ch->substate (%d)", ch->substate
);
      bug(buf,0);
	d->connected = CON_PLAYING;
	return;
   }
   
   if ( !ch->editor )
   {
	send_to_char( "You can't do that!\n\r", ch );
	bug( "Edit_buffer: null editor", 0 );
	d->connected = CON_PLAYING;
	return;
   }
   
   edit = ch->editor;
   save = FALSE;
   max_buf_lines = 24;

   if ( ch->substate == SUB_MPROG_EDIT || ch->substate == SUB_HELP_EDIT )
     max_buf_lines = 48;

   if ( argument[0] == '/' || argument[0] == '\\' )
   {
	one_argument( argument, cmd );
	if ( !str_cmp( cmd+1, "?" ) )
	{
	    send_to_char( "Editing commands\n\r---------------------------------\n\r", ch );
	    send_to_char( "/l              list buffer\n\r",	ch );
	    send_to_char( "/c              clear buffer\n\r",	ch );
	    send_to_char( "/d [line]       delete line\n\r",	ch );
	    send_to_char( "/g <line>       goto line\n\r",	ch );
	    send_to_char( "/i <line>       insert line\n\r",	ch );
	    send_to_char( "/r <old> <new>  global replace\n\r",	ch );
	    send_to_char( "/a              abort editing\n\r",	ch );
	    if ( get_trust(ch) > MAX_LEVEL - 4 )
	      send_to_char( "/! <command>    execute command (do not use another editing command)\n\r",  ch );
	    send_to_char( "/s              save buffer\n\r\n\r> ",ch );
	    return;
	}
	if ( !str_cmp( cmd+1, "c" ) )
	{
	    memset( edit, '\0', sizeof(EDITOR_DATA) );
	    edit->numlines = 0;
	    edit->on_line   = 0;
	    send_to_char( "Buffer cleared.\n\r> ", ch );
	    return;
	}
	if ( !str_cmp( cmd+1, "r" ) )
	{
	    char word1[MAX_INPUT_LENGTH];
	    char word2[MAX_INPUT_LENGTH];
	    char *sptr, *wptr, *lwptr;
	    int x, count, wordln, word2ln, lineln;

	    sptr = one_argument( argument, word1 );
	    sptr = one_argument( sptr, word1 );
	    sptr = one_argument( sptr, word2 );
	    if ( word1[0] == '\0' || word2[0] == '\0' )
	    {
		send_to_char( "Need word to replace, and replacement.\n\r>", ch );
		return;
	    }
	    if ( strcmp( word1, word2 ) == 0 )
	    {
		send_to_char( "Done.\n\r> ", ch );
		return;
	    }
	    count = 0;  wordln = strlen(word1);  word2ln = strlen(word2);
	    sprintf( buf, "Replacing all occurrences of %s with %s...\n\r", word1, word2 );
	     stc(buf, ch);
	    for ( x = edit->on_line; x < edit->numlines; x++ )
	    {
		lwptr = edit->line[x];
		while ( (wptr = strstr( lwptr, word1 )) != NULL )
		{
		    sptr = lwptr;
		    lwptr = wptr + wordln;
		    sprintf( buf, "%s%s", word2, wptr + wordln );
		    lineln = wptr - edit->line[x] - wordln;
		    ++count;
		    if ( strlen(buf) + lineln > 79 )
		    {
			lineln = UMAX(0, (79 - strlen(buf)));
			buf[lineln] = '\0';
			break;
		    }
		    else
			lineln = strlen(buf);
		    buf[lineln] = '\0';
		    strcpy( wptr, buf );
		}
	    }
	    sprintf( buf, "Found and replaced %d occurrence(s).\n\r> ", count );
		stc(buf, ch);
	    return;
	}

	if ( !str_cmp( cmd+1, "i" ) )
	{
	    if ( edit->numlines >= max_buf_lines )
		send_to_char( "Buffer is full.\n\r> ", ch );
	    else
	    {
		if ( argument[2] == ' ' )
		  line = atoi( argument + 2 ) - 1;
		else
		  line = edit->on_line;
		if ( line < 0 )
		  line = edit->on_line;
		if ( line < 0 || line > edit->numlines )
		  send_to_char( "Out of range.\n\r> ", ch );
		else
		{
		  for ( x = ++edit->numlines; x > line; x-- )
			strcpy( edit->line[x], edit->line[x-1] );
		  strcpy( edit->line[line], "" );
		  send_to_char( "Line inserted.\n\r> ", ch );
		}
 	    }
	    return;
	}
	if ( !str_cmp( cmd+1, "d" ) )
	{
	    if ( edit->numlines == 0 )
		send_to_char( "Buffer is empty.\n\r> ", ch );
	    else
	    {
		if ( argument[2] == ' ' )
		  line = atoi( argument + 2 ) - 1;
		else
		  line = edit->on_line;
		if ( line < 0 )
		  line = edit->on_line;
		if ( line < 0 || line > edit->numlines )
		  send_to_char( "Out of range.\n\r> ", ch );
		else
		{
		  if ( line == 0 && edit->numlines == 1 )
		  {
			memset( edit, '\0', sizeof(EDITOR_DATA) );
			edit->numlines = 0;
			edit->on_line   = 0;
			send_to_char( "Line deleted.\n\r> ", ch );
			return;
		  }
		  for ( x = line; x < (edit->numlines - 1); x++ )
			strcpy( edit->line[x], edit->line[x+1] );
		  strcpy( edit->line[edit->numlines--], "" );
		  if ( edit->on_line > edit->numlines )
		    edit->on_line = edit->numlines;
		  send_to_char( "Line deleted.\n\r> ", ch );
		}
 	    }
	    return;
	}
	if ( !str_cmp( cmd+1, "g" ) )
	{
	    if ( edit->numlines == 0 )
		send_to_char( "Buffer is empty.\n\r> ", ch );
	    else
	    {
		if ( argument[2] == ' ' )
		  line = atoi( argument + 2 ) - 1;
		else
		{
		    send_to_char( "Goto what line?\n\r> ", ch );
		    return;
		}
		if ( line < 0 )
		  line = edit->on_line;
		if ( line < 0 || line > edit->numlines )
		  send_to_char( "Out of range.\n\r> ", ch );
		else
		{
		  edit->on_line = line;
		  sprintf( buf, "(On line %d)\n\r> ", line+1 );
		  stc(buf,ch);
		}
 	    }
	    return;
	}
	if ( !str_cmp( cmd+1, "l" ) )
	{
	    if ( edit->numlines == 0 )
	      send_to_char( "Buffer is empty.\n\r> ", ch );
	    else
	    {
	      send_to_char( "------------------\n\r", ch );
	      for ( x = 0; x < edit->numlines; x++ )
	      {
	        sprintf( buf, "%2d> %s\n\r", x+1, edit->line[x] );
	        stc(buf, ch);
	      }
	      send_to_char( "------------------\n\r> ", ch );
	    }
	    return;
	}
	if ( !str_cmp( cmd+1, "a" ) )
	{
	    send_to_char( "\n\rAborting... ", ch );
	    stop_editing( ch );
	    return;
	}
	if ( get_trust(ch) > LEVEL_IMMORTAL && !str_cmp( cmd+1, "!" ) )
	{
	    int substate = ch->substate;

	    ch->substate = SUB_RESTRICTED;
	    interpret(ch, argument+3);
	    ch->substate = substate;
	    send_to_char( "\n\r> ", ch );
	    return;
	}
	if ( !str_cmp( cmd+1, "s" ) )
	{
	    d->connected = CON_PLAYING;
	    do_hedit(ch, "");
            if ( !ch->last_cmd )
              return;
            (*ch->last_cmd) ( ch, "" );
            return;
	}
   }

   if ( edit->size + strlen(argument) + 1 >= MAX_STRING_LENGTH - 1 )
	send_to_char( "You buffer is full.\n\r", ch );
   else
   {
	if ( strlen(argument) > 79 )
	{
	  strncpy( buf, argument, 79 );
	  buf[79] = 0; 
	  send_to_char( "(Long line trimmed)\n\r> ", ch );
	}
	else
	  strcpy( buf, argument );
	strcpy( edit->line[edit->on_line++], buf );
	if ( edit->on_line > edit->numlines )
	  edit->numlines++;
	if ( edit->numlines > max_buf_lines )
	{
	  edit->numlines = max_buf_lines;
	  send_to_char( "Buffer full.\n\r", ch );
	  save = TRUE;
	}
   }

   if ( save )
   {
      d->connected = CON_PLAYING;
            if ( !ch->last_cmd )
              return;
            (*ch->last_cmd) ( ch, "" );
            return;  
   }
   send_to_char( "> ", ch );
}

/*
 * Remove carriage returns from a line
 */
char *strip_cr( char *str )
{
    static char newstr[MAX_STRING_LENGTH];
    int i, j;

    for ( i=j=0; str[i] != '\0'; i++ )
	if ( str[i] != '\r' )
	{
	  newstr[j++] = str[i];	
	}
    newstr[j] = '\0';
    return newstr;
}

void smush_tilde( char *str )
{
    int len;
    char last;
    char *strptr;
    
    strptr = str;
    
    len  = strlen( str );
    if ( len )
      last = strptr[len-1];
    else
      last = '\0';

    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }
    if ( len )
      strptr[len-1] = last;

    return;
}

void start_editing( CHAR_DATA *ch, char *data )
{
	EDITOR_DATA *edit;
	sh_int lines, size, lpos;
	char c;
	
	if ( !ch->desc )
	{
	   bug( "Fatal: start_editing: no desc", 0 );
	   return;
	}
	if ( ch->substate == SUB_RESTRICTED )
	   bug( "NOT GOOD: start_editing: ch->substate == SUB_RESTRICTED", 0 );

	send_to_char( "Begin entering your text now (/? = help /s = save /c = clear /l = list)\n\r", ch );
	send_to_char( "-----------------------------------------------------------------------\n\r> ", ch );
	if ( ch->editor )
	  stop_editing( ch );
	
	CREATE( edit, EDITOR_DATA, 1 );
	edit->numlines = 0;
	edit->on_line  = 0;
	edit->size     = 0;
	size = 0;  lpos = 0;  lines = 0;
	if ( !data )
	    bug("editor: data is NULL!\n\r",0);
	else
	for ( ;; )
	{  
	   c = data[size++];
	   if ( c == '\0' )
	   {
		edit->line[lines][lpos] = '\0';
		break;
	   }
	   else
	   if ( c == '\r' );
	   else
	   if ( c == '\n' || lpos > 78)
	   {
		edit->line[lines][lpos] = '\0';
		lines++;
		lpos = 0;
	   }
	   else
	     edit->line[lines][lpos++] = c;
	   if ( lines >= 49 || size > 4096 )
	   {
		edit->line[lines][lpos] = '\0';
		break;
	   }	   
	}
	edit->numlines = lines;
	edit->size = size;
	edit->on_line = lines;
	ch->editor = edit;
	ch->desc->connected = CON_EDITING;
}

char *copy_buffer( CHAR_DATA *ch )
{
   char buf[MAX_STRING_LENGTH];
   char tmp[100];
   sh_int x, len;

   if ( !ch )
   {
	bug( "copy_buffer: null ch", 0 );
	return STRALLOC( "" );
   } 

   if ( !ch->editor )
   {
	bug( "copy_buffer: null editor", 0 );
	return STRALLOC( "" );
   }

   buf[0] = '\0';
   for ( x = 0; x < ch->editor->numlines; x++ )
   {
      strcpy( tmp, ch->editor->line[x] );
      smush_tilde( tmp );
      len = strlen(tmp);
      if ( tmp && tmp[len-1] == '~' )
        tmp[len-1] = '\0';
      else
        strcat( tmp, "\n\r" );
      strcat( buf, tmp );
   }
   return STRALLOC( buf );
}

void stop_editing( CHAR_DATA *ch )
{
    DISPOSE( ch->editor );
    ch->editor = NULL;
    send_to_char( "Done.\n\r", ch );
    ch->dest_buf  = NULL;
    ch->spare_ptr = NULL;
    ch->substate  = SUB_NONE;
    if ( !ch->desc )
    {
	bug( "Fatal: stop_editing: no desc", 0 );
	return;
    }
    ch->desc->connected = CON_PLAYING;
}


/*
 * Moved into a separate function so it can be used for other things
 * ie: online help editing				-Thoric
 */
HELP_DATA *get_help( CHAR_DATA *ch, char *argument )
{
    char argall[MAX_INPUT_LENGTH];
    char argone[MAX_INPUT_LENGTH];
    char argnew[MAX_INPUT_LENGTH];
    HELP_DATA *pHelp;
    int lev;

    if ( argument[0] == '\0' )
	argument = "summary";

    if ( isdigit(argument[0]) )
    {
	lev = number_argument( argument, argnew );
	argument = argnew;
    }
    else
	lev = -2;
    /*
     * Tricky argument handling so 'help a b' doesn't match a.
     */
    argall[0] = '\0';
    while ( argument[0] != '\0' )
    {
	argument = one_argument( argument, argone );
	if ( argall[0] != '\0' )
	    strcat( argall, " " );
	strcat( argall, argone );
    }

    for ( pHelp = first_help; pHelp; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;
	if ( lev != -2 && pHelp->level != lev )
	    continue;

	if ( is_name( argall, pHelp->keyword ) )
	    return pHelp;
    }

    return NULL;
}


/*
 * Now this is cleaner
 */
void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;

    if ( (pHelp = get_help( ch, argument )) == NULL )
    {
	send_to_char( "No help on that word.\n\r", ch );
	return;
    }

    if ( pHelp->level >= 0 && str_cmp( argument, "imotd" ) )
    {
	stc( pHelp->keyword, ch );
	stc( "\n\r", ch );
    }

    /*
     * Strip leading '.' to allow initial blanks.
     */
    if( !str_cmp(argument,"MXP_DEF") && !IS_MXP(ch) )
    {
     stc("This file can only be viewed by clients with MXP support.  (Help MXP)\n\r",ch);
     return;
    }
    if ( pHelp->text[0] == '.' )
	stc( pHelp->text+1, ch );
    else
	stc( pHelp->text, ch );
    return;
}

/*
 * Help editor							-Thoric
 */
void do_hedit( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;

    if ( !ch->desc )
    {
	send_to_char( "You have no descriptor.\n\r", ch );
	return;
    }

    switch( ch->substate )
    {
	default:
	  break;
	case SUB_HELP_EDIT:
	  if ( (pHelp = ch->dest_buf) == NULL )
	  {
		bug( "hedit: sub_help_edit: NULL ch->dest_buf", 0 );
		stop_editing( ch );
		return;
	  }
	  STRFREE( pHelp->text );
	  pHelp->text = copy_buffer( ch );
	  stop_editing( ch );
	  return;
    }
    if ( (pHelp = get_help( ch, argument )) == NULL )	/* new help */
    {
	char argnew[MAX_INPUT_LENGTH];
	int lev;

	if ( isdigit(argument[0]) )
	{
	    lev = number_argument( argument, argnew );
	    argument = argnew;
	}
	else
	    lev = get_trust(ch);
	CREATE( pHelp, HELP_DATA, 1 );
	pHelp->keyword = STRALLOC( strupper(argument) );
	pHelp->text    = STRALLOC( "" );
	pHelp->level   = lev;
	add_help( pHelp );
    }
    ch->substate = SUB_HELP_EDIT;
    ch->dest_buf = pHelp;
    start_editing( ch, pHelp->text );
}

/*
 * Stupid leading space muncher fix				-Thoric
 */
char *help_fix( char *text )
{
    char *fixed;

    if ( !text )
      return "";
    fixed = strip_cr(text);
    if ( fixed[0] == ' ' )
      fixed[0] = '.';
    return fixed;
}

void do_hset( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: hset <field> [value] [help page]\n\r",	ch
);
	send_to_char( "\n\r",						ch
);
	send_to_char( "Field being one of:\n\r",			ch
);
	send_to_char( "  level keyword remove save\n\r",		ch
);
	return;
    }

    if ( !str_cmp( arg1, "save" ) )
    {
	FILE *fpout;

	rename( "help.are", "help.are.bak" );
	fclose( fpReserve );
	if ( ( fpout = fopen( "help.are", "w" ) ) == NULL )
	{
	   bug( "hset save: fopen", 0 );
	   perror( "help.are" );
	   fpReserve = fopen( NULL_FILE, "r" );
	   return;
	}
    
	fprintf( fpout, "#HELPS\n\n" );
	for ( pHelp = first_help; pHelp; pHelp = pHelp->next )
	    fprintf( fpout, "%d %s~\n%s~\n\n",
			pHelp->level, pHelp->keyword,
help_fix(pHelp->text) );

	fprintf( fpout, "0 $~\n\n\n#$\n" );
	fclose( fpout );
	fpReserve = fopen( NULL_FILE, "r" );
	send_to_char( "Saved.\n\r", ch );
	return;
    }
    if ( str_cmp( arg1, "remove" ) )
	argument = one_argument( argument, arg2 );

    if ( (pHelp = get_help( ch, argument )) == NULL )
    {
	send_to_char( "Cannot find help on that subject.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg1, "remove" ) )
    {
	UNLINK( pHelp, first_help, last_help, next, prev );
	STRFREE( pHelp->text );
	STRFREE( pHelp->keyword );
	DISPOSE( pHelp );
	send_to_char( "Removed.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg1, "level" ) )
    {
	pHelp->level = atoi( arg2 );
	send_to_char( "Done.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg1, "keyword" ) )
    {
	STRFREE( pHelp->keyword );
	pHelp->keyword = STRALLOC( strupper(arg2) );
	send_to_char( "Done.\n\r", ch );
	return;
    }

    do_hset( ch, "" );
}

/*
 * Show help topics in a level range				-Thoric
 * Idea suggested by Gorog
 */
void do_hlist( CHAR_DATA *ch, char *argument )
{
    int min, max, minlimit, maxlimit, cnt;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    HELP_DATA *help;

    maxlimit = get_trust(ch);
    minlimit = maxlimit >= (MAX_LEVEL - 3) ? -1 : 0;
    argument = one_argument( argument, arg );
    if ( arg[0] != '\0' )
    {
	min = URANGE( minlimit, atoi(arg), maxlimit );
	if ( argument[0] != '\0' )
	    max = URANGE( min, atoi(argument), maxlimit );
	else
	    max = maxlimit;
    }
    else
    {
	min = minlimit;
	max = maxlimit;
    }
    sprintf( buf, "Help Topics in level range %d to %d:\n\r\n\r", min, max
);
	stc(buf,ch);
    for ( cnt = 0, help = first_help; help; help = help->next )
	if ( help->level >= min && help->level <= max )
	{
	    sprintf( buf, "  %3d %s\n\r", help->level, help->keyword );
	    stc(buf, ch);
	    ++cnt;
	}
    if ( cnt )
    {
	sprintf( buf, "\n\r%d pages found.\n\r", cnt );
	stc(buf,ch);
    }
    else
	send_to_char( "None found.\n\r", ch );
}

