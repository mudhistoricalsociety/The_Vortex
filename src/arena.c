#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
bool status_kill( CHAR_DATA *ch, CHAR_DATA *victim);
bool valid_clan_challenge( CHAR_DATA *ch, CHAR_DATA *victim);

int	arena;
int     arena_type;
bool valid_clan_challenge( CHAR_DATA *ch, CHAR_DATA *victim)
{
  int class1 = ch->class;
  int class2 = victim->class;
  if( class1 != class2)
  return FALSE;
  if( str_cmp( ch->clan, victim->clan))
  return FALSE;
  else 
  return FALSE;
}
bool status_kill( CHAR_DATA *ch, CHAR_DATA *victim)
{
  if( (ch->race > victim->race) && ((ch->race - victim->race) <= 10))
  return TRUE;
  if( ch->race < victim->race)
  return TRUE;
  else
  return FALSE;
}
/*=======================================================================*
 * function: do_challenge                                                *
 * purpose: sends initial arena match query                              *
 * written by: Doug Araya (whiplash@tft.nacs.net) 6-10-96                *
 *=======================================================================*/
void do_challenge(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim; 
        char arg1[MAX_STRING_LENGTH];
        char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH]; 
	char buf1[MAX_STRING_LENGTH]; 
	char buf2[MAX_STRING_LENGTH]; 
	char buf3[MAX_STRING_LENGTH]; 
	/*DESCRIPTOR_DATA *d;*/
        argument = one_argument( argument, arg1);
        argument = one_argument( argument, arg2);
/* == First make all invalid checks == */

	if(IS_NPC(ch))
		return; 
	if( deathmatch )
	{
		stc("Not while in Deathmatch mode!\n\r", ch);
		return;
	}
	else if ( arena == FIGHT_START )
	{
		send_to_char("Sorry, some one else has already started a challenge, please try later.\n\r",ch);
		return;
	}

	else if ( arena == FIGHT_BUSY )
	{
		send_to_char("Sorry, there is a fight in progress, please wait a few moments.\n\r",ch);
		return; 
	}
	else if ( arena == FIGHT_LOCK )
	{
		send_to_char("Sorry, the arena is currently locked from use.\n\r",ch);
		return;
	}
	else if (IS_SET(ch->act,PLR_CHALLENGED))
	{
		send_to_char("You have already been challenged, either AGREE or DECLINE first.\n\r",ch);
		return; 
	}
	else if  (IS_SET(ch->act, PLR_CHALLENGER))
	{
		send_to_char("You have already challenged someone else\n\r", ch);
		return;
	}
	else if (ch->level < 3)
	{
		send_to_char("You must be an avatar to challenge for the arena.\n\r",ch);
		return;
	}
	else if (ch->hit < ch->max_hit)
	{
		send_to_char("You must be fully healed to fight in the arena.\n\r",ch);
		return;
	}
	else if  (ch->max_hit < 10000)
	{
		send_to_char("You require at least 10k hp before you can challenge.\n\r", ch);
		return;
	}
	else if (IS_IMMORTAL(ch))
	{
		send_to_char("You may not do that.\n\r", ch);
		return;
	}
	else if (arg1[0] == '\0')
	{
		send_to_char("You must specify whom you wish to challenge.\n\r",ch);
		return;
	}
	else if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char("They are not playing.\n\r",ch);
		return;
	}
	else if (IS_NPC(victim))
	{
		send_to_char("You cannot challenge NPC's.\n\r",ch);
		return;
	}
	else if  (victim == ch )
	{
		send_to_char("You cannot do that.\n\r", ch);
	}
	else if(IS_IMMORTAL(victim))
	{
		send_to_char("You really are not the smartest cookie are you?\n\r", ch);
		return;
	}
	else if (IS_SET(victim->act,PLR_CHALLENGER))
	{
		send_to_char("They have already challenged someone else.\n\r",ch);
		return;
	}
	else if  (IS_SET(victim->act, PLR_CHALLENGED))
	{
		send_to_char("They are already challenging someone else.\n\r", ch);
		return;
	}
	else if (victim->fighting != NULL )
	{
		send_to_char("That person is engaged in battle right now.\n\r",ch); 
		return; 
	}
	else if( victim->fight_timer > 0 )
	{
	 stc("You can't arena somebody who was recently fighting!\n\r",ch);
	 return;
	}
	else if (victim->hit < victim->max_hit)
	{
		send_to_char("That player is not healthy enough to fight right now.\n\r",ch);
		return;
	}
	else if  (victim->max_hit < 10000)
	{
		send_to_char("Your opponent requires more than 10k hp before they are viable.\n\r", ch);
		return;
	}
	else if  ( victim->level < 3)
	{
		send_to_char("You may not challenge a mortal to a fight.\n\r", ch);
		return;
	}
	else if  ( victim->desc == NULL )
	{
		send_to_char("That player is linkdead at the moment, try them later.\n\r",ch);
		return;
	}
        else
	{
/* == Now for the challenge == */
	ch->challenged = victim;
	SET_BIT(ch->act,PLR_CHALLENGER);
	victim->challenger = ch;
	SET_BIT(victim->act,PLR_CHALLENGED);
	arena = FIGHT_START;
	send_to_char("Challenge has been sent\n\r",ch);
        act("$n has challenged you to a death match.",ch,NULL,victim,TO_VICT);
	sprintf(buf1, "To meet the challenge type : AGREE %s\n\r", ch->pcdata->switchname);
	send_to_char(buf1, victim);
	sprintf(buf2, "To meet the challenge type : DECLINE %s\n\r", ch->pcdata->switchname);
	send_to_char(buf2, victim);
	sprintf(buf3, "To withdraw the challenge type : WITHDRAW %s\n\r", victim->pcdata->switchname);
	send_to_char(buf3, ch);
	sprintf(buf,"[ARENA] %s has been challenged by %s.",victim->pcdata->switchname,ch->pcdata->switchname);
	do_info(ch,buf);
	return;
        }
  return;
} 





/*=======================================================================*

 * function: do_accept                                                   *

 * purpose: to accept the arena match, and move the players to the arena *

 * written by: Doug Araya (whiplash@tft.nacs.net) 6-10-96                *

 *=======================================================================*/

void do_agree(CHAR_DATA *ch, char *argument)
{
	/*DESCRIPTOR_DATA *d;*/
	CHAR_DATA *victim; 
	char buf1[MAX_STRING_LENGTH]; 
	char buf2[MAX_STRING_LENGTH];
	int char_room; 
	int vict_room; 
/*== the room VNUM's for our arena.are ==*/
/* we use 1051 thru 1066 for a 4x4 arena */
	char_room = 50;
	vict_room = 68;
/* == first make all invalid checks == */
	victim = ch->challenger;
	if(IS_NPC(ch))
		return;
	if( !IS_SET(ch->act,PLR_CHALLENGED) )
	{
		send_to_char("You have not been challenged.\n\r",ch);
		return;
	}
	if( arena == FIGHT_BUSY )
	{
		send_to_char("Sorry, there is a fight in progress, please wait a few moments.\n\r",ch);
		return;
	}
	if( arena == FIGHT_LOCK )
	{
		send_to_char("Sorry, the arena is currently locked from use.\n\r",ch);
		return;
	}
/*	if(argument[0]=='\0')
	{
		send_to_char("You must specify whose challenge you wish to accept.\n\r",ch);
		return;
	}
	if((victim = get_char_world(ch,argument)) == NULL)
	{
		send_to_char("They aren't logged in!\n\r",ch);
		return;
	}
*/	if(victim == ch)
	{
		send_to_char("You haven't challenged yourself!\n\r",ch);
		return;
	}
/*	if(!IS_SET(victim->act,PLR_CHALLENGER) || victim != ch->challenger )
	{
		send_to_char("That player hasn't challenged you!\n\r",ch); 
		return; 
	}
*/
/* == now get to business == */
	send_to_char("You have accepted the challenge!\n\r",ch);
	act("$n accepts your challenge!",ch,NULL,victim,TO_VICT);
/* == announce the upcoming event == */
	sprintf(buf1, "[ARENA] %s (%d wins) (%d losses)", victim->name,victim->pcdata->awins,victim->pcdata->alosses );
	sprintf(buf2,"[ARENA] %s (%d wins) (%d losses)", ch->name,ch->pcdata->awins,ch->pcdata->alosses );
	do_info(ch,buf1);
	do_info(ch,buf2);
/*	strcpy(buf3,"[Arena] To wager on the fight, type: bet (amount) (player name)\n\r");
	for ( d = descriptor_list; d; d = d->next )
	{
		if( d->connected == CON_PLAYING)
		{
			ADD_COLOUR(ch,buf1,L_RED);
			ADD_COLOUR(ch,buf2,L_RED);
			ADD_COLOUR(ch,buf3,L_RED);
			send_to_char( buf1, d->character );
			send_to_char( buf2, d->character );
			send_to_char( buf3, d->character );
			d->character->gladiator = NULL;
		}
	}*/

/* == now move them both to an arena for the fun == */
        victim->pcdata->c_chal++;
	send_to_char("You make your way into the arena.\n\r",ch);
	char_from_room(ch); 
	char_to_room(ch,get_room_index(char_room));
	do_look(ch, "auto");
	send_to_char("You make your way to the arena.\n\r",victim); 
	char_from_room(victim); 
	char_to_room(victim,get_room_index(vict_room)); 
	do_look(victim, "auto");
        ch->pcdata->held = str_dup( "");
        ch->pcdata->held_by = str_dup( "");
        victim->pcdata->held = str_dup( "");
        victim->pcdata->held_by = str_dup("");
	arena = FIGHT_BUSY; 
	return; 
}



/*=======================================================================*
 * function: do_decline                                                  *
 * purpose: to chicken out from a sent arena challenge                   *
 * written by: Doug Araya (whiplash@tft.nacs.net) 6-10-96                *
 *=======================================================================*/

void do_decline(CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim; 
	char buf[MAX_STRING_LENGTH];
	/*DESCRIPTOR_DATA *d;*/
        victim = ch->challenger;
/*== make all invalid checks == */
	if(IS_NPC(ch))
		return;
	if( !IS_SET(ch->act,PLR_CHALLENGED) )
	{
		send_to_char("You have not been challenged.\n\r",ch);
		return;
	}
/*	if(argument[0]=='\0')
	{
		send_to_char("You must specify whose challenge you wish to decline.\n\r",ch);
		return;
	}
	if((victim = get_char_world(ch,argument)) == NULL)
	{
		send_to_char("They aren't logged in!\n\r",ch);
		return;
	}
	if(!IS_SET(victim->act,PLR_CHALLENGER) || victim != ch->challenger )
	{
		send_to_char("That player hasn't challenged you.\n\r",ch);
		return;
	}
*/	if(victim == ch)
		return;
	if(victim == NULL)
		return;

	if ( arena == FIGHT_BUSY )
	{
//		send_to_char("Your fight has started.\n\r", ch);
		return;
	}
        if( (ch->pcdata->c_chal = 5) && (arena_type == ARENA_CLAN))
        {
         send_to_char("Your clan leadership is forfeit.\n\r", ch);
         return;
        }	
/*== now actually decline == */
        if( arena_type == ARENA_CLAN)
         ch->pcdata->c_chal++;
	victim->challenged = NULL;
	REMOVE_BIT(victim->act,PLR_CHALLENGER); 
	ch->challenger = NULL;
	REMOVE_BIT(ch->act,PLR_CHALLENGED);
	arena = FIGHT_OPEN;
	send_to_char("Challenge declined!\n\r",ch);
	act("$n has declined your challenge.",ch,NULL,victim,TO_VICT);
	sprintf(buf,"[ARENA] %s has declined %s's challenge.", ch->name, victim->name);
/*	for ( d = descriptor_list; d; d = d->next )
	{
		if( d->connected == CON_PLAYING)
		{
			ADD_COLOUR(ch,buf,L_RED);
			send_to_char( buf, d->character );
		}
	}*/
	do_info(ch,buf);

	return;
}


void do_withdraw(CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim; 
	char buf[MAX_STRING_LENGTH];
	/*DESCRIPTOR_DATA *d;*/
        victim = ch->challenged;
/*== make all invalid checks == */
	if(IS_NPC(ch))
		return;
	if( !IS_SET(ch->act,PLR_CHALLENGER) )
	{
		send_to_char("You have not been challenged.\n\r",ch);
		return;
	}
/*	if(argument[0]=='\0')
	{
		send_to_char("You must specify the challenge you wish to withdraw.\n\r",ch);
		return;
	}
	if((victim = get_char_world(ch,argument)) == NULL)
	{
		send_to_char("They aren't logged in!\n\r",ch);
		return;
	}
	if(!IS_SET(victim->act,PLR_CHALLENGED) || victim != ch->challenged )
	{
		send_to_char("That player hasn't been challenged by you.\n\r",ch);
		return;
	}
*/	if(victim == ch)
		return; 

        if( victim == NULL)
        return;
	if ( arena == FIGHT_BUSY )

	{

//		send_to_char("Your fight has started.\n\r", ch);

		return;

	}
/*== now actually decline == */
        if( arena_type == ARENA_CLAN)
         ch->pcdata->c_chal--;
	victim->challenger = NULL;
	REMOVE_BIT(victim->act,PLR_CHALLENGED); 
	ch->challenged = NULL;
	REMOVE_BIT(ch->act,PLR_CHALLENGER);
	arena = FIGHT_OPEN;
	send_to_char("Challenge withdrawn!\n\r",ch);
	act("$n has withdrawn their challenge.",ch,NULL,victim,TO_VICT);
	sprintf(buf,"[ARENA] %s has withdrawn their challenge on %s.",ch->pcdata->switchname, victim->pcdata->switchname);
	do_info(ch,buf);
/*	for ( d = descriptor_list; d; d = d->next )
	{
		if( d->connected == CON_PLAYING)
		{
			ADD_COLOUR(ch,buf,L_RED);
			send_to_char( buf, d->character );
		}
	}*/
	return;
}
void do_arena( CHAR_DATA *ch, char *argument )
{
    char buf2[MAX_STRING_LENGTH];
	char bufrogue[MAX_STRING_LENGTH];
	char bufultimate[MAX_STRING_LENGTH];
	char bufsupreme[MAX_STRING_LENGTH];
	char bufgreater[MAX_STRING_LENGTH];
	char buflesser[MAX_STRING_LENGTH];
	char bufdemi[MAX_STRING_LENGTH];
	char bufgodling[MAX_STRING_LENGTH];
	char bufimmortal[MAX_STRING_LENGTH];
	char bufavatar[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    /*char rep[MAX_STRING_LENGTH];*/
    char openb[5];
    char closeb[5];
    DESCRIPTOR_DATA *d;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
	/*float  totalpk;*/
    bool fClassRestrict;
    bool fImmortalOnly;
	CHAR_DATA *wch;
    if (IS_NPC(ch)) return;
    /* Set default arguments. */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fImmortalOnly  = FALSE;
    /* Parse arguments. */
    nNumber = 0;
    for ( ;; )
    {
		char arg[MAX_STRING_LENGTH];
		argument = one_argument( argument, arg );
		if ( arg[0] == '\0' )
			break;
		if ( is_number( arg ) )
		{
			return;
		}
		else
		{
	    /* Look for classes to turn on. */
			arg[3]    = '\0';
			if (   !str_cmp( arg, "imm" ) || !str_cmp( arg, "immortal" ) || !str_cmp( arg, "ava" ) || !str_cmp( arg, "avatar"   ) )
			{
				fClassRestrict = TRUE;
			}
			else if ( !str_cmp( arg, "god" ) || !str_cmp( arg, "imp" ) )
			{
				fImmortalOnly = TRUE;
			}
			else
			{
				send_to_char("Enter 'Avatar' for level 3's, or 'God' for level 4's and 5's.\n\r",ch);
				return;
			}
		}
    }

    /*  Now show matching chars. */
    nMatch = 0;
	buf2[0] = '\0';
	bufrogue[0] = '\0';
	bufultimate[0] = '\0';
	bufsupreme[0] = '\0';
	bufgreater[0] = '\0';
	buflesser[0] = '\0';
	bufdemi[0] = '\0';
	bufgodling[0] = '\0';
	bufimmortal[0] = '\0';
	bufavatar[0] = '\0';

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
		
		char const *class;
	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
		if ( (d->connected != CON_PLAYING && d->connected != CON_EDITING) || (!can_see( ch, d->character ) && (!IS_SET(ch->act, PLR_WATCHER)) )  )
			continue;
		wch   = ( d->original != NULL ) ? d->original : d->character;
		if ( wch->level < iLevelLower ||   wch->level > iLevelUpper || ( fImmortalOnly  && wch->level <  LEVEL_IMMORTAL ) || ( fClassRestrict && wch->level != LEVEL_HERO     ) )
			continue;
		if ( (wch->max_hit < 10000) || (wch->level < 3) || (wch->level > 6) )
			continue;
		if ((IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH)) && wch->pcdata->chobj != NULL)
			continue;
		nMatch++;

	/*
	 * Figure out what to print for class.
	 */
		class = " ";
		if ((IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH)) && wch->pcdata->chobj != NULL)
		{
			if (wch->pcdata->chobj->pIndexData->vnum == 12)
	    		class = "A Head       ";
			else if (wch->pcdata->chobj->pIndexData->vnum == 30005)
	    		class = "A Brain      ";
			else
	    		class = "An Object    ";
		}
		else if (!str_cmp(wch->pcdata->switchname, "Sage"))
			class = "#3LoW Founder     ";

		else
			switch ( wch->level )
			{
				default:
					if (wch->race <= 0 )
						class = "#1Avatar#n       ";
					else if (wch->race <= 4 )
						class = "#3Immortal#n     ";
					else if (wch->race <= 9 )
						class = "#4Godling#n      ";
					else if (wch->race <= 14)
						class = "#6DemiGod#n      ";
					else if (wch->race <= 19)
						class = "#0Lesser God#n   ";
					else if (wch->race <= 24)
						class = "#1Greater God#n  ";
					else if (wch->race <= 29)
						class = "#5Supreme God#n  ";
					else if (wch->race <= 34)
						class = "#7Ultimate God#n ";
					else if (wch->race <= 60)
						class = "#6Master   #n    ";
					break;
			}
	/* Format it up. */
			strcpy( openb, "[" );
			strcpy( closeb, "]" ); 
				sprintf( kav, " %sNONE%s",openb,closeb );
		if ((wch->level < 7) && (wch->level > 2) && (wch->race >= 35))
			sprintf( bufrogue + strlen(bufrogue), "%-13s#n %-10s%-20s #2%-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 35) && (wch->race >=30))
			sprintf( bufultimate + strlen(bufultimate), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 30) && (wch->race >= 25))
			sprintf( bufsupreme + strlen(bufsupreme), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 25) && (wch->race >= 20))
			sprintf( bufgreater + strlen(bufgreater), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 20) && (wch->race >= 15))
			sprintf( buflesser + strlen(buflesser), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 15) && (wch->race >= 10))
			sprintf( bufdemi + strlen(bufdemi),     "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 10) && (wch->race >= 5))
			sprintf( bufgodling + strlen(bufgodling), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 5) && (wch->race >= 1))
			sprintf( bufimmortal + strlen(bufimmortal), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
		if ((wch->level < 7) && (wch->level > 2) && (wch->race < 1))
			sprintf( bufavatar + strlen(bufavatar), "%-13s#n %-10s%-20s#2 %-6d wins     %-6d losses\n\r", class, kav, wch->pcdata->switchname, wch->pcdata->awins, wch->pcdata->alosses);
	}
    sprintf(buf2,"The Vortex");
	banner_to_char(buf2,ch);
	send_to_char("Arena Status : ",ch);
	if (arena == FIGHT_OPEN)
		send_to_char("OPEN FOR CHALLENGES\n\r", ch);
	if (arena == FIGHT_START)
		send_to_char("CHALLENGE IN PROGRESS\n\r", ch);
	if (arena == FIGHT_BUSY)
		send_to_char("FIGHT IN PROGRESS\n\r", ch);
	if (arena == FIGHT_LOCK)
		send_to_char("ARENA IS CLOSED\n\r", ch);
	send_to_char( bufrogue,ch );
	send_to_char( bufultimate,ch );
	send_to_char( bufsupreme,ch );
	send_to_char( bufgreater,ch );
	send_to_char( buflesser,ch );
	send_to_char( bufdemi,ch );
	send_to_char( bufgodling,ch );
	send_to_char( bufimmortal,ch );
	send_to_char( bufavatar,ch );
	sprintf(buf2,"Players Challengeable: %d",nMatch);
    banner_to_char(buf2,ch);
    return;
}
