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

void do_evolve(CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg);
 if( arg[0] == '\0')
 {
  display_class_stats(ch);
  return;
 }
 if( !str_prefix(arg, "nerves"))
 {
  improve_class(ch, ALIEN_SPEED);
  return;
 }
 else if( !str_prefix(arg, "muscles"))
 {
  improve_class(ch, ALIEN_STR);
  return;
 }
 else if( !str_prefix(arg, "carapace"))
 {
  improve_class(ch, ALIEN_ARMOR);
  return;
 }
 else if( !str_prefix( arg, "senses"))
 {
  improve_class(ch, ALIEN_SENSE);
  return;
 }
 else
 {
  stc("You can't evolve that.\n\r", ch);
  return;
 }
 return;
}

void do_primal_urge(CHAR_DATA *ch, char *argument)
{
 int cost = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 cost = (GET_POWERS(ch, ALIEN_PRIMAL) +1) * 250;
 if( ch->move < cost)
 {
  stc("You are too tired to reach the beast within.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, ALIEN_PRIMAL) >= 6)
 {
  stc("You have reach the pinnacle of your fury.\n\r", ch);
  return;
 }
 INC_POWERS(ch, ALIEN_PRIMAL);
 ch->move -= cost;
 stc("You can feel the rage mounting in your soul.\n\r", ch);
 act("$n screams as the inner rage of the soul is unleashed.", ch, NULL, NULL, TO_ROOM);
 return;
}

void do_aclaw( CHAR_DATA *ch, char *argument)
{
 int cost = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 cost = GET_POWERS(ch, ALIEN_SIZE) * 500;
 if( ch->mana < cost)
 {
  stc("You haven't the strength to re-grow your claws.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLAWS) > 0)
  stc("Your claws fall out as a new set takes their place.\n\r", ch);
 else
  stc("You grow a set of razor sharp claws.\n\r", ch);
 ch->pcdata->powers[ALIEN_CLAWS] = GET_POWERS(ch, ALIEN_SIZE)+1;
 ch->mana -= cost;
 return;
}

void do_atail( CHAR_DATA *ch, char *argument)
{
 int cost = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 cost = GET_POWERS(ch, ALIEN_SIZE) * 500;
 if( ch->mana < cost)
 {
  stc("You haven't the strength to re-grow your tail.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, ALIEN_TAIL) > 0)
  stc("Your tail falls out as a new one takes its place.\n\r", ch);
 else
  stc("You grow a long spiny tail from the base of your spine.\n\r", ch);
 ch->pcdata->powers[ALIEN_TAIL] = GET_POWERS(ch, ALIEN_SIZE)+1;
 ch->mana -= cost;
 return;
} 

void do_pain(CHAR_DATA *ch, char *argument)
{
 int cost = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 cost = (GET_POWERS(ch, ALIEN_PAIN)+1) * 125;
 if( ch->move < cost)
 {
  stc("You are too tired to ignore pain.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, ALIEN_PAIN) >= 10)
 {
  stc("Pain already means nothing to you.\n\r", ch);
  return;
 }
 stc("You no longer feel the pain of your wounds.\n\r", ch);
 ch->move -= cost;
 INC_POWERS(ch, ALIEN_PAIN);
 return;
}

void do_slash( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam = number_range(1000,1800);
 int cost = 500;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLAWS) <= 0)
 {
  stc("But you have no claws.\n\r", ch);
  return;
 }
 if( ch->move < cost)
 {
  stc("You are too tired to swing your claws.\n\r", ch);
  return;
 }
 if( (victim = ch->fighting) == NULL)
 {
   if( arg[0] == '\0')
   {
    stc("Slash your claws at who?\n\r", ch);
    return;
   }
   else if( (victim = get_char_room(ch, arg)) == NULL)
   {
    stc("They are not here.\n\r", ch);
    return;
   }
 } 
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( !IS_NPC(victim) && ((victim->trust < 3) || (victim->level < 3)))
 {
  stc("You both must be avatars.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = class_mod_damage(ch, victim, dam);
 hurt_person(ch, victim, dam);
 ch->move -= cost;
 act2("You slash your claws at $N violently! [$D].", ch, NULL, victim, TO_CHAR, dam);
 act2("$n swings razor sharp claws at you violently! [$D].", ch, NULL, victim, TO_VICT, dam);
 act2("$n slashes violently at $N with razor sharp claws. [$D].", ch, NULL, victim, TO_NOTVICT, dam);
 WAIT_STATE(ch, 18);
 if( (ch->fighting == NULL) && victim)
  set_fighting(ch, victim);
 return;
}

void do_taillash( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam = number_range(1200,2100);
 int cost = 500;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLASS) < ALIEN_WAR)
 {
  stc("Only warriors and above have the knowledge to use a tail.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, ALIEN_TAIL) <= 0)
 {
  stc("But you have no tail.\n\r", ch);
  return;
 }
 if( ch->move < cost)
 {
  stc("You are too tired to lash out with your tail.\n\r", ch);
  return;
 }
 if( (victim = ch->fighting) == NULL)
 {
   if( arg[0] == '\0')
   {
    stc("Lash your tail at who?\n\r", ch);
    return;
   }
   else if( (victim = get_char_room(ch, arg)) == NULL)
   {
    stc("They are not here.\n\r", ch);
    return;
   }
 }
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 } 
 if( !IS_NPC(victim) && ((victim->trust < 3) || (victim->level < 3)))
 {
  stc("You both must be avatars.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = class_mod_damage(ch, victim, dam);
 hurt_person(ch, victim, dam);
 ch->move -= cost;
 act2("You lash at $N with your tail! [$D].", ch, NULL, victim, TO_CHAR, dam);
 act2("$n lashs at you with a razor sharp tail! [$D].", ch, NULL, victim, TO_VICT, dam);
 act2("$n lashs at $N with a razor sharp tail! [$D].", ch, NULL, victim, TO_NOTVICT, dam);
 WAIT_STATE(ch, 18);
 if( (ch->fighting == NULL) && victim)
  set_fighting(ch, victim);
 return;
}

void do_infect( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 DESCRIPTOR_DATA *d;
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 int cost = 10000;

 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
  return;
 if( GET_POWERS(ch, ALIEN_CLASS) < ALIEN_PRAE)
 {
  stc("You must be a praetorian to use this.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, ALIEN_SENSE) < 7)
 {
  stc("You don know how to infect.\n\r", ch);
  return;
 }
 if( ch->desc->infect != NULL)
 {
  stc("You can't infect while you are infected.\n\r", ch);
  return;
 } 
 if( arg[0] == '\0')
 {
  stc("Infect who?\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  sprintf( buf, "You need %d mana to infect someone.\n\r", cost);
  stc(buf, ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if ( victim == ch )
 {
  send_to_char( "Severing psychic connection with infecting parasite.\n\r", ch );
  for ( d = descriptor_list; d != NULL; d = d->next )
  {
   if ( d->infect == ch->desc )
     d->infect = NULL;
  }
  return;
 }
 if( IS_NPC(victim))
  return;
 if( IS_IMMORTAL(victim))
  return;
 if( !IS_HERO(victim))
 {
  stc("Only on avatars.\n\r", ch);
  return;
 }
 if( victim->desc->infect)
 {
  stc("They are already infected.\n\r", ch);
  return;
 }
 ch->mana -= cost;
 act("You have infected $N.", ch, NULL, victim, TO_CHAR);
 victim->desc->infect = ch->desc;
 return;
}

void do_atransport(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 500;

 argument = one_argument( argument, arg);
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
   return;
 }
 if( GET_POWERS(ch, ALIEN_CLASS) < ALIEN_RUN)
 {
  stc("You must be at least a runner.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Transport to who?\n\r", ch);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You lack the energy to transport yourself.\n\r", ch);
  return;
 }
 if( (victim = get_char_area(ch, arg)) == NULL)
 {
  stc("They arent here.\n\r", ch);
  return;
 }
 if( IS_IMMORTAL(victim) || !IS_HERO(victim))
 {
  stc("You can only transport to avatars.\n\r", ch);
  return;
 }
 ch->mana -= cost;
 if( number_percent() < 30)
 {
  stc("Your mother ship failed to lock onto your target.\n\r", ch);
  return;
 }
 if( is_ds_range(ch) ) return;
 stc("You go to your target in a dazzling beam of light.\n\r", ch);
 char_from_room(ch);
 char_to_room(ch, victim->in_room);
 act("$n appears before you in a dazzling beam of light!", ch, NULL, NULL, TO_NOTVICT);
 WAIT_STATE(ch, 12);
 return;
}

void do_cloak(CHAR_DATA *ch, char *argument)
{ 

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLASS) < ALIEN_WAR)
 {
  stc("You need to be a warrior to cloak.\n\r", ch);
  return;
 }
 if( ch->mana < 2000)
 {
  stc("You are too tired.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 0)
 {
  stc("Not with a fight timer.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,ALIEN_BITS), ALIEN_CLOAK))
 {
  stc("You turn off you cloaking field.\n\r", ch);
  REM_BIT_POWERS(ch, ALIEN_BITS, ALIEN_CLOAK);
  act("$n drops $s cloaking field.", ch, NULL, NULL, TO_NOTVICT);
  return;
 }
 else
 {
  stc("You put up your cloaking field.\n\r", ch);
  act("$n disappears as $s cloaking field engages.", ch, NULL, NULL, TO_NOTVICT);
  SET_BIT_POWERS(ch, ALIEN_BITS, ALIEN_CLOAK);
  return;
 }
 return;
}

void do_metamorph( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 argument = one_argument(argument, arg);
 if( arg[0] == '\0')
 {
  stc("Metamorphose into what?\n\r", ch);
  return;
 }
 if( !str_prefix(arg, "warrior"))
 {
  if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_RUN)
  {
   stc("You must be a runner first.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_DNA) < 5000)
  {
   stc("You need at least 5000 dna points to change to a warrior.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_SIZE) < 3)
  {
   stc("You are too small to form into a warrior.\n\r", ch);
   return;
  }
  GET_POWERS(ch, ALIEN_CLASS) = ALIEN_WAR;
  GET_POWERS(ch, ALIEN_TIMER) = 30;
  GET_POWERS(ch, ALIEN_DNA) -= 5000;
  stc("You begin to morph into a warrior.\n\r", ch);
  return;
 }
 else if( !str_prefix( arg, "runner"))
 {
  if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_DRONE)  
  {
   stc("You must be a Drone.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_DNA) < 5000)
  {
   stc("You need at least 5000 dna points to change to a runner.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_SIZE) < 2)
  {
   stc("You are too small to form into a runner.\n\r", ch);
   return;
  }
  GET_POWERS(ch, ALIEN_CLASS) = ALIEN_RUN;
  GET_POWERS(ch, ALIEN_TIMER) = 30;
  GET_POWERS(ch, ALIEN_DNA) -= 5000;
  stc("You begin to morph into a runner.\n\r", ch);
  return;
 }
 else if( !str_prefix( arg, "drone"))
 {
  if( GET_POWERS(ch, ALIEN_CLASS) == ALIEN_DRONE)
  {
   stc("You are already a drone.\n\r", ch);
   return;
  }
 GET_POWERS(ch, ALIEN_CLASS) = ALIEN_DRONE;
 GET_POWERS(ch, ALIEN_TIMER) = 20;
 stc("You begin to revert back to a drone.\n\r", ch);
 return;
 }
 else if( !str_prefix( arg, "praetorian"))
 {
  if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_WAR)  
  {
   stc("You must be a warrior first.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_DNA) < 5000)
  {
   stc("You need at least 5000 dna points to change to a praetorian.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_SIZE) < 6)
  {
   stc("You are too small to form into a praetorian.\n\r", ch);
   return;
  }
  GET_POWERS(ch, ALIEN_CLASS) = ALIEN_PRAE;
  GET_POWERS(ch, ALIEN_TIMER) = 30;
  GET_POWERS(ch, ALIEN_DNA) -= 5000;
  stc("You begin to morph into a praetorian.\n\r", ch);
  return;
 }
 else if( !str_prefix(arg, "scorpion"))
 {
  if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_PRAE)  
  {
   stc("You must be a Praetorian first.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_DNA) < 5000)
  {
   stc("You need at least 5000 dna points to change to a warrior.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_SIZE) < 9)
  {
   stc("You are too small to form into a scorpion.\n\r", ch);
   return;
  }
  GET_POWERS(ch, ALIEN_CLASS) = ALIEN_SCORP;
  GET_POWERS(ch, ALIEN_TIMER) = 30;
  GET_POWERS(ch, ALIEN_DNA) -= 5000;
  stc("You begin to morph into a scorpion hybrid.\n\r", ch);
  return;
 }
 else if( !str_prefix( arg, "gorilla"))
 {
  if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_PRAE)  
  {
   stc("You must be a praetorian first.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_DNA) < 5000)
  {
   stc("You need at least 5000 dna points to change to a warrior.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, ALIEN_SIZE) < 9)
  {
   stc("You are too small to form into a warrior.\n\r", ch);
   return;
  }
  GET_POWERS(ch, ALIEN_CLASS) = ALIEN_GOR;
  GET_POWERS(ch, ALIEN_TIMER) = 30;
  GET_POWERS(ch, ALIEN_DNA) -= 5000;
  stc("You begin to morph into a gorilla.\n\r", ch);
  return;
 }
 else
 {
  stc("Syntax: metamorph <drone/runner/warrior/praetorian/gorilla/scorpion>\n\r", ch);
  return;
 }
 return;
}

void do_bash(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 700;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_GOR)
 {
  stc("You must be a gorilla hybrid.\n\r", ch);
  return;
 }
 if( ch->move < cost)
 {
  stc("Your too tired stun.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
   if( (victim = ch->fighting) == NULL)
   {
    stc("Bash who?\n\r", ch);
    return;
   }
 }
 else if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They aren't here.\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
  return;
 if( IS_NPC(victim))
  return;
 if( IS_IMMORTAL(victim))
  return;
 if( number_percent() >= 50 )
 {
  act("You try to bash $N but miss $S.", ch, NULL, victim, TO_CHAR);
  act("$n tries to bash you but fails.", ch, NULL, victim, TO_VICT);
  act("$n tries to bash $N but misses $S.", ch, NULL, victim, TO_NOTVICT);
  WAIT_STATE(ch,12);
  return;
 }

 set_stun(victim, number_range(1,2));
 act("You stun $N leaving $S dazed.", ch, NULL, victim, TO_CHAR);
 act("$n stuns you, leaving you dazed.", ch, NULL, victim, TO_VICT);
 act("$n stuns $N leaving $S dazed.", ch, NULL, victim, TO_NOTVICT);
 ch->move -= cost;
 WAIT_STATE(ch,12);
 return;
}

void do_spit(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 500;
 int dam = number_range(700, 1500);
 AFFECT_DATA af;
 int sn;

 argument = one_argument(argument, arg);
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_vamp_spit(ch,arg);
  return;
 }
 if( IS_NPC(ch) || !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_SCORP)
 {
  stc("You must be a scorpion to spit.\n\r", ch);
  return;
 }
 if( ch->move < cost)
 {
  stc("You are to tired to spit.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
   if( (victim = ch->fighting) == NULL)
   {
    stc("Spit on  who?\n\r", ch);
    return;
   }
 }
 else if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They aren't here.\n\r", ch);  
  return;
 }

 if( is_safe(ch, victim) )
 {
   stc("You cannot spit on someone who is under protection.\n\r", ch);
   return;
 }

 if( !IS_HERO(victim))
 {
  stc("Only on avatars.\n\r", ch);
  return;
 }
 hurt_person(ch, victim, dam);
 ch->move -= cost;
 if( !IS_AFFECTED(ch, AFF_POISON))
 {
  if( (sn = skill_lookup("poison")) < 0)
   return;
  af.type = sn;
  af.duration = number_range(2,6);
  af.location = APPLY_STR;
  af.modifier = -10;
  af.bitvector = AFF_POISON;
  affect_to_char(victim, &af);
  af.type = sn;
  af.duration = number_range(2,6);
  af.location = APPLY_HITROLL;
  af.modifier = -250;
  af.bitvector = AFF_POISON;
  affect_to_char(victim, &af);
  af.type = sn;
  af.duration = number_range(2,6);
  af.location = APPLY_DAMROLL;
  af.modifier = -250;
  af.bitvector = AFF_POISON;
  affect_to_char(victim, &af); 
 }
 act2("You spit a stream of venom at $N. [$D]", ch, NULL, victim, TO_CHAR,dam);
 act2("$n spits a stream of venom at you. [$D]", ch, NULL, victim, TO_VICT,dam);
 act2("$n spits a stream of venom at $N. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
 WAIT_STATE(ch,8);
 return;
}

void do_explode( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam;
 int value;
 int cost = 7000;

 if( IS_NPC(ch) || !IS_CLASS(ch, CLASS_ALIEN))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, ALIEN_CLASS) != ALIEN_SCORP)
 {
  stc("Only scorpion hybrids can explode.\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You cant muster the strength.\n\r", ch);
  return;
 }
 value = (ch->hit/ch->max_hit) * 100;
 if( value > 10)
 {
  stc("You are not yet injured enough to commit suicide.\n\r", ch);
  return;
 }
 for( victim = ch->in_room->people; victim; victim = victim->next_in_room)
 {
  if( victim == ch)
   continue;
  if( !IS_HERO(victim))
   continue;
  if( IS_IMMORTAL(victim))
   continue;
  dam = number_range( 500,1000);
  hurt_person(ch, victim,dam);
  act2("$n explodes covering you in acid! [$D]", ch, NULL, victim, TO_VICT, dam);
 }
 ch->mana = 0;
 ch->hit = -5;
 update_pos(ch);
 stc("You explode covering everyone in corrosive acid!\n\r", ch);
 return;
}
