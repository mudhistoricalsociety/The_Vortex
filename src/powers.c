/* This module is to supply the source code for all the powerblocks
   and the needed utility functions to aid in installation and
   processing of said code.  If any more powerblocks get added, 
   add a define in player.h, then make the appropriate entries 
   into has_power(),find_field(), and has_power_level(), is_opposite_school(),
   max_school_power, show_power_command(), power_name().  -- Kale
*/

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

char* power_name(int type);
int max_school_power(int type);
char* show_power_command( int type, int level);
bool is_opposite_school( int type1, int type2);
bool race_ok( CHAR_DATA *ch, int value);
bool race_sub( CHAR_DATA *ch, int value, int value2);

bool race_ok( CHAR_DATA *ch, int value)
{
 int val1;
 int val2;

 val1 = ch->pcdata->race->group1;
 val2 = ch->pcdata->race->group2;

 if( val1 == 0 && val2 == 0)
  return TRUE; /* this should never happen but just in case*/
 if( race_sub(ch, val1,value) && race_sub(ch, val2,value))
  return TRUE;
 else
  return FALSE;
}

bool race_sub( CHAR_DATA *ch, int value,int value2)
{
 bool ret_val = TRUE;
 if( value == 1)
 {
  if( (value2 == SCHOOL_FIRE) || (value2 == SCHOOL_CHAOS))
   ret_val = FALSE;
 }
 if( value == 2)
 {
  if( (value2 == SCHOOL_EARTH) || (value2 == SCHOOL_PLANT))
   ret_val = FALSE;
 }
 if( value == 3)
 {
  if( (value2 == SCHOOL_HOLY) || (value2 == SCHOOL_ORDER))
   ret_val = FALSE;
 }
 if( value == 4)
 {
  if( (value2 == SCHOOL_TIME) || (value2 == SCHOOL_LUCK))
   ret_val = FALSE;
 }
 if( value == 5)
 {
  if( (value2 == SCHOOL_ARCANE) || (value2 == SCHOOL_SPACE))
   ret_val = FALSE;
 }
 if( value == 6)
 {
  if( (value2 == SCHOOL_LIFE) || (value2 == SCHOOL_WATER))
   ret_val = FALSE;
 }
 if( value == 7)
 {
  if( (value2 == SCHOOL_DEATH) || (value2 == SCHOOL_WIND))
   ret_val = FALSE;
 }
 if( value == 8)
 {
  if( (value2 == SCHOOL_ANIMAL) || (value2 == SCHOOL_EVIL))
   ret_val = FALSE;
 }
 return ret_val;
}

int random_block(CHAR_DATA *ch)
{
 int value = number_range(1, MAX_BLOCK);

 if( IS_NPC(ch))
  return 0;
 if( has_power(ch, value)
  || !race_ok(ch, value)
  || is_opposite_school(ch->ABlock[POWER_TYPE], value)
  || is_opposite_school(ch->BBlock[POWER_TYPE], value) )
  random_block(ch);
 return value;
}

void do_detox( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch))
  return;
 if( IS_CLASS(ch, CLASS_NONE) )
 {
  stc("You don't even have a class!\n\r", ch);
  return;
 }
 if( ch->in_room->vnum != 3206)
 {
  stc("You must be in the detox chamber to do this.\n\r", ch);
  return;
 }
 if( ch->max_hit < 10000 || ch->max_mana < 10000 
    || ch->max_move < 10000)
 {
  stc("You must have at least 10000 in hp mana and move.\n\r", ch);
  return;
 }
 if( IS_DAMNED(ch) ) return;

 do_clearrace(ch, "self");
 ch->max_mana -= 5000;
 ch->max_hit -= 5000;
 ch->max_move -= 5000;
 ch->level = 2;
 ch->trust = 2;
 stc("Your class has been cleared.\n\r", ch);
 return;
}

bool standard_calls( CHAR_DATA *ch, int power, int level)
{
 if( IS_NPC(ch))
  return FALSE;
 if( ch->trust >= 7)
  return TRUE;
 if( !has_power(ch, power))
 {
//  do_huh(ch, "");
  stc("You cant do that.\n\r", ch);
  return FALSE;
 }
 if( !has_power_level(ch, power, level))
 {
  stc("You have yet to learn that skill.\n\r", ch);
  return FALSE;
 }
 else 
  return TRUE;
}

bool has_power(CHAR_DATA *ch, int power)
{
  bool ret_val = FALSE;

  if( IS_IMMORTAL(ch))
   ret_val = TRUE;
  if( (ch->ABlock[POWER_TYPE] == power) || (ch->BBlock[POWER_TYPE] == power)
   || (ch->CBlock[POWER_TYPE] == power))
   ret_val = TRUE;
  else 
   ret_val = FALSE;
 return ret_val;
}

int find_field(CHAR_DATA *ch, int power)
{
  if( !has_power(ch, power))
   return 0;
  else if( ch->ABlock[POWER_TYPE] == power)
   return 1;
  else if( ch->BBlock[POWER_TYPE] == power)
   return 2;
  else if( ch->CBlock[POWER_TYPE] == power)
   return 3;
  else
   return 0;
}

bool has_power_level(CHAR_DATA *ch, int power, int level)
{
 int field = find_field(ch,power);

 if( ch->trust  > 6)
  return TRUE;
 else if( field == 0)
  return FALSE;
 else if( field == 1)
 {
  if( ch->ABlock[POWER_LEVEL] >= level)
   return TRUE;
  else
   return FALSE;
 }
 else if( field == 2)
 {
  if( ch->BBlock[POWER_LEVEL] >= level)
   return TRUE;
  else
   return FALSE;
 }
 else if( field == 3)
 {
  if( ch->CBlock[POWER_LEVEL] >= level)
   return TRUE;
  else
   return FALSE;
 }
 else
  return FALSE;
}

void do_powers( CHAR_DATA *ch, char *argument)
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 int i,j;
 sh_int power[2];
 int cost;

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if( IS_NPC(ch))
  return;
 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  do_wolfpowers(ch,"");
  return;
 }
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_vamppowers(ch,"");
  return;
 }
 if( !IS_CLASS(ch,CLASS_APOCRYPHIC))
  return;
 if( arg1[0] == '\0')
 {
  stc("Syntax is: power <show/gain> (school1,school2,school3).\n\r", ch);
  return;
 }
 if( !str_cmp(arg1, "show"))
 {
  if( ch->ABlock[POWER_TYPE] == 0)
  {
   stc("You have no powers.\n\r", ch);
   return;
  }
  for( i = 1; i != 4; i++)
  {
   if( i == 1)
   {
    power[POWER_TYPE] = ch->ABlock[POWER_TYPE];
    power[POWER_LEVEL] = ch->ABlock[POWER_LEVEL];
   }
   else if( i == 2)
   {
    power[POWER_TYPE] = ch->BBlock[POWER_TYPE];
    power[POWER_LEVEL] = ch->BBlock[POWER_LEVEL];
   }
   else if( i == 3)
   {
    power[POWER_TYPE] = ch->CBlock[POWER_TYPE];
    power[POWER_LEVEL] = ch->CBlock[POWER_LEVEL];
   }
   if( power[POWER_TYPE] == 0)
    return;
   sprintf( buf, power_name(power[POWER_TYPE]));
   banner_to_char(buf, ch);
  for( j = 1; j != (max_school_power(power[POWER_TYPE])+1); j ++)
   {
    if( power[POWER_LEVEL] < j)
     return;
    else  
     sprintf( buf, "%s\n\r",show_power_command(power[POWER_TYPE], j));
    stc(buf, ch);
   }
  }
  return;
 }
 else if( !str_cmp( arg1, "gain"))
 {
   if( arg2[0] =='\0')
   {
    stc("Gain powers in which school?\n\r", ch);
    return;
   }
   if( !str_cmp(arg2, "school1"))
   {
    if( ch->ABlock[POWER_TYPE] == 0)
    {
     stc("You have yet to learn a school.\n\r", ch);
     return;
    }
    if( ch->ABlock[POWER_LEVEL] == max_school_power(ch->ABlock[POWER_TYPE]))
    {
     stc("You have learned all you can learn in this school.\n\r", ch);
     return;
    }
    cost = ( (ch->ABlock[POWER_LEVEL] + 1)* 20);
    if( ch->practice < cost)
    {
     sprintf(arg1, "You need at least %d primal for this.\n\r", cost);
     stc(arg1, ch);
     return;
    }
    ch->practice -= cost;
    ch->ABlock[POWER_LEVEL]++;
    do_save(ch, "");
    stc("Your powers have risen.\n\r", ch);
    do_powers(ch, "show");
    return;
   }
   else if( !str_cmp( arg2, "school2"))
   {
    if( ch->BBlock[POWER_TYPE] == 0)
    {
     stc("You have yet to learn a school.\n\r", ch);
     return;
    }
    if( ch->BBlock[POWER_LEVEL] == max_school_power(ch->BBlock[POWER_TYPE]))
    {
     stc("You have learned all you can learn in this school.\n\r", ch);
     return;
    }
    cost = ( (ch->BBlock[POWER_LEVEL] +1) * 20);
    if( ch->practice < cost)
    {
     sprintf(arg1, "You need at least %d primal for this.\n\r", cost);
     stc(arg1, ch);
     return;
    }
    ch->practice -= cost;
    ch->BBlock[POWER_LEVEL]++;
    do_save(ch, "");
    stc("Your powers have risen.\n\r", ch);
    do_powers(ch, "show");
    return;
   }
   else if( !str_cmp( arg2, "school3"))
   {
    if( ch->CBlock[POWER_TYPE] == 0)
    {
     stc("You have yet to learn a school.\n\r", ch);
     return;
    }
    if( ch->CBlock[POWER_LEVEL] == max_school_power(ch->CBlock[POWER_TYPE]))
    {
     stc("You have learned all you can learn in this school.\n\r", ch);
     return;
    }
    cost = ( (ch->CBlock[POWER_LEVEL] + 1) * 20);
    if( ch->practice < cost)
    {
     sprintf(arg1, "You need at least %d primal for this.\n\r", cost);
     stc(arg1, ch);
     return;
    }
    ch->practice -= cost;
    ch->CBlock[POWER_LEVEL]++;
    do_save(ch, "");
    stc("Your powers hae risen.\n\r", ch);
    do_powers(ch, "show");
    return;
   }
   else
   {
    stc("Syntax: power gain <school1, school2, or school3>.\n\r", ch);
    return;
   }
 }
 else
  return;
}

void do_gainschool( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 int school1 = 0;
 int school2 = 0;
 int school3 = 0;
 int value;
 bool allow = FALSE;
 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if(!IS_CLASS(ch, CLASS_APOCRYPHIC))
  return;
 school1 = ch->ABlock[POWER_TYPE];
 school2 = ch->BBlock[POWER_TYPE];
 school3 = ch->CBlock[POWER_TYPE];
 if( school1 == 0 )
   allow = TRUE;
/* if( !allow && (ch->BBlock[POWER_TYPE] == 0) && ch->race < 5)
 {
  stc("You need to have at least 5 status to gain a secondary powerblock.\n\r", ch);
  return;
 }
 allow = TRUE;
 if( !allow && (ch->CBlock[POWER_TYPE] == 0) && ch->race < 10)
 {
  stc("You need to have at least 10 status to gain a third powerblock.\n\r", ch);
  return;
 }
 if( ch->BBlock[POWER_TYPE] != 0)
  school2 = ch->BBlock[POWER_TYPE];
*/ if( ch->CBlock[POWER_TYPE] != 0)
 {
  stc("You have already gained all your schools.\n\r", ch);
  return;
 }
/*
if( school1 != 0 && school2 != 0 && school3 == 0 )
{
 value = random_block(ch);
 ch->CBlock[POWER_TYPE] = value;
 stc("The powers of eternity bestow upon you a final school!\n\r", ch );
 return;
}
*/
if( arg[0] == '\0' && ( school1 == 0 || school2 == 0 || school3 == 0) )
 {
  stc("Gain which school?\n\r", ch);
  return;
 }
 if( !str_cmp(arg, "fire"))
  value = SCHOOL_FIRE;
 else if(!str_cmp(arg, "water"))
  value = SCHOOL_WATER;
 else if(!str_cmp(arg, "earth"))
  value = SCHOOL_EARTH;
 else if( !str_cmp(arg, "wind"))
  value = SCHOOL_WIND;
 else if( !str_cmp( arg, "holy"))
  value = SCHOOL_HOLY;
 else if( !str_cmp(arg, "evil"))
  value = SCHOOL_EVIL;
 else if( !str_cmp(arg, "plant"))
  value = SCHOOL_PLANT;
 else if( !str_cmp( arg, "animal"))
  value = SCHOOL_ANIMAL;
 else if( !str_cmp( arg, "space"))
  value = SCHOOL_SPACE;
 else if( !str_cmp( arg, "time"))
  value = SCHOOL_TIME;
 else if( !str_cmp( arg, "arcane"))
  value = SCHOOL_ARCANE;
 else if( !str_cmp( arg, "luck"))
  value = SCHOOL_LUCK;
 else if( !str_cmp( arg, "order"))
  value = SCHOOL_ORDER;
 else if( !str_cmp( arg, "chaos"))
  value = SCHOOL_CHAOS;
 else if( !str_cmp( arg, "life"))
  value = SCHOOL_LIFE;
 else if( !str_cmp( arg, "death"))
  value = SCHOOL_DEATH;
 else
 {
  stc("Not a valid option.\n\r", ch);
  return;
 }
 if( school1 == 0)
 {
  if( is_opposite_school( school1, value) || is_opposite_school( school2, value) || is_opposite_school( school3, value) )
  {
   stc("You cant have oposing schools.\n\r", ch);
   return;
  }
  else if( !race_ok(ch, value))
  {
   stc("Your race is barred from that power.\n\r", ch);
   return;
   }
  else if( ch->ABlock[POWER_TYPE] == value || ch->BBlock[POWER_TYPE] == value || ch->CBlock[POWER_TYPE] == value )
  {
   stc("You already have that school.\n\r", ch);
   return;
  }  
  else
  {
   stc("New school chosen.\n\r", ch);
   ch->ABlock[POWER_TYPE] = value;
   return;
  }
  return;
 }
 if( school2 == 0)
 {
  if( is_opposite_school( school1, value) || is_opposite_school( school2, value) || is_opposite_school( school3, value) )
  {
   stc("You cant have oposing schools.\n\r", ch);
   return;
  }
  else if( !race_ok(ch, value))
  {
   stc("Your race is barred from that power.\n\r", ch);
   return;
   }
  else
  {
   stc("New school chosen.\n\r", ch);
   ch->BBlock[POWER_TYPE] = value;
   return;
  }
  return;
 }
 if( school3 == 0)
 {
  if( is_opposite_school( school1, value) || is_opposite_school( school2, value) || is_opposite_school( school3, value) )
  {
   stc("You cant have oposing schools.\n\r", ch);
   return;
  }
  else if( !race_ok(ch, value))
  { 
   stc("Your race is barred from that power.\n\r", ch);
   return;
   }
  else
  {
   stc("New school chosen.\n\r", ch);
   ch->CBlock[POWER_TYPE] = value;
   return;
  }
  return; 
 }  
 return;
}

bool is_opposite_school(  int type1, int type2)
{
 if( ((type1 == SCHOOL_FIRE) && (type2 == SCHOOL_WATER))
  || ((type1 == SCHOOL_WATER) && (type2 == SCHOOL_FIRE)))
   return TRUE;
 else if( ((type1 == SCHOOL_PLANT) && (type2 == SCHOOL_ANIMAL))
  || ((type1 == SCHOOL_ANIMAL) && (type2 == SCHOOL_PLANT)))
   return TRUE;
 else if( ((type1 == SCHOOL_EARTH) && (type2 == SCHOOL_WIND))
  || ((type1 == SCHOOL_WIND) && (type2 == SCHOOL_EARTH)))
   return TRUE;
 else if( ((type1 == SCHOOL_ARCANE) && (type2 == SCHOOL_LUCK))
  || ((type1 == SCHOOL_LUCK) && (type2 == SCHOOL_ARCANE)))
   return TRUE;
 else if( ((type1 == SCHOOL_TIME) && (type2 == SCHOOL_SPACE))
  || ((type1 == SCHOOL_SPACE) && (type2 == SCHOOL_TIME)))
   return TRUE;
 else if( ((type1 == SCHOOL_ORDER) && (type2 == SCHOOL_CHAOS))                                                                                                                                                                                                                                                                                                                                                                                                                                          || ((type1 == SCHOOL_CHAOS) && (type2 == SCHOOL_ORDER)))
   return TRUE;
 else if( ((type1 == SCHOOL_LIFE) && (type2 == SCHOOL_DEATH))
  || ((type1 == SCHOOL_DEATH) && (type2 == SCHOOL_LIFE)))
   return TRUE;
 else if( ((type1 == SCHOOL_HOLY) && (type2 == SCHOOL_EVIL))
  || ((type1 == SCHOOL_EVIL) && (type2 == SCHOOL_HOLY)))
   return TRUE;
 else
   return FALSE;
}

char* show_power_command( int type, int level)
{
   char *name = "";
   static char* const fire[7] = {"None", "Firebolt", "Hitroll", "Damroll", "FireRain", "Firetrap",
"Fireshield"};
   static char* const water[7] = {"None", "Cleanse", "Flood", "Damroll", "Ice","IceShield", "Dodge"};
   static char* const earth[7] = {"None", "Damroll", "Damage mod", "Damage mod", "AcidShield", "Parry",
"Stasis"};
   static char* const wind[9] = {"None", "Dodge", "Parry", "Hitroll", "Gust", "Whirlwind","Dodge",
"Parry","Hitroll"};
   static char* const holy[7] = {"None", "Heal", "Banish", "Godsbless", "Summon: Gabriel", "Fanaticism",
"Summon: Michael"};
   static char* const evil[8] = {"None", "Curse", "Defile", "Frighten", "Shadowplane", "Damroll",
  "Summon: Mephisto", "Summon: Baal"};
   static char* const time[8] = {"None", "Extra Attack", "Damage mod", "Slow", "Extra attack",
   "Temporal Stasis", "Extra Attack", "Haste"};
   static char* const space[7] = {"None", "Dimension shift", "Gate", "Travel", "BodySlide",
"Ac","Hitroll"};
   static char* const plant[7] = {"None", "Barkskin", "Entangle", "Root Travel", "Poison", "Shield of Thorns", "Thorn Blast"};
   static char* const animal[7] = {"None", "Rage", "Burrow", "Hitroll","Damroll", "Claw", "Hitroll"};
   static char* const arcane[8] = {"None", "Magic levels", "Enhanced Scry", "Mana Shield","Enhanced Casting", "Amight",
"Parasite", "Scry Detection"};
   static char* const luck[7] = {"None", "Dodge", "Parry", "Damage Mod", "Damroll", "Hitroll",
"Counter-Attack"};
   static char* const order[7] = {"None", "Calm", "Hold Person", "Balance", "Dodge", "Parry", "Hitroll"};
   static char* const chaos[7] = {"None", "Shatter", "Rage", "Damroll", "Chaos Shield", "Damage mod",
"Strike"};
   static char* const life[8] = {"None", "Life Essence", "Givelife", "Ac", "Dodge",  "Aura", "Restore",
"Regenerate"};
   static char* const death[7] = {"None", "Syphon", "Contagion", 
"Damroll","Ac", "Damage Reducer", "Sense Aura"};

  switch(type)
  {
   case SCHOOL_FIRE:
    name = fire[level];
    break;
   case SCHOOL_WATER:
    name = water[level];
    break;
   case SCHOOL_EARTH:
    name = earth[level];
    break;
   case SCHOOL_WIND:
    name = wind[level];
    break;
   case SCHOOL_HOLY:
    name = holy[level];
    break;
   case SCHOOL_EVIL:
    name =  evil[level];
    break;
   case SCHOOL_TIME:
    name = time[level];
    break;
   case SCHOOL_SPACE:
    name = space[level];
    break;
   case SCHOOL_PLANT:
    name = plant[level];
    break;
   case SCHOOL_ANIMAL:
    name = animal[level];
    break;
   case SCHOOL_ARCANE:
    name = arcane[level];
    break;
   case SCHOOL_LUCK:
    name = luck[level];
    break;
   case SCHOOL_ORDER:
    name = order[level];
    break;
   case SCHOOL_CHAOS:
    name = chaos[level];
    break;
   case SCHOOL_LIFE:
    name = life[level];
    break;
   case SCHOOL_DEATH:
    name = death[level];
    break;
   default:
    break;
  }
  return name;
}

char* power_name(int type)
{
  static char* const names[17] = {"None", "Fire", "Water", "Earth", "Wind", "Holy", "Evil",
                   "Time", "Space", "Plant", "Animal", "Arcane", "Luck", "Order",
                   "Chaos", "Life", "Death"};
  return names[type];
}
  
int max_school_power(int type)
{
 switch(type)
 {
  case SCHOOL_FIRE:
  case SCHOOL_WATER:
  case SCHOOL_EARTH:
  case SCHOOL_HOLY:
  case SCHOOL_SPACE:
  case SCHOOL_ANIMAL:
  case SCHOOL_LUCK:
  case SCHOOL_ORDER:
  case SCHOOL_CHAOS:
  case SCHOOL_PLANT:
  case SCHOOL_DEATH:
   return 6;
   break;
  case SCHOOL_WIND:
   return 8;
   break;
  case SCHOOL_ARCANE:
  case SCHOOL_EVIL:
  case SCHOOL_TIME:
  case SCHOOL_LIFE:
   return 7;
   break;
  default:
   return 8;
   break;
 }
}

/* Power block of Fire */
void do_firebolt( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam;
 int cost = 1500;
 char arg[MAX_INPUT_LENGTH];
 argument = one_argument( argument, arg);
 
 if( !standard_calls(ch, SCHOOL_FIRE, 1))
  return;
 if( ch->fighting == NULL)
 {
   stc("You must be fighting.\n\r", ch);
   return;
 }
 if( ch->mana < cost)
 {
  stc("You cant afford it.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 if( !victim)
  return;  
 dam = number_range(1500, 2500);
 if( has_power(victim, SCHOOL_WATER))
  dam = dam/2;
 if( has_power(victim, SCHOOL_FIRE))
  dam = dam/1.5;
 act2("#1You sear the flesh of $N with a bolt of flame.#n[#7$D#n]", ch, NULL,victim,TO_CHAR, dam);
 act2("#1$n sears your flesh with a bolt of flame.#n[#7$D#n]\n\r", ch, NULL, victim, TO_VICT,dam);
 act2("#1$n sears $N's flesh with a bolt of flame.#n[#7$D#n]\n\r", ch, NULL, victim, TO_NOTVICT,dam);
 ch->mana -= cost;
 hurt_person(ch, victim, dam);
 if( (victim->fighting == NULL) && (victim != ch))
  set_fighting(ch, victim);
 WAIT_STATE(ch, 12);
 return;
}

void do_rainfire( CHAR_DATA *ch, char *argument)
{
 int damage;
 DESCRIPTOR_DATA *d;
 int cost = 1500;

 if( !standard_calls(ch, SCHOOL_FIRE, 4))
  return;
 if( ch->mana < cost)
 {
  stc("You can't afford the cost.\n\r", ch);
  return;
 }
 for( d = descriptor_list; d != NULL; d = d->next)
 {
  if( d->character == NULL)
   continue;
  if( d->connected != CON_PLAYING)
   continue;
  if( d->character->level < 3)
   continue;
  if( d->character->in_room->area != ch->in_room->area)
   continue;
  if( d->character->level > 6)
   continue;
  if( d->character == ch)
   continue;
  if( is_safe(ch, d->character))
   continue;
  damage = number_range(1500, 2500);
  if( has_power(d->character, SCHOOL_FIRE))
   damage = damage / 1.5;
  if( has_power(d->character, SCHOOL_WATER))
   damage = damage / 2;
  hurt_person(ch, d->character, damage);
  act2("The sky turns a bright shade of red and begins to rain fire![$D]", ch, NULL, d->character,TO_VICT, damage);
 }
 ch->mana -= cost;
 stc("You raise your arms to the sky and fire rains from the heavens.\n\r", ch);
 act("$n raises their arms to the sky and fire rains from the heavens!", ch, NULL, NULL, TO_NOTVICT);
 ch->fight_timer += 10;
 if( ch->fight_timer > 30)
  ch->fight_timer = 30;
 WAIT_STATE(ch, 15);
  return;
}

void do_firetrap(CHAR_DATA *ch, char *argument)
{
 int cost = 4000;
 int sn;
 AFFECT_DATA af;
 
 if( !standard_calls(ch, SCHOOL_FIRE, 5))
  return;
 if( ch->mana < cost)
 {
  stc("You dont have the strength to set a firetrap.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("firetrap")) < 0)
  return;
 ch->mana -= cost;
 af.type = sn;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.duration = number_range(4,10);
 af.bitvector = AFF_ROOM_FIRETRAP;
 affect_to_room(ch->in_room, &af);
 act("An explosive trap has been set.", ch, NULL, NULL, TO_ALL);
 WAIT_STATE(ch, 10);
 return;
} 
/* end fire commands (the rest are auto)*/

/* BEGIN WATER POWERS*/

void do_cleanse(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int damage = number_range(500,900);
  int cost = 100;
  char arg[MAX_INPUT_LENGTH];

  argument = one_argument(argument, arg);
  
  if( !standard_calls(ch, SCHOOL_WATER, 1))
   return;
  if( arg[0] == '\0')
  {
   stc("Cleanse whose wounds?\n\r", ch);
   return;
  }
  if( ch->mana < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("They are not present.\n\r", ch);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    return;
  }

  if( !can_heal(ch, victim) ) return;

  victim->hit += damage;
  ch->mana -= cost;
  if( victim->hit > victim->max_hit)
   victim->hit = victim->max_hit;
  update_pos(victim);
  act2("You have cleansed $N's wounds. [$D]", ch, NULL, victim, TO_CHAR, damage);
  act2("$n has cleansed your wounds.[$D]", ch, NULL, victim, TO_VICT, damage);
  act2("$n has cleansed $N of $S wounds. [$D]", ch, NULL, victim, TO_NOTVICT, damage);
  WAIT_STATE(ch, 12);
  return;
}

void do_flood(CHAR_DATA *ch, char *argument)
{
  int cost = 3000;
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;
  AFFECT_DATA af;
  int sn;

  if( !standard_calls(ch, SCHOOL_WATER, 2))
   return;
  if( ch->mana < cost)
  {
   stc("You lack the mana for this.\n\r", ch);
   return;
  }
  if( ch->in_room->area == NULL)
  {
   sprintf("%s is in a NULL area.\n\r", ch->name);
   bug(buf,0);
  }
  for( d = descriptor_list; d != NULL; d = d->next)
  {

    if( d->connected != CON_PLAYING) 
     continue;
    if(d->character == NULL)
     continue;
    if( d->character->in_room->area != ch->in_room->area)
     continue;
   stc("The water level rises and floods the area.\n\r", d->character);
  }
  ch->mana -= cost;
  stc("You raise your arms chanting, and water fills the area.\n\r", ch);
  if( (sn = skill_lookup( "flood" )) < 0 )
   return;
  af.type = sn;
  af.bitvector = AFF_AREA_FLOOD;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = number_range(5,10);
  affect_to_area(ch->in_room->area, &af);
  WAIT_STATE(ch, 12);
  return;
}

void do_ice(CHAR_DATA *ch, char *argument)
{
 int cost = 2000;
 int sn;
 AFFECT_DATA af;
 
 if( !standard_calls(ch, SCHOOL_WATER, 4))
  return;
 if( ch->mana < cost)
 {
  stc("You haven't the strength to ice the floor.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("iced")) < 0)
  return;
 ch->mana -= cost;
 af.type = sn;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.duration = number_range(4,10);
 af.bitvector = AFF_ROOM_ICED;
 affect_to_room(ch->in_room, &af);
 act("You have covered the floor in ice.", ch, NULL, NULL, TO_CHAR);
 act("The room has been covered in ice.", ch, NULL, NULL, TO_ROOM);
 WAIT_STATE(ch, 10);
 return;
} 
/* END WATER POWERS, (REMAINDER ARE AUTOMATIC)*/

/*BEGIN EARTH POWERS*/
void do_stasis(CHAR_DATA *ch, char *argument)
{
 if( !standard_calls(ch, SCHOOL_EARTH,6))
  return;
 if( ch->pcdata->pulse_timer[PULSE_STASIS] < 1 )
 {
  stc("You are too weak to enter stasis.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 0)
 {
  stc("Not with an active fight timer.\n\r", ch);
  return;
 }
 WAIT_STATE(ch, 10);
 if( ch->stasis)
 {
  stc("You return from stasis.\n\r", ch);
  act("$n returns from stasis.",ch, NULL, NULL, TO_NOTVICT);
  ch->stasis = FALSE;
  return;
 }
 if( !ch->stasis)
 {
  stc("You bring yourself into stasis.\n\r", ch);
  act("$n goes into stasis.", ch, NULL, NULL, TO_NOTVICT);
  ch->stasis = TRUE;
  return;
 }
}

/* END EARTH POWERS */

/* BEGIN WIND POWERS */
void do_gust(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA       *victim;
    ROOM_INDEX_DATA *to_room = NULL;
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
  
    if( !standard_calls(ch, SCHOOL_WIND, 4))
     return;
    if (!TIME_UP(ch, TIMER_CAN_GUST))
    {
        send_to_char("You are out of breath.\n\r", ch);
        return;
    }   
    if ( arg1[0] == '\0' )
    {
        send_to_char("Who do you wish to blow a gust of wind at?\n\r", ch);
        return;
    }
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }   
    if ( victim == ch )
    {
        send_to_char( "Not on yourself!\n\r", ch );
        return;
    }
    
    if ( is_safe( ch, victim ) )
      return;    
    WAIT_STATE( ch, 12 );

    SET_TIMER(ch, TIMER_CAN_GUST, dice(1,3)+1);
    act("You breathe a gust of cold air at $N.", ch, NULL, victim, TO_CHAR);
    act("$n breathes a gust of cold air at $N.", ch, NULL, victim, TO_NOTVICT);
    act("$n breathes a gust of cold air at you.", ch, NULL, victim, TO_VICT);
        
    rev_dir = 0;
    if ( arg2[0] == '\0' )
        door = number_range(0,3);
    else
    {
        if      ( !str_prefix( arg2, "north" ) )   
            door = 0;
        else if ( !str_prefix( arg2, "east" ) )
            door = 1;
        else if ( !str_prefix( arg2, "south" ) )
            door = 2;
        else if ( !str_prefix( arg2, "west" ) )
            door = 3;
        else
        {
            send_to_char("You can only blow people north, south, east or west.\n\r", ch);
            return;
        }
    }
    
    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}
     
    if (( pexit = ch->in_room->exit[door]) == NULL)
    {
        sprintf(buf,"$N flies into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$N flies into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"You smash into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        dam = dice(30,60);
          damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
        return;
    }
        
    pexit = victim->in_room->exit[door];
    if (IS_SET(pexit->exit_info, EX_CLOSED) )   
    {
        if (IS_SET(pexit->exit_info, EX_LOCKED))
            REMOVE_BIT(pexit->exit_info, EX_LOCKED);
        if (IS_SET(pexit->exit_info, EX_CLOSED))
            REMOVE_BIT(pexit->exit_info, EX_CLOSED);
        sprintf(buf,"$N is blown into the air and flies %s.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$N is blown into the air and flies %s.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"You are blown into the air and fly %s.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        sprintf(buf,"There is a loud crash as $n smashes through the $d.");
        act(buf,victim,NULL,pexit->keyword,TO_ROOM);
    
        if ( ( to_room   = pexit->to_room ) != NULL 
        && ( pexit_rev = to_room->exit[rev_dir] ) != NULL
        &&   pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL )
        {
            if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
                REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
            if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
                REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
            if (door == 0) sprintf(direction,"south");
            if (door == 1) sprintf(direction,"west");
            if (door == 2) sprintf(direction,"north");
            if (door == 3) sprintf(direction,"east");
            char_from_room(victim);
            char_to_room(victim,to_room);
            sprintf(buf,"$n comes smashing in through the %s $d.", direction);
            act(buf,victim,NULL,pexit->keyword,TO_ROOM);
            dam = dice(30,50);
            damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
        }
    }
    else
    {
        sprintf(buf,"$N flies %s.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"$N flies %s.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"You fly %s.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        if (door == 0) sprintf(direction,"south");
        if (door == 1) sprintf(direction,"west");   
        if (door == 2) sprintf(direction,"north");
        if (door == 3) sprintf(direction,"east");
        char_from_room(victim);
        char_to_room(victim,pexit->to_room);
        sprintf(buf,"$n comes flying in from the %s.", direction);
        act(buf,victim,NULL,NULL,TO_ROOM);
        dam = dice(30, 50);
        damage_old(ch, victim, dam, 0, DAM_OTHER, FALSE);
    }
    return;
}

void do_whirlwind(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA af;
  int cost = 500;
  int sn;

  if( !standard_calls(ch, SCHOOL_WIND, 5))
   return;
  if( ch->mana < cost)
  {
   stc("You aren't strong enough.\n\r", ch);
   return;
  }
  ch->mana -= cost;
 if( (sn = skill_lookup( "whirlwind" )) < 0 )
   return;
 af.type = sn;
 af.bitvector = AFF_ROOM_WHIRLWIND;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.duration = number_range(1, 2);
 affect_to_room(ch->in_room, &af);
 stc("You send a massive whirlwind to surround the room.\n\r", ch);
 act("$n summons a massive whirlwind to surround the room.", ch, NULL, NULL, TO_ROOM);
 WAIT_STATE(ch, 12);
 return;
}
  
void do_fly(CHAR_DATA *ch, char *argument)
{
  if( IS_CLASS(ch, CLASS_DRAGON))
  {
   do_dragon_fly(ch, argument);
   return;
  }
  if( !standard_calls(ch, SCHOOL_WIND, 5))
   return;
  if( IS_AFFECTED( ch, AFF_FLYING))
  {
   REMOVE_BIT( ch->affected_by, AFF_FLYING);
   send_to_char("The wind dies down and you land on the ground.\n\r", ch);
   return;
  }
  if( !IS_AFFECTED(ch, AFF_FLYING))
  {  
   SET_BIT( ch->affected_by, AFF_FLYING);
   send_to_char("The winds picks up and holds you aloft.\n\r", ch);
   return;
  }
  return; 
}

/* END WIND POWERS */
 /* BEGIN HOLY POWERS */
void do_heal( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int heal = number_range(500,900);
  int cost = number_range( 200, 350);
  char arg[MAX_STRING_LENGTH];
  argument = one_argument(argument, arg);

  if( IS_CLASS(ch,CLASS_DROW) )
  {
   do_drow_heal(ch,argument);
   return;
  }
  if( !standard_calls(ch, SCHOOL_HOLY, 1))
   return;
  if( arg[0] == '\0')
  {
   stc("Heal who?\n\r", ch);
   return;
  }
  if( ch->mana < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  
  if( !can_heal(ch, victim) ) return;

  victim->hit += heal;
  ch->mana -= cost;
  if( victim->hit > victim->max_hit)
    victim->hit = victim->max_hit; 
  act("$N wounds begins to close.", ch, NULL, victim, TO_ROOM);
  stc("Your wounds begin to close.\n\r", ch);
  WAIT_STATE(ch, 10);
  return;
}

void do_banish(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int cost = 3500;
  char arg[MAX_STRING_LENGTH];

  argument = one_argument(argument, arg);
  
  if( !standard_calls(ch, SCHOOL_HOLY, 2))
   return;
  if( ch->mana < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("They are not here.\n\r", ch);
   return;
  }
  if( IS_NPC(victim))
  {
   stc("Not on mobs.\n\r", ch);
   return;
  }
  if( ch->fighting != NULL)
  {
   stc("Not while fighitng.\n\r", ch);
   return;
  }
  if( in_arena(victim))
  {
   stc("Not while in the arena.\n\r", ch);
   return;
  }
  if( is_safe(ch, victim))
  {
   stc("Not while safe.\n\r", ch);
   return;
  }
  if( victim->in_room->vnum >= 26000 && victim->in_room->vnum <= 26206 )
  {
   stc("Your spell is deflected back at you and you are banished!\n\r", ch);
   act("$n's spell is deflected, and they are banished!\n\r", ch, NULL, victim, TO_ROOM);
   do_chat(victim, "I like this room! Get out and STAY OUT ASSHOLE!\n\r");
   ch->mana -= cost;
   if( ch->fighting != NULL )
    stop_fighting(ch, TRUE);
   char_from_room(ch);
   char_to_room(ch, get_random_room(ch));
   ch->fight_timer += 5;
   do_look(ch, "");
   WAIT_STATE(ch, 12);
   return;
  }
  ch->mana -= cost;
  act("$N has been banished!\n\r", ch, NULL, victim, TO_ROOM);
  if( victim->fighting != NULL)
   stop_fighting(victim, TRUE);
  char_from_room(victim);
  char_to_room(victim, get_random_room(victim));  
  ch->fight_timer += 5;
  do_look(victim, "");
  WAIT_STATE(ch, 12);
  WAIT_STATE(victim, 12);
  return;
}

void do_godsbless( CHAR_DATA *ch, char *argument)
{
 int level;
 int sn;
 int spelltype;
 int cost = 3000;

 if( !standard_calls(ch, SCHOOL_HOLY, 3))
  return;
 if( ch->mana < cost)
 {
  stc("You cant afford it.\n\r", ch);
  return;
 }
 ch->mana -= cost;
 if( (sn = skill_lookup( "godbless" )) < 0 ) 
  return;
 spelltype = skill_table[sn].target;
 level = 500;
  (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
 WAIT_STATE( ch, 12 );
 return;
}

void do_fanatisism(CHAR_DATA *ch, char *argument)
{
 if( !standard_calls(ch, SCHOOL_HOLY, 5))
  return;
 if( IS_SET(ch->pcdata->powers[0], POWER_LIFE))
 {
  stc("But you have already set your fanatisisim.\n\r", ch);
  return;
 }
 if( ch->hit <= 5000)
 {
  stc("You lack the strength to continue with your fanatisism.\n\r", ch);
  return;
 }
 SET_BIT(ch->pcdata->powers[0], POWER_LIFE);
 ch->hit -= 5000;
 stc("You are now fanatical about your cause.\n\r", ch);
 return;
}

void do_summon( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];

 bool is_evil;
 argument = one_argument( argument, arg);
 argument = one_argument( argument, arg2);
 
 if( IS_NPC(ch))
  return;/*
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_vamp_summon(ch,arg);
  return;
 }*/
 if( !has_power( ch, SCHOOL_HOLY) && !has_power(ch, SCHOOL_EVIL))
 {
  stc("Huh?\n\r", ch);
  return;
 }
 if(has_power(ch, SCHOOL_HOLY))
  is_evil = FALSE;
 if( has_power(ch, SCHOOL_EVIL))
  is_evil = TRUE;
 if( !is_evil && !has_power_level(ch, SCHOOL_HOLY, 4))
 {
  stc("You are not powerful enough to summon angels yet.\n\r", ch);
  return;
 }
 if( is_evil && !has_power_level(ch, SCHOOL_EVIL, 6))
 {
  stc("You lack the power to summon demons to your aid.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Summon who?\n\r",ch);
  return;
 }
 if( !TIME_UP(ch, TIMER_CAN_SUMMON))
 {
  stc("You are still tired from your last summons.\n\r", ch);
  return;
 }
 if( !str_cmp(arg, "gabriel"))
 {
   if( is_evil)
   {
    stc("Summon who?\n\r", ch);
    return;
   }
   if( ch->fighting != NULL)
   {
    stc("You can not summon Gabriel while fighting.\n\r", ch);
    return;
   }
   if( ch->mana < 5000)
   {
    stc("You lack the power to summon gabriel.\n\r", ch);
    return;
   }
   for(victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)
   {
    if( IS_NPC(victim))
     continue;
    victim->hit += 5500;
    if( victim->hit > victim->max_hit)
     victim->hit = victim->max_hit;
   }
   ch->mana -= 5000;
   SET_TIMER(ch, TIMER_CAN_SUMMON, 5);
   act("The sky opens up and a great light beams down from the heavens.", ch,NULL,NULL, TO_ALL);
   act("A great winged figure streaks down from the light and lands in", ch, NULL, NULL, TO_ALL);
   act("front of you.  With a deep baritone voice, Gabriel says 'And by", ch, NULL, NULL, TO_ALL);
   act("the light of the true divine power, Thou art healed.' Spreading", ch, NULL, NULL, TO_ALL);
   act("his wings, Gabriel flies back into the heavens.", ch, NULL, NULL, TO_ROOM);
   stc("You feel exhausted.\n\r", ch);
   return;
  }
  if( !str_cmp(arg, "michael"))
  {
   if( is_evil)
   {
    stc("Summon who?\n\r", ch);
    return;
   }
   if( ch->fighting == NULL)
   {
    stc("Michael refuses to come to your aid if you are not in mortal danger.\n\r", ch);
    return;
   }
   if( !has_power_level(ch, SCHOOL_HOLY, 6))
   {
    stc("You lack the power to summon Michael.\n\r", ch);
    return;
   }
   if( ch->mana < 4000)
   {
     stc("You lack the strength to summon Michael.\n\r", ch);
     return;
   }
   if( (victim = ch->fighting) == NULL)
   {
    bug("Do_summon: Michael, ch fighting NULL.", 0);
    return;
   } 
   sprintf(arg, "You call upon the power of Michael to smite %s.\n\r", victim->name);
   stc(arg, ch);
   sprintf(arg, "%s calls upon the Archangel Michael to smite you!\n\r", ch->name);
   stc(arg, victim);
   act("$n calls upon the Archangel Michael to smite $N!\n\r", ch, NULL, victim, TO_NOTVICT);
   ch->mana -= 4000;
   SET_TIMER(ch, TIMER_CAN_SUMMON,5);
   victim->hit -= (3000 + dice( 50, 75));
   ch->hit += (2000 + dice(20, 15));
   update_pos(victim);
   return;
  }
  if( !str_cmp( arg, "mephisto"))
  {
   if( !is_evil )
   {
    stc("Summon who?\n\r", ch);
    return;
   }
   if( ch->fighting != NULL)
   {
    stc("You cant control Mephisto while fighting.\n\r", ch);
    return;
   }
   if( !has_power_level(ch, SCHOOL_EVIL, 6))
   {
    stc("You don't have the power to summon Mephisto.\n\r", ch);
    return;
   }
   if( ch->mana < 5000)
   {
    stc("You haven't the strength to summon Mephisto.\n\r", ch);
    return;
   }
   if( (victim = get_char_world(ch, arg2)) == NULL)
   {
    stc("They aren't here.\n\r", ch);
    return;
   }
   if( ch->in_room->area != victim->in_room->area)
   {
    stc("You must be in the same area.\n\r", ch);
    return;
   }
   if( is_safe(ch, victim))
   {
    stc("Not while safe.\n\r", ch);
    return;
   }
   ch->mana -= 5000;
   act("You summon Mephisto to blast $N with the power of Hell!", ch, NULL, victim, TO_CHAR);
   act("Mephisto appears in front of you in a flash of flame!  With a loud", victim, NULL, NULL,TO_ALL);
   act("roar raises $n high in the air and slams them into the ground.", victim, NULL, NULL, TO_ALL);
   act("In another inferno of hellfire, Mephisto is gone.", victim, NULL, NULL, TO_ALL);
   hurt_person(ch, victim, number_range(5500, 6500));
   SET_TIMER(ch, TIMER_CAN_SUMMON, 5);
   return;
  }
  if( !str_cmp( arg,"Baal"))
  {
   if( 1 )
   {
    stc("Summon who?\n\r", ch);
    return;
   }
   if( !has_power_level(ch, SCHOOL_EVIL, 7))
   {
    stc("You dont have the power to summon Baal?\n\r", ch);
    return;
   }
   if( ch->mana < 5000)
   {
    stc("You lack the strength to summon Baal.\n\r", ch);
    return;
   }
   if( (victim = get_char_world(ch, arg2)) == NULL)
   {
    stc("Your victim is not here.\n\r", ch);
    return;
   }
   if( IS_NPC( victim))
    return;
   if( is_safe(ch, victim))
   {
    stc("Not while they are safe.\n\r", ch);
    return;
   }
   if( victim->in_room->area != ch->in_room->area)
   {
    stc("They are to far away for Baal to reach them.\n\r", ch);
    return;
   }
   if( is_ds_range(ch) ) return;
   ch->mana -= 5000;
   act("$n diappears in a flash of flame!", victim, NULL, NULL, TO_ROOM);
   char_from_room(victim);
   char_to_room(victim, ch->in_room);
   act("$n summons Baal to drag you to your doom!", ch, NULL, victim, TO_VICT);
   act("$n summons Baal to drag $N to $S doom!", ch, NULL, victim, TO_NOTVICT);
   act("You summon Baal to drag $N to $S doom!", ch, NULL, victim, TO_CHAR);
   WAIT_STATE(ch, 10);
   do_look(victim,"");
   return;
  }
  else
  {
   stc("Not a valid summons.\n\r", ch);
   return;
  }
 return;
}                                                                                                                                                                                                                                                                                                                                                                                                                        

void do_curse(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 AFFECT_DATA af;
 int sn;
 int cost = 100;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg);

 if( !standard_calls(ch, SCHOOL_EVIL, 1))
  return;
 if( arg[0] == '\0')
 {
  stc("Curse who?\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You can't afford it.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("You can not curse a person who is not here.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc( "Not on yourself.\n\r", ch );
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while they are safe.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("curse")) < 0)
  return;
 af.type = sn;
 af.location = APPLY_NONE;
 af.modifier = 0;
 af.duration = number_range(4,8);
 af.bitvector = AFF_CURSE;
 affect_to_char(victim, &af);
 act("You afflict $N with a potent curse.", ch, NULL, victim, TO_CHAR);
 act("$n afflicts you with a potent curse.", ch, NULL, victim, TO_VICT);
 act("$n afflicts $N with a potent curse.", ch, NULL, victim, TO_NOTVICT);
 set_fighting(victim,ch);
 set_fighting(ch,victim);
 update_pos(victim);
 update_pos(ch);
 ch->mana -= cost;
 WAIT_STATE(ch, 10);
 return;
}

void do_defile(CHAR_DATA *ch, char *argument)
{
 int cost = 4000;
 int sn;
 AFFECT_DATA af;

 if( !standard_calls(ch, SCHOOL_EVIL, 2))
  return;
 if( ch->mana < cost)
 {
  stc("You haven't the strength to defile this room.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("defile")) < 0)
  return;
 ch->mana -= cost;
 af.type = sn;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.duration = number_range(4,10);
 af.bitvector = AFF_ROOM_DEFILE;
 affect_to_room(ch->in_room, &af);
 act("Your surroundings have been defiled by evil.", ch, NULL, NULL, TO_ROOM);
 WAIT_STATE(ch, 10);
 return;
}

void do_frighten(CHAR_DATA *ch, char *argument)
{
 int cost = 2000;
 int attempt;
 CHAR_DATA *victim;
 ROOM_INDEX_DATA *was_in;
 ROOM_INDEX_DATA *now_in;
 char arg[MAX_INPUT_LENGTH];
 argument = one_argument(argument, arg);

 if( !standard_calls(ch, SCHOOL_EVIL, 3))
  return;
 if( ch->fighting == NULL)
 {
  stc("But your not fighting.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Frighten who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( victim->fighting != ch)
 {
  stc("They are not fighting you.\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You haven't the strength to frighten someone.\n\r", ch);
  return;
 }
  if( number_percent() > 75)
  {
    send_to_char("You failed!\n\r", ch);
    act("$n roars loudly at $N, but fails to scare $S\n\r", ch, NULL, victim, TO_ROOM);
    return;
  }
  was_in = ch->in_room;
  for ( attempt = 0; attempt < 6; attempt++ )
  {  
   EXIT_DATA *pexit;
   int door;
   door = number_door( );
   if( ( pexit = was_in->exit[door] ) == 0 
   || pexit->to_room == NULL
   || IS_SET(pexit->exit_info, EX_CLOSED) 
   || ( IS_NPC(victim) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
       continue;
   move_char( victim, door );
   if( ( now_in = victim->in_room ) == was_in )
    continue;
   victim->in_room = was_in;
   victim->in_room = now_in;
   if( !IS_NPC(victim) )
    send_to_char( "You have been frightened from combat!\n\r", victim);
   WAIT_STATE(victim,10);
    stop_fighting( victim, TRUE );
   return;
  }
   WAIT_STATE(ch,6);
   ch->mana -= cost;
   act("$n scares $N from combat.\n\r", ch, NULL, victim, TO_ROOM);
   return;
}

void do_shadowplane(CHAR_DATA *ch, char *argument)
{
 int cost = 2000;
 char arg[MAX_INPUT_LENGTH];
 OBJ_DATA *obj;

 argument = one_argument(argument, arg);

 if( !IS_CLASS(ch,CLASS_WOLF) && !IS_CLASS(ch,CLASS_VAMPIRE)
  && (!IS_CLASS(ch,CLASS_APOCRYPHIC) && !standard_calls(ch, SCHOOL_EVIL,4)))
 {
  do_huh(ch,"");
  return;
 }
 if( IS_CLASS(ch,CLASS_WOLF) && GET_POWERS(ch,DISC_OWL) < 6 )
 {
  stc("You need to obtain level 6 in Owl to enter the shadowplane.\n\r",ch);
  return;
 }
 if( IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,DISC_OBTEN) < 5 )
 {
  stc("You need Obtenebration 5 to enter the shadowplane.\n\r",ch);
  return;
 }
 if( ch->pcdata->pulse_timer[PULSE_SHADOWPLANE] < 1 )
 {
  stc("You are too weak to enter shadowplane.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 0)
 {
  stc("Not with a fight timer.\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You haven't the strength to enter the shadowplane.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
    {
        if (!IS_AFFECTED(ch, AFF_SHADOWPLANE))
        {
            send_to_char("You fade into the plane of shadows.\n\r",ch);
            act("The shadows flicker and swallow up $n.",ch,NULL,NULL,TO_ROOM);
            SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
            do_look(ch,"auto");
            return;
        }
        REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
        send_to_char("You fade back into the real world.\n\r",ch);
        act("The shadows flicker and $n fades into existance.",ch,NULL,NULL,TO_ROOM);
        do_look(ch,"auto");
        return;
    }
        
    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
        send_to_char( "What do you wish to toss into the shadow plane?\n\r", ch );
        return;
    }
    
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
        send_to_char( "You toss it to the ground and it vanishes.\n\r", ch );
    else
        send_to_char( "You toss it into a shadow and it vanishes.\n\r", ch );
    return;
}

/* END GOOD & EVIL */
/* BEGIN TIME */

void do_slow( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 2000;
 int sn;
 AFFECT_DATA af;

 argument = one_argument( argument, arg);

 if( !standard_calls(ch, SCHOOL_TIME, 3))
  return;
 if( arg[0] == '\0')
 {
  stc("Slow who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You haven't the energy to slow them.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("slow")) < 0)
  return;
 if( IS_SET(victim->aff2, AFF2_SLOW))
  return;
 if( is_affected(victim, sn))
  return;
 af.type = sn;
 af.location = APPLY_AC;
 af.modifier = 500;
 af.duration = number_range(1,2);
 af.level = 1;
 af.bitvector = AFF2_SLOW;
 affect_to_char2(victim, &af, 1);
 act("Your movements become really slow.", ch, NULL, victim, TO_VICT);
 act("You slow $N's movements.", ch, NULL, victim, TO_CHAR);
 act("$N seems to be moving very slowly.", ch, NULL, victim, TO_NOTVICT);
 WAIT_STATE(ch, 10);
 return;
}

void do_temp_stasis(CHAR_DATA *ch, char *argument)
{
 if( !standard_calls(ch, SCHOOL_TIME, 5))
  return;
 if( ch->pcdata->pulse_timer[PULSE_STASIS] < 1 )
 {
  stc("You are too weak to enter stasis.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 0)
 {
  stc("Not with an active fight timer.\n\r", ch);
  return;
 }
 WAIT_STATE(ch, 10);
 if( ch->stasis)
 {
  stc("You return from stasis.\n\r", ch);
  act("$n returns from stasis.",ch, NULL, NULL, TO_NOTVICT);
  ch->stasis = FALSE;
  return;
 }
 if( !ch->stasis)
 {
  stc("You bring yourself into stasis.\n\r", ch);
  act("$n goes into stasis.", ch, NULL, NULL, TO_NOTVICT);
  ch->stasis = TRUE;
  return;
 }
}

/* END TIME */
/* BEGIN SPACE */
void do_dshift( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 1500;
 int damage = number_range( 1500, 2500);
 argument = one_argument( argument, arg);

 if( IS_CLASS(ch, CLASS_DRAGON))
 {
  do_dragon_shift( ch, arg);
  return;
 } 
 if( !standard_calls(ch, SCHOOL_SPACE, 1))
  return;
 if( ch->mana < cost)
 {
  stc("You haven't the strength for that.\n\r", ch);
  return;
 }
 if( ch->fighting == NULL)
 {
  stc("You must be in conflict to use this skill.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 if( !victim)
  return;
 if( ch == victim)
  return;
 act2("You send dimensional ripples through $N! [$D].", ch, NULL,victim,TO_CHAR, damage);
 act2("$n sends dimensional ripples throughout your body! [$D]", ch,NULL, victim, TO_VICT, damage);
 act2("$n sends dimensional ripples throughout $N's body! [$D]", ch, NULL, victim,TO_NOTVICT, damage);
 ch->mana -= cost;
 hurt_person(ch, victim, damage);
 WAIT_STATE(ch, 12);
 return;
}

void do_gate( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 4000;
 argument = one_argument( argument, arg);
 
 if( !standard_calls(ch, SCHOOL_SPACE, 2))
  return;
 if( arg[0] == '\0')
 {
  stc("Gate who?\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You do not have the strength to Gate.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 10)
 {
  stc("You are tainted from the fight.\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Not on yourself.\n\r",ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( victim->trust >= 7)
 {
  stc("NOT ON IMMORTALS!\n\r", ch);
  return;
 }
 if( victim->mana > ch->mana)
 {
  stc("They are too powerful for you to target.\n\r", ch);
  return;
 }
 if( in_arena(victim) || in_arena(ch))
 {
  stc("Not while in the arena.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if ( ch->in_room->vnum == 26300 )
 {
  stc("Garet Jax silences you before the spell is complete!\n\r", ch);
  act("Garet Jax tells you '#6$n tried to gate you to my home, but I have" 
      " spared you#n'.\n\r", ch, NULL, victim, TO_VICT);
  return;
 }
 if( is_ds_range(ch) ) return;
 if ( victim->in_room->vnum == 26112 )
 {
  stc("You cannot bring them back from their insanity!\n\r", ch );
  return;
 }
 char_from_room(victim);
 char_to_room(victim, ch->in_room);
 sprintf( arg, "You gate in %s.\n\r", victim->name);
 stc( arg, ch);
 sprintf( arg, "%s gates you to their location.\n\r", ch->name);
 stc(arg, victim);
 act("$n gates $N into the room.", ch, NULL, victim, TO_NOTVICT);
 ch->mana -= cost;
 WAIT_STATE(ch, 12);
 return;
}

void do_travel(CHAR_DATA *ch, char *argument)
{
 int cost = 5000;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 argument = one_argument(argument, arg);
 
 if( !standard_calls(ch, SCHOOL_SPACE, 3))
  return;
 if( ch->mana < cost)
 {
  stc("You haven't the strength to travel.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Travel to who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("They aren't here.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Not on yourself.\n\r",ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( victim->trust >= 7)
 {
  stc("NOT ON IMMORTALS.\n\r", ch);
  return;
 }
 if( in_arena(victim))
 {
  stc("Not while they are in the arena.\n\r", ch);
  return;
 }
 if (ch->mana <= victim->mana )
 {
  stc("You lack the power to travel to them.\n\r", ch);
  return;
 }
 if( is_ds_range(victim) ) return;
 act("$n opens a dimensional gateway and steps through it.", ch, NULL, NULL, TO_ROOM);
 char_from_room(ch);
 char_to_room(ch, victim->in_room);
 act("$n has arrive through a dimensional gateway.", ch, NULL, NULL, TO_ROOM);
 WAIT_STATE(ch, 12);
 ch->mana -= cost;
 return;
}

void do_bodyslide( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 
 if( !standard_calls(ch, SCHOOL_SPACE, 4))
  return;
 if( ch->fighting == NULL)
 {
  stc("You must be fighting to use this.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 /* a redundant safety check */
 if( victim == NULL)
 {
  bug("Do_bodyslide: null victim.\n\r", 0);
  return;
 }
 act("$n disappears then reappears behind $N with a vicious back attack.", ch, NULL, victim,
     TO_NOTVICT);
 act("You bodyslide behind $N and slash $S in the back.", ch, NULL, victim, TO_CHAR);
 act("$n disappears! $n reappears behind you swinging $s weapons.", ch, NULL, victim, TO_VICT);
 multi_hit(ch, victim, TYPE_UNDEFINED);
 multi_hit(ch, victim, TYPE_UNDEFINED);
 WAIT_STATE(ch, 24);
 return;
}
/* END SPACE*/
/* BEGIN PLANT*/
void do_thornblast( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	int chance = 0;
	int dam = 0;

	if ( !standard_calls(ch, SCHOOL_PLANT, 6) )
		return;

	if ( ch->move < 2000 )
	{
		stc( "You are too exhausted!\n\r", ch );
		return;
	}

	if ( ( ( victim = ch->fighting ) == NULL ) || ( victim->in_room == NULL ) )
	{
		stc( "You must be fighting someone first!\n\r", ch );
		return;
	}

   	/* The lower your moves, the less the chance */
   	chance = ( ( ( ( (float) ch->move) / ( (float) ch->max_move) ) ) * 100 ) - number_range(45,75);
   	if(victim->stuntimer > 0)
	chance -= 15;
	ch->move -= number_range(1000,2000);
	if( chance < 0 )
		chance = 0;
   	WAIT_STATE(ch, 12);

   	if ( number_percent() < chance )
   	{
		dam = number_range(500,1000);
		if ( !IS_AFFECTED(victim, AFF_WEBBED) )
			dam *= .8;
		else
			dam *= 1.05;
		/* Success - Damage and a stuntimer */
	   	act2("You hit $N with a flurry of thorns that leaves $M stunned! [$D]",ch, NULL, victim, 
                      TO_CHAR, dam );
	   	act2("$n hits you with a flurry of thorns that leaves you stunned! [$D]",ch, NULL, victim, 
                      TO_VICT, dam );
		act2("$n hits $N with a flurry of thorns that leaves $M stunned! [$D]", ch, NULL, victim, 
                      TO_NOTVICT,dam );
		hurt_person(ch,victim,dam);
		set_stun(victim, number_range( 1, 3 ));
		update_pos(victim);
   	}
   
   	else
   	{
		dam = number_range(1000,2000);
                if ( !IS_AFFECTED(victim, AFF_WEBBED) )
                        dam *= .8; 
                else
                        dam *= 1.05;
		/* Failure - More damage than a success, but no stuntimer */
                act2("You blast $N with thorns, leaving $M unphased! [$D]",ch, NULL, victim, TO_CHAR, dam);
		act2("$n blasts you with thorns, but you are unphased! [$D]",ch, NULL, victim, TO_VICT, dam);
		act2("$n blasts $N with thorns, but $E is unphased! [$D]",ch, NULL, victim, TO_NOTVICT, dam);
		hurt_person(ch,victim,dam);
	   	return;
   	}
   	return;
}

void do_entangle(CHAR_DATA *ch, char *argument)
{
 AFFECT_DATA af;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn;
 argument = one_argument( argument, arg);
 
 if( !standard_calls(ch, SCHOOL_PLANT, 2))
  return;
 if( arg[0] == '\0')
 {
  stc("Entangle who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They arent here.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( IS_AFFECTED(victim, AFF_WEBBED))
 {
  stc("They are already webbed.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("web")) < 0)
  return;
 af.type = sn;
 af.location = APPLY_AC;
 af.modifier = 400;
 af.duration = number_range(1,2);
 af.bitvector = AFF_WEBBED;
 affect_to_char(victim, &af);
 act("You cause the plants to grow around $N's legs.", ch, NULL, victim, TO_CHAR);
 act("$n raises $s arms as a bunch of plants grow around your legs.", ch, NULL, victim,             TO_VICT);
 act("$n raises $s arms and causes plants to grow around $N's legs.", ch, NULL, victim,
   TO_NOTVICT);
 WAIT_STATE(ch, 10);
 return;
}

void do_roottravel(CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 int cost = 3000;
 argument = one_argument(argument, arg);

 if( !standard_calls(ch, SCHOOL_PLANT, 3))
  return;
 if( arg[0] == '\0')
 {
  if( ch->recall2 <= 0)
  {
   stc("You have yet to plant the exit root.\n\r", ch);
   return;
  }
  if( ch->fighting != NULL)
  {
   stc("Not while fighting.\n\r", ch);
   return;
  }
  if( (ch->position == POS_STUNNED) || (ch->hit <=0) )
  {
   stc("You must be conscious for this.\n\r", ch);
   return;
  }
  if( IS_AFFECTED(ch, AFF_CURSE))
  {
   stc("Not while cursed.\n\r", ch);
   return;
  }
  if( in_arena(ch))
  {
   stc("Not while in the arena.\n\r", ch);
   return;
  }
  stc("You summon the power of the plants to trasport you.\n\r", ch);
  act("A horde of vines burst forth from the ground and drag $n underground with them.\n\r", ch, NULL, NULL, TO_ROOM);
  char_from_room(ch);
  char_to_room(ch, get_room_index(ch->recall2));
  WAIT_STATE(ch, 12);
  do_look(ch, "auto");
  return;
 }
 else
 {
  if( str_cmp(arg, "plant"))
  {
   stc("Syntax: roottravel plant.\n\r", ch);
   return;
  }
  else
  if( is_ds_range(ch) ) return;
  if( ch->mana < cost)
  {
   stc("You lack the power to plant the seed of magic.\n\r", ch);
   return;
  }
  if( ch->in_room == NULL)
  {
   bug("roottravel plant: ch->in_room == NULL.\n\r", 0);
   return;
  }
  stc("You plant a magical seed.\n\r", ch);
  ch->recall2 = ch->in_room->vnum;
  ch->mana -= cost;
  do_save(ch, "");
  return;
 }
 return;
}

void do_poison( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int cost = 500;
 int sn;
 AFFECT_DATA af;

 if( !standard_calls(ch, SCHOOL_PLANT, 4))
  return;
 if( ch->fighting == NULL)
 {
  stc("You must be fighting for this.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 if( victim == NULL)
 {
  bug( "Do_poison: NULL victim.\n\r", 0);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You lack the power to spray a poison gas.\n\r", ch);
  return;
 }
 if( IS_AFFECTED(victim, AFF_POISON))
 {
  stc("They are already poisoned.\n\r", ch);
  return;
 }
 if( victim->level > 550 )
 {
  stc("They are too powerful for your poison!\n\r", ch);
  return;
 }
 if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DROW) && IS_SET(GET_POWERS(victim,DROW_BITS),DPOWER_DROWPOISON) )
 {
  stc("Your poison is unable to affect them!\n\r",ch);
  return;
 }
 if( (sn = skill_lookup("poison")) < 0)
  return;
 af.type = sn;
 af.duration = number_range(2,6);
 af.location = APPLY_STR;
 af.modifier = -3;
 af.bitvector = AFF_POISON;
 affect_to_char(victim, &af);
 act("$N turns a sickly green as you spew a poison cloud in $S face.", ch, NULL,         victim,TO_CHAR);
 act("Your vision blurs and get dizzy when $n spits a green cloud in your face.", ch,
     NULL, victim, TO_VICT);
 act("$N turns a sickly green when $n blows a green cloud in $S face.", ch, NULL, victim,
      TO_NOTVICT);
 ch->mana -= cost;
 WAIT_STATE(ch, 10);
 return;
}

/* end plant*/
/* BEGIN ANIMAL*/

void do_rage(CHAR_DATA *ch, char *argument)
{
 int rage = 0;
 bool animal = FALSE;
 CHAR_DATA *vch;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch,CLASS_APOCRYPHIC) && !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch, "");
  return;
 }
 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  do_wolf_rage(ch,"");
  return;
 }
 if( !has_power(ch, SCHOOL_ANIMAL) && !has_power(ch, SCHOOL_CHAOS) )
   return;
 if( has_power(ch, SCHOOL_ANIMAL))
   animal = TRUE; /* i could check for Chaos and make animal false, but no need really*/
 if( IS_CLASS(ch,CLASS_APOCRYPHIC) &&( (animal && !has_power_level(ch, SCHOOL_ANIMAL, 1))
  || (!animal && !has_power_level(ch, SCHOOL_CHAOS, 2))))
 {
  stc("You have yet to learn that skill.\n\r", ch);
  return;
 }
 if(ch->rage >= 180)
 {
  stc("Your already highly pissed off.\n\r", ch);
  return;
 }
 if( (ch->rage + rage) >= 100)
 {
  act("$n goes apeshit!", ch, NULL, NULL, TO_ROOM);
  ch->rage += 200;
  if( ch->rage > 300 )
    ch->rage = 300;
  for( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
  {
   if( vch == ch)
    continue; 
   if( is_safe(ch, vch))
    continue;
   if( vch->trust > 6)
    continue;
   multi_hit(ch, vch, 0);
   multi_hit(ch, vch, 0);
   multi_hit(ch, vch, 0);
  }
 }
 rage = number_range(8,15);
 ch->rage += rage;
  act("$n turns red in the face and looks pissed off.", ch, NULL, NULL, TO_ROOM);
  stc("You can feel your blood pressure starting to rise.\n\r", ch);
  WAIT_STATE(ch,12);
 return;
}

void do_burrow(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  do_wburrow(ch,arg);
  return;
 }
 if( !standard_calls(ch, SCHOOL_ANIMAL, 2))
  return;
 if( arg[0] == '\0')
 {
  stc("Burrow to who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not to an npc.\n\r", ch);
  return;
 }
 if( ch->in_room->area != victim->in_room->area)
 {
  stc("Must be in the same area.\n\r", ch);
  return;
 }
 if( is_ds_range(victim) )
  return;
 char_from_room(ch);
 char_to_room(ch, victim->in_room);
 act("$n burrows from out of the ground.", ch, NULL, victim, TO_ROOM);
 do_look(ch, "auto");
 return;
}

void do_claw(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int damage = number_range(1300, 1800);

 if( !standard_calls(ch, SCHOOL_ANIMAL, 5))
  return;
 if( ch->fighting == NULL)
 {
  stc("You must be fighting.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 act2("You slash $N hard with your sharp claws. [$D]", ch, NULL,victim,TO_CHAR, damage);
 act2("$n slashes you hard with $s sharp claws. [$D]", ch, NULL,victim,TO_VICT, damage);
 act2("$n slashes $N hard with $s claws. [$D]", ch, NULL, victim, TO_NOTVICT, damage);
 hurt_person(ch,victim,damage);
 WAIT_STATE(ch, 12);
 return;
}
/* END ANIMAL*/
/* BEGIN ARCANE */
void do_arcanic_gate( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 4000;
 argument = one_argument( argument, arg);
 
 if( !standard_calls(ch, SCHOOL_ARCANE, 4))
  return;
 if( arg[0] == '\0')
 {
  stc("Gate who?\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You have the strength to open a gateway of magic.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 10)
 {
  stc("You are tainted from the fight.\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Not on yourself.\n\r",ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( victim->trust >= 7)
 {
  stc("NOT ON IMMORTALS!\n\r", ch);
  return;
 }
 if( victim->mana > ch->mana)
 {
  stc("They are too powerful for you to target.\n\r", ch);
  return;
 }
 if( in_arena(ch) || in_arena(victim))
 {
  stc("Not while in the arena.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( is_ds_range(ch) ) return;
 if ( ch->in_room->vnum == 26300 )
 {
  stc("Garet Jax silences you before the spell is complete!\n\r", ch);
  act("Garet Jax tells you '#6$n tried to agate you to my home, but I have"
      " spared you#n'.\n\r", ch, NULL, victim, TO_VICT);
  return;
 }
 if ( victim->in_room->vnum == 26112 )
 {
  stc("You cannot bring them back from their insanity!\n\r", ch);
  return;
 }
 char_from_room(victim);
 char_to_room(victim, ch->in_room);
 do_look(victim, "auto");
 sprintf( arg, "You gate in %s.\n\r", victim->name);
 stc( arg, ch);
 sprintf( arg, "%s gates you to their location.\n\r", ch->name);
 stc(arg, victim);
 act("$n gates $N into the room.", ch, NULL, victim, TO_NOTVICT);
 ch->mana -= cost;
 WAIT_STATE(ch, 12);
 return;
}

void do_atravel(CHAR_DATA *ch, char *argument)
{
 int cost = 5000;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 argument = one_argument(argument, arg);
 
 if( !standard_calls(ch, SCHOOL_ARCANE, 3))
  return;
 if( ch->mana < cost)
 {
  stc("You haven't the strength to travel.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Travel to who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("They aren't here.\n\r", ch);
  return;
 }
 if( victim == ch)
 {
  stc("Traveling to your self would be rather dumb.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( victim->trust >= 7)
 {
  stc("NOT ON IMMORTALS.\n\r", ch);
  return;
 }
 if( in_arena(ch) || in_arena(victim))
 {
  stc("Not while in the arena.\n\r", ch);
  return;
 }
 if (ch->mana <= victim->mana ) 
 {
  stc("You lack the power to travel to them.\n\r", ch);
  return;
 }
 if( is_ds_range(victim) ) return;
 act("$n opens a magical gateway and steps through it.", ch, NULL, NULL, TO_ROOM);
 char_from_room(ch);
 char_to_room(ch, victim->in_room);
 do_look(ch, "auto");
 act("$n has arrive through a magical gateway.", ch, NULL, NULL, TO_ROOM);
 WAIT_STATE(ch, 12);
 ch->mana -= cost;
 return;
}

void do_arcanic_might(CHAR_DATA *ch, char *argument)
{
 int cost = 10000;
 int sn;
 CHAR_DATA *victim;
 char arg[MAX_STRING_LENGTH];
 AFFECT_DATA af;

 argument = one_argument( argument, arg);

 if( !standard_calls(ch, SCHOOL_ARCANE, 5))
  return;
 if( arg[0] == '\0')
 {
  stc("Give might to whom?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL )
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( victim->in_room  == ch->in_room)
  cost *= 0.5;
 if( ch->mana < cost)
 {
  stc("You havent the strength for this.\n\r", ch);
  return;
 }
 if( ch->fighting != NULL)
 {
  stc("You can't concentrate while fighting.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("arcane magic")) < 0)
  return;
 if( is_affected(victim, sn))
  return;
 af.type = sn;
 af.duration = number_range(10,15);
 af.location = APPLY_HIT;
 af.modifier = 2000;
 af.bitvector = 0;
 affect_to_char(victim, &af);
 af.location = APPLY_MOVE;
 af.modifier = 2000;
 affect_to_char(victim, &af);
 af.location = APPLY_MANA;
 af.modifier = 2000;
 affect_to_char(victim, &af);
 af.location = APPLY_HITROLL;
 af.modifier = 250;
 affect_to_char(victim, &af);
 af.location = APPLY_DAMROLL;
 af.modifier = 250;
 affect_to_char(victim, &af);
 af.location = APPLY_AC;
 af.modifier = -500;
 affect_to_char(victim, &af);
 stc("You feel the power of the ages enter your soul.\n\r", victim);
 act("You feel the power of the ages go into $N.", ch, NULL, victim, TO_CHAR);
 WAIT_STATE(ch, 12);
 ch->mana -= cost;
 return;
}
 
void do_parasite(CHAR_DATA *ch, char *argument)
{
 int cost = 10000;
 int sn;
 CHAR_DATA *victim;
 char arg[MAX_STRING_LENGTH];
 AFFECT_DATA af;

 argument = one_argument( argument, arg);

 if( !standard_calls(ch, SCHOOL_ARCANE, 6))
  return;
 if( arg[0] == '\0')
 {
  stc("Give a parasite to whom?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL )
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim->in_room->vnum  == ch->in_room->vnum)
  cost = cost * 0.5;
 if( ch->mana < cost)
 {
  stc("You havent the strength for this.\n\r", ch);
  return;
 }
 if( ch->fighting != NULL)
 {
  stc("You can't concentrate while fighting.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("arcane magic")) < 0)
  return;
 if( is_affected(victim, sn))
  return;
 af.type = sn;
 af.duration = number_range(10,15);
 af.location = APPLY_HIT;
 af.modifier = -2000;
 af.bitvector = 0;
 affect_to_char(victim, &af);
 af.location = APPLY_MOVE;
 af.modifier = -2000;
 affect_to_char(victim, &af);
 af.location = APPLY_MANA;
 af.modifier = -2000;
 affect_to_char(victim, &af);
 af.location = APPLY_HITROLL;
 af.modifier = -250;
 affect_to_char(victim, &af);
 af.location = APPLY_DAMROLL;
 af.modifier = -250;
 affect_to_char(victim, &af);
 af.location = APPLY_AC;
 af.modifier = 500;
 affect_to_char(victim, &af);
 stc("You feel something sapping your strength.\n\r", victim);
 act("You send a magical parasite into $N.", ch, NULL, victim, TO_CHAR);
 ch->fight_timer += 10;
 if( ch->fight_timer > 30);
  ch->fight_timer = 30;
 WAIT_STATE(ch, 12);
 ch->mana -= cost;
 return;
} 

/*END ARCANE*/
/*BEGIN ORDER*/
void do_calm(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 sh_int cost = 350;
 sh_int rage = number_range(8,15);
 
 argument = one_argument( argument, arg); 

 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  do_wolf_calm(ch,"");
  return;
 }
 if( (IS_CLASS(ch,CLASS_APOCRYPHIC) && !standard_calls(ch, SCHOOL_ORDER, 1)) )
 {
  do_huh(ch,"");
  return;
 }
 if( ch->mana < cost )
 {
  stc("You don't have the strength for that.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Calm who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( victim->rage <= 0 )
 {
  stc("But they aren't raged.\n\r", ch);
  return;
 }
 victim->rage -= rage;
  ch->mana -= cost;
  act("$n is calming the rage in your soul.", ch, NULL, victim, TO_VICT);
  act("You calm the rage in $N's soul.", ch, NULL, victim, TO_CHAR);
  WAIT_STATE(ch, 10);
 return;
}

void do_holdperson(CHAR_DATA *ch, char *argument)
{
 AFFECT_DATA af;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn;
 argument = one_argument( argument, arg);
 
 if( !standard_calls(ch, SCHOOL_ORDER, 2))
  return;
 if( arg[0] == '\0')
 {
  stc("Hold who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They arent here.\n\r", ch);
  return;
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( IS_AFFECTED(victim, AFF_WEBBED))
 {
  stc("They are already webbed.\n\r", ch);
  return;
 }
 if( (sn = skill_lookup("web")) < 0)
  return;
 af.type = sn;
 af.location = APPLY_AC;
 af.modifier = 400;
 af.duration = number_range(1,2);
 af.bitvector = AFF_WEBBED;
 affect_to_char(victim, &af);
 act("You wrap $N in bands of pure willpower.", ch, NULL, victim, TO_CHAR);
 act("$n stops your movement with pure willpower.", ch, NULL, victim, TO_VICT);
 act("$n causes $N to stop moving with pure willpower", ch, NULL, victim, TO_NOTVICT);
 WAIT_STATE(ch, 10);
 return;
}

void do_balance(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int cost = 0;
 bool token = FALSE; /* to prevent possible loops*/

 if( !standard_calls(ch, SCHOOL_ORDER, 3))
  return;
 if( ch->mana < cost)
 {
  stc("You can't afford it.\n\r", ch);
  return;
 }
 if( ch->fighting == NULL)
 {
  stc("You must be in combat.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 ch->mana -= cost;
 if( (ch->hit >= victim->hit) && !token)
 {
  hurt_person(ch, ch,number_range(600, 1000));
  victim->hit += number_range( 300, 900);
  update_pos(ch);
  update_pos(victim);
  token = TRUE;
 }
 else if( (victim->hit >= ch->hit) && !token)
 {
  ch->hit += number_range( 900, 1200);
  hurt_person(ch,victim,number_range( 300,900));
  update_pos(ch);
  update_pos(victim);
  token = TRUE;
 }
 token = FALSE;
 if( (ch->mana >= victim->mana) && !token)
 {
  ch->mana -= 250;
  victim->mana += 500;
  token = TRUE;
 }
 else if( (victim->mana >= ch->mana) && !token)
 {
  ch->mana += 1000;
  victim->mana -= 500;
  token = TRUE;
 }
 token = FALSE;
 if( (ch->move >= victim->move) && !token)
 {
  ch->move -= 250;
  victim->move += 500;
  token = TRUE;
 }
 else if( (victim->move >= ch->move) && !token)
 {
  ch->move += 1000;
  victim->move -= 500;
  token = TRUE;
 }
 act("You call upon the power of the cosmos to restore balance.", ch,NULL,victim, TO_CHAR);
 act("$n calls seeks balance with $N.",ch, NULL, victim, TO_NOTVICT);
 act("$n balances the forces between the two of you.", ch, NULL, victim, TO_VICT);
 WAIT_STATE(ch, 10);
 return;
}
/* END ORDER */
/*BEGIN CHAOS */
void do_shatter( CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 int cost = 500;
 int chance = number_range(1,100);
 argument = one_argument(argument, arg);

 if( !standard_calls(ch, SCHOOL_CHAOS, 1))
  return;
 if( arg[0] == '\0')
 {
  stc("Shatter what?\n\r", ch);
  return;
 }
 if( (obj = get_obj_carry(ch, arg)) == NULL)
 {
  stc("You can't shatter what you dont have.\n\r", ch);
  return;
 }
 if( chance < 85)
 {
  stc("You rant and rave, but cant seem to break it.\n\r", ch);
  ch->move -= cost;
  return;
 }
 sprintf( arg, "You shatter %s.\n\r", obj->short_descr);
 stc(arg, ch);
 extract_obj(obj);
 ch->move -= cost;
 WAIT_STATE(ch, 12);
 return;
}

void do_strike(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam = number_range( 1800, 2600);
 int cost = 500;

 if( !standard_calls(ch, SCHOOL_CHAOS, 6))
  return;
 if( ch->fighting == NULL)
 {
  stc("You must be figting to strike someone down.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 if( ch->mana < cost)
 {
  stc("You haven't the strength for this.\n\r", ch);
  return;
 }
 ch->mana -= cost;
 damage(ch, victim, dam, gsn_strike);
 WAIT_STATE(ch, 12);
 return;
}
/* END CHAOS */
/* BEING LIFE */
void do_essence( CHAR_DATA *ch, char *argument)
{
 int cost = 300;
 int heal = number_range( 250, 400);

 if( !standard_calls( ch, SCHOOL_LIFE, 1))
  return;
 if( ch->mana < cost)
 {
  stc("You haven't the strength for this.\n\r", ch);
  return;
 }
 act2("Your spiritual essense flares to heal your wounds. [$D]", ch, NULL, NULL, TO_CHAR,heal);
 act2("$n's glows with a soft light and $s wounds begin to heal. [$D]", ch, NULL, NULL, TO_ROOM, heal);
 ch->hit += heal;
 if( ch->hit > ch->max_hit)
  ch->hit = ch->max_hit;
 ch->mana -= cost;
 WAIT_STATE(ch, 10);
 return;
}

void do_givelife( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 300;
 int heal = 500;

 argument = one_argument( argument, arg);

 if( !standard_calls(ch, SCHOOL_LIFE, 2))
  return;
 if( arg[0] == '\0')
 {
  stc("Give life to who?\n\r", ch);
  return;
 }
 if( ch->hit <= cost)
 {
  stc("That would kill you though.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }

 if( !can_heal(ch, victim) ) return;

 ch->hit -= cost;
 victim->hit += heal;
 update_pos(victim);
 act("A ray of healing light flows from $n into $N.  $n looks very tired.", ch, NULL,
      victim, TO_NOTVICT);
 act("You send $N a bit of your life force.", ch, NULL, victim, TO_CHAR);
 act("$n sends you a bit of $s life force.", ch, NULL, victim, TO_VICT);
 WAIT_STATE(ch, 5);
 return;
}

void do_life_restore( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 6000;
 int heal = 5000;
 char buf[MAX_INPUT_LENGTH];
 argument = one_argument(argument, arg);

 if( !standard_calls(ch, SCHOOL_LIFE, 6))
  return;
 if( arg[0] == '\0')
 {
  stc("Restore who?\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You cant afford it.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They aren't here.\n\r", ch);
  return;
 }

 if( !can_heal(ch, victim) ) return;

 ch->mana -= cost;
 victim->hit += heal;
 if( victim->hit > victim->max_hit)
  victim->hit = victim->max_hit;
 sprintf(buf, "You restore a portion of $N's health. [%d]", heal);
 act(buf, ch, NULL, victim, TO_CHAR);
 sprintf( buf, "$n restores a portion of your health. [%d]", heal);
 act(buf, ch, NULL, victim, TO_VICT);
 sprintf( buf, "$n restores a portion of $N's health. [%d]", heal);
 act(buf, ch, NULL, victim, TO_NOTVICT);
 WAIT_STATE(ch, 30);
 return;
}

void do_syphon( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int cost = 450;
 int damage = number_range( 1000, 2000);
 int heal = number_range( 500, 1000);

 if( !standard_calls(ch, SCHOOL_DEATH, 1))
  return;
 if( ch->mana < cost)
 {
  stc("You can't afford it.\n\r", ch);
  return;
 }
 if( ch->fighting == NULL)
 {
  stc("You must be fighting to do this.\n\r", ch);
  return;
 }
 victim = ch->fighting;
 act2("$n syphons off a bit of $N's health [$D].",ch, NULL, victim, TO_NOTVICT, damage);
 act2("You syphon off a bit of $N's health. [$D]",ch, NULL, victim, TO_CHAR, damage);
 act2("$n syphons off a bit of your health. [$D]", ch, NULL, victim, TO_VICT, damage);
 ch->mana -= cost;
 ch->hit += heal;
 hurt_person(ch,victim,damage);
 if( ch->hit > ch->max_hit)
  ch->hit = ch->max_hit;
 WAIT_STATE(ch, 8);
 return;
}

void do_remort( CHAR_DATA *ch, char *argument)
{
	int i;
	char buf[MAX_STRING_LENGTH];

	if( IS_NPC(ch))
		return;
	if( ch->race < 35)
	{
		stc("You must have at least 35 status to remort.\n\r", ch);
		return;
	}
	if( ch->in_room->vnum != 3230 )
	{
		stc("You must be standing in the autoclassing chamber to remort.\n\r", ch);
		return;
	}
	if( IS_DAMNED(ch))
	{
		stc("You must repent yourself before you can remort.\n\r", ch);
		return;
	}
	for( i = 0; i < 13; i++)
		ch->wpn[i] = 0;
	for( i = 0; i < 5; i++)
		ch->spl[i] = 0;
	for( i = 0; i < 11; i++)
		ch->stance[i] = 0;
        ch->exp = 0;
        ch->practice = 0;
	ch->race = 0;
	ch->max_hit = 1000;
	ch->max_mana = 500;
	ch->max_move = 500;
	ch->warp = 0;
	ch->warpcount = 0;
	ch->pcdata->remort++;
                  ch->pcdata->race->name = '\0';
 	ch->pcdata->race->group1 = 0;
	ch->pcdata->race->group2 = 0;
	ch->pcdata->race->complete = FALSE;
 	ch->ABlock[POWER_TYPE] = 0;
 	ch->ABlock[POWER_LEVEL] = 0;
 	ch->BBlock[POWER_TYPE] = 0;
 	ch->BBlock[POWER_LEVEL] = 0;
 	ch->CBlock[POWER_TYPE] = 0;
 	ch->CBlock[POWER_LEVEL] = 0;
 	for( i = 0; i < 25; i++)
  		GET_POWERS( ch, i) = 0;
 	ch->class = CLASS_NONE;
	sprintf( buf, "%s has just remorted.\n\r", ch->pcdata->switchname);
	do_info( ch, buf);	
	return;
}
