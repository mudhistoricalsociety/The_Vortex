/*****************************************************
* Cyborg code written for Godwars: The final chapter *
* Code written by Kale/Jalerit.  In the spirit of    *
* free code, it may be used by anyone as long as     *
* proper credit is given to me (Kale/Jalerit..you can*
* use either name)                                   *
*****************************************************/

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
#include "player.h"

/***********************************************************************
  *  implant                     requirements                          *
  * -----------------------------------------------------------        *
  *  heart                            none                             *
  *  liver                            none                             * 
  *  stomach                          none                             *
  *  neuro_processor                  none                             *
  *  adrenal_boost                    heart                            *
  *  booster_ware                     neuro_processor                  *
  *  boost_master                     booster_ware                     *
  *  nanite_factory                   stomach, liver, battery          *
  *  wolvers                          both arms replaced               *
  *  cham_field                       nanites, nero_processor, eyes    *
  *  muscle enhance                   nanites                          *
  *  bone lace                        nanites                          *
  *  radio_splice                     ears, neuro_proc                 *
  *  targeting_comp                   eyes, nero_proc                  *
  *  arms/legs                        neuro_proc/nanites               *
  *  battery                          none                             *
  *  eyes/ears                        neuroproc/nanites                *
  *  railgun                          arms, tele_optics, target_comp   *
  *  tele_optics                      eyes                             *
  *  taser                            arms, battery                    *
  **********************************************************************/

void do_taser( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_STRING_LENGTH];
 CHAR_DATA *victim;
 int dam;
 char buf[MAX_STRING_LENGTH];
 int cost = number_range(100, 300);

 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CYBORG(ch))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 if( !IS_SET(ch->pcdata->powers[IMPLANT], TASER))
 {
  stc("You have no taser installed.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("You cant seem to find them.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on npc's.\n\r", ch);
  return;
 }
 if( ch->pcdata->powers[TASER_TIME] > 0)
 {
  stc("The batteries have yet to fully charge.\n\r", ch);
  return;
 }
 if( ch->pcdata->powers[NANITES] < cost)
 {
  stc("You dont have enough nanites to taser with.\n\r", ch);
  return;
 }
 if( victim->level < 3)
 {
  stc("Not on mortals.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 ch->pcdata->powers[NANITES] -= cost;
 dam = number_range( 900, 1500);
 sprintf( buf, "You hit %s with your taser![%d]\n\r", victim->name, dam);
 stc( buf, ch);
 sprintf(buf, "%s hits you with a taser![%d]\n\r", ch->name, dam);
 stc(buf, victim);
 sprintf(buf, "%s hits %s with a taser![%d]\n\r", ch->name, victim->name, dam);
 act(buf, ch, NULL, victim, TO_NOTVICT);
 ch->fight_timer += 5;
 ch->pcdata->powers[TASER_TIME] = 3;
 if( ch->fighting == NULL)
  multi_hit(ch, victim, TYPE_UNDEFINED);
 WAIT_STATE(ch, 10);
 WAIT_STATE(victim, 15);
 return;
}
 
void do_rifle( CHAR_DATA *ch, char *argument)
{  
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_INPUT_LENGTH];
 int cost;
  
 argument = one_argument( argument, arg);
  
 if( IS_NPC(ch))
  return;
 if( !IS_CYBORG(ch))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 cost = (ch->pcdata->powers[RIFLE]+1) * 15;
if( arg[0] == '\0')
 {
  stc("Syntax: rifle <improve/show>.\n\r", ch);
  return;
 }
 if( !str_cmp( arg, "improve"))
 {
  if( ch->pcdata->powers[RIFLE] >= 12)
  {
  stc("SYSTEM NOTICE: Memory has already been fully augmented for ranged combat.\n\r", ch);
  return;
  }
  if( ch->practice < cost)
  {
  sprintf(buf,"SYSTEM ERROR: Missing the %d primal points required to purchase memory files.\n\r", cost);
  stc(buf, ch);
  return;
  }
  stc("SYSTEM NOTICE: Memory files have been upgraded.\n\r", ch);
  ch->pcdata->powers[RIFLE] += 1;
  ch->practice -= cost;
  return;
 }
 else if( !str_cmp(arg, "show"))
 {
  stc("SYSTEM: Analysing system...\n\r", ch);
  stc("SYSTEM: Analysis complete...\n\r", ch);
  sprintf(buf, "Memory files on demolitions have been upgraded %d times.\n\r",
           ch->pcdata->powers[RIFLE]);
  stc(buf, ch);
 sprintf(buf, "Bullet damage is estimated at %d to %d.\n\r", (100 * ch->pcdata->powers[RIFLE]),
           (165 * ch->pcdata->powers[RIFLE]));
  stc(buf, ch);
  return;
 }
 else
 {
  stc("Not a valid option.\n\r", ch);
  return;
 }
 return; 
}

void do_snipe( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    char  arg1[MAX_STRING_LENGTH];
    char  arg2[MAX_STRING_LENGTH];          
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if( IS_NPC(ch))
     return;
    if( !IS_CYBORG(ch))
    {
     stc("Huh?\n\r", ch);
     return;
    }   
    if( arg1[0] == '\0' )
    {
     send_to_char("Who would you like to snipe?\n\r", ch);
     return;
    }
    if( arg2[0] == '\0' )
    {
        send_to_char("Which direction do you wish to snipe?\n\r", ch);
        return;
    }
    if( !IS_SET(ch->pcdata->powers[IMPLANT], TELE_OPTICS))
    {
     send_to_char("You need telescopic vision.\n\r", ch);
     return;
    }
    if( !IS_SET(ch->pcdata->powers[IMPLANT], RAIL_GUN))
    {
     send_to_char("You need a rail gun.\n\r", ch);
     return;
    }
    if( (victim = get_char_world(ch, arg1)) == NULL)
    {
     send_to_char("They are not there.\n\r", ch);
     return;
    }
    if( ch->pcdata->powers[RIFLE] < 1)
    {
     send_to_char("You dont have the skill to shoot a gun.\n\r", ch);
     return;
    }
    if( safe_room(ch))
    {
     send_to_char("Not while you are safe.\n\r", ch);
     return;
    }
    if( safe_room(victim))
    {
     send_to_char("They are safe.\n\r", ch);
     return;
    }
    location = ch->in_room;
    
    snipedirection(ch,victim,3,arg2);
        
    /* Move them back */
    char_from_room(ch);
    char_to_room(ch,location);
    return;
}

void snipedirection( CHAR_DATA *ch, CHAR_DATA *victim,int range,char arg[MAX_STRING_LENGTH] )
{       
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA       *pexit;  
    CHAR_DATA       *vch;    
    CHAR_DATA       *vch_next;
    int             door;
    bool            found = FALSE;
    int             dam;
    char            buf[MAX_STRING_LENGTH];
    int             level;      
    bool            did_hit = FALSE;
    if( IS_NPC(ch))
     return;
    if ( arg[0] == '\0' )   
        return;
         
    if      ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else 
      {
        send_to_char("You can only snipe people north, south, east, west, up or down.\n\r", ch);
        return;
      }     
    
     if ( ((pexit = ch->in_room->exit[door]) == NULL) 
        || ((to_room = pexit->to_room) == NULL))
        return;
    level = 7*ch->pcdata->powers[RIFLE];
    if( level >= number_percent())
     did_hit = TRUE; 
    pexit = ch->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) )
    {
        send_to_char("Cant aim through doors.\n\r", ch);
        return;
    }
    char_from_room(ch);
    char_to_room(ch,to_room);
    for(vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
     vch_next = vch->next_in_room;
     if( vch == victim)
      found = TRUE;
    }
    if(!found && (range != 0))
     snipedirection(ch, victim, range-1, arg);
    if(!found && (range == 0))
    {
     send_to_char("They are not in range.\n\r", ch);
     return;
    }
    if( found && !did_hit)
    {
     stc("You missed your mark!\n\r", ch);
     act("A bullet flies out of nowhere and slams into the wall!\n\r", ch, NULL,victim,TO_VICT);
     return;
    }
    if(found && did_hit)
    {
     dam = number_range( (100 * ch->pcdata->powers[RIFLE]), (165 * ch->pcdata->powers[RIFLE]));
     victim->hit -= dam;
     if( victim->hit < -10)
     victim->hit = -10;
     sprintf(buf, "#5A bullet comes out of nowhere and slams into your body![%d]#n\n\r", dam);
     stc(buf,victim);
     sprintf(buf, "You hit %s with a well aimed shot!.[%d]\n\r", victim->name, dam);     
     stc( buf, ch);
     ch->fight_timer += 10;
     if( ch->fight_timer > 30)
       ch->fight_timer = 30;
     WAIT_STATE(ch, 12);
     return;
    }
  return;
}
void do_repair( CHAR_DATA *ch, char *argument)
{
  int heal = number_range( 300, 750);
  int cost = 400;
  if(IS_NPC(ch))
   return;
  if( !IS_CYBORG(ch))
   { stc("Huh?\n\r", ch); return;}
  if( ch->pcdata->powers[NANITES] < cost)
  {
   stc("SYSTEM ERROR: Not enough nanites to heal.\n\r", ch);
   return;
  }
  if( ch->hit >= ch->max_hit)
  {
   stc("SYSTEM ERROR: Nothing left to repair.\n\r", ch);
   return;
  }
  ch->hit += heal;
  ch->pcdata->powers[NANITES] -= cost;
  stc("Nanites crawl all over your body, repairing various damage.\n\r", ch);
  act("Nanites are exuded from $n's body and being repairs.\n\r", ch, NULL, NULL, TO_ROOM);
  WAIT_STATE(ch, 6);
  return;
}

void do_implant( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int humanity;
  int cost;
  int bit;
  bool can_do = TRUE;
  argument = one_argument(argument, arg);

  if( IS_NPC(ch))
  return;
  if( !IS_CYBORG(ch))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( arg[0] == '\0')
  {
   send_to_char("Implant what?\n\r", ch);
   send_to_char("Options are: neuroprocessor, boosterware, boostmaster,nanite, battery,\n\r",ch);
   send_to_char("wolvers, chameleon, muscle, bonelace, radiosplice, adrenalboost,\n\r", ch);
   send_to_char("targetcomp,teleoptic, railgun, taser.\n\r", ch);
   return;
  }
  if( !str_cmp( arg, "neuroprocessor"))
  {
   bit = NEUROPROCESSOR;
   humanity = 8;
   cost = 50;
  }
  else if( !str_cmp( arg, "boosterware"))
  {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
   {
    send_to_char("You need a neuro-processor first.\n\r", ch);
    can_do = FALSE;   
   }
   bit = BOOSTER_WARE;
   humanity = 7;
   cost = 30;
  }
  else if( !str_cmp( arg, "taser"))
  {
   if( !IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
    || !IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM)
    || !IS_SET(ch->pcdata->powers[IMPLANT], BATTERY))
   {
    stc("You must have both arms, and a battery installed.\n\r", ch);
    return;
   }
   bit = TASER;
   humanity = 4;
   cost = 20;
  }
  else if( !str_cmp( arg, "railgun"))
  {
   if( !IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
    || !IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM)
    || !IS_SET(ch->pcdata->powers[IMPLANT], TELE_OPTICS))
   {
    stc("You need both arms replaced, and teleoptics installed.\n\r", ch);
    return;
   }
   bit = RAIL_GUN;
   humanity = 6;
   cost = 30;
  }
  else if( !str_cmp( arg, "teleoptics"))
  {
   if( !IS_SET(ch->pcdata->powers[REPLACED], REP_EYES))
   {
    stc("You need your eyes replaced first.\n\r", ch);
    return;
   }
   bit = TELE_OPTICS;
   humanity = 3;
   cost = 25;
  } 
  else if( !str_cmp( arg, "boostmaster"))
  {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], BOOSTER_WARE))
   {
    send_to_char("You need boosterware first.\n\r", ch);
    can_do = FALSE;
   }
   bit = BOOST_MASTER;
   humanity = 6;
   cost = 30;
  }
  else if( !str_cmp( arg, "nanite"))
  {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], BATTERY)
   || !IS_SET(ch->pcdata->powers[REPLACED], REP_STOMACH)
   || !IS_SET(ch->pcdata->powers[REPLACED], REP_LIVER))
   {
    send_to_char("You first need a battery, new stomach, and a new liver.\n\r", ch);
    can_do = FALSE;
   }
   bit = NANITE_FACTORY;
   humanity = 4;
   cost = 25;
  }
  else if( !str_cmp( arg, "battery"))
  {
   bit = BATTERY;
   humanity = 5;
   cost = 30;
  }
  else if( !str_cmp( arg, "wolvers"))
  {
   if( !IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
   || !IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM))
   {
    send_to_char("You must have both arms replaced first.\n\r", ch);
    can_do = FALSE;
   }
   bit = WOLVERS;
   humanity = 8;
   cost = 35;
  }
  else if( !str_cmp( arg, "chameleon"))
  {
   if( !IS_SET(ch->pcdata->powers[REPLACED], REP_EYES)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR)
   || !IS_SET(ch->pcdata->powers[IMPLANT], BATTERY))
   {
    send_to_char("You need implant a nanite factory, neuro_processor, a batter, and replace.\n\r", ch);
    send_to_char("your eyes to get the chameleon generator.\n\r", ch);
    can_do = FALSE;
   }
   bit = CHAM_FIELD;
   humanity = 10;
   cost = 50;
  }
  else if( !str_cmp( arg, "muscle"))
  {
   if( !IS_SET( ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
   {
    send_to_char("You need a nanite factory to install this.\n\r", ch);
    can_do = FALSE;
   }
   bit = MUSCLE_ENHANCE;
   humanity = 8;
   cost = 30;
  }
  else if( !str_cmp(arg, "bonelace"))
  {
   if( !IS_SET( ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
   {
    send_to_char("You need a nanite factory to install this.\n\r", ch);
    can_do = FALSE;
   }
   bit = BONE_LACE;
   humanity = 6;
   cost = 25;
  }
  else if( !str_cmp(arg, "radiosplice"))
  {
   if( !IS_SET( ch->pcdata->powers[REPLACED], REP_EARS)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
   {
    send_to_char("You need ear replacements and a neuroprocessor for this.\n\r", ch);
    can_do = FALSE;
   }
   bit = RADIO_SPLICE;
   humanity = 7;
   cost = 25;
  }
  else if( !str_cmp( arg, "adrenalboost"))
  {
   if( !IS_SET( ch->pcdata->powers[REPLACED], REP_HEART))
   {
    send_to_char("You need a replaced heart first.\n\r", ch);
    can_do = FALSE;
   }
   bit = ADRENAL_BOOST;
   humanity = 5;
   cost = 25;
  }
  else if( !str_cmp(arg, "targetcomp"))
  {
   if( !IS_SET( ch->pcdata->powers[REPLACED], REP_EYES)
   || !IS_SET(ch->pcdata->powers[IMPLANT], BATTERY)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
   {
    send_to_char("You need eye replacements, a battery, and a neuroprocessor for this.\n\r", ch);
    can_do = FALSE;
   }
   bit = TARGET_COMP;
   humanity = 8;
   cost = 35;
  }
  else
  {
   send_to_char("That is not a valid option.\n\r", ch);
   return;
  }
  if( can_do == FALSE)
    return;
  if( ch->practice < cost)
  {
   sprintf( buf, "You need at least %d primal to gain that implant.\n\r", cost);
   send_to_char( buf, ch);
   return;
  }
/*  if( ch->pcdata->powers[HUMANITY] < humanity)
  {
   sprintf( buf, "You need at least %d humanity to gain this.\n\r", humanity);
   send_to_char( buf, ch);
   return;
  }*/
  if( bit <= 0)
  {
   send_to_char("BUG: REPORT TO IMPS.\n\r", ch);
   return;
  }
  if( IS_SET( ch->pcdata->powers[IMPLANT], bit))
  {
   send_to_char("That has already been implanted in you.\n\r", ch);
   return;
  }
  ch->pcdata->powers[HUMANITY] -= humanity;
  ch->practice -= cost;
  SET_BIT(ch->pcdata->powers[IMPLANT], bit);
  do_save(ch, "");
  sprintf( buf, "You have installed: %s.\n\r", arg);
  return;
}

void do_replace(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int cost;
  int bit;
  int humanity;
  bool can_do = TRUE;
  argument = one_argument( argument, arg);

  if( IS_NPC(ch))
  return;
  if( !IS_CYBORG(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( arg[0] == '\0')
  {
   send_to_char("Replace what?\n\r", ch);
   send_to_char("Options are: arm, leg, heart, liver, stomach, eyes, and ears.\n\r", ch);
   return;
  }
  if( !str_cmp( arg, "arm"))
  {
   if( !IS_SET( ch->pcdata->powers[IMPLANT], NEUROPROCESSOR)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
   {
    send_to_char("You need a neuroprocessor and a nanite factory.\n\r", ch);
    can_do = FALSE;
   }
   if( !IS_SET( ch->pcdata->powers[REPLACED], REP_L_ARM)
   && IS_SET( ch->pcdata->powers[REPLACED], REP_R_ARM))
   {
    bit = REP_L_ARM;
    humanity = 8;
    cost = 40;
   }
   else if( !IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM))
   {
    bit = REP_R_ARM;
    humanity = 8;
    cost = 40;
   }
   else
    can_do = FALSE;
  }
  else if( !str_cmp( arg, "leg"))
  {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR)
   || !IS_SET( ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
   {
    send_to_char("You need a neuroprocessor and a nanite factory.\n\r", ch);
    can_do = FALSE;
   }
   if( !IS_SET( ch->pcdata->powers[REPLACED], REP_L_LEG)
   && IS_SET( ch->pcdata->powers[REPLACED], REP_R_LEG))
   {
    bit = REP_L_LEG;
    humanity = 8;
    cost = 40;
   }
   else if( !IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG))
   {
    bit = REP_R_LEG;
    humanity = 8;
    cost = 40;
   }
   else
    can_do = FALSE;
  }
  else if( !str_cmp( arg, "heart"))
  {
   bit = REP_HEART;
   humanity = 4;
   cost = 30;
  }
  else if( !str_cmp( arg, "liver"))
  {
   bit = REP_LIVER;
   humanity = 4;
   cost = 30;
  }
  else if( !str_cmp( arg, "stomach"))
  {
   bit = REP_STOMACH;
   humanity = 4;
   cost = 30;
  }
  else if( !str_cmp( arg, "eyes"))
  {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR)
    || !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
   {
    send_to_char("You need a neuro-processor and nanites to get eyes.\n\r", ch);
    can_do = FALSE;
   }
   bit = REP_EYES;
   humanity = 8;
   cost = 30;
  }
  else if( !str_cmp( arg, "ears"))
  {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR)
    || !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
   {
    send_to_char("You need a neuro-processor and nanites to get ears.\n\r", ch);
    can_do = FALSE;
   }
   bit = REP_EARS;
   humanity = 8;
   cost = 30;
  }
  else
  {
   send_to_char("That is not a valid option.\n\r", ch);
   return;
  }
  if( can_do == FALSE)
   return;
/*  if( ch->pcdata->powers[HUMANITY] < humanity)
  {
   sprintf( buf, "You need at least %d humanity to replace your %s.\n\r", humanity, arg);
   send_to_char( buf, ch);
   return;
  }*/
  if( ch->practice < cost)
  {
   sprintf(buf, "You need at least %d primal to replace your %s.\n\r", cost, arg);
   send_to_char( buf, ch);
   return;
  }
  if( IS_SET( ch->pcdata->powers[REPLACED], bit))
  {
   send_to_char("That has already been replaced.\n\r", ch);
   return;
  }
  ch->pcdata->powers[HUMANITY] -= humanity;
  ch->practice -= cost;
  SET_BIT(ch->pcdata->powers[REPLACED], bit);
  sprintf( buf, "You have replaced your %s.\n\r", arg);
  send_to_char( buf, ch);
  do_save( ch, "");
  return;
}

void do_make_bomb( CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *obj2;
  int cost = 1000;
  
  if( IS_NPC(ch))
  return;
  if( !IS_CYBORG(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->level < 3)
  return;
  if( ch->pcdata->powers[NANITES] < cost)
  {
   send_to_char("SYSTEM ERROR: Nanite reserves are insufficent for bomb creation.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
  {
   send_to_char("SYSTEM ERROR: Missing implants: Nanite factory or a neuroprocessor.\n\r", ch);
   return;
  }
  if ( (obj2 = get_obj_index( OBJ_VNUM_BOMB ) ) == NULL )
  {
   send_to_char( "Error...missing object, please inform Kale.\n\r", ch );
   return;
  }
  obj = create_object( obj2, 25 );
  obj->item_type = ITEM_BOMB;
  obj->value[3] = -1;
  obj->value[4] = 0;
/*  free_string(obj->name);
  obj->name = str_dup("Bomb");
  free_string(obj->short_descr);
  obj->short_descr = str_dup("A bomb");
  free_string(obj->description);
  obj->description = str_dup("A bomb has been left to explode on the ground.");*/
  obj->timer = 7;
  obj_to_char(obj, ch);
  send_to_char("You have made a bomb.\n\r", ch);
  act("$n has made a bomb!\n\r", ch, NULL, NULL, TO_ROOM);
  ch->pcdata->powers[NANITES] -= cost;
  WAIT_STATE(ch, 12);
  return;
}

void do_arm( CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  int timer;
  argument = one_argument( argument, arg1);
  argument = one_argument( argument, arg2);

  if( IS_NPC(ch))
  return;
  if( !IS_CYBORG(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->level < 3)
  return;
  if( (arg1[0] == '\0') || (arg2[0] =='\0'))
  {
   send_to_char("SYSTEM ERROR: No target bomb or missing timer specifications.\n\r", ch);
   return;
  }
  if( !is_number(arg2))
  {
   send_to_char("SYSTEM ERROR: Timer input is non-numeric, please re-specify.\n\r", ch);
   return;
  }
  if(( obj = get_obj_carry( ch, arg1 )) == NULL)
  {
   send_to_char( "SYSTEM ERROR: No bomb found.\n\r", ch );
   return;
  }
  timer = atoi(arg2);
  if( (timer < 1) || (timer > 30))
  {
   stc("SYSTEM ERROR: Fuse can't burn for less than 1 second or longer than 30 seconds.\n\r", ch);
   return;
  }
  if( obj->item_type != ITEM_BOMB)
  {
   stc("SYSTEM ERROR: Specified target object is not categorized as an explosive..aborting.\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DEMOLITION] == 0)
  {
   stc("SYSTEM ERROR: Missing memory files on demolition technology..aborting.\n\r", ch);
   return;
  }
  obj->value[0] = 125 * (ch->pcdata->powers[DEMOLITION]);
  obj->value[1] = 350 * (ch->pcdata->powers[DEMOLITION]);
  obj->value[2] = 2;
  obj->value[3] = timer;
  stc("Arming...\n\r", ch);
  stc("Arming successful, vacate blast zone immediately.\n\r", ch);
  act("$n has just armed a bomb!\n\r", ch, NULL,NULL, TO_ROOM);
  WAIT_STATE(ch, 6);
  return;
}

void do_mine( CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 OBJ_INDEX_DATA *obj2;
 int cost = 1500;
 
 if( IS_NPC(ch))
  return;
 if( !IS_CYBORG(ch))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 if( ch->pcdata->powers[DEMOLITION] < 1)
 {
  stc("SYSTEM ERROR: Missing memory files on demolition technology...aborting.\n\r", ch);
  return;
 }
 for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
 {
  if( obj->item_type != ITEM_BOMB)
  continue;
  if( (obj->item_type == ITEM_BOMB) &&  (obj->value[4] == BOMB_MINE));
  {
   send_to_char("SYSTEM ERROR: A mine has already been lain here...aborting.\n\r", ch);
   return;
  }
 }
 if( ch->pcdata->powers[NANITES] < cost)
 {
  stc("SYSTEM ERROR: Missing required amount of nanites...aborting.\n\r", ch);
  return;
 }
 if( !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY)
   || !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
 {
  send_to_char("SYSTEM ERROR: Missing implants: Nanite factory or a neuroprocessor.\n\r", ch);
  return;
 }
 if ( (obj2 = get_obj_index( OBJ_VNUM_MINE ) ) == NULL )
 {
  send_to_char( "Error...missing object, please inform Kale.\n\r", ch );
  return;
 }
 obj = create_object( obj2, 25 );
 obj->item_type = ITEM_BOMB;
 obj->value[0] = 130 * ch->pcdata->powers[DEMOLITION];
 obj->value[1] = 360 * ch->pcdata->powers[DEMOLITION];
 obj->value[2] = 3;
 obj->value[3] = -1;
 obj->value[4] = BOMB_MINE;
 obj->armed_by = ch;
/* free_string(obj->name);
 obj->name = str_dup("Mine");
 free_string(obj->short_descr);
 obj->short_descr = str_dup("A mine");
 free_string(obj->description);
 obj->description = str_dup("A mine has been left as a trap.");*/
 obj_to_room(obj, ch->in_room);
 stc("SYSTEM: Constructing...construction complete.\n\r", ch);
 act("$n has made a mine!\n\r", ch, NULL, NULL, TO_ROOM);
 ch->pcdata->powers[NANITES] -= cost;
 if( IS_SET(obj->wear_flags, ITEM_TAKE))
  REMOVE_BIT( obj->wear_flags, ITEM_TAKE);
 WAIT_STATE(ch, 12);
 return;
}

void do_demolitions( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_INPUT_LENGTH];
 int cost;

 argument = one_argument( argument, arg);
 
 if( IS_NPC(ch))
  return;
 if( !IS_CYBORG(ch))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 cost = (ch->pcdata->powers[DEMOLITION]+1) * 15;
 if( arg[0] == '\0')
 {
  stc("Syntax: demolition <improve/show>.\n\r", ch);
  return;
 }
 if( !str_cmp( arg, "improve"))
 {
  if( ch->pcdata->powers[DEMOLITION] >= 12)
  {
  stc("SYSTEM NOTICE: Memory has already been fully augmented for demolition knowledge.\n\r", ch);
  return;
  }
  if( ch->practice < cost)
  {
  sprintf(buf,"SYSTEM ERROR: Missing the %d primal points required to purchase memory files.\n\r", cost);
  stc(buf, ch);
  return;
  }
  stc("SYSTEM NOTICE: Memory files have been upgraded.\n\r", ch);
  ch->pcdata->powers[DEMOLITION] += 1;
  ch->practice -= cost;
  return;
 }
 else if( !str_cmp(arg, "show"))
 {
  stc("SYSTEM: Analysing system...\n\r", ch);
  stc("SYSTEM: Analysis complete...\n\r", ch);
  sprintf(buf, "Memory files on demolitions have been upgraded %d times.\n\r",
           ch->pcdata->powers[DEMOLITION]);
  stc(buf, ch);
  sprintf(buf, "Bomb damage is estimated at %d to %d.\n\r", (125 * ch->pcdata->powers[DEMOLITION]),
           (350 * ch->pcdata->powers[DEMOLITION]));
  stc(buf, ch);
  sprintf(buf, "Mine damage is estimated at %d to %d.\n\r", (130 * ch->pcdata->powers[DEMOLITION]),
           (360 * ch->pcdata->powers[DEMOLITION]));
  stc(buf, ch);
  sprintf(buf, "Missle damage is estimated at %d to %d.\n\r", (140 * ch->pcdata->powers[DEMOLITION]),
           (420 * ch->pcdata->powers[DEMOLITION]));
  stc(buf, ch);
  return;
 }
 else
 {
  stc("Not a valid option.\n\r", ch);
  return;
 }
 return;
}

void do_factory( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  
  argument = one_argument(argument, arg);
 
  if(IS_NPC(ch))
   return;
  if( !IS_CYBORG(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( arg[0] == '\0')
  {
   send_to_char("Syntax: factory <on/off>.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
  {
   stc("SYSTEM ERROR: Missing implant: Nanite factory.\n\r", ch);
   return;
  }
  if( !str_cmp( arg, "on"))
  {
   if( ch->pcdata->powers[FACTORY] == 1)
   {
    stc("SYSTEM ERROR: Factory has already been turned on.\n\r", ch);
    return;
   }
   else
   {
    ch->pcdata->powers[FACTORY] = 1;
    stc("SYSTEM: Acknowledged. Nanite factory has been turned on.\n\r", ch);
   }
   return;
  }
  else if( !str_cmp( arg, "off"))
  {
   if( ch->pcdata->powers[FACTORY] == 0)
   {
    stc("SYSTEM ERROR: Factory is already in the off position.\n\r", ch);
    return;
   }
   else
   {
    ch->pcdata->powers[FACTORY] = 0;
    stc("SYSTEM: Acknowledged. Nanite factory has been powered down.\n\r", ch);
   }
   return;
  }
  else
  {
   stc("SYSTEM ERROR: Option not identified.\n\r", ch);
   return;
  }
  return;
}

void do_sys_status(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
  return;
  if( !IS_CYBORG(ch))
  {
   stc("Huh?\n\r", ch);
   return;
  }
  stc("SYSTEM: Checking system configuration.\n\r", ch);
  stc("        System check complete...compiling results.\n\r", ch);
  stc("        Compilation complete..sending data to output platform.\n\r", ch);
  stc("        Implants:\n\r", ch);
  stc("        -----------------------------------------------------------\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], ADRENAL_BOOST))
  stc("         Adrenal booster\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
  stc("         Neuroprocessor\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], BOOSTER_WARE))
  stc("         Boosterware neural accelerator.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], BOOST_MASTER))
  stc("         BoostMaster upgrade for Boosterware.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
  stc("         Nanite factory.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], WOLVERS))
  stc("         Wolver wrist blades.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], TASER))
  stc("         Stun taser deteced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], CHAM_FIELD))
  stc("         Chameleon cloaking field.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], MUSCLE_ENHANCE))
  stc("         Muscle enhancing fibers detected.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], BONE_LACE))
  stc("         Bone strengthening fibers detected.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], RADIO_SPLICE))
  stc("         Radio splicing installed.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], TARGET_COMP))
  stc("         Targeting computer detected.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], BATTERY))
  stc("          Nuclear power cell detected.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], RAIL_GUN))
  stc("          Rail gun add on detected.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[IMPLANT], TELE_OPTICS))
  stc("          Tele optics upgrade detected.\n\r", ch);
  stc("          Replacements:\n\r", ch);
  stc("          ---------------------------------------------------------\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM))
  stc("          Left arm has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM))
  stc("          Right arm has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_L_LEG))
  stc("          Left leg has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG))
  stc("          Right leg has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_HEART))
  stc("          Your heart has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_LIVER))
  stc("          Your liver has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_STOMACH))
  stc("          Your stomach has been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_EYES))
  stc("          Your eyes have been replaced.\n\r", ch);
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_EARS))
  stc("          Your ears have been replaced.\n\r", ch);
  stc("          --------------------------------------------------------\n\r", ch);
  sprintf( buf, "You have %d nanites in stasis.\n\r", ch->pcdata->powers[NANITES]);
  stc( buf, ch);
  stc("Psychological evaluation program running....\n\r", ch);
  sprintf(buf, "You have %d percent of your humanity left.\n\r", ch->pcdata->powers[HUMANITY]);
  stc(buf, ch);
  stc("End of report...terminating analysis program.\n\r", ch);
  return;
}

void do_launch(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  int cost = 2000;
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *obj2;
  argument = one_argument( argument, arg);

  if( IS_NPC(ch))
  return;
  if( !IS_CYBORG(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[NANITES] < cost)
  {
   send_to_char("SYSTEM ERROR: Not enough nanites to make a missle.\n\r", ch);
   return;
  }
  if( arg[0] =='\0')
  {
   stc("SYSTEM ERROR: No target designated.\n\r", ch);
   return;
  }
  if( (victim = get_char_world( ch, arg)) == NULL)
  {
   stc("SYSTEM ERROR: Designated target can not be found.\n\r", ch);
   return;
  }
  if( victim->in_room == NULL || ch->in_room->area != victim->in_room->area)
  {
   stc("SYSTEM ERROR: Target is out of range.\n\r", ch);
   return;
  }
  if( is_safe(ch, victim))
  {
   stc("SYSTEM ERROR: Target is shielded from targetting sensors.\n\r", ch);
   return;
  }
  if( victim->level < 3)
  {
   stc("Only on avatars.\n\r", ch);
   return;
  }
  if( IS_SET(ch->in_room->room_flags, ROOM_SAFE))
  {
   stc("Not in safe room.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_L_LEG) 
   || !IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG))
  {
   stc("SYSTEM_ERROR: Your inferior organic legs cant handle the pressure.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->pcdata->powers[IMPLANT], TARGET_COMP))
  {
   stc("SYSTEM_ERROR: No targetting computer connected to system...aborting.\n\r", ch);
   return;
  }
  if ( (obj2 = get_obj_index( OBJ_VNUM_MISSLE ) ) == NULL )
  {
   send_to_char( "Error...missing object, please inform Kale.\n\r", ch );
   return;
  }
  obj = create_object( obj2, 25 );
  obj->item_type = ITEM_BOMB;
  obj->value[0] = 140 * ch->pcdata->powers[DEMOLITION];
  obj->value[1] = 420 * ch->pcdata->powers[DEMOLITION];
  obj->value[2] = 4;
  obj->value[3] = 5;
  obj->value[4] = 0;
  if( IS_SET(obj->wear_flags, ITEM_TAKE))
   REMOVE_BIT( obj->wear_flags, ITEM_TAKE);
  obj_to_room(obj, victim->in_room);
  act("$n has launched a missle!\n\r", ch, NULL, NULL, TO_ROOM);
  stc("You have launched a missle.\n\r", ch);
  if( ch->pcdata->powers[CHAM] == 1)
   ch->pcdata->powers[CHAM] = 0;
  ch->fight_timer = 30;
  ch->pcdata->powers[NANITES] -= cost;
  WAIT_STATE(ch, 20);
  return;
}

void do_splice( CHAR_DATA *ch, char *argument)
{
 int channel = 0;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CYBORG(ch) || (ch->level < 3))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 if( arg[0] =='\0')
 {
  stc("SYSTEM ERROR: No frequency selected.\n\r", ch);
  return;
 }
 if( !IS_SET(ch->pcdata->powers[IMPLANT], RADIO_SPLICE))
 {
  stc("SYSTEM ERROR: No Radio slicer detected in system.\n\r", ch);
  return;
 }
 if( !str_cmp( arg, "none"))
  channel = 0;
 else if( !str_cmp( arg, "vampire"))
  channel = SPLICE_VAMP;
 else if( !str_cmp( arg, "wolf"))
  channel = SPLICE_WOLF;
 else if( !str_cmp( arg, "ninja"))
  channel = SPLICE_NINJA;
 else if( !str_cmp( arg, "drow"))
  channel = SPLICE_DROW;
 else if( !str_cmp( arg, "dragon"))
  channel = SPLICE_DRAG;
 else if( !str_cmp( arg, "monk"))
  channel = SPLICE_MONK;
 else if( !str_cmp( arg, "mage"))
  channel = SPLICE_MAGE;
 else if( !str_cmp( arg, "demon"))
  channel = SPLICE_DEMON;
 else if( !str_cmp( arg, "jedi"))
  channel = SPLICE_JEDI;
 else if( !str_cmp( arg, "amazon"))
  channel = SPLICE_AMA;
 else
 {
  stc("SYSTEM ERROR: Not a valid frequency.\n\r", ch);
  return;
 }
 stc("SYSTEM: Frequency has been found, sending audio transmission now...\n\r", ch);
 ch->pcdata->powers[SPLICE] = channel;
 WAIT_STATE(ch, 12);
 return;
} 

void do_uninstall( CHAR_DATA *ch, char *argument)
{
 /*oh this func is gonna be fuuuun...NOT*/
 char arg[MAX_STRING_LENGTH];
 int humanity = 0;
 bool ok = TRUE;
 int bit = 0;
 bool implant = FALSE;
 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CYBORG(ch))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("SYSTEM ERROR: No implant designated..aborting.\n\r", ch);
  return;
 }
 /* first those implants with no dependancies */
 if( !str_cmp( arg, "taser"))
 {
   if( !IS_SET(ch->pcdata->powers[IMPLANT], TASER))
   {
    stc("SYSTEM ERROR: No taser installed.\n\r", ch);
    ok = FALSE;
   }
   bit = TASER;
   implant = TRUE;
   humanity = 4;
 } 
 else if( !str_cmp( arg, "railgun"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], RAIL_GUN))
  {
   stc("SYSTEM ERROR: Rail gun not installed.\n\r", ch);
   ok = FALSE;
  }
  bit = RAIL_GUN;
  implant = TRUE;
  humanity = 6;
 }
 else if( !str_cmp( arg, "teleoptics"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], TELE_OPTICS))
  {
  stc("SYSTEM ERROR: Teleoptics not installed.\n\r", ch);
  return;
  }
  bit = TELE_OPTICS;
  implant = TRUE;
  humanity = 3;
 }
 else if( !str_cmp( arg, "adrenalboost"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], ADRENAL_BOOST))
  { 
   stc("SYSTEM ERROR: Adrenal booster not installed.\n\r", ch);
   ok = FALSE;
  }
  bit = ADRENAL_BOOST;
  implant = TRUE;
  humanity = 5;
 }
 else if( !str_cmp( arg, "boostmaster"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], BOOST_MASTER))
  {
   stc("SYSTEM ERROR: Boost Master not installed.\n\r", ch);
   ok = FALSE;
  }
  bit = BOOST_MASTER;
  implant = TRUE;
  humanity = 6;
 }
 else if( !str_cmp( arg, "chameleon"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], CHAM_FIELD))
  {
   stc("SYSTEM ERROR: Chameleon cloaking field not installed.\n\r", ch);
   ok = FALSE;
  }
  bit = CHAM_FIELD;
  implant = TRUE;
  humanity = 10;
 }
 else if( !str_cmp(arg, "muscle"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], MUSCLE_ENHANCE))
  {
   stc("SYSTEM ERROR: No muscle augmentation found.\n\r", ch);
   ok = FALSE;
  }
  bit = MUSCLE_ENHANCE;
  implant = TRUE;
  humanity = 8;
 }
 else if( !str_cmp( arg, "bonelace"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], BONE_LACE))
  {
   stc("SYSTEM ERROR: Bone augmentation not installed.\n\r", ch);
   ok = FALSE;
  }
  bit = BONE_LACE;
  implant = TRUE;
  humanity = 6;
 }
 else if( !str_cmp( arg, "radiosplice"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], RADIO_SPLICE))
  {
   stc("SYSTEM ERROR: Radio splicer not found.\n\r", ch);
   ok = FALSE;
  }
  ch->pcdata->powers[SPLICE] = 0;
  bit = RADIO_SPLICE;
  implant = TRUE;
  humanity = 7;
 }
 else if( !str_cmp( arg, "targetcomp"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], TARGET_COMP))
  {
   stc("SYSTEM ERROR: No targeting computer installed.\n\r", ch);
   ok = FALSE;
  }
  bit = TARGET_COMP;
  implant = TRUE;
  humanity = 8;
 }
 else if( !str_cmp( arg, "leg"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG)
   && !IS_SET(ch->pcdata->powers[REPLACED], REP_L_LEG))
  {
   stc("SYSTEM ERROR: No cybernetic leg replacements installed.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_L_LEG)
   && !IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG))
    bit = REP_L_LEG;
  else
    bit = REP_R_LEG;
  humanity = 8;
  implant = FALSE;
 }
 /* now we start those that have dependant systems */
 else if( !str_cmp( arg, "heart"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_HEART))
  {
   stc("SYSTEM ERROR: No cybernetic heart installed.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], ADRENAL_BOOST))
  {
   stc("SYSTEM ERROR: Adrenal boost must be removed first.\n\r", ch);
   ok = FALSE;
  }
  bit = REP_HEART;
  implant = FALSE;
  humanity = 4;
 }
 else if( !str_cmp( arg, "stomach"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_STOMACH))
  {
   stc("SYSTEM ERROR: No stomach replacement found.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
  {
   stc("SYSTEM ERROR: Nanite factory must be removed first.\n\r", ch);
   ok = FALSE;
  }
  bit = REP_STOMACH;
  implant = FALSE;
  humanity = 4;
 }
 else if( !str_cmp( arg, "liver"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_LIVER))
  {
   stc("SYSTEM ERROR: No liver replacement found.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
  {
   stc("SYSTEM ERROR: Nanite factory must be removed first.\n\r", ch);
   ok = FALSE;
  }
  bit = REP_LIVER;
  implant = FALSE;
  humanity = 4;
 }
 else if( !str_cmp( arg, "wolvers"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], WOLVERS))
  {
   stc("SYSTEM ERROR: Wolvers not installed.\n\r", ch);
   ok = FALSE;
  }
  bit = WOLVERS;
  implant = TRUE;
  humanity = 8;
 }
 else if( !str_cmp( arg, "arm"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
   && !IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM))
  {
   stc("SYSTEM ERROR: No cybernetic arm replacements found.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], WOLVERS)
   || IS_SET(ch->pcdata->powers[IMPLANT], RAIL_GUN)
   || IS_SET(ch->pcdata->powers[IMPLANT], TASER))
  {
   stc("SYSTEM ERROR: Wolvers,taser, and rail gun must be removed first.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
   && !IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM))
   bit = REP_L_ARM;
  else
   bit = REP_R_ARM;
  humanity = 8;
  implant = FALSE;
 }
 else if( !str_cmp(arg, "battery"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], BATTERY))
  {
   stc("SYSTEM ERROR: No battery installed.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY)
   || IS_SET(ch->pcdata->powers[IMPLANT], TASER))
  {
   stc("SYSTEM ERROR: Nanite factory and taser must be removed first.\n\r", ch);
   ok = FALSE;
  }
  bit = BATTERY;
  implant = TRUE;
  humanity = 5;
 }
 else if( !str_cmp( arg, "ears"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_EARS))
  {
   stc("SYSTEM ERROR: No auditory rebuild detected.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET( ch->pcdata->powers[IMPLANT], RADIO_SPLICE))
  {
   stc("SYSTEM ERROR: Radio splice must be removed first.\n\r", ch);
   ok = FALSE;
  }
  bit = REP_EARS;
  implant = FALSE;
  humanity = 8;
 }
 else if( !str_cmp( arg, "eyes"))
 {
  if( !IS_SET(ch->pcdata->powers[REPLACED], REP_EYES))
  {
   stc("SYSTEM ERROR: No optical rebuild detected.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], CHAM_FIELD)
   || IS_SET(ch->pcdata->powers[IMPLANT], TELE_OPTICS)
   || IS_SET(ch->pcdata->powers[IMPLANT], TARGET_COMP))
  {
   stc("SYSTEM ERROR: Must remove following items: Cham field, Targeting computer,Teleoptics.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->act, PLR_HOLYLIGHT) && (ok == TRUE))
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
  bit = REP_EYES;
  implant = FALSE;
  humanity = 8;
 }
 else if( !str_cmp(arg, "boosterware"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], BOOSTER_WARE))
  {
   stc("SYSTEM ERROR: No Boosterware installed.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], BOOST_MASTER))
  {
   stc("SYSTEM ERROR: Must remove boost master first.\n\r", ch);
   ok = FALSE;
  }
  bit = BOOSTER_WARE;
  implant = TRUE;
  humanity = 7;
 }
 else if( !str_cmp( arg, "neuroprocessor"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], NEUROPROCESSOR))
  {
   stc("SYSTEM ERROR: Neuro-Processor not installed.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], CHAM_FIELD)
   || IS_SET(ch->pcdata->powers[IMPLANT], RADIO_SPLICE)
   || IS_SET(ch->pcdata->powers[IMPLANT], TARGET_COMP)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_EYES)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_EARS)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_L_LEG)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG))
  {
   stc("SYSTEM ERROR: Following items must be removed first: Chameleon field, Radio splice,\n\r", ch);
   stc("targeting computer, eyes, ears, arms and legs.\n\r", ch);
   ok = FALSE;
  }
  bit = NEUROPROCESSOR;
  implant = TRUE;
  humanity = 7;
 }
 else if( !str_cmp( arg, "nanite"))
 {
  if( !IS_SET(ch->pcdata->powers[IMPLANT], NANITE_FACTORY))
  {
   stc("SYSTEM ERROR: No Nanite factory found.\n\r", ch);
   ok = FALSE;
  }
  if( IS_SET(ch->pcdata->powers[IMPLANT], CHAM_FIELD)
   || IS_SET(ch->pcdata->powers[IMPLANT], MUSCLE_ENHANCE)
   || IS_SET(ch->pcdata->powers[IMPLANT], BONE_LACE)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_R_ARM)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_L_ARM)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_R_LEG)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_L_LEG)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_EARS)
   || IS_SET(ch->pcdata->powers[REPLACED], REP_EYES))
  {
   stc("SYSTEM ERROR: Following items must be removed first: Chameleon field, bone lace,\n\r", ch);
   stc("muscle enhancement, arms, legs, ears, eyes.\n\r", ch);
   ok = FALSE;
  }
  bit = NANITE_FACTORY;
  implant = TRUE;
  humanity = 4;
 }
 else
 {
  stc("Options are: heart, liver, eyes, ears, nanite, stomach, neuroprocessor,\n\r", ch);
  stc("adrenalboost, boosterware, boostmaster, wolvers, chameleon, muscle, bonelace,\n\r", ch);
  stc("radiosplice, targetcomp, arm, leg, battery.\n\r", ch);
  return;
 }
 if( !ok)
  return;
 if( implant)
  REMOVE_BIT(ch->pcdata->powers[IMPLANT], bit);
 else
  REMOVE_BIT(ch->pcdata->powers[REPLACED], bit);
 ch->pcdata->powers[HUMANITY] += humanity;
 stc("SYSTEM ERROR: Enhancement uninstalled, re-routing power.\n\r", ch);
 return;
}

void do_chameleon( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  
  argument = one_argument( argument, arg);
 
  if( IS_NPC(ch))
   return;
  if( !IS_CYBORG(ch))
  {
   stc("Huh?\n\r", ch);
   return;
  }
  if( arg[0] == '\0')
  {
   send_to_char("SYSTEM ERROR: No argument provided.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->pcdata->powers[IMPLANT], CHAM_FIELD))
  {
   stc("SYSTEM ERROR: No chamleon cloak installed.\n\r", ch);
   return;
  }
  if( !str_cmp(arg, "on"))
  {
   if( ch->fight_timer > 0)
   {
    stc("Not with a fight timer.\n\r", ch);
    return;
   }
   if( ch->pcdata->powers[CHAM_PWR] == 0)
   {
    stc("SYSTEM: Chameleon battery has been drained, recharging.\n\r", ch);
    return;
   }
   ch->pcdata->powers[CHAM] = 1;
   stc("SYSTEM: Chameleon cloaking field engaged.\n\r", ch);
   return;
  }
  else if( !str_cmp( arg, "off"))
  {
   ch->pcdata->powers[CHAM] = 0;
   stc("SYSTEM: Powering down chameleon cloaking field.\n\r", ch);
   return;
  }
  else
  {
   stc("On or off?\n\r", ch);
   return;
  }
  return;
}
