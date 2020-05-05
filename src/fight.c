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
#include "clantable.h"

int stance_mod_apd args(( CHAR_DATA *ch, CHAR_DATA *victim ));
int stance_mod_dam args(( CHAR_DATA *ch, CHAR_DATA *victim, int dam ));
bool compare_stance args(( CHAR_DATA *ch, CHAR_DATA *victim, int ch_stance, int vict_stance ));
void damage       args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void one_hit      args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype) );
void	decap_gain	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int type ) );
int	get_danger	args( ( CHAR_DATA *ch ) );
bool	check_dodge		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	check_parry		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	dam_message		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ) );
void	death_cry		args( ( CHAR_DATA *ch ) );
//void	group_gain		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	xp_compute		args( ( CHAR_DATA *gch, CHAR_DATA *victim ) );
void	set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	number_attacks	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    check_arena       args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    did_hit            args( (CHAR_DATA *ch, CHAR_DATA *victim));
bool  is_gen_dox   args( (CHAR_DATA *ch, CHAR_DATA *victim));
bool  is_status_dox   args( (CHAR_DATA *ch, CHAR_DATA *victim));
bool  is_soul_dox   args( (CHAR_DATA *ch, CHAR_DATA *victim));
bool  is_status_kill   args((CHAR_DATA *ch, CHAR_DATA *victim));
bool   is_gen_kill     args((CHAR_DATA *ch, CHAR_DATA *victim));
bool   is_soul_kill    args((CHAR_DATA *ch, CHAR_DATA *victim)); 
void   check_warp    args(( CHAR_DATA *ch));
void register_kill( CHAR_DATA *ch, CHAR_DATA *victim);
void get_dm_winner(CHAR_DATA *ch);
void check_vkills( CHAR_DATA *ch );
bool check_beast( CHAR_DATA *ch );

void set_stun( CHAR_DATA * victim, int stuntime )
{
	if( victim->stuntimer > 0 )
		return;
	if( victim->position == POS_SLEEPING )
		victim->position = POS_STUNNED;
	if ( stuntime > 2 )
		stuntime = 2;
	victim->stuntimer += stuntime;
        if( victim->stuntimer > 3 )
         victim->stuntimer = 3;
        update_pos(victim);
	return;
}

bool in_arena(CHAR_DATA *ch)
{
  if( !ch)
   return FALSE;
  if( !ch->in_room)
   return FALSE;
  if( (ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
   return TRUE;
  else
   return FALSE;
}

void check_warp(CHAR_DATA *ch)
{
 int warp;

 if( ch->warpcount >= 6)
  return;
 warp = get_warp(ch);
 SET_BIT(ch->warp, warp);
 ch->warpcount++;
 stc("You have been mutated by the chaotic forces that be.\n\r", ch);
 return; 
}

int get_warp(CHAR_DATA *ch)
{
 int value = number_range(1,MAX_WARP);
 int warp; 

 switch(value)
 {
  case 1:
   warp = WARP_CRYSTAL;
   break;
  case 2:
   warp = WARP_STEEL;
   break;
  case 3:
   warp = WARP_VENOM;
   break;
  case 4:
   warp = WARP_SPIKE;
   break;
  case 5:
   warp = WARP_QUICK;
   break;
  case 6:
   warp = WARP_SLOW;
   break;
  case 7:
   warp = WARP_TERROR;
   break;
  case 8:
   warp = WARP_REGEN;
   break;
  case 9:
   warp = WARP_WEAPON;
   break;
  case 10:
   warp = WARP_INEPT;
   break;
  case 11:
   warp = WARP_WEAK;
   break;
  case 12:
   warp = WARP_VULN;
   break;
  case 13:
   warp = WARP_SHARDS;
   break;
  case 14:
   warp = WARP_CLUMSY;
   break;
  case 15:
   warp = WARP_TENT;
   break;
  case 16:
   warp = WARP_SLOWHEAL;
   break;
  case 17:
   warp = WARP_ARCANE;
   break;
  case 18:
   warp = WARP_NOMAGE;
   break;
  case 19:
   warp = WARP_STRONG;
   break;
  case 20:
   warp = WARP_SMELLY;
   break;
  case 21:
   warp = WARP_WART;
    break;
  case 22:
   warp = WARP_FEATHER;
   break;
  case 23:
   warp = WARP_HAIRY;
   break;
  case 24:
   warp = WARP_BIGFEET;
   break;
  case 25:
   warp = WARP_BALD;
   break;
  case 26:
   warp = WARP_TAIL;
   break;
  default:
   warp = 0;
   break;
 }
 if( warp == 0 )
 {
  bug("Invalid warp type: get_warp.", 0);
  return 0;
 }
 if( !is_valid_warp(ch, warp))
  warp = get_warp(ch);

 return warp;
}

bool is_valid_warp(CHAR_DATA *ch, int warp)
{
 bool ret_val = TRUE;

 if( IS_SET(ch->warp, warp))
  return FALSE;
 if( warp == WARP_CRYSTAL)
 {
  if(IS_SET(ch->warp, WARP_STEEL))
   ret_val = FALSE;
 }
 if( warp == WARP_STEEL)
 {
  if(IS_SET(ch->warp, WARP_CRYSTAL))
   ret_val = FALSE;
 }
 if( warp == WARP_QUICK)
 {
  if( IS_SET(ch->warp, WARP_SLOW))
   ret_val = FALSE;
 }
 if( warp == WARP_SLOW)
 {
  if( IS_SET(ch->warp, WARP_QUICK))
   ret_val = FALSE;
 }
 if( warp == WARP_REGEN)
 {
  if( IS_SET(ch->warp, WARP_SLOWHEAL))
   ret_val = FALSE;
 }
 if( warp == WARP_SLOWHEAL)
 {
  if( IS_SET(ch->warp, WARP_REGEN))
  ret_val = FALSE;
 }
 if( warp == WARP_WEAPON)
 {
  if( IS_SET(ch->warp, WARP_INEPT))
   ret_val = FALSE;
 }
 if( warp == WARP_INEPT)
 {
  if( IS_SET(ch->warp, WARP_WEAPON))
   ret_val = FALSE;
 }
 if( warp == WARP_WEAK)
 {
  if( IS_SET(ch->warp, WARP_STRONG))
   ret_val = FALSE;
 }
 if( warp == WARP_STRONG)
 {
  if( IS_SET(ch->warp, WARP_WEAK))
   ret_val = FALSE;
 }
 if( warp == WARP_ARCANE)
 {
  if( IS_SET(ch->warp, WARP_NOMAGE))
   ret_val = FALSE;
 }
 if( warp == WARP_NOMAGE)
 {
  if( IS_SET(ch->warp, WARP_ARCANE))
   ret_val = FALSE;
 }
 
 return ret_val;
}  

void get_dm_winner(CHAR_DATA *ch)
{
  int ava_count = 0;
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;

  for( d = descriptor_list; d != NULL; d = d->next )
  {
	if( d->character == NULL )
	{
	   log_string("Bug: NULL descriptor");
	   sprintf(buf, "#rThe #1Deathmatch #rhas ended with no victor.#n\n\r");
	   do_info(ch, buf);
	   sprintf(buf, "Report #1NULL descriptor bug #nto Loimos or Kline.");
	   do_info(ch, buf);
           if( IS_SET(d->character->act,PLR_WAS_AVATAR) )
           {
            REMOVE_BIT(d->character->act,PLR_WAS_AVATAR);
            if( !str_cmp(ch->clan,"DAMNED") )
            {
             d->character->trust = 4;
             d->character->level = 4;
             stc("Your soul has returned to the realm of the damned.\n\r",d->character);
            }
            else
            {
             d->character->trust = 3;
             d->character->level = 3;
             stc("Your immortal status has been restored.\n\r",d->character);
            }
           }
	   deathmatch = FALSE;
	   return;
	}
	if( d->character->level >= 3 && d->character->level <= 4 )
		ava_count++;

  }
  perfect_diamond(ch);
  if( ava_count > 1 )
  {
	sprintf(buf, "#1%d players #rremain in the #1Deathmatch!#n\n\r", ava_count);
	do_info(ch, buf);
	return;
  }
  if( ava_count == 1 )
  {
	ch->hit = ch->max_hit;
	ch->mana = ch->max_mana;
	ch->move = ch->max_move;
	ch->fight_timer = 0;
	ch->level = 2;
	ch->trust = 2;
	stc("You have been rewarded by th Gods!\n\r", ch);
	act("$n has been rewarded by the Gods!\n\r", ch, NULL, NULL, TO_ROOM);
	do_make_eq(ch, "DMWINNER");
	deathmatch = FALSE;
 	sprintf(buf, "#1%s #rhas won the #1Deathmatch!#n\n\r", ch->name);
	do_info(ch, buf);
        sprintf(buf,"#7The immortal status of all participants has been restored.#n\n\r");
        do_info(ch,buf);
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
           stc("Your soul has returned to the realm of the damned.\n\r",d->character);
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
  	sprintf(buf, "#rThe #1Deathmatch #rhas ended with no victor.#n\n\r");
	do_info(ch, buf);
	deathmatch = FALSE;
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
           stc("Your soul has returned to the realm of the damned.\n\r",d->character);
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
}		
  
void do_stealsoul(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
   
  argument = one_argument(argument, arg);
  
  if( IS_NPC(ch) )
   return;
  if( ch->level < 3)
  {
   stc("You must be an avatar.\n\r", ch);
   return;
  }
  if( IS_DAMNED(ch) && !deathmatch ) return;

  if( arg[0] == '\0')
  {
   stc("Absorb who?\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  if( IS_NPC(victim))
  {
   stc("Not on NPCs.\n\r", ch);
   return;
  }
  if( in_arena(ch))
  {
   stc("Not while in the arena.\n\r", ch);
   return;
  }
  if( victim->level < 3)
  {
   stc("You can only stealsoul avatars.\n\r", ch);
   return;
  }
  if( victim->hit >= 0)
  {
   stc("You victim must be mortally wounded to stealsoul them.\n\r", ch);
   return;
  }
  if( nogroup )
  {
   if( !deathmatch )
   {
    if( IS_SET(ch->act,PLR_NO_GROUP) && IS_SET(victim->act,PLR_NO_GROUP) )
    {
     if( victim->pcdata->target != NULL && str_cmp(ch->pcdata->switchname,victim->pcdata->target) )   
     {
      stc("Kill your own victims.\n\r",ch);
      return;
     }
    }
    if( IS_SET(ch->act,PLR_NO_GROUP) && !IS_SET(victim->act,PLR_NO_GROUP) )
    {  
     stc("Groupers can't kill solo fighters.\n\r",ch);
     return;
    }
    if( !IS_SET(ch->act,PLR_NO_GROUP) && IS_SET(victim->act,PLR_NO_GROUP) ) 
    {
     stc("Solo fighters can't kill groupers.\n\r",ch);     
     return;
    }
   }
  }
  decap_gain(ch,victim,3);
  return;
}

void do_absorb(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  
  argument = one_argument(argument, arg);
  
  if( IS_NPC(ch) )
   return;
  if( ch->level < 3)
  {
   stc("You must be an avatar.\n\r", ch);
   return;
  }
  if( IS_DAMNED(ch) && !deathmatch ) return;

  if( arg[0] == '\0')
  {
   stc("Absorb who?\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  if( IS_NPC(victim))
  {
   stc("Not on NPCs.\n\r", ch);
   return;
  }
  if( in_arena(ch))
  {
   stc("Not while in the arena.\n\r", ch);
   return;
  }
  if( victim->level < 3)
  {
   stc("You can only absorb avatars.\n\r", ch);
   return;
  }
  if( victim->hit >= 0)
  {
   stc("You victim must be mortally wounded to absorb them.\n\r", ch);
   return;
  }
  if( nogroup )
  {
   if( !deathmatch )
   {
    if( IS_SET(ch->act,PLR_NO_GROUP) && IS_SET(victim->act,PLR_NO_GROUP) )
    {
     if( victim->pcdata->target != NULL && str_cmp(ch->pcdata->switchname,victim->pcdata->target) )   
     {
      stc("Kill your own victims.\n\r",ch);
      return;
     }
    }
    if( IS_SET(ch->act,PLR_NO_GROUP) && !IS_SET(victim->act,PLR_NO_GROUP) )
    {  
     stc("Groupers can't kill solo fighters.\n\r",ch);
     return;
    }
    if( !IS_SET(ch->act,PLR_NO_GROUP) && IS_SET(victim->act,PLR_NO_GROUP) ) 
    {
     stc("Solo fighters can't kill groupers.\n\r",ch);     
     return;
    }
   }
  }
  decap_gain(ch,victim,1);
  return;
}

void do_decapitate(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
   
  argument = one_argument(argument, arg);
  
  if( IS_NPC(ch) )
   return;
  if( ch->level < 3)
  {
   stc("You must be an avatar.\n\r", ch);
   return;
  }
  if( arg[0] == '\0')
  {
   stc("Decapitate who?\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  if( IS_NPC(victim))
  {
   stc("Not on NPCs.\n\r", ch);
   return;
  }
  if( in_arena(ch))
  {
   stc("Not while in the arena.\n\r", ch);
   return;
  }
  if( victim->level < 3)
  {
   stc("You can only decapitate avatars.\n\r", ch);
   return;
  }
  if( (IS_DAMNED(ch) || IS_DAMNED(victim)) && !deathmatch )
  {
   if( IS_DAMNED(ch) && !IS_DAMNED(victim)  )
    stc("A spirit of the damned can not do that.\n\r",ch);
   else if( !IS_DAMNED(ch) && IS_DAMNED(victim) )
    stc("A spirit of the damned can not be killed in such a manner.\n\r",ch);
   else if( IS_DAMNED(ch) && IS_DAMNED(victim) )
    stc("Spirits of the damned can not kill one another in such a manner.\n\r",ch);
   else
    stc("This shouldn't be happening...Tell Kline IMMEDIATELY.\n\r",ch);
   return;
  }
  if( victim->hit >= 0)
  {
   stc("You victim must be mortally wounded to decapitate them.\n\r", ch);
   return;
  }
  if( nogroup )
  {
   if( !deathmatch )
   {
    if( IS_SET(ch->act,PLR_NO_GROUP) && IS_SET(victim->act,PLR_NO_GROUP) )
    {
     if( victim->pcdata->target != NULL && str_cmp(ch->pcdata->switchname,victim->pcdata->target) )
     {
      stc("Kill your own victims.\n\r",ch);
      return;
     }
    }
    if( IS_SET(ch->act,PLR_NO_GROUP) && !IS_SET(victim->act,PLR_NO_GROUP) )
    {
     stc("Groupers can't kill solo fighters.\n\r",ch);
     return;
    }
    if( !IS_SET(ch->act,PLR_NO_GROUP) && IS_SET(victim->act,PLR_NO_GROUP) )
    {  
     stc("Solo fighters can't kill groupers.\n\r",ch);
     return;
    }
   }
  }
  decap_gain(ch,victim,2);
  return;
}

bool is_gen_dox( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( victim->pkill >= 5 )
   return FALSE;
 if( victim->generation == 1 && ch->generation > 1 )
   return TRUE;
 if( is_gen_kill(ch,victim) )
   return FALSE;
 return FALSE;
}

bool is_status_dox( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( victim->pkill >= 5 )
  return FALSE;
 if( victim->race == 0 && ch->race > 5 )
  return TRUE;
 if( is_status_kill(ch,victim) )
  return FALSE;
 return FALSE;
}

bool is_soul_dox( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( victim->pkill >= 5 )
  return FALSE;
 if( victim->pcdata->souls == 0 && ch->pcdata->souls > 5 )
  return TRUE;
 if( is_soul_kill(ch,victim) )
  return FALSE;
 return FALSE;
}

bool is_status_kill( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( victim->race == 0 && ch->race > 5 )
  return FALSE;
 if( victim->race >= ch->race )
  return TRUE;
 if( ch->race > victim->race )
 {
  if( (ch->race - victim->race) > 10 )
   return FALSE;
  else
   return TRUE;
 }
 return TRUE;
}

bool is_soul_kill( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( victim->pcdata->souls == 0 && ch->pcdata->souls > 5 )
  return FALSE;
 if( victim->pcdata->souls >= ch->pcdata->souls )
  return TRUE;
 if( ch->pcdata->souls > victim->pcdata->souls )
 {
  if( (ch->pcdata->souls - victim->pcdata->souls) > 10 )
   return FALSE;
  else
    return TRUE;
 }
 return TRUE;
}

bool is_gen_kill( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( victim->generation >= ch->generation )
  return TRUE;
 else
  return FALSE;
 return TRUE;
}
void violence_update( void )
{
	CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    CHAR_DATA *rch_next;
    CHAR_DATA *mount;
    int i;
    void (*handle)();
    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
		ch_next	= ch->next;
		if (ch->fight_timer > 0 )
		{
		 ch->fight_timer--;
		 if( IS_SET(ch->act,PLR_CHALLENGED) )
		  do_decline(ch,"");
		}
		if( IS_SET(ch->affected_by, AFF_ETHEREAL) && !IS_NPC(ch) )
		{
		   ch->mana -= 750;
		   if( ch->mana < 750 )
		   {
			REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
			stc("You return to your normal form, leaving the ethereal plane.\n\r", ch);
			act("$n returns to $s normal form, leaving the ethereal plane.", ch, NULL, NULL, TO_ROOM);
		   }
		}
                if( ch->login_timer > 0)
                        ch->login_timer--;
		if( ch->stuntimer > 0)
			ch->stuntimer--;       
		if( !IS_NPC(ch) && ch->pcdata->target != NULL && ch->hit > 0 && ch->fight_timer < 10 )
		 ch->pcdata->target = NULL;
		if( !IS_NPC(ch) && ch->position == POS_FIGHTING && ch->fighting == NULL )
		 stop_fighting(ch,TRUE);
		if( !IS_NPC(ch) && ch->position == POS_FIGHTING && ch->hit < 0 )
		 stop_fighting(ch,TRUE);
        class_regen(ch);

        if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_JEDI) )
        {
         if( (handle = dlsym(jedilib,"jedi_force_upkeep")) != NULL )
          (*handle)(ch);
         else
          log_string("violence_update: can't find jedi_force_upkeep");
        }
        if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK))
        {
         if( GET_POWERS(ch, MONK_CURR_CHI) > 0)
         {
          ch->move -= (number_range(25, 100) * GET_POWERS(ch, MONK_CURR_CHI));
          if( ch->move <= 0)
          {
           GET_POWERS(ch, MONK_CURR_CHI) = 0;
           ch->move = 0;
          }
         }
         if( MONK_SET(ch, MONK_CUST))
         {  
          ch->move -= number_range( 25, 100);
          if( ch->move <= 0)
          {
           REMOVE_BIT( GET_POWERS(ch, MONK_BITS), MONK_CUST);
           ch->move = 0;
          }
         }
        }
       if( !IS_NPC(ch) && IS_HERO(ch) && ch->level != 4 && ch->trust != 4 && IS_CLASS(ch,CLASS_DRAGON) )
       {
        if( GET_POWERS(ch, DRAGON_AGE) < 10000
         && GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON
	 && !IS_SET(ch->in_room->room_flags, ROOM_SAFE)
         && !IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
	 && ch->desc != NULL && ch->wimpy == 0 && ch->hit > 0 
	 && !IS_EXTRA(ch, TIED_UP) && !IS_EXTRA(ch, GAGGED)
	 && !IS_EXTRA(ch, BLINDFOLDED) && !deathmatch && ch->timer < 3)
        {
         GET_POWERS(ch, DRAGON_DAYS)++;   
	 class_attributes(ch);         
         if( GET_POWERS(ch, DRAGON_DAYS) == 30)
         {
          stc("You are one dragon year older.\n\r", ch);
          GET_POWERS(ch, DRAGON_DAYS) = 0;
          GET_POWERS(ch, DRAGON_AGE)++;
         }
        }
        if( GET_POWERS( ch, DRAGON_BTIME) > 0)
        {
         GET_POWERS(ch, DRAGON_BTIME)--;
         if( GET_POWERS(ch, DRAGON_BTIME) == 0)
          stc("You can use your breath weapon again.\n\r", ch);
        }
        if( GET_POWERS( ch, DRAGON_STIME) > 0)
        {
         GET_POWERS(ch, DRAGON_STIME)--;
         if( GET_POWERS(ch, DRAGON_STIME) == 0)
          stc("You can shift your form again.\n\r", ch);
        }
       }
        for( i = 0; i < MAX_PULSE_TIMER; i++)
        {
         if( IS_NPC(ch))
          continue;
         if(!IS_NPC(ch) &&  ch->pcdata->pulse_timer[i] < 1)
          continue;
         if( i == PULSE_HEALTOUCH)
         {
          if( !IS_HERO(ch))
          {
           ch->pcdata->pulse_timer[i] = 0;
           continue;
          }
          ch->hit += number_range( 300, 400);
          stc("Your lifeforce restores itself.\n\r", ch);
          update_pos(ch);
         }
         if( i == PULSE_HEATMETAL)
         {
          OBJ_DATA *obj;
          OBJ_DATA *obj_next;
          int dam = number_range(200,500);

          if( !IS_HERO(ch))
          {
           ch->pcdata->pulse_timer[i] = 0;
           continue;
          }
          for ( obj = ch->carrying; obj != NULL; obj = obj_next )
          {
            obj_next = obj->next_content;
           if( number_range(1,100) < 25)
           {
            if( obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ) )
              remove_obj( ch, obj->wear_loc, TRUE );
           }
          }
          act2("Your equipment is scorching you.[$D]",ch, NULL, ch, TO_CHAR, dam);
          act2("$n's equipment is scorching them.[$D]", ch, NULL, ch, TO_NOTVICT, dam); 
         } 
         if( i == PULSE_DEATHTOUCH)
         {
          if( !IS_HERO(ch))
          {
           ch->pcdata->pulse_timer[i] = 0;
           continue;
          }
          ch->hit -= number_range(700, 1000);
          stc("Your lifeforce is diminishing.\n\r", ch);
          update_pos(ch);
         }
         if( i == PULSE_BEAR)
         {
          if( ch->pcdata->pulse_timer[i] == 1)
          {
           stc("The spirit of the bear has left you.\n\r", ch);
           act("The spirit of the bear has left $n.", ch, NULL, NULL, TO_ROOM);
          }
         } 
         if( i == PULSE_STASIS) 
         {
          if( ch->stasis == FALSE)
           continue;
          if( ch->pcdata->pulse_timer[i] == 1)
          {
           ch->stasis = FALSE;
           stc("You have come out of stasis.\n\r", ch);
           act("$n has come out of stasis.", ch, NULL, NULL, TO_ROOM);
          }
         }
         if( i == PULSE_SHADOWPLANE)
         {
          if( !IS_AFFECTED(ch,AFF_SHADOWPLANE))
           continue;
          if( ch->pcdata->pulse_timer[i] == 1)
          {
           REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
           stc("You have come out of the shadowplane.\n\r", ch);
           act("$n has come out of the shadowplane.\n\r", ch, NULL, NULL, TO_ROOM);
          }
         }
         if( i == PULSE_DARKNESS)
         {
          if( !IS_SET(ch->newbits,NEW_DARKNESS))
           continue;
          if( ch->pcdata->pulse_timer[i] == 1)
          {
           REMOVE_BIT(ch->newbits,NEW_DARKNESS);
           if( ch->in_room != NULL && IS_SET(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS) )
            REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
           stc("You banish your globe of darkness.\n\r",ch);
           act("The globe of darkness around $n disappears.\n\r", ch, NULL, NULL, TO_ROOM);
          }
         }	  
	 if( i == PULSE_GRAB )
	 {
	   if( ch->pcdata->pulse_timer[i] == 1 
	   &&  ch->pcdata->held != NULL
	   &&  ch->pcdata->held != '\0' )
		do_drelease(ch, ch->pcdata->held);
	 }
         ch->pcdata->pulse_timer[i]--;
        }

	if( !IS_NPC(ch) && ch->in_room != NULL && IS_SET(ch->in_room->affected_by, AFF_ROOM_DRAIN) )
	{
	 CHAR_DATA *vch;
	 int z = 0;
	 for( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
 	 {
	  if( IS_NPC(vch) || safe_room(vch) || ch == vch )
	   continue;
          else
  	   z++;
	 }
	 if( !str_cmp(ch->in_room->rune_by, ch->name) && z > 1 && ch->hit >= 1000 )
	 {
	  if( ch->hit < ch->max_hit )
	   ch->hit += ( 100 * (z-1) );
	  if( ch->mana < ch->max_mana )
	   ch->mana += ( 100 * (z-1) );
	  if( ch->move < ch->max_move )
	   ch->move += ( 100 * (z-1) );
	  stc("You feel rejuvinated by the lifeforce of your enemies.\n\r", ch);
	 }
	 else if( str_cmp(ch->in_room->rune_by, ch->name) )
	 {
	  ch->hit -= 300;
	  ch->mana -= 300;
	  ch->move -= 300;
	  stc("You feel your lifeforce being slowly drained away...\n\r", ch);
	 }
	 else
	  continue;
	}
	if( !IS_NPC(ch) && IS_SET(ch->affected_by,AFF_ETHEREAL) )
	{
         if( IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,VAMP_FORM) == FORM_MIST )
          continue;
	 ch->mana -= 1500;
	 if( ch->mana < 1500 )
	  do_ethereal(ch,"");
	}
        if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ALIEN))
        {
         if( GET_POWERS(ch, ALIEN_TIMER) > 0)
         {
          if( GET_POWERS(ch, ALIEN_TIMER) == 1)
           stc("You have finished your metamorphisis.\n\r", ch);
          DEC_POWERS(ch, ALIEN_TIMER);
         }
         if( GET_POWERS(ch, ALIEN_PRIMAL) > 0)
          if( number_percent() < 5)
          {
           stc("Your primal urges have calmed.\n\r", ch);
           DEC_POWERS(ch, ALIEN_PRIMAL);
          }
         if( GET_POWERS(ch, ALIEN_PAIN) > 0)
          if( number_percent() < 5)
          {
           stc("Your tolerance to pain has begun to fade.\n\r", ch);
           DEC_POWERS(ch, ALIEN_PAIN);
          }
         if( IS_SET(GET_POWERS(ch, ALIEN_BITS), ALIEN_CLOAK))
         {
          ch->mana -= 1000;
          if( ch->mana < 1000)
          {
           stc("Your cloaking field fades.\n\r", ch);
           act("$n suddenly appears from a cloaking field.", ch, NULL, NULL, TO_NOTVICT);
           REM_BIT_POWERS(ch, ALIEN_BITS, ALIEN_CLOAK);
          }
	 }
        }
	 if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_SHADOW) )
	 {
	  if( IS_SET(GET_POWERS(ch,SHADOW_BITS),SHADOW_INVIS) )
	  {
	   ch->mana -= 2000;
	   if( ch->mana < 2000 )
	    conceal_count(ch);
	  }
	 }
        if( IS_NPC(ch) && (ch->hit < -10))
            ch->hit = 1;
         if( ch->rage > 0)
         ch->rage -= number_range( 1,3);
	if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WOLF) )
	{
	 if( ch->rage > 99 )
	  do_rage_up(ch,"");
	 if( ch->rage < 100 )
	  do_rage_down(ch,"");
         if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_GONE) && ch->fighting == NULL )
         {
          if( number_range(1,100) < (GET_POWERS(ch,DISC_HAWK)*2) )
          stc("#7Your form slowly flickers back into visibility!#n\n\r",ch);
          REM_BIT_POWERS(ch,WOLF_BITS,BIT_GONE);
          act("#7$n's form slowly flickers into visiblity!#n",ch,NULL,NULL,TO_ROOM);
         }
	}
        if(ch->rage < 0)
         ch->rage = 0;
       if ( IS_SET(ch->newbits, NEW_DARKNESS) && ch->hit < 1 )
       {
	REMOVE_BIT(ch->newbits, NEW_DARKNESS);
	 if( IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
	  REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
	stc("You banish your globe of darkness.\n\r", ch );
	act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
       }
	if( is_affected(ch, skill_lookup("contagion")))
        {
         ch->hit -= number_range(500,1000);
         update_pos(ch);
         stc("Your disease causes open sores to appear on your skin.\n\r", ch);
         if( IS_NPC(ch) && ch->hit <= 0 )
		raw_kill(ch);
        }
        if( is_affected(ch, skill_lookup("poison")))
        {
         ch->hit -= number_range(500,1000);
         update_pos(ch);
         stc("Poison courses throug your veins killing you slowly.\n\r", ch);
	 if( IS_NPC(ch) && ch->hit <= 0 )
		raw_kill(ch);
        }
	if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
		    continue;
	if (!IS_NPC(ch) && !IS_NPC(victim))
		ch->fight_timer = 10;

		if ( IS_AWAKE(ch) && IS_AWAKE(victim) && ch->in_room == victim->in_room ) 
			multi_hit( ch, victim, TYPE_UNDEFINED );
		else
		    stop_fighting( ch, FALSE );
		if ( ( victim = ch->fighting ) == NULL )
		    continue;
		for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
		{
		    rch_next = rch->next_in_room;
		    if ( IS_AWAKE(rch) && rch->fighting == NULL )
		    {
				/* Mount's auto-assist their riders and vice versa.*/
				if ( (mount = rch->mount) != NULL )
				{
					if (mount == ch) multi_hit( rch, victim, TYPE_UNDEFINED );
						continue;
				}
		
				/* PC's auto-assist others in their group.*/
				if ( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) )
				{
			    if ( ( !IS_NPC(rch) || IS_AFFECTED(rch, AFF_CHARM) ) && is_same_group(ch, rch) )
					multi_hit( rch, victim, TYPE_UNDEFINED );
					continue;
				}
				/* NPC's assist NPC's of same type or 12.5% chance regardless.*/
				if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) )
				{
					if ( rch->pIndexData == ch->pIndexData ||   number_bits( 3 ) == 0 )
					{
						CHAR_DATA *vch;
						CHAR_DATA *target;
						int number;
						target = NULL;
						number = 0;
						for ( vch = ch->in_room->people; vch; vch = vch->next )
						{
							if ( can_see( rch, vch ) && is_same_group( vch, victim ) && number_range( 0, number ) == 0 )
							{
								target = vch;
								number++;
							}
						}
					if ( target != NULL )
				    multi_hit( rch, target, TYPE_UNDEFINED );
					} 
				}/* End of if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) ) */
			} /* End of if ( IS_AWAKE(rch) && rch->fighting == NULL )*/
		}/*End of for ( rch = ch->in_room->people; rch != NULL; rch = rch_next ) */
    }/* End of for ( ch = char_list; ch != NULL; ch = ch->next )*/
    return;
}
/* much simpler method to decide if a player hits or not.
   This should always give at least a 15% chance to continue
   on to the dodge and parry functions. -Kale
*/
bool did_hit(CHAR_DATA *ch, CHAR_DATA *victim)
{
 int chance;
 int level1, level2;
 int diff;
 bool ret_val = FALSE;

 level1 = ((class_hitroll(ch)/100)+(class_ac(ch)/ -100));
 level2 = ((class_hitroll(victim)/100) + (class_ac(victim)/-100));
 if( IS_NPC(ch))
  level1 /= 1.5;
 if( IS_NPC(victim))
  level2 /= 1.5;
 diff = level1 - level2;
 if( diff < -20)
  diff = -20;
 if( diff > 20)
  diff = 20;
 switch(diff)
 {
  case -20:
   chance = 30;
   break;
  case -19:
   chance = 32;
   break;
  case -18:
   chance = 34;
   break;
  case -17:
   chance = 36;
   break;
  case -16:
   chance = 38;
   break;
  case -15:
   chance = 40;
   break;
  case -14:
   chance = 42;
   break;
  case -13:
   chance = 44;
   break;
  case -12:
   chance = 46;
   break;
  case -11:
   chance = 48;
   break;
  case -10:
   chance = 50;
   break;
  case -9:
   chance = 52;
   break;
  case -8:
   chance = 54;
   break;
  case -7:
   chance = 56;
   break;
  case -6:
   chance = 58;
   break;
  case -5:
   chance = 60;
   break;
  case -4:
   chance = 62;
   break;
  case -3:
   chance = 64;
   break;
  case -2:
   chance = 66;
   break;
  case -1:
   chance = 68;
   break;
  case 0:
   chance = 70;
   break;
  case 1:
   chance = 71;
   break;
  case 2:
   chance = 72;
   break;
  case 3:
   chance = 73;
   break;
  case 4:
   chance = 74;
   break;
  case 5:
   chance = 75;
   break;
  case 6:
   chance = 76;
   break;
  case 7:
   chance = 77;
   break;
  case 8:
   chance = 78;
   break;
  case 9:
   chance = 79;
   break;
  case 10:
   chance = 80;
   break;
  case 11:
   chance = 81;
   break;
  case 12:
   chance = 82;
   break;
  case 13:
   chance = 83;
   break;
  case 14:
   chance = 84;
   break;
  case 15:
   chance = 85;
   break;
  case 16:
   chance = 86;
   break;
  case 17:
   chance = 87;
   break;
  case 18:
   chance = 88;
   break;
  case 19:
   chance = 89;
   break;
  case 20:
   chance = 90;
   break;
  default:
   chance = 50;
   break;
 }
 if( number_range(1,100) < chance)
  ret_val = TRUE;
 if( ret_val == TRUE)
  if( !IS_NPC(victim) && IS_CLASS(ch, CLASS_MONK))
 {
  if( (GET_POWERS(ch, MONK_BLOCK) >= number_range(1, MAX_MONK_BLOCK))
     && (number_range(1, 12) < GET_POWERS(ch, MONK_CURR_CHI)) )
  {
   if(IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
    act("#7You block $N's attack.#n", ch, NULL, victim, TO_CHAR);
   if(IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
    act("$n blocks your attack.", ch, NULL, victim, TO_VICT);
   ret_val = FALSE;
  }
 }
return ret_val;
}

void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *wield;
    OBJ_DATA *wield1;
    OBJ_DATA *wield2;
    OBJ_DATA *wield3;
    OBJ_DATA *wield4;
    int sn;
    int level;
    int chance;
    int unarmed;
    int wieldorig = 0;
    int wieldtype = 0;
    int maxcount;
    int countup;
    int wieldie;
      
    if( ch->position < POS_SLEEPING )
      return;
    
    if( !IS_NPC(ch))
    {
     if( IS_SET(ch->warp, WARP_TERROR) && (number_range(1,100) < 10))
      do_flee(victim, "");
    }
    if( dt == gsn_headbutt )
    {
     one_hit( ch, victim, dt, 1 );
     return;
    }  
    if( dt == gsn_fangs )
    {
     one_hit( ch, victim, dt, 1 );
     return;
    }
    if( dt==gsn_venomtong)
    {
     one_hit(ch,victim,dt,1);
     return;
    }
    if( dt == gsn_cattack && !IS_NPC(ch) && !IS_NPC(victim))
    {
     one_hit(ch, victim, dt, 1);
     return;
    }
    if( dt==gsn_tentacle)
    {
     one_hit(ch,victim,dt,1);
     return;
    }
    if( dt == gsn_venom)
    {
     one_hit(ch, victim,dt, 1);
     return;
    }
    if( dt==gsn_spiketail)
    {
     one_hit(ch,victim,dt,1); 
     return;
    }
    if( dt==gsn_badbreath)
    {
     one_hit(ch,victim,dt,1);
     return;
    }
    if( dt==gsn_magma)  
    { 
     one_hit(ch,victim,dt,1);
     return;
    }  
    if( dt==gsn_shards)
    {
     one_hit(ch,victim,dt,1);
     return;
    }  
    if( dt == gsn_buffet )
    {
     one_hit( ch, victim, dt, 1 );
     return;
    }
    if( dt == gsn_sweep )
    {
     one_hit( ch, victim, dt, 1 );
     return;
    }
    if( dt == gsn_rfangs )
    {
     one_hit( ch, victim, dt, 1);
     return;
    }
       
    if (!IS_NPC(ch) && IS_ITEMAFF(ch, ITEMA_PEACE))
    {
     stc( "You are unable to attack them.\n\r", ch );
     return;
    }
    if (!IS_NPC(victim) && IS_ITEMAFF(victim, ITEMA_PEACE))
    {
     stc( "You can't seem to attack them.\n\r", ch );
     return;
    }
    wield1 = get_eq_char( ch, WEAR_WIELD );
    wield2 = get_eq_char( ch, WEAR_HOLD );
    wield3 = get_eq_char( ch, WEAR_THIRD );
    wield4 = get_eq_char( ch, WEAR_FOURTH);
    if( wield1 != NULL && wield1->item_type == ITEM_WEAPON )
                wieldorig  = 1;
    if ( wield2 != NULL && wield2->item_type == ITEM_WEAPON )
                wieldorig += 2;
    if ( wield3 != NULL && wield3->item_type == ITEM_WEAPON )
                wieldorig += 4;
    if ( wield4 != NULL && wield4->item_type == ITEM_WEAPON )
                wieldorig += 8;
    wieldtype = wieldorig;
    wieldie = number_range(1,4);
    if( wieldorig == 15 ) /* all hands */ 
    {
      if(wieldie == 1)
        wieldtype = 8;
      else if(wieldie == 2)
         wieldtype = 4;
      else if(wieldie == 3)
        wieldtype = 2;
      else
        wieldtype = 1; 
    }
    if( wieldorig == 14 ) /* 2 extra arms and one normal hand */
    {
      if(wieldie == 2)
         wieldtype = 8;
      else if (wieldie == 1)
         wieldtype = 4;
      else
         wieldtype = 2;
    }
    if( wieldorig == 13 ) /* 2 extra and normal */
    {
     if( wieldie == 1)
       wieldtype = 8;
     else if(wieldie == 2)
       wieldtype = 4;
     else 
       wieldtype = 1;
    }
    if ( wieldorig == 12 ) /* just the 2 extras */
    {
      if( (wieldie == 1) || (wieldie == 2) )
       wieldtype = 8;
      else
       wieldtype = 4;
    }
    if( wieldorig == 11 ) /* one extra 2 normal */
    {
      if( wieldie == 1 )   
        wieldtype = 8;
      if( wieldie == 2 )
        wieldtype = 2;
      else 
        wieldtype = 1;
    }
    if( wieldorig == 10 ) /*one extra, 1 normal*/
    {
      if( wieldie < 3 )   
        wieldtype = 8;
      else 
        wieldtype = 2;
    }
    if( wieldorig == 9 ) /*one extra, 1 normal*/
    {
      if( wieldie < 3)
        wieldtype = 8;
      else 
        wieldtype = 1;   
    }
    if( wieldorig == 8 ) /* just one extra arm */
        wieldtype = 8;
    if( wieldorig == 7 ) /* one extra, 2 normal */
    {
      if( wieldie == 1 )
        wieldtype = 4;
      else if( wieldie == 2 )
        wieldtype = 2;
      else 
        wieldtype = 1;   
    }
    if( wieldorig == 6 ) /*one extra on normal*/ 
    {
      if( wieldie == 1 || wieldie == 2)
        wieldtype = 2;
      else 
        wieldtype = 4;
    }
    if( wieldorig == 5 ) /* one extra other normal */
    {
      if( wieldie < 3)
       wieldtype = 4;
      else 
       wieldtype = 1;
    }
    if( wieldorig == 4 ) /* one normal */
       wieldtype = 4;
    if( wieldorig == 3 ) /* 2 normal hands */
    {
     if( wieldie == 2 || wieldie == 4)
       wieldtype = 2;
     else 
       wieldtype = 1;
    }
    if( wieldorig == 2 ) /* one normal hand */
       wieldtype = 2;
    if( wieldorig == 1 ) /* other normal hand */
       wieldtype = 1;
                
    if( wieldtype == 8 )
       wield = wield4;
    else if( wieldtype == 4 )
       wield = wield3;
    else if( wieldtype == 2 )
       wield = wield2;
    else
       wield = wield1;                                                     
    if (!IS_NPC(ch) && ch->stance[0] > 0 && number_percent() == 1)
    {
     int stance = ch->stance[0];
     if (ch->stance[stance] >= 200)
     {
      special_move(ch,victim);
      return;
     }
    }
    unarmed = 0;
        one_hit( ch, victim, dt, wieldtype );
     
    if( victim == NULL || victim->position != POS_FIGHTING )
      return;
        
    if ( dt == TYPE_UNDEFINED )
    {
     dt = TYPE_HIT;
     if ( wield != NULL && wield->item_type == ITEM_WEAPON )
     {
      dt += wield->value[3];
      if (wield->value[0] >= 1)
      {
       /* Look, just don't ask...   KaVir */
       if(wield->value[0] >= 1000)
        sn = wield->value[0] - ((wield->value[0] / 1000) * 1000);
       else
        sn = wield->value[0];
       if (sn != 0 && victim->position == POS_FIGHTING)
        (*skill_table[sn].spell_fun) (sn,wield->level,ch,victim);
      }
     }
    }
                
    if ( victim == NULL || victim->position != POS_FIGHTING )
                return;
                        
    if ( ch->fighting != victim 
        || dt == gsn_backstab || dt == gsn_garotte 
        || dt == gsn_headbutt || dt == gsn_circle
        || dt == gsn_shiroken || dt == gsn_hakun)
                return;
    
    maxcount = number_attacks(ch, victim);
    if(!IS_NPC(ch))
    {
     chance = 0;
     if(wield != NULL && wield->item_type == ITEM_WEAPON)
     {
      int tempnum = wield->value[3];
      chance = (ch->wpn[tempnum]) * 0.5;
     }
     else
      chance = (ch->wpn[0]) * 0.5;
     if(number_percent() <= chance)
      maxcount += 1;
    }
    if(wieldorig == 3) /* two weapons, extra attack */
      maxcount += 1;
    for(countup = 0; countup <= maxcount; countup++) /* lets hit a lot */
    {
      wieldtype = wieldorig;
      if( wieldorig == 15 )
      {
       if( wieldie == 1)
        wieldtype = 8;
       else if( wieldie == 2)
        wieldtype = 4;
       else if( wieldie == 3)
        wieldtype = 2;
       else
        wieldtype = 1;
      }
      if( wieldorig == 14 )
      {
       if(wieldie == 2)
        wieldtype = 8;
       else if(wieldie == 1)
        wieldtype = 4;
       else
        wieldtype = 2;
      }
      if( wieldorig == 13 )
      {
       if( wieldie == 1)
        wieldtype = 8;
       else if(wieldie == 2) 
        wieldtype = 4;
       else wieldtype = 1;
      }
      if( wieldorig == 12 )
      {
       if( wieldie < 3 )
        wieldtype = 8;
       else  
        wieldtype = 4;
      }
      if( wieldorig == 11 )
      {
       if( wieldie == 1 )   
        wieldtype = 8;
       if( wieldie == 2 )
        wieldtype = 2;
       else 
        wieldtype = 1;
      }
      if( wieldorig == 10 )
      {
       if( wieldie < 3 )   
        wieldtype = 8;
       else 
        wieldtype = 2;
      }
      if( wieldorig == 9 )
      {
       if( wieldie < 3 )
        wieldtype = 8;
       else 
        wieldtype = 1;   
      }
      if( wieldorig == 7 )
      {
       if( wieldie == 1 )
        wieldtype = 4;
       else if( wieldie == 2 )
        wieldtype = 2;
       else 
        wieldtype = 1;   
      }
      if( wieldorig == 6 )
      {
       if( wieldie == 1 || wieldie == 2)
        wieldtype = 2;
       else 
        wieldtype = 4;   
      }
      if( wieldorig == 5 )
      {
       if( wieldie < 3)
        wieldtype = 4;
       else 
        wieldtype = 1;
      }
      if( wieldorig == 3 )
      {
       if( wieldie == 2 || wieldie == 4)
        wieldtype = 2;
       else 
        wieldtype = 1;
      }
      one_hit( ch, victim, -1, wieldtype );
      if( victim == NULL || victim->position != POS_FIGHTING )
       return;
      if( ch->fighting != victim )
       return;
    }/* End of for (countup = 0; countup <= maxcount; countup++)*/
    if( !IS_NPC(ch) )
    {
     int hld = 0;
     if( ASKILL(ch,AS_SWORDSMAN) > 1 )
     {
      CHAR_DATA *att;
      for( att = ch->in_room->people; att; att = att->next_in_room)
      {
       if( att == ch )
        continue;
       if( is_same_group(ch,att) )
        continue;
       if( att->fighting == ch && !IS_NPC(att) )
        hld++;
       if( hld > 1 )
       {
        if( (ASKILL(ch,AS_SWORDSMAN)/2) >= number_range(1,100) )
        {
         one_hit(ch,att,gsn_swordstrike,1);
         if( number_range(1,3) == 2 )
          one_hit(ch,att,gsn_swordstrike,1);
         if( number_range(1,10) == 6 )
          one_hit(ch,att,gsn_swordstrike,1);
         if( number_range(1,20) == 17 )
          one_hit(ch,att,gsn_swordstrike,1);
        }
       }
      }
     }
    }
    if( IS_SET(ch->warp, WARP_VENOM) && (number_range(1,3) == 2))
    {
     multi_hit(ch, victim, gsn_venom);
    }
    if( IS_SET(ch->warp, WARP_SHARDS) && (number_range(1,3) == 2))
    {
     multi_hit(ch, victim, gsn_shards);
    }
    if( IS_SET(ch->warp, WARP_SPIKE) && (number_range(1,3) == 2))
    {
     multi_hit(ch, victim, gsn_spiketail);
    }
    if( IS_SET(ch->warp, WARP_TENT) && (number_range(1,3) == 2))
    {
     multi_hit(ch, victim, gsn_tentacle);
    }
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DROW) && IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SPIDERFORM)
      && IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_ARMS) )
    {
     int cnt = 0;
     for( cnt = 0; cnt < 4; cnt++ )
      if( number_range(1,4) == 3 )
       one_hit(ch,victim,gsn_spiderform,1);
    }
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLAMEHANDS) && !IS_AFFECTED(victim,AFF_FLAMING))
     { 
      if( number_percent() > 35 && victim != NULL)
      {
       SET_BIT(victim->affected_by, AFF_FLAMING);
       act("Your flaming hands catch $N on fire!", ch, NULL, victim, TO_CHAR);
       act("$n's flaming hands catch $N on fire!", ch, NULL, victim, TO_NOTVICT);
       act("$n's flaming hands catch you on fire!", ch, NULL, victim, TO_VICT);
      }
     }
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_HORNS) && number_range(1,6) == 2 )
      one_hit(ch,victim,gsn_horns,1);
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_WINGS) && number_range(1,6) == 2 )
      one_hit(ch,victim,gsn_buffet,1);
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_TAIL) && number_range(1,6) == 2 )
      one_hit(ch,victim,gsn_sweep,1);
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_HEAD) && number_range(1,6) == 2 )
      one_hit(ch,victim,gsn_headbutt,1);
     if( number_range(1,6) == 2 )
      one_hit(ch,victim,gsn_fangs,1);
     if( number_range(1,6) == 2 )
      one_hit(ch,victim,gsn_claws,1);
    }
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WOLF) )
    {
     if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_VANISH) && ch->fighting != NULL && !IS_NPC(ch->fighting) )
     {
      if( number_range(1,100) < (GET_POWERS(ch,DISC_HAWK)*2) && !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_GONE) )
      {
       stc("#7Your form flickers for a moment, then vanishes from sight!#n\n\r",ch);
       act("#7$n's form flickers for a moment, then vanishes from sight!#n",ch,NULL,NULL,TO_ROOM);
       SET_BIT_POWERS(ch,WOLF_BITS,BIT_GONE);
      }
      if( number_range(1,100) < (GET_POWERS(ch,DISC_HAWK)*3) && IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_GONE) )
      {
       stc("#7Your form slowly flickers back into visibility!#n\n\r",ch);
       REM_BIT_POWERS(ch,WOLF_BITS,BIT_GONE);
       act("#7$n's form slowly flickers into visiblity!#n",ch,NULL,NULL,TO_ROOM);
      }
     }
     if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SLAM) && number_range(1,4) == 2 )
     {
      act("You shoulder charge $N hard!",ch,NULL,victim,TO_CHAR);
      act("$n shoulder charges you hard!",ch,NULL,victim,TO_VICT);
      act("$n shoulder charges $N hard!",ch,NULL,victim,TO_NOTVICT);
      one_hit(ch,victim,gsn_cheapshot,1);
      one_hit(ch,victim,gsn_cheapshot,1);
      one_hit(ch,victim,gsn_cheapshot,1);
     }
     if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_QUILLS) && number_range(1,3) == 2 )
      one_hit(ch,victim,gsn_quills,1);
     if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_CLAWS) && number_range(1,3) == 2 )
      one_hit(ch,victim,gsn_claws,1);
     if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FANGS) && number_range(1,3) == 2 )
      one_hit(ch,victim,gsn_fangs,1);
     if( ch->rage < 150 )
      ch->rage += number_range(8,15);
     if( ch->rage > 99 )
      do_rage_up(ch,"");
     if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW) && !IS_AFFECTED(victim,AFF_FLAMING))
     {
      if( number_percent() > 35 && victim != NULL)
      {
       SET_BIT(victim->affected_by, AFF_FLAMING);
       act("Your flaming claws catch $N on fire!", ch, NULL, victim, TO_CHAR);
       act("$n's flaming claws catch $N on fire!", ch, NULL, victim, TO_NOTVICT);
       act("$n's flaming claws catch you on fire!", ch, NULL, victim, TO_VICT);
      }
     }
    }
       
    if( victim == NULL || victim->position != POS_FIGHTING )
      return;
    if( victim == NULL || victim->position != POS_FIGHTING )
     return;
    /*if(victim->itemaffect < 1)
                return;*/
    if(IS_NPC(victim) || victim->spl[1] < 4) 
      level = victim->level;
    else level = (victim->spl[1] * 0.5);    
               
    if ( has_power_level(victim, SCHOOL_ORDER, 7) && ch->position == POS_FIGHTING)
                if ( (sn = skill_lookup( "lightning bolt" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if ( has_power_level(victim, SCHOOL_FIRE, 6) && ch->position == POS_FIGHTING)
                if ( (sn = skill_lookup( "fireball" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (has_power_level(victim, SCHOOL_WATER, 5) && ch->position == POS_FIGHTING)
                if ( (sn = skill_lookup( "chill touch" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if ( has_power_level(victim, SCHOOL_EARTH, 4) && ch->position == POS_FIGHTING)
                if ( (sn = skill_lookup( "acid blast" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if(  has_power_level(victim, SCHOOL_CHAOS, 4) && ch->position == POS_FIGHTING)
                if ( (sn = skill_lookup( "chaos blast" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if( has_power_level(victim, SCHOOL_PLANT, 5) && ch->position == POS_FIGHTING)
                if( (sn = skill_lookup("thornspray") ) > 0)
                        (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    return;
}

int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int count = 1;
    /* stance bonuses */
    if ( IS_NPC(ch) )
     count += (ch->level/100);
    else
     count += class_num_attack(ch);
    count += stance_mod_apd(ch,victim);
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DRAGON) && ch->stance[0] <= 0 )
     count = 2;
    else if( ch->stance[0] >= 1 )
     count /= 2;
    /* minimum of 1 attack */
    if ( count < 1)
		count = 1;
    return count;
}


void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype)
{
 OBJ_DATA *wield, *obj;
 int dam;
 int wdam;
 int level;
 int right_hand;
 int improveskill;
 int i;

 if( ch->in_room != victim->in_room || victim->position <= POS_DEAD )
  return;              
 if( IS_NPC(victim) && victim->hit < 0 )
 {
  victim->position = POS_DEAD;
  group_gain(ch,victim);
  return;
 }
 if( handtype == 2 )
 {
  wield = get_eq_char(ch,WEAR_HOLD);
  right_hand = 2;
 }
 else if( handtype == 0 )
 {
  wield = NULL;
  right_hand = 0;
 }
 else
 {           
  wield = get_eq_char(ch,WEAR_WIELD);
  right_hand = 1;
 }
 if( dt == TYPE_UNDEFINED )
 {
  dt = TYPE_HIT;
  if( wield != NULL && wield->item_type == ITEM_WEAPON )
   dt += wield->value[3];
 }
 if( ch->wpn[dt-1000] > 5 )
  level = (ch->wpn[dt-1000] / 5);
 else
  level = 1; 
 if( level > 40 )
  level = 40;
 if( !IS_NPC(ch) && !IS_NPC(victim) && has_power_level(victim,SCHOOL_LUCK,6) && number_range(1,100) < 10 )
  multi_hit(victim,ch,gsn_cattack);     
 if( !did_hit(ch,victim) )
 {
  damage(ch,victim,0,dt);
  tail_chain();
  improveskill = 0;
  if( !is_safe(ch,victim) )
  {  
   while( improveskill < 4 )
   {
    improve_wpn(ch,dt,right_hand);
    improve_stance(ch);
    improveskill++;
   }
  }
  return;
 }
 if( IS_NPC(ch) )
  dam = number_range(ch->level,ch->level * 2);
 else
 {
  if( wield != NULL && wield->item_type == ITEM_WEAPON )
  {
   dam = dice(wield->value[1],wield->value[2]);
   dam *= wield->durability;
   dam /= 100;
   if( number_percent() >= 99 && number_percent() <= 50 && !IS_NPC(victim) && !deathmatch && !in_arena(ch) )
   {
    wield->durability--;
   }
  }
  else if( IS_CLASS(ch,CLASS_MONK) )
  {
   dam = dice((ch->wpn[0]/20),(ch->wpn[0]/2));
   dam *= 0.85;
  }
  else if( IS_CLASS(ch,CLASS_DRAGON) && GET_POWERS(ch,DRAGON_FORM) == DRAGON_FORM_DRAGON)
  {
   dam = dice((ch->wpn[0]/25),(ch->wpn[0]));
   dam *= 0.75;
  }
  else
  {
   dam = dice((ch->wpn[0]/30),(ch->wpn[0]));
   dam *= 0.65;
  }
 }/*klinemark
 if( number_percent() >= 60 && !IS_NPC(victim) && !deathmatch && !in_arena(ch) )
 {
  for( i = 0; i < MAX_WEAR; i++ )
  {
   if( (obj = get_eq_char(ch,i) ) == NULL )
    continue;
   else if( obj->item_type == ITEM_ARMOR )
    obj->durability--;
   if( obj->durability < (number_percent()-50) )
    remove_obj( ch, obj->wear_loc, TRUE );
  }
 }*/
 if( IS_CLASS(ch,CLASS_DRAGON) && GET_POWERS(ch,DRAGON_FORM) == DRAGON_FORM_DRAGON)
  dam += (class_damroll(ch)/2.5) + (class_hitroll(ch)/4.5) + (class_ac(victim)/10);
 else
  dam += (class_damroll(ch)/5) + (class_hitroll(ch)/9) + (class_ac(victim)/5);
 if( !IS_AWAKE(victim) )
  dam *= 2;
 if( dt == gsn_backstab || dt == gsn_circle || dt == gsn_garotte || dt == gsn_hakun )
  dam *= number_range(3,6);
 if( !IS_NPC(ch) && dt >= TYPE_HIT && ch->wpn[dt-1000] >= 200 )
  dam *= (ch->wpn[dt-1000] / 100);
 if( dam < 0 )
  dam = 0;
 if( IS_NPC(ch) )
 {
  if( ch->level >= 50 )
   dam *= 4.3;
  else if( ch->level >= 100 )
   dam *= 4.6;
  else if( ch->level >= 200 )
   dam *= 4.9;
  else if( ch->level >= 300 )
   dam *= 5.2;
  else if( ch->level >= 400 )
   dam *= 5.5;
  else if( ch->level >= 500 )
   dam *= 5.8;
  else if( ch->level >= 600 )
   dam *= 6.1;
  else if( ch->level >= 700 )
   dam *= 6.4;
  else if( ch->level >= 800 )
   dam *= 6.7;   
  else if( ch->level >= 900 )
   dam *= 7.0;
  else
   dam *= 1.0;
 }
 if( IS_NPC(victim) )
  {
  if( ch->level >= 50 )
   dam *= 0.30;
  else if( victim->level >= 100 )
   dam *= 0.27;
  else if( victim->level >= 200 )
   dam *= 0.24;
  else if( victim->level >= 300 )
   dam *= 0.21;
  else if( victim->level >= 400 )
   dam *= 0.18;
  else if( victim->level >= 500 )
   dam *= 0.15;
  else if( victim->level >= 600 )
   dam *= 0.12;
  else if( victim->level >= 700 )
   dam *= 0.08;
  else if( victim->level >= 800 )
   dam *= 0.05;
  else if( victim->level >= 900 )
   dam *= 0.02;
  else
   dam *= 1.0;
 }                 
 if( IS_ITEMAFF(victim,ITEMA_RESISTANCE) )
  dam /= 2;
 if( !IS_NPC(ch) && !IS_NPC(victim) )
 {
  FIGHT_TIMER(ch,1);
  FIGHT_TIMER(victim,1);
 }
 if( IS_NPC(ch) && ch->stance[0] < 1 )
 {
  char *stance;
  if( ch->level < 150 )
   switch( number_range(1,5) )
   {
    default: stance = str_dup("viper");    break;
    case 1:  stance = str_dup("viper");    break;
    case 2:  stance = str_dup("crane");    break;
    case 3:  stance = str_dup("crab");     break;
    case 4:  stance = str_dup("mongoose"); break;
    case 5:  stance = str_dup("bull");     break;
   }
  else if( ch->level >= 150 )
   switch( number_range(6,10) )
   {
    default: stance = str_dup("mantis");   break;
    case 6:  stance = str_dup("mantis");   break;
    case 7:  stance = str_dup("dragon");   break; 
    case 8:  stance = str_dup("tiger");    break;
    case 9:  stance = str_dup("monkey");   break;
    case 10: stance = str_dup("swallow");  break;
   }
  do_stance(ch,stance);
 }
 if( IS_NPC(victim) && victim->stance[0] < 1 )
 {
  char *stance;
  if( victim->level < 150 )
   switch( number_range(1,5) )
   {
    default: stance = str_dup("viper");    break;
    case 1:  stance = str_dup("viper");    break;
    case 2:  stance = str_dup("crane");    break;
    case 3:  stance = str_dup("crab");     break;
    case 4:  stance = str_dup("mongoose"); break;
    case 5:  stance = str_dup("bull");     break;
   }
  else if( victim->level >= 150 )
   switch( number_range(6,10) )
   {
    default: stance = str_dup("mantis");   break;
    case 6:  stance = str_dup("mantis");   break;
    case 7:  stance = str_dup("dragon");   break;
    case 8:  stance = str_dup("tiger");    break;
    case 9:  stance = str_dup("monkey");   break;
    case 10: stance = str_dup("swallow");  break;
   }
  do_stance(victim,stance);
 }
 dam = stance_mod_dam(ch,victim,dam);
 if( !IS_NPC(ch) && wield != NULL && ch->fighting == victim && number_range(1,5) == 2 )
 {
  if( IS_WEAP(wield,WEAPON_VAMPIRIC) )
  {
   wdam = number_range(1,wield->level / 5 + 1) * 1.2;
   act2("$p draws life from $n. [$D]",victim,wield,NULL,TO_ROOM,wdam);
   act2("You feel $p drawing your life away. [$D]",victim,wield,NULL,TO_CHAR,wdam);
   victim->hit -= wdam;
   update_pos(victim);
   ch->alignment = UMAX(-1000,ch->alignment - 1);
   ch->hit += wdam;
  }
  if( (IS_WEAP(wield,WEAPON_FLAMING) || IS_WEAP(wield, WEAPON_ELE_FLAME)))
  {
   wdam = number_range(1,wield->level / 4 + 1) * 1.2;
   act2("$n is burned by $p. [$D]",victim,wield,NULL,TO_ROOM,wdam);
   act2("$p sears your flesh. [$D]",victim,wield,NULL,TO_CHAR,wdam);
   victim->hit -= wdam;
   update_pos(victim);
  }
  if( IS_WEAP(wield,WEAPON_FROST) || IS_WEAP(wield,WEAPON_ELE_WATER) )
  {
   wdam = number_range(1,wield->level / 6 + 2) * 1.2;
   act2("$p freezes $n. [$D]",victim,wield,NULL,TO_ROOM,wdam);
   act2("The cold touch of $p surrounds you with ice. [$D]",victim,wield,NULL,TO_CHAR,wdam);
   victim->hit -= wdam;
   update_pos(victim);
  }
  if( IS_WEAP(wield,WEAPON_ELE_EARTH) )
  {
   wdam = number_range(1,wield->level / 5 + 2) * 1.2;
   act2("$p covers $n in acid. [$D]",victim,wield,NULL,TO_ROOM,wdam);
   act2("The corrosive touch of $p surrounds you with acid. [$D]",victim,wield,NULL,TO_CHAR,wdam);
   victim->hit -= wdam; 
   update_pos( victim );
  }
  if( IS_WEAP(wield,WEAPON_SHOCKING) || IS_WEAP(wield,WEAPON_ELE_AIR) )
  {
   wdam = number_range(1,wield->level/5 + 1) * 1.2;
   act2("$n is struck by lightning from $p. [$D]",victim,wield,NULL,TO_ROOM,wdam);
   act2("You are shocked by $p. [$D]",victim,wield,NULL,TO_CHAR,wdam);
   victim->hit -= wdam;
   update_pos(victim);
  }
  if( IS_WEAP(wield,WEAPON_SUNBLADE))
  {
   wdam = number_range(wield->level/2,wield->level+1);
   if( victim->alignment < -100 )
    wdam *= 1.5;
   if( victim->alignment < -300 )
    wdam *= 1.7;
   if( victim->alignment == -1000 )
    wdam *= 1.8;
   if( wdam > 0 )
   {
    act2("$n's flesh is seared by the radiant aura of $p. [$D]",victim,wield,NULL,TO_NOTVICT,wdam);
    act2("Your flesh is seared by the radiant aura of $p. [$D]",victim,wield,NULL,TO_VICT,wdam);
    victim->hit -= wdam;
    update_pos(victim);
    ch->alignment = UMAX(-1000,ch->alignment+1);
   }
  }
 }
 if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_MONK)
  && HAS_TECH(victim,TECH_BLOCK)
  && !IS_NPC(ch)
  && number_range(1,10) < GET_POWERS(victim, MONK_CURR_CHI)
  && GET_POWERS(victim, MONK_BLOCK) < MAX_MONK_BLOCK)
 {
  GET_POWERS(victim,MONK_BLOCK)++;
  stc("#7Your ability to block attacks has increased.#n\n\r",victim);
 }
 if( !deathmatch && !IS_NPC(victim) && !IS_NPC(ch) && GET_POWERS(ch,JEDI_STANCE) >= 1
  && GET_POWERS(ch,JEDI_STANCE) < JEDI_SKILL_CAP && ( ch->in_room->vnum < 50
  || ch->in_room->vnum > 68 ) )
 {
  if( number_range(1,2) == 1 )
   GET_POWERS(ch,JEDI_STANCE_COUNT)++;
  if( GET_POWERS(ch,JEDI_STANCE_COUNT) >= ((GET_POWERS(ch,JEDI_STANCE)+1) * JEDI_ADVANCE_COUNT) )
  {
   GET_POWERS(ch,JEDI_STANCE)++;
   GET_POWERS(ch,JEDI_STANCE_COUNT) = 0;
   stc("#7You feel a surge of energy as you have become more proficient in combat stances!#n\n\r",ch);
  }
 }
 if( !deathmatch && !IS_NPC(victim) && !IS_NPC(ch) && GET_POWERS(ch,JEDI_DEFENSE) >= 1
  && GET_POWERS(ch,JEDI_DEFENSE) < JEDI_SKILL_CAP && ( ch->in_room->vnum < 50
  || ch->in_room->vnum > 68 ) )
 {
  if( number_range(1,2) == 1 )
   GET_POWERS(ch,JEDI_DEFENSE_COUNT)++;
  if( GET_POWERS(ch,JEDI_DEFENSE_COUNT) >= ((GET_POWERS(ch,JEDI_DEFENSE)+1) * JEDI_ADVANCE_COUNT) )
  {
   GET_POWERS(ch,JEDI_DEFENSE)++;
   GET_POWERS(ch,JEDI_DEFENSE_COUNT) = 0;
   stc("#7You feel a surge of energy as you have become more proficient in defense!#n\n\r",ch);
  }
 }
 if( !deathmatch && !IS_NPC(victim) && !IS_NPC(ch) && GET_POWERS(ch,JEDI_ATTACK) >= 1
  && GET_POWERS(ch,JEDI_ATTACK) < JEDI_SKILL_CAP && ( ch->in_room->vnum < 50
  || ch->in_room->vnum > 68 ) )
 {
  if( number_range(1,2) == 1 )
   GET_POWERS(ch,JEDI_ATTACK_COUNT)++;
  if( GET_POWERS(ch,JEDI_ATTACK_COUNT) >= ((GET_POWERS(ch,JEDI_ATTACK)+1) * JEDI_ADVANCE_COUNT) )
  {
   GET_POWERS(ch,JEDI_ATTACK)++;
   GET_POWERS(ch,JEDI_ATTACK_COUNT) = 0;
   stc("#7You feel a surge of energy as you have become more proficient in attack!#n\n\r",ch);
  }
 }
 dam = class_mod_damage(ch,victim,dam);
 if( dt == gsn_swordstrike )
  dam = number_range(1000,4000);
 damage(ch,victim,dam,dt);
 tail_chain();
 improveskill = 0;
 if( !is_safe(ch,victim) )
 {
  while( improveskill < 5 )
  {
   improve_wpn(ch,dt,right_hand);
   improve_stance(ch);
   improveskill++;
  }
 }
}

int check_immune(CHAR_DATA *ch, int dam_type)
{
    int immune, def;
    int bit;

    immune = 2;
    def = IS_NORMAL;

    if (dam_type == DAM_NONE)
	return immune;

    /* Returns immune if it has been changed at all ! */
	if (immune != 2)
		return immune;

    
    /* set bits to check -- VULN etc. must ALL be the same or this will fail */
    switch (dam_type)
    {
		case(DAM_BASH):         bit = RES_BASH;         break;
		case(DAM_PIERCE):       bit = RES_PIERCE;       break;
		case(DAM_SLASH):        bit = RES_SLASH;        break;
		case(DAM_FIRE):         bit = RES_FIRE;         break;
		case(DAM_COLD):         bit = RES_COLD;         break;
		case(DAM_LIGHTNING):    bit = RES_LIGHTNING;    break;
		case(DAM_ACID):         bit = RES_ACID;         break;
		case(DAM_POISON):       bit = RES_POISON;       break;
		case(DAM_NEGATIVE):     bit = RES_NEGATIVE;     break;
		case(DAM_HOLY):         bit = RES_HOLY;         break;
		case(DAM_ENERGY):       bit = RES_ENERGY;       break;
		case(DAM_MENTAL):       bit = RES_MENTAL;       break;
		case(DAM_DISEASE):      bit = RES_DISEASE;      break;
		case(DAM_DROWNING):     bit = RES_DROWNING;     break;
		case(DAM_LIGHT):        bit = RES_LIGHT;        break;
		case(DAM_CHARM):        bit = RES_CHARM;        break;
		case(DAM_SOUND):        bit = RES_SOUND;        break;
		default:                return def;
    }

    if (immune == 2)
	return def;
    else
	return immune;
}

bool damage_old( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show ) 
{

    OBJ_DATA *corpse;
    bool immune;


    if ( victim->position == POS_DEAD )
		return FALSE;
    
    if ( victim != ch )
    {
		/* Certain attacks are forbidden. Most other attacks are returned. */
		if ( is_safe( ch, victim ) )
			return FALSE;
		if ( victim->position > POS_STUNNED )
		{
	    /* If victim is charmed, ch might attack victim's master. */
			if ( IS_NPC(ch) && IS_NPC(victim) && IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
				&& victim->master->in_room == ch->in_room && number_bits( 3 ) == 0 )
			{
				stop_fighting( ch, FALSE );
				set_fighting( ch, victim->master);
				return FALSE;
			}
		}
		/* More charm stuff.*/
		if ( victim->master == ch )
			stop_follower( victim );
    }

    /* Inviso attacks ... not.  */
    if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
    {
		affect_strip( ch, gsn_invis );
		affect_strip( ch, gsn_mass_invis );
		REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
		act( "$n fades into existence.", ch, NULL, NULL, TO_ROOM );
    }
    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ALIEN) && IS_SET(GET_POWERS(ch, ALIEN_BITS), ALIEN_CLOAK))
     REM_BIT_POWERS(ch, ALIEN_BITS, ALIEN_CLOAK);
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_SHADOW) && IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_INVIS) )
     conceal_count(ch);  
    /* Damage modifiers.*/

    if ( dam > 1 && !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK]  > 10 )
		dam = 9 * dam / 10;
 
    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) )
		dam /= 2;
    if( IS_SET(victim->warp, WARP_CRYSTAL))
        dam /= 1.5;
    if( IS_SET(victim->warp, WARP_STEEL))
        dam /= 1.75;
    if( IS_SET(ch->warp, WARP_STRONG))
        dam *= 1.10;
    if( IS_SET(ch->warp, WARP_WEAK))
        dam /= 1.10;
    if( IS_SET(victim->warp, WARP_VULN))
        dam *= 1.25;
    if( dam > 1 && is_affected(ch, skill_lookup("aura")))
           dam /= 2;
 
    immune = FALSE;
    switch(check_immune(victim,dam_type))
    {
		case(IS_DIMMUNE):
		    dam -= dam/3;
			break;
		case(IS_RESISTANT):     
		    dam -= dam / 2;
			break;
		case(IS_VULNERABLE):
		    dam += dam * 2;
			break;
    }

    if (show)
		dam_message( ch, victim, dam, dt);

    if (dam == 0)
		return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;
    if ( !IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL && victim->hit < 1 )
		victim->hit = 1;
	update_pos( victim );
    switch( victim->position )
    {
		case POS_MORTAL:
			act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
			stc( "You are mortally wounded, and will die soon, if not aided.\n\r", victim );
			break;
	    case POS_INCAP:
			act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
			stc("You are incapacitated and will slowly die, if not aided.\n\r",victim );
			break;
	    case POS_STUNNED:
			act( "$n is stunned, but will probably recover.",victim, NULL, NULL, TO_ROOM );
			stc("You are stunned, but will probably recover.\n\r",victim );
			break;
		case POS_DEAD:
			act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
			stc( "You have been KILLED!!\n\r\n\r", victim );
			stop_fighting(ch, TRUE);
			break;
		default:
			if ( dam > victim->max_hit / 4 )
				stc( "That really did HURT!\n\r", victim );
			if ( victim->hit < victim->max_hit / 4 )
				stc( "#1You sure are BLEEDING!\n\r", victim );
			break;
    }

    /* Sleep spells and extremely wounded folks. */
    if (victim->position == POS_DEAD && !IS_NPC(victim) && victim->class !=0 )
    {
		stop_fighting(victim, TRUE);
		return TRUE;
    }
    else
		update_pos(victim);

    if ( !IS_AWAKE(victim) )
		stop_fighting( victim, FALSE );

    /*  Payoff for killing things. */
    if ( IS_NPC(victim) && victim->position == POS_DEAD)
    {
		group_gain( ch, victim );
		if (IS_NPC(victim) && !IS_NPC(ch) )
		    ch->mkill += 1;	
		if ( !IS_NPC(victim)) 
            {    
			if (IS_NPC(ch))
				victim->mdeath += 1;
			sprintf(log_buf, "%s killed by %s at %d", victim->name, (IS_NPC(ch) ? ch->short_descr : ch->name),victim->in_room->vnum );
			log_string( log_buf );
		}		    
		raw_kill(victim);

		if ( !IS_NPC(ch) && IS_NPC(victim) )
		{
			corpse = get_obj_list( ch, "corpse", ch->in_room->contents );
	 	    if ( IS_SET(ch->act, PLR_AUTOLOOT) && corpse && corpse->contains) /* exists and not empty */
				do_get( ch, "all corpse" );
      
			if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
				return TRUE;  /* leave if corpse has treasure */
			else
				do_sacrifice( ch, "corpse" );
		}
		return TRUE;
	}
	if ( victim == ch )
		return TRUE;

    /* Wimp out? */
/*    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
		if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
		&& victim->hit < victim->max_hit / 5) 
		|| ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
		&& victim->master->in_room != victim->in_room ) )
			do_flee( victim, "" );
    }
*/
    if ( !IS_NPC(victim) &&  victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait < PULSE_VIOLENCE / 2 )
		do_flee( victim, "" );
    tail_chain( );
    return TRUE;
}

void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
 int fake_dam = 0;

 if( victim->position == POS_DEAD )
  return;
 if( victim != ch )
 {
  if( is_safe(ch,victim) )
   return;
  check_killer( ch, victim );
  if( victim->position > POS_STUNNED )
  {
   if( victim->fighting == NULL )
    set_fighting(victim,ch);
   if( ch->fighting == NULL )
    set_fighting(ch,victim);
   if ( IS_NPC(ch) && IS_NPC(victim) && IS_AFFECTED(victim,AFF_CHARM)
    && victim->master != NULL && victim->master->in_room == ch->in_room && number_bits(3) == 0 )
   {
    stop_fighting(ch,FALSE);
    multi_hit(ch,victim->master,TYPE_UNDEFINED);
    return;
   }
  }
 }
 if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_ALIEN) && IS_SET(GET_POWERS(ch,ALIEN_BITS),ALIEN_CLOAK) )
  REM_BIT_POWERS(ch,ALIEN_BITS,ALIEN_CLOAK);
 if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_SHADOW) && IS_SET(GET_POWERS(ch,SHADOW_BITS),SHADOW_INVIS) )
  conceal_count(ch);
 if( victim->master == ch )
  stop_follower(victim);
 if( IS_AFFECTED(ch,AFF_HIDE) )
 {
  dam *= 1.5;
  stc("You use your concealment to get a surprise attack!\n\r",ch);
  REMOVE_BIT(ch->affected_by,AFF_HIDE);
  act("$n leaps from $s concealment!",ch,NULL,NULL,TO_ROOM);
 }
 if( IS_SET(victim->warp,WARP_CRYSTAL) )
  dam *= 0.9;
 if( IS_SET(victim->warp,WARP_STEEL) )
  dam *= 0.8;
 if( IS_SET(ch->warp,WARP_STRONG) )
  dam *= 1.10;
 if( IS_SET(ch->warp,WARP_WEAK) )
  dam *= 0.90;
 if( IS_SET(victim->warp,WARP_VULN) )
  dam *= 1.10;
 if( is_affected(victim,skill_lookup("aura")))
  dam *= 0.65;
 if ( IS_AFFECTED(victim,AFF_SANCTUARY) )
  dam *= 0.5;
 if ( IS_AFFECTED(victim,AFF_PROTECT) )
  dam *= 0.95;
 if ( dam < 0 )
  dam = 0;
 if( dt >= TYPE_HIT )   
 {
  if( IS_NPC(ch) && number_percent() < ch->level * 0.5 )
   disarm(ch,victim);
  if( IS_NPC(ch) && number_percent() < ch->level * 0.5 )
   trip(ch,victim);
  if( check_parry(ch,victim,dt) )
   return;
  if( check_dodge(ch,victim,dt) )
   return;
 }
 if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DRAGON) && GET_POWERS(victim,DRAGON_FORM) == DRAGON_FORM_DRAGON
  && number_range(1,30) < 10 )
 {
  act("#3Your tough scales protect against $n's attack!#n",ch,NULL,victim,TO_VICT);
  act("#3$N's tough scales protect against your attack!#n",ch,NULL,victim,TO_CHAR);
  act("#3$N's tough scales protect against $n's attack!#n",ch,NULL,victim,TO_NOTVICT);
 }
 if( is_affected(victim,skill_lookup("manashield")) && (victim->mana > (dam * 2)) )
 {
  fake_dam = dam;
  fake_dam *= 2;
  victim->mana -= fake_dam;
  dam = 0;
 }
 if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_WOLF) && ( GET_POWERS(victim,DISC_LUNA) > 9
  && ( weather_info.sunlight == SUN_DARK || weather_info.sunlight == SUN_SET ) ) && (number_range(1,25) < 6) 
  && victim->hit > 100 )
 {
  act("#3You absorb the damage and $n is struck by Luna's wrath!#n", ch,NULL,victim,TO_VICT);
  act("#3$N absorbs $n's hit and $n is struck by Luna's wrath!#n",ch,NULL,victim,TO_NOTVICT);
  act("#3$N absorbs your damage and you are struck by Luna's wrath!#n",ch,NULL,victim,TO_CHAR);
  ch->hit -= 200;
  victim->hit += 100;
 }
 if( !IS_NPC(victim)
  && IS_CLASS(victim,CLASS_VAMPIRE)
  && ( weather_info.sunlight == SUN_DARK || weather_info.sunlight == SUN_SET )  
  && (number_range(1,30) < 10)   
  && victim->hit > 0 && GET_POWERS(victim,VAMP_AGE) > AGE_CAITIFF )
 {
  act("#3The night enshrouds you, protecting you from harm!#n", ch,NULL,victim,TO_VICT);
  act("#3The night enshrouds $N, protecting $M from harm!#n",ch,NULL,victim,TO_NOTVICT);
  act("#3The night enshrouds $N, protecting $M from your attack!#n",ch,NULL,victim,TO_CHAR);
  dam /= GET_POWERS(victim,VAMP_AGE);
 }
 if( !IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK && victim->hit > 0)
  && number_range(1,23) <= GET_POWERS(victim, MONK_CURR_CHI) )
 {
  act("#3Your chi energy absorbs $n's attack.#n.", ch, NULL, victim, TO_VICT);
  act("#3$N's chi energy absorbs your attack#n.", ch, NULL, victim, TO_CHAR);
  act("#3$N's chi energy absorbs $n' attack#n.", ch, NULL, victim, TO_NOTVICT);
  dam = 0;
 }
 if( victim->hit > victim->max_hit )
  victim->hit = victim->max_hit;
 if( ch->hit > ch->max_hit )
  ch->hit = ch->max_hit;

 dam_message(ch,victim,dam,dt);
 hurt_person(ch,victim,dam);
 return;
}

void adv_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
    if ( victim->position == POS_DEAD )
		return;
    if ( dam > 1000 ) dam = 1000;
    if ( victim != ch )
    {
		if ( is_safe( ch, victim ) )
		    return;
		check_killer( ch, victim );
		if ( victim->position > POS_STUNNED )
		{
			if ( victim->fighting == NULL )
			set_fighting( victim, ch );
		    victim->position = POS_FIGHTING;
		}	
		if ( victim->position > POS_STUNNED )
		{
			if ( ch->fighting == NULL )
			set_fighting( ch, victim );
		    /* If victim is charmed, ch might attack victim's master.  */
		    if ( IS_NPC(ch)
		    &&   IS_NPC(victim)
			&&   IS_AFFECTED(victim, AFF_CHARM)
		    &&   victim->master != NULL
		    &&   victim->master->in_room == ch->in_room
			&&   number_bits( 3 ) == 0 )
		    {
				stop_fighting( ch, FALSE );
				multi_hit( ch, victim->master, TYPE_UNDEFINED );
				return;
			}
		}

		if ( victim->master == ch )
		    stop_follower( victim );
		if ( IS_AFFECTED(victim, AFF_SANCTUARY) && dam > 1 )
		    dam /= 2;
		if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
		    dam -= dam / 4;
		if (!IS_NPC(victim) && !IS_NPC(ch)) dam = dam *2/3;
		if ( dam < 0 )
		    dam = 0;
    }
    return;
}/* End of void adv_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam )*/

void hurt_person( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
	char buf[MAX_STRING_LENGTH];
    bool is_npc = FALSE;

    /* Hurt the victim. Inform the victim of his new state.  */
    victim->hit -= dam;
    if ( !IS_NPC(victim)  && victim->level >= LEVEL_IMMORTAL && victim->hit < 1 )
		victim->hit = 1;
    if ( !IS_NPC(victim)  && IS_SET(victim->pcdata->powers[0], POWER_LIFE) && victim->hit <= 1 )
    {
            victim->hit += 5000;
            REMOVE_BIT(victim->pcdata->powers[0], POWER_LIFE);
            act( "$N continues to fight on with a wild fanatisism!", ch, NULL, victim, TO_CHAR );
            act( "$N continues to fight on with a wild fanatisism!", ch, NULL, victim, TO_NOTVICT );
            act( "You continue to fight with a wild fanatisism", ch, NULL, victim, TO_VICT );
    }
    if ( !IS_NPC(victim)  && IS_SET(victim->newbits, NEW_CLOAK) && victim->hit < 1 )
    {
		victim->hit += (victim->max_hit * .25);
		REMOVE_BIT(victim->newbits, NEW_CLOAK);
    }
    update_pos( victim );
    switch( victim->position )
    {
	    case POS_MORTAL:
			act( "$n is mortally wounded, and spraying blood everywhere.", victim, NULL, NULL, TO_ROOM );
			stc( "You are mortally wounded, and spraying blood everywhere.\n\r", victim );
			/* Arena Code Retry */
			if (((IS_SET(victim->act, PLR_CHALLENGER) && IS_SET(ch->act, PLR_CHALLENGED))
				|| (IS_SET(victim->act, PLR_CHALLENGED) && IS_SET(ch->act, PLR_CHALLENGER)))
			&& ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
			&& ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68)) )
			{
				if (ch->challenged != NULL)
					victim = ch->challenged;
				else
					victim = ch->challenger;
				char_from_room(ch);
				char_to_room(ch,get_room_index(69));
				char_from_room(victim);
				char_to_room(victim,get_room_index(70));
				ch->hit = ch->max_hit;
				ch->mana = ch->max_mana;
				ch->move = ch->max_move;
				ch->loc_hp[0] = 0;
			        ch->loc_hp[1] = 0;
				ch->loc_hp[2] = 0;
		     	        ch->loc_hp[3] = 0;
				ch->loc_hp[4] = 0;
				ch->loc_hp[5] = 0;
				ch->loc_hp[6] = 0;
			/*	if (IS_AFFECTED(ch, AFF_FLAMING))
				    REMOVE_BIT(ch->affected_by, AFF_FLAMING); */
				update_pos(ch);
				do_save(ch,"");
				do_look(ch, "auto");
				victim->hit = victim->max_hit;
				victim->mana = victim->max_mana;
				victim->move = victim->max_move;
				victim->loc_hp[0] = 0;
			        victim->loc_hp[1] = 0;
				victim->loc_hp[2] = 0;
			        victim->loc_hp[3] = 0;
				victim->loc_hp[4] = 0;
				victim->loc_hp[5] = 0;
				victim->loc_hp[6] = 0;
			/*	if (IS_AFFECTED(victim, AFF_FLAMING))
				    REMOVE_BIT(victim->affected_by, AFF_FLAMING); */
				update_pos(victim);
				do_save(victim,"");
				do_look(victim, "auto");
				ch->challenger = NULL;
				ch->challenged = NULL;
				victim->challenged = NULL;
				victim->challenger = NULL;
				if IS_SET(victim->act, PLR_CHALLENGED)
				{
					REMOVE_BIT(victim->act,PLR_CHALLENGED);
					REMOVE_BIT(ch->act,PLR_CHALLENGER);
				}
				else
				{
					REMOVE_BIT(ch->act,PLR_CHALLENGED);
					REMOVE_BIT(victim->act,PLR_CHALLENGER);
				}
				stc("You were beaten.\n\r",victim);
				stc("You were victorious.\n\r",ch);
				ch->fight_timer = 0;
				victim->fight_timer = 0;
				ch->stuntimer = 0;
				victim->stuntimer = 0;
				victim->pcdata->alosses++;
				victim->pcdata->target = NULL;
				victim->fight_timer = 0;
				ch->pcdata->awins++;
				ch->pcdata->target = NULL;
				ch->fight_timer = 0;
				arena = FIGHT_OPEN;			
		    	        sprintf(buf,"[ARENA] %s has been beaten by %s.",victim->pcdata->switchname,ch->pcdata->switchname);
		    	        do_info(ch,buf);
                                ch->pcdata->held = str_dup( "");
                                ch->pcdata->held_by = str_dup( "");
                                victim->pcdata->held = str_dup( "");
                                victim->pcdata->held_by = str_dup("");
				arena = FIGHT_OPEN;			
			}
			else if (IS_SET(ch->act, PLR_CHALLENGER))
				do_withdraw(ch,"");
			else if (IS_SET(ch->act, PLR_CHALLENGED))
				do_decline(ch,"");
			else if (IS_SET(victim->act, PLR_CHALLENGER))
				do_withdraw(victim,"");
			else if (IS_SET(victim->act, PLR_CHALLENGED))
				do_decline(victim,"");
			/* This is never going to work */
			break;
	    case POS_INCAP:
			act( "$N is incapacitated, and bleeding badly.",
			ch, NULL, victim, TO_ROOM );
			stc("You are incapacitated, and bleeding badly.\n\r", victim );
			break;
	    case POS_STUNNED:
			act( "$N is stunned, but will soon recover.",ch, NULL, victim, TO_ROOM );
			stc("You are stunned, but will soon recover.\n\r",victim );
			break;
	    case POS_DEAD:
			act( "$N is DEAD!!", ch, NULL, victim, TO_ROOM );
			stc( "You have been KILLED!!\n\r", victim );
			break;
	    default:
			if ( dam > victim->max_hit / 4 )
				stc( "That really did HURT!\n\r", victim );
			if ( victim->hit < victim->max_hit / 4 && dam > 0 )
			{
					stc( "You sure are BLEEDING!\n\r", victim );
			}
			break;
    }

    /* Sleep spells and extremely wounded folks.  */
    if ( !IS_AWAKE(victim) )
		stop_fighting( victim, FALSE );
    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
		group_gain( ch, victim );

		if ( !IS_NPC(victim) )
		{
			sprintf( log_buf, "%s killed by %s at %d",
			victim->name,
			(IS_NPC(ch) ? ch->short_descr : ch->name),
			victim->in_room->vnum );
			log_string( log_buf );
	        /* Dying penalty: 1/2 your current exp.  */
			if ( victim->exp > 0 )
				victim->exp = victim->exp / 2;
		}
    	if (IS_NPC(victim) && !IS_NPC(ch))
		{  
                    is_npc = TRUE;
		    ch->mkill += 1;
		    if (ch->level == 1 && ch->mkill > 4)
		    {
				ch->level = 2;
				do_save(ch,"");
			}
		}
		if (!IS_NPC(victim) && IS_NPC(ch)) 
	    {
			victim->mdeath = victim->mdeath + 1;
	    }
		raw_kill( victim );
		if ( IS_SET(ch->act, PLR_AUTOLOOT) )
		    do_get( ch, "all corpse" );
		else
		    do_look( ch, "in corpse" );

		if ( !IS_NPC(ch) && is_npc )
		{
			if ( IS_SET(ch->act, PLR_AUTOSAC) )
			{
			 if( IS_CLASS(ch,CLASS_WOLF) && GET_POWERS(ch,DISC_RAPTOR) > 4 )
			  do_devour(ch,"corpse");
			 else
		          do_sacrifice( ch, "corpse" );
			}
		}
		return;
    }

    if ( victim == ch )
		return;

    /* Take care of link dead people. */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
		if ( number_range( 0, victim->wait ) == 0 )
		{
			do_recall( victim, "" );
			return;
		}
    }

    /* Wimp out? */
    if ( IS_NPC(victim) && dam > 0 )
    {
		if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 1 ) == 0 && victim->hit < victim->max_hit / 2 )
		|| ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL && victim->master->in_room != victim->in_room ) )
			do_flee( victim, "" );
    }
    if ( !IS_NPC(victim) && victim->hit > 0 && victim->hit <= victim->wimpy && victim->wait == 0 )
		do_flee( victim, "" );
    tail_chain( );
    return;
}
bool IS_DAMNED (CHAR_DATA *ch)
{
    if( ch->level == 4 && ch->trust == 4 && !IS_NPC(ch) )
    {
	stc("Spirits of the damned cannot do that.\n\r", ch);
	return TRUE;
    }
    else
	return FALSE;
}
bool can_heal (CHAR_DATA *ch, CHAR_DATA *victim)
{
    if( ch->level == victim->level || IS_NPC(victim) ) return TRUE;
    if( IS_DAMNED(ch) || IS_DAMNED(victim) ) return FALSE;
    else return FALSE;
}
bool can_stun (CHAR_DATA *ch, CHAR_DATA *victim)
{

    int vict_health_index = 0;
    int ch_health_index = 0;

    vict_health_index = 100 * (victim->hit / victim->max_hit);
    ch_health_index = 100 * (ch->hit / ch->max_hit);
    if( number_percent() > vict_health_index && ch_health_index > 10 )
	return TRUE;
    else
	return FALSE;
}


bool is_safe (CHAR_DATA * ch, CHAR_DATA * victim)
{
    bool found = FALSE;
    OBJ_DATA *obj;
   
    if( ch == victim)
     return FALSE;
    if( (ch->level >= 3 && victim->level >= 3) && deathmatch )
     return FALSE; 
    if( victim->stasis)
    {
     stc("Not while they are in stasis.\n\r", ch);
     return TRUE;
    }
    if( ch->in_room == NULL || victim->in_room == NULL)
     return TRUE;
    if( (ch->in_room->vnum < 50 || ch->in_room->vnum > 68)
    ||  (victim->in_room->vnum < 50 || victim->in_room->vnum > 68) )
    {
      if ( !IS_NPC(victim) && !IS_NPC(ch) )
      {
	if( (IS_DAMNED(ch) || IS_DAMNED(victim) ) && (!deathmatch) ) return TRUE;
      }
    }
    if (!IS_NPC (victim) && !IS_NPC (ch))
    {
		/*if (!CAN_PK (ch) || !CAN_PK (victim))*/
		if ( (ch->level < 3) || (victim->level < 3))
		{
			stc ("Both players must be avatars to fight.\n\r", ch);
			return TRUE;
		}
    }
 /* It seems we have a problem with people PKing in groups.
  * I don't have much of a taste for this tactic...And it
  * seems to be driving a few players away; so I'll fix it.
  * --Kline
  */
    if( !IS_NPC(victim) && !IS_NPC(ch) && nogroup )
    {
     if( ( IS_SET(victim->act,PLR_NO_GROUP) && !IS_SET(ch->act,PLR_NO_GROUP) )
      || ( !IS_SET(victim->act,PLR_NO_GROUP) && IS_SET(ch->act,PLR_NO_GROUP) ) )
     {
      stc("You can only fight another person of the same grouping status!\n\r",ch);
      return TRUE;
     }
     if( IS_SET(victim->act,PLR_NO_GROUP) && victim->fight_timer > 10 && victim->pcdata->target != NULL )
     {
      if( str_cmp(victim->pcdata->target,ch->pcdata->switchname) )
      {
       stc("You can't attack somebody who was just recently in a fight with another!\n\r",ch);
       return TRUE;
      }
     }
    }
    if( ch->login_timer > 0)
      {  
       stc("Not with your login timer active.\n\r", ch);
       return TRUE;  
      }
    if( victim->login_timer > 0)
      {  
       stc("Not with their login timer active.\n\r", ch);
       return TRUE;  
      }
    if( victim->paintball[PAINT_ON] == 1)
    {
     stc("They are currently in paintball wars.\n\r", ch);
     return TRUE;
    }
    if(ch->paintball[PAINT_ON] == 1)
    {
     stc("You are in paintball wars..no fighting.\n\r", ch);
     return TRUE;
    }
    if (victim->fight_timer > 0 && !IS_NPC(victim))
		return FALSE;
    /* Ethereal people can only attack other ethereal people */
    if (IS_AFFECTED (ch, AFF_ETHEREAL) && !IS_AFFECTED (victim, AFF_ETHEREAL))
	{
		stc ("You cannot while ethereal.\n\r", ch);
		return TRUE;
    }
    if (!IS_AFFECTED (ch, AFF_ETHEREAL) && IS_AFFECTED (victim, AFF_ETHEREAL))
	{
		stc ("You cannot fight an ethereal person.\n\r", ch);
		return TRUE;
    }
    /* You cannot attack across planes */
    if (IS_AFFECTED (ch, AFF_SHADOWPLANE) && !IS_AFFECTED (victim, AFF_SHADOWPLANE))
	{
		act ("You are too insubstantial!", ch, NULL, victim, TO_CHAR);
		return TRUE;
    }
    if (!IS_AFFECTED (ch, AFF_SHADOWPLANE) && IS_AFFECTED (victim, AFF_SHADOWPLANE))
	{
		act ("$E is too insubstantial!", ch, NULL, victim, TO_CHAR);
		return TRUE;
    }
    if( (ch->in_room == NULL) || (victim == NULL))
     return TRUE;
    if( IS_SET(ch->in_room->room_flags, ROOM_SAFE) && !IS_NPC(victim)) 
	{
         if( (victim->level >= 3) && victim->pkill >= 5)
          return FALSE; 
        if( (victim->level >= 3) && victim->pkill < 5)
         return TRUE;
         for( obj = victim->carrying; obj != NULL; obj = obj->next_content )
            {
              if( IS_SET( obj->quest, QUEST_ARTIFACT))
               found = TRUE;
            }
         if( found == TRUE)
        return FALSE;
        if (IS_ITEMAFF(victim, ITEMA_ARTIFACT))
            return FALSE;
		stc ("You cannot fight in a safe room.\n\r", ch);
			return TRUE;
    }
    if( !IS_NPC(victim) && IS_SET(victim->in_room->room_flags, ROOM_SAFE) && !IS_NPC(ch))
    {
        if( (victim->level >= 3) && victim->pkill >= 5)
         return FALSE; 
        if( (victim->level >= 3) && victim->pkill < 5)
         return TRUE;
         for( obj = victim->carrying; obj != NULL; obj = obj->next_content )
            {
              if( IS_SET( obj->quest, QUEST_ARTIFACT))
               found = TRUE;
            }
         if( found == TRUE)
        return FALSE;
        if (IS_ITEMAFF(victim, ITEMA_ARTIFACT))
            return FALSE;
       stc ("You cannot fight in a safe room.\n\r", ch);
       return TRUE;
    }

    /* ARENA room checking */
    if( (ch->in_room != NULL) && (victim->in_room != NULL) && IS_SET (ch->in_room->room_flags, ROOM_ARENA) 
     && IS_SET (victim->in_room->room_flags, ROOM_ARENA))
		return FALSE;
    if (IS_HEAD (ch, LOST_HEAD) || IS_EXTRA (ch, EXTRA_OSWITCH))
	{
		stc ("Objects cannot fight!\n\r", ch);
		return TRUE;
    }
	else if (IS_HEAD (victim, LOST_HEAD) || IS_EXTRA (victim, EXTRA_OSWITCH))
	{
		stc ("You cannot attack an object.\n\r", ch);
		return TRUE;
    }
	/*boogie*/
	if (IS_AFFECTED(victim, AFF_CHARM) && IS_NPC(victim) && ch->level < 3 && !IS_NPC(ch))
		return TRUE;
	if (IS_AFFECTED(ch, AFF_CHARM) && IS_NPC(ch) && victim->level < 3 && !IS_NPC(victim))
		return TRUE;

	if (is_same_group(ch, victim))
		return TRUE;
	if (IS_NPC(ch) && IS_NPC(victim) && victim->level < 5)
		return TRUE;
        if (IS_NPC (ch) || IS_NPC (victim))
		return FALSE;

	if (!IS_NPC(victim) && victim->level > 7)
	{
		stc("You are not permitted to attack an immortal.\n\r", ch);
		return TRUE;
	}
    /* Thx Josh! */
    if (victim->fighting == ch)
		return FALSE;
    if (IS_ITEMAFF (ch, ITEMA_PEACE))
	{
		stc ("You are unable to attack them.\n\r", ch);
		return TRUE;
    }
    if (IS_ITEMAFF (victim, ITEMA_PEACE))
	{
		stc ("You can't seem to attack them.\n\r", ch);
		return TRUE;
    }
    return FALSE;
    if( (ch->in_room->vnum >= 50 && ch->in_room->vnum <= 68)
     && (victim->in_room->vnum >= 50 && victim->in_room->vnum <= 68) )
     return FALSE;
}

bool no_attack (CHAR_DATA * ch, CHAR_DATA * victim)
{


    if (!IS_NPC (ch) && !IS_NPC (victim)) {
	/*if (!CAN_PK (ch) || !CAN_PK (victim)) */
        if ( (ch->level <= 2) || (victim->level <= 2))
	{
	    stc ("Both players must be avatars to fight.\n\r", ch);
	    return TRUE;
	}
    }
    if (victim->fight_timer > 0 && !IS_NPC(victim))
	return FALSE;

    /* Ethereal people can only attack other ethereal people */
    if (IS_AFFECTED (ch, AFF_ETHEREAL) && !IS_AFFECTED (victim, AFF_ETHEREAL)) {
	stc ("You cannot while ethereal.\n\r", ch);
	return TRUE;
    }
    if (!IS_AFFECTED (ch, AFF_ETHEREAL) && IS_AFFECTED (victim, AFF_ETHEREAL)) {
	stc ("You cannot fight an ethereal person.\n\r", ch);
	return TRUE;
    }
    /* You cannot attack across planes */
    if (IS_AFFECTED (ch, AFF_SHADOWPLANE) && !IS_AFFECTED (victim, AFF_SHADOWPLANE)) {
	act ("You are too insubstantial!", ch, NULL, victim, TO_CHAR);
	return TRUE;
    }
    if (!IS_AFFECTED (ch, AFF_SHADOWPLANE) && IS_AFFECTED (victim, AFF_SHADOWPLANE)) {
	act ("$E is too insubstantial!", ch, NULL, victim, TO_CHAR);
	return TRUE;
    }
    if (IS_SET (ch->in_room->room_flags, ROOM_SAFE) && !IS_NPC(victim)) 
	{
		if ((victim->level >= 3) && victim->race >= 10)
			return FALSE;

	stc ("You cannot fight in a safe room.\n\r", ch);
	return TRUE;
    }
    if (IS_HEAD (ch, LOST_HEAD) || IS_EXTRA (ch, EXTRA_OSWITCH)) {
	stc ("Objects cannot fight!\n\r", ch);
	return TRUE;
    } else if (IS_HEAD (victim, LOST_HEAD) || IS_EXTRA (victim, EXTRA_OSWITCH)) {
	stc ("You cannot attack objects.\n\r", ch);
	return TRUE;
    }
        if (!IS_NPC(victim) && victim->level > 7)
        {
                stc("You are not permitted to attack an immortal.\n\n", ch);
                return TRUE;
        }
    return FALSE;
}


void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
    /*Follow charm thread to responsible character.Attacking someone's charmed char is hostile! */
    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
		victim = victim->master;

    /* NPC's are fair game.So are killers and thieves. */
    if ( IS_NPC(victim) )
		return;

    /* Charm-o-rama. */
    if ( IS_SET(ch->affected_by, AFF_CHARM) )
    {
		if ( ch->master == NULL )
		{
			affect_strip( ch, gsn_charm_person );
		    REMOVE_BIT( ch->affected_by, AFF_CHARM );
		    return;
		}
		stop_follower( ch );
		return;
    }
    return;
}/* End of void check_killer( CHAR_DATA *ch, CHAR_DATA *victim ) */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *obj = NULL;
    int chance = 0;

    if ( !IS_AWAKE(victim) )
		return FALSE;

    if ( IS_NPC(victim) )
		obj = NULL;
    if( ch->in_room == NULL)
     return FALSE;
    if( victim->in_room == NULL)
     return FALSE;
    else
    {
     if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL || obj->item_type != ITEM_WEAPON )
     {
     if ( ( obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL || obj->item_type != ITEM_WEAPON )
	return FALSE;
     }
    }
    if (dt < 1000 || dt > 1012)
		return FALSE;
    if (!IS_NPC(ch))
		chance -= (ch->wpn[dt-1000] * 0.1);
	else
		chance -= (ch->level * 0.1);
    if (!IS_NPC(victim))
		chance += (victim->wpn[dt-1000] * 0.5);
	else
		chance += (victim->level * 0.25);

    chance -= (class_hitroll(ch) * 0.005);

    if( !IS_NPC(ch))
     chance -= class_parry(ch);
    if( !IS_NPC(victim))
     chance += class_parry(victim);
    chance += stance_mod_apd(victim,ch);
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DRAGON) && ch->stance[0] <= 0 )
     chance = 25;
    else if( ch->stance[0] >= 1 )
     chance /= 4;
    if( chance > 75)
     chance = 75;
    if( chance < 25)
     chance = 25;

    if ( number_percent() >= chance )
     return FALSE;

    if (!IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0 && obj->value[3] <= 12)
    {
		if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		    act( "You parry $n's blow with $p.",  ch, obj, victim, TO_VICT );
		if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		    act( "$N parries your blow with $p.", ch, obj, victim, TO_CHAR );
		return TRUE;
    }
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT    );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		act( "$N parries your attack.", ch, NULL, victim, TO_CHAR    );
    return TRUE;
}

bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance = 0;
    
    if( (ch->in_room == NULL) || (victim->in_room == NULL))
     return FALSE;
    if ( !IS_AWAKE(victim) )
		return FALSE;
    if (!IS_NPC(ch))
		chance -= (ch->wpn[dt-1000] * 0.1);
	else
		chance -= (ch->level * 0.1);
    if (!IS_NPC(victim))
		chance += (victim->wpn[0] * 0.5);
	else
		chance += (victim->level * 0.25);

    chance -= (class_hitroll(ch) * 0.005);
    if( !IS_NPC(ch))
     chance -= class_dodge(ch);
    if( !IS_NPC(victim))
     chance += class_dodge(victim);
    chance += stance_mod_apd(victim,ch);
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DRAGON) && ch->stance[0] <= 0 )
     chance = 25; 
    else if( ch->stance[0] >= 1 )
     chance /= 4;
    if( chance > 75)
     chance = 75;
    if( chance < 25 )
     chance = 25;
    if ( number_percent( ) < 100 && number_percent( ) >= chance )
		return FALSE;

    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF))
		act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF))
		act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR );
    return TRUE;
}

void update_pos( CHAR_DATA *victim )
{
//	CHAR_DATA *mount;
    if( victim == NULL)
     return;
    if ( victim->hit > 0 )
    {
    	if ( victim->position <= POS_STUNNED && victim->stuntimer == 0 )
		{
			bool gm_stance = FALSE;
		    victim->position = POS_STANDING;
		    if (!IS_NPC(victim) && victim->stance[0] > 0)
		    {
				int stance = victim->stance[0];
				if (victim->stance[stance] >= 200) gm_stance = TRUE;
		    }
			if (IS_NPC(victim) || victim->max_hit * 0.25 > victim->hit || !gm_stance)
		    {
				act( "$n clambers back to $s feet.", victim, NULL, NULL, TO_ROOM);
				act( "You clamber back to your feet.", victim, NULL, NULL, TO_CHAR);
		    }
		    else
		    {
				act( "$n flips back up to $s feet.", victim, NULL, NULL, TO_ROOM);
				act( "You flip back up to your feet.", victim, NULL, NULL, TO_CHAR);
		    }
		}
		return;
    }
    /*else if ( (mount = victim->mount) != NULL)
    {
		if (victim->mounted == IS_MOUNT)
		{
		    act("$n rolls off $N.",mount,NULL,victim,TO_ROOM);
			act("You roll off $N.",mount,NULL,victim,TO_CHAR);
		}
		else if (victim->mounted == IS_RIDING)
		{
		    act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
		    act("You fall off $N.",victim,NULL,mount,TO_CHAR);
		}
		mount->mount    = NULL;
		victim->mount   = NULL;
		mount->mounted  = IS_ON_FOOT;
		victim->mounted = IS_ON_FOOT;
	}
    */
    if (!IS_NPC(victim) && victim->hit <=-11 && IS_HERO(victim))
    {
        victim->hit = -10;
        if (victim->position == POS_FIGHTING) 
           stop_fighting(victim,TRUE);
        return;
    }

    if ( IS_NPC(victim) || victim->hit <= -11 )
    {
		victim->position = POS_DEAD;
		return;
    }

    if ( victim->hit <= -6 )
		victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 )
    {
     if( !IS_NPC(victim) && IS_SET(victim->pcdata->powers[0], POWER_LIFE))
     {
      victim->hit += 5000;
      victim->position = POS_STANDING;
      REMOVE_BIT(victim->pcdata->powers[0], POWER_LIFE);
      stc("Your fanatisism drives you to keep fighting!\n\r",victim);
      act("$n rises and continues to fight!", victim, NULL, NULL, TO_ROOM);
     }
     else
		victim->position = POS_INCAP;
     }
    else
		victim->position = POS_STUNNED;
    return;
}/* End of void update_pos( CHAR_DATA *victim ) */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{

//    CHAR_DATA *holder;

    if ( ch->fighting != NULL )
    {
		/*bug( "Set_fighting: already fighting", 0 );*/
		return;
    }
    if ( IS_AFFECTED(ch, AFF_SLEEP) )
		affect_strip( ch, gsn_sleep);

    ch->fighting = victim;
    if( !IS_NPC(ch) && !IS_NPC(victim) )
     ch->pcdata->target = str_dup(victim->pcdata->switchname);
    ch->position = POS_FIGHTING;
 /*   if(!IS_NPC(ch) && ch->pcdata->held_by != NULL
    &&  ch->pcdata->held_by != '\0' )
    {
	holder = get_char_world(ch, ch->pcdata->held_by);
        if( holder == NULL)
           ch->pcdata->held_by = strdup( "");
	else
	 do_drelease(holder, holder->pcdata->held);
    }
    if( !IS_NPC(victim) && victim->pcdata->held_by != NULL
    &&  victim->pcdata->held_by != '\0' )
    {
	holder = get_char_world(victim, victim->pcdata->held_by);
	if( holder == NULL )

	do_drelease(holder, holder->pcdata->held);
    }

    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON) 
    && ch->pcdata->held != NULL
    && ch->pcdata->held != '\0' )
	do_drelease(ch, ch->pcdata->held);
    if( !IS_NPC(ch) && IS_CLASS(victim, CLASS_DRAGON)
    && victim->pcdata->held != NULL
    && victim->pcdata->held != '\0' )
	do_drelease(victim, victim->pcdata->held);
   */
    return;
}
void stop_embrace( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if (victim==NULL)
	{
		ch->embracing=NULL;
		return;
	}
	victim->embraced=NULL;
	ch->embracing=NULL;
	return;
}
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
		if ( fch == ch || ( fBoth && fch->fighting == ch ) )
		{
		    fch->fighting	= NULL;
		    fch->position	= POS_STANDING;
			update_pos( fch );
		}
    }
    if( IS_CLASS(ch, CLASS_DRAGON)
    &&  ch->pcdata->held != NULL
    &&  ch->pcdata->held != '\0' 
    &&  !IS_NPC(ch)             
    &&  ch->hit <= 0 )
    {
	do_drelease(ch, ch->pcdata->held);
    } 
    return;
}
void make_corpse( CHAR_DATA *ch )
{
	char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;

    
	if ( IS_NPC(ch) )
    {
		name		= ch->short_descr;
		corpse		= create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
		corpse->timer	= number_range( 4, 8 );
		corpse->value[2]= ch->pIndexData->vnum;
		corpse->level	= ch->level;
    }
    else
    {
		name		= ch->name;
		corpse		= create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
		corpse->timer	= number_range( 25, 30);
    }
	if (IS_SET(ch->extra, EXTRA_ZOMBIE))
		SET_BIT(corpse->quest, QUEST_ZOMBIE);

    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );
   
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
		obj_next = obj->next_content;
		obj_from_char( obj );
		if ( IS_SET( obj->extra_flags, ITEM_VANISH ) )
			extract_obj( obj );
		else
		{
			if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
				SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
			obj_to_obj( obj, corpse );
		}
    }

    /* If you die in the shadowplane, your corpse should stay there */
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
		SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);
    obj_to_room( corpse, ch->in_room );
    return;
}
void death_cry( CHAR_DATA *ch )
{
	ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;

    if ( IS_NPC(ch) )
		msg = "You hear something's death cry.";
    else
		msg = "You hear someone's death cry.";
    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
		EXIT_DATA *pexit;
		if ( ( pexit = was_in_room->exit[door] ) != NULL && pexit->to_room != NULL && pexit->to_room != was_in_room )
		{
			ch->in_room = pexit->to_room;
		    act( msg, ch, NULL, NULL, TO_ROOM );
		}
    }
    ch->in_room = was_in_room;
    return;
}
void make_part( CHAR_DATA *ch, char *argument )
{
    char  arg [MAX_INPUT_LENGTH];
    int vnum;

    argument = one_argument(argument,arg);
    vnum = 0;

    if (arg[0] == '\0')
		return;
    if (!str_cmp(arg,"head")) vnum = OBJ_VNUM_SEVERED_HEAD;
    else if (!str_cmp(arg,"arm")) vnum = OBJ_VNUM_SLICED_ARM;
    else if (!str_cmp(arg,"leg")) vnum = OBJ_VNUM_SLICED_LEG;
    else if (!str_cmp(arg,"heart")) vnum = OBJ_VNUM_TORN_HEART;
    else if (!str_cmp(arg,"turd")) vnum = OBJ_VNUM_FINAL_TURD;
    else if (!str_cmp(arg,"entrails")) vnum = OBJ_VNUM_SPILLED_ENTRAILS;
    else if (!str_cmp(arg,"brain")) vnum = OBJ_VNUM_QUIVERING_BRAIN;
    else if (!str_cmp(arg,"eyeball")) vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
    else if (!str_cmp(arg,"blood")) vnum = OBJ_VNUM_SPILT_BLOOD;
    else if (!str_cmp(arg,"face")) vnum = OBJ_VNUM_RIPPED_FACE;
    else if (!str_cmp(arg,"windpipe")) vnum = OBJ_VNUM_TORN_WINDPIPE;
    else if (!str_cmp(arg,"cracked_head")) vnum = OBJ_VNUM_CRACKED_HEAD;
    else if (!str_cmp(arg,"ear")) vnum = OBJ_VNUM_SLICED_EAR;
    else if (!str_cmp(arg,"nose")) vnum = OBJ_VNUM_SLICED_NOSE;
    else if (!str_cmp(arg,"tooth")) vnum = OBJ_VNUM_KNOCKED_TOOTH;
    else if (!str_cmp(arg,"tongue")) vnum = OBJ_VNUM_TORN_TONGUE;
    else if (!str_cmp(arg,"hand")) vnum = OBJ_VNUM_SEVERED_HAND;
    else if (!str_cmp(arg,"foot")) vnum = OBJ_VNUM_SEVERED_FOOT;
    else if (!str_cmp(arg,"thumb")) vnum = OBJ_VNUM_SEVERED_THUMB;
    else if (!str_cmp(arg,"index")) vnum = OBJ_VNUM_SEVERED_INDEX;
    else if (!str_cmp(arg,"middle")) vnum = OBJ_VNUM_SEVERED_MIDDLE;
    else if (!str_cmp(arg,"ring")) vnum = OBJ_VNUM_SEVERED_RING;
    else if (!str_cmp(arg,"little")) vnum = OBJ_VNUM_SEVERED_LITTLE;
    else if (!str_cmp(arg,"toe")) vnum = OBJ_VNUM_SEVERED_TOE;

    if ( vnum != 0 )
    {
		char buf[MAX_STRING_LENGTH];
		OBJ_DATA *obj;
		char *name;
		name = IS_NPC(ch) ? ch->short_descr : ch->name;
		obj = create_object( get_obj_index( vnum ), 0 );
		if (IS_NPC(ch))
			obj->timer = number_range(2,5);
		else
			obj->timer = -1;
		if (!str_cmp(arg,"head") && IS_NPC(ch))
			obj->value[1] = ch->pIndexData->vnum;
		else if (!str_cmp(arg,"head") && !IS_NPC(ch))
	    {
			ch->pcdata->chobj = obj;
			obj->chobj = ch;
			obj->timer = number_range(2,3);
		}
		else if (!str_cmp(arg,"brain") && !IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH) && IS_HEAD(ch, LOST_HEAD))
		{
			if (ch->pcdata->chobj != NULL)
				ch->pcdata->chobj->chobj = NULL;
			ch->pcdata->chobj = obj;
			obj->chobj = ch;
		    obj->timer = number_range(2,3);
		}

		/* For blood :) KaVir */
		if (vnum == OBJ_VNUM_SPILT_BLOOD) obj->timer = 2;

		/* For voodoo dolls - KaVir */
		if (!IS_NPC(ch))
		{
		    sprintf( buf, obj->name, name );
			free_string( obj->name );
		    obj->name = str_dup( buf );
		}
		else
		{
			sprintf( buf, obj->name, "mob" );
		    free_string( obj->name );
		    obj->name = str_dup( buf );
		}

		sprintf( buf, obj->short_descr, name );
		free_string( obj->short_descr );
		obj->short_descr = str_dup( buf );
	
		sprintf( buf, obj->description, name );
		free_string( obj->description );
		obj->description = str_dup( buf );

    	if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
			SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
		obj_to_room( obj, ch->in_room );
    }
    return;
}
void raw_kill( CHAR_DATA *victim )
{
    
    CHAR_DATA *mount;
    stop_fighting( victim, TRUE );
    //death_cry( victim );
    make_corpse( victim );

    if ( (mount = victim->mount) != NULL)
    {
		if (victim->mounted == IS_MOUNT)
		{
		    act("$n rolls off the corpse of $N.",mount,NULL,victim,TO_ROOM);
		    act("You roll off the corpse of $N.",mount,NULL,victim,TO_CHAR);
		}
		else if (victim->mounted == IS_RIDING)
		{
		    act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
			act("You fall off $N.",victim,NULL,mount,TO_CHAR);
		}
		mount->mount    = NULL;
		victim->mount   = NULL;
		mount->mounted  = IS_ON_FOOT;
		victim->mounted = IS_ON_FOOT;
    }
 
    if ( IS_NPC(victim) )
    {
		victim->pIndexData->killed++;
		extract_char( victim, TRUE );
		return;
    }

    extract_char( victim, FALSE );
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

    REMOVE_BIT(victim->immune, IMM_STAKE);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);

    victim->itemaffect	 = 0;
    victim->loc_hp[0]    = 0;
    victim->loc_hp[1]    = 0;
    victim->loc_hp[2]    = 0;
    victim->loc_hp[3]    = 0;
    victim->loc_hp[4]    = 0;
    victim->loc_hp[5]    = 0;
    victim->loc_hp[6]    = 0;
    victim->armor	 = 100;
    victim->position	 = POS_RESTING;
    victim->hit		 = UMAX( 1, victim->hit  );
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->carry_weight = 0;
    victim->carry_number = 0;
    do_call(victim,"all");
    save_char_obj( victim );
    return;
}
void behead( CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if (IS_NPC(victim))
		return;

    location = victim->in_room;
    stop_fighting( victim, TRUE );
    make_corpse( victim );
	extract_char( victim, FALSE );
    char_from_room(victim);
    char_to_room(victim,location);

    make_part( victim, "head" );

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

    REMOVE_BIT(victim->immune, IMM_STAKE);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);

    victim->itemaffect	 = 0;
    victim->loc_hp[0]    = 0;
    victim->loc_hp[1]    = 0;
    victim->loc_hp[2]    = 0;
    victim->loc_hp[3]    = 0;
    victim->loc_hp[4]    = 0;
    victim->loc_hp[5]    = 0;
    victim->loc_hp[6]    = 0;
    victim->affected_by	 = 0;
    victim->armor	 = 100;
    victim->position	 = POS_STANDING;
    victim->hit		 = 1;
    victim->mana	 = UMAX( 1, victim->mana );
    victim->move	 = UMAX( 1, victim->move );
    victim->hitroll	 = 0;
    victim->damroll	 = 0;
    victim->saving_throw = 0;
    victim->carry_weight = 0;
    victim->carry_number = 0;

    SET_BIT(victim->loc_hp[0],LOST_HEAD);
    SET_BIT(victim->affected_by,AFF_POLYMORPH);
    sprintf(buf,"the severed head of %s",victim->name);
    free_string(victim->morph);
    victim->morph = str_dup(buf);
    do_call(victim,"all");
    save_char_obj( victim );
    return;
}
void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    CHAR_DATA *mount;
    int xp;
    int members;

    /* Monsters don't get kill xp's or alignment changes.P-killing doesn't help either.Dying of mortal wounds or poison doesn't give xp to anyone!  */
    if ( (IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch )
		return;
    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
		if ( is_same_group( gch, ch ) )
			members++;
    }

    if ( members == 0 )
    {
		members = 1;
    }

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
		OBJ_DATA *obj;
		OBJ_DATA *obj_next;

		if ( !is_same_group( gch, ch ) )
			continue;
		xp = xp_compute( gch, victim );
		xp = xp / members;
                get_points(gch, victim);
		sprintf( buf, "You receive %d experience points.\n\r", xp );
		stc( buf, gch );
		if ((mount = gch->mount) != NULL) stc( buf, mount );
			gain_exp( gch, xp );
		for ( obj = ch->carrying; obj != NULL; obj = obj_next )
		{
		    obj_next = obj->next_content;
		    if ( obj->wear_loc == WEAR_NONE )
				continue;
		    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
		    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
			||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
			{
				act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
				act( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );
				obj_from_char( obj );
				obj_to_room( obj, ch->in_room );
		    }
		}
    }
    return;
}

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
    int xp = 0;
    int diff;
        
    diff = (victim->level * 20) - gch->explevel + 5;

    if (diff > 10000)
        diff = 10000;

    if (diff > 39) xp = diff * 40;
    else if (diff > 29) xp = diff * 35;
    else if (diff > 9) xp = diff * 30;
    else if (diff >= 0) xp = 100 + (diff * 20);
    else if (diff >= -10) xp = 100 + (diff * 5);
    else if (diff >= -20) xp = 90 + (diff * 4);
    else return 0;

    // PLayer kills
    if (!IS_NPC(victim)) xp *= 3;
   
    // randomize the rewards 
    xp = number_range (xp * 6/5, xp * 7/6);

    // No exp for zombies and shit like that. 
    if (IS_SET(victim->act, ACT_NOEXP)) xp = 0;

    if (xp > 7500) xp = 7500 + xp / 7;
    if( get_hours_played(gch) <= 4 )
    {
     xp *= 1.5;
     stc("Newbie Exp!\n\r", gch);
    }
    xp *= 3;  /* This was 4, till Kline came along. */
    xp *= config_info->research_exp_mult;
    xp /= config_info->research_exp_div; 
    if( !IS_NPC(gch) && gch->level < 3 && gch->trust < 3 && gch->pcdata->remort > 0)
      xp /= (gch->pcdata->remort + 1);
    if (xp < 0)
        xp = 1;
    xp *= global_exp_mult;
    xp *= exp_who;
    xp *= config_info->ap_exp_mult;
    return xp;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
    static char * const attack_table[] =
    {
		"hit",
		"slice",  "stab",  "slash", "whip", "claw",
		"blast",  "pound", "crush", "grep", "bite",
		"pierce", "suck"
    };
    static char * const attack_table2[] =
    {
		"hits",
		"slices",  "stabs",  "slashes", "whips", "claws",
		"blasts",  "pounds", "crushes", "greps", "bites",
		"pierces", "sucks"
    };

    char buf1[512], buf2[512], buf3[512];
    const char *vs;
    const char *vp;
    const char *attack;
    const char *attack2;
    int damp;
    int bodyloc;
    int recover;
    bool critical = FALSE;
    char punct;

	if (!IS_NPC(ch) && ( IS_CLASS(ch,CLASS_WOLF && IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_REND))
         && dam>0 && number_range(1,7) == 5)) critical=TRUE;
        if( critical && IS_SET(victim->warp, WARP_CRYSTAL))
           dam *= 2;
         if ( dam ==   0 ) { vs = " miss";	vp = " misses";		}
	else if ( dam <=  25 ) { vs = " with laughable force"; vp = " with laughable force";}
    else if ( dam <=  50 ) { vs = " lightly";	vp = " lightly";	}
    else if ( dam <= 100 ) { vs = "";		vp = "";		}
    else if ( dam <= 250 ) { vs = " hard";	vp = " hard";		}
    else if ( dam <= 500 ) { vs = " very hard";	vp = " very hard";	}
    else if ( dam <= 700 ) { vs = " really hard"; vp = " really hard";}
	else if ( dam <= 900 ) { vs = " insanely hard!"; vp = " insanely hard!";}
    else if ( dam <= 1100 ) { vs = " amazingly hard!!";vp = " amazingly!!";}	
    else if ( dam <= 1300 ) { vs = " GODDAMN HARD";vp= " GODDAMN HARD";}
	else if ( dam <= 1550 ) { vs = " -#3LUDICROUSLY HARD#n-";vp= " -#3LUDICROUSLY HARD#n-";}
	else if ( dam <= 1750 ) { vs = " for #3**#1 SOUL WRENCHING DAMAGE #3**#n";vp= " #3**#1 SOUL WRENCHING DAMAGE #3**#n";}
    else                   { vs = " for #1-==*#6 EARTH #3SHATTERING #6DAMAGE #1*==-";vp = " for #1-==*#6 EARTH #3SHATTERING #6DAMAGE #1*==-";}

    /* If victim's hp are less/equal to 'damp', attacker gets a death blow */
    if (IS_NPC(victim))
		damp = 0;
    else 
		damp = -10;
    if ( (victim->hit - dam > damp ) || (dt >= 0 && dt < MAX_SKILL) || (IS_NPC(victim) && IS_SET(victim->act, ACT_NOPARTS)) )
    {
    	punct   = (dam <= 250) ? '.' : '!';
    	if ( dt == TYPE_HIT && !IS_NPC(ch) )
    	{
		    if (dam == 0)
		    {
				sprintf( buf1, "$n%s $N%c[%d]",  vp, punct, dam );
	    		sprintf( buf2, "You%s $N%c[%d]", vs, punct, dam );
		    	sprintf( buf3, "$n%s you%c", vp, punct );
		    }
			else
		    {
		    	sprintf( buf1, "$n hits $N%s%c[%d]",  vp, punct, dam );
				sprintf( buf2, "You hit $N%s%c[%d]", vs, punct, dam );
	    		sprintf( buf3, "$n hits you%s%c[%d]", vp, punct, dam );
				critical = TRUE;
		    }
		    if(  IS_CLASS(ch,CLASS_WOLF) && IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_CLAWS) )
		    {
                     if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW) )
		     {
			sprintf( buf1, "$n's claws hits $N%s%c[%d]", vp, punct, dam);
			sprintf( buf2, "Your claws hits $N%s%c[%d]", vp, punct, dam);
			sprintf( buf3, "$n's claws hits you%s%c[%d]", vp, punct, dam);
		     }
		     else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW) )
		     {
			sprintf( buf1, "$n's flaming claws hits $N%s%c[%d]", vp, punct, dam);
			sprintf( buf2, "Your flaming claws hits $N%s%c[%d]", vp, punct, dam);
			sprintf( buf3, "$n's flaming claws hits you%s%c[%d]", vp, punct, dam);
		     }
                    }
                    if( IS_CLASS(ch, CLASS_DRAGON) && GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
                    {
                        sprintf( buf1, "$n's enormous claws hit $N%s%c[%d]", vp, punct, dam);
                        sprintf( buf2, "Your enormous claws hit $N%s%c[%d]", vp, punct, dam);
                        sprintf( buf3, "$n's enormous claws hit you%s%c[%d]", vp, punct, dam);
                    }
 	}
    	else
    	{
			if ( dt == TYPE_HIT && !IS_NPC(ch))
		        {
		    	attack  = attack_table[dt - TYPE_HIT + 5];
			attack2 = attack_table2[dt - TYPE_HIT + 5];
			}
			else if ( dt >= 0 && dt < MAX_SKILL )
			{
		    	attack = skill_table[dt].noun_damage;
		    	attack2 = skill_table[dt].noun_damage;
			}
			else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]) )
			{
	    		attack  = attack_table[dt - TYPE_HIT];
		    	attack2 = attack_table2[dt - TYPE_HIT];
		    }
			else
			{
				dt  = TYPE_HIT;
				attack  = attack_table[0];
				attack2 = attack_table2[0];
			}
		    if (dam == 0)
		    {
				sprintf( buf1, "$n's %s%s $N%c[%d]",  attack, vp, punct, dam );
	    		sprintf( buf2, "Your %s%s $N%c[%d]",  attack, vp, punct, dam );
		    	sprintf( buf3, "$n's %s%s you%c[%d]", attack, vp, punct, dam );
		    }
		    else
		    {
				if ( dt >= 0 && dt < MAX_SKILL )
				{
				    sprintf( buf1, "$n's %s strikes $N%s%c[%d]",  attack2, vp, punct, dam );
	    		    sprintf( buf2, "Your %s strikes $N%s%c[%d]",  attack, vp, punct, dam );
	    			sprintf( buf3, "$n's %s strikes you%s%c[%d]", attack2, vp, punct, dam );
				}
				else if ((IS_CLASS(ch,CLASS_WOLF) && IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW)))
				{
			        sprintf( buf1, "$n's flaming %s hits $N%s%c[%d]", attack2, vp, punct, dam);
	                sprintf( buf2, "Your flaming %s hits $N%s%c[%d]", attack, vp, punct, dam);
		            sprintf( buf3, "$n's flaming %s hits you%s%c[%d]", attack2, vp, punct, dam);
			    }
    			else
				{
		    	    sprintf( buf1, "$n %s $N%s%c[%d]",  attack2, vp, punct, dam );
				    sprintf( buf2, "You %s $N%s%c[%d]",  attack, vp, punct, dam );
	    		    sprintf( buf3, "$n %s you%s%c[%d]", attack2, vp, punct, dam );
					critical = TRUE;
				}
		    }
			recover = 0;
		    /* Check for weapon resistance - KaVir */
		    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SLASH) && (attack == "slash" || attack == "slice") && dam > 0 )
				recover = number_range(1,dam);
		    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_STAB) && (attack == "stab" || attack == "pierce") && dam > 0 )
				recover = number_range(1,dam);
			if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_SMASH) && (attack == "blast" || attack == "pound"
			|| attack == "crush") && dam > 0 )
				recover = number_range(1,dam);
			if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_ANIMAL) && (attack == "bite" || attack == "claw") && dam > 0 )
				recover = number_range(1,dam);
		    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_MISC) && (attack == "grep" || attack == "suck" || attack == "whip") && dam > 0 )
				recover = number_range(1,dam);
		}
		act( buf1, ch, NULL, victim, TO_NOTVICT );
	    act( buf2, ch, NULL, victim, TO_CHAR );
		act( buf3, ch, NULL, victim, TO_VICT );
		if (critical) critical_hit(ch,victim,dt,dam);
			return;
	}
    if ( dt == TYPE_HIT && !IS_NPC( ch ))
    {
		damp=number_range(1,5);
		if ( damp == 1 )
		{
			act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR);
		    act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT);
		    act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT);
			make_part(victim,"face");
		}
		else if ( damp == 2 )
		{
			act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR);
		    act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT);
		    act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT);
			make_part(victim,"windpipe");
		}
		else if ( damp == 3 )
		{
		    act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR);
		    act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT);
			act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"entrails");
		}
		else if ( damp == 4 )
		{
		    if (!IS_BODY(victim,BROKEN_SPINE))
				SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
			    act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR);
			    act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT);
				act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT);
		}	
		else if ( damp == 5 )
		{
		    act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR);
		    act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT);
		    act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT);
		    if (!IS_BODY(victim,BROKEN_NECK))
			{
				act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR);
				act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM);
				SET_BIT(victim->loc_hp[1],BROKEN_NECK);
			}
		}
		return;
    }
    if ( dt >= 0 && dt < MAX_SKILL )
		attack = skill_table[dt].noun_damage;
    else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]) )
		attack = attack_table[dt - TYPE_HIT];
    else
    {
		dt  = TYPE_HIT;
		attack  = attack_table[0];
    }

    if ( attack == "slash" || attack == "slice" )
    {
		damp=number_range(1,8);
		if ( damp == 1 )
		{
		    act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR);
			act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT);
		    act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"entrails");
		}
		else if ( damp == 2 )
		{
		    act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
			act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
		    act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 3 )
		{
			if (!IS_BODY(victim,CUT_THROAT))
				SET_BIT(victim->loc_hp[1],CUT_THROAT);
		    if (!IS_BLEEDING(victim,BLEEDING_THROAT))
				SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
		    act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR);
		    act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT);
			act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT);
			make_part(victim,"blood");
		}
		else if ( damp == 4 )
		{
		    if (!IS_BODY(victim,CUT_THROAT))
				SET_BIT(victim->loc_hp[1],CUT_THROAT);
		    if (!IS_BLEEDING(victim,BLEEDING_THROAT))
		    	SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
		    act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
		    act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
			act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"blood");
		}
		else if ( damp == 5 )
		{
		    if (!IS_HEAD(victim,BROKEN_SKULL))
		    {
				act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"brain");
				SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
			}
		    else
			{
				act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
				act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
		    }
		}
		else if ( damp == 6 )
		{
		    act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR);
		    act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT);
			act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 7 )
		{
			if (!IS_ARM_L(victim,LOST_ARM))
		    {
		    	act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
				act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
	    		act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"arm");
				SET_BIT(victim->loc_hp[2],LOST_ARM);
				if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
				    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
				if (IS_BLEEDING(victim,BLEEDING_HAND_L))
					REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
			}
			else if (!IS_ARM_R(victim,LOST_ARM))
		    {
		    	act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"arm");
				SET_BIT(victim->loc_hp[3],LOST_ARM);
				if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
				    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
				if (IS_BLEEDING(victim,BLEEDING_HAND_R))
				    REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
			}
			else
			{
				act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
				act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
			}
		}
		else if ( damp == 8 )
		{
		    if (!IS_LEG_L(victim,LOST_LEG))
		    {
				act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"leg");
				SET_BIT(victim->loc_hp[4],LOST_LEG);
				if (!IS_BLEEDING(victim,BLEEDING_LEG_L))
				    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
				if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
				    REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
			}
			else if (!IS_LEG_R(victim,LOST_LEG))
		    {
		    	act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
				act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
	    		act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"leg");
				SET_BIT(victim->loc_hp[5],LOST_LEG);
				if (!IS_BLEEDING(victim,BLEEDING_LEG_R))
				    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
				if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
					REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
			}
		    else
		    {
				act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
				act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
			}
		}
    }
    else if ( attack == "stab" || attack == "pierce" )
    {
		damp=number_range(1,5);
		if ( damp == 1 )
		{
		    act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR);
			act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT);
		    act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"blood");
		}
		else if ( damp == 2 )
		{
			act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
		    act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
		    act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 3 )
		{
		    act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR);
		    act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT);
			act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 4 )
		{
		    act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR);
			act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT);
		    act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT);
		}
		else if ( damp == 5 )
		{
		    act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR);
		    act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT);
			act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT);
		    if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
		    else if (!IS_HEAD(victim,LOST_EYE_R))
				SET_BIT(victim->loc_hp[0],LOST_EYE_R);
		    else if (!IS_HEAD(victim,LOST_EYE_L))
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
		}
    }
    else if ( attack == "blast" || attack == "pound" || attack == "crush" )
    {
		damp=number_range(1,3);
		bodyloc = 0;
		if ( damp == 1)
		{
		    act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR);
			act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT);
		    act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT);
		    if (IS_BODY(victim,BROKEN_RIBS_1 )) {bodyloc += 1;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
		    if (IS_BODY(victim,BROKEN_RIBS_2 )) {bodyloc += 2;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
		    if (IS_BODY(victim,BROKEN_RIBS_4 )) {bodyloc += 4;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
		    if (IS_BODY(victim,BROKEN_RIBS_8 )) {bodyloc += 8;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
		    if (IS_BODY(victim,BROKEN_RIBS_16)) {bodyloc += 16;
				REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
		    bodyloc += number_range(1,3);
		    if (bodyloc > 24)
				bodyloc = 24;
			if (bodyloc >= 16)
			{
				bodyloc -= 16;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);
			}
		    if (bodyloc >= 8)
			{
				bodyloc -= 8;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);
			}
		    if (bodyloc >= 4)
			{
				bodyloc -= 4;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);
			}
		    if (bodyloc >= 2)
			{
				bodyloc -= 2;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);
			}
			if (bodyloc >= 1)
			{
				bodyloc -= 1;
				SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);
			}
		}
		else if ( damp == 2)
		{
			act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_CHAR);
		    act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_NOTVICT);
		    act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, victim, TO_VICT);
			if (!IS_BODY(victim,BROKEN_SPINE))
				SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
		}
		else if ( damp == 3)
		{
		    if (!IS_HEAD(victim,BROKEN_SKULL))
		    {
				act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR);
	    		act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"brain");
				SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
		    }
		    else
		    {
				act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, TO_CHAR);
		    	act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, TO_VICT);
			}
		}
    }
    else if ( !IS_NPC( ch ) && (attack == "bite"))
    {
		act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR);
		act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT);
		act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT);
		make_part(victim,"blood");
		if (!IS_BODY(victim,CUT_THROAT))
		    SET_BIT(victim->loc_hp[1],CUT_THROAT);
		if (!IS_BLEEDING(victim,BLEEDING_THROAT))
		    SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    else if ( !IS_NPC(ch) && (attack == "claw"))
    {
		damp=number_range(1,2);
		if ( damp == 1 )
		{
		    act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
		    act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
			act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
		    make_part(victim,"blood");
		    if (!IS_BODY(victim,CUT_THROAT))
				SET_BIT(victim->loc_hp[1],CUT_THROAT);
		    if (!IS_BLEEDING(victim,BLEEDING_THROAT))
		    	SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
		}
		if ( damp == 2 )
		{
			if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
		    {
		    	act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
	    		act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
				act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"eyeball");
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
			}	
			else if (!IS_HEAD(victim,LOST_EYE_R))
		    {
		    	act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
				act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
	    		act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"eyeball");
				SET_BIT(victim->loc_hp[0],LOST_EYE_R);
			}
		    else if (!IS_HEAD(victim,LOST_EYE_L))
		    {
				act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
	    		act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
		    	act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
		    	make_part(victim,"eyeball");
				SET_BIT(victim->loc_hp[0],LOST_EYE_L);
			}
			else
			{
		    	act("You claw open $N's chest.", ch, NULL, victim, TO_CHAR);
		    	act("$n claws open $N's chest.", ch, NULL, victim, TO_NOTVICT);
				act("$n claws open $N's chest.", ch, NULL, victim, TO_VICT);
			}
		}
    }
    else if ( attack == "whip" )
    {
		act("You entangle $N around the neck, and squeeze the life out of $S.", ch, NULL, victim, TO_CHAR);
		act("$n entangle $N around the neck, and squeezes the life out of $S.", ch, NULL, victim, TO_NOTVICT);
		act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, victim, TO_VICT);
		if (!IS_BODY(victim,BROKEN_NECK))
			SET_BIT(victim->loc_hp[1],BROKEN_NECK);
    }
    else if ( attack == "suck" || attack == "grep" )
    {
		act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_CHAR);
		act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_NOTVICT);
		act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, TO_VICT);
    }
    return;
}
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    char buf [MAX_STRING_LENGTH];

    /* I'm fed up of being disarmed every 10 seconds - KaVir */
    if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5)
		return;
    if (!IS_IMMUNE(ch, IMM_DISARM))
		return;
    if ( ( (obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON )
    {
		if ( ( (obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON )
			return;
    }
    sprintf(buf,"$n disarms you!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf, ch, NULL, victim, TO_VICT    );
    sprintf(buf,"You disarm $N!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_CHAR    );
    sprintf(buf,"$n disarms $N!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    obj_from_char( obj );

    /* Loyal weapons come back ;)  KaVir */
    if (IS_SET(obj->extra_flags, ITEM_LOYAL) && ( !IS_NPC(victim) ) )
    {
        act( "$p leaps back into your hand!", victim, obj, NULL, TO_CHAR );
        act( "$p leaps back into $n's hand!",  victim, obj, NULL, TO_ROOM );
		obj_to_char( obj, victim );
		do_wear( victim, obj->name);
    }
    else if ( IS_NPC(victim) )
		obj_to_char( obj, victim );
    else
		obj_to_room( obj, victim->in_room );
    return;
}
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];

    if (IS_AFFECTED(victim,AFF_FLYING))
		return;
    if( !IS_NPC(victim) && IS_CLASS(victim, CLASS_DRAGON))
    {
     if( IS_SET(GET_POWERS(victim, DRAGON_FLAGS), DRAGON_FLY))
      return;
    }
    if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5)
		return;

    if ( victim->wait == 0)
    {
    	sprintf(buf,"$n trips you and you go down!");
    	ADD_COLOUR(ch,buf,WHITE);
    	act( buf, ch, NULL, victim, TO_VICT    );
    	sprintf(buf,"You trip $N and $E goes down!");
    	ADD_COLOUR(ch,buf,WHITE);
    	act( buf,  ch, NULL, victim, TO_CHAR    );
    	sprintf(buf,"$n trips $N and $E goes down!");
    	ADD_COLOUR(ch,buf,WHITE);
    	act( buf,  ch, NULL, victim, TO_NOTVICT );
		WAIT_STATE( ch,     2 * PULSE_VIOLENCE );
		WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
		victim->position = POS_RESTING;
    }
    return;
}
void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );
	if ((IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)) && ch->pcdata->chobj != NULL)
	{
		stc("You cannot do this as a weapon.\n\r", ch);
		return;
	}

    if ( arg[0] == '\0' )
    {
		stc( "Kill whom?\n\r", ch );
		return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		stc( "They aren't here.\n\r", ch );
		return;
    }
    if ( victim == ch )
    {
		stc( "You cannot kill yourself!\n\r", ch );
		return;
    }

    if ( is_safe( ch, victim ) )
		return;
	if ((ch->class == 0) && ch->in_room->vnum == 3230)
		return;
	if (IS_AFFECTED(victim,AFF_ETHEREAL) && !IS_AFFECTED(ch,AFF_ETHEREAL))
	{
		stc("You cannot attack their vapoury form.\n\r", ch);
		return;
	}
    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
		act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
		return;
    }

    if( victim != ch->fighting )
    {
     if( IS_NPC(victim) )
      sprintf(buf, "#4You turn your attack on #7%s#4!#n\n\r", victim->short_descr);
     else
      sprintf(buf, "#4You turn your attack on #7%s#4!#n\n\r", victim->name);
     stc(buf,ch);
     if( !IS_NPC(victim) && IS_AWAKE(victim) )
     {
      sprintf(buf, "#7%s #4turns their attack towards you!#n\n\r", ch->name);
      stc(buf,victim);
     }
     set_fighting(ch,victim);
     if( victim->fighting == NULL )
      set_fighting(victim,ch);
    }
    else
    {
     stc("#4You are already fighting them!#n\n\r", ch);
     return;
    }
/*
    if ( ch->position == POS_FIGHTING )
    {
		stc( "You do the best you can!\n\r", ch );
		return;
    }*/
    if( IS_NPC(victim) )
    {
     WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
     check_killer( ch, victim );
     multi_hit( ch, victim, TYPE_UNDEFINED );
     return;
    }
    else
    {
     check_killer(ch,victim);
     multi_hit(ch,victim,TYPE_UNDEFINED);
     return;
    }
    return;
}
/* Old circle, new moved to shadow.c
void do_circle (CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument (argument, arg);

    if (IS_NPC (ch))
	return;

    if ((victim = get_char_room (ch, arg)) == NULL) {
	if (ch->fighting == NULL) {
	    stc ("They aren't here.\n\r", ch);
	    return;
	} else
	    victim = ch->fighting;
    }
    if (victim == ch) {
	stc ("How can you circle yourself?\n\r", ch);
	return;
    }
    if (is_safe (ch, victim))
	return;

    if (((obj = get_eq_char (ch, WEAR_WIELD)) == NULL
	 || obj->value[3] != 11)
	&& ((obj = get_eq_char (ch, WEAR_HOLD)) == NULL
	    || obj->value[3] != 11)) {
	stc ("You need to wield a piercing weapon.\n\r", ch);
	return;
    }
    check_killer (ch, victim);
    if (number_percent () > 10)
	multi_hit (ch, victim, gsn_circle);
    else
	damage (ch, victim, 0, gsn_circle);

    WAIT_STATE (ch, 10);
    return;
}*/
void do_backstab( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int dam;
 int chance;
 char buf[MAX_STRING_LENGTH];
 bool go = FALSE;
 bool class = FALSE;

 one_argument( argument, arg );

 if( arg[0] == '\0' )
 {
  stc("Backstab whom?\n\r", ch);
  return;
 }

 if( ( victim = get_char_room( ch, arg ) ) == NULL )
 {
  stc("They aren't here.\n\r", ch);
  return;
 }

 if( victim == ch )
 {
  stc("How can you sneak up on yourself?\n\r", ch);
  return;
 }

 if( is_safe( ch, victim ) )
  return;

 if( victim->fighting !=NULL )
 {
  stc("You can't backstab a fighting person.\n\r", ch);
  return;
 }

 if( victim->hit < (victim->max_hit * 0.85) && victim->position > POS_SLEEPING )
 {
  act( "$N is hurt and suspicious ... you can't sneak up.", ch, NULL, victim, TO_CHAR );
  return;
 }

 if( ch->race >= 12 && !IS_CLASS(ch, CLASS_SHADOW) )
   class = FALSE;
 else if( IS_CLASS(ch, CLASS_SHADOW) )
   class = TRUE;

 check_killer( ch, victim );

 if( class && GET_POWERS(ch, SHADOW_BACKSTAB) > 2 )
  WAIT_STATE( ch, skill_table[gsn_backstab].beats / 2);
 else
  WAIT_STATE( ch, skill_table[gsn_backstab].beats );

 if( ( ch->race < 12 && !class ) || ( class && GET_POWERS(ch, SHADOW_BACKSTAB) < 1 ) )
 {
  stc("You fumble about clumsily; unable to figure out the proper technique!\n\r", ch);
  return;
 }
 
 if( class )
 {
  chance = 10+number_range(5,15)+GET_POWERS(ch, SHADOW_BACKSTAB);
  if( IS_AWAKE( victim ) )  
   dam = number_range(500,750);
  else
   dam = number_range(1000,1500); 
  if( IS_AFFECTED(ch,AFF_HIDE) )
   dam *= 1.5;
  if( IS_AFFECTED(ch,AFF_SNEAK) )
   dam *= 1.5;
  if( GET_POWERS(ch, SHADOW_BACKSTAB) == 5 )
   dam *= 2;
  if( number_range(1,100) < chance )
  {
   go = TRUE;
   dam *= 2;
  }
  if( GET_POWERS(ch, SHADOW_BACKSTAB) < 5 )
   INC_POWERS(ch, SHADOW_LEV_BS);
  if( GET_POWERS(ch, SHADOW_LEV_BS) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_BACKSTAB) )
   && GET_POWERS(ch, SHADOW_BACKSTAB) < 5 )
  {
   stc("Your mastery of backstab has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_BS) = 0;
   INC_POWERS(ch, SHADOW_BACKSTAB);
  }
 }

 else
 {
  dam = number_range(250,500); 
  if( IS_AFFECTED(ch,AFF_HIDE) )
   dam *= 1.30;
  if( IS_AFFECTED(ch,AFF_SNEAK) )
   dam *= 1.30;
  if( !IS_AWAKE(victim) )
   dam *= 1.30;
 }

  sprintf( buf, "You plunge your weapon deep into $N's back! [%d]", dam );
  act( buf, ch, NULL, victim, TO_CHAR    );
  sprintf( buf, "$n plunges $s weapon deep into your back! [%d]", dam );
  act( buf, ch, NULL, victim, TO_VICT    );
  sprintf( buf, "$n plunges $m weapon deep into $N's back! [%d]", dam );
  act( buf, ch, NULL, victim, TO_NOTVICT );
  
  if( go )
  {
   sprintf(buf, "You hear a loud CRACK!");
   do_recho(ch,buf);
  }

  victim->hit -= dam;
  multi_hit(ch,victim,TYPE_UNDEFINED);

 return;
}

/* Original backstab
void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
		stc( "Backstab whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		stc( "They aren't here.\n\r", ch );
		return;
    }

    if ( victim == ch )
    {
		stc( "How can you sneak up on yourself?\n\r", ch );
		return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11 ) && ( ( obj = get_eq_char( ch, WEAR_HOLD 
) ) == NULL
    ||   obj->value[3] != 11 ) )
    {
		stc( "You need to wield a piercing weapon.\n\r", ch );
		return;
    }

    if ( victim->fighting != NULL )
    {
		stc( "You can't backstab a fighting person.\n\r", ch );
		return;
    }

    if (victim->hit <= 5000 )
    {
		act( "$N is hurt and suspicious ... you can't sneak up.", ch, NULL, victim, TO_CHAR );
		return;
    }
    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_BACKSTAB))
		damage( ch, victim, 0, gsn_backstab );
    else if ( !IS_AWAKE(victim) ||  IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_backstab] )
		multi_hit( ch, victim, gsn_backstab );
    else
		damage( ch, victim, 0, gsn_backstab );
    return;
}
*/
void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    int attempt;

    if ( ( victim = ch->fighting ) == NULL )
    {
		if ( ch->position == POS_FIGHTING )
		    ch->position = POS_STANDING;
		stc( "You aren't fighting anyone.\n\r", ch );
		return;
    }
	if(ch->move <= 0)
	{
		stc("You can't find the energy to leave.\n\r",ch);
		return;
	}
  
    if (IS_AFFECTED(ch, AFF_WEBBED))
	{
		stc("You are unable to move with all this sticky webbing on.\n\r",ch);
		WAIT_STATE(ch,12);
		return;
    }
    if (!IS_NPC(ch) && ch->rage >= 100)
    {
     stc("Your rage is too great!\n\r",ch); 
     WAIT_STATE(ch,12);
     return;
    }
    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_WHIRLWIND) && (number_percent() > 30))
    {
      stc("A wall of whirling wind blocks your way.\n\r", ch);
      WAIT_STATE(ch, 12);
      return;
    }
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_WOLF) && IS_SET(GET_POWERS(victim,WOLF_BITS),BIT_JAW) && (number_percent() > 25))
    {
     act("Not with $N's jaws clamped on your neck!",ch,NULL,victim,TO_CHAR);		
     act("$n cannot escape your clamped jaws!",ch,NULL,victim,TO_VICT);
     act("$n cant escape $N's clamped jaws!",ch,NULL,victim,TO_NOTVICT);
     WAIT_STATE(ch,12);
     return;
    }
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_VAMPIRE) && IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_COIL) && (number_percent() > 25))
    {
     act("Not with $N's body coiled around you!",ch,NULL,victim,TO_CHAR);
     act("$n cannot escape your coiled body!",ch,NULL,victim,TO_VICT);
     act("$n cant escape $N's coiled body!",ch,NULL,victim,TO_NOTVICT);
     WAIT_STATE(ch,12);
     return;
    }
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WOLF) && IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_JAW) )
    {
     act("Not with your jaws clamped around $N!",ch,NULL,victim,TO_CHAR);
     WAIT_STATE(ch,12);
     return;
    }
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) && IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_COIL) )
    {
     act("Not with your body coiled around $N!",ch,NULL,victim,TO_CHAR);
     WAIT_STATE(ch,12);
     return;
    }
    was_in = ch->in_room;
    for ( attempt = 0; attempt < 6; attempt++ )
    {
		EXIT_DATA *pexit;
		int door;
		door = number_door( );
		if( ((pexit = was_in->exit[door] ) == 0) ||( pexit->to_room == NULL)
		|| (IS_SET(pexit->exit_info, EX_CLOSED) && (!IS_AFFECTED(ch, AFF_PASS_DOOR))) 
                || ( IS_NPC(ch) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) ) )
			continue;
		move_char( ch, door );
		if ( ( now_in = ch->in_room ) == was_in )
			continue;
        ch->in_room = was_in;
		act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
		ch->in_room = now_in;
		if ( !IS_NPC(ch) )
		    stc( "You flee from combat!  Coward!\n\r", ch );
		WAIT_STATE(ch,12);
		stop_fighting( ch, TRUE );
                check_trap(ch);
		return;
    }
	WAIT_STATE(ch,12);
    stc( "You were unable to escape!\n\r", ch );
    return;
}
void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if( nogroup && IS_SET(ch->act,PLR_NO_GROUP) )
    {
     stc("You fight solo, no need to rescue others!\n\r",ch);
     return;
    }
    if ( arg[0] == '\0' )
    {
		stc( "Rescue whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		stc( "They aren't here.\n\r", ch );
		return;
    }

    if ( victim == ch )
    {
		stc( "What about fleeing instead?\n\r", ch );
		return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
		stc( "Doesn't need your help!\n\r", ch );
		return;
    }

    if ( ch->fighting == victim )
    {
		stc( "Too late.\n\r", ch );
		return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
		stc( "That person is not fighting right now.\n\r", ch );
		return;
    }
    if( nogroup && IS_SET(victim->act,PLR_NO_GROUP) )
    {
     stc("They fight solo, no need to rescue them!\n\r",ch);
     return;
    }
    if ( is_safe(ch, fch) || is_safe(ch, victim) )
		return;
    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
    {
		stc( "You fail the rescue.\n\r", ch );
		return;
    }
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );
    check_killer( ch, fch );
    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}
void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int dam;
    int stance;
    int hitpoints;
	
	if (IS_NPC(ch))
		return;

    if ( !IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
    {
		stc("First you should learn to kick.\n\r", ch );
		return;
    }
    if ( ( victim = ch->fighting ) == NULL )
    {
		stc( "You aren't fighting anyone.\n\r", ch );
		return;
    }
    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_kick] )
    	dam = number_range(1,4);
    else
    {
    	dam = 0;
    	damage( ch, victim, dam, gsn_kick );
		return;
    }
    dam += (class_damroll(ch)/number_range(8,4));
    if (dam == 0)
		dam = 1;
    if ( !IS_AWAKE(victim) )
		dam *= 2;
    if ( !IS_NPC(ch) )
		dam = dam + (dam * ((ch->wpn[0]+1) / 100));

    if ( !IS_NPC(ch) )
    {
		stance = ch->stance[0];
		if ( IS_STANCE(ch, STANCE_NORMAL) )
			dam *= 1.25;
		else 
			dam *= 1.50;
    }

    if ( !IS_NPC(victim) && dam > 750 )
		dam = 750;
    if ( dam <= 0 )
		dam = 2;

    hitpoints = victim->hit;
    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_KICK))
    {
		victim->hit += dam;
		damage( ch, victim, dam, gsn_kick );
		victim->hit = hitpoints;
    }
    else damage( ch, victim, dam, gsn_kick );
    return;
}

void do_punch( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;

    one_argument(argument,arg);

    if ( IS_NPC(ch) )
		return;

    if ( ch->level < skill_table[gsn_punch].skill_level)
    {
		stc( "First you should learn to punch.\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		stc( "You aren't fighting anyone.\n\r", ch );
		return;
    }

    if ( ch == victim )
    {
		stc( "You cannot punch yourself!\n\r", ch );
		return;
    }

    if (is_safe(ch,victim))
		return;

    if ( victim->hit < victim->max_hit )
    {
		stc( "They are hurt and suspicious.\n\r", ch );
		return;
    }

    if ( victim->position < POS_FIGHTING )
    {
		stc( "You can only punch someone who is standing.\n\r", ch );
		return;
    }

    act("You draw your fist back and aim a punch at $N.",ch,NULL,victim,TO_CHAR);
    act("$n draws $s fist back and aims a punch at you.",ch,NULL,victim,TO_VICT);
    act("$n draws $s fist back and aims a punch at $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, skill_table[gsn_punch].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_punch] )
    	dam = number_range(1,4);
    else
    {
    	dam = 0;
    	damage( ch, victim, dam, gsn_punch );
		return;
    }

    dam += class_damroll(ch);
    if (dam == 0)
		dam = 1;
    if ( !IS_AWAKE(victim) )
		dam *= 2;
    if ( !IS_NPC(ch) )
		dam = dam + (dam * (ch->wpn[0] / 100));
    if ( dam <= 0 )
		dam = 1;

    damage( ch, victim, dam, gsn_punch );
    if (victim == NULL || victim->position == POS_DEAD || dam < 1)
		return;
    if (victim->position == POS_FIGHTING)
		stop_fighting(victim,TRUE);
    if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
    {
    	act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    	act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
		SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_JAW))
    {
    	act("Your jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
    	act("$n's jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
		SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
    act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
    victim->position = POS_STUNNED;
    return;
}
void do_berserk( CHAR_DATA *ch, char *argument )
{
	char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    int number_hit = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
	if ((IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)) && ch->pcdata->chobj != NULL)
	{
		stc("You cannot do this as a weapon.\n\r", ch);
		return;
	}


    if ( ch->level < skill_table[gsn_berserk].skill_level)
    {
		stc( "You are not wild enough to go berserk.\n\r", ch );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_berserk].beats );
    if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
    {
    	act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
    	act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
	return;
    }

    act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
    act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
		vch_next	= vch->next;
		if (number_hit > 4)
			continue;
		if ( vch->in_room == NULL )
			continue;
		if ( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
			continue;
		if ( ch == vch )
		    continue;
		if ( vch->in_room == ch->in_room )
		{
			if ( ( mount = ch->mount ) != NULL )
			{
				if ( mount == vch )
					continue;
			}
			if (is_safe(ch, vch))
				continue;
			if (can_see(ch,vch))
			{
                         if( check_beast(ch) )
                         {
                          if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA)
                           && !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
                          {
                           if( GET_POWERS(ch,VAMP_BEAST) < 200 )
                            GET_POWERS(ch,VAMP_BEAST)++;
                           if( GET_POWERS(ch,VAMP_BEAST) == 100 )
                            stc("You feel your beast consume you!\n\r",ch);            
                           else
                            stc("You feel your beast taking more control.\n\r",ch);
                          }
                         }
				multi_hit( ch, vch, TYPE_UNDEFINED );
                number_hit++;	        
		    }
		}
    }
    return;
}
void do_hurl( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *victim;
    CHAR_DATA       *mount;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *pexit;
    EXIT_DATA       *pexit_rev;
    char            buf       [MAX_INPUT_LENGTH];
    char            direction [MAX_INPUT_LENGTH];
    char            arg1      [MAX_INPUT_LENGTH];
    char            arg2      [MAX_INPUT_LENGTH];
    int             door;
    int             rev_dir;
    int             dam;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1 )
    {
		stc("Maybe you should learn the skill first?\n\r",ch);
		return;
    }

    if ( arg1[0] == '\0' )
    {
		stc("Who do you wish to hurl?\n\r", ch);
		return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
		stc("They are not here.\n\r", ch);
		return;
    }

    if ( victim == ch )
    {
		stc( "How can you hurl yourself?\n\r", ch );
		return;
    }

    if ( is_safe( ch, victim ) )
		return;

    if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
    {
		stc("But they have someone on their back!\n\r",ch);
		return;
    }
    else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
		stc("But they are riding!\n\r",ch);
		return;
    }

    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
    {
		stc("You are unable to get their feet of the ground.\n\r",ch);
		return;
    }

    if ( IS_NPC(victim) && victim->level > 500 )
    {
		stc("You are unable to get their feet of the ground.\n\r",ch);
		return;
    }

    if ( (victim->hit < victim->max_hit) || ( victim->position == POS_FIGHTING && victim->fighting != ch) )
    {
		act( "$N is hurt and suspicious, and you are unable to approach $M.",
	    ch, NULL, victim, TO_CHAR );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_hurl].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_hurl] )
    {
	    stc("You are unable to get their feet of the ground.\n\r",ch);
	    multi_hit( victim, ch, TYPE_UNDEFINED );
	    return;
    }

    rev_dir = 0;

    if ( arg2[0] == '\0' )
	door = number_range(0,3);
    else
    {
    	if      ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "north" ) )
			door = 0;
    	else if ( !str_cmp( arg2, "e" ) || !str_cmp( arg2, "east" ) )
			door = 1;
    	else if ( !str_cmp( arg2, "s" ) || !str_cmp( arg2, "south" ) )
			door = 2;
    	else if ( !str_cmp( arg2, "w" ) || !str_cmp( arg2, "west" ) )
			door = 3;
    	else
    	{
			stc("You can only hurl people north, south, east or west.\n\r", ch);
			return;
    	}
    }

    if (door == 0)
	{
		sprintf(direction,"north");
		rev_dir = 2;
	}
    if (door == 1)
	{
		sprintf(direction,"east");
		rev_dir = 3;
	}
    if (door == 2)
	{
		sprintf(direction,"south");
		rev_dir = 0;
	}
    if (door == 3) 
	{
		sprintf(direction,"west");
		rev_dir = 1;
	}

    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
		sprintf(buf,"$n hurls $N into the %s wall.", direction);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		sprintf(buf,"You hurl $N into the %s wall.", direction);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n hurls you into the %s wall.", direction);
		act(buf,ch,NULL,victim,TO_VICT);
		dam = number_range(ch->level, (ch->level * 4));
		victim->hit = victim->hit - dam;
		update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch))
			ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch))
			victim->mdeath = victim->mdeath + 1;
		if (victim->position == POS_DEAD)
		{
			raw_kill(victim);
			return;
		}
		return;
    }
    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim,AFF_PASS_DOOR ) && !IS_AFFECTED(victim,AFF_ETHEREAL) )
    {
		if( ch->in_room->vnum >= 26000 && ch->in_room->vnum <= 26206 )
		{
		  stc("They ricochet off the closed exit and land on your dumb ass!\n\r", ch);
		  ch->hit -= 5000;
		  return;
		}		

		if (IS_SET(pexit->exit_info, EX_LOCKED))
		    REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		if (IS_SET(pexit->exit_info, EX_CLOSED))
		    REMOVE_BIT(pexit->exit_info, EX_CLOSED);
		sprintf(buf,"$n hoists $N in the air and hurls $M %s.", direction);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		sprintf(buf,"You hoist $N in the air and hurl $M %s.", direction);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n hurls you %s, smashing you through the %s.", direction,pexit->keyword);
		act(buf,ch,NULL,victim,TO_VICT);
		sprintf(buf,"There is a loud crash as $n smashes through the $d.");
		act(buf,victim,NULL,pexit->keyword,TO_ROOM);
		if ( ( to_room   = pexit->to_room  ) != NULL &&  ( pexit_rev = to_room->exit[rev_dir] ) != NULL
		&&   pexit_rev->to_room == ch->in_room &&   pexit_rev->keyword != NULL )
		{
			if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
				REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
		    if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
			REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
		    if (door == 0)
				sprintf(direction,"south");
			if (door == 1)
				sprintf(direction,"west");
		    if (door == 2)
				sprintf(direction,"north");
		    if (door == 3)
				sprintf(direction,"east");
			char_from_room(victim);
		    char_to_room(victim,to_room);
			sprintf(buf,"$n comes smashing in through the %s $d.", direction);
		    act(buf,victim,NULL,pexit->keyword,TO_ROOM);
		    dam = number_range(ch->level, (ch->level * 6));
			victim->hit = victim->hit - dam;
		    update_pos(victim);
    	    if (IS_NPC(victim) && !IS_NPC(ch))
				ch->mkill = ch->mkill + 1;
    	    if (!IS_NPC(victim) && IS_NPC(ch))
				victim->mdeath = victim->mdeath + 1;
			if (victim->position == POS_DEAD)
			{
				raw_kill(victim);
				return;
			}
		}
    }
    else
    {
		sprintf(buf,"$n hurls $N %s.", direction);
		act(buf,ch,NULL,victim,TO_NOTVICT);
		sprintf(buf,"You hurl $N %s.", direction);
		act(buf,ch,NULL,victim,TO_CHAR);
		sprintf(buf,"$n hurls you %s.", direction);
		act(buf,ch,NULL,victim,TO_VICT);
		if (door == 0)
			sprintf(direction,"south");
		if (door == 1)
			sprintf(direction,"west");
		if (door == 2)	
			sprintf(direction,"north");
		if (door == 3)
			sprintf(direction,"east");
		char_from_room(victim);
		char_to_room(victim,to_room);
		sprintf(buf,"$n comes flying in from the %s.", direction);
		act(buf,victim,NULL,NULL,TO_ROOM);
		dam = number_range(ch->level, (ch->level * 2));
		victim->hit = victim->hit - dam;
		update_pos(victim);
    	if (IS_NPC(victim) && !IS_NPC(ch))
			ch->mkill = ch->mkill + 1;
    	if (!IS_NPC(victim) && IS_NPC(ch))
			victim->mdeath = victim->mdeath + 1;
		if (victim->position == POS_DEAD)
		{
			raw_kill(victim);
			return;
		}
    }
    return;
}
void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    if ( !IS_NPC(ch) && ch->level < skill_table[gsn_disarm].skill_level)
    {
		stc( "You don't know how to disarm opponents.\n\r", ch );
		return;
    }

    if ( ( get_eq_char( ch, WEAR_WIELD ) == NULL ) && ( get_eq_char( ch, WEAR_HOLD  ) == NULL ) )
    {
		stc( "You must wield a weapon to disarm.\n\r", ch );
		return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
		stc( "You aren't fighting anyone.\n\r", ch );
		return;
    }

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )  &&   ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL ) )
    {
		stc( "Your opponent is not wielding a weapon.\n\r", ch );
		return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + victim->level - ch->level;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
		stc( "You failed.\n\r", ch );
    else if ( IS_NPC(ch) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
		disarm( ch, victim );
    else
		stc( "You failed.\n\r", ch );
    return;
}
void do_sla( CHAR_DATA *ch, char *argument )
{
    stc( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}
void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
		stc( "Slay whom?\n\r", ch );
		return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
		stc( "They aren't here.\n\r", ch );
		return;
    }

    if ( ch == victim )
    {
		stc( "Suicide is a mortal sin.\n\r", ch );
		return;
    }

    if ( !IS_NPC(victim) && victim->level >= ch->level )
    {
		stc( "You failed.\n\r", ch );
		return;
    }

    act( "You slay $M in cold blood!",  ch, NULL, victim, TO_CHAR    );
    act( "$n slays you in cold blood!", ch, NULL, victim, TO_VICT    );
    act( "$n slays $N in cold blood!",  ch, NULL, victim, TO_NOTVICT );
    raw_kill( victim );
    return;
}
void do_killperson( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
		return;
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
		return;
    stc( "You have been KILLED!!\n\r\n\r", victim );
    if (IS_NPC(victim) && !IS_NPC(ch))
		ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch))
		victim->mdeath = victim->mdeath + 1;
    raw_kill( victim );
    return;
}

void do_crack( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    OBJ_DATA  *right;
    OBJ_DATA  *left;

    right = get_eq_char( ch, WEAR_WIELD );
    left = get_eq_char( ch, WEAR_HOLD );
    if (right != NULL && right->pIndexData->vnum == 12)
		obj = right;
    else if (left != NULL && left->pIndexData->vnum == 12)
		obj = left;
    else
    {
		stc("You are not holding any heads.\n\r",ch);
		return;
    }
    act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR);
    act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM);
    act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_CHAR);
    if (obj->chobj != NULL)
    {
		act("$p cracks open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_NOTVICT);
    	act("$p crack open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_VICT);
	}
    else
 		act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_ROOM);
	crack_head(ch,obj,obj->name);
    obj_from_char(obj);
    extract_obj(obj);
}
void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument )
{
    CHAR_DATA *victim;
    MOB_INDEX_DATA *pMobIndex; 

    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char       buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if (str_cmp(arg2,"mob") && obj->chobj != NULL && !IS_NPC(obj->chobj) && IS_AFFECTED(obj->chobj,AFF_POLYMORPH))
    {
		victim = obj->chobj;
		make_part(victim,"cracked_head");
		make_part(victim,"brain");
		sprintf(buf,"the quivering brain of %s",victim->name);
		free_string(victim->morph);
		victim->morph = str_dup(buf);
		return;
    }
    else if (!str_cmp(arg2,"mob"))
    {
		if ( ( pMobIndex = get_mob_index( obj->value[1] ) ) == NULL )
			return;
		victim = create_mobile( pMobIndex );
		char_to_room(victim,ch->in_room);
		make_part(victim,"cracked_head");
		make_part(victim,"brain");
		extract_char(victim,TRUE);
		return;
    }
    else
    {
		if ( ( pMobIndex = get_mob_index( 30002 ) ) == NULL )
			return;
		victim = create_mobile( pMobIndex );

		sprintf( buf, capitalize(arg2) );
		free_string( victim->short_descr );
		victim->short_descr = str_dup( buf );
		char_to_room(victim,ch->in_room);
		make_part(victim,"cracked_head");
		make_part(victim,"brain");
		extract_char(victim,TRUE);
		return;
    }
    return;
}

/* Voodoo skill by KaVir */

void do_voodoo( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    char       buf [MAX_INPUT_LENGTH];
    char     part1 [MAX_INPUT_LENGTH];
    char     part2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0')
    {
		stc( "Who do you wish to use voodoo magic on?\n\r", ch );
		return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
		stc( "You are not holding a voodoo doll.\n\r", ch );
		return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
		stc( "They are not here.\n\r", ch );
		return;
    }

    if ( IS_NPC(victim) )
    {
		stc( "Not on NPC's.\n\r", ch );
		return;
    }

    sprintf(part2,obj->name);
    sprintf(part1,"%s voodoo doll",victim->name);

    if ( str_cmp(part1,part2) )
    {
		sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
		stc( buf, ch );
		return;
    }

    if ( arg2[0] == '\0')
		stc( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
    else if ( !str_cmp(arg2, "stab") )
    {
		WAIT_STATE(ch,12);
		act("You stab a pin through $p.", ch, obj, NULL, TO_CHAR); 
 		act("$n stabs a pin through $p.", ch, obj, NULL, TO_ROOM);
		if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
			return;
		act("You feel an agonising pain in your chest!", victim, NULL, NULL, TO_CHAR);
 		act("$n clutches $s chest in agony!", victim, NULL, NULL, TO_ROOM);
    }
    else if ( !str_cmp(arg2, "burn") )
    {
		WAIT_STATE(ch,12);
		act("You set fire to $p.", ch, obj, NULL, TO_CHAR);
 		act("$n sets fire to $p.", ch, obj, NULL, TO_ROOM);
		act("$p burns to ashes.", ch, obj, NULL, TO_CHAR);
 		act("$p burns to ashes.", ch, obj, NULL, TO_ROOM);
		obj_from_char(obj);
		extract_obj(obj);
		if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
			return;
		if (IS_AFFECTED(victim,AFF_FLAMING) )
			return;
		SET_BIT(victim->affected_by, AFF_FLAMING);
		act("You suddenly burst into flames!", victim, NULL, NULL, TO_CHAR);
	 	act("$n suddenly bursts into flames!", victim, NULL, NULL, TO_ROOM);
    }
    else if ( !str_cmp(arg2, "throw") )
    {
		WAIT_STATE(ch,12);
		act("You throw $p to the ground.", ch, obj, NULL, TO_CHAR);
	 	act("$n throws $p to the ground.", ch, obj, NULL, TO_ROOM);
		obj_from_char(obj);
		obj_to_room(obj, ch->in_room);
		if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO))
			return;
		if (victim->position < POS_STANDING)
			return;
		if (victim->position == POS_FIGHTING)
		    stop_fighting(victim, TRUE);
		act("A strange force picks you up and hurls you to the ground!", victim, NULL, NULL, TO_CHAR);
 		act("$n is hurled to the ground by a strange force.", victim, NULL, NULL, TO_ROOM);
		victim->position = POS_RESTING;
		victim->hit = victim->hit - number_range(ch->level,(5*ch->level));
		update_pos(victim);
		if (victim->position == POS_DEAD && !IS_NPC(victim))
		{
			do_killperson(ch,victim->name);
			return;
		}
    }
    else
		stc( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
    return;
}

bool has_timer(CHAR_DATA *ch) 
{
	if (ch->fight_timer > 0 && !IS_NPC(ch)) 
	{
		stc("Not until your fight timer runs out!\n\r", ch );
		return TRUE;
	}
	return FALSE;
}

void do_bounty( CHAR_DATA *ch, char *argument)
{
  int bounty;
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
  argument = one_argument( argument, arg1);
  argument = one_argument( argument, arg2);
 
  if( arg1[0] == '\0')
   {
    stc("Syntax: bounty <person> <qp amount>.\n\r", ch);
    return;
   } 
  if((victim = get_char_world(ch, arg1)) == NULL)
  {
   stc("Put a bounty on who?\n\r", ch);
   return;
  }
  if( !is_number(arg2))
  {
   stc("That is not a number.\n\r", ch);
   return;
  }
  if( IS_NPC(ch) || IS_NPC(victim))
  return;
  bounty = atoi(arg2);
  if(bounty < 50)
  {
    stc("Bounty must be at least 50 qps.\n\r", ch);
    return;
  }
  if( (ch->level < 3)||( victim->level < 3))
  {
   stc("You and the hunted must be avatars.\n\r", ch);
   return;
  }
  if( victim->level > 6)
  {
   stc("NOT ON IMMORTALS.\n\r", ch);
   return;
  }
  if( ch->pcdata->quest < bounty)
  {
   stc("You dont have enough quest points for that bounty.\n\r",ch);
   return;
  }
  ch->pcdata->quest -= bounty;
  victim->pcdata->bounty[0] = 1;
  victim->pcdata->bounty[1] += bounty;
  sprintf( buf, "%s has put a %d qp bounty on %s's head.\n\r", ch->name, bounty, victim->name);
  do_info(ch, buf);
  return;
}

void bounty_reward( CHAR_DATA *ch, CHAR_DATA *victim)
{
  char buf[MAX_STRING_LENGTH];
  if( IS_NPC(victim) || IS_NPC(ch))
  return;
  ch->pcdata->quest += victim->pcdata->bounty[1];
  victim->pcdata->bounty[0] = 0;
  victim->pcdata->bounty[1] = 0;
  sprintf( buf, "%s claims the bounty on %s's head.", ch->name, victim->name);
  do_info( ch, buf);
  return;
}

bool has_bounty( CHAR_DATA *ch)
{
  if( IS_NPC(ch))
   return FALSE;
  if(ch->pcdata->bounty[0] != 0)
  return TRUE;
  else
  return FALSE;
}

void do_final_blow( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int ws, dam, i = 0;
 int mult = 1.010;
 int blah = number_range(1,10);
 int ap;
 
 if( IS_NPC(ch) )
  return;
  ap = ( ASKILL(ch, AS_FINAL) * 50 );
 if( ASKILL(ch, AS_FINAL) < 1 )
 {
  stc("You have no knowledge of the final blow technique!\n\r", ch);
  return;
 }
 if( ch->fighting == NULL )
 {
  stc("You need to be fighting!\n\r", ch);
  return;
 }
 if( IS_NPC(ch->fighting) )
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( ch->fighting != NULL && (ch->hit > (ch->max_hit * 0.25)) )
 {   
  stc("You are not yet desparate enough to attempt such a maneuver!\n\r", ch);
  return;
 }
 if( ch->fighting != NULL && ch->pcdata->apoints < ap )
 {
  stc("You lack the ability points for such a maneuver!\n\r", ch);
  return;
 }
 switch(blah)
 {
  default:
   log_string("*** do_final_blow returned NULL ***");
   break;
  case 1:
   ws = 12;
   dam = number_range(500,1500);
   for( i = 0; i < ASKILL(ch, AS_FINAL); i++ )
    dam *= mult;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   act2("You focus dark matter energies and drain the lifeforce of $N! [$D]",ch,NULL,victim,TO_CHAR,dam);
   act2("$n focuses dark matter energies and drains your lifeforce! [$D]",ch,NULL,victim,TO_VICT,dam);
   act2("$n focuses dark matter energies and drains the lifeforce of $N! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
   ch->hit += dam;
   victim->hit -=dam;
   break;
  case 2:
   ws = 18;
   dam = number_range(1000,2000);
   for( i = 0; i < ASKILL(ch, AS_FINAL); i++ )
    dam *= mult;
   act2("You draw forth primal energies from deep within and close your wounds! [$D]",ch,NULL,victim,TO_CHAR,dam);
   act2("$n draws forth primal energies from deep within, and closes $s wounds! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
   ch->hit += dam;
   break;
  case 3:
   ws = 15;
   act("You move about in a crazed manner and nothing happens!",ch,NULL,victim,TO_CHAR);
   act("$n moves about in a crazed manner and nothing happens!",ch,NULL,victim,TO_NOTVICT);
   break;
  case 4:
   ws = 12;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   act("You call upon fallen warriors past and send a wave a force charging into $N!",ch,NULL,victim,TO_CHAR);
   act("$n calls upon fallen warriors past and sends a wave of force charging into you!",ch,NULL,victim,TO_VICT);
   act("$n calls upon fallen warriors past and sends a wave of force charging into $N!",ch,NULL,victim,TO_NOTVICT);
   set_stun(victim,3);
   break;
  case 5:
   ws = 6;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   act("You mutter a silent prayer to God, and he responds!",ch,NULL,victim,TO_CHAR);
   act("$n mutters a silent prayer to God, and he responds!",ch,NULL,victim,TO_NOTVICT);
   stop_fighting(ch,TRUE);
   stop_fighting(victim,TRUE);
   break;
  case 6:
   ws = 24;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   act("You let forth a massive scream, and charge madly at $N!",ch,NULL,victim,TO_CHAR);
   act("$n lets forth a massive scream, and charges madly at you!",ch,NULL,victim,TO_VICT);
   act("$n lets forth a massive scream, and charges madly at $N!",ch,NULL,victim,TO_NOTVICT);
   multi_hit(ch,victim,TYPE_UNDEFINED);
   multi_hit(ch,victim,TYPE_UNDEFINED);
   break;
  case 7:
   ws = 18;
   dam = number_range(500,1500);
   for( i = 0; i < ASKILL(ch, AS_FINAL); i++ )
    dam *= mult;
   act2("You feel rejuvinated as the forces of nature heal your spirit! [$D]",ch,NULL,victim,TO_CHAR,dam);
   act2("$n looks rejuvinated as the forces of nature heal $s spirit! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
   ch->hit += dam;
   ch->mana += dam;
   ch->move += dam;
   break;
  case 8:
   ws = 12;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   act("You focus your energies and strike at $N with a blast of elemental forces!",ch,NULL,victim,TO_CHAR);
   act("$n focuses $s energies and strikes at you with a blast of elemental forces!",ch,NULL,victim,TO_VICT);
   act("$n focuses $s energies and strikes at $N with a blast of elemental forces!",ch,NULL,victim,TO_NOTVICT);
   spell_dispel_magic(skill_lookup("dispel magic"),1000,ch,victim );
   spell_lightning_breath(skill_lookup("lightning breath"),1000,ch,victim );
   spell_frost_breath(skill_lookup("frost breath"),1000,ch,victim );
   spell_fire_breath(skill_lookup("fire breath"),1000,ch,victim );
   spell_acid_breath(skill_lookup("acid breath"),1000,ch,victim );
   break;
  case 9:
   ws = 18;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   dam = number_range(500,1500);
   for( i = 0; i < ASKILL(ch, AS_FINAL); i++ )
    dam *= mult;
   act2("You focus your hatred into a ball of negative energies and engulf $N in it! [$D]",ch,NULL,victim,TO_CHAR,dam);
   act2("$n focuses $s hatred into a ball of negative energies and engulfs you in it! [$D]",ch,NULL,victim,TO_VICT,dam);
   act2("$n focuses $s hatred into a ball of negative energies and engulfs $N in it! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
   victim->hit -= dam;
   victim->mana -= dam;
   victim->move -= dam;
   break;
  case 10:
   ws = 24;
   victim = ch->fighting;
   if( victim == NULL )
    break;
   dam = (victim->hit * .20);
   act2("You bond your spirit to $N and are rejuvinated by their lifeforce! [$D]",ch,NULL,victim,TO_CHAR,dam);
   act2("$n bonds $s spirit to you and is rejuvinated by your lifeforce! [$D]",ch,NULL,victim,TO_VICT,dam/2);
   act2("$n bonds $s spirit to $N and is rejuvinated by $s lifeforce! [$D]",ch,NULL,victim,TO_NOTVICT,dam/2);
   ch->hit += dam;
   ch->mana += dam;
   ch->move += dam;
   victim->hit -= dam/2;
   victim->mana -= dam/2;
   victim->move -= dam/2;
   break;
 }
 ch->pcdata->apoints -= ap;
 update_pos(ch);
 if( victim != NULL )
  update_pos(victim);
 WAIT_STATE(ch,ws);
 return;
  
}

bool is_vorpal( CHAR_DATA *ch )
{
 OBJ_DATA *obj;

 obj = get_eq_char(ch,WEAR_WIELD);

 if( IS_CLASS(ch,CLASS_MONK) )
  return FALSE;
 if( ch->race >= 25 )
  return TRUE;
 if( obj == NULL || obj->item_type != ITEM_WEAPON)
  return FALSE;
 else if( (obj->value[0] / 1000) == 18 )
  return TRUE;

 return FALSE;
}

void decap_gain( CHAR_DATA *ch, CHAR_DATA *victim, int type )
{
 char buf[MAX_STRING_LENGTH];

 switch(type)
 {
  default:
   log_string("*** NULL passed to decap_gain ***");
   break;
  case 1:
   if( is_gen_dox(ch,victim) && !deathmatch )
    do_paradox(ch,"self");
   if( is_gen_kill(ch,victim) && !deathmatch )
   {
    if( ch->generation < victim->generation )
     ch->generation += 2;
    else if( ch->generation == victim->generation )
     ch->generation++;
    if( victim->generation > 1 )
     victim->generation--;
    check_warp(ch);
    ch->pkill++;
    victim->pdeath++;
    victim->level = 2;
    victim->trust = 2;
    victim->exp /= 2;
    ch->exp += victim->exp;
    ch->fight_timer += 15;   
    get_points(ch,victim);   
    victim->stuntimer = 0;
    ch->pcdata->target = NULL;
    victim->pcdata->target = NULL;
    register_kill(ch,victim);
    if( has_bounty(victim) )
     bounty_reward(ch,victim);
    if( IS_CLASS(victim,CLASS_SHADOW) && IS_SET(GET_POWERS(victim,SHADOW_BITS),SHADOW_FOLLOW) )
    {
     REM_BIT_POWERS(victim,SHADOW_BITS,SHADOW_FOLLOW);
     GET_POWERS(victim,SHADOW_COUNTER) = 0;
    }
    if( IS_CLASS(victim,CLASS_MONK) )
    {
     GET_POWERS(victim, MONK_CUSTOM) /= 2;
     GET_POWERS(victim, MONK_BLOCK) = 0;
    }
    if( IS_CLASS(victim,CLASS_WOLF) )
     do_rage_down(victim,"");
    victim->rage = 0;
    sprintf(buf,"%s absorbs the life essence of %s's limp body!",
     ch->pcdata->switchname,victim->pcdata->switchname);
    do_info(ch,buf);
    sprintf(buf,"%s absorbed %s at %d for GAIN.",ch->pcdata->switchname,victim->pcdata->switchname,
     ch->in_room->vnum);
    log_string(buf);
    do_save(ch,"");     
    do_save(victim,"");
    behead(victim);
   }
   else if( !is_gen_kill(ch,victim) || deathmatch )
   {
    victim->level = 2;
    victim->trust = 2;
    if( !deathmatch )
     ch->fight_timer += 30;
    victim->stuntimer = 0;
    ch->pcdata->target = NULL;
    victim->pcdata->target = NULL;
    if( IS_CLASS(victim,CLASS_SHADOW) && IS_SET(GET_POWERS(victim,SHADOW_BITS),SHADOW_FOLLOW) )
    {
     REM_BIT_POWERS(victim,SHADOW_BITS,SHADOW_FOLLOW);
     GET_POWERS(victim,SHADOW_COUNTER) = 0;
    }
    if( IS_CLASS(victim,CLASS_WOLF) )
     do_rage_down(victim,"");
    victim->rage = 0;
    sprintf(buf,"%s attempted to absorb the life essence of %s's limp body, and failed!",
     ch->pcdata->switchname,victim->pcdata->switchname);
    do_info(ch,buf);
    sprintf(buf,"%s absorbed %s at %d for NOGAIN",ch->pcdata->switchname,victim->pcdata->switchname,
     ch->in_room->vnum);
    log_string(buf);
    do_save(ch,"");     
    do_save(victim,"");
    behead(victim);
    if( deathmatch )
     get_dm_winner(ch);
   }
   break;
  case 2:
   if( IS_SET( ch->newbits, NEW_SETDECAP) )
    sprintf(buf,ch->pcdata->decap_mess,ch->pcdata->switchname,victim->pcdata->switchname);
   else
    sprintf(buf,"%s slices off %s's head and watches it fall to the ground!",
     ch->pcdata->switchname,victim->pcdata->switchname);
   do_info(ch,buf);
   if( has_bounty(victim) )
    bounty_reward(ch,victim);
   if( is_status_dox(ch,victim) && !deathmatch )
    do_paradox(ch,"self");
   if( !deathmatch )
   {
    if( is_status_kill(ch,victim) )
    {
     if( ch->race < victim->race )
      ch->race += 2;
     else if( ch->race >= victim->race )
      ch->race++;
     if( victim->race > 0 )
      victim->race--;
     get_points(ch,victim);
    }
    check_warp(ch);
    ch->pkill++;
    victim->pdeath++;
    victim->level = 2;
    victim->trust = 2;
    victim->exp /= 2;
    ch->exp += victim->exp;
    ch->fight_timer += 15;   
    victim->stuntimer = 0;
    ch->pcdata->target = NULL;
    victim->pcdata->target = NULL;
    if( victim->generation < 2 && victim->race < 60 )
     victim->generation = 2;
    if( victim->generation < 3 && victim->race < 35 )
     victim->generation = 3;
    if( victim->generation < 4 && victim->race < 15 )
     victim->generation = 4;
    register_kill(ch,victim);
   }
   if( deathmatch )
   {
    do_sacrifice(ch, "head");
    victim->hit = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    victim->fight_timer = 0;
    victim->level = 2;
    victim->trust = 2;
    get_dm_winner(ch);
   }
   if( IS_CLASS(victim,CLASS_SHADOW) && IS_SET(GET_POWERS(victim,SHADOW_BITS),SHADOW_FOLLOW) )
   {
    REM_BIT_POWERS(victim,SHADOW_BITS,SHADOW_FOLLOW);
    GET_POWERS(victim,SHADOW_COUNTER) = 0;
   }
   if( IS_CLASS(victim,CLASS_MONK) )
   {
    GET_POWERS(victim, MONK_CUSTOM) /= 2;
    GET_POWERS(victim, MONK_BLOCK) = 0;
   }
   if( IS_CLASS(victim,CLASS_WOLF) )
    do_rage_down(victim,"");
   if( IS_CLASS(victim,CLASS_VAMPIRE) && IS_CLASS(ch,CLASS_VAMPIRE) )
   {
    GET_POWERS(ch,VAMP_KILLS)++;
    check_vkills(ch);
   }
   victim->rage = 0;
   sprintf(buf,"%s decapped %s at %d.",ch->pcdata->switchname,victim->pcdata->switchname,
    ch->in_room->vnum);
   log_string(buf);
   do_save(ch,"");     
   do_save(victim,"");
   behead(victim);
   break;
   /*{  The mud is ready for true killing now...
   else if( !is_status_kill(ch,victim) || deathmatch )
   
    victim->level = 2;
    victim->trust = 2;
    if( !deathmatch )
     ch->fight_timer += 30;
    victim->stuntimer = 0;
    ch->pcdata->target = NULL;
    victim->pcdata->target = NULL;
    if( IS_CLASS(victim,CLASS_SHADOW) && IS_SET(GET_POWERS(victim,SHADOW_BITS),SHADOW_FOLLOW) )
    {
     REM_BIT_POWERS(victim,SHADOW_BITS,SHADOW_FOLLOW);
     GET_POWERS(victim,SHADOW_COUNTER) = 0;
    }
    if( IS_CLASS(victim,CLASS_WOLF) )
     do_rage_down(victim,"");
    if( IS_CLASS(victim,CLASS_VAMPIRE) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
     GET_POWERS(ch,VAMP_KILLS)++;
     check_vkills(ch);
    }
    victim->rage = 0;
    sprintf(buf,"%s attempted to slice off %s's head, and failed!",
     ch->pcdata->switchname,victim->pcdata->switchname);
    do_info(ch,buf);
    sprintf(buf,"%s decapped %s at %d for NOGAIN",ch->pcdata->switchname,victim->pcdata->switchname,
     ch->in_room->vnum);
    log_string(buf);
    do_save(ch,"");     
    do_save(victim,"");
    behead(victim);
    if( deathmatch )
    {
     do_sacrifice(ch, "head");
     victim->hit = victim->max_hit;
     victim->mana = victim->max_mana;
     victim->move = victim->max_move;
     victim->fight_timer = 0;
     get_dm_winner(ch);
    }
   }*/
  case 3:
   if( is_soul_dox(ch,victim) && !deathmatch )
    do_paradox(ch,"self");
   if( is_soul_kill(ch,victim) && !deathmatch )
   {
    if( ch->pcdata->souls < victim->pcdata->souls )
     ch->pcdata->souls += 2;
    else if( ch->pcdata->souls >= victim->pcdata->souls )
     ch->pcdata->souls++;
    if( victim->pcdata->souls > 0 )
     victim->pcdata->souls--;
    check_warp(ch);
    ch->pkill++;
    victim->pdeath++;
    victim->level = 2;
    victim->trust = 2;
    victim->exp /= 2;
    ch->exp += victim->exp;
    ch->fight_timer += 15;   
    get_points(ch,victim);   
    victim->stuntimer = 0;
    ch->pcdata->target = NULL;
    victim->pcdata->target = NULL;
    register_kill(ch,victim);
    if( has_bounty(victim) )
     bounty_reward(ch,victim);
    if( IS_CLASS(victim,CLASS_SHADOW) && IS_SET(GET_POWERS(victim,SHADOW_BITS),SHADOW_FOLLOW) )
    {
     REM_BIT_POWERS(victim,SHADOW_BITS,SHADOW_FOLLOW);
     GET_POWERS(victim,SHADOW_COUNTER) = 0;
    }
    if( IS_CLASS(victim,CLASS_MONK) )
    {
     GET_POWERS(victim, MONK_CUSTOM) /= 2;
     GET_POWERS(victim, MONK_BLOCK) = 0;
    }
    if( IS_CLASS(victim,CLASS_WOLF) )
     do_rage_down(victim,"");
    victim->rage = 0;
    sprintf(buf,"%s tears %s's soul screaming from their body!",
     ch->pcdata->switchname,victim->pcdata->switchname);
    do_info(ch,buf);
    sprintf(buf,"%s souled %s at %d for GAIN",ch->pcdata->switchname,victim->pcdata->switchname,
     ch->in_room->vnum);
    log_string(buf);
    do_save(ch,"");     
    do_save(victim,"");
    behead(victim);
   }
   else if( !is_soul_kill(ch,victim) || deathmatch )
   {
    victim->level = 2;
    victim->trust = 2;
    if( !deathmatch )
     ch->fight_timer += 30;
    victim->stuntimer = 0;
    ch->pcdata->target = NULL;
    victim->pcdata->target = NULL;
    if( IS_CLASS(victim,CLASS_SHADOW) && IS_SET(GET_POWERS(victim,SHADOW_BITS),SHADOW_FOLLOW) )
    {
     REM_BIT_POWERS(victim,SHADOW_BITS,SHADOW_FOLLOW);
     GET_POWERS(victim,SHADOW_COUNTER) = 0;
    }
    if( IS_CLASS(victim,CLASS_WOLF) )
     do_rage_down(victim,"");
    victim->rage = 0;
    sprintf(buf,"%s attempted tear %s's soul from their body, and failed!",
     ch->pcdata->switchname,victim->pcdata->switchname);
    do_info(ch,buf);
    sprintf(buf,"%s souled %s at %d for NOGAIN",ch->pcdata->switchname,victim->pcdata->switchname,
     ch->in_room->vnum);
    log_string(buf);
    do_save(ch,"");     
    do_save(victim,"");
    behead(victim);
    if ( deathmatch )
     get_dm_winner(ch);
   }
   break;
  }
 return;
}

int get_danger( CHAR_DATA *ch )
{
 int hr, dr, ac, status, hp, mp, mv, kills, deaths, danger = 0;

 if( IS_NPC(ch) )
  return 0;

 hr     = class_hitroll(ch);
 dr     = class_damroll(ch);
 ac     = class_ac(ch);
 status = ch->race;
 hp     = ch->max_hit;
 mp     = ch->max_mana;
 mv     = ch->max_move;
 kills  = ch->pkill;
 deaths = ch->pdeath;

 danger = ((ac/10) + (dr/10) + (hr/10) + (hp/1000) + (mp/1000) + (mv/1000) + (status*5) + (kills*2) - (deaths*2));

 return danger;
}

void do_get_danger( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;

 if( IS_NPC(ch) )
  return;
 if( argument == NULL )
 {
  stc("Need to supply a victim.\n\r",ch);
  return;
 }
 if( (victim = get_char_world(ch,argument)) == NULL )
 {
  stc("Can't find the victim.\n\r",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  stc("Not on NPCs.\n\r",ch);
  return;
 }
 ch_output(ch,"Danger rating for %s is %d.",victim->pcdata->switchname,get_danger(victim));
 return;
}

int stance_mod_dam( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
 if( ch == NULL || victim == NULL )
 {
  log_string("NULL ch or vict in stance_mod_dam()");
  return dam;
 }
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_NONE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_NORMAL) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_VIPER) )
  dam *= 0.80;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_CRANE) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_MONGOOSE) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_BULL) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_CRAB) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_MANTIS) )
  dam *= 0.55;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_TIGER) )
  dam *= 0.55;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_DRAGON) )
  dam *= 0.50;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_SWALLOW) )
  dam *= 0.50;

 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_NONE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_NORMAL) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_VIPER) )
  dam *= 0.80;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_CRANE) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_MONGOOSE) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_BULL) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_CRAB) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_MANTIS) )
  dam *= 0.55;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_TIGER) )
  dam *= 0.55;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_DRAGON) )
  dam *= 0.50;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_SWALLOW) )
  dam *= 0.50;

 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_NONE) )
  dam *= 1.20;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_NORMAL) )
  dam *= 1.20;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_VIPER) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_CRANE) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_MONGOOSE) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_BULL) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_CRAB) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_MANTIS) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_TIGER) )
  dam *= 0.65;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_DRAGON) )
  dam *= 0.55;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_SWALLOW) )
  dam *= 0.80;

 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_NONE) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_NORMAL) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_VIPER) )
  dam *= 0.85;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_CRANE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_MONGOOSE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_BULL) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_CRAB) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_MANTIS) )
  dam *= 0.60;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_TIGER) )
  dam *= 0.70;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_DRAGON) )
  dam *= 0.95;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_SWALLOW) )
  dam *= 0.85;

 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_NONE) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_NORMAL) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_VIPER) )
  dam *= 0.85;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_CRANE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_MONGOOSE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_BULL) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_CRAB) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_MANTIS) )
  dam *= 0.60;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_TIGER) )
  dam *= 0.70;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_DRAGON) )
  dam *= 0.95;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_SWALLOW) )
  dam *= 0.85;

 if( compare_stance(ch,victim,STANCE_BULL,STANCE_NONE) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_NORMAL) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_VIPER) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_CRANE) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_MONGOOSE) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_BULL) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_CRAB) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_MANTIS) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_TIGER) )
  dam *= 0.85;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_DRAGON) )
  dam *= 0.85;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_SWALLOW) )
  dam *= 0.75;

 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_NONE) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_NORMAL) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_VIPER) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_CRANE) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_MONGOOSE) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_BULL) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_CRAB) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_MANTIS) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_TIGER) )
  dam *= 0.75;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_DRAGON) )
  dam *= 0.65;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_SWALLOW) )
  dam *= 0.65;

 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_NONE) )
  dam *= 1.45;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_NORMAL) )
  dam *= 1.45;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_VIPER) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_CRANE) )
  dam *= 1.40;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_MONGOOSE) )
  dam *= 1.40;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_BULL) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_CRAB) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_MANTIS) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_TIGER) )
  dam *= 0.85;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_DRAGON) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_SWALLOW) )
  dam *= 1.15;

 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_NONE) )
  dam *= 1.45;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_NORMAL) )
  dam *= 1.45;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_VIPER) )
  dam *= 1.35;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_CRANE) )
  dam *= 1.30;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_MONGOOSE) )
  dam *= 1.30;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_BULL) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_CRAB) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_MANTIS) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_TIGER) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_DRAGON) )
  dam *= 0.90;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_SWALLOW) )
  dam *= 1.05;

 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_NONE) )
  dam *= 1.50;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_NORMAL) )
  dam *= 1.50;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_VIPER) )
  dam *= 1.45;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_CRANE) )
  dam *= 1.05;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_MONGOOSE) )
  dam *= 1.05;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_BULL) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_CRAB) )
  dam *= 1.35;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_MANTIS) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_TIGER) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_DRAGON) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_SWALLOW) )
  dam *= 0.90;

 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_NONE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_NORMAL) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_VIPER) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_CRANE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_MONGOOSE) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_BULL) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_CRAB) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_MANTIS) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_TIGER) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_DRAGON) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_MONKEY) )
  dam *= 1.45;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_SWALLOW) )
  dam *= 1.00;

 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_NONE) )
  dam *= 1.50;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_NORMAL) )
  dam *= 1.50;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_VIPER) )
  dam *= 1.20;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_CRANE) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_MONGOOSE) )
  dam *= 1.15;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_BULL) )
  dam *= 1.25;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_CRAB) )
  dam *= 1.35;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_MANTIS) )
  dam *= 0.85;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_TIGER) )
  dam *= 0.95;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_DRAGON) )
  dam *= 1.10;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_MONKEY) )
  dam *= 1.00;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_SWALLOW) )
  dam *= 1.00;

 return dam;
}

int stance_mod_apd( CHAR_DATA *ch, CHAR_DATA *victim )
{
 int mod = 0;

 if( ch == NULL || victim == NULL )
 {
  log_string("NULL ch or vict in stance_mod_apd()");
  return mod;
 }
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_NONE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_NORMAL) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_VIPER) )
  mod += -4;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_CRANE) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_MONGOOSE) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_BULL) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_CRAB) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_MANTIS) )
  mod += -9;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_TIGER) )
  mod += -9;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_DRAGON) )
  mod += -10;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_NONE,STANCE_SWALLOW) )
  mod += -10;

 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_NONE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_NORMAL) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_VIPER) )
  mod += -4;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_CRANE) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_MONGOOSE) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_BULL) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_CRAB) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_MANTIS) )
  mod += -9;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_TIGER) )
  mod += -9;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_DRAGON) )
  mod += -10;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_NORMAL,STANCE_SWALLOW) )
  mod += -10;

 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_NONE) )
  mod += 4;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_NORMAL) )
  mod += 4;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_VIPER) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_CRANE) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_MONGOOSE) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_BULL) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_CRAB) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_MANTIS) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_TIGER) )
  mod += -7;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_DRAGON) )
  mod += -9;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_VIPER,STANCE_SWALLOW) )
  mod += -4;

 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_NONE) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_NORMAL) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_VIPER) )
  mod += -3;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_CRANE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_MONGOOSE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_BULL) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_CRAB) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_MANTIS) )
  mod += -8;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_TIGER) )
  mod += -6;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_DRAGON) )
  mod += -1;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_CRANE,STANCE_SWALLOW) )
  mod += -3;

 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_NONE) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_NORMAL) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_VIPER) )
  mod += -3;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_CRANE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_MONGOOSE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_BULL) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_CRAB) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_MANTIS) )
  mod += -8;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_TIGER) )
  mod += -6;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_DRAGON) )
  mod += -1;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONGOOSE,STANCE_SWALLOW) )
  mod += -3;

 if( compare_stance(ch,victim,STANCE_BULL,STANCE_NONE) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_NORMAL) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_VIPER) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_CRANE) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_MONGOOSE) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_BULL) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_CRAB) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_MANTIS) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_TIGER) )
  mod += -3;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_DRAGON) )
  mod += -3;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_BULL,STANCE_SWALLOW) )
  mod += -5;

 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_NONE) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_NORMAL) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_VIPER) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_CRANE) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_MONGOOSE) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_BULL) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_CRAB) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_MANTIS) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_TIGER) )
  mod += -5;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_DRAGON) )
  mod += -7;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_CRAB,STANCE_SWALLOW) )
  mod += -7;

 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_NONE) )
  mod += 9;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_NORMAL) )
  mod += 9;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_VIPER) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_CRANE) )
  mod += 8;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_MONGOOSE) )
  mod += 8;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_BULL) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_CRAB) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_MANTIS) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_TIGER) )
  mod += -3;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_DRAGON) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MANTIS,STANCE_SWALLOW) )
  mod += 3;

 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_NONE) )
  mod += 9;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_NORMAL) )
  mod += 9;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_VIPER) )
  mod += 7;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_CRANE) )
  mod += 6;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_MONGOOSE) )
  mod += 6;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_BULL) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_CRAB) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_MANTIS) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_TIGER) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_DRAGON) )
  mod += -2;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_TIGER,STANCE_SWALLOW) )
  mod += 1;

 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_NONE) )
  mod += 10;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_NORMAL) )
  mod += 10;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_VIPER) )
  mod += 9;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_CRANE) )
  mod += 1;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_MONGOOSE) )
  mod += 1;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_BULL) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_CRAB) )
  mod += 7;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_MANTIS) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_TIGER) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_DRAGON) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_DRAGON,STANCE_SWALLOW) )
  mod += -2;

 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_NONE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_NORMAL) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_VIPER) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_CRANE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_MONGOOSE) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_BULL) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_CRAB) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_MANTIS) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_TIGER) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_DRAGON) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_MONKEY) )
  mod += 9;
 if( compare_stance(ch,victim,STANCE_MONKEY,STANCE_SWALLOW) )
  mod += 0;

 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_NONE) )
  mod += 10;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_NORMAL) )
  mod += 10;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_VIPER) )
  mod += 4;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_CRANE) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_MONGOOSE) )
  mod += 3;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_BULL) )
  mod += 5;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_CRAB) )
  mod += 7;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_MANTIS) )
  mod += -3;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_TIGER) )
  mod += -1;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_DRAGON) )
  mod += 2;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_MONKEY) )
  mod += 0;
 if( compare_stance(ch,victim,STANCE_SWALLOW,STANCE_SWALLOW) )
  mod += 0;

 return mod;
}

bool compare_stance( CHAR_DATA *ch, CHAR_DATA *victim, int ch_stance, int vict_stance )
{
 if( ch == NULL || victim == NULL )
 {
  log_string("NULL ch or vict in compare_stance()");
  return FALSE;
 }
 if( IS_STANCE(ch,ch_stance) && IS_STANCE(victim,vict_stance) )
  return TRUE;
 else
  return FALSE;
}
