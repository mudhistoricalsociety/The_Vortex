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

sh_int combo_type( sh_int value1, sh_int value2);
sh_int embody_combo( CHAR_DATA *ch);
sh_int style_combo( CHAR_DATA *ch);
void combo_move( CHAR_DATA *ch, sh_int type);
void combo_dot( CHAR_DATA *ch, CHAR_DATA *victim);
void combo_damage( CHAR_DATA *ch, CHAR_DATA *victim);
void combo_affect( CHAR_DATA *ch, CHAR_DATA *victim);
void combo_heal( CHAR_DATA *ch, CHAR_DATA *victim);
void combo_stun( CHAR_DATA *ch, CHAR_DATA *victim);

#define DOT_TIME 3
#define MAX_COMBO_DOT 3
#define MAX_COMBO_DAMAGE 4
#define MAX_COMBO_HEAL 3
#define MAX_COMBO_AFFECT 5
#define MAX_COMBO_STUN 3
#define MONK_LAG 6
#define COMBO_LIMIT 45
#define OPT_COM 3
#define OPT_COM2 2
#define OPT_COM3 -3
#define MIN_DAM 400
#define MAX_DAM 900

void do_clairvoyance( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *location;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  argument = one_argument( argument, arg);
  if( IS_NPC(ch))
   return;
  if( (victim = get_char_world( ch, arg)) == NULL)
  {
   send_to_char("They arent here.\n\r", ch);
   return;
  }
  if( ch->level < 3)
  {
   send_to_char("You must be avatar.\n\r", ch);
   return;
  }
  if((location = victim->in_room) == NULL)
  {
   send_to_char("BUG: report to imms.\n\r", ch);
   return;
  }
  sprintf(buf, "%s is in %s.\n\r", victim->name, location->area->name);
  send_to_char(buf, ch);
  if( !IS_NPC(victim))
   ch->fight_timer += 7;
  if( ch->fight_timer > 30)
    ch->fight_timer = 30;
  return;
} 
    
void do_monkgain(CHAR_DATA *ch, char *argument)
{
  int cost;
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  sh_int improve;
  
  argument = one_argument( argument, arg1);
  argument = one_argument( argument, arg2);

  if( IS_NPC(ch))
  return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( arg1[0] == '\0')
  {
   stc("=-=-=-=-=-=-=-=-=-=-=-=-=-=- Monk Powers -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\r", ch);
   stc("Embodies:\n\r", ch);
   sprintf( buf, "Fire: [%d]      Earth: [%d]        Air: [%d]         Water: [%d]\n\r",
            ch->pmonk[EMBODY_FIRE], ch->pmonk[EMBODY_EARTH], ch->pmonk[EMBODY_AIR],
            ch->pmonk[EMBODY_WATER]);
   stc( buf, ch);
   stc("Styles:\n\r", ch);
   sprintf( buf, "Ape: [%d]       Cobra: [%d]        Swan: [%d]        Hawk: [%d]\n\r",
            ch->pmonk[STYLE_APE], ch->pmonk[STYLE_COBRA], ch->pmonk[STYLE_SWAN],
            ch->pmonk[STYLE_HAWK]);
   stc(buf, ch);
   return;
  }
  if( ch->level < 3)
  {
   send_to_char("You must be an avatar.\n\r", ch);
   return;
  }
  if( arg2[0] == '\0')
  return;
  if( !str_cmp( arg2, "improve"))
  { 
     if( !str_cmp( arg1, "fire"))
      improve = EMBODY_FIRE;
     else if( !str_cmp( arg1, "earth"))
      improve = EMBODY_EARTH;
     else if( !str_cmp( arg1, "air"))
      improve = EMBODY_AIR;
     else if( !str_cmp( arg1, "water"))
      improve = EMBODY_WATER;
     else if( !str_cmp( arg1, "ape"))
      improve = STYLE_APE;
     else if( !str_cmp( arg1, "cobra"))
      improve = STYLE_COBRA;
     else if( !str_cmp( arg1, "swan"))
      improve = STYLE_SWAN;
     else if( !str_cmp( arg1, "hawk"))
      improve = STYLE_HAWK;
     else
      improve = -1;
  }
  if( str_cmp( arg2, "improve") && (arg2[0] != '\0'))
  {
    send_to_char("You must say if you want to improve it or not.\n\r", ch);
    return;
  }
  if( improve == -1)
  {
   send_to_char("That is not a valid option.\n\r", ch);
   return;
  }
  if( ch->pmonk[improve] >= 10)
  {
   send_to_char("That skill is already at maximum.\n\r", ch);
   return;
  }
  cost = (ch->pmonk[improve]+1) * 10;
  if( ch->practice < cost)
  {
   send_to_char("You cant afford it.\n\r", ch);
   return;
  }
  ch->practice -= cost;
  ch->pmonk[improve]++;
  send_to_char("Your abilities improve.\n\r", ch);
  return;
}

sh_int embody_combo( CHAR_DATA *ch)
{
 sh_int value = 0;
 int embody = ch->pmonk[CURR_EMBODY];
 
 if( embody == EMBODY_AIR)
 {
   if( number_percent() < 50)
    value = 3;
   else
    value = 2;
 }
 else if( embody == EMBODY_EARTH)
 {
  if( number_percent() < 50)
   value = 5; 
  else
   value = 2;
 }
 else if( embody == EMBODY_WATER)
 {
  if( number_percent() < 50)
   value = 4;
  else
   value = 1;
 }
 else if( embody == EMBODY_FIRE)
 {
  if( number_percent() < 50)
   value = 1;
  else
   value = 5;
 }
 else
  value = 0;
 return value;
}

sh_int style_combo( CHAR_DATA *ch)
{
 sh_int value = 0;
 int style = ch->pmonk[CURR_STYLE];

 if( style == STYLE_SWAN)
 {
  if( number_percent() < 50)
   value = 5;
  else
   value = 4;
 }
 else if( style == STYLE_APE)
 {
  if( number_percent() < 50)
   value = 4;
  else
   value = 3;
 }
 else if( style == STYLE_HAWK)
 {
  if( number_percent() < 50)
   value = 3;
  else
   value = 2;
 }
 else if( style == STYLE_COBRA)
 {
  if( number_percent() < 50)
   value = 1;
  else
   value = 5;
 }
 else
  value = 0;
 return value;
}

sh_int combo_type( sh_int value1, sh_int value2)
{
 if( number_percent() < 50)
  return value1;
 else
  return value2;
}
void combo_move( CHAR_DATA *ch, sh_int value)
{
 CHAR_DATA *victim;
 
 if( ch->fighting == NULL)
  return;
 if( IS_NPC(ch)) /* this should never happen..but just in case */
  return;
 if( !IS_CLASS( ch, CLASS_NEW_MONK))
  return;
 if( value == 0)
  return;
 victim = ch->fighting;
 if( value == 1)
 {
  combo_dot(ch, victim);
  return;
 }
 else if( value == 2)
 {
  combo_damage(ch, victim);
  return;
 }
 else if( value == 3)
 {
  combo_affect(ch, victim);
  return;
 }
 else if( value == 4)
 {
  combo_heal(ch, victim);
  return;
 }
 else if( value == 5)
 {
  combo_stun(ch, victim);
  return;
 }
 else
 return;
}
void combo_dot( CHAR_DATA *ch, CHAR_DATA *victim)
{
 sh_int type = number_range(1,MAX_COMBO_DOT);
 char buf[MAX_STRING_LENGTH];
 
 if(IS_NPC(ch))
  return;
 if( type == 1)
 {
  sprintf( buf, "%s floods your body with a deadly poison.\n\r", ch->name);
  send_to_char( buf, victim);
  sprintf( buf, "You afflict %s with a deadly posion.\n\r", victim->name);
  send_to_char( buf, ch);
  act("$n afflicts $N with a deadly poison.\n\r", ch, NULL, victim, TO_NOTVICT);
  victim->pulse_timer[TIMER_DOT1] = DOT_TIME;
  return;
 }
 else if( type == 2)
 {
  sprintf( buf, "%s touches you and your skin begins to rot.\n\r", ch->name);
  send_to_char( buf, victim);
  sprintf( buf, "You watch as %s's skin begins to rot when you touch them.\n\r", victim->name);
  send_to_char( buf, ch);
  act("$N's skin begins to rot when $n touches them.\n\r", ch, NULL, victim, TO_NOTVICT);
  victim->pulse_timer[TIMER_DOT2] = DOT_TIME;
  return;
 }
 else if( type == 3)
 {
  sprintf( buf, "%s touches you and you feel a wierd vibration in your body.\n\r", ch->name);
  send_to_char( buf, victim);
  sprintf( buf, "You set up a sonic vibration in %s.\n\r", victim->name);
  send_to_char( buf, ch);
  act("$N gasps as $n set up a sonic vibration within them.\n\r", ch, NULL, victim, TO_NOTVICT);
  victim->pulse_timer[TIMER_DOT3] = DOT_TIME;
  return;
 }
 else /* INSERT NEW DOT HERE */
 return;
}

void combo_damage( CHAR_DATA *ch, CHAR_DATA *victim)
{
 sh_int type = number_range(1, MAX_COMBO_DAMAGE);
 char buf[MAX_STRING_LENGTH];
 int damage = 0;
 if( IS_NPC(ch))
 return;
 if( type == 1)
 {
  damage = number_range(2000, 4500);
  do_yell( ch, "HODOKU TSUNE!");
  sprintf(buf, "You hurl a fireball of chi energy at %s. [%d]\n\r", victim->name, damage);
  send_to_char(buf, ch);
  sprintf(buf, "%s hurls a fireball of pure chi at you! [%d]\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "$n hurls a fireball of pure chi energy at $N! [%d]", damage);
  act( buf, ch, NULL, victim, TO_NOTVICT);
  victim->hit -= damage;
  if( victim->hit < -10)
   victim->hit = -10;
  update_pos(victim);
  tail_chain();
  return;
 }
 else if( type == 2)
 {
  damage = number_range( 3000, 5000);
  do_yell( ch, "JUTSUNE IKAKO!");
  sprintf( buf, "You knock %s back with a vicious dragon punch! [%d]\n\r", victim->name, damage);
  send_to_char(buf, ch);
  sprintf( buf, "%s knocks you back with a vicious dragon punch! [%d]\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "$n knocks $N back with a vicious dragon punch! [%d]", damage);
  act( buf, ch, NULL, victim, TO_NOTVICT);
  victim->hit -= damage;
  if( victim->hit < -10)
   victim->hit = -10;
  update_pos(victim);
  tail_chain();
  return;
 }
 else if( type == 3)
 {
  damage = number_range( 4000, 6000);
  do_yell( ch, "KOBUNATE IKISHINE!");
  sprintf( buf, "You let loose a charged fireball onto %s! [%d]\n\r", victim->name, damage);
  send_to_char(buf, ch);
  sprintf( buf, "%s hits your with a massive charged fireball! [%d]\n\r", ch->name, damage);
  send_to_char( buf, victim);
  sprintf( buf, "$n hits $N with a massive charged fireball! [%d]", damage);
  act( buf, ch, NULL, victim, TO_NOTVICT);
  victim->hit -= damage;
  if( victim->hit < -10)
    victim->hit = -10;
  update_pos(victim);
  tail_chain();
  return;
 }
 else if( type == 4)
 {
  damage = number_range( 5000, 6000);
  do_yell( ch, "GOKI KENUGA!");
  sprintf( buf, "You destroy %s with your flaming dragon punch! [%d]\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf( buf, "%s destroys you with their flaming dragon punch! [%d]\n\r", ch->name, damage);
  send_to_char( buf, victim);
  sprintf( buf, "$n destroys $N with their flaming dragon punch! [%d]", damage);
  act( buf, ch, NULL, victim, TO_NOTVICT);
  victim->hit -= damage;
  if( victim->hit < -10)
    victim->hit = -10;
  update_pos(victim);
  tail_chain();
  return;
 }
 return;
}

void combo_heal( CHAR_DATA *ch, CHAR_DATA *victim)
{
  sh_int type = number_range( 1, MAX_COMBO_HEAL);
  int heal = 0;
  
  if( type == 1)
  {
   heal = number_range(2000, 3000);
   send_to_char("You glow with a bright yellow aura.\n\r", ch);
   act("$n glows with a bright yellow aura.\n\r", ch, NULL, NULL, TO_ROOM);
   ch->hit += heal;
   if( ch->hit > ch->max_hit)
    ch->hit = ch->max_hit;
   return;
  }
  else  if( type == 2)
  {
   heal = number_range( 3000, 4500);
   send_to_char("You glow with a bright green aura.\n\r", ch);
   act("$n glows with a bright green aura.\n\r", ch, NULL, NULL, TO_ROOM);
   ch->hit += heal;
   if( ch->hit > ch->max_hit)
    ch->hit = ch->max_hit;
   return;
  }
  else if( type == 3)
  {
   heal = number_range( 4500, 5500);
   send_to_char("You glow with a bright blue aura.\n\r", ch);
   act("$n glows with a bright blue aura.\n\r", ch, NULL, NULL, TO_ROOM);
   ch->hit += heal;
   if( ch->hit > (ch->max_hit+ 5000))
     ch->hit = ch->max_hit+5000;
   return;
  }
  else /* insert new heal combos here */
  return;
}

void combo_affect( CHAR_DATA *ch, CHAR_DATA *victim)
{
  sh_int type = number_range( 1, MAX_COMBO_AFFECT);
 // char buf[MAX_INPUT_LENGTH];
  
  if( type == 1)
  {
   if( IS_SET(ch->monk_affects, MONK_HASTE))
   return;
   else
   {
   send_to_char("Your action begin to move faster than the eye can see.\n\r", ch);
   act("$n begins to move extremely fast.\n\r", ch, NULL, NULL, TO_ROOM);
   SET_BIT(ch->monk_affects, MONK_HASTE);
   ch->pulse_timer[TIMER_MONKHASTE] = 30;
   return;
   }
   return;
  }
  else if( type == 2)
  {
   if( IS_SET(victim->monk_affects, MONK_SLOW))
   return;
   else
   {
   send_to_char("You begin to move slowly.\n\r", victim);
   act("$n begins to move real slow.\n\r", victim, NULL, NULL, TO_ROOM);
   SET_BIT(victim->monk_affects, MONK_SLOW);
   ch->pulse_timer[TIMER_MONKSLOW] = 4;
   return;
   }
   return;
  }
  else if( type == 3)
  {
   /*if( IS_SET(victim->monk_affects, MONK_BLIND))
   return;
   else
   {
   send_to_char("YOU HAVE BEEN STRICKEN BLIND!\n\r", victim);
   SET_BIT(victim->monk_affects, MONK_BLIND);
   ch->pulse_timer[TIMER_MONKBLIND] = 3;
   return;
   }
   return;*/
  }
  else if( (type == 4) || (type == 3))
  {
   if( IS_SET( ch->monk_affects, MONK_DIAMONDSKIN))
   return;
   else
   {
   send_to_char("Your skin turns as hard as a diamond.\n\r", ch);
   act("$n's skin turns as hard as diamond.\n\r", ch, NULL, NULL, TO_ROOM);
   SET_BIT(ch->monk_affects, MONK_DIAMONDSKIN);
   ch->pulse_timer[TIMER_DIAMONDSKIN] = 30;
   return;
   }
   return;
  }
  else if( type == 5)
  {
   if( IS_SET( ch->monk_affects, MONK_IRONSKIN))
   return;
   else
   {
   send_to_char("You skin becomes like iron.\n\r", ch);
   act("$n's skin becomes like iron.\n\r", ch, NULL, NULL, TO_ROOM);
   SET_BIT( ch->monk_affects, MONK_IRONSKIN);
   ch->pulse_timer[TIMER_IRONSKIN] = 30;
   return;
   }
   return;
  }
  else
  return;
}

void combo_stun( CHAR_DATA *ch, CHAR_DATA *victim)
{
  sh_int type = number_range( 1, MAX_COMBO_STUN);
  char buf[MAX_STRING_LENGTH];
  
  if( type == 1)
  {
   sprintf(buf, "#5You stun %s with a chop to the throat.#n\n\r", victim->name);
   send_to_char( buf, ch);
   sprintf(buf, "#5%s stuns you with a chop to the throat.#n\n\r", ch->name);
   send_to_char( buf, victim);
   sprintf( buf, "#5%s stuns %s with a chop to the throar.#n\n\r", ch->name, victim->name);
   act( buf, ch, NULL, victim, TO_NOTVICT);
   victim->position = POS_STUNNED;
   stop_fighting(ch,TRUE);
   stop_fighting(victim, TRUE);
   return;
  }
  else if( type == 2)
  {
   sprintf(buf, "#5You stun %s with a chop to the throat.#n\n\r", victim->name);
   send_to_char( buf, ch);
   sprintf(buf, "#5%s stuns you with a chop to the throat.#n\n\r", ch->name);
   send_to_char( buf, victim);
   sprintf( buf, "#5%s stuns %s with a chop to the throar.#n\n\r", ch->name, victim->name);
   act( buf, ch, NULL, victim, TO_NOTVICT);
   victim->position = POS_STUNNED;
   stop_fighting(ch, TRUE);
   stop_fighting(victim, TRUE);
   return;
  }
  else if( type == 3)
  {
   sprintf(buf, "#5You kick %s in the head, stunning and knocking them back.#n\n\r", victim->name);
   send_to_char( buf, ch);
   sprintf( buf,"#5%s kicks you in the head, stunning you and knocking you back.#n\n\r", ch->name);
   send_to_char( buf, victim);
   act("#5$n stun and knocks $N to the next room with a kick to the head.#n\n\r", ch, NULL, victim, TO_NOTVICT);
   do_flee(victim, "");
   victim->position = POS_STUNNED;
   return;
  }
  else
   return;
}
void do_embody( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  argument = one_argument( argument, arg);

  if( IS_NPC(ch))
  return;
  if( !IS_NEW_MONK(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  /*if( arg[0] != '\0' )
  {
   if( ch->pmonk[CURR_EMBODY] > 0)
   {
    if( ch->position == POS_FIGHTING)
    {
     stc("You can not release your embodiments while fighting.\n\r", ch);
     return;
    }
   }
   else
   {
    stc("You release your elemental furies.\n\r", ch);
    ch->pmonk[CURR_EMBODY] = 0;
    return;
   }
   return;
  }*/
  if( !str_cmp( arg, "air"))
  {
   if( ch->pmonk[EMBODY_AIR] < 1)
   {
    stc("You have yet to learn the art of air.\n\r", ch);
    return;
   }
   else
   {
   ch->pmonk[CURR_EMBODY] = EMBODY_AIR;
   stc("You embody the element of air.\n\r", ch);
   return;
   }
   return;
  }
  if( !str_cmp( arg, "earth"))
  {
   if( ch->pmonk[EMBODY_EARTH] < 1)
   {
    stc("You have yet to learn the art of earth.\n\r", ch);
    return;
   }
   else
   {
    stc("You embody the element of earth.\n\r", ch);
    ch->pmonk[CURR_EMBODY] = EMBODY_EARTH;
    return;
   }
   return;
  }
  if( !str_cmp( arg, "water"))
  {
   if( ch->pmonk[EMBODY_WATER] < 1)
   {
    stc("You have yet to learn the art of water.\n\r", ch);
    return;
   }
   else
   {
    stc("You embody the element of water.\n\r", ch);
    ch->pmonk[CURR_EMBODY] = EMBODY_WATER;
    return;
   }
   return;
  }
  if( !str_cmp(arg, "fire"))
  {
   if( ch->pmonk[EMBODY_FIRE] < 1)
   {
    stc("You have yet to learn the art of fire.\n\r", ch);
    return;
   }
   else
   {
    stc("You embody the element of fire.\n\r", ch);
    ch->pmonk[CURR_EMBODY] = EMBODY_FIRE;
    return;
   }
   return;
  }
  else
  {
   stc("Syntax: <earth/air/fire/water>.\n\r", ch);
   return;
  }
  return;
}

void do_elbow( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_WATER) && (style == STYLE_APE))
  {
    damage *= 1.5;
    combo += OPT_COM; 
  }
  else if( (embody == EMBODY_FIRE) && (style == STYLE_HAWK))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_FIRE) && (style == STYLE_APE))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2You slam your elbow into %s's gut! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s slams their elbow deep into your gut! [%d]#n\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s slams an elbow deep into %s's gut! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  hurt_person(ch, victim, damage);
  WAIT_STATE(ch, MONK_LAG); 
  ch->pmonk[COMBO] += combo;
 // if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_heartpunch( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_AIR) && (style == STYLE_SWAN))
  {
    damage *= 1.5;
    combo += OPT_COM; 
  }
  else if( (embody == EMBODY_WATER) && (style == STYLE_COBRA))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_EARTH) && (style == STYLE_COBRA))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2You punch %s slam in the heart! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s punchs you hard in the chest! [%d]#n\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s punches %s hard in the chest! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  hurt_person(ch, victim, damage);
  WAIT_STATE(ch, MONK_LAG);
  ch->pmonk[COMBO] += combo;
  //if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_kneethrust( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_AIR) && (style == STYLE_HAWK))
  {
    damage *= 1.5;
    combo += OPT_COM; 
  }
  else if( (embody == EMBODY_EARTH) && (style == STYLE_SWAN))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_FIRE) && (style == STYLE_HAWK))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2You slam your knee into %s's groin! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s slams their knee deep into your groin! [%d]#n\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s slams an elbow deep into %s's groin! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  hurt_person(ch, victim, damage);
  WAIT_STATE(ch, MONK_LAG);
  ch->pmonk[COMBO] += combo;
 // if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_groundsweep( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_EARTH) && (style == STYLE_HAWK))
  {
    damage *= 1.5;
    combo += OPT_COM; 
  }
  else if( (embody == EMBODY_AIR) && (style == STYLE_COBRA))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_WATER) && (style == STYLE_APE))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2You knock %s off their feet with a sweep! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s knocks you off your feet with a sweep! [%d]#n\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s knocks %s off their feet with a sweep! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  victim->hit -= damage;
  update_pos(victim);
  WAIT_STATE(ch, MONK_LAG);
  ch->pmonk[COMBO] += combo;
 // if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_thrustkick( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_FIRE) && (style == STYLE_APE))
  {
    damage *= 1.5;
    combo += OPT_COM; 
  }
  else if( (embody == EMBODY_EARTH) && (style == STYLE_APE))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_EARTH) && (style == STYLE_SWAN))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2You kick %s hard in the chest! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s kicks you hard in the chest! [%d]#n\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s kicks %s hard in the chest! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  hurt_person(ch, victim, damage);
  WAIT_STATE(ch, MONK_LAG);
  ch->pmonk[COMBO] += combo;
 // if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_roundhouse( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_FIRE) && (style == STYLE_COBRA))
  {
    damage *= 1.5;
    combo += OPT_COM;
  }
  else if( (embody == EMBODY_FIRE) && (style == STYLE_HAWK))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_AIR) && (style == STYLE_HAWK))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2Your roundhouse catches %s in the head! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s catchs you in the head with a roundhouse kick! [%d]#n\n\r", ch->name,
  damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s sends a roundhouse kick to %s's head! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  hurt_person(ch, victim, damage);
  WAIT_STATE(ch, MONK_LAG);
  ch->pmonk[COMBO] += combo;
  //if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_jumpkick( CHAR_DATA *ch, char *argument)
{
  int damage = number_range( MIN_DAM, MAX_DAM);
  CHAR_DATA *victim;
  int combo = 1;
  sh_int embody;
  sh_int style;
  int mod;
  char buf[MAX_STRING_LENGTH];

  if( IS_NPC(ch))
   return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
    send_to_char("Huh?\n\r", ch);
    return;
  }
  if( ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You are not fighting anyone.\n\r", ch);
   return;
  }
  embody = ch->pmonk[CURR_EMBODY];
  style = ch->pmonk[CURR_STYLE];
  /* optimizers*/
  if( (embody == EMBODY_EARTH) && (style == STYLE_SWAN))
  {
    damage *= 1.5;
    combo += OPT_COM; 
  }
  else if( (embody == EMBODY_WATER) && (style == STYLE_SWAN))
  {    
    damage *= 1.25;
    combo += OPT_COM2;
  }
  else if( (embody == EMBODY_WATER) && (style == STYLE_COBRA))
  {
    damage *= 0.85;
    combo -= OPT_COM3;
  }
  else
  {
   mod = number_range(1, 3);
   if( mod == 1)
   {
    damage *= 0.9;
    combo -= 1;
   }
   /* no code for mod == 2, just leave as is */
   if( mod == 2)
   {
     damage = damage;
     combo = combo;
   }
   if( mod == 3)
   {
     damage *= 1.15;
     combo += 1;
   }
  }
  sprintf(buf, "#2You snap %s's head back with a jumpkick! [%d]#n\n\r", victim->name, damage);
  send_to_char( buf, ch);
  sprintf(buf, "#2%s snaps your head back with a jumpkick! [%d]#n\n\r", ch->name, damage);
  send_to_char(buf, victim);
  sprintf( buf, "#2%s snaps %s's head back with a jumpkick! [%d]#n\n\r", ch->name, victim->name,
  damage);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  hurt_person(ch, victim, damage);
  ch->pmonk[COMBO] += combo;
  WAIT_STATE(ch, MONK_LAG);
 // if( ch->pmonk[COMBO] >= COMBO_LIMIT)
  if( (ch->pmonk[COMBO] * 2) >= number_range(1,100))
  {
    combo_move( ch, combo_type( embody_combo(ch), style_combo(ch)));
    ch->pmonk[COMBO] = 0;
  }
  return;
}

void do_style( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_STRING_LENGTH];
 argument = one_argument(argument, arg);
 
 if( IS_NPC(ch))
 return;
 if( !IS_NEW_MONK(ch))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( ch->pmonk[CURR_STYLE] == 0)
  {
   stc("Which style would you like to use?\n\r", ch);
   return;
  }
  else
  {
   stc("You drop out of your fight style.\n\r", ch);
   ch->pmonk[CURR_STYLE] = 0;
   return;
  }
  return;
 }
 if( !str_cmp( arg, "ape"))
 {
  if( ch->pmonk[STYLE_APE] < 1)
  {
   stc("You have yet to learn the ways of the ape.\n\r", ch);
   return;
  }
  else
  {
   stc("Your motions become as fluid as the ape.\n\r", ch);
   ch->pmonk[CURR_STYLE] = STYLE_APE;
   return;
  }
  return;
 }
 if( !str_cmp( arg, "hawk"))
 {
  if( ch->pmonk[STYLE_HAWK] < 1)
  {
   stc("You have yet to learn the ways of the hawk.\n\r", ch);
   return;
  }
  else
  {
   stc("Your angle your body to the form of a hawk.\n\r", ch);
   ch->pmonk[CURR_STYLE] = STYLE_HAWK;
   return;
  }
  return;
 }
 if( !str_cmp( arg, "swan"))
 {
  if( ch->pmonk[STYLE_SWAN] < 1)
  {
   stc("You have yet to learn the ways of the swan.\n\r", ch);
   return;
  }
  else
  {
   stc("You motions become as graceful as the swan.\n\r", ch);
   ch->pmonk[CURR_STYLE] = STYLE_SWAN;
   return;
  }
  return;
 }
 if( !str_cmp( arg, "cobra"))
 {
  if( ch->pmonk[STYLE_COBRA] < 1)
  {
   stc("You have yet to learn the ways of the cobra.\n\r", ch);
   return;
  }
  else
  {
   stc("You arch your back in the form of the cobra.\n\r", ch);
   ch->pmonk[CURR_STYLE] = STYLE_COBRA;
   return;
  }
  return;
 }
 else
 {
  stc("Syntax: Style <ape/hawk/cobra/swan>.\n\r", ch);
  return;
 }
 return;
}

void do_healtouch( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  int cost = 750;

  if( IS_NPC(ch))
  return;
  if( !IS_CLASS(ch, CLASS_NEW_MONK))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  argument = one_argument(argument, arg);
  if( arg[0] == '\0')
   victim = ch;
  else
  {
   if( (victim = get_char_room(ch, arg)) == NULL)
   {
    send_to_char("They arent here.\n\r", ch);
    return;
   }
   if( IS_NPC(victim))
   {
    send_to_char("Not on NPC's.\n\r", ch);
    return;
   }
  }
  if( ch->mana < cost)
  {
   send_to_char("You cant afford it.\n\r", ch);
   return;
  }
  ch->mana -= cost;
  victim->pulse_timer[TIMER_MONK_HEAL] += 10;
  sprintf(arg, "You heal %s's wounds.\n\r", victim->name);
  send_to_char(arg, ch);
  sprintf( arg, "%s heals your wounds.\n\r", ch->name);
  send_to_char(arg, victim);
  act("$n heals $N's wounds.\n\r", ch, NULL, victim, TO_NOTVICT);
  return;
}
  
