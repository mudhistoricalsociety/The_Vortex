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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



/*
 * Local functions.
 */

void vamp_disciplines( CHAR_DATA *ch, char *argument, char *argument2 );
void wolf_disciplines( CHAR_DATA *ch, char *argument, char *argument2 );
void			call_all	args( ( CHAR_DATA *ch ) );
char			buf		[MAX_STRING_LENGTH];
char			arg		[MAX_INPUT_LENGTH];
FILE 			*popen		args( ( const char *command, const char *type ) );
int 			pclose		args( ( FILE *stream ) );
char			*fgetf		args( ( char *s, int n, register FILE *iop ) );


void do_checkwizlist( CHAR_DATA *ch, char *argument)
{
 char buf[MAX_STRING_LENGTH];
 WIZ_DATA *list;

 for( list = wiz_first; list; list = list->next)
 {
  sprintf( buf, "Name: %s\n\r Level: %d\n\r Message: %s\n\r", list->name, list->level, list->message);
  stc(buf, ch);
 }
 return;
}

void do_delimmortal( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 WIZ_DATA *list;

 argument = one_argument( argument, arg);
 
 if( IS_NPC(ch))
  return;
 if( (list = get_immortal(arg)) == NULL)
 {
  stc("Immortal not found.\n\r", ch);
  return;
 }
 if( (list->level >= ch->level) && str_cmp(ch->pcdata->switchname,"Kline") )
 {
  stc("You are not allowed to delete immortals equal or above you.\n\r", ch);
  return;
 }
 UNLINK( list, wiz_first, wiz_last, next, prev);
 DISPOSE( list);
 stc("Immortal deleted.\n\r", ch);
 return;
}     

void do_savewizlist( CHAR_DATA *ch, char *argument)
{
 FILE *fp;
 WIZ_DATA *list;

 if( IS_NPC(ch))
  return;
 if( (fp = fopen( IMM_FILE, "w")) == NULL)
 {
  log_string("Can not open IMM_FILE for writing.");
  return;
 }
 for( list = wiz_first; list; list = list->next)
 {
  fprintf( fp, "#NEW\n");
  fprintf( fp, "Name %s~\n", list->name);
  fprintf( fp, "Level %d\n", list->level);
  fprintf( fp, "Message %s~\n", list->message);
  fprintf( fp, "End\n");
 }
 fprintf( fp, "#END\n");
 stc("Wizlist has been saved.\n\r", ch);
 fclose(fp);
 return;
}
 
WIZ_DATA *get_immortal( char *argument)
{
  WIZ_DATA *list;

  list = NULL;
  for( list = wiz_first; list; list = list->next)
  {
   if( !str_cmp( list->name, argument))
    return list;
  }
  return NULL;
}

void do_makeimmortal( CHAR_DATA *ch, char *argument)
{
 WIZ_DATA *new;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( get_immortal( arg) != NULL)
 {
  stc("An immortal already exists with that name.\n\r", ch);
  return;
 }
 CREATE( new, WIZ_DATA, 1);
 new->name = strdup( up_case(arg));
 new->message = strdup( arg);
 new->level = 7;
 LINK( new, wiz_first, wiz_last, next, prev);
 stc("New immortal file created.\n\r", ch);
 return;
}

void do_setimmortal( CHAR_DATA *ch, char *argument)
{
 WIZ_DATA *imm;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];

 if( IS_NPC(ch))
  return;
 argument = one_argument( argument, arg1);
 argument = one_argument( argument, arg2);
 
 if( arg1[0] =='\0')
 {
  stc("Set which immortal file to what?\n\r", ch);
  return;
 }
 if( arg2[0] == '\0')
 {
  stc("Options are: name, message, level.\n\r", ch);
  return;
 }
 if( (imm = get_immortal( arg1)) == NULL)
 {
  stc("That file does not exist.\n\r", ch);
  return;
 }
 if( imm->level > ch->level)
 {
  stc("You can't alter an imm file that is higher than your level.\n\r", ch);
  return;
 }
 if( !str_cmp( arg2, "name"))
 {
  argument = one_argument( argument, arg3);
  if( arg3[0] == '\0')
  {
   stc("Set it to what?\n\r", ch);
   return;
  }
  imm->name = strdup( arg3);
  stc("New name set.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg2, "level"))
 {
  argument = one_argument( argument, arg3);
  if( arg3[0] == '\0')
  {
   stc("Set it to what?\n\r", ch);
   return;
  }
  if( !is_number( arg3))
  {
   stc("Thats not a number.\n\r", ch);
   return;
  }
  if( atoi( arg3) > ch->level)
  {
   stc("I dont think so.\n\r", ch);
   return;
  }
  if( (atoi( arg3) < 7) || (atoi(arg3) > 12))
  {
   stc("Valid range is 7 to 12.\n\r", ch);
   return;
  }
  imm->level = atoi( arg3);
  stc("Level set.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg2, "message"))
 {
  if( argument[0] == '\0')
  {
   stc("Set the message to what?\n\r", ch);
   return;
  }
  imm->message = strdup( argument);
  stc("Message set.\n\r", ch);
  return;
 }
 else 
 {
  stc("Valid options are: name, level, message.\n\r", ch);
  return;
 }
 return;
}
 
void do_rformat( CHAR_DATA *ch, char *argument)
{
 char *Fstring;
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 ROOM_INDEX_DATA *room;
 int length = 79; /* default to 79...80 character*/
 
 argument = one_argument(argument, arg);
 if( arg[0] != '\0' && is_number(arg))
  length = atoi(arg);
 if( !ch->in_room)
  return;
 
 room = ch->in_room;
 Fstring = format_string(room->description);
 strcpy(buf, Fstring);
 free_string(room->description);
 room->description = str_dup(buf);
 stc("Done.\n\r", ch);
 return;
}

void do_savecommands(CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch))
  return;
 save_commands();
 stc("Done.\n\r", ch);
 return;
} 

void do_clearrace(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int i =0;

 one_argument(argument, arg);
 
 if( IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Clear whose race?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 { 
  stc("They are not here.\n\r", ch);
  return;
 }
 if( victim->trust > ch->trust)
 {
  stc("Not on someone with higher trust then you.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
  return;
 victim->pcdata->race->name = '\0';
 victim->pcdata->race->group1 = 0;
 victim->pcdata->race->group2 = 0;
 victim->pcdata->race->complete = FALSE;
 victim->ABlock[POWER_TYPE] = 0;  
 victim->ABlock[POWER_LEVEL] = 0;
 victim->BBlock[POWER_TYPE] = 0;
 victim->BBlock[POWER_LEVEL] = 0;
 victim->CBlock[POWER_TYPE] = 0;
 victim->CBlock[POWER_LEVEL] = 0; 
 for( i = 0; i < 25; i++)
  GET_POWERS(victim, i) = 0;
 victim->class = CLASS_NONE;
 stc("Race is cleared.\n\r", ch);
 stc("Your race has been cleared.\n\r", victim);
 do_save(victim,"");
 return;
}

void do_reimb( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  int x;
  if(IS_NPC(ch))
   return;
  argument = one_argument(argument, arg);
  if( arg[0] == '\0')
  {
   stc("Remimb who?\n\r", ch);
   return;
  }
  if( (victim = get_char_world(ch, arg)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  if( IS_NPC(victim))
   return;

  victim->max_hit = 30000;
  victim->max_mana = 15000;
  victim->max_move = 10000;
  victim->pcdata->quest = 25000;
  victim->stance[STANCE_CRANE] = 200;
  victim->stance[STANCE_BULL] = 200;
  victim->stance[STANCE_MONGOOSE] = 200;
  victim->stance[STANCE_CRAB] = 200;
  victim->stance[STANCE_VIPER] = 200;
  victim->stance[STANCE_DRAGON] = 200;
  victim->stance[STANCE_TIGER] = 200;
  victim->stance[STANCE_SWALLOW] = 200;
  victim->stance[STANCE_MONKEY] = 200;
  victim->stance[STANCE_MANTIS] = 200;
  for( x =0; x < 12; x++)
   victim->wpn[x] = 200;
  victim->spl[RED_MAGIC] = 200;
  victim->spl[BLUE_MAGIC] = 200;
  victim->spl[YELLOW_MAGIC] = 200;
  victim->spl[PURPLE_MAGIC] = 200;
  victim->spl[GREEN_MAGIC] = 200;
  do_save(victim, "");
  stc("Done.\n\r", ch);
  return;
}

void do_checkvnum(CHAR_DATA *ch, char *argument)
{
 FILE *list;
 FILE *area;
 FILE *save;

 char *word;
 char *word2;
 int high = 0;
 int low = 0;
 int holder;
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_STRING_LENGTH];
 
 return;
 if( (list = fopen("area.lst", "r")) == NULL)
 {
  send_to_char("Cannot open area.lst for reading.\n\r", ch);
  return;
 }
 if( (save = fopen("vnums.txt", "w")) == NULL)
 {
  send_to_char("Cannot open area.lst for reading.\n\r", ch);
  return;
 }
 word = fread_word(list);
 one_argument(word, arg); 
 while( str_cmp(word, "$"))
 {
  if( (area = fopen(arg, "r")) == NULL)
  {
   sprintf(buf, "Can not open %s for reading.\n\r", word);
   stc(buf, ch);
   return;
  }
  word2 = fread_word(area);
  while( str_cmp(word2, "#ROOMS"))
   word2 = fread_word(area);
  if( !str_cmp(word2, "#ROOMS"))
  {
     while( str_cmp(word2, "#0"))
     {
      if( word2[0] == '#')
      {
       smash_color(word2);
       holder = atoi(word2);
        if(low == 0)
         low = holder;
        if( high == 0)
         high = holder;
        if( holder < low)
         low = holder;
        if( holder > high)
         high = holder;
       }
       fread_word(area);
      }
  } 
  fprintf( save, "Low value is :%d\nHigh value is:%d\n", low, high);
  fclose(area);
 }
 fclose(list);
 fclose(save);
 return;

}
void do_anum(CHAR_DATA *ch, char *argument)
{
  FILE *fp;
  char *word;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int low = 0;
  int high = 0;
  int holder;
  
  one_argument(argument, arg);

  if( strlen(arg) < 5)
  {
   stc("That is not a valid argument.\n\r", ch);
   return;
  }
  if( (fp = fopen(argument, "r")) == NULL)
  {
   stc("That file doesnt exist or can not be read.\n\r", ch);
   return;
  }
  word = fread_word(fp);
  while( str_cmp(word, "#ROOMS"))
  {
   word = fread_word(fp);
   if( !str_cmp(word, "#$"))
    return;
  }
  if( !str_cmp(word, "#ROOMS"))
  {
   while( str_cmp(word, "#0"))
   {
    if( word[0] == '#')
    {
     smash_color(word);
     holder = atoi(word);
     if(low == 0)
      low = holder;
     if( high == 0)
      high = holder;
     if( holder < low)
      low = holder;
     if( holder > high)
      high = holder;
    } 
    fread_word(fp);
   }
  }
  sprintf( buf, "Low value is :%d\n\rHigh value is:%d\n\r", low, high);
  stc(buf, ch); 
  fclose(fp);
  return;
}
void do_auto_shutdown()
{

/*This allows for a shutdown without somebody in-game actually calling it.
		-Ferric*/
    FILE *fp;
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;
    merc_down = TRUE;

    /* This is to write to the file. */
    fclose(fpReserve);
    if((fp = fopen(LAST_COMMAND,"a")) == NULL)
      bug("Error in do_auto_save opening last_command.txt",0);
   
      fprintf(fp,"Last Command: %s\n",
            last_command);

    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );

    for ( d = descriptor_list; d != NULL; d = d_next)
      {
      if(d->character)
	  do_save (d->character, "");
      d_next = d->next;
      close_socket(d);
      }
    abort();
    return;
}


void do_resetarea(CHAR_DATA *ch, char *argument)
{
    send_to_char("You patiently twiddle your thumbs, waiting for the reset.\n\r",ch);
    send_to_char("Oh fuck this, you MAKE it reset!\n\r",ch);
    area_update();
}

void do_tick(CHAR_DATA *ch, char *argument)
{
    send_to_char("TICK!  Now wasn't that fun for you.\n\r",ch);
    weather_update();
    char_update();
    obj_update();
    area_update();
    room_update();
}

void do_form(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    if (argument[0]=='\0')
    {
	send_to_char("Change your form to resemble what?\n\r",ch);
	return;
    }

    if ((!str_cmp(argument, "SaTaN") 
       ||!str_cmp(argument, "Sluggo") 
       ||!str_cmp(argument, "Chasm") 
       ||!str_cmp(argument, "Fang"))
	&& str_cmp(argument,ch->pcdata->switchname))
    {
	send_to_char("Hmmm... not a good idea.\n\r", ch);
	return;
    }

    sprintf(buf, "%s's form blurs and shifts to resemble %s.",
			ch->name,
			argument);
    act(buf,ch,NULL,NULL,TO_ROOM);
    set_pc_name(ch, argument);
    send_to_char("Your form blurs and shifts to resemble ",ch);
    send_to_char(argument,ch);
    send_to_char(".\n\r",ch);
}



void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level > LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}


void do_testarm( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   CHAR_DATA* victim;
   OBJ_DATA* obj;

   argument = one_argument(argument, arg);
   argument = one_argument(argument, arg2);

   if (arg[0] == '\0')
   {
	send_to_char("Test extra arms on who ?\n\r", ch);
	return;
   }

   if (str_cmp(arg2, "grow") && str_cmp(arg2, "remove"))
   {
	send_to_char("Syntax: testarm <char> <grow|remove>\n\r",ch);
	return;
   }
	
   if ( ( victim = get_char_world(ch, arg)) == NULL)
   {
	send_to_char("They are not here.\n\r", ch);
	return;
   }

   if (!str_cmp(arg2, "grow"))
   {
      if ( IS_SET(victim->newbits, THIRD_HAND) &&
IS_SET(victim->newbits, FOURTH_HAND))
	{
	  send_to_char("They already have 4 hands!\n\r", ch);
	   return;
	}
	
	if (!IS_SET(victim->newbits, THIRD_HAND))
	{
	  SET_BIT(victim->newbits, THIRD_HAND);
	  act("$n grows a third arm!\n\r", victim, NULL, NULL, TO_ROOM);
	  send_to_char("You grow a third arm!\n\r", victim);
	}

	if (!IS_SET(victim->newbits, FOURTH_HAND))
	{
	  SET_BIT(victim->newbits, FOURTH_HAND);
	  act("$n grows a fourth arm!\n\r", victim, NULL, NULL, TO_ROOM);
	  send_to_char("You grow a fourth arm!\n\r", victim);
	}

	send_to_char("Ok.\n\r", ch);
	return;
	}
   else	if (!IS_SET(victim->newbits, THIRD_HAND) &&
!IS_SET(victim->newbits,
FOURTH_HAND))
	{
	  send_to_char("They do not have any extra arms!\n\r", ch);
	  return;
	}

	if (IS_SET(victim->newbits, THIRD_HAND))
	{
	  if (( obj = get_eq_char(victim, WEAR_THIRD)) != NULL)
	  {
		unequip_char(victim, obj);
		act("$n stops using $p.", ch, obj, NULL, TO_ROOM);
		act("You stop using $p.", ch, obj, NULL, TO_CHAR);
	  }
	  REMOVE_BIT(victim->newbits, THIRD_HAND);
	  act("$n's third arm shrivels and falls from their body.\n\r",
victim, NULL, NULL, TO_ROOM);
	  send_to_char("Your third arm shrivels and falls off.\n\r",
victim);
	}

	if (IS_SET(victim->newbits, FOURTH_HAND))
	{
	  if (( obj = get_eq_char(victim, WEAR_FOURTH)) != NULL)
	  {
		unequip_char(victim, obj);
		act("$n stops using $p.", ch, obj, NULL, TO_ROOM);
		act("You stop using $p.", ch, obj, NULL, TO_CHAR);
	  }
	  REMOVE_BIT(victim->newbits, FOURTH_HAND);
	  act("$n's fourth arm shrivels and falls from their body.\n\r",
victim, NULL, NULL, TO_ROOM);
	  send_to_char("Your fourth arm shrivels and falls off.\n\r",
victim);
	}

	send_to_char("Ok.\n\r", ch);
}

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );
	free_string( ch->pcdata->bamfin );
	ch->pcdata->bamfin = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );
	free_string( ch->pcdata->bamfout );
	ch->pcdata->bamfout = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
    }
    return;
}

void do_astat( CHAR_DATA *ch, char *argument )
{
     if( arena == FIGHT_OPEN )
         send_to_char("Arena is [CLEAR]\n\r",ch);

     if( arena == FIGHT_START )
         send_to_char("A challenge has been started.\n\r",ch);

     if( arena == FIGHT_BUSY )
         send_to_char("Arena is [BUSY]\n\r",ch);

     if( arena == FIGHT_LOCK )
         send_to_char("Arena is [LOCKED]\n\r",ch);

     return;
}

void do_aclear( CHAR_DATA *ch, char *argument )
{
     
       arena = FIGHT_OPEN;
       send_to_char("Arena now set [CLEARED]\n\r",ch);
       return;
       sprintf(buf,"[Arena] The arena has been opened.\n\r");
{
 DESCRIPTOR_DATA *d;    
 for ( d = descriptor_list; d; d = d->next )
  {
     if( d->connected == CON_PLAYING)
     {
        send_to_char( buf, d->character );
     }
  }
}
     }

void do_abusy( CHAR_DATA *ch, char *argument )
{
       arena = FIGHT_BUSY;
       send_to_char("Arena now set [BUSY]\n\r",ch);
       sprintf(buf,"[Arena] The arena is now busy.\n\r");
{
 DESCRIPTOR_DATA *d;
 for ( d = descriptor_list; d; d = d->next )
  {
     if( d->connected == CON_PLAYING)
     {
        send_to_char( buf, d->character );
     }
  }
    }
     }

void do_alock( CHAR_DATA *ch, char *argument )
{
     
       arena = FIGHT_LOCK;
       send_to_char("Arena now set [LOCKED]\n\r",ch);
       sprintf(buf,"[Arena] The arena has been locked.\n\r");
 {
   DESCRIPTOR_DATA *d;    
 for ( d = descriptor_list; d; d = d->next )
  {
     if( d->connected == CON_PLAYING)
     {
        send_to_char( buf, d->character );
     }
  }
       
 }
}


void do_nosummon( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch summon ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_SUMMON);
	send_to_char("You now cant be the target of summon and portal.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_SUMMON);
	send_to_char("You now can be the target of summon and portal.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_SUMMON) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_transport( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0')
    {
	send_to_char( "Do you wish to switch transport ON or OFF?\n\r", ch );
	return;
    }

    if (IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"off"))
    {
	REMOVE_BIT(ch->immune, IMM_TRANSPORT);
	send_to_char("You can no longer be the target of transport spells.\n\r", ch);
    }
    else if (!IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"off")) {
	send_to_char("But it is already off!\n\r", ch);
	return;}
    else if (!IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"on"))
    {
	SET_BIT(ch->immune, IMM_TRANSPORT);
	send_to_char("You can now be the target of transport spells.\n\r", ch);
    }
    else if (IS_IMMUNE(ch, IMM_TRANSPORT) && !str_cmp(arg,"on")) {
	send_to_char("But it is already on!\n\r", ch);
	return;}
    else
	send_to_char( "Do you wish to switch it ON or OFF?\n\r", ch );
    return;
}

void do_watcher( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_WATCHER)
	|| (ch->level > 6)
	|| (ch->level < 2)
	|| (ch->trust > 0) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	do_users(ch,"");
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (ch == victim)
    {
	send_to_char( "Not on yourself.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "You better not, they are being snooped.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_WATCHER);
    act( "$n has made you a watcher.", ch, NULL, victim, TO_VICT );
    act( "You make $N a watcher.", ch, NULL, victim, TO_CHAR );
    return;
}



void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Deny %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    send_to_char( "OK.\n\r", ch );
    do_quit( victim, "" );

    return;
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Disconnect %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    } 	if (ch->level < victim->level) 	{ 		send_to_char("You cannot disconnect them.\n\r", ch); 		return; 	}

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}



void do_info( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }

    if( IS_NPC(ch) )
	return;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING && 
		!IS_SET(d->character->deaf, CHANNEL_INFO) )
	{
	    send_to_char( "#6Info ->#n ",d->character );
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_watching( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING && 
	    IS_SET(d->character->act, PLR_WATCHER) &&
	    !IS_SET(d->character->deaf, CHANNEL_INFO) )
	{
	    send_to_char( "Monitor -> ",d->character );
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void logchan( char *argument )
{
    CHAR_DATA *ch;
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( ( ch = d->character ) == NULL ) continue;
	if ( (d->connected == CON_PLAYING) && IS_JUDGE(ch) && 
	    !IS_SET(ch->deaf,CHANNEL_LOG) )
	{
	    send_to_char( "[", ch );
	    send_to_char( argument, ch );
	    send_to_char( "]\n\r",  ch );
	}
    }

    return;
}

void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Echo %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Echo what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Recho %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Recho what?\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}



ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) && atoi(arg) != 30008 )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL && obj->in_room != NULL)
	return obj->in_room;

    if ( obj != NULL && obj->carried_by != NULL && obj->carried_by->in_room != NULL)
	return obj->carried_by->in_room;

    if ( obj != NULL && obj->in_obj != NULL && obj->in_obj->in_room != NULL)
	return obj->in_obj->in_room;

    if ( obj != NULL && obj->in_obj != NULL && obj->in_obj->carried_by && obj->in_obj->carried_by->in_room != NULL)
	return obj->in_obj->carried_by->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    CHAR_DATA *mount;

    sprintf(buf,"%s: Transfer %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}

	if ( room_is_private( location ) )
	{
	    send_to_char( "That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( "$n arrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( "$n has transferred you.", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
    if ( (mount = victim->mount) == NULL) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    if ( ch != mount )
	act( "$n has transferred you.", ch, NULL, mount, TO_VICT );
    do_look( mount, "auto" );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    CHAR_DATA *wch;
    
    sprintf(buf,"%s: At %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    break;
	}
    }

    return;
}



void do_goto( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *in_room;
    int vnum;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	vnum = atoi( arg );
	if ( vnum < 0 || get_room_index( vnum ) )
	{
		send_to_char( "You cannot find that...\n\r", ch );
		return;
	}
	if ( get_trust( ch ) < MAX_LEVEL - 2
	||   vnum < 1 || IS_NPC(ch) )
	{
	  send_to_char( "No such location.\n\r", ch );
	  return;
	}

	location = make_room( vnum );
	if ( !location )
	{
	  bug( "Goto: make_room failed", 0 );
	  return;
	}

	send_to_char( "Waving your hand, you form order from swirling chaos,\n\rand step into a new reality...\n\r", ch ); 
    }

    in_room = ch->in_room;
    if ( ch->fighting )
	stop_fighting( ch, TRUE );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
	act( "$n $T", ch, NULL,
	    (ch->pcdata && ch->pcdata->bamfout[0] != '\0')
	    ? ch->pcdata->bamfout : "leaves in a swirling mist.",  TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );

    if ( !IS_SET(ch->act, PLR_WIZINVIS) )
	act("$n $T", ch, NULL,
	    (ch->pcdata && ch->pcdata->bamfin[0] != '\0')
	    ? ch->pcdata->bamfin : "appears in a swirling mist.", TO_ROOM );

    do_look( ch, "auto" );

    if ( ch->in_room == in_room )
      return;

    return;
}



void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;
	char *area_name;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( ch->in_room != location && room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

	/* area_name fix by Dral */
	if ( location->area == NULL )
	{
		area_name = "No area";
	}
	else if ( location->area->name == NULL )
	{
		area_name = "Unnamed area";
	}
	else
	{
		area_name = location->area->name;
	}
	/* end of area_name fix */
    sprintf( buf, "Name: '%s.'\n\rArea: '%s'.\n\r",
	location->name,
	area_name );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %d.  Sector: %d.  Light: %d.\n\r",
	location->vnum,
	location->sector_type,
	location->light );
    send_to_char( buf, ch );

    sprintf( buf,
	"Room flags: %d.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );
    if(location->trapped_by != NULL)
    {
     sprintf( buf, "Trapped by: %s\n\r", location->trapped_by);
     stc(buf, ch);
    }
    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
     if( IS_IMMORTAL(rch) && !can_see(ch,rch) )
      continue;
	send_to_char( " ", ch );
	one_argument( rch->name, buf );
	send_to_char( buf, ch );
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",

		door,
		pexit->to_room != NULL ? pexit->to_room->vnum : 0,
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }

    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char nm1[40];
    char nm2[40];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Ostat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
	sprintf(nm1,obj->questmaker); else sprintf(nm1,"None");
    if (obj->questowner != NULL && strlen(obj->questowner) > 1)
	sprintf(nm2,obj->questowner); else sprintf(nm2,"None");

    sprintf( buf, "Name: %s.\n\r",
	obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d.  Type: %s.\n\r",
	obj->pIndexData->vnum, item_type_name( obj ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );

    sprintf( buf, "Object creator: %s.  Object owner: %s.  Quest points: %d.\n\r", nm1, nm2, obj->points );
    send_to_char( buf, ch );
    if (obj->quest != 0)
    {
    	send_to_char( "Quest selections:", ch );
    	if (IS_SET(obj->quest,QUEST_STR)) send_to_char(" Str",ch);
    	if (IS_SET(obj->quest,QUEST_DEX)) send_to_char(" Dex",ch);
    	if (IS_SET(obj->quest,QUEST_INT)) send_to_char(" Int",ch);
    	if (IS_SET(obj->quest,QUEST_WIS)) send_to_char(" Wis",ch);
    	if (IS_SET(obj->quest,QUEST_CON)) send_to_char(" Con",ch);
    	if (IS_SET(obj->quest,QUEST_HIT)) send_to_char(" Hp",ch);
    	if (IS_SET(obj->quest,QUEST_MANA)) send_to_char(" Mana",ch);
    	if (IS_SET(obj->quest,QUEST_MOVE)) send_to_char(" Move",ch);
    	if (IS_SET(obj->quest,QUEST_HITROLL)) send_to_char(" Hit",ch);
    	if (IS_SET(obj->quest,QUEST_DAMROLL)) send_to_char(" Dam",ch);
    	if (IS_SET(obj->quest,QUEST_AC)) send_to_char(" Ac",ch);
    	send_to_char( ".\n\r", ch );
    }
    sprintf( buf, "Wear bits: %d.  Extra bits: %s.\n\r",
	obj->wear_flags, extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Weight: %d/%d.\n\r",
	obj->weight, get_obj_weight( obj ) );
    send_to_char( buf, ch );

    sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.\n\r",
	obj->cost, obj->timer, obj->level );
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %d.  In object: %s.  Carried by: %s.  Wear_loc: %d.\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" : obj->carried_by->name,
	obj->wear_loc );
    send_to_char( buf, ch );
    
    sprintf( buf, "Values: %d %d %d %d.\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    send_to_char( buf, ch );

    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	send_to_char( "'.\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier );
	send_to_char( buf, ch );
    }

    return;
}

void do_pstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		send_to_char( "Pstat whom?\n\r", ch );
		return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }
	if (IS_NPC(victim) ) 	{ 		send_to_char("Cannot do that to a mob\n\r", ch); 		return; 	}     sprintf( buf, "Player Statistics For %s.\n\r", victim->name );
    send_to_char( buf, ch );
    sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r", get_curr_str(victim),
	get_curr_int(victim), get_curr_wis(victim), get_curr_dex(victim), get_curr_con(victim) );
    send_to_char( buf, ch );
    sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Primal: %d.\n\r",
	victim->hit, victim->max_hit, victim->mana, victim->max_mana, victim->move, victim->max_move, victim->practice );
    send_to_char( buf, ch );
	send_to_char("Stances.\n\r", ch);
	sprintf( buf, "Vipe : %3d Cran : %3d Mong : %3d Bull : %3d Crab : %3d\n\r",
	victim->stance[STANCE_VIPER], victim->stance[STANCE_CRANE], victim->stance[STANCE_MONGOOSE], victim->stance[STANCE_BULL], victim->stance[STANCE_CRAB]);
	send_to_char( buf, ch);
	sprintf( buf, "Mant : %3d Tige : %3d Drag : %3d Monk : %3d Swal : %3d",
	victim->stance[STANCE_MANTIS], victim->stance[STANCE_TIGER], victim->stance[STANCE_DRAGON], victim->stance[STANCE_MONKEY], victim->stance[STANCE_SWALLOW]);
	send_to_char( buf, ch);
	send_to_char ("\n\r", ch);
	send_to_char("Weapon Skills.\n\r", ch);
	sprintf( buf, "Unarm: %3d Slice: %3d Stab : %3d Slash: %3d Whip : %3d Claw : %3d Blast: %3d\n\r",
	victim->wpn[0],victim->wpn[1],victim->wpn[2],victim->wpn[3],victim->wpn[4],victim->wpn[5],victim->wpn[6]);
	send_to_char(buf,ch);
	sprintf( buf, "Pound: %3d Crush: %3d Grep : %3d Bite : %3d Pierc: %3d Suck : %3d\n\r",
	victim->wpn[7],victim->wpn[8],victim->wpn[9],victim->wpn[10], victim->wpn[11],victim->wpn[12]);
	send_to_char(buf,ch);
	send_to_char("Magic.\n\r", ch);
	sprintf( buf, "Purpl: %3d Red  : %3d Blue : %3d Green: %3d Yello: %3d\n\r",
	victim->spl[0], victim->spl[1], victim->spl[2], victim->spl[3], victim->spl[4]);
	send_to_char( buf, ch);
}

void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		send_to_char( "Mstat whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
    }

    sprintf( buf, "Name: %s.\n\r", victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d.  Sex: %s.  Room: %d.\n\r",	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	victim->sex == SEX_MALE    ? "male"   :
	victim->sex == SEX_FEMALE  ? "female" : "neutral",
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    sprintf( buf, "Str: %d.  Int: %d.  Wis: %d.  Dex: %d.  Con: %d.\n\r",
	get_curr_str(victim),
	get_curr_int(victim),
	get_curr_wis(victim),
	get_curr_dex(victim),
	get_curr_con(victim) );
    send_to_char( buf, ch );

    sprintf( buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Primal: %d.\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
	victim->practice );
    send_to_char( buf, ch );
	
    sprintf( buf,
	"Lv: %d.  Align: %d.  AC: %d.  Exp: %d.\n\r",
	victim->level,       victim->alignment,
	class_ac(victim), victim->exp );
    send_to_char( buf, ch );

    sprintf( buf, "Hitroll: %d.  Damroll: %d.  Position: %d.  Wimpy: %d.\n\r",
	class_hitroll(victim), class_damroll(victim),
	victim->position,    victim->wimpy );
    send_to_char( buf, ch );
	
    sprintf( buf, "Fighting: %s.\n\r",
	victim->fighting ? victim->fighting->name : "(none)" );
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "Thirst: %d.  Full: %d.  Drunk: %d.  Saving throw: %d.\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK],
	    victim->saving_throw );
	send_to_char( buf, ch );


    }

    sprintf( buf, "Carry number: %d.  Carry weight: %d.\n\r",
	victim->carry_number, victim->carry_weight );
    send_to_char( buf, ch );

    sprintf( buf, "Age: %d.  Played: %d.  Timer: %d.  Act: %d.\n\r",
	get_age( victim ), (int) victim->played, victim->timer, victim->act );
    send_to_char( buf, ch );

    sprintf( buf, "Master: %s.  Leader: %s.  Affected by: %s.\n\r",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	affect_bit_name( victim->affected_by ) );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s.\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none).\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
	send_to_char( "Mobile has spec fun.\n\r", ch );

	if (!IS_NPC(victim))
	{
		for ( paf = victim->affected; paf != NULL; paf = paf->next )
		{
                  if( paf->level != 1)
                  {
			sprintf( buf,
			"Spell: '%s' modifies %s by %d for %d hours with bits %s.\n\r",
			skill_table[(int) paf->type].name,
			affect_loc_name( paf->location ),
			paf->modifier,
			paf->duration,
			affect_bit_name( paf->bitvector )
			);
                  }
                  else
                  {
                 sprintf( buf,
                "Spell: '%s'  modifies %s by %d for %d hours with bits %s.\n\r",
                 skill_table[(int) paf->type].name,   
                 affect_loc_name(paf->location),
                 paf->modifier,
                 paf->duration,  
                 affect_bit_name2(paf->bitvector));
                 }
		}
	}
    return;
}



void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mfind whom?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pMobIndex->vnum, capitalize( pMobIndex->short_descr ) );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Ofind what?\n\r", ch );
	return;
    }

    fAll	= !str_cmp( arg, "all" );
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name( arg, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pObjIndex->vnum, capitalize( pObjIndex->short_descr ) );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );

    return;
}



void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Mwhere whom?\n\r", ch );
	return;
    }

    found = FALSE;
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if ( IS_NPC(victim)
	&&   victim->in_room != NULL
	&&   is_name( arg, victim->name ) )
	{
	    found = TRUE;
	    sprintf( buf, "[%5d] %-28s [%5d] %s\n\r",
		victim->pIndexData->vnum,
		victim->short_descr,
		victim->in_room->vnum,
		victim->in_room->name );
	    send_to_char( buf, ch );
	}
    }

    if ( !found )
	act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );

    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}



void do_reboot( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    sprintf( buf, "\n\rThe Vortex Has Been Rebooted.");
    do_echo( ch, buf );
    do_forceauto(ch,"call all");
    call_all(ch);
    do_forceauto(ch,"save");
    do_autosave(ch,"");
    merc_down = TRUE;
    return;
}



void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    sprintf( buf, "Shutdown by %s.", ch->name );
    append_file( ch, SHUTDOWN_FILE, buf );
    strcat( buf, "\n\r" );
    do_echo( ch, buf );
    do_forceauto(ch,"save");
    do_autosave(ch,"");
    merc_down = TRUE;
    return;
}



void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Snoop %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act, PLR_WATCHER))
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oswitch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *mount;

    sprintf(buf,"%s: Oswitch %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into what?\n\r", ch );
	return;
    }

    if ( IS_EXTRA(ch, EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD) )
    {
	send_to_char( "You are already oswitched.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
    {
	send_to_char( "Not while polymorphed.\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) || IS_EXTRA(ch,EXTRA_SWITCH) )
    {
	send_to_char( "Not while switched.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( obj->chobj != NULL )
    {
	send_to_char( "Object in use.\n\r", ch );
	return;
    }

    if ( (mount = ch->mount) != NULL) do_dismount(ch,"");
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oreturn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    sprintf(buf,"%s: Oreturn",ch->name);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    
    if ( IS_NPC(ch) )
	return;
    
    if ( !IS_EXTRA(ch, EXTRA_OSWITCH) && !IS_HEAD(ch,LOST_HEAD) )
    {
	send_to_char( "You are not oswitched.\n\r", ch );
	return;
    }
    if ( ( obj = ch->pcdata->chobj ) != NULL ) obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    if (IS_HEAD(ch,LOST_HEAD)) REMOVE_BIT(ch->loc_hp[0], LOST_HEAD);
    free_string(ch->morph);
    ch->morph = str_dup("");
    char_from_room(ch);
    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Switch %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim))
    {
	send_to_char( "Only on NPC's.\n\r", ch );
	return;
    }

    SET_BIT(ch->extra, EXTRA_SWITCH);
    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;
    send_to_char( "Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Return %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( "You return to your original body.\n\r", ch );
    REMOVE_BIT(ch->desc->original->extra, EXTRA_SWITCH);
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc; 
    ch->desc                  = NULL;
    return;
}



void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;

    sprintf(buf,"%s: Mload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
    
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: mload <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
    act( "You have created $N!", ch, NULL, victim, TO_CHAR );
    return;
}




void do_pload( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *in_room;
    bool fOld;

    if ( IS_NPC(ch) || ch->desc == NULL || ch->in_room == NULL ) return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Syntax: pload <name>.\n\r", ch );
	return;
    }

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( !char_exists(FALSE,argument) )
    {
	send_to_char( "That player doesn't exist.\n\r", ch );
	return;
    }

    sprintf(buf,"%s: Pload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
    
    argument[0] = UPPER(argument[0]);

    sprintf(buf,"You transform into %s.\n\r",argument);
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms into %s.",argument);
    act(buf,ch,NULL,NULL,TO_ROOM);

    d = ch->desc;
    do_autosave(ch,"");
    in_room = ch->in_room;
    extract_char(ch, TRUE);
    d->character = NULL;
    fOld = load_char_obj( d, argument );
    ch   = d->character;
    ch->next = char_list;
    char_list = ch;
    char_to_room(ch,in_room);
    return;
}



void do_preturn( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool fOld;

    if (IS_NPC(ch)) {send_to_char("Huh?\n\r",ch);return;}

    if (ch->pload == NULL) {send_to_char("Huh?\n\r",ch);return;}
    sprintf(arg,ch->pload);
    if (strlen(arg) < 3 || strlen(arg) > 8) 
	{send_to_char("Huh?\n\r",ch);return;}

    if (!str_cmp(ch->name,arg)) {send_to_char("Huh?\n\r",ch);return;}

    d = ch->desc;

    sprintf(buf,"You transform back into %s.\n\r",capitalize(ch->pload));
    send_to_char(buf,ch);
    sprintf(buf,"$n transforms back into %s.",capitalize(ch->pload));
    act(buf,ch,NULL,NULL,TO_ROOM);
    do_autosave(ch,"");
    if (ch != NULL && ch->desc != NULL)
	extract_char(ch,TRUE);
    else if (ch != NULL)
	extract_char(ch,TRUE);
    if (ch->desc) ch->desc->character = NULL;

    fOld = load_char_obj(d, capitalize(arg));
    if (ch->in_room != NULL)
    	char_to_room(ch,ch->in_room);
    else
    	char_to_room(ch,get_room_index(3001));
    free_string(ch->pload);
    ch->pload = str_dup("");
    return;
}


void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    sprintf(buf,"%s: Oload %s",ch->name,argument);
    if (ch->level < NO_WATCH && ch->trust > 3) do_watching(ch,buf);
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
        return;
    }
 
    if ( arg2[0] == '\0' )
    {
	level = get_trust( ch );
    }
    else
    {
	/*
	 * New feature from Alander.
	 */
        if ( !is_number( arg2 ) )
        {
	    send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	    return;
        }
        level = atoi( arg2 );
	if ( level < 0 || level > get_trust( ch ) )
        {
	    send_to_char( "Limited to your trust level.\n\r", ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    if( obj->pIndexData->vnum == 3390 )
      newbie_trans(obj);
    if ( CAN_WEAR(obj, ITEM_TAKE) )
    {
	obj_to_char( obj, ch );
	act( "$p appears in $n's hands!", ch, obj, NULL, TO_ROOM );
    }
    else
    {
	obj_to_room( obj, ch->in_room );
	act( "$n has created $p!", ch, obj, NULL, TO_ROOM );
    }
    act( "You create $p.", ch, obj, NULL, TO_CHAR );
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    return;
}



void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if (IS_NPC(victim) && victim->desc == NULL && (mount = victim->mount) == NULL)
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    extract_obj( obj );
	}

	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on PC's.\n\r", ch );
	return;
    }
    if ( victim->desc != NULL )
    {
	send_to_char( "Not on switched players.\n\r", ch );
	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE );
    return;
}



void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int level;

    sprintf(buf,"%s: Trust %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: trust <char> <trust>.\n\r", ch );
	send_to_char( "Trust being one of: None, Mortal, Avatar, Builder, Questmaker, Enforcer, Judge, or Highjudge.\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

         if ( !str_cmp( arg2, "none"       ) ) level = 0;
    else if ( !str_cmp( arg2, "mortal"     ) ) level = 2;
    else if ( !str_cmp( arg2, "avatar"      ) ) level = 3;
    else if ( !str_cmp( arg2, "builder"    ) ) level = 7;
    else if ( !str_cmp( arg2, "questmaker" ) ) level = 8;
    else if ( !str_cmp( arg2, "enforcer"   ) ) level = 9;
    else if ( !str_cmp( arg2, "judge"      ) ) level = 10;
    else if ( !str_cmp( arg2, "highjudge"  ) ) level = 11;
    else
    {
	send_to_char( "Please enter: None, Mortal, Builder, Questmaker, Enforcer, Judge, or Highjudge.\n\r", ch );
	return;
    }

    if ( level >= get_trust( ch ) )
    {
	send_to_char( "Limited to below your trust.\n\r", ch );
	return;
    }
    send_to_char("Ok.\n\r",ch);
    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;

    sprintf(buf,"%s: Restore %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Restore whom?\n\r", ch );
	return;
    }

    if ( get_trust(ch) >= MAX_LEVEL - 2  && !str_cmp(arg,"all"))
    {
    /* cure all */
    	
        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;
                
          
            {
		affect_strip(victim,gsn_drowfire);
		affect_strip(victim,gsn_poison);
		affect_strip(victim,gsn_blindness);
		affect_strip(victim,gsn_sleep);
		affect_strip(victim,gsn_curse);
            
		victim->hit 	= victim->max_hit;
		victim->mana	= victim->max_mana;
		victim->move	= victim->max_move;
		update_pos( victim);
		if (victim->in_room != NULL)
		    act("$n has restored you.",ch,NULL,victim,TO_VICT);
	    }
        }
	send_to_char("All active players restored.\n\r",ch);
	return;
    }



    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    victim->loc_hp[0] = 0;
    victim->loc_hp[1] = 0;
    victim->loc_hp[2] = 0;
    victim->loc_hp[3] = 0;
    victim->loc_hp[4] = 0;
    victim->loc_hp[5] = 0;
    victim->loc_hp[6] = 0;
    update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Freeze %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Log %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Noemote %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }


    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Notell %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_NO_TELL) )
    {
	REMOVE_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NO_TELL removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_NO_TELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NO_TELL set.\n\r", ch );
    }

    return;
}


void do_undeny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Undeny %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Undeny whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

 
    if ( IS_SET(victim->act, PLR_DENY) )
    {
	REMOVE_BIT(victim->act, PLR_DENY);
	send_to_char( "You are no longer denied.\n\r", victim );
	send_to_char( "DENY removed.\n\r", ch );
    }
    
    return;
}

void do_silence( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    sprintf(buf,"%s: Silence %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Silence whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_SILENCE) )
    {
	REMOVE_BIT(victim->act, PLR_SILENCE);
	send_to_char( "You can use channels again.\n\r", victim );
	send_to_char( "SILENCE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_SILENCE);
	send_to_char( "You can't use channels!\n\r", victim );
	send_to_char( "SILENCE set.\n\r", ch );
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Peace %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



BAN_DATA *		ban_free;
BAN_DATA *		ban_list;

void do_ban( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    BAN_DATA *pban;

    sprintf(buf2,"%s: Ban %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	strcpy( buf, "Banned sites:\n\r" );
	for ( pban = ban_list; pban != NULL; pban = pban->next )
	{
	    strcat( buf, pban->name );
	    strcat( buf, "\n\r" );
	}
	send_to_char( buf, ch );
	return;
    }

    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_cmp( arg, pban->name ) )
	{
	    send_to_char( "That site is already banned!\n\r", ch );
	    return;
	}
    }

    if ( ban_free == NULL )
    {
	pban		= alloc_perm( sizeof(*pban) );
    }
    else
    {
	pban		= ban_free;
	ban_free	= ban_free->next;
    }

    pban->name	= str_dup( arg );
    pban->next	= ban_list;
    ban_list	= pban;
    send_to_char( "Ok.\n\r", ch );
	save_bans();
    return;
}



void do_allow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    BAN_DATA *prev;
    BAN_DATA *curr;

    sprintf(buf,"%s: Allow %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove which site from the ban list?\n\r", ch );
	return;
    }

    prev = NULL;
    for ( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
    {
	if ( !str_cmp( arg, curr->name ) )
	{
	    if ( prev == NULL )
		ban_list   = ban_list->next;
	    else
		prev->next = curr->next;

	    free_string( curr->name );
	    curr->next	= ban_free;
	    ban_free	= curr;
	    send_to_char( "Ok.\n\r", ch );
	   save_bans();
	    return;
	}
    }

    send_to_char( "Site is not banned.\n\r", ch );
    return;
}

void do_wizlock( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Wizlock %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);
    wizlock = !wizlock;

    if ( wizlock )
	send_to_char( "Game wizlocked.\n\r", ch );
    else
	send_to_char( "Game un-wizlocked.\n\r", ch );

    return;
}



void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Slookup what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
		sn, skill_table[sn].slot, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %4d Slot: %4d Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].slot, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}



void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    sprintf(buf,"%s: Sset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: sset <victim> <skill> <value>\n\r",	ch );
	send_to_char( "or:     sset <victim> all     <value>\n\r",	ch );
	send_to_char( "Skill being any skill or spell.\n\r",		ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL )
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    send_to_char("Ok.\n\r",ch);
    return;
}



void do_pack(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  OBJ_DATA *obj2;
  char arg[MAX_STRING_LENGTH];
  one_argument(argument,arg);
  if (arg[0] == '\0')
  {send_to_char("Newbiepack Who?\n\r",ch);
  return;}
  if ((victim=get_char_world(ch,arg)) == NULL)
  {send_to_char("They arent here.\n\r",ch);
  return;}
  obj2 = create_object(get_obj_index(3032),0);
  obj  = create_object(get_obj_index(3374),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3375),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3375),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3376),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3376),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3377),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3377),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3378),0);
  obj_to_char(obj,victim);
  obj = NULL;
 
  obj  = create_object(get_obj_index(3379),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3380),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3381),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3382),0);
  obj_to_char(obj,victim);
  obj = NULL;
 
  obj  = create_object(get_obj_index(3383),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3384),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3385),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3386),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3387),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3388),0);
  obj_to_char(obj,victim);
  obj = NULL;
  obj  = create_object(get_obj_index(3389),0);
  obj_to_char(obj,victim);
  obj = NULL; 
  obj  = create_object(get_obj_index(3390),0);
  newbie_trans(obj);
  obj_to_char(obj,victim);
  obj = NULL;
 
  obj_to_char(obj2,victim);
  send_to_char( "Tons of stuff appears in your hands in a blast of flames.\n\r",victim);
  send_to_char("You newbiepack them.\n\r",ch);
  return;
}
 
 

void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *morph;
    OBJ_INDEX_DATA *pObjIndex;
    int value;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    sprintf(buf,"%s: Oset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: oset <object> <field>  <value>\n\r",	ch );
	send_to_char( "or:     oset <object> <string> <value>\n\r",	ch );
	send_to_char( "or:     oset <object> <affect> <value>\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  value0 value1 value2 value3\n\r",		ch );
	send_to_char( "  level weight cost timer morph\n\r",		ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "String being one of:\n\r",			ch );
	send_to_char( "  name short long ed type extra wear owner\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Affect being one of:\n\r",			ch );
	send_to_char( "  str dex int wis con\n\r",			ch );
	send_to_char( "  hit dam ac hp mana move\n\r",			ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker)))
    {
	send_to_char("You don't have permission to change that item.\n\r", ch);
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
	if (obj->item_type == ITEM_WEAPON && !IS_JUDGE(ch)) {
		send_to_char("You are not authorised to create spell weapons.\n\r", ch);
		return;}
	else if (obj->item_type == ITEM_QUEST) {
		send_to_char("You cannot change a quest tokens value with oset.\n\r", ch);
		return;}
	else if (obj->item_type == ITEM_ARMOR && value > 15)
		obj->value[0] = 15;
	else
		obj->value[0] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
	obj->value[1] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
	obj->value[2] = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	if (obj->item_type == ITEM_ARMOR && !IS_JUDGE(ch))
		send_to_char("You are not authorised to create spell armour.\n\r", ch);
	else
		{obj->value[3] = value; send_to_char("Ok.\n\r",ch);}
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "morph" ) )
    {
	int mnum;
	if ( IS_SET(obj->spectype, SITEM_MORPH) )
    	{
	    send_to_char( "This item can already morph.\n\r", ch );
	    return;
    	}
    	if ( ( pObjIndex = get_obj_index( value ) ) == NULL )
    	{
	    send_to_char( "No object has that vnum.\n\r", ch );
	    return;
    	}
    	morph = create_object( pObjIndex, obj->level );
    	if ( !CAN_WEAR(morph, ITEM_TAKE) )
	    SET_BIT(morph->wear_flags, ITEM_TAKE);
	if ( !IS_SET(obj->spectype, SITEM_MORPH) )
	    SET_BIT(obj->spectype, SITEM_MORPH);
	if ( !IS_SET(morph->spectype, SITEM_MORPH) )
	    SET_BIT(morph->spectype, SITEM_MORPH);
	obj_to_char( morph, ch );
	act( "$p morphs into $P in $n's hands!", ch, obj, morph, TO_ROOM );
	act( "$p morphs into $P in your hands!", ch, obj, morph, TO_CHAR );
	mnum = obj->wear_loc;
	obj_from_char( obj );
	obj_to_obj( obj, morph );
	if ( morph->wear_flags == obj->wear_flags && mnum != WEAR_NONE )
	    equip_char( ch, morph, mnum );
    	if (morph->questmaker != NULL) free_string(morph->questmaker);
    	morph->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "extra" ) )
    {
	if      ( !str_cmp( arg3, "glow"         )) value = ITEM_GLOW;
	else if ( !str_cmp( arg3, "hum"          )) value = ITEM_HUM;
	else if ( !str_cmp( arg3, "thrown"       )) value = ITEM_THROWN;
	else if ( !str_cmp( arg3, "vanish"       )) value = ITEM_VANISH;
	else if ( !str_cmp( arg3, "invis"        )) value = ITEM_INVIS;
	else if ( !str_cmp( arg3, "magic"        )) value = ITEM_MAGIC;
	else if ( !str_cmp( arg3, "nodrop"       )) value = ITEM_NODROP;
	else if ( !str_cmp( arg3, "bless"        )) value = ITEM_BLESS;
	else if ( !str_cmp( arg3, "anti-good"    )) value = ITEM_ANTI_GOOD;
	else if ( !str_cmp( arg3, "anti-evil"    )) value = ITEM_ANTI_EVIL;
	else if ( !str_cmp( arg3, "anti-neutral" )) value = ITEM_ANTI_NEUTRAL;
	else if ( !str_cmp( arg3, "noremove"     )) value = ITEM_NOREMOVE;
	else if ( !str_cmp( arg3, "inventory"    )) value = ITEM_INVENTORY;
	else if ( !str_cmp( arg3, "loyal"        )) value = ITEM_LOYAL;
	else {
	    send_to_char("Extra flag can be from the following: Glow, Hum, Vanish, Invis,\n\r",ch);
	    stc("Magic, Nodrop, Bless, Anti-Good, Anti-Evil, Anti-Neutral, Noremove,\n\r",ch);
	    stc("Inventory, Loyal.\n\r",ch); return;}

	/* Removing magic flag allows multiple enchants */
	if (IS_SET(obj->extra_flags,value) && value == ITEM_MAGIC && !IS_JUDGE(ch))
	{send_to_char("Sorry, no can do...\n\r",ch); return;}

	if (IS_SET(obj->extra_flags,value))
	    REMOVE_BIT(obj->extra_flags, value);
	else
	    SET_BIT(obj->extra_flags, value);
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "wear" ) )
    {
	if      ( !str_cmp( arg3, "none" ) || !str_cmp( arg3, "clear" ))
	{
	    obj->wear_flags = 0;
	    send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);
	    return;
	}
	else if ( !str_cmp( arg3, "take"   ))
	{
	    if (IS_SET(obj->wear_flags,ITEM_TAKE))
		REMOVE_BIT(obj->wear_flags, ITEM_TAKE);
	    else
		SET_BIT(obj->wear_flags, ITEM_TAKE);
	    send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);
	    return;
	}
	else if ( !str_cmp( arg3, "finger" )) value = ITEM_WEAR_FINGER;
	else if ( !str_cmp( arg3, "neck"   )) value = ITEM_WEAR_NECK;
	else if ( !str_cmp( arg3, "body"   )) value = ITEM_WEAR_BODY;
	else if ( !str_cmp( arg3, "head"   )) value = ITEM_WEAR_HEAD;
	else if ( !str_cmp( arg3, "legs"   )) value = ITEM_WEAR_LEGS;
	else if ( !str_cmp( arg3, "feet"   )) value = ITEM_WEAR_FEET;
	else if ( !str_cmp( arg3, "hands"  )) value = ITEM_WEAR_HANDS;
	else if ( !str_cmp( arg3, "arms"   )) value = ITEM_WEAR_ARMS;
	else if ( !str_cmp( arg3, "about"  )) value = ITEM_WEAR_ABOUT;
	else if ( !str_cmp( arg3, "waist"  )) value = ITEM_WEAR_WAIST;
	else if ( !str_cmp( arg3, "wrist"  )) value = ITEM_WEAR_WRIST;
	else if ( !str_cmp( arg3, "hold"   )) value = ITEM_WIELD;
	else if ( !str_cmp( arg3, "face"   )) value = ITEM_WEAR_FACE;
	else {
	    send_to_char("Wear location can be from: None, Take, Finger, Neck, Body, Head, Legs,\n\r",ch);
	    stc("Hands, Arms, About, Waist, Hold, Face.\n\r",ch); return;}
	if (IS_SET(obj->wear_flags,ITEM_TAKE) ) value += 1;
	obj->wear_flags = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "level" ) )
    {
	if (value < 1) value = 1; else if (value > 50) value = 50;
	if (!IS_JUDGE(ch))
	    send_to_char("You are not authorised to change an items level.\n\r", ch);
	else
	    {obj->level = value; send_to_char("Ok.\n\r",ch);
	    if (obj->questmaker != NULL) free_string(obj->questmaker);
	    obj->questmaker = str_dup(ch->name);}
	return;
    }
	
    if ( !str_cmp( arg2, "weight" ) )
    {
	obj->weight = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "cost" ) )
    {
	if (value > 100000 && !IS_JUDGE(ch))
		send_to_char("Don't be so damn greedy!\n\r",ch);
	else
		{obj->cost = value; send_to_char("Ok.\n\r",ch);
	    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    		obj->questmaker = str_dup(ch->name);}
	return;
    }

    if ( !str_cmp( arg2, "timer" ) )
    {
	obj->timer = value;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }
	
    if      ( !str_cmp( arg2, "hitroll" ) || !str_cmp( arg2, "hit" ) )
	{oset_affect(ch,obj,value,APPLY_HITROLL,FALSE);return;}
    else if ( !str_cmp( arg2, "damroll" ) || !str_cmp( arg2, "dam" ) )
	{oset_affect(ch,obj,value,APPLY_DAMROLL,FALSE);return;}
    else if ( !str_cmp( arg2, "armor" ) || !str_cmp( arg2, "ac" ) )
	{oset_affect(ch,obj,value,APPLY_AC,FALSE);return;}
    else if ( !str_cmp( arg2, "hitpoints" ) || !str_cmp( arg2, "hp" ) )
	{oset_affect(ch,obj,value,APPLY_HIT,FALSE);return;}
    else if ( !str_cmp( arg2, "mana" ) )
	{oset_affect(ch,obj,value,APPLY_MANA,FALSE);return;}
    else if ( !str_cmp( arg2, "move" ) || !str_cmp( arg2, "movement" ) )
	{oset_affect(ch,obj,value,APPLY_MOVE,FALSE);return;}
    else if ( !str_cmp( arg2, "str" ) || !str_cmp( arg2, "strength" ) )
	{oset_affect(ch,obj,value,APPLY_STR,FALSE);return;}
    else if ( !str_cmp( arg2, "dex" ) || !str_cmp( arg2, "dexterity" ) )
	{oset_affect(ch,obj,value,APPLY_DEX,FALSE);return;}
    else if ( !str_cmp( arg2, "int" ) || !str_cmp( arg2, "intelligence" ) )
	{oset_affect(ch,obj,value,APPLY_INT,FALSE);return;}
    else if ( !str_cmp( arg2, "wis" ) || !str_cmp( arg2, "wisdom" ) )
	{oset_affect(ch,obj,value,APPLY_WIS,FALSE);return;}
    else if ( !str_cmp( arg2, "con" ) || !str_cmp( arg2, "constitution" ) )
	{oset_affect(ch,obj,value,APPLY_CON,FALSE);return;}

    if ( !str_cmp( arg2, "type" ) )
    {
	if (!IS_JUDGE(ch))
	{
	    send_to_char("You are not authorised to change an item type.\n\r", ch);
	    return;
	}
	if      (!str_cmp( arg3, "light")) obj->item_type = 1;
	else if (!str_cmp( arg3, "scroll")) obj->item_type = 2;
	else if (!str_cmp( arg3, "wand")) obj->item_type = 3;
	else if (!str_cmp( arg3, "staff")) obj->item_type = 4;
	else if (!str_cmp( arg3, "weapon")) obj->item_type = 5;
	else if (!str_cmp( arg3, "treasure")) obj->item_type = 8;
	else if (!str_cmp( arg3, "armor")) obj->item_type = 9;
	else if (!str_cmp( arg3, "armour")) obj->item_type = 9;
	else if (!str_cmp( arg3, "potion")) obj->item_type = 10;
	else if (!str_cmp( arg3, "furniture")) obj->item_type = 12;
	else if (!str_cmp( arg3, "trash")) obj->item_type = 13;
	else if (!str_cmp( arg3, "container")) obj->item_type = 15;
	else if (!str_cmp( arg3, "drink")) obj->item_type = 17;
	else if (!str_cmp( arg3, "key")) obj->item_type = 18;
	else if (!str_cmp( arg3, "food")) obj->item_type = 19;
	else if (!str_cmp( arg3, "money")) obj->item_type = 20;
	else if (!str_cmp( arg3, "boat")) obj->item_type = 22;
	else if (!str_cmp( arg3, "corpse")) obj->item_type = 23;
	else if (!str_cmp( arg3, "fountain")) obj->item_type = 25;
	else if (!str_cmp( arg3, "pill")) obj->item_type = 26;
	else if (!str_cmp( arg3, "portal")) obj->item_type = 27;
	else if (!str_cmp( arg3, "stake")) obj->item_type = 30;
	else {send_to_char("Type can be one of: Light, Scroll, Wand, Staff, Weapon, Treasure, Armor,\n\r",ch);
	      stc("Potion, Furniture, Trash, Container, Drink, Key, Food, Money, Boat, Corpse,\n\r",ch);
	      stc("Fountain, Pill, Portal, Stake.\n\r", ch);
	     return;}
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "owner" ) )
    {
	if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch);return;}
	if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker)))
	{send_to_char("Someone else has already changed this item.\n\r",ch);return;}
	if ( ( victim = get_char_world( ch, arg3 ) ) == NULL )
	{send_to_char("You cannot find any player by that name.\n\r",ch);return;}
	if (IS_NPC(victim)) {send_to_char("Not on NPC's.\n\r",ch);return;}
	if (obj->questmaker != NULL) free_string(obj->questmaker);
	obj->questmaker = str_dup(ch->name);
	if (obj->questowner != NULL) free_string(obj->questowner);
	obj->questowner = str_dup(victim->name);
	send_to_char("Ok.\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2, "name" ) )
    {
	free_string( obj->name );
	obj->name = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "short" ) )
    {
	free_string( obj->short_descr );
	obj->short_descr = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "long" ) )
    {
	free_string( obj->description );
	obj->description = str_dup( arg3 );
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    if ( !str_cmp( arg2, "ed" ) )
    {
	EXTRA_DESCR_DATA *ed;

	argument = one_argument( argument, arg3 );
	if ( argument == NULL )
	{
	    send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		ch );
	    return;
	}

	if ( extra_descr_free == NULL )
	{
	    ed			= alloc_perm( sizeof(*ed) );
	}
	else
	{
	    ed			= extra_descr_free;
	    extra_descr_free	= ed;
	}

	ed->keyword		= str_dup( arg3     );
	ed->description		= str_dup( argument );
	ed->next		= obj->extra_descr;
	obj->extra_descr	= ed;
	send_to_char("Ok.\n\r",ch);
    	if (obj->questmaker != NULL) free_string(obj->questmaker);
    	obj->questmaker = str_dup(ch->name);
	return;
    }

    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    sprintf(buf,"%s: Rset %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax: rset <location> <field> value\n\r",	ch );
	send_to_char( "\n\r",						ch );
	send_to_char( "Field being one of:\n\r",			ch );
	send_to_char( "  flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_cmp( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}

void do_expmult( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  argument = one_argument(argument, arg);
  if( !is_number(arg))
  return;
  if( IS_NPC(ch))
  return;
  if( atoi(arg) > 3)
  {
   send_to_char("3 is the limit.\n\n", ch);
   return;
  }
  /*if( (ch->level < 11) || (ch->trust < 11))
  {
   send_to_char("You are not godly enough for that.\n\r", ch);
   return;
  }
  else
  {*/
   send_to_char("Done.\n\r", ch);
   if( atoi(arg) == 2)
    do_info(ch, "Oh yeah! Double Exp!\n\r");
   if( atoi(arg) == 3 && ch->level > 10 && ch->trust > 10 )
    do_info(ch, "OOOOHH YYEEAAAHH! Triple exp!\n\r");
   if( atoi(arg) == 3 && ch->level <= 10 && ch->trust <= 10 )
   {
    stc("Try as you might, double is the best you can do...\n\r", ch);
    return;
   }
   if( atoi(arg) == 1)
    do_info(ch, "Fun's over.  Expereince back to normal.\n\r");
   global_exp_mult = atoi(arg);
 /*  return;
  }*/
 return;
}

void do_omni(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if (IS_NPC(ch)) return;
	sprintf(buf, "        Name|Level|Trust|Class|Gen|  Hit| Mana| Move|  HR|  DR|   AC|Quest|Stat\n\r");
	send_to_char(buf,ch);
	sprintf(buf,
"------------|-----|-----|-----|---|-----|-----|-----|----|----|-----|-----|----\n\r");
	send_to_char(buf,ch);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		CHAR_DATA *wch;

		if (d->connected != CON_PLAYING) continue;
		wch = ( d->original != NULL) ? d->original : d->character;

                if( !can_see(ch, wch))
                continue;
sprintf(buf,"%12s|%5d|%5d|%5d|%3d|%5d|%5d|%5d|%4d|%4d|%5d|%5d|%4d\n\r",
		wch->pcdata->switchname,
		wch->level,
		wch->trust,
		wch->class,
		wch->generation,
		wch->max_hit,
		wch->max_mana,
		wch->max_move,
		class_hitroll(wch),
		class_damroll(wch),
		class_ac(wch),
		wch->pcdata->quest,
		wch->race
		);	


		send_to_char(buf,ch);
	}

	return;
}


void do_users( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int count;
	char *st;

    count	= 0;
    buf[0]	= '\0';

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character != NULL && can_see( ch, d->character ) )
	{
	    count++;


	switch (d->connected) {
      case CON_PLAYING:              st = "    PLAYING    ";    break;
      case CON_GET_NAME:             st = "   Get Name    ";    break;
      case CON_GET_OLD_PASSWORD:     st = "Get Old Passwd ";    break;
      case CON_CONFIRM_NEW_NAME:     st = " Confirm Name  ";    break;
      case CON_GET_NEW_PASSWORD:     st = "Get New Passwd ";    break;
      case CON_CONFIRM_NEW_PASSWORD: st = "Confirm Passwd ";    break;
      case CON_GET_NEW_SEX:          st = "  Get New Sex  ";    break;
      case CON_GET_NEW_VT102:        st = " Picking VT102 ";    break;
      case CON_GET_NEW_ANSI:         st = " Picking ANSI  ";    break;
      case CON_GET_NEW_CLASS:        st = " Get New Class ";    break;
      case CON_READ_MOTD:            st = "  Reading MOTD ";    break;
      case CON_NOT_PLAYING:          st = " *Not* Playing ";    break;
      default:                       st = "   !UNKNOWN!   ";    break;
	}

	if (strlen(d->character->lasthost) < 2)
	{
	    sprintf( buf + strlen(buf), "[%3d %16s] [Idle %3d] %s@%s\n\r",
		d->descriptor,
		st,
		d->character->timer,
		d->original  ? d->original->pcdata->switchname  :
		d->character ? d->character->pcdata->switchname : "(none)",
		d->host
		);
	}
	else
	{
	    sprintf( buf + strlen(buf), "[%3d %16s] [Idle %3d] %s@%s\n\r",
		d->descriptor,
		st,
		d->character->timer,
		d->original  ? d->original->pcdata->switchname  :
		d->character ? d->character->pcdata->switchname : "(none)",
		d->character->lasthost
		);
	}

	}
    }

    sprintf( buf2, "%d user%s\n\r", count, count == 1 ? "" : "s" );
    send_to_char( buf2, ch );
    send_to_char( buf, ch );
    return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    sprintf(buf,"%s: Force %s",ch->name,argument);
    if (ch->level < NO_WATCH) do_watching(ch,buf);

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    
       
    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (get_trust(ch) < MAX_LEVEL - 4)
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
	    {
		act( buf, ch, NULL, vch, TO_VICT );
		interpret( vch, argument );
	    }
	}
    }

	

    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	act( "$n forces you to '$t'.", ch, argument, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_forceauto( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( !IS_NPC(vch) && vch != ch )
	{
	    act( "Autocommand: $t.", ch, argument, vch, TO_VICT );
	    interpret( vch, argument );
	}
    }
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
	SET_BIT(ch->act, PLR_WIZINVIS);
    }

    return;
}

void do_incog( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_INCOG) )
    {
	REMOVE_BIT(ch->act, PLR_INCOG);
	act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade back into existence.\n\r", ch );
    }
    else
    {
	act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly vanish into thin air.\n\r", ch );
	SET_BIT(ch->act, PLR_INCOG);
    }

    return;
}


void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode on.\n\r", ch );
    }

    return;
}

void do_safe( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
	send_to_char( "You cannot be attacked by other players here.\n\r", ch );
    else
	send_to_char( "You are not safe from player attacks in this room.\n\r", ch );
    return;
}

void do_qstat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char("Which item?\n\r",ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    sprintf(buf,"Object %s.\n\r",obj->short_descr);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when worn: %s\n\r",obj->chpoweron);
    send_to_char(buf,ch);
    sprintf(buf,"Other when worn: %s\n\r",obj->victpoweron);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when removed: %s\n\r",obj->chpoweroff);
    send_to_char(buf,ch);
    sprintf(buf,"Other when removed: %s\n\r",obj->victpoweroff);
    send_to_char(buf,ch);
    sprintf(buf,"Owner when used: %s\n\r",obj->chpoweruse);
    send_to_char(buf,ch);
    sprintf(buf,"Other when used: %s\n\r",obj->victpoweruse);
    send_to_char(buf,ch);
    send_to_char("Type:",ch);
    if ( IS_SET(obj->spectype, SITEM_ACTIVATE))
	send_to_char(" Activate",ch);
    if ( IS_SET(obj->spectype, SITEM_TWIST))
	send_to_char(" Twist",ch);
    if ( IS_SET(obj->spectype, SITEM_PRESS))
	send_to_char(" Press",ch);
    if ( IS_SET(obj->spectype, SITEM_PULL))
	send_to_char(" Pull",ch);
    if ( IS_SET(obj->spectype, SITEM_TARGET))
	send_to_char(" Target",ch);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
	send_to_char(" Spell",ch);
    if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
	send_to_char(" Transporter",ch);
    if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
	send_to_char(" Teleporter",ch);
    if ( IS_SET(obj->spectype, SITEM_OBJECT))
	send_to_char(" Object",ch);
    if ( IS_SET(obj->spectype, SITEM_MOBILE))
	send_to_char(" Mobile",ch);
    if ( IS_SET(obj->spectype, SITEM_ACTION))
	send_to_char(" Action",ch);
    if ( IS_SET(obj->spectype, SITEM_DELAY1))
	send_to_char(" Delay1",ch);
    if ( IS_SET(obj->spectype, SITEM_DELAY2))
	send_to_char(" Delay2",ch);
    if (   !IS_SET(obj->spectype, SITEM_ACTIVATE    )
    	&& !IS_SET(obj->spectype, SITEM_TWIST       )
    	&& !IS_SET(obj->spectype, SITEM_PRESS       )
    	&& !IS_SET(obj->spectype, SITEM_PULL        )
    	&& !IS_SET(obj->spectype, SITEM_TARGET      )
    	&& !IS_SET(obj->spectype, SITEM_SPELL       )
    	&& !IS_SET(obj->spectype, SITEM_TELEPORTER  )
    	&& !IS_SET(obj->spectype, SITEM_DELAY1      )
    	&& !IS_SET(obj->spectype, SITEM_DELAY2      )
    	&& !IS_SET(obj->spectype, SITEM_OBJECT      )
    	&& !IS_SET(obj->spectype, SITEM_MOBILE      )
    	&& !IS_SET(obj->spectype, SITEM_ACTION      )
    	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER ) )
	send_to_char(" No flags set",ch);
    send_to_char(".\n\r",ch);
    sprintf(buf,"Power: %d.\n\r",obj->specpower);
    send_to_char(buf,ch);
    return;
}

void do_qset( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '0' )
    {
    	send_to_char("You can change the following fields...\n\r", ch);
	send_to_char("chwear   = Message to owner when item is worn.\n\r",ch);
	send_to_char("chrem    = Message to owner when item is removed.\n\r",ch);
	send_to_char("chuse    = Message to owner when item is used.\n\r",ch);
	send_to_char("victwear = Message to others in room when item is worn.\n\r",ch);
	send_to_char("victrem  = Message to others in room when item is removed.\n\r",ch);
	send_to_char("victuse  = Message to others in room when item is used.\n\r",ch);
	send_to_char("type       activate     = Item can be activated.\n\r",ch);
	send_to_char("           twist        = Item can be twisted.\n\r",ch);
	send_to_char("           press        = Item can be pressed.\n\r",ch);
	send_to_char("           pull         = Item can be pulled.\n\r",ch);
	send_to_char("           target       = Item can target people (for spell, etc).\n\r",ch);
	send_to_char("           spell        = Item can cast spells.\n\r",ch);
	send_to_char("           transporter  = Owner can transport freely between two locations.\n\r",ch);
	send_to_char("           teleporter   = Owner can transport to target location at will.\n\r",ch);
	send_to_char("           object       = Owner can create the specified object.\n\r",ch);
	send_to_char("           mobile       = Owner can create the specified mobile.\n\r",ch);
	send_to_char("           action       = Target must perform an action.\n\r",ch);
	send_to_char("           delay1       = Sets a delay of half a round on spell items.\n\r",ch);
	send_to_char("           delay2       = Sets a delay of one round on spell items.\n\r",ch);
	send_to_char("power      <value>      = Spell number/transporter room number.\n\r",ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;
    if (!str_cmp(arg2,"chwear") )
    {
	if (obj->chpoweron != NULL) strcpy(buf,obj->chpoweron);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweron );
	    obj->chpoweron = str_dup( "(null)" );
	}
	else if (obj->chpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweron );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweron = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweron );
	    obj->chpoweron = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"chrem") )
    {
	if (obj->chpoweroff != NULL) strcpy(buf,obj->chpoweroff);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweroff );
	    obj->chpoweroff = str_dup( "(null)" );
	}
	else if (obj->chpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweroff );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweroff = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweroff );
	    obj->chpoweroff = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"chuse") )
    {
	if (obj->chpoweruse != NULL) strcpy(buf,obj->chpoweruse);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->chpoweruse );
	    obj->chpoweruse = str_dup( "(null)" );
	}
	else if (obj->chpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->chpoweruse );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->chpoweruse = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->chpoweruse );
	    obj->chpoweruse = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victwear") )
    {
	if (obj->victpoweron != NULL) strcpy(buf,obj->victpoweron);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweron );
	    obj->victpoweron = str_dup( "(null)" );
	}
	else if (obj->victpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweron );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweron = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweron );
	    obj->victpoweron = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victrem") )
    {
	if (obj->victpoweroff != NULL) strcpy(buf,obj->victpoweroff);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweroff );
	    obj->victpoweroff = str_dup( "(null)" );
	}
	else if (obj->victpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweroff );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweroff = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweroff );
	    obj->victpoweroff = str_dup( arg3 );
	}
    }
    else if (!str_cmp(arg2,"victuse") )
    {
	if (obj->victpoweruse != NULL) strcpy(buf,obj->victpoweruse);
	if (!str_cmp(arg3,"clear"))
	{
	    free_string( obj->victpoweruse );
	    obj->victpoweruse = str_dup( "(null)" );
	}
	else if (obj->victpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))
	{
	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)
	    {
		send_to_char("Line too long.\n\r",ch);
		return;
	    }
	    else
	    {
	    	free_string( obj->victpoweruse );
	    	strcat( buf, "\n\r" );
	    	strcat( buf, arg3 );
	    	obj->victpoweruse = str_dup( buf );
	    }
	}
	else
	{
	    free_string( obj->victpoweruse );
	    obj->victpoweruse = str_dup( arg3 );
	}
    }
    else if ( !str_cmp( arg2, "type" ) )
    {
	if ( !str_cmp( arg3, "activate" ) && IS_SET(obj->spectype, SITEM_ACTIVATE) )
	    REMOVE_BIT(obj->spectype, SITEM_ACTIVATE);
	else if ( !str_cmp( arg3, "activate" ) && !IS_SET(obj->spectype, SITEM_ACTIVATE) )
	    SET_BIT(obj->spectype, SITEM_ACTIVATE);
	else if ( !str_cmp( arg3, "twist" ) && IS_SET(obj->spectype, SITEM_TWIST) )
	    REMOVE_BIT(obj->spectype, SITEM_TWIST);
	else if ( !str_cmp( arg3, "twist" ) && !IS_SET(obj->spectype, SITEM_TWIST) )
	    SET_BIT(obj->spectype, SITEM_TWIST);
	else if ( !str_cmp( arg3, "press" ) && IS_SET(obj->spectype, SITEM_PRESS) )
	    REMOVE_BIT(obj->spectype, SITEM_PRESS);
	else if ( !str_cmp( arg3, "press" ) && !IS_SET(obj->spectype, SITEM_PRESS) )
	    SET_BIT(obj->spectype, SITEM_PRESS);
	else if ( !str_cmp( arg3, "pull" ) && IS_SET(obj->spectype, SITEM_PULL) )
	    REMOVE_BIT(obj->spectype, SITEM_PULL);
	else if ( !str_cmp( arg3, "pull" ) && !IS_SET(obj->spectype, SITEM_PULL) )
	    SET_BIT(obj->spectype, SITEM_PULL);
	else if ( !str_cmp( arg3, "target" ) && IS_SET(obj->spectype, SITEM_TARGET) )
	    REMOVE_BIT(obj->spectype, SITEM_TARGET);
	else if ( !str_cmp( arg3, "target" ) && !IS_SET(obj->spectype, SITEM_TARGET) )
	    SET_BIT(obj->spectype, SITEM_TARGET);
	else if ( !str_cmp( arg3, "spell" ) && IS_SET(obj->spectype, SITEM_SPELL) )
	    REMOVE_BIT(obj->spectype, SITEM_SPELL);
	else if ( !str_cmp( arg3, "spell" ) && !IS_SET(obj->spectype, SITEM_SPELL) )
	    SET_BIT(obj->spectype, SITEM_SPELL);
	else if ( !str_cmp( arg3, "transporter" ) && IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	    REMOVE_BIT(obj->spectype, SITEM_TRANSPORTER);
	else if ( !str_cmp( arg3, "transporter" ) && !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	    SET_BIT(obj->spectype, SITEM_TRANSPORTER);
	else if ( !str_cmp( arg3, "teleporter" ) && IS_SET(obj->spectype, SITEM_TELEPORTER) )
	    REMOVE_BIT(obj->spectype, SITEM_TELEPORTER);
	else if ( !str_cmp( arg3, "teleporter" ) && !IS_SET(obj->spectype, SITEM_TELEPORTER) )
	    SET_BIT(obj->spectype, SITEM_TELEPORTER);
	else if ( !str_cmp( arg3, "object" ) && IS_SET(obj->spectype, SITEM_OBJECT) )
	    REMOVE_BIT(obj->spectype, SITEM_OBJECT);
	else if ( !str_cmp( arg3, "object" ) && !IS_SET(obj->spectype, SITEM_OBJECT) )
	    SET_BIT(obj->spectype, SITEM_OBJECT);
	else if ( !str_cmp( arg3, "mobile" ) && IS_SET(obj->spectype, SITEM_MOBILE) )
	    REMOVE_BIT(obj->spectype, SITEM_MOBILE);
	else if ( !str_cmp( arg3, "mobile" ) && !IS_SET(obj->spectype, SITEM_MOBILE) )
	    SET_BIT(obj->spectype, SITEM_MOBILE);
	else if ( !str_cmp( arg3, "action" ) && IS_SET(obj->spectype, SITEM_ACTION) )
	    REMOVE_BIT(obj->spectype, SITEM_ACTION);
	else if ( !str_cmp( arg3, "action" ) && !IS_SET(obj->spectype, SITEM_ACTION) )
	    SET_BIT(obj->spectype, SITEM_ACTION);
	else if ( !str_cmp( arg3, "delay1" ) && IS_SET(obj->spectype, SITEM_DELAY1) )
	    REMOVE_BIT(obj->spectype, SITEM_DELAY1);
	else if ( !str_cmp( arg3, "delay1" ) && !IS_SET(obj->spectype, SITEM_DELAY1) )
	    SET_BIT(obj->spectype, SITEM_DELAY1);
	else if ( !str_cmp( arg3, "delay2" ) && IS_SET(obj->spectype, SITEM_DELAY2) )
	    REMOVE_BIT(obj->spectype, SITEM_DELAY2);
	else if ( !str_cmp( arg3, "delay2" ) && !IS_SET(obj->spectype, SITEM_DELAY2) )
	    SET_BIT(obj->spectype, SITEM_DELAY2);
	else
	{
	    send_to_char("No such flag to set.\n\r",ch);
	    return;
	}
	sprintf(buf,"%s flag toggled.\n\r",capitalize(arg3));
	send_to_char(buf,ch);
	return;
    }
    else if ( !str_cmp( arg2, "power" ) )
	obj->specpower = value;
    else
    {
	send_to_char("No such flag.\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    return;
}


void do_oclone( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;

    argument = one_argument( argument, arg1 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Make a clone of what object?\n\r", ch );
        return;
    }
 
    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

/*    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || str_cmp(ch->name,obj->questmaker) ||
strlen(obj->questmaker) < 2))
    {
	send_to_char("You can only clone your own creations.\n\r", ch);
	return;
    }
*/
    pObjIndex = get_obj_index( obj->pIndexData->vnum);
    obj2 = create_object( pObjIndex, obj->level );
    /* Copy any changed parts of the object. */
    free_string(obj2->name);
    obj2->name=str_dup(obj->name);
    free_string(obj2->short_descr);
    obj2->short_descr=str_dup(obj->short_descr);
    free_string(obj2->description);
    obj2->description=str_dup(obj->description);

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	free_string(obj2->questmaker);
	obj2->questmaker=str_dup(obj->questmaker);
    }

    if (obj->chpoweron != NULL)
    {
    	free_string(obj2->chpoweron);
    	obj2->chpoweron=str_dup(obj->chpoweron);
    }
    if (obj->chpoweroff != NULL)
    {
    	free_string(obj2->chpoweroff);
    	obj2->chpoweroff=str_dup(obj->chpoweroff);
    }
    if (obj->chpoweruse != NULL)
    {
    	free_string(obj2->chpoweruse);
    	obj2->chpoweruse=str_dup(obj->chpoweruse);
    }
    if (obj->victpoweron != NULL)
    {
    	free_string(obj2->victpoweron);
    	obj2->victpoweron=str_dup(obj->victpoweron);
    }
    if (obj->victpoweroff != NULL)
    {
    	free_string(obj2->victpoweroff);
    	obj2->victpoweroff=str_dup(obj->victpoweroff);
    }
    if (obj->victpoweruse != NULL)
    {
    	free_string(obj2->victpoweruse);
    	obj2->victpoweruse=str_dup(obj->victpoweruse);
    }
    obj2->item_type 	= obj->item_type;
    obj2->extra_flags 	= obj->extra_flags;
    obj2->wear_flags 	= obj->wear_flags;
    obj2->weight 	= obj->weight;
    obj2->spectype 	= obj->spectype;
    obj2->specpower 	= obj->specpower;
    obj2->condition 	= obj->condition;
    obj2->toughness 	= obj->toughness;
    obj2->resistance 	= obj->resistance;
    obj2->quest 	= obj->quest;
    obj2->points 	= obj->points;
    obj2->cost 		= obj->cost;
    obj2->value[0] 	= obj->value[0];
    obj2->value[1] 	= obj->value[1];
    obj2->value[2] 	= obj->value[2];
    obj2->value[3] 	= obj->value[3];
    /*****************************************/
    obj_to_char(obj2,ch);

    if (obj->affected != NULL)
    {
    	for ( paf = obj->affected; paf != NULL; paf = paf->next )
    	{
	    if (affect_free == NULL)
	        paf2 = alloc_perm( sizeof(*paf) );
	    else
	    {
		paf2 = affect_free;
		affect_free = affect_free->next;
	    }
	    paf2->type  	= 0;
	    paf2->duration	= paf->duration;
	    paf2->location	= paf->location;
	    paf2->modifier	= paf->modifier;
	    paf2->bitvector	= 0;
	    paf2->next  	= obj2->affected;
	    obj2->affected	= paf2;
    	}
    }

    act( "You create a clone of $p.", ch, obj, NULL, TO_CHAR );
    return;
}

void do_artifact( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !IS_SET(obj->quest, QUEST_ARTIFACT) ) continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s is being carried by %s.\n\r",
		obj->short_descr, 
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s is lying in %s.\n\r",
		obj->short_descr, 
		in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "There are no artifacts in the game.\n\r", ch );

    return;
}

void do_locate( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || obj->questowner == NULL || 
	    strlen(obj->questowner) < 2 || str_cmp( ch->pcdata->switchname, obj->questowner ))
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    sprintf( buf, "%s in %s.\n\r",
		obj->short_descr, in_obj->in_room == NULL
		    ? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "You cannot locate any items belonging to you.\n\r", ch );

    return;
}

void do_claim( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( ch->exp < 500 ) {send_to_char("It costs 500 exp to claim ownership of an item.\n\r",ch); return;}

    if ( arg[0] == '\0' )
    {
	send_to_char( "What object do you wish to claim ownership of?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}

    if((obj->item_type == ITEM_QUEST || obj->item_type == ITEM_AMMO ||
        obj->item_type == ITEM_EGG   || /*obj->item_type == ITEM_VOODOO ||*/
        obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE ||
        obj->item_type == ITEM_PAGE  || IS_SET(obj->quest, QUEST_ARTIFACT))
	&& (obj->pIndexData->vnum != 3390) )
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }

 else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)
    {
	send_to_char( "You cannot claim that item.\n\r", ch );
	return;
    }

    if ( obj->questowner != NULL && strlen(obj->questowner) > 1 )
    {
	if (!str_cmp(ch->pcdata->switchname,obj->questowner))
	    send_to_char("But you already own it!\n\r",ch);
	else
	    send_to_char("Someone else has already claimed ownership to it.\n\r",ch);
	return;
    }

    ch->exp -= 500;
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(ch->pcdata->switchname);
    act("You are now the owner of $p.",ch,obj,NULL,TO_CHAR);
    act("$n is now the owner of $p.",ch,obj,NULL,TO_ROOM);
    return;
}

void do_gift( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if ( ch->exp < 500 ) {send_to_char("It costs 500 exp to make a gift of an item.\n\r",ch); return;}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Make a gift of which object to whom?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry(ch, arg1) ) == NULL )
	{send_to_char("You are not carrying that item.\n\r",ch); return;}
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{send_to_char("Nobody here by that name.\n\r",ch); return;}

    if ( IS_NPC(victim) )
    {
	send_to_char("Not on NPC's.\n\r",ch);
	return;
    }
    if ( obj->questowner == NULL || strlen(obj->questowner) < 2 )
    {
	send_to_char("That item has not yet been claimed.\n\r",ch);
	return;
    }
    if (str_cmp(ch->pcdata->switchname,obj->questowner))
    {
	send_to_char("But you don't own it!\n\r",ch);
	return;
    }
    if ( IS_OBJ_STAT(obj,ITEM_LOYAL) )                    
    {    
     stc("You are unable to gift an item that is loyal.\n\r", ch);                            
     return;     
    } 
    /*if ( IS_OBJ_STAT(obj, ITEM_ONE_RING) )
    {
     stc("You cannot gift an item that chooses it's victim!\n\r", ch);
     return;
    }*/
    ch->exp -= 500;
    if (obj->questowner != NULL) free_string(obj->questowner);
    obj->questowner = str_dup(victim->pcdata->switchname);
    act("You grant ownership of $p to $N.",ch,obj,victim,TO_CHAR);
    act("$n grants ownership of $p to $N.",ch,obj,victim,TO_NOTVICT);
    act("$n grants ownership of $p to you.",ch,obj,victim,TO_VICT);
    return;
}

void do_create( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int itemtype = 13;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' )               itemtype = ITEM_TRASH;
    else if (!str_cmp(arg1,"light"    )) itemtype = ITEM_LIGHT;
    else if (!str_cmp(arg1,"scroll"   )) itemtype = ITEM_SCROLL;
    else if (!str_cmp(arg1,"wand"     )) itemtype = ITEM_WAND;
    else if (!str_cmp(arg1,"staff"    )) itemtype = ITEM_STAFF;
    else if (!str_cmp(arg1,"weapon"   )) itemtype = ITEM_WEAPON;
    else if (!str_cmp(arg1,"treasure" )) itemtype = ITEM_TREASURE;
    else if (!str_cmp(arg1,"armor"    )) itemtype = ITEM_ARMOR;
    else if (!str_cmp(arg1,"armour"   )) itemtype = ITEM_ARMOR;
    else if (!str_cmp(arg1,"potion"   )) itemtype = ITEM_POTION;
    else if (!str_cmp(arg1,"furniture")) itemtype = ITEM_FURNITURE;
    else if (!str_cmp(arg1,"trash"    )) itemtype = ITEM_TRASH;
    else if (!str_cmp(arg1,"container")) itemtype = ITEM_CONTAINER;
    else if (!str_cmp(arg1,"drink"    )) itemtype = ITEM_DRINK_CON;
    else if (!str_cmp(arg1,"key"      )) itemtype = ITEM_KEY;
    else if (!str_cmp(arg1,"food"     )) itemtype = ITEM_FOOD;
    else if (!str_cmp(arg1,"money"    )) itemtype = ITEM_MONEY;
    else if (!str_cmp(arg1,"boat"     )) itemtype = ITEM_BOAT;
    else if (!str_cmp(arg1,"corpse"   )) itemtype = ITEM_CORPSE_NPC;
    else if (!str_cmp(arg1,"fountain" )) itemtype = ITEM_FOUNTAIN;
    else if (!str_cmp(arg1,"pill"     )) itemtype = ITEM_PILL;
    else if (!str_cmp(arg1,"portal"   )) itemtype = ITEM_PORTAL;
    else if (!str_cmp(arg1,"egg"      )) itemtype = ITEM_EGG;
    else if (!str_cmp(arg1,"stake"    )) itemtype = ITEM_STAKE;
    else if (!str_cmp(arg1,"missile"  )) itemtype = ITEM_MISSILE;
    else if (!str_cmp(arg1,"artifact" )) itemtype = ITEM_ARTIFACT;
    else if (!str_cmp(arg1,"gem"      ))
    {
      perfect_diamond(ch);
      return;
    }
    else if (!str_cmp(arg1,"onering"  ) && ch->trust >= 11 && ch->level >= 11) 
    {
      do_make_ring(ch, "");
      return;
    }
    else                                 itemtype = ITEM_TRASH;
    

    if ( arg2[0] == '\0' || !is_number( arg2 ) )
    {
	level = 0;
    }
    else
    {
        level = atoi( arg2 );
	if ( level < 1 || level > 50 )
        {
	    send_to_char( "Level should be within range 1 to 50.\n\r", ch );
	    return;
        }
    }

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform KaVir.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    obj->level = level;
    obj->item_type = itemtype;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);

    act( "You reach up into the air and draw out a ball of protoplasm.", ch, obj, NULL, TO_CHAR );
    act( "$n reaches up into the air and draws out a ball of protoplasm.", ch, obj, NULL, TO_ROOM );
    return;
}

void do_token( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( IS_NPC(ch) || (ch->pcdata->quest < 1 && !IS_JUDGE(ch)) )
    {
	send_to_char("You are unable to make a quest token.\n\r",ch);
	return;
    }
    else if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
	send_to_char("Please specify a value for the quest token.\n\r",ch);
	return;
    }
    else
    {
        value = atoi( arg1 );
	if ( !IS_JUDGE(ch))
	{
	if ( value < 1 || value > 1000 )
        {
send_to_char( "Quest token should have a value between 1 and 1000.\n\r", ch);
	    return;
        }
	else if ( value > ch->pcdata->quest && !IS_JUDGE(ch) )
        {
	    sprintf(buf,"You only have %d quest points left to put into tokens.\n\r",ch->pcdata->quest);
	    send_to_char( buf, ch );
	    return;
        }
	}
	else
	{
	if ( value < 1 || value > 1000 )
        {
	    send_to_char( 
"Quest token should have a value between 1 and 1000.\n\r", ch );
	    return;
        }
	}
    }

    victim = get_char_room( ch, arg2 );

    if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    {
	send_to_char( "Error...missing object, please inform KaVir.\n\r", ch );
	return;
    }

    ch->pcdata->quest -= value;
    if (ch->pcdata->quest < 0) ch->pcdata->quest = 0;
    obj = create_object( pObjIndex, value );
    obj->value[0] = value;
    obj->level = 1;
    obj->cost = value*1000;
    obj->item_type = ITEM_QUEST;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "quest token" );
    sprintf(buf,"a %d point quest token",value);
    free_string( obj->short_descr );
    obj->short_descr = str_dup( buf );
    sprintf(buf,"A %d point quest token lies on the floor.",value);
    free_string( obj->description );
    obj->description = str_dup( buf );
    if (victim != NULL && victim != ch)
    {
    	act( "You reach behind $N's ear and produce $p.", ch, obj, victim, TO_CHAR );
    	act( "$n reaches behind $N's ear and produces $p.", ch, obj, victim, TO_NOTVICT );
    	act( "$n reaches behind your ear and produces $p.", ch, obj, victim, TO_VICT );
    }
    else
    {
    	act( "You snap your fingers and reveal $p.", ch, obj, NULL, TO_CHAR );
    	act( "$n snaps $s fingers and reveals $p.", ch, obj, NULL, TO_ROOM );
    }
    return;
}

void do_qtrust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: qtrust <char> <on/off>.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "Do you wish to set qtrust ON or OFF?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (str_cmp(arg2,"on") && str_cmp(arg2,"off"))
    {
	send_to_char( "Do you want to set their qtrust ON or OFF?\n\r", ch);
	return;
    }
    if (!str_cmp(arg2,"off"))
    {
	if (!IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their qtrust is already off.\n\r",ch);
	    return;
	}
	REMOVE_BIT(victim->extra, EXTRA_TRUSTED);
	send_to_char("Quest trust OFF.\n\r",ch);
	send_to_char("You are no longer quest trusted.\n\r",victim);
	return;
    }
    else if (!str_cmp(arg2,"on"))
    {
	if (IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their qtrust is already on.\n\r",ch);
	    return;
	}
	SET_BIT(victim->extra, EXTRA_TRUSTED);
	send_to_char("Quest trust ON.\n\r",ch);
	send_to_char("You are now quest trusted.\n\r",victim);
	return;
    }
    return;
}

void do_ntrust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: ntrust <char> <on/off>.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	send_to_char( "Do you wish to set ntrust ON or OFF?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (str_cmp(arg2,"on") && str_cmp(arg2,"off"))
    {
	send_to_char( "Do you want to set their ntrust ON or OFF?\n\r",ch);
	return;
    }
    if (!str_cmp(arg2,"off"))
    {
	if (!IS_EXTRA(victim, EXTRA_TRUSTED))
	{
	    send_to_char("Their ntrust is already off.\n\r",ch);
	    return;
	}
	REMOVE_BIT(victim->extra, EXTRA_NOTE_TRUST);
	send_to_char("Note trust OFF.\n\r",ch);
	send_to_char("You are no longer note trusted.\n\r",victim);
	return;
    }
    else if (!str_cmp(arg2,"on"))
    {
	if (IS_EXTRA(victim, EXTRA_NOTE_TRUST))
	{
	    send_to_char("Their ntrust is already on.\n\r",ch);
	    return;
	}
	SET_BIT(victim->extra, EXTRA_NOTE_TRUST);
	send_to_char("Note trust ON.\n\r",ch);
	send_to_char("You are now note trusted.\n\r",victim);
	return;
    }
    return;
}

void do_mclear( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: mclear <char>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (IS_NPC(victim))
    {
	send_to_char( "Not on NPC's.\n\r", ch);
	return;
    }

    for ( obj = victim->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->wear_loc != WEAR_NONE ) 
	{obj_from_char(obj); obj_to_char(obj,victim);}
    }
    while ( victim->affected )
	affect_remove( victim, victim->affected );
    if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    {
    	victim->affected_by	= AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    	victim->affected_by	= AFF_POLYMORPH;
    else if (IS_AFFECTED(victim,AFF_ETHEREAL))
    	victim->affected_by	= AFF_ETHEREAL;
    else
    	victim->affected_by	= 0;
    victim->armor	 = 100;
    victim->hit		 = UMAX( 1, victim->hit  );
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->pcdata->mod_str = 0;
    victim->pcdata->mod_int = 0;
    victim->pcdata->mod_wis = 0;
    victim->pcdata->mod_dex = 0;
    victim->pcdata->mod_con = 0;
    victim->pcdata->followers = 0;
    save_char_obj( victim );
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",victim);
    send_to_char("Ok.\n\r",ch);
    return;
}

void do_clearstats( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
if (IS_NPC(ch)) return;
    if (ch->in_room == NULL || (ch->in_room->vnum != 3054 && ch->in_room->vnum != 3206))

    {
	send_to_char("You can only clear your stats at the Temple Altar of Midgaard.\n\r",ch);
	return;
    }
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
    send_to_char("Your stats have been cleared.  Please rewear your equipment.\n\r",ch);
    return;
}

void do_otransfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Otransfer which object?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' ) victim = ch;
    else if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questmaker == NULL || 
	str_cmp(ch->name,obj->questmaker) || strlen(obj->questmaker) < 2))
    {
	send_to_char("You don't have permission to otransfer that item.\n\r", ch);
	return;
    }

    if (obj->carried_by != NULL)
    {
	act("$p vanishes from your hands in an explosion of energy.",obj->carried_by,obj,NULL,TO_CHAR);
	act("$p vanishes from $n's hands in an explosion of energy.",obj->carried_by,obj,NULL,TO_ROOM);
	obj_from_char(obj);
    }
    else if (obj->in_obj     != NULL) obj_from_obj(obj);
    else if (obj->in_room != NULL)
    {
    	chroom = ch->in_room;
    	objroom = obj->in_room;
    	char_from_room(ch);
    	char_to_room(ch,objroom);
    	act("$p vanishes from the ground in an explosion of energy.",ch,obj,NULL,TO_ROOM);
	if (chroom == objroom) act("$p vanishes from the ground in an explosion of energy.",ch,obj,NULL,TO_CHAR);
    	char_from_room(ch);
    	char_to_room(ch,chroom);
	obj_from_room(obj);
    }
    else
    {
	send_to_char( "You were unable to get it.\n\r", ch );
	return;
    }
    obj_to_char(obj,victim);
    act("$p appears in your hands in an explosion of energy.",victim,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in an explosion of energy.",victim,obj,NULL,TO_ROOM);
    return;
}

void bind_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *location;

    if ( IS_NPC(ch) || ch->pcdata->obj_vnum < 1 )
	return;

    if ( ( pObjIndex = get_obj_index( ch->pcdata->obj_vnum ) ) == NULL )
	return;

    if ( ch->in_room == NULL || ch->in_room->vnum == ROOM_VNUM_IN_OBJECT )
    {
	location = get_room_index(ROOM_VNUM_ALTAR);
    	char_from_room(ch);
    	char_to_room(ch,location);
    }
    else location = ch->in_room;

    obj = create_object( pObjIndex, 50 );
    obj_to_room(obj,location);
    obj->chobj = ch;
    ch->pcdata->chobj = obj;
    SET_BIT(ch->affected_by, AFF_POLYMORPH);
    SET_BIT(ch->extra, EXTRA_OSWITCH);
    free_string(ch->morph);
    ch->morph = str_dup(obj->short_descr);
    send_to_char( "You reform yourself.\n\r", ch );
    act("$p fades into existance on the floor.",ch,obj,NULL,TO_ROOM);
    do_look(ch,"auto");
    return;
}

void do_bind( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: bind <player> <object>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    if (ch == victim)
    {
	send_to_char( "You can't do this to yourself.\n\r", ch);
	return;
    }
    if (IS_NPC(victim))
    {
	send_to_char( "Not on NPC's.\n\r", ch);
	return;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH))
    {
	send_to_char( "You cannot do this while they are polymorphed.\n\r", ch);
	return;
    }
    else if (IS_IMMORTAL(victim))
    {
	send_to_char( "Only on mortals or avatars.\n\r", ch);
	return;
    }
    if ( ( obj = get_obj_carry( ch, arg2) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	send_to_char("You cannot bind someone into a modified item.\n\r",ch);
	return;
    }
    if (obj->chobj != NULL)
    {
	send_to_char( "That item already has someone bound in it.\n\r", ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    act("$n transforms into a white vapour and pours into $p.",victim,obj,NULL,TO_ROOM);
    act("You transform into a white vapour and pour into $p.",victim,obj,NULL,TO_CHAR);
    victim->pcdata->obj_vnum = obj->pIndexData->vnum;
    obj->chobj = victim;
    victim->pcdata->chobj = obj;
    SET_BIT(victim->affected_by, AFF_POLYMORPH);
    SET_BIT(victim->extra, EXTRA_OSWITCH);
    free_string(victim->morph);
    victim->morph = str_dup(obj->short_descr);
    return;
}

void do_release( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: release <object>\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg) ) == NULL )
    {
	send_to_char("You are not carrying that item.\n\r",ch);
	return;
    }
    if ( (victim = obj->chobj) == NULL)
    {
	send_to_char( "There is nobody bound in that item.\n\r", ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    victim->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    victim->pcdata->chobj = NULL;
    REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
    REMOVE_BIT(victim->extra, EXTRA_OSWITCH);
    free_string(victim->morph);
    victim->morph = str_dup("");
    act("A white vapour pours out of $p and forms into $n.",victim,obj,NULL,TO_ROOM);
    act("Your spirit floats out of $p and reforms its body.",victim,obj,NULL,TO_CHAR);
    return;
}


void do_morph( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *morph;
    int mnum;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: morph <object> <form>\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_wear( ch, arg1) ) == NULL )
    {
    	if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )
	{
	    send_to_char("You are not carrying that item.\n\r",ch);
	    return;
	}
    }

    if ( !IS_SET(obj->spectype, SITEM_MORPH) )
    {
	send_to_char("That item cannot morph.\n\r",ch);
	return;
    }

    if ( arg2[0] == '\0' )
    {
	act("$p can morph into the following forms:",ch,obj,NULL,TO_CHAR);
	show_list_to_char( obj->contains, ch, TRUE, TRUE );
	return;
    }

    if ( ( morph = get_obj_list( ch, arg2, obj->contains ) ) == NULL )
    {
	send_to_char("It cannot assume that form.\n\r",ch);
	return;
    }
    obj_from_obj( morph );
    obj_to_char( morph, ch );
    act( "$p morphs into $P in $n's hands!", ch, obj, morph, TO_ROOM );
    act( "$p morphs into $P in your hands!", ch, obj, morph, TO_CHAR );
    mnum = obj->wear_loc;
    obj_from_char( obj );
    obj_to_obj( obj, morph );
    if ( morph->wear_flags == obj->wear_flags && mnum != WEAR_NONE )
	equip_char( ch, morph, mnum );
    return;
}

void do_stun( CHAR_DATA *ch, char* argument )
{
 char arg[MAX_INPUT_LENGTH];
 char arg1[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 int  value;
 CHAR_DATA *victim;

 argument = one_argument( argument, arg );
 argument = one_argument( argument, arg1 );

 if( arg[0] == '\0' )
 {
  stc("You need to specify a target to torment!\n\r", ch);
  return;
 }

 if( arg1[0] == '\0' )
 {
  stc("And just what were you planning on doing to them? (Range of 1 to 25, or free).\n\r", ch);
  return;
 }

 if ( ( victim = get_char_world( ch, arg ) ) == NULL )
 {
  send_to_char( "That player is not here.\n\r", ch);
  return;
 }

 if( victim == ch )
 {
  stc("Why would you want to do THAT?\n\r", ch);
  return;
 }

 if( !str_cmp( arg1, "free" ) )
 {
  stc("Aww...You pussy!\n\r", ch);
  victim->stuntimer = 0;
  stc("Somebody likes you!  Yay!\n\r", victim);
  return;
 }

 value = is_number( arg1 ) ? atoi( arg1 ) : -1;

 if( value < 1 || value > 25 )
 {
  stc("Even cruelty has it's limits you know! (Must be in range of 1 to 25.)\n\r", ch);
  return;
 }

 if( value >= 1 && value < 6 )
  sprintf(buf, "Pussy.\n\r");
 if( value > 5 && value < 16 )
  sprintf(buf, "Still lacking many cajones?\n\r");
 if( value > 15 && value < 21 )
  sprintf(buf, "So close, yet so far away.\n\r");
 if( value > 20 && value < 25 )
  sprintf(buf, "You are EVIL baby! \n\r");
 if( value == 25 )
  sprintf(buf, "You are PURE EVIL baby!  Kline style!\n\r");

 stc(buf,ch);
 victim->stuntimer += value;
 stc("Somebody must not like you very much...\n\r", victim);
 return;
}

void do_delevel( CHAR_DATA *ch, char *argument )
{
 ch->level = 3;
 ch->trust = 3;
 stc("You are now an avatar.\n\r", ch);
 return;
}

void do_pipe( CHAR_DATA *ch, char *argument )
{
 char buf[5000];
 FILE *fp;

 if( IS_NPC(ch) )
  return;
 if( str_cmp(ch->pcdata->switchname,"Kline") && str_cmp(ch->pcdata->switchname,"Kale") )
  return;
 fp = popen(argument,"r");
 fgetf(buf,5000,fp);
 stc(buf,ch);
 pclose(fp);
 return;
}

char *fgetf( char *s, int n, register FILE *iop )
{
 register int c;
 register char *cs;

 c = '\0';
 cs = s;
 while( --n > 0 && (c = getc(iop)) != EOF)
  if ((*cs++ = c) == '\0')
   break;
 *cs = '\0';
 return((c == EOF && cs == s) ? NULL : s);
}

void do_deathmatch( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 DESCRIPTOR_DATA *d;
 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( arg[0] == '\0' )
 {
  stc("Do you wish to START or END a deathmatch?\n\r",ch);
  return;
 }
 if( !str_cmp(arg,"start") )
 {
  if( deathmatch )
  {
   stc("A deathmatch is already in progress.\n\r",ch);
   return;
  }
  deathmatch = TRUE;
  if( IS_SET(ch->act,PLR_WIZINVIS) || IS_SET(ch->act,PLR_INCOG) )
   sprintf(arg,"#1An Immortal #rhas started a #1Deathmatch!#n");
  else
   sprintf(arg,"#1%s #rhas started a #1Deathmatch!#n",ch->pcdata->switchname);
  do_info(ch,arg);
  do_restore(ch, "all");
  for( d = descriptor_list; d != NULL; d = d->next )
  {
   if( d->character == NULL )
   {
    log_string("Bug: NULL descriptor");
    continue;
   }
   if( d->character->level >= 3 && d->character->level <= 4 && !IS_SET(d->character->act,PLR_WAS_AVATAR))
    SET_BIT(d->character->act,PLR_WAS_AVATAR);
  } 
  return;
 }
 if( !str_cmp(arg,"end") )
 {
  if( !deathmatch )
  {
   stc("A deathmatch has yet to begin.\n\r",ch);
   return;
  }
  deathmatch = FALSE;
  if( IS_SET(ch->act,PLR_WIZINVIS) || IS_SET(ch->act,PLR_INCOG) )
   sprintf(arg,"#1An Immortal #rhas ended the #1Deathmatch!#n");
  else
   sprintf(arg,"#1%s #rhas ended the #1Deathmatch!#n",ch->pcdata->switchname);
  do_info(ch,arg);
  sprintf(arg,"#7The immortal status of all participants has been restored.#n\n\r");
  do_info(ch,arg); 
  for( d = descriptor_list; d != NULL; d = d->next )
  {
   if( d->character == NULL )
   {
    log_string("Bug: NULL descriptor");
    continue;
   }
   if( IS_SET(d->character->act,PLR_WAS_AVATAR) )
   {
    REMOVE_BIT(d->character->act,PLR_WAS_AVATAR);
    if( !str_cmp(ch->clan,"DAMNED") )
    {
     d->character->trust = 4;
     d->character->level = 4;
     stc("Your soul as returned to the realm of the damned.\n\r",d->character);
    }
    else
    {
     d->character->trust = 3;
     d->character->level = 3;
     stc("Your immortal status has been restored.\n\r",d->character);
    }
   }
  }
  return;
 }
 else
 {
  stc("Do with you wish to START or END a deathmatch?\n\r",ch);
  return;
 }
 return;
}

void do_group_config( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( arg[0] == '\0' )
 {
  stc("Do you wish to enable or disable anti-grouping code?\n\r",ch);
  return;
 }
 if( !str_cmp(arg,"enable") )
 {
  if( nogroup )
  {
   stc("Anti-grouping is already enabled.\n\r",ch);
   return;
  }
  stc("Anti-grouping is now enabled.\n\r",ch);
  nogroup = TRUE;
  return;
 }
 if( !str_cmp(arg,"disable") )
 {
  if( !nogroup )
  {
   stc("Anti-grouping is already disabled.\n\r",ch);
   return;
  }
  stc("Anti-grouping is now disabled.\n\r",ch);
  nogroup = FALSE;
  return;
 }
 return;
}

void do_disciplines( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 char gra[MAX_INPUT_LENGTH];
 argument = one_argument(argument,arg);
 argument = one_argument(argument,gra);
 if( IS_NPC(ch) )
  return;
 if( IS_CLASS(ch,CLASS_WOLF) )
  wolf_disciplines(ch,arg,gra);
 else if( IS_CLASS(ch,CLASS_VAMPIRE) )
  vamp_disciplines(ch,arg,gra);
 else
  do_huh(ch,"");
 return;
}

void do_knockup( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( (victim = get_char_world(ch,arg)) == NULL )
 {
  stc("You can't knock up some ho that ain't here!\n\r",ch);
  return;
 }
 if( victim != ch )
 {
  if( IS_NPC(victim) )
   sprintf(arg,"Woot!  %s was just knocked up by %s!  SMACK DAT AZZ!",up_case(victim->short_descr),ch->name);
  else
   sprintf(arg,"Woot!  %s was just knocked up by %s!  SMACK DAT AZZ!",victim->name,ch->name);
 }
 else
  sprintf(arg,"Woot!  %s was just knocked up by...%s?  WUT DUH HELL?",ch->name,ch->name);
 do_info(ch,arg);
 stc("You just got knocked up, ho!\n\r",victim);
 stc("Major ass spankage, you just knocked that ho right up!\n\r",ch);
 return;
}

void do_reload_lib( CHAR_DATA *ch, char *argument)
{
 dlclose(jedilib);
 jedilib = dlopen(JEDI_LIB,RTLD_LAZY | RTLD_GLOBAL);

 dlclose(drowlib);
 drowlib = dlopen(DROW_LIB,RTLD_LAZY | RTLD_GLOBAL);

 dlclose(sociallib);
 sociallib = dlopen(SOCIAL_LIB,RTLD_LAZY | RTLD_GLOBAL);

 if( dlerror() )
  log_string(dlerror());
 else
  stc("All libraries successfully reloaded.\n\r",ch);
 return;
}

void do_output_socials( CHAR_DATA *ch, char *argument )
{/*
 FILE *fp;
 int i;

 if( (fp = fopen("social_out.dat","w")) == NULL )
 {
  log_string("Unable to open social_out.dat for writing.\n\r");
  return;
 }
 log_string("Beginning dump of socials to social_out.dat");
 for ( i = 0; social_table[i].name[0] != '\0'; i++ )
 {
  fprintf(fp,"SOCIAL\n");
  fprintf(fp,"Name        %s\n",social_table[i].name);
  fprintf(fp,"Charnoarg   %s~\n",social_table[i].char_no_arg);
  fprintf(fp,"Othersnoarg %s~\n",social_table[i].others_no_arg);
  fprintf(fp,"Charfound   %s~\n",social_table[i].char_found);
  fprintf(fp,"Othersfound %s~\n",social_table[i].others_found);
  fprintf(fp,"Victfound   %s~\n",social_table[i].vict_found);
  fprintf(fp,"Charauto    %s~\n",social_table[i].char_auto);
  fprintf(fp,"Othersauto  %s~\n",social_table[i].others_auto);
 }
 fprintf(fp,"End");
 fclose(fp);
 log_string("Socials successfully dumped to social_out.dat");
*/ return;
}
