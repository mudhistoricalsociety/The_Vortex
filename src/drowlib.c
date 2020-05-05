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


void drowgrant( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 int inpart = 0;
 int cost = 0;

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_DROW) )
 {
  do_huh(ch,"");
  return;
 }

 if( arg1[0] == '\0' || arg2[0] == '\0' )
 {
  stc("Syntax: Grant <person> <power>\n\r",ch);
  stc("Warrior      (0)         Mage         (0)         Cleric     (0)\n\r",ch);  
  stc("Drowfire     (2,500)     Darkness     (7,500)     Drowpoison (2,500)\n\r",ch);
  stc("Speed        (7,500)     Drowshield   (5,000)     Levitation (1,000)\n\r",ch);
  stc("Scry         (10,000)    Garotte      (5,000)     Spiderarms (7,500)\n\r",ch);
  stc("Drowhate     (10,000)    Spiderform   (15,000)    Web        (5,000)\n\r",ch);
  stc("Dgarotte     (2,500)     Earthshatter (7,500)     Toughskin  (7,500)\n\r",ch);
  return;
 }

 if( (victim = get_char_world(ch,arg1)) == NULL )
 {
  stc("Nobody by that name.\n\r",ch);
  return;
 }

 if( IS_NPC(victim) )
 {
  stc("Not on NPC's.\n\r",ch);
  return;
 }

 if( !IS_CLASS(victim,CLASS_DROW) )
 { 
  stc("Only on Drow.\n\r",ch);
  return;
 }

 if( !str_cmp(arg2,"mage") || !str_cmp(arg2,"warrior") || !str_cmp(arg2,"cleric") )
 {
  if( victim->generation > 3 )
  {
   stc("They are not worthy enough of such an honor!\n\r",ch);
   return;
  }

  if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_WAR)
   || IS_SET(GET_POWERS(victim,DROW_BITS),DROW_MAG)
   || IS_SET(GET_POWERS(victim,DROW_BITS),DROW_CLE) )
  {
   stc("They already have a profession.\n\r",ch);
   return;
  }
  if( !str_cmp(arg2,"mage") )
   SET_BIT_POWERS(victim,DROW_BITS,DROW_MAG);
  else if( !str_cmp(arg2,"cleric") )
   SET_BIT_POWERS(victim,DROW_BITS,DROW_CLE);
  else if( !str_cmp(arg2,"warrior") )
   SET_BIT_POWERS(victim,DROW_BITS,DROW_WAR);
  stc("You give them a profession.\n\r",ch);
  stc("You have been given a profession.\n\r",victim);
  save_char_obj(victim);
  return;
 }

 if( !str_cmp(arg2,"drowfire") )          { inpart = DPOWER_DROWFIRE;     cost = 2500;  }
 else if( !str_cmp(arg2,"darkness") )     { inpart = DPOWER_DARKNESS;     cost = 7500;  }
 else if( !str_cmp(arg2,"spiderarms") )   { inpart = DPOWER_ARMS;         cost = 7500;  }
 else if( !str_cmp(arg2,"web") )          { inpart = DPOWER_WEB;          cost = 5000;  }
 else if( !str_cmp(arg2,"spiderform") )   { inpart = DPOWER_SPIDERFORM;   cost = 15000; }
 else if( !str_cmp(arg2,"drowhate") )     { inpart = DPOWER_DROWHATE;     cost = 10000; }
 else if( !str_cmp(arg2,"drowshield") )   { inpart = DPOWER_DROWSHIELD;   cost = 5000;  }
 else if( !str_cmp(arg2,"levitation") )   { inpart = DPOWER_LEVITATION;   cost = 1000;  }
 else if( !str_cmp(arg2,"garotte") )      { inpart = DPOWER_GAROTTE;      cost = 5000;  }
 else if( !str_cmp(arg2,"dgarotte") )     { inpart = DPOWER_DGAROTTE;     cost = 2500;  }
 else if( !str_cmp(arg2,"drowpoison") )   { inpart = DPOWER_DROWPOISON;   cost = 2500;  }
 else if( !str_cmp(arg2,"earthshatter") ) { inpart = DPOWER_EARTHSHATTER; cost = 7500;  }
 else if( !str_cmp(arg2,"speed") )        { inpart = DPOWER_SPEED;        cost = 7500;  }
 else if( !str_cmp(arg2,"toughskin") )    { inpart = DPOWER_TOUGHSKIN;    cost = 7500;  } 
 else if( !str_cmp(arg2,"scry") )         { inpart = DPOWER_SCRY;         cost = 10000; }
 else
 {
  stc("Available powers are:\n\r",ch);
  stc("Warrior      (0)         Mage         (0)         Cleric     (0)\n\r",ch);  
  stc("Drowfire     (2,500)     Darkness     (7,500)     Drowpoison (2,500)\n\r",ch);
  stc("Speed        (7,500)     Drowshield   (5,000)     Levitation (1,000)\n\r",ch);
  stc("Scry         (10,000)    Garotte      (5,000)     Spiderarms (7,500)\n\r",ch);
  stc("Drowhate     (10,000)    Spiderform   (15,000)    Web        (5,000)\n\r",ch);
  stc("Dgarotte     (2,500)     Earthshatter (7,500)     Toughskin  (7,500)\n\r",ch);
  return;
 }

 if( IS_SET(GET_POWERS(victim,DROW_BITS),inpart) )
 {
  if( victim != ch )
   stc("They already have that power.\n\r",ch);
  else
   stc("You already have that power.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,DROW_POINTS) < cost ) 
 {
  stc("You have insufficient power to grant that gift.\n\r",ch);
  return;
 }
 SET_BIT_POWERS(victim,DROW_BITS,inpart);
 GET_POWERS(ch,DROW_POINTS) -= cost;
 stc("You have been granted a gift from Lloth!\n\r",victim);
 if( victim != ch )
  stc("Ok.\n\r",ch);
 if( victim != ch )
  save_char_obj(ch);
 save_char_obj(victim);
 return; 
}

void chaosblast( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn;
 int level;
 int spelltype;
 int cost = 100;

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || (!IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) && ch->generation > 2) )
 {
  do_huh(ch,"");
  return;
 }

 if( arg[0] == '\0' )
 {
  stc("Chaosblast whom?\n\r",ch);
  return;
 }

 if( (victim = get_char_room(ch,arg) ) == NULL && (victim = ch->fighting) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }

 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }

 if( ch->mana < cost )
 {
  stc("You don't have enough mana.\n\r",ch);
  return;
 }

 if( (sn = skill_lookup("chaos blast")) < 0 )
  return;
 spelltype = skill_table[sn].target;
 level = (ch->spl[spelltype] * 2);
 act("You concentrate your power on $N.",ch,NULL,victim,TO_CHAR);
 act("$n concentrates $s power on you.",ch,NULL,victim,TO_VICT);
 act("$n concentraces $s power on $N.",ch,NULL,victim,TO_NOTVICT);
 (*skill_table[sn].spell_fun)(sn,level,ch,victim);
 WAIT_STATE(ch,12);
 ch->mana -= cost;
 return;
}

void drowfire( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn;
 int level;
 int spelltype;
 int cost = 100;

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWFIRE) )
 {
  do_huh(ch,"");
  return;
 }

 if( arg[0] == '\0' )
 {
  stc("Drowfire whom?\n\r",ch);
  return;
 }

 if( (victim = get_char_room(ch,arg) ) == NULL && (victim = ch->fighting) == NULL )
 {
  stc("They are't here.\n\r",ch);
  return;
 }

 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }

 if( ch->mana < cost )
 {
  stc("You don't have enough mana.\n\r",ch);
  return;
 }

 if( (sn = skill_lookup("drowfire")) < 0 )
  return;
 spelltype = skill_table[sn].target;
 level = ch->spl[spelltype] * 0.5;
 (*skill_table[sn].spell_fun)(sn,level,ch,victim);
 WAIT_STATE(ch,12);
 ch->mana -= cost;
 return;
}

void heal( CHAR_DATA *ch, char *argument ) 
{
 int cost = 100;

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || (!IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) && ch->generation > 2)) 
 {
  do_huh(ch,"");
  return;
 }

 if( ch->mana < cost )
 {
  stc("You don't have enough mana.\n\r",ch);
  return;
 }

 ch->mana -= cost;
 ch->hit += (ch->spl[BLUE_MAGIC] * 2);
 if( ch->hit > ch->max_hit )
  ch->hit = ch->max_hit;
 stc("Lloth heals you.\n\r",ch);
 act("$n is healed by Lloth.",ch,NULL,NULL,TO_ROOM);
 WAIT_STATE(ch,12);
 return;
}

void drowhate( CHAR_DATA *ch, char *argument )
{

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWHATE) )
 {
  do_huh(ch,"");
  return;
 }

 if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
 {
  stc("You calm your hatred.\n\r",ch);
  REM_BIT_POWERS(ch,DROW_BITS,DROW_HATE);
 }
 else
 {
  stc("You invoke your hatred for others.\n\r",ch);
  SET_BIT_POWERS(ch,DROW_BITS,DROW_HATE);
 }
 return;
}

void spiderform( CHAR_DATA *ch, char *argument ) 
{
 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_SPIDERFORM) )
 {
  do_huh(ch,"");
  return;
 }

 if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SPIDERFORM) )
 {
  stc("You return to your normal form.\n\r",ch);
  REM_BIT_POWERS(ch,DROW_BITS,DROW_SPIDERFORM);  
 }
 else
 {
  stc("You mutate into a large spider.\n\r",ch);
  SET_BIT_POWERS(ch,DROW_BITS,DROW_SPIDERFORM);
 }
 return;
}

void drowshield( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )           
  return;
  
 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWSHIELD) )
 {
  do_huh(ch,"");
  return;
 }
  
 if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SHIELD) )
 {
  stc("You stop shielding your aura.\n\r",ch);
  REM_BIT_POWERS(ch,DROW_BITS,DROW_SHIELD); 
 }
 else
 {   
  stc("You shield your aura from those around you.\n\r",ch);
  SET_BIT_POWERS(ch,DROW_BITS,DROW_SHIELD); 
 }
 return;
}

void drowpowers( CHAR_DATA *ch,char *argument )
{
 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) )
 {
  do_huh(ch,"");
  return;
 }

 display_class_stats(ch);
 return;
}

void darkness(CHAR_DATA *ch, char *argument)
{
 bool blah = FALSE;

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DARKNESS) )
 {
  do_huh(ch,"");
  return;
 }
 
 if( ch->pcdata->pulse_timer[PULSE_DARKNESS] < 1 )
 {
  stc("You lack the strength to summon darkness.\n\r",ch);
  return;
 }
 if( ch->fight_timer > 0 )
 {
  stc("You can not call upon the darkness with a fight timer active.\n\r",ch);
  return;
 }
 if( IS_SET(ch->newbits,NEW_DARKNESS) )
 {
  stc("You banish your globe of darkness.\n\r",ch);
  act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
  REMOVE_BIT(ch->newbits,NEW_DARKNESS);
  
  if(ch->in_room != NULL)
   if( IS_SET(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS) )
    REMOVE_BIT(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS);
  return;
 }
 else
 { 
  stc("You summon a globe of darkness.\n\r",ch);
  act("$n summons a globe of darkness.",ch,NULL,NULL,TO_ROOM);

  if( IS_SET(ch->extra,TIED_UP) )
  {
   REMOVE_BIT(ch->extra,TIED_UP);
   blah = TRUE;
  }

  if( IS_AFFECTED(ch,AFF_WEBBED) )
  {
   REMOVE_BIT(ch->affected_by,AFF_WEBBED);
   blah = TRUE;
  }
  SET_BIT(ch->newbits, NEW_DARKNESS);

  if( blah )
   stc("The darkness sets you free.\n\r",ch);

  if( !IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) )
   SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
  
  return;
 }
 
 return;
}
	
void earthshatter( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *vch;
 CHAR_DATA *vch_next;
 int dam;
 int level;
 int cost = 100;   

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_EARTHSHATTER) )
 {
  do_huh(ch,"");
  return;
 }

 if( ch->mana < cost )
 {
  stc("You need more mana.\n\r",ch);
  return;
 }
 
 level = ch->spl[PURPLE_MAGIC] * 2;
 ch->mana -= cost;
 stc("You summon the power of the underworld, shattering the earth!\n\r",ch);
 act("$n summons the power of the underworld, shattering the earth!",ch,NULL,NULL,TO_ROOM);

 for( vch = ch->in_room->people; vch != NULL; vch = vch_next )
 {
  vch_next = vch->next_in_room;
  if( vch == ch )
   continue;
  if( is_same_group(ch,vch) )
   continue;
  if( is_safe(ch,vch) )
  {
   stc("Not while safe.\n\r",ch);
   continue;
  }
  dam = dice(level,7);
  if( saves_spell(level,vch) )
   dam /= 2;
  damage(ch,vch,dam,skill_lookup("earthquake"));
 }
 WAIT_STATE(ch,12);
 return;
}

void garotte( CHAR_DATA *ch, char *argument )
{
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 OBJ_DATA *obj;
 int chance;

 one_argument(argument, arg);
 chance = number_percent();

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_GAROTTE) )
 {
  do_huh(ch,"");
  return;
 }

 if( arg[0] == '\0' )
 {
  stc("Garotte whom?\n\r",ch);
  return;
 }

 if( (victim = get_char_room(ch,arg)) == NULL)
 {
  stc("They aren't here.\n\r",ch);
  return;
 }

 if( victim == ch )
 {
  stc("That would be a bad idea.\n\r",ch);
  return;
 }

 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }

 if( ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL || obj->value[3] != 4)
  && ((obj = get_eq_char(ch,WEAR_HOLD)) == NULL || obj->value[3] != 4) )
 {
  stc("You need to wield a whip.\n\r",ch);
  return;
 }
     
 if( victim->fighting != NULL )
 {
  stc("You can't garotte a fighting person.\n\r",ch);
  return;
 }

 if( victim->hit < (victim->max_hit * 0.10) )
 {
  act("$N is too hurt for you to sneak up.",ch,NULL,victim,TO_CHAR);
  return;
 }
 check_killer(ch,victim);
 WAIT_STATE(ch,12);

 if( chance <= 5 )
 {
  ch->fight_timer += 5;
  damage(ch,victim,0,gsn_garotte);
  return;
 }

 ch->fight_timer += 5;
 one_hit(ch,victim,gsn_garotte,1);
 multi_hit(ch,victim,TYPE_UNDEFINED);
 return;
}

void dgarotte( CHAR_DATA *ch, char *argument )
{
 char buf[MAX_STRING_LENGTH];                 
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 OBJ_DATA *obj;
 int chance;
 
 one_argument(argument, arg);
 chance = number_percent();
  
 if( IS_NPC(ch) )
  return;
        
 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DGAROTTE) )
 {      
  do_huh(ch,"");
  return;
 }

 if( !IS_SET(ch->newbits,NEW_DARKNESS) )
 {
  stc("You have to be surrounded in darkness.\n\r",ch);
  return;
 }

 if( arg[0] == '\0' )
 {
  stc("Garotte whom?\n\r",ch);
  return;
 }

 if( (victim = get_char_room(ch,arg)) == NULL)
 {      
  stc("They are't here.\n\r",ch);
  return;
 }      
 
 if( victim == ch )
 {      
  stc("That would be a bad idea.\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 
 if( ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL || obj->value[3] != 4)
  && ((obj = get_eq_char(ch,WEAR_HOLD)) == NULL || obj->value[3] != 4) )
 {
  stc("You need to wield a whip.\n\r",ch);
  return;
 }
        
 if( victim->fighting != NULL )
 {      
  stc("You can't garotte a fighting person.\n\r",ch);
  return;
 }      
        
 if( victim->hit < (victim->max_hit * 0.20) ) 
 {      
  act("$N is too hurt for you to sneak up.",ch,NULL,victim,TO_CHAR);
  return;
 }
 if( IS_SET(ch->newbits,NEW_DARKNESS) )
 {
  stc("You banish your globe of darkness.\n\r",ch);
  act("The globe of darkness around $n disappears.",ch,NULL,NULL,TO_ROOM);
  REMOVE_BIT(ch->newbits,NEW_DARKNESS);
 
  if(ch->in_room != NULL)
   if( IS_SET(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS) )
    REMOVE_BIT(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS);
 }
 check_killer(ch,victim);
 WAIT_STATE(ch,24);

 if( chance <= 5 )
 {
  ch->fight_timer += 5; damage(ch,victim,0,gsn_garotte);
  return;
 }
  
 ch->fight_timer += 5;
 one_hit(ch,victim,gsn_garotte,1);
 multi_hit(ch,victim,TYPE_UNDEFINED);
 return;
}

void levitate( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_LEVITATION) )
 {
  do_huh(ch,"");
  return;
 }
 
 if( IS_SET(ch->affected_by,AFF_FLYING) )
 {
  stc("You gently float to the ground.\n\r",ch);
  REMOVE_BIT(ch->affected_by,AFF_FLYING);
 }
 else
 {
  stc("You slowly rise into the air.\n\r",ch);
  SET_BIT(ch->affected_by,AFF_FLYING);
 }
 return;
}

void strangle( CHAR_DATA * ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = 0;
 int cost = 100;
  
 if( IS_NPC(ch) )  
  return;

 if( !IS_CLASS(ch,CLASS_DROW) || (!IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) && ch->generation > 2) )
 {
  do_huh(ch,"");
  return;
 }

 if( ch->move < cost )  
 { 
  stc("You are too tired to strangle somebody.\n\r",ch);
  return;
 }

 if( ch->fighting == NULL )
 {
  stc("You need to be fighting in order to use strangle.\n\r",ch);
  return;
 }

 else if( ch->fighting != NULL )
  victim = ch->fighting;

 if( victim == ch )
 {
  stc("Not the brightest, are you?\n\r",ch);
  return;
 }

 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 
 check_killer(ch,victim);
 WAIT_STATE(ch,12);
 dam = (class_damroll(ch)/2);
 dam += number_range(ch->damroll/16,ch->damroll/8);
 ch->move -= cost;
 hurt_person(ch,victim,dam);
 update_pos(victim);

 act2("You wrap your whip around $N's neck, strangling them! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n wraps $s whip around your neck, strangling you! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n wraps $s whip around $N's neck, strangling $M! [$D]",ch,NULL,victim,TO_NOTVICT,dam);

 return;
}
