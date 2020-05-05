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
#include <time.h>
#include "merc.h"



/*Local functions.*/
bool	is_note_to	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void	note_attach	args( ( CHAR_DATA *ch ) );
void	note_remove	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void	talk_channel	args( ( CHAR_DATA *ch, char *argument,
			    int channel, const char *verb ) );
bool is_in		args( (char *, char *) );
bool all_in		args( (char *, char *) );
char * socialc		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );
char * socialv		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );
char * socialn		args( ( CHAR_DATA *ch, char *argument, char *you, char *them ) );


void do_radio( CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char buf[MAX_STRING_LENGTH];

	if( IS_NPC(ch))
		return;
	if( argument[0] == '\0' )
	{
		stc( "Radio what?\n\r", ch );
		return;
	}
	if( ch->pcdata->radio == -1 )
	{
		stc("But your radi is turned off.\n\r", ch);
		return;
	} 
	for( d = descriptor_list; d; d = d->next )
	{
		if( d->connected != CON_PLAYING )
			continue;
		if( IS_NPC(d->character))
			continue;
		if( d->character->pcdata->radio == -1 )
			continue;
		if( IS_IMMORTAL(d->character))
		{
			sprintf( buf, "#4-=<#7%s#4>=- #n[#7%d#n] '#2%s#n'\n\r", ch->name, ch->pcdata->radio, argument);
			stc( buf, d->character );
			continue;
		}
		if( d->character->pcdata->radio == ch->pcdata->radio)
		{
			sprintf( buf, "#4-=<#7%s#4>=- #n'#2%s#n'\n\r", ch->name, argument);
			stc( buf, d->character);
			continue;
		}
		if( abs(d->character->pcdata->radio - ch->pcdata->radio) <=5)
		{
			stc( "You hear some static on your radio.\n\r", d->character);
			continue;
		}
	}
	return;
}

void do_tune( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg);

	if( IS_NPC(ch) )
         return;

	if( arg[0] == '\0' )
	{
		stc("Syntax tune <off> or tune<1 - 32000>.\n\r", ch);
		return;
	}
	if( !str_cmp( arg, "off"))
	{
		ch->pcdata->radio = -1;
		stc("You turn your radio off.\n\r", ch);
		return;
	}
	if( !is_number(arg))
	{
		stc("You can only tune to a value between 1 and 32000.\n\r", ch);
		return;
	}
	if( atoi( arg ) < 1 || atoi( arg ) > 32000 )
	{
		stc("Value must be between 1 and 32000.\n\r", ch);
		return;
	}
	ch->pcdata->radio = atoi( arg );
	sprintf( arg, "You have tuned your radio to frequency to %d.\n\r", ch->pcdata->radio );
	stc( arg, ch);
	return;
}

void play_sound( CHAR_DATA *ch, char *sound, int type)
{
 char buf[MAX_STRING_LENGTH];
 CHAR_DATA *vch;
 DESCRIPTOR_DATA *d;

 if( (sound == NULL)
  || (ch == NULL))
  return;
 sprintf( buf, "!!SOUND(%s)\n\r", sound);
 switch( type)
 {
  default:
  case SND_CHAR:
   if( HAS_SOUND(ch))
    send_to_char(buf, ch);
   break;
  case SND_ROOM:
   for( vch = ch->in_room->people; vch; vch = vch->next_in_room)
   {
    if( IS_NPC(vch))
     continue;
    if( HAS_SOUND(vch))
     stc(buf, vch);
   }
   break;
  case SND_AREA:
   for( d = descriptor_list; d; d = d->next)
   {
    if( d->connected != CON_PLAYING)
     continue;
    if (d->character == NULL)
     continue;
    vch = d->character;
    if( vch->in_room->area != ch->in_room->area)
     continue;
    if( HAS_SOUND(vch))
     send_to_char(buf, vch);
   }
   break;
 case SND_WORLD:
  for( d = descriptor_list; d; d = d->next)
  {
   if( d->connected != CON_PLAYING)
    continue;
   if( d->character == NULL)
    continue;
   vch = d->character;
   if( HAS_SOUND(vch))
    send_to_char(buf, vch);
  }
  break;
 }
 return;
}

bool is_note_to( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    if( !str_cmp( ch->pcdata->switchname, pnote->sender ) )
	return TRUE;
    if( is_name( "all", pnote->to_list ) )
	return TRUE;
    if( IS_IMMORTAL(ch) && is_name( "imm", pnote->to_list ) )
	return TRUE;
    if( IS_IMMORTAL(ch) && is_name( "immortal", pnote->to_list ) )
	return TRUE;
    if( is_name( ch->pcdata->switchname, pnote->to_list ) )
	return TRUE;
    if( ch->class == CLASS_APOCRYPHIC && is_name( "apocryphic", pnote->to_list) )
	return TRUE;
    if( ch->class == CLASS_ALIEN && is_name( "alien", pnote->to_list) )
	return TRUE;
    if( ch->class == CLASS_CYBORG && is_name( "cyborg", pnote->to_list) )
	return TRUE;
    if( ch->class == CLASS_MONK && is_name( "monk", pnote->to_list) )
	return TRUE;
    if( ch->class == CLASS_SHADOW && is_name( "shadow", pnote->to_list) )
	return TRUE;
    if( ch->class == CLASS_WOLF && is_name( "werewolf", pnote->to_list) )
	return TRUE;
    if( ch->class == CLASS_JEDI && is_name( "jedi", pnote->to_list) )    
        return TRUE;  
    if( ch->class == CLASS_VAMPIRE && is_name( "vampire", pnote->to_list) )    
        return TRUE;  
    if( ch->class == CLASS_DRAGON && is_name( "dragon", pnote->to_list) )    
        return TRUE;  
    if( !str_cmp(ch->clan,pnote->to_list) )
	return TRUE;
    if(ch->level == 12)
	return TRUE;
    return FALSE;
}



void note_attach( CHAR_DATA *ch )
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    if ( note_free == NULL )
    {
	pnote	  = alloc_perm( sizeof(*ch->pnote) );
    }
    else
    {
	pnote	  = note_free;
	note_free = note_free->next;
    }

    pnote->next		= NULL;
    pnote->sender	= str_dup( ch->pcdata->switchname );
    pnote->date		= str_dup( "" );
    pnote->to_list	= str_dup( "" );
    pnote->subject	= str_dup( "" );
    pnote->text		= str_dup( "" );
    ch->pnote		= pnote;
    return;
}



void note_remove( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    char to_new[MAX_INPUT_LENGTH];
    char to_one[MAX_INPUT_LENGTH];
    FILE *fp;
    NOTE_DATA *prev;
    char *to_list;

    /*Build a new to_list.Strip out this recipient. */
    to_new[0]	= '\0';
    to_list	= pnote->to_list;
    while ( *to_list != '\0' )
    {
	to_list	= one_argument( to_list, to_one );
	if ( to_one[0] != '\0' && str_cmp( ch->pcdata->switchname, to_one ) )
	{
	    strcat( to_new, " " );
	    strcat( to_new, to_one );
	}
    }

    /*
     * Just a simple recipient removal?
     */
    if (( str_cmp( ch->pcdata->switchname, pnote->sender ) && to_new[0] != '\0' && (ch->level < 11)))
    {
	free_string( pnote->to_list );
	pnote->to_list = str_dup( to_new + 1 );
        stc("Recipient removal.\n\r",ch);
	return;
    }

    /*
     * Remove note from linked list.
     */
    if ( pnote == note_list )
    {
		note_list = pnote->next;
    }
    else
    {
	for ( prev = note_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == pnote )
		break;
	}

	if ( prev == NULL )
	{
	    bug( "Note_remove: pnote not found.", 0 );
	    return;
	}

	prev->next = pnote->next;
    }

    free_string( pnote->text    );
    free_string( pnote->subject );
    free_string( pnote->to_list );
    free_string( pnote->date    );
    free_string( pnote->sender  );
    pnote->next	= note_free;
    note_free	= pnote;

    /*
     * Rewrite entire list.
     */
    fclose( fpReserve );
    if ( ( fp = fopen( NOTE_FILE, "w" ) ) == NULL )
    {
	perror( NOTE_FILE );
    }
    else
    {
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    fprintf( fp, "Sender  %s~\nDate    %s~\nTo      %s~\nSubject %s~\nText\n%s~\n\n",
		pnote->sender,
		pnote->date,
		pnote->to_list,
		pnote->subject,
		pnote->text
		);
	}
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    stc("Note Removed.\n\r",ch);
    return;
}



void do_note( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    NOTE_DATA *pnote;
    int vnum;
    int anum;

    if ( IS_NPC(ch) )
	return;

    argument = one_argument( argument, arg );
    smash_tilde( argument );

    if ( !str_cmp( arg, "list" ) )
    {
	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) )
	    {
		sprintf( buf, "[%3d] %s: %s\n\r", vnum, pnote->sender, pnote->subject );
		send_to_char( buf, ch );
		vnum++;
	    }
	}
	return;
    }

    if ( !str_cmp( arg, "read" ) )
    {
	bool fAll;

	if ( !str_cmp( argument, "all" ) )
	{
	    fAll = TRUE;
	    anum = 0;
	}
	else if ( is_number( argument ) )
	{
	    fAll = FALSE;
	    anum = atoi( argument );
	}
	else
	{
	    send_to_char( "Note read which number?\n\r", ch );
	    return;
	}

	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && ( vnum++ == anum || fAll ) )
	    {
		sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
		    vnum - 1,
		    pnote->sender,
		    pnote->subject,
		    pnote->date,
		    pnote->to_list
		    );
		send_to_char( buf, ch );
		send_to_char( pnote->text, ch );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "+" ) )
    {
	note_attach( ch );
        smash_color( buf);
	strcpy( buf, ch->pnote->text );
	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 4 )
	{
	    send_to_char( "Note too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->pnote->text );
	ch->pnote->text = str_dup( buf );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "subject" ) )
    {
	note_attach( ch );
	free_string( ch->pnote->subject );
	ch->pnote->subject = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "to" ) )
    {
	note_attach( ch );
	free_string( ch->pnote->to_list );
	ch->pnote->to_list = str_dup( argument );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "clear" ) )
    {
	if ( ch->pnote != NULL )
	{
	    free_string( ch->pnote->text );
	    free_string( ch->pnote->subject );
	    free_string( ch->pnote->to_list );
	    free_string( ch->pnote->date );
	    free_string( ch->pnote->sender );
	    ch->pnote->next	= note_free;
	    note_free		= ch->pnote;
	    ch->pnote		= NULL;
	}

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "show" ) )
    {
	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

	sprintf( buf, "%s: %s\n\rTo: %s\n\r",
	    ch->pnote->sender,
	    ch->pnote->subject,
	    ch->pnote->to_list
	    );
	send_to_char( buf, ch );
	send_to_char( ch->pnote->text, ch );
	return;
    }

    if ( !str_cmp( arg, "post" ) )
    {
	FILE *fp;
	char *strtime;

	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}
/*
	if ( !IS_SET( ch->extra, EXTRA_NOTE_TRUST ) )
        {
		send_to_char("You are not note trusted to post a note.\n\r", ch);
		return;
        }
*/
	ch->pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	ch->pnote->date			= str_dup( strtime );

	if ( note_list == NULL )
	{
	    note_list	= ch->pnote;
	}
	else
	{
	    for ( pnote = note_list; pnote->next != NULL; pnote = pnote->next )
		;
	    pnote->next	= ch->pnote;
	}
	pnote		= ch->pnote;
	ch->pnote	= NULL;

	fclose( fpReserve );
	if ( ( fp = fopen( NOTE_FILE, "a" ) ) == NULL )
	{
	    perror( NOTE_FILE );
	}
	else
	{
	    fprintf( fp, "Sender  %s~\nDate    %s~\nTo      %s~\nSubject %s~\nText\n%s~\n\n",
		pnote->sender,
		pnote->date,
		pnote->to_list,
		pnote->subject,
		pnote->text
		);
	    fclose( fp );
	}
	fpReserve = fopen( NULL_FILE, "r" );

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "remove" ) )
    {
	if ( !is_number( argument ) )
	{
	    send_to_char( "Note remove which number?\n\r", ch );
	    return;
	}

	anum = atoi( argument );
	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && vnum++ == anum )
	    {
		note_remove( ch, pnote );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }

    send_to_char( "Huh?  Type 'help note' for usage.\n\r", ch );
    return;
}



/*
 * Generic channel function.
 */
void talk_channel( CHAR_DATA *ch, char *argument, int channel, const char *verb )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int pos; 
    bool ignored;
    int position;
	
//if (IS_NPC(ch)) return;

    if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
    {
     send_to_char("Something prevents you from speaking in this room.\n\r", ch);
     return;
    }
    if ( argument[0] == '\0' )
    {
	sprintf( buf, "%s what?\n\r", verb );
	buf[0] = UPPER(buf[0]);
	return;
    }

    if ( IS_HEAD(ch, LOST_TONGUE) )
    {
	sprintf( buf, "You can't %s without a tongue!\n\r", verb );
	send_to_char( buf, ch );
	return;
    }
    
    if ( IS_EXTRA(ch, GAGGED) )
    {
	sprintf( buf, "You can't %s with a gag on!\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	sprintf( buf, "You can't %s.\n\r", verb );
	send_to_char( buf, ch );
	return;
    }

    REMOVE_BIT(ch->deaf, channel);

    switch ( channel )
    {
    default:
	sprintf( buf, "You %s '#1%s#n'.\n\r", verb, argument );
	send_to_char( buf, ch );

	sprintf( buf, "$n %ss '#1$t#n'.",     verb );
	sprintf( buf2, "$n %ss '#1$t#n'.",     verb );
	break;

    case CHANNEL_IMMTALK:
	sprintf( buf, "#5$n: $t.#n" );
	sprintf( buf2, "#5$n: $t.#n" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;

   case CHANNEL_CLAN:
        sprintf( buf, "$n clantalks '#7$t#n'" );
	sprintf( buf2, "$n clantalks '#7$t#n'" );
	position	= ch->position;
	ch->position	= POS_STANDING;
	act( buf, ch, argument, NULL, TO_CHAR );
	ch->position	= position;
	break;
   case CHANNEL_CLAN1:
        sprintf( buf, "#4+o{#7$n#4}o+#n '#2$t#n'" );
        sprintf( buf2,"#4+o{#7$n#4}o+#n '#2$t#n'" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;
   case CHANNEL_CLAN2:
        sprintf( buf, "#4[#l\\#7-#0$n#7-#l/#4]#n '#7$t#n'" );
        sprintf( buf2, "#4[#l\\#7-#0$n#7-#l/#4]#n '#7$t#n'" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;
   case CHANNEL_CLAN3:
        sprintf( buf, "#0(#g->#2$n#g<-#0) #n'#7$t#n'" );
        sprintf( buf2, "#0(#g->#2$n#g<-#0) #n'#7$t#n'" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

   case CHANNEL_CLAN4:
        sprintf( buf, "#7=#5)#p[ #0$n #p]#5(#7= #n'#0$t#n'" );
        sprintf( buf2, "#7=#5)#p[ #0$n #p]#5(#7= #n'#0$t#n'" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

   case CHANNEL_CLAN5:
        sprintf( buf, "#7<#0-(#4$n#0)-#7> #n'#7$t#n'" );
        sprintf( buf2, "#7<#0-(#4$n#0)-#7> #n'#7$t#n'" );
        position        = ch->position;
        ch->position    = POS_STANDING;
        act( buf, ch, argument, NULL, TO_CHAR );
        ch->position    = position;
        break;

   case CHANNEL_CLAN6:
	sprintf( buf, "#4[#6>>#7-#0$n#7-#6<<#4]#n '#7$t#n'");
	sprintf(buf2, "#4[#6>>#7-#0$n#7-#6<<#4]#n '#7$t#n'");
	position = ch->position;
	ch->position = POS_STANDING;
	act(buf, ch, argument, NULL, TO_CHAR);
	ch->position = position;
	break;

   case CHANNEL_CLAN7:
        sprintf( buf, "#r=#1^#0]#7$n#0[#1^#r= #c'$t'#n");
        sprintf(buf2, "#r=#1^#0]#7$n#0[#1^#r= #c'$t'#n");
        position = ch->position;
        ch->position = POS_STANDING;
        act(buf, ch, argument, NULL, TO_CHAR);
        ch->position = position;
        break;

    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *och;
	CHAR_DATA *vch;
        char buf3[MAX_STRING_LENGTH];

	och = d->original ? d->original : d->character;
	vch = d->character;

	ignored = FALSE;

	if ( d->connected == CON_PLAYING
	&&   vch != ch
	&&  !IS_SET(och->deaf, channel) )
	{
	for (pos = 0; pos < MAX_FORGET; pos++) 
        {
	if (och->pcdata->forget[pos] == NULL) break;
	if (!str_cmp(ch->name, och->pcdata->forget[pos]))
	ignored = TRUE;
	}
	if (ignored) continue;
	
	if ( channel == CHANNEL_IMMTALK && !IS_IMMORTAL(och) )
		continue;
	if ( channel == CHANNEL_NEWTALK && !IS_NPC(och) && (och->level > 2 || och->max_hit > 3000) && !IS_IMMORTAL(och))
		continue;
	if ( channel == CHANNEL_CLAN && (!IS_NPC(och) && (str_cmp(ch->clan,och->clan)) && !IS_IMMORTAL(och) ))
	    continue;
        if ( channel == CHANNEL_CLAN1 && (!IS_NPC(och) && (str_cmp(och->clan, "Legion")) &&!IS_IMMORTAL(och)))
           continue; 
        if ( channel == CHANNEL_CLAN2 && (!IS_NPC(och) && (str_cmp(och->clan, "Dementia")) && !IS_IMMORTAL(och)))
           continue;
        if ( channel == CHANNEL_CLAN3 && (!IS_NPC(och) && (str_cmp(och->clan, "Fallen")) && !IS_IMMORTAL(och)))
           continue;
        if ( channel == CHANNEL_CLAN4 && (!IS_NPC(och) && (str_cmp(och->clan, "Nocturnal")) && !IS_IMMORTAL(och)))
           continue;
        if ( channel == CHANNEL_CLAN5 && (!IS_NPC(och) && (str_cmp(och->clan, "Legends")) && !IS_IMMORTAL(och)))
           continue;
	if ( channel == CHANNEL_CLAN6 && (!IS_NPC(och) && (str_cmp(och->clan, "DAMNED")) && !IS_IMMORTAL(och)))
	   continue;
        if ( channel == CHANNEL_CLAN7 && (!IS_NPC(och) && (str_cmp(och->clan, "BadAss")) && !IS_IMMORTAL(och)))
           continue;
	if ( channel == CHANNEL_YELL &&   vch->in_room->area != ch->in_room->area )
		continue;
        position = vch->position;
        if ( channel != CHANNEL_SHOUT && channel != CHANNEL_YELL )
		vch->position	= POS_STANDING;
	if (channel == CHANNEL_NEWTALK && !IS_NPC(vch) && ( (vch->level < 3 && vch->max_hit <= 3000) ||
IS_IMMORTAL(vch) ) )
	{
		act( buf2, ch, argument, vch, TO_VICT);
		vch->position = position;
		continue;
	}
        else
        {
            act( buf2, ch,argument, vch, TO_VICT);
            if( ((channel == CHANNEL_CLAN) || (channel == CHANNEL_CLAN1)
             || (channel == CHANNEL_CLAN2) || (channel == CHANNEL_CLAN3)
             || (channel == CHANNEL_CLAN4) || (channel == CHANNEL_CLAN5)
	     || (channel == CHANNEL_CLAN6))
             && (vch->desc->infect))
            {
             sprintf(buf3, "#4%s:#n ", vch->name);
             strcat(buf3, buf2);
             act(buf3, ch, argument, vch->desc->infect->character, TO_VICT);
            } 
            vch->position = position;
            continue;
        }
      }
    }
    return;
}

void do_newtalk( CHAR_DATA *ch, char *argument )
{
	if (IS_NPC(ch) || (!IS_IMMORTAL(ch) && (ch->level > 2 || ch->max_hit > 3000)) )

	{
		send_to_char("Huh?\n\r", ch);
		return;
	}		
    talk_channel( ch, argument, CHANNEL_NEWTALK, "newtalk" );
    return;
}

void do_clan1(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "Legion") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN1, "conspire");
  return;
}

void do_clan2(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "Dementia") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN2, "asylum");
  return;
}

void do_clan3(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "Fallen") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN3, "kink");
  return;
}

void do_clan4(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "Nocturnal") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN4, "darkspeak");
  return;
}

void do_clan5(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "Legends") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN5, "scheme");
  return;
}

void do_clan6(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "DAMNED") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN6, "seance");
  return;
}

void do_clan7(CHAR_DATA *ch, char *argument)
{
  if( str_cmp(ch->clan, "BadAss") && !IS_IMMORTAL(ch))
   return;
  talk_channel( ch, argument, CHANNEL_CLAN7, "klineownsus");
  return;
}

void room_message(ROOM_INDEX_DATA *room, char *message)
{
    CHAR_DATA *rch;

    if ( (rch = room->people) == NULL) return;

    act(message, rch, NULL, NULL, TO_ROOM);
    act(message, rch, NULL, NULL, TO_CHAR);
}

void do_chat( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_CHAT, "chat" );
 return;
}

void do_clantalk( CHAR_DATA *ch, char *argument )
{ 
 if( !str_cmp(ch->clan, "DAMNED" ))
  do_clan6(ch,argument);
 else if( !str_cmp(ch->clan, "Legends" ))
  do_clan5(ch,argument);
 else if( !str_cmp(ch->clan,"Dementia" ))
  do_clan2(ch,argument);
 else if( !str_cmp(ch->clan, "Fallen" ))
  do_clan3(ch,argument);
 else if( !str_cmp(ch->clan, "Legion" ))
  do_clan1(ch,argument);
 else if( !str_cmp(ch->clan, "Nocturnal" ))
  do_clan4(ch,argument);
 else if( !str_cmp(ch->clan, "BadAss" ))
  do_clan7(ch,argument);
 else
  return;
 return;
}

/*
 * Alander's new channels.
 */
void do_music( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_MUSIC, "music" );
 return;
}

void do_question( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_QUESTION, "question" );
 return;
}

void do_answer( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_QUESTION, "answer" );
 return;
}

void do_shout( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_SHOUT, "shout" );
 return;
}

void do_yell( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_YELL, "yell" );
 return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
 talk_channel( ch, argument, CHANNEL_IMMTALK, "immtalk" );
 return;
}

void do_say( CHAR_DATA *ch, char *argument )
{
    char name   [80];
    char poly   [MAX_STRING_LENGTH];
    char speak  [10];
    char speaks [10];
    char endbit [2];
    char secbit [2];
    CHAR_DATA *to;
    bool is_ok;

    if (RTIMER(ch->in_room, RTIMER_SILENCE) != 0)
    {
     send_to_char("Something prevents you from speaking in this room.\n\r", ch);
     return;
    }
    if ( IS_HEAD(ch, LOST_TONGUE) )
    {
	send_to_char( "You can't speak without a tongue!\n\r", ch );
	return;
    }
    if ( IS_EXTRA(ch, GAGGED) )
    {
	send_to_char( "You can't speak with a gag on!\n\r", ch );
	return;
    }

    if (strlen(argument) > MAX_INPUT_LENGTH)
    {
	send_to_char( "Line too long.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }

    endbit[0] = argument[strlen(argument)-1];
    endbit[1] = '\0';

    if (strlen(argument) > 1) secbit[0] = argument[strlen(argument)-2];
	else secbit[0] = '\0';
    secbit[1] = '\0';

    if (IS_BODY(ch,CUT_THROAT))
    {
	sprintf(speak,"rasp");
	sprintf(speaks,"rasps");
    }
    else if (!IS_NPC(ch) && !str_cmp(ch->pcdata->switchname,"Kline") )
    {
	sprintf(speak,"demonically chant");
	sprintf(speaks,"demonically chants");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_FROG)
    {
	sprintf(speak,"croak");
	sprintf(speaks,"croaks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_RAVEN)
    {
	sprintf(speak,"squark");
	sprintf(speaks,"squarks");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_CAT)
    {
	sprintf(speak,"purr");
	sprintf(speaks,"purrs");
    }
    else if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_DOG)
    {
	sprintf(speak,"bark");
	sprintf(speaks,"barks");
    }
    else if (!str_cmp(endbit,"!"))
    {
	sprintf(speak,"exclaim");
	sprintf(speaks,"exclaims");
    }
    else if (!str_cmp(endbit,"?"))
    {
	sprintf(speak,"ask");
	sprintf(speaks,"asks");
    }
    else if (secbit[0] != '\0' && str_cmp(secbit,".") && !str_cmp(endbit,"."))
    {
	sprintf(speak,"state");
	sprintf(speaks,"states");
    }
    else if (secbit[0] != '\0' && !str_cmp(secbit,".") && !str_cmp(endbit,"."))
    {
	sprintf(speak,"mutter");
	sprintf(speaks,"mutters");
    }
    else if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
    {
	sprintf(speak,"slur");
	sprintf(speaks,"slurs");
    }
    else
    {
	sprintf(speak,"say");
	sprintf(speaks,"says");
    }
    sprintf(poly,"You %s '#3$T#n'.", speak);
    if( !IS_NPC(ch) )
     act( poly, ch, NULL, argument, TO_CHAR );

    sprintf(poly,"$n %s '#3$T'.", speaks);
    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
        act( poly, ch, NULL, argument, TO_ROOM );
        room_text( ch, strlower(argument) );
        return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

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

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
        else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s '%s'.\n\r", name,speaks,argument);
	send_to_char(poly,to);
    }

    room_text( ch, strlower(argument) );
    return;
}

void room_text( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA     *vch;
    CHAR_DATA     *vch_next;
    CHAR_DATA     *mob;
    OBJ_DATA      *obj;
    ROOMTEXT_DATA *rt;
    bool           mobfound;
    bool           hop;

    char           arg  [MAX_INPUT_LENGTH];
    char           arg1 [MAX_INPUT_LENGTH];
    char           arg2 [MAX_INPUT_LENGTH];

    for ( rt = ch->in_room->roomtext; rt != NULL; rt = rt->next )
    {
	if (!strcmp(argument,rt->input)
	 || is_in(argument, rt->input)
	 || all_in(argument, rt->input)) 
	{
	    if ( rt->name != NULL         && rt->name != '\0'
	    &&   str_cmp(rt->name,"all")  && str_cmp(rt->name,"|all*") ) 
	    	if (!is_in(ch->name, rt->name) ) continue;
	    mobfound = TRUE;
	    if (rt->mob != 0)
	    {
		mobfound = FALSE;
	    	for ( vch = char_list; vch != NULL; vch = vch_next )
	    	{
		    vch_next	= vch->next;
		    if ( vch->in_room == NULL ) continue;
		    if ( !IS_NPC(vch)         ) continue;
		    if ( vch->in_room == ch->in_room
			&& vch->pIndexData->vnum == rt->mob )
		    {
			mobfound = TRUE;
		    	break;
		    }
	    	}
	    }
	    if (!mobfound) continue;
	    hop = FALSE;
	    switch(rt->type % RT_RETURN)
	    {
		case RT_SAY:
		    break;
		case RT_LIGHTS:
		    do_changelight(ch,"");
		    break;
		case RT_LIGHT:
		    REMOVE_BIT(ch->in_room->room_flags, ROOM_DARK);
		    break;
		case RT_DARK:
		    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
		    break;
		case RT_OBJECT:
		    if ( get_obj_index(rt->power) == NULL ) return;
		    if ( IS_NPC(ch) )
			{
			   act("\n\rAsmodeus, Overlord of the Demons prevents your evil deed!\n\r", ch, NULL, NULL, TO_ROOM);
		  	   return;
			}
	 	    obj = create_object(get_obj_index(rt->power), ch->level);
		    if (IS_SET(rt->type, RT_TIMER) ) obj->timer = 1;
		    if (CAN_WEAR(obj,ITEM_TAKE)) obj_to_char(obj,ch);
		    else obj_to_room(obj,ch->in_room);
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, obj, NULL, TO_CHAR );
		    else
		    	act( rt->choutput, ch, obj, NULL, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, obj, NULL, TO_ROOM );
		    hop = TRUE;
		    break;
		case RT_MOBILE:
		    if ( get_mob_index(rt->power) == NULL ) return;
		    mob = create_mobile(get_mob_index(rt->power));
		    char_to_room(mob,ch->in_room);
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, NULL, mob, TO_CHAR );
		    else
		    	act( rt->choutput, ch, NULL, mob, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, NULL, mob, TO_ROOM );
		    hop = TRUE;
		    break;
		case RT_SPELL:
		    (*skill_table[rt->power].spell_fun) ( rt->power, number_range(20,30), ch, ch );
		    break;
		case RT_PORTAL:
		    if ( get_obj_index(OBJ_VNUM_PORTAL) == NULL ) return;
		    obj = create_object(get_obj_index(OBJ_VNUM_PORTAL), 0);
		    obj->timer = 1;
		    obj->value[0] = rt->power;
		    obj->value[1] = 1;
		    obj_to_room(obj,ch->in_room);
		    break;
		case RT_TELEPORT:
                    if( ch->fighting != NULL && ch->in_room->vnum != 26300 ) return;
		    if ( get_room_index(rt->power) == NULL ) return;
		    if (!str_cmp(rt->choutput,"copy"))
		    	act( rt->output, ch, NULL, NULL, TO_CHAR );
		    else
		    	act( rt->choutput, ch, NULL, NULL, TO_CHAR );
		    if (!IS_SET(rt->type, RT_PERSONAL) ) 
			act( rt->output, ch, NULL, NULL, TO_ROOM );
		    if( ch->fighting != NULL && ch->in_room->vnum == 26300 )
		    {
			stop_fighting(ch, TRUE);
			return;
		    }
		    char_from_room(ch);
		    char_to_room(ch,get_room_index(rt->power));
		    act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
		    do_look(ch,"auto");
		    hop = TRUE;
		    break;
		case RT_ACTION:
		    sprintf(arg,argument);
		    argument = one_argument( arg, arg1 );
		    argument = one_argument( arg, arg2 );
		    if ( (mob = get_char_room(ch, arg2) ) == NULL ) continue;
		    interpret( mob, rt->output );
		    break;
		case RT_OPEN_LIFT:
		    open_lift(ch);
		    break;
		case RT_CLOSE_LIFT:
		    close_lift(ch);
		    break;
		case RT_MOVE_LIFT:
		    move_lift(ch,rt->power);
		    break;
		default:
		    break;
	    }
	    if (hop && IS_SET(rt->type, RT_RETURN) ) return;
	    else if (hop) continue;
	    if (!str_cmp(rt->choutput,"copy") && !IS_SET(rt->type, RT_ACTION))
	    	act( rt->output, ch, NULL, NULL, TO_CHAR );
	    else if (!IS_SET(rt->type, RT_ACTION))
	    	act( rt->choutput, ch, NULL, NULL, TO_CHAR );
	    if (!IS_SET(rt->type, RT_PERSONAL) && !IS_SET(rt->type, RT_ACTION) )
		act( rt->output, ch, NULL, NULL, TO_ROOM );
	    if (IS_SET(rt->type, RT_RETURN) ) return;
	}
    }
    return;
}

char *strlower(char *ip)
{
	static char buffer[MAX_INPUT_LENGTH];
	int pos;

	for (pos = 0; pos < (MAX_INPUT_LENGTH - 1) && ip[pos] != '\0'; pos++) {
		buffer[pos] = tolower(ip[pos]);
	}	
	buffer[pos] = '\0';
	return buffer;
}

bool is_in(char *arg, char *ip)
{
	char *lo_arg;
	char cmp[MAX_INPUT_LENGTH];
	int fitted;

	if (ip[0] != '|')
		return FALSE;
	cmp[0] = '\0';
	lo_arg = strlower(arg);
	do {
		ip += strlen(cmp) + 1;
		fitted = sscanf(ip, "%[^*]", cmp);
		if (strstr(lo_arg, cmp) != NULL) {
			return TRUE;
		}
	} while (fitted > 0);
	return FALSE;
}

bool all_in(char *arg, char *ip)
{
	char *lo_arg;
	char cmp[MAX_INPUT_LENGTH];
	int fitted;

	if (ip[0] != '&')
		return FALSE;
	cmp[0] = '\0';
	lo_arg = strlower(arg);
	do {
		ip += strlen(cmp) + 1;
		fitted = sscanf(ip, "%[^*]", cmp);
		if (strstr(lo_arg, cmp) == NULL) {
			return FALSE;
		}
	} while (fitted > 0);
	return TRUE;
}	

void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char poly [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	stc( "The can't hear you.", ch);
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	stc( "That person is currently link dead.", ch);
	return;
    }

    if ( IS_SET(victim->deaf, CHANNEL_TELL) && !IS_IMMORTAL(ch) )
    {
	if (IS_NPC(victim) || IS_NPC(ch))
	{
	    stc( "That person can't hear you.", ch);
	    return;
	}
    }
    sprintf(poly,"You tell %s '#6%s#n'.\n\r", victim->name, argument);
    stc(poly, ch);
   
    position		= victim->position;
    victim->position	= POS_STANDING;

    sprintf(poly,"%s tells you '#6%s#n'.\n\r", ch->name, argument);
    stc(poly, victim);
    victim->position	= position;
    victim->reply	= ch;

    return;
}



void do_whisper( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Not with a gag on!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Syntax: whisper <person> <message>\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_AWAKE(victim) )
    {
	act( "$E cannot hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	act( "$E is currently link dead.", ch, 0, victim, TO_CHAR );
	return;
    }

    act( "You whisper to $N '$t'.", ch, argument, victim, TO_CHAR );
    act( "$n whispers to you '$t'.", ch, argument, victim, TO_VICT );
    act( "$n whispers something to $N.", ch, NULL, victim, TO_NOTVICT );

    return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    char poly [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int position;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if (IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }
    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	stc( "That person can't hear you.", ch);
	return;
    }

    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	stc( "That person is currently link dead.", ch);
	return;
    }
    sprintf(poly,"You reply to %s '#6%s#n'.\n\r", victim->name, argument);
    stc(poly, ch);
    position		= victim->position;
    victim->position	= POS_STANDING;

    sprintf(poly,"%s replies '#6%s#n'.\n\r", ch->name, argument);
    stc(poly, victim);
    victim->position	= position;
    victim->reply	= ch;

    return;
}



void do_emote( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *plast;

    char name   [80];
    char poly   [MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    bool is_ok;


    if ( IS_HEAD(ch, LOST_TONGUE) || IS_HEAD(ch, LOST_HEAD) || IS_EXTRA(ch, GAGGED))
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Pose what?\n\r", ch );
	return;
    }

    for ( plast = argument; *plast != '\0'; plast++ )
	;

    strcpy( buf, argument );
    if ( isalpha(plast[-1]) )
	strcat( buf, "." );

    act( "$n $T", ch, NULL, buf, TO_CHAR );

    if (ch->in_room->vnum != ROOM_VNUM_IN_OBJECT)
    {
    	act( "$n $T", ch, NULL, buf, TO_ROOM );
	return;
    }

    to = ch->in_room->people;
    for ( ; to != NULL; to = to->next_in_room )
    {
	is_ok = FALSE;

	if ( to->desc == NULL || !IS_AWAKE(to) )
	    continue;

	if ( ch == to )
	    continue;

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

	if (IS_NPC(ch))
	    sprintf(name, ch->short_descr);
        else
	    sprintf(name, ch->name);
	name[0]=UPPER(name[0]);
	sprintf(poly,"%s %s\n\r", name,buf);
	send_to_char(poly,to);
    }
    return;
}

void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}

void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;
    location = get_room_index( 3207 );

    if ( IS_NPC(ch) )
		return;

	if (ch->fight_timer > 0)
	{
		send_to_char("You cannot quit out with your fight timer active.\n\r", ch);
		sprintf(buf,"You have %d rounds left on your fight timer.\n\r", ch->fight_timer);
		send_to_char(buf,ch);
		return;
	}
	if( deathmatch )
	{
	 stc("A deathmatch is currently in progress.  Please wait for it to finish first.\n\r",ch);
	 return;
	}
    if( !IS_IMMORTAL(ch) && ch->login_timer > 5 )
    {
     stc("You cannot quit out with a login timer greater than 5.\n\r",ch);
     sprintf(buf,"You have %d rounds left on your login timer.\n\r",ch->login_timer);
     stc(buf,ch);
     return;
    }
    /*if( !str_cmp(ch->pcdata->switchname,"Cyroth") )
    {
     do_huh(ch,"");
     return;
    }*/
    if ( ch->position == POS_FIGHTING )
    {
		send_to_char( "No way! You are fighting.\n\r", ch );
		return;
    }

    if ( ch->position  < POS_SLEEPING )
    {
		send_to_char( "You're not DEAD yet.\n\r", ch );
		return;
    }
	if ((IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)) && ch->pcdata->chobj != NULL)
	{
		send_to_char("You cannot quit out as a weapon.\n\r", ch);
		return;
	}
      
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68) && ( (IS_SET(ch->act, PLR_CHALLENGED)) || (IS_SET(ch->act, PLR_CHALLENGER)) ) )
	{
		send_to_char("You may not quit out from the arena whilst you are fighting.\n\r", ch);
		return;
	}
	if( (IS_CLASS(ch, CLASS_DRAGON)) && (ch->pcdata->held != NULL && ch->pcdata->held[0] != '\0') )
	{
		stc("You cannot quit while holding someone!\n\r", ch);
		return;
	}

	if( ch->pcdata->held_by != NULL && ch->pcdata->held_by[0] != '\0' )
	{
		stc("You cannot quit while you're being held!\n\r", ch);
		return;
	}
	save_char_obj_backup(ch);

	if (IS_SET(ch->act, PLR_CHALLENGER))
		do_withdraw(ch, ch->challenged->name);
	if (IS_SET(ch->act, PLR_CHALLENGED))
		do_decline(ch, ch->challenger->name);
	if (IS_SET(ch->newbits, HAS_HADES) )
		REMOVE_BIT(ch->newbits, HAS_HADES);
	if (IS_SET(ch->newbits, NEW_DARKNESS))
	{
		send_to_char("You banish your globe of darkness.\n\r", ch );
		act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
		REMOVE_BIT(ch->newbits, NEW_DARKNESS);
		if (IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS))
			REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	}
	if (( ch->level < 8) && IS_SET(ch->act, PLR_SACREDINVIS) )
		REMOVE_BIT(ch->act, PLR_SACREDINVIS);
	if( IS_CLASS(ch,CLASS_SHADOW) && IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_INVIS) )
		conceal_count(ch);
    if ( (mount = ch->mount) != NULL ) do_dismount(ch,""); 
          switch( number_range(1,3) )
          {
          case 1:
           send_to_char("        Which came first:\n\r", ch);
           send_to_char("         The violent impulse, \n\r", ch);
           send_to_char("         or the weapon that answers it's call?\n\r", ch);
           break;
          case 2:
           send_to_char("          I will carry out great vengeance on them and\n\r", ch);
           send_to_char("            punish them in my wrath.  Then they will\n\r", ch);
           send_to_char("               know that I am the Lord.\n\r", ch);
           send_to_char("                      Ezekiel 25:17\n\r", ch);
           break;
          case 3:
           stc("          The more you know, the more you know\n\r",ch);
           stc("           that there is -TO- know...Therefore,\n\r",ch);
           stc("            the more you know, the stupider you are.\n\r",ch);
           stc("             --Unknown\n\r",ch);
          }
    /*
     * After extract_char the ch is no longer valid!
     */

    d = ch->desc;
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if (obj->wear_loc == WEAR_NONE)
		continue;
    if ( !IS_NPC(ch) && ((obj->chobj != NULL && !IS_NPC(obj->chobj) 
      && obj->chobj->pcdata->obj_vnum != 0)
      || obj->item_type == ITEM_KEY))
	    unequip_char(ch,obj);
       }
    for (obj= ch->carrying; obj != NULL; obj = obj_next )
    {
      obj_next = obj->next_content;
      if (IS_SET (obj->quest, QUEST_ARTIFACT) || obj->item_type == ITEM_CORPSE_NPC)
          {  
            obj_from_char (obj);
            obj_to_room (obj, location);
          }
          }

	if (ch->level >= 2)
	{
		save_char_obj_backup( ch );
		save_char_obj( ch );
	}
	else
		send_to_char("Your character has not killed 5 mobs and thus cannot be saved\n\r", ch);
    if (ch->pcdata->obj_vnum != 0)
    	act( "$n slowly fades out of existance.", ch, NULL, NULL, TO_ROOM );
    else
    	act( "$n has left the game.", ch, NULL, NULL, TO_ROOM );
    if ( d != NULL )
		close_socket2( d, FALSE );
    if (ch->in_room != NULL) 
        char_from_room(ch);
		char_to_room(ch,get_room_index(ROOM_VNUM_DISCONNECTION));
	sprintf( log_buf, "%s has quit.", ch->name );
    log_string( log_buf );
/*klinemark logout*/
    if (ch->pcdata->obj_vnum == 0)
    {
	if (IS_SET(ch->newbits, NEW_SETEXIT))
	sprintf(buf,"%s", ch->setexit);
	else
		sprintf( buf, "#3%s #2has escaped The Vortex.#n", ch->pcdata->switchname );
	if( !IS_SET(ch->act,PLR_WIZINVIS) && !IS_SET(ch->act,PLR_INCOG) )
	 do_info(ch,buf);
    }

    if (ch->pcdata->chobj != NULL) 
          extract_obj(ch->pcdata->chobj);
		extract_char( ch, TRUE );
    return;
}

void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( ch->level < 2 )
    {
	send_to_char( "You must kill at least 5 mobs before you can save.\n\r", ch );
	return;
    }

    save_char_obj_backup( ch );
    save_char_obj( ch );
    send_to_char( "Saved.\n\r", ch );
    return;
}



void do_autosave( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if ( ch->level < 2 ) return;
    save_char_obj( ch );
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower( ch );
	return;
    }
    
    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}



void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) && IS_SET(GET_POWERS(ch, SHADOW_BITS),SHADOW_FOLLGO) )
    {
     ch->master = master;
     ch->leader = NULL;
     act( "You start shadowing $N.", ch, NULL, ch->master, TO_CHAR    );
     SET_BIT_POWERS(ch, SHADOW_BITS, SHADOW_FOLLOW);
     return;
    }

    else if( can_see( master, ch ) )
    {
     ch->master = master;
     ch->leader = NULL;
     act( "$n now follows you.", ch, NULL, master, TO_VICT );
     act( "You now follow $N.",  ch, NULL, master, TO_CHAR );
     return;
    }
return;
}



void stop_follower( CHAR_DATA *ch )
{

    CHAR_DATA *master;

    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }


    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    if( IS_NPC(ch) )
    {
    	if( ch->master != NULL )
	{
	   master = ch->master;
           if( master->pcdata->followers > 0 )	
	   	master->pcdata->followers--;
	}
    }
    if( IS_NPC(ch) )
	if( ch->pIndexData->vnum == 8 && IS_SET(master->newbits, HAS_HADES) )
	   REMOVE_BIT(master->newbits, HAS_HADES);

    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) && IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_FOLLOW) )
    {
     act( "You stop shadowing $N.", ch, NULL, ch->master, TO_CHAR    );
     ch->master = NULL;
     ch->leader = NULL;
     REM_BIT_POWERS(ch, SHADOW_BITS, SHADOW_FOLLOW);
     REM_BIT_POWERS(ch,SHADOW_BITS,SHADOW_FOLLGO);
     GET_POWERS(ch, SHADOW_COUNTER) = 0;
     return;
    }

    else if( can_see( ch->master, ch ) )
    {
     act( "$n stops following you.", ch, NULL, ch->master, TO_VICT );
     act( "You stop following $N.", ch, NULL, ch->master, TO_CHAR    );
     ch->master = NULL;
     ch->leader = NULL;
     if( IS_CLASS(ch, CLASS_SHADOW) )
      GET_POWERS(ch, SHADOW_COUNTER) = 0;
     return;
    }
return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
	stop_follower( ch );

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if ( (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch) )

	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}


    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;
	if ( och == ch ) continue;
	if ((IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) ))
	{
	    found = TRUE;
	    act( "$n orders you to '$t'.", ch, argument, och, TO_VICT );
	    interpret( och, argument );
	}
    }

    if ( found ) 

	send_to_char( "Ok.\n\r", ch );
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}

void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		sprintf( buf,
		"[%-16s] %4d/%4d hp %4d/%4d mana %4d/%4d mv %5d xp\n\r",
		    capitalize( PERS(gch, ch) ),
		    gch->hit,   gch->max_hit,
		    gch->mana,  gch->max_mana,
		    gch->move,  gch->max_move,
		    gch->exp    );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	return;
    }
	/* Added this to hopefully stop the crashing when attack a mob with a mob */
	if (IS_NPC(victim) && victim->level < 5)
	{
		send_to_char("You may not group that mob.\n\r", ch);
		return;
	}
    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
	act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
	act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );
	return;
    }

    victim->leader = ch;
    act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
    act( "You join $n's group.", ch, NULL, victim, TO_VICT    );
    act( "$N joins your group.", ch, NULL, victim, TO_CHAR    );
    return;
}

void do_gtell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->act, PLR_NO_TELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
    sprintf( buf, "%s tells the group '%s'.\n\r", ch->name, argument );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	{
          send_to_char( buf, gch );
          if( gch->desc->infect)
          {
            sprintf( buf2, "#4%s:#n", gch->name);
            strcat(buf2, buf);
            stc(buf2, gch->desc->infect->character);
          }
        }
    }

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

void do_changelight(CHAR_DATA *ch, char *argument )
{
    if (IS_SET(ch->in_room->room_flags, ROOM_DARK))
    {
	REMOVE_BIT(ch->in_room->room_flags, ROOM_DARK);
	act("The room is suddenly filled with light!",ch,NULL,NULL,TO_CHAR);
	act("The room is suddenly filled with light!",ch,NULL,NULL,TO_ROOM);
	return;
    }
    SET_BIT(ch->in_room->room_flags, ROOM_DARK);
    act("The lights in the room suddenly go out!",ch,NULL,NULL,TO_CHAR);
    act("The lights in the room suddenly go out!",ch,NULL,NULL,TO_ROOM);
    return;
}

void open_lift( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (is_open(ch)) return;

    act("The doors open.",ch,NULL,NULL,TO_CHAR);
    act("The doors open.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_ROOM);
    if (!same_floor(ch,in_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_ROOM);
    if (is_open(ch)) act("The doors open.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    open_door(ch,TRUE);
    move_door(ch);
    open_door(ch,TRUE);
    thru_door(ch,in_room);
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void close_lift( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (!is_open(ch)) return;
    act("The doors close.",ch,NULL,NULL,TO_CHAR);
    act("The doors close.",ch,NULL,NULL,TO_ROOM);
    open_door(ch,FALSE);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void move_lift( CHAR_DATA *ch, int to_room )
{
    ROOM_INDEX_DATA *location;
    int in_room;

    in_room  = ch->in_room->vnum;
    location = get_room_index(in_room);

    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_CHAR);
    if (is_open(ch)) act("The doors close.",ch,NULL,NULL,TO_ROOM);
    if (!same_floor(ch,to_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_CHAR);
    if (!same_floor(ch,to_room)) act("The lift judders suddenly.",ch,NULL,NULL,TO_ROOM);
    move_door(ch);
    open_door(ch,FALSE);
    char_from_room(ch);
    char_to_room(ch,location);
    open_door(ch,FALSE);
    thru_door(ch,to_room);
    return;
}

bool same_floor( CHAR_DATA *ch, int cmp_room )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[0] == cmp_room ) return TRUE;
	else return FALSE;
    }
    return FALSE;
}

bool is_open( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[2] == 0 ) return TRUE;
	else return FALSE;
    }
    return FALSE;
}

void move_door( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *pRoomIndex;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	pRoomIndex = get_room_index(obj->value[0]);
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	return;
    }
    return;
}

void thru_door( CHAR_DATA *ch, int doorexit )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	obj->value[0] = doorexit;
	return;
    }
    return;
}

void open_door( CHAR_DATA *ch, bool be_open )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( obj->item_type != ITEM_PORTAL ) continue;
	if ( obj->pIndexData->vnum == 30001 ) continue;
	if ( obj->value[2] == 0 && !be_open ) obj->value[2] = 3;
	else if ( obj->value[2] == 3 && be_open ) obj->value[2] = 0;
	return;
    }
    return;
}

char *socialc( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "you are",	"$E is"		},
	{ "you.",	"$M."		},
	{ "you,",	"$M,"		},
	{ "you ",	"$M "		},
	{ " you",	" $M"		},
	{ "your ",	"$S "		},
	{ " your",	" $S"		},
	{ "yours.",	"theirs."	},
	{ "yours,",	"theirs,"	},
	{ "yours ",	"theirs "	},
	{ " yours",	" theirs"	},
	{ "begins",	"begin"		},
	{ "caresses",	"caress"	},
	{ "gives",	"give"		},
	{ "glares",	"glare"		},
	{ "grins",	"grin"		},
	{ "licks",	"lick"		},
	{ "looks",	"look"		},
	{ "loves",	"love"		},
	{ "plunges",	"plunge"	},
	{ "presses",	"press"		},
	{ "pulls",	"pull"		},
	{ "runs",	"run"		},
	{ "slaps",	"slap"		},
	{ "slides",	"slide"		},
	{ "smashes",	"smash"		},
	{ "squeezes",	"squeeze"	},
	{ "stares",	"stare"		},
	{ "sticks",	"stick"		},
	{ "strokes",	"stroke"	},
	{ "tugs",	"tug"		},
	{ "thinks",	"think"		},
	{ "thrusts",	"thrust"	},
	{ "whistles",	"whistle"	},
	{ "wraps",	"wrap"		},
	{ "winks",	"wink"		},
	{ "wishes",	"wish"		},
	{ " winks",	" wink"		},
	{ " his",	" your"		},
	{ "his ",	"your "		},
	{ " her",	" your"		},
	{ "her ",	"your "		},
	{ " him",	" your"		},
	{ "him ",	"your "		},
	{ "the",	"the"		},
	{ " he",	" you"		},
	{ "he ",	"you "		},
	{ " she",	" you"		},
	{ "she ",	"you "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *socialv( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ " his",	" $s"		},
	{ "his ",	"$s "		},
	{ " her",	" $s"		},
	{ "her ",	"$s "		},
	{ " him",	" $m"		},
	{ "him ",	"$m "		},
	{ " he",	" $e"		},
	{ "he ",	"$e "		},
	{ " she",	" $e"		},
	{ "she ",	"$e "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}

char *socialn( CHAR_DATA *ch, char *argument, char *you, char *them )
{
    char buf  [MAX_STRING_LENGTH];
    char *pName;
    int iSyl;
    int length;

    struct spk_type
    {
	char *	old;
	char *	new;
    };

    static const struct spk_type spk_table[] =
    {
	{ " ",		" "		},
	{ "you are",	"$N is"		},
	{ "you.",	"$N."		},
	{ "you,",	"$N,"		},
	{ "you ",	"$N "		},
	{ " you",	" $N"		},
	{ "your.",	"$N's."		},
	{ "your,",	"$N's,"		},
	{ "your ",	"$N's "		},
	{ " your",	" $N's"		},
	{ "yourself",	"$Mself"	},
	{ " his",	" $s"		},
	{ "his ",	"$s "		},
	{ " her",	" $s"		},
	{ "her ",	"$s "		},
	{ " him",	" $m"		},
	{ "him ",	"$m "		},
	{ " he",	" $e"		},
	{ "he ",	"$e "		},
	{ " she",	" $e"		},
	{ "she ",	"$e "		},
	{ "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" },
	{ "e", "e" }, { "f", "f" }, { "g", "g" }, { "h", "h" },
	{ "i", "i" }, { "j", "j" }, { "k", "k" }, { "l", "l" },
	{ "m", "m" }, { "n", "n" }, { "o", "o" }, { "p", "p" },
	{ "q", "q" }, { "r", "r" }, { "s", "s" }, { "t", "t" },
	{ "u", "u" }, { "v", "v" }, { "w", "w" }, { "x", "x" },
	{ "y", "y" }, { "z", "z" }, { ",", "," }, { ".", "." },
	{ ";", ";" }, { ":", ":" }, { "(", "(" }, { ")", ")" },
	{ ")", ")" }, { "-", "-" }, { "!", "!" }, { "?", "?" },
	{ "1", "1" }, { "2", "2" }, { "3", "3" }, { "4", "4" },
	{ "5", "5" }, { "6", "6" }, { "7", "7" }, { "8", "8" },
	{ "9", "9" }, { "0", "0" }, { "%", "%" }, {  "",  "" }
    };
    buf[0]	= '\0';

    if ( argument[0] == '\0' ) return argument;

    for ( pName = str_dup(argument); *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(spk_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( spk_table[iSyl].old, pName ) )
	    {
		strcat( buf, spk_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    argument[0] = '\0';
    strcpy(argument,buf);
    argument[0] = UPPER(argument[0]);

    return argument;
}


void do_ignore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int pos;
    bool found = FALSE;

    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;

    if (IS_NPC(rch))
	return;

    smash_tilde( argument );

    argument = one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
	if (rch->pcdata->forget[0] == NULL)
	{
	    send_to_char("You are not ignoring anyone.\n\r",ch);
	    return;
	}
	send_to_char("You are currently ignoring:\n\r",ch);

	for (pos = 0; pos < MAX_FORGET; pos++)
	{
	    if (rch->pcdata->forget[pos] == NULL)
		break;

	    sprintf(buf,"    %s\n\r",rch->pcdata->forget[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;

	if (!str_cmp(arg,rch->pcdata->forget[pos]))
	{
	    send_to_char("You have already ignored that person.\n\r",ch);
	    return;
	}
    }

    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;
	
	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if (!str_cmp(arg,wch->name))
	{
	    found = TRUE;
	    if (wch == ch)
	    {
		send_to_char("You can't ignore yourself!\n\r", ch );
		return;
	    }
	    if (wch->level >= LEVEL_IMMORTAL)
	    {
		send_to_char("That person is very hard to ignore.\n\r",ch);
		return;
	    }
	}
    }

    if (!found)
    {
	send_to_char("No one by that name is playing.\n\r",ch);
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;
     }

     if (pos >= MAX_FORGET)
     {
	send_to_char("Sorry, you have reached the ignore limit.\n\r",ch);
	return;
     }
  
     /* make a new forget */
     rch->pcdata->forget[pos]		= str_dup(arg);
     sprintf(buf,"You are now ignoring %s.\n\r",arg);
     send_to_char(buf,ch);
}

void do_unignore(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    int pos;
    bool found = FALSE;
 
    if (ch->desc == NULL)
	rch = ch;
    else
	rch = ch->desc->original ? ch->desc->original : ch;
 
    if (IS_NPC(rch))
	return;
 
    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	if (rch->pcdata->forget[0] == NULL)
	{
	    send_to_char("You are not ignoring anyone.\n\r",ch);
	    return;
	}
	send_to_char("You are currently ignoring:\n\r",ch);

	for (pos = 0; pos < MAX_FORGET; pos++)
	{
	    if (rch->pcdata->forget[pos] == NULL)
		break;

	    sprintf(buf,"    %s\n\r",rch->pcdata->forget[pos]);
	    send_to_char(buf,ch);
	}
	return;
    }

    for (pos = 0; pos < MAX_FORGET; pos++)
    {
	if (rch->pcdata->forget[pos] == NULL)
	    break;

	if (found)
	{
	    rch->pcdata->forget[pos-1]		= rch->pcdata->forget[pos];
	    rch->pcdata->forget[pos]		= NULL;
	    continue;
	}

	if(!strcmp(arg,rch->pcdata->forget[pos]))
	{
	    send_to_char("Ignore removed.\n\r",ch);
	    free_string(rch->pcdata->forget[pos]);
	    rch->pcdata->forget[pos] = NULL;
	    found = TRUE;
	}
    }

    if (!found)
	send_to_char("No one by that name is ignored.\n\r",ch);
}

void send_to_room( char *message, ROOM_INDEX_DATA *room )
{
 CHAR_DATA *vch;
 for ( vch = room->people; vch != NULL; vch = vch->next )
 if ( IS_AWAKE(vch) && vch->in_room == room )
 {
  stc( message, vch );         
  stc( "\n\r",  vch );
 }
 return;
}
