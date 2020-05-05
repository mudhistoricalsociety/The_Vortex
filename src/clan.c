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

void	adv_spell_damage	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_affect	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	adv_spell_action	args( ( CHAR_DATA *ch, OBJ_DATA *book, 
				OBJ_DATA *page, char *argument) );
void	birth_write		args( ( CHAR_DATA *ch, char *argument ) );
bool	birth_ok		args( ( CHAR_DATA *ch, char *argument ) );

/* Need to get rid of those flames somehow - KaVir */

void do_clan(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);

  if( IS_NPC(ch))
   return;
  if( arg1[0] == '\0')
  {
   stc("Clan who?\n\r", ch);
   return;
  }
  if( arg2[0] == '\0')
  {
   stc("What do you want to do with them?\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg1)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->clanbits, CLAN_LDR) && !IS_SET(ch->clanbits, CLAN_CO))
  {
   stc("You are not a clan leader.\n\r", ch);
   return;
  }
  if( victim->class <= 0)
  {
   stc("But they dont have a class yet.\n\r", ch);
   return;
  }
  if( victim->level < 3)
  {
   stc("They are not an avatar.\n\r", ch);
   return;
  }
  if( ch->level < 3)
  {
   stc("You must be an avatar.\n\r", ch);
   return;
  }
  if( !str_cmp(arg2, "accept"))
  {
   if( str_cmp( victim->clan, ""))
   {
    stc("They already have a clan.\n\r", ch);
    return;
   }
   if( IS_SET(victim->clanbits, CLAN_LDR))
   {
    stc("Not on a clan leader.\n\r", ch);
    return;
   }
   if( IS_SET(victim->clanbits, CLAN_CO))
   {
    stc("Not on a coleader.\n\r", ch);
    return;
   }
   free_string(victim->clan);
   victim->clan = str_dup(ch->clan);
   stc("You have inducted a new member.\n\r", ch);
   stc("You have been inducted into a clan.\n\r", victim);
   return;
  }
  else if( !str_cmp(arg2, "banish"))
  {
    if( str_cmp(ch->clan, victim->clan))
    {
     stc("You are not in the same clan.\n\r", ch);
     return;
    }
   if( IS_SET(ch->clanbits, CLAN_LDR)
    && IS_SET(victim->clanbits, CLAN_CO))
     REMOVE_BIT(victim->clanbits, CLAN_CO);
   if( IS_SET(victim->clanbits, CLAN_LDR))
   {
    stc("Not on your clan leader.\n\r", ch);
    return;
   }
   free_string(victim->clan);
   victim->clan = str_dup("");
   stc("You have kicked them out of your clan!\n\r", ch);
   stc("You have been removed from your clan!\n\r", victim);
   return;
  }
  else
  {
   stc("Not a valid option.\n\r", ch);
   return;
  }
  return;
}

void do_flex( CHAR_DATA *ch, char *argument )
{

    CHAR_DATA *holder;
    int chance;

    chance = number_range(1, 100);

    act("You flex your bulging muscles.",ch,NULL,NULL,TO_CHAR);
    act("$n flexes $s bulging muscles.",ch,NULL,NULL,TO_ROOM);
    if (IS_NPC(ch)) return;
    if( ch->pcdata->held_by != NULL
    &&  ch->pcdata->held_by != '\0' 
    &&  chance <= 65 )
    {
	holder = get_char_world(ch, ch->pcdata->held_by);
        if( holder != NULL && holder != ch)
	do_drelease(holder, holder->pcdata->held);
	if( ch->pcdata->held_by != NULL
 	 && ch->pcdata->held_by != '\0')
		ch->pcdata->held_by = strdup( "" );
    }
 
    if ( IS_EXTRA(ch, TIED_UP) )
    {
        act("The ropes restraining you snap.",ch,NULL,NULL,TO_CHAR);
        act("The ropes restraining $n snap.",ch,NULL,NULL,TO_ROOM);
        REMOVE_BIT(ch->extra, TIED_UP);
    }
    if (is_affected(ch, gsn_web))
    { 
        act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
        send_to_char("The webbing entrapping you breaks away.\n\r",ch);
        affect_strip(ch, gsn_web);
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
        act("The webbing entrapping $n breaks away.",ch,NULL,NULL,TO_ROOM);
        send_to_char("The webbing entrapping you breaks away.\n\r",ch);
        REMOVE_BIT(ch->affected_by, AFF_WEBBED);
    }
    WAIT_STATE(ch,12);
    return;
}

void do_loner( CHAR_DATA *ch, char*argument)
{
  char arg[MAX_INPUT_LENGTH];
  argument = one_argument(argument, arg);

  if( IS_NPC(ch))
  return;
  if( !str_cmp(ch->clan, ""))
  {
   send_to_char("You are already clanless.\n\r", ch);
   return;
  }
  if( arg[0] == '\0')
  {
   send_to_char("You must supply your password to validate the declanning.\n\r", ch);
   return;
   }
  if( str_cmp(arg, ch->pcdata->pwd))
  {
   send_to_char("THAT IS NOT YOUR PASSWORD.\n\r", ch);
   return;
  }
  free_string( ch->clan);
  ch->clan = str_dup("");
  send_to_char("You are now a loner.\n\r", ch);
  do_save(ch, "");
  return;
}


void do_rolldirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *inroom;
    one_argument( argument, arg );
    inroom=ch->in_room;
    
    if (!IS_AFFECTED(ch, AFF_FLAMING))
    {
	send_to_char( "You aren't on fire\n\r", ch );
	return;
    }

    if ( number_range(1,100) < 70 )
    {
	send_to_char("You roll in the dirt trying to put out the flames.\n\r", ch);
	act( "$n rolls in the dirt trying to stop the burning flames",  ch, NULL, NULL, TO_ROOM );
	return;
    }

	send_to_char("You manage to roll off the flames around you.\n\r", ch);
	act( "$n manages to roll of the flames around them",  ch, NULL, NULL, TO_ROOM );
    REMOVE_BIT(ch->affected_by, AFF_FLAMING);
    return;
}
/* End of Roll Dirt Command */

void do_smother( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *inroom;
    one_argument( argument, arg );
    if (IS_NPC( ch )) return;
    inroom=ch->in_room;
    if ( arg[0] == '\0' && !IS_SET(inroom->room_flags,ROOM_FLAMING))
    {
	send_to_char( "Smother whom?\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot smother yourself.\n\r", ch );
	return;
    }

    if (!IS_AFFECTED(victim, AFF_FLAMING))
    {
	send_to_char( "But they are not on fire!\n\r", ch );
	return;
    }

    if ( number_range(1,100) < 80 )
    {
	act( "You try to smother the flames around $N but fail!",  ch, NULL, victim, TO_CHAR    );
	act( "$n tries to smother the flames around you but fails!", ch, NULL, victim, TO_VICT    );
	act( "$n tries to smother the flames around $N but fails!",  ch, NULL, victim, TO_NOTVICT );
	if ( number_percent() > 98 && !IS_AFFECTED(ch,AFF_FLAMING) )
	{
	    act( "A spark of flame from $N's body sets you on fire!",  ch, NULL, victim, TO_CHAR    );
	    act( "A spark of flame from your body sets $n on fire!", ch, NULL, victim, TO_VICT    );
	    act( "A spark of flame from $N's body sets $n on fire!",  ch, NULL, victim, TO_NOTVICT );
	    SET_BIT(ch->affected_by, AFF_FLAMING);
	}
	return;
    }

    act( "You manage to smother the flames around $M!",  ch, NULL, victim, TO_CHAR    );
    act( "$n manages to smother the flames around you!", ch, NULL, victim, TO_VICT    );
    act( "$n manages to smother the flames around $N!",  ch, NULL, victim, TO_NOTVICT );
    REMOVE_BIT(victim->affected_by, AFF_FLAMING);
    return;
}

/* Loads of Vampire powers follow.  KaVir */
void do_generation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int gen;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: generation <char> <generation>.\n\r", ch );
    send_to_char("Generation 1 is a Master <Class> and 2 is clan leader.\n\r", ch);
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    
    gen = is_number( arg2 ) ? atoi( arg2 ) : -1;

    send_to_char("Generation Set.\n\r",ch);
    victim->generation = gen;
    return;
}

void reset_weapon( CHAR_DATA *ch, int dtype )
{
	if ( ch->wpn[dtype] > 200 )
		ch->wpn[dtype] = 200;
	return;
}

void reset_spell( CHAR_DATA *ch, int dtype )
{
	if ( ch->spl[dtype] > 200 )
		ch->spl[dtype] = 200;
	return;
}

/*Added in from sage vic code for multi class clananme*/
void do_clanname( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' )
    {
    send_to_char( "Who's clan do you wish to change or name?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) ) return;

    smash_tilde( argument );
    if ( strlen(argument) < 0 || strlen(argument) > 20 )
    {
        send_to_char( "Clan name should be between 0 and 20 letters long.\n\r", ch );
        send_to_char( "Leave a blank to remove clan.\n\r", ch);
        return;
    }
    free_string( victim->clan );
    victim->clan = str_dup( argument );
        send_to_char("Clan set.\n\r", ch);
        send_to_char("Clan set.\n\r", victim);
    return;
}

void do_truesight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
    if( !IS_SET(ch->pcdata->askills[AS_BITS],ABIT_TRUESIGHT) )
        return;
    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses return to normal.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your senses increase to incredible proportions.\n\r", ch );
    }

    return;
}
/*
void do_scry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *victimroom;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Scry on whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    chroom = ch->in_room;
    victimroom = victim->in_room;

    char_from_room(ch);
    char_to_room(ch,victimroom);
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) && (!IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) && (IS_AFFECTED(victim, AFF_SHADOWPLANE)))
    {
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch,"auto");
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
    }
    else
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch,chroom);
    if (!IS_NPC(victim))
    {
      if (ch->fight_timer > 30)
        ch->fight_timer = 30;
      else 
        ch->fight_timer += 10;
    }
    return;
}
*/
void werewolf_regen( CHAR_DATA *ch )
{
    int min = 1;
    int max;

    if (IS_NPC(ch)) return;

    if (ch->hit < 1 ) 
    {
		ch->hit = ch->hit + number_range(1,3);
		update_pos(ch);
    }
	else if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
		return;
    else
    {
		if (((ch->max_hit) > (ch->max_mana)) && ((ch->max_hit) > (ch->max_move)))
			max = ch->max_hit / 175;
		else if (((ch->max_move) > (ch->max_hit)) && ((ch->max_move) > (ch->max_mana)))
			max = ch->max_move / 175;
		else
			max = ch->max_mana / 175;
		if (max < 25)
			max = 50;
    	ch->hit = UMIN(ch->hit + number_range(min,max), ch->max_hit);
    	ch->mana = UMIN(ch->mana + number_range(min,max), ch->max_mana);
    	ch->move = UMIN(ch->move + number_range(min,max), ch->max_move);
		if ( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move)
			send_to_char("Your body has completely regenerated itself.\n\r",ch);
		if ((ch->position == POS_STUNNED) && (ch->hit > ch->max_hit * 0.05) && (number_range(1,3) == 1))
		{
			act( "$n clambers back to $s feet.", ch, NULL, NULL, TO_ROOM);
			act( "You clamber back to your feet.", ch, NULL, NULL, TO_CHAR);
		    ch->position = POS_STANDING;
		}
	}
    return;
}

void mortal_regen( CHAR_DATA *ch )
{
    int min = 5;
    int max = 10;

    if (IS_NPC(ch)) return;

    if (ch->hit < 1 ) 
    {
		send_to_char( "You have been KILLED!!\n\r\n\r", ch );
		raw_kill(ch);
		return;
    }
    else if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
	{
		if (((ch->max_hit) > (ch->max_mana)) && ((ch->max_hit) > (ch->max_move)))
			max = ch->max_hit / 190;
		else if (((ch->max_move) > (ch->max_hit)) && ((ch->max_move) > (ch->max_mana)))
			max = ch->max_move / 190;
		else if (((ch->max_mana) > (ch->max_hit)) && ((ch->max_mana) > (ch->max_move)))
			max = ch->max_mana / 190;
		if (max < 25)
			max = 50;
    	ch->hit = UMIN(ch->hit + number_range(min,max), ch->max_hit);
    	ch->mana = UMIN(ch->mana + number_range(min,max), ch->max_mana);
    	ch->move = UMIN(ch->move + number_range(min,max), ch->max_move);

	}
    return;
}

void reg_mend( CHAR_DATA *ch )
{
    int ribs = 0;
    int teeth = 0;

    if (IS_BODY(ch,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(ch,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(ch,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(ch,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(ch,BROKEN_RIBS_16)) ribs += 16;
    if (IS_HEAD(ch,LOST_TOOTH_1  )) teeth += 1;
    if (IS_HEAD(ch,LOST_TOOTH_2  )) teeth += 2;
    if (IS_HEAD(ch,LOST_TOOTH_4  )) teeth += 4;
    if (IS_HEAD(ch,LOST_TOOTH_8  )) teeth += 8;
    if (IS_HEAD(ch,LOST_TOOTH_16 )) teeth += 16;

    if (IS_BLEEDING(ch,BLEEDING_HEAD))
    {
        act("$n's head stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("Your head stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_HEAD);
    }
    else if (IS_BLEEDING(ch,BLEEDING_THROAT))
    {
        act("$n's throat stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("Your throat stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_THROAT);
    }
    else if (IS_BLEEDING(ch,BLEEDING_ARM_L))
    {
        act("The stump of $n's left arm stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your left arm stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_ARM_L);
    }
    else if (IS_BLEEDING(ch,BLEEDING_ARM_R))
    {
        act("The stump of $n's right arm stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your right arm stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_ARM_R);
    }
    else if (IS_BLEEDING(ch,BLEEDING_LEG_L))
    {
        act("The stump of $n's left leg stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your left leg stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_LEG_L);
    }
    else if (IS_BLEEDING(ch,BLEEDING_LEG_R))
    {
        act("The stump of $n's right leg stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your right leg stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_LEG_R);
    }
    else if (IS_BLEEDING(ch,BLEEDING_HAND_L))
    {
        act("The stump of $n's left wrist stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your left wrist stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_HAND_L);
    }
    else if (IS_BLEEDING(ch,BLEEDING_HAND_R))
    {
        act("The stump of $n's right wrist stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your right wrist stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_HAND_R);
    }
    else if (IS_BLEEDING(ch,BLEEDING_FOOT_L))
    {
        act("The stump of $n's left ankle stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your left ankle stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_FOOT_L);
    }
    else if (IS_BLEEDING(ch,BLEEDING_FOOT_R))
    {
        act("The stump of $n's right ankle stops bleeding.",ch,NULL,NULL,TO_ROOM);
        act("The stump of your right ankle stops bleeding.",ch,NULL,NULL,TO_CHAR);
        REMOVE_BIT(ch->loc_hp[6],BLEEDING_FOOT_R);
    }
    else if (ribs > 0)
    {
    	if (IS_BODY(ch,BROKEN_RIBS_1 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_1);
    	if (IS_BODY(ch,BROKEN_RIBS_2 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_2);
	if (IS_BODY(ch,BROKEN_RIBS_4 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_4);
	if (IS_BODY(ch,BROKEN_RIBS_8 ))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_8);
	if (IS_BODY(ch,BROKEN_RIBS_16))
	    REMOVE_BIT(ch->loc_hp[1],BROKEN_RIBS_16);
	ribs -= 1;
	if (ribs >= 16) {ribs -= 16;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_16);}
	if (ribs >= 8 ) {ribs -= 8;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_8);}
	if (ribs >= 4 ) {ribs -= 4;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_4);}
	if (ribs >= 2 ) {ribs -= 2;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_2);}
	if (ribs >= 1 ) {ribs -= 1;
	    SET_BIT(ch->loc_hp[1],BROKEN_RIBS_1);}
	act("One of $n's ribs snap back into place.",ch,NULL,NULL,TO_ROOM);
	act("One of your ribs snap back into place.",ch,NULL,NULL,TO_CHAR);
    }
    else if (IS_HEAD(ch,LOST_EYE_L))
    {
	act("An eyeball appears in $n's left eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your left eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_L);
    }
    else if (IS_HEAD(ch,LOST_EYE_R))
    {
	act("An eyeball appears in $n's right eye socket.",ch,NULL,NULL,TO_ROOM);
	act("An eyeball appears in your right eye socket.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EYE_R);
    }
    else if (IS_HEAD(ch,LOST_EAR_L))
    {
	act("An ear grows on the left side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the left side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_L);
    }
    else if (IS_HEAD(ch,LOST_EAR_R))
    {
	act("An ear grows on the right side of $n's head.",ch,NULL,NULL,TO_ROOM);
	act("An ear grows on the right side of your head.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_EAR_R);
    }
    else if (IS_HEAD(ch,LOST_NOSE))
    {
	act("A nose grows on the front of $n's face.",ch,NULL,NULL,TO_ROOM);
	act("A nose grows on the front of your face.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_NOSE);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (teeth > 0)
    {
    	if (IS_HEAD(ch,LOST_TOOTH_1 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);
    	if (IS_HEAD(ch,LOST_TOOTH_2 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);
    	if (IS_HEAD(ch,LOST_TOOTH_4 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);
    	if (IS_HEAD(ch,LOST_TOOTH_8 ))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);
    	if (IS_HEAD(ch,LOST_TOOTH_16))
	    REMOVE_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);
	teeth -= 1;
	if (teeth >= 16) {teeth -= 16;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_16);}
	if (teeth >= 8 ) {teeth -= 8;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_8);}
	if (teeth >= 4 ) {teeth -= 4;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_4);}
	if (teeth >= 2 ) {teeth -= 2;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_2);}
	if (teeth >= 1 ) {teeth -= 1;
	    SET_BIT(ch->loc_hp[LOC_HEAD],LOST_TOOTH_1);}
	act("A missing tooth grows in your mouth.",ch,NULL,NULL,TO_CHAR);
	act("A missing tooth grows in $n's mouth.",ch,NULL,NULL,TO_ROOM);
    }
    else if (IS_HEAD(ch,BROKEN_NOSE))
    {
	act("$n's nose snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your nose snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (IS_HEAD(ch,BROKEN_JAW))
    {
	act("$n's jaw snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your jaw snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    else if (IS_HEAD(ch,BROKEN_SKULL))
    {
	act("$n's skull knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your skull knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_HEAD],BROKEN_SKULL);
    }
    else if (IS_BODY(ch,BROKEN_SPINE))
    {
	act("$n's spine knits itself back together.",ch,NULL,NULL,TO_ROOM);
	act("Your spine knits itself back together.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_SPINE);
    }
    else if (IS_BODY(ch,BROKEN_NECK))
    {
	act("$n's neck snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your neck snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],BROKEN_NECK);
    }
    else if (IS_ARM_L(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's left shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your left shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
    }
    else if (IS_ARM_R(ch,LOST_ARM))
    {
	act("An arm grows from the stump of $n's right shoulder.",ch,NULL,NULL,TO_ROOM);
	act("An arm grows from the stump of your right shoulder.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_ARM);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
    }
    else if (IS_LEG_L(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's left hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your left hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_LEG))
    {
	act("A leg grows from the stump of $n's right hip.",ch,NULL,NULL,TO_ROOM);
	act("A leg grows from the stump of your right hip.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_LEG);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
	SET_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,BROKEN_ARM))
    {
	act("$n's left arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_ARM);
    }
    else if (IS_ARM_R(ch,BROKEN_ARM))
    {
	act("$n's right arm snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right arm snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_ARM);
    }
    else if (IS_LEG_L(ch,BROKEN_LEG))
    {
	act("$n's left leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],BROKEN_LEG);
    }
    else if (IS_LEG_R(ch,BROKEN_LEG))
    {
	act("$n's right leg snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right leg snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],BROKEN_LEG);
    }
    else if (IS_ARM_L(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's left wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your left wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_HAND))
    {
	act("A hand grows from the stump of $n's right wrist.",ch,NULL,NULL,TO_ROOM);
	act("A hand grows from the stump of your right wrist.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_HAND);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	SET_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
    }
    else if (IS_LEG_L(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's left ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your left ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_L],LOST_FOOT);
    }
    else if (IS_LEG_R(ch,LOST_FOOT))
    {
	act("A foot grows from the stump of $n's right ankle.",ch,NULL,NULL,TO_ROOM);
	act("A foot grows from the stump of your right ankle.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_LEG_R],LOST_FOOT);
    }
    else if (IS_ARM_L(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,BROKEN_THUMB))
    {
	act("$n's left thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_THUMB);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_I))
    {
	act("$n's left index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_M))
    {
	act("$n's left middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_R))
    {
	act("$n's left ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);
    }
    else if (IS_ARM_L(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's left hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your left hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_L(ch,BROKEN_FINGER_L))
    {
	act("$n's left little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your left little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);
    }
    else if (IS_ARM_R(ch,LOST_THUMB))
    {
	act("A thumb slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A thumb slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_THUMB);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,BROKEN_THUMB))
    {
	act("$n's right thumb snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right thumb snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_THUMB);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_I))
    {
	act("An index finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("An index finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_I);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_I))
    {
	act("$n's right index finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right index finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_M))
    {
	act("A middle finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A middle finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_M);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_M))
    {
	act("$n's right middle finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right middle finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);
    }
    else if (IS_ARM_R(ch,LOST_FINGER_R))
    {
	act("A ring finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A ring finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_R);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if (IS_ARM_R(ch,BROKEN_FINGER_R))
    {
	act("$n's right ring finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right ring finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);
    }
    else if ( IS_ARM_R(ch,LOST_FINGER_L))
    {
	act("A little finger slides out of $n's right hand.",ch,NULL,NULL,TO_ROOM);
	act("A little finger slides out of your right hand.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],LOST_FINGER_L);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if ( IS_ARM_R(ch,BROKEN_FINGER_L))
    {
	act("$n's right little finger snaps back into place.",ch,NULL,NULL,TO_ROOM);
	act("Your right little finger snaps back into place.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);
    }
    else if (IS_BODY(ch,CUT_THROAT))
    {
    	if (IS_SET(ch->loc_hp[6], BLEEDING_THROAT)) return;
	act("The wound in $n's throat closes up.",ch,NULL,NULL,TO_ROOM);
	act("The wound in your throat closes up.",ch,NULL,NULL,TO_CHAR);
	REMOVE_BIT(ch->loc_hp[LOC_BODY],CUT_THROAT);
    }
    return;
}
/*
void do_familiar( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *familiar;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to make your familiar?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Become your own familiar?\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {
	send_to_char( "Not on players.\n\r", ch );
	return;
    }

    if ( ( familiar = ch->pcdata->familiar ) != NULL ) 
	familiar->wizard = NULL;
    ch->pcdata->familiar = victim;
    victim->wizard = ch;
    send_to_char("Ok.\n\r",ch);

    return;
}
*/

bool char_exists( bool backup, char *argument )
{
    FILE *fp;
    char buf [MAX_STRING_LENGTH];
    bool found = FALSE;

    fclose( fpReserve );
    if (backup)
    	sprintf( buf, "%sbackup/%s", PLAYER_DIR, capitalize( argument ) );
    else
    	sprintf( buf, "%s%s", PLAYER_DIR, capitalize( argument ) );
    if ( ( fp = fopen( buf, "r" ) ) != NULL )
    {
	found = TRUE;
	fclose( fp );
    }
    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}
