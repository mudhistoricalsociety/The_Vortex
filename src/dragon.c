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

int gas_breath(CHAR_DATA *ch);
void dragon_breath_affect( CHAR_DATA *ch, CHAR_DATA *victim, int type);
void destroy_part(CHAR_DATA *ch, int type);

int get_breath(CHAR_DATA *ch)
{
 int ret_val = 0;
 
 if( IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FIRE))
  ret_val++;
 if( IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ICE))
  ret_val++;
 if( IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_LIGHTNING))
  ret_val++;
 if( IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ACID))
  ret_val++;
 if( IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_POISON))
  ret_val++;
 return ret_val;
}

void dragon_breath_affect(CHAR_DATA *ch, CHAR_DATA *victim, int type)
{
 int sn, level;
 AFFECT_DATA af;

 if( victim == NULL)
  return;
 if( ch == NULL)
  return;
 switch( type )
 {
  case DRAGON_TYPE_FIRE:
   if( !IS_SET(victim->affected_by, AFF_FLAMING))
   {
    SET_BIT(victim->affected_by, AFF_FLAMING);
    act("$N has been set on fire by $n's firebreath.", ch, NULL, victim, TO_ROOM);
    act("You have set $N on fire with your firebreath.", ch, NULL, victim, TO_CHAR);
    act("$n has set you on fire with $s firebreath.", ch, NULL, victim, TO_VICT);
   }
   break;
  case DRAGON_TYPE_ICE:
   level = ch->spl[1] * 0.35;
   if( (sn = skill_lookup( "chill touch" ) ) > 0)
    (*skill_table[sn].spell_fun) (sn,level,ch, victim);
   break;
  case DRAGON_TYPE_POISON:
   if( !IS_AFFECTED(victim, AFF_POISON))
   {
    if( (sn = skill_lookup("poison")) > 0)
    {
     af.type = sn;
     af.duration = 3;
     af.location = APPLY_STR;
     af.modifier = -5;
     af.bitvector = AFF_POISON;
     affect_to_char(victim, &af);
     act("Your poisonous breath has affected $N.", ch, NULL, victim, TO_CHAR);
     act("$n's poisonous breath causes you to choke and gag.", ch, NULL, victim, TO_VICT);
     act("$n's poisonous breath causes $N to choke and gag.", ch, NULL, victim, TO_NOTVICT);
    }
   }
   break;
  case DRAGON_TYPE_LIGHTNING:
   stop_fighting( ch, TRUE);
   stop_fighting( victim, TRUE);
   victim->position = POS_STUNNED;
   victim->stuntimer = 2;
   update_pos(victim);
   break;
  case DRAGON_TYPE_ACID:
   sn = number_range(1,4);
   destroy_part(victim,sn);
   break;
  default:
   break;
 } 
 return;
}

void destroy_part( CHAR_DATA *ch, int type)
{
 OBJ_DATA *obj;
 CHAR_DATA *victim = ch; /* i copied code and am VERY lazy*/
 
 if( ch == NULL)
  return;
 if( type <= 0)
  return;
 switch( type)
 {
  case 1:
   if (IS_ARM_L(victim,LOST_ARM)) 
    return;  
   SET_BIT(victim->loc_hp[2],LOST_ARM);
   if (!IS_BLEEDING(victim,BLEEDING_ARM_L))
    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
   if (IS_BLEEDING(victim,BLEEDING_HAND_L))
    REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
   act("Acid turns $n's left arm into goo.", ch, NULL, NULL, TO_ROOM);
   stc("Acid turns your left arm into goo.", ch); 
   make_part(victim,"arm");
   if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
   {
    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
     take_item(victim,obj);
   }
   if ((obj = get_eq_char( victim, WEAR_HOLD )) != NULL)
    take_item(victim,obj);
   if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
    take_item(victim,obj);
   if ((obj = get_eq_char( victim, WEAR_WRIST_L )) != NULL)
    take_item(victim,obj);
   if ((obj = get_eq_char( victim, WEAR_FINGER_L )) != NULL)
    take_item(victim,obj);
    break;
  case 2:
   if( IS_ARM_R(victim,LOST_ARM))
    return;
   SET_BIT(victim->loc_hp[3],LOST_ARM);
   if (!IS_BLEEDING(victim,BLEEDING_ARM_R))
    SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
   if (IS_BLEEDING(victim,BLEEDING_HAND_R))
    REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
   act("Acid turns $n's right arm into goo.", ch, NULL, NULL, TO_ROOM);
   stc("Acid turns your right arm into goo.", ch); 
   make_part(victim,"arm");
   if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
   {
    if ((obj = get_eq_char( victim, WEAR_ARMS )) != NULL)
     take_item(victim,obj);
   }
   if ((obj = get_eq_char( victim, WEAR_WIELD )) != NULL)
    take_item(victim,obj);
   if ((obj = get_eq_char( victim, WEAR_HANDS )) != NULL)
    take_item(victim,obj);
   if ((obj = get_eq_char( victim, WEAR_WRIST_R )) != NULL) 
    take_item(victim,obj);
   if ((obj = get_eq_char( victim, WEAR_FINGER_R )) != NULL)
    take_item(victim,obj);
   break;
  case 3:
   if (IS_LEG_L(victim,LOST_LEG)) 
    return;
   SET_BIT(victim->loc_hp[4],LOST_LEG);
   if (!IS_BLEEDING(victim,BLEEDING_LEG_L))
    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
   if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
    REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
   act("Acid turns $n's left leg into goo.", ch, NULL, NULL, TO_ROOM);
   stc("Acid turns your left leg into goo.", ch); 
   make_part(victim,"leg");
   if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
   {
    if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
     take_item(victim,obj);
   }
   if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
    take_item(victim,obj);
   break;
  case 4:
   if (IS_LEG_R(victim,LOST_LEG)) 
    return;
   SET_BIT(victim->loc_hp[5],LOST_LEG);
   if(!IS_BLEEDING(victim,BLEEDING_LEG_R))
    SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
   if(IS_BLEEDING(victim,BLEEDING_FOOT_R))
    REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
   act("Acid turns $n's right leg into goo.", ch, NULL, NULL, TO_ROOM);
   stc("Acid turns your right leg into goo.", ch); 
   make_part(victim,"leg");
   if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
   {
    if ((obj = get_eq_char( victim, WEAR_LEGS )) != NULL)
     take_item(victim,obj);
   }
   if ((obj = get_eq_char( victim, WEAR_FEET )) != NULL)
   take_item(victim,obj);
  default:
   break;
 }
 return;
}

void do_dragongain( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_STRING_LENGTH];
 int cost = 0;

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( arg[0] == '\0')
 {
  display_class_stats(ch);
  return;
 }
 else if( !str_cmp( arg, "might"))
 {
  improve_class(ch, DRAGON_MIGHT);
  return;
 }
 else if( !str_cmp( arg, "tough"))
 {
  improve_class(ch, DRAGON_TOUGH);
  return;
 }
 else if( !str_cmp( arg, "speed"))
 {
  improve_class(ch, DRAGON_SPEED);
  return;
 }
 else if( !str_cmp( arg, "magic"))
 {
  improve_class( ch, DRAGON_MAGIC);
  return;
 }
 else if( !str_cmp( arg, "age"))
 {
  improve_class( ch, DRAGON_AGE);
  return;
 }
 else if( !str_cmp( arg, "grab"))
 {
  cost = 50 * POINT_STEP;
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_GRAB))
  {
   stc("You already know how to grab someone.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the powers to learn to grab your victim.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_GRAB);
  stc("Your claws elongate so you can hold your opponent.\n\r", ch);
  return;
 } 
 else if( !str_cmp( arg, "wings"))
 {
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_WINGS))
  {
   stc("You already have dragon wings.\n\r", ch);
   return;
  }
  cost = 50 * POINT_STEP;
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the power to grow your wings.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_WINGS);
  stc("Wings sprout from your shoulders in full draconic majesty!\n\r", ch);
  act("Wings sprout from $n's shoulders in full draconic majesty!", ch, NULL, NULL, TO_ROOM);
  return;
 }
 else if( !str_cmp( arg, "fire"))
 {
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FIRE))
  {
   stc("You can already breathe fire.\n\r", ch);
   return;
  }
  if( get_breath(ch) >= 2)
  {
   stc("You can already breathe 2 kinds of dragonbreath.\n\r", ch);
   return;
  } 
  cost = 20000;
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the power to learn to breathe fire.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_FIRE);
  stc("Small gouts of flame shoot from your nostrils.\n\r", ch);
  act("Small gouts of flame shoot from $n's nostrils.", ch, NULL, NULL, TO_ROOM);
  return;
 }
 else if( !str_cmp( arg, "ice"))
 {
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ICE))
  {
   stc("You can already breathe ice.\n\r", ch);
   return;
  }
  if( get_breath(ch) > 2)
  {
   stc("You can already breathe 2 kinds of dragonbreath.\n\r", ch);
   return;
  } 
  cost = 20000;
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the power to learn to breathe ice.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_ICE);
  stc("Frost rims the outside of your mouth.\n\r", ch);
  act("Frost rims the outside of $n's mouth.", ch, NULL, NULL, TO_ROOM);
  return;
 }
 else if( !str_cmp( arg, "lightning"))
 {
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_LIGHTNING))
  {
   stc("You can already breathe lightning.\n\r", ch);
   return;
  }
  if( get_breath(ch) > 2)
  {
   stc("You can already breathe 2 kinds of dragonbreath.\n\r", ch);
   return;
  } 
  cost = 20000;
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the power to learn to breathe lightning.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_LIGHTNING);
  stc("Bolts of electricty course around your muzzle.\n\r", ch);
  act("Bolts of electricty course around $n's muzzle.", ch, NULL, NULL, TO_ROOM);
  return;
 }
 else if( !str_cmp( arg, "acid"))
 {
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ACID))
  {
   stc("You can already breathe acid.\n\r", ch);
   return;
  }
  if( get_breath(ch) > 2)
  {
   stc("You can already breathe 2 kinds of dragonbreath.\n\r", ch);
   return;
  } 
  cost = 20000;
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the power to learn to breathe acid.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_ACID);
  stc("Small drops of acid drip from your mouth.\n\r", ch);
  act("Small drops of acid drip from $n's mouth.", ch, NULL, NULL, TO_ROOM);
  return;
 }
 else if( !str_cmp( arg, "poison"))
 {
  if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_POISON))
  {
   stc("You can already breathe poison.\n\r", ch);
   return;
  }
  if( get_breath(ch) > 2)
  {
   stc("You can already breathe 2 kinds of dragonbreath.\n\r", ch);
   return;
  } 
  cost = 20000;
  if( GET_POWERS(ch, DRAGON_POINTS) < cost)
  {
   stc("You lack the power to learn to breathe poison.\n\r", ch);
   return;
  }
  GET_POWERS(ch, DRAGON_POINTS) -= cost;
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_POISON);
  stc("Swirls of poisonous gas rise from your nostrils.\n\r", ch);
  act("Swirls of poisonout gas rise from $n's nostrils.", ch, NULL, NULL, TO_ROOM);
  return;
 }
 else
 {
  stc("Syntax dragongain <field>.\n\r", ch);
  stc("Valid fields are: grab, might, tough, speed, magic, wings, fire, ice, poison, acid, lightning, age.\n\r", ch);
  return;
 }
 return;
}

void do_dbreath( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 int cost = 2000;
 int dam = number_range( 3000, 4500);
 int type;

 argument = one_argument( argument, arg1);
 argument = one_argument( argument, arg2);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to use a breathweapon.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, DRAGON_BTIME) > 0)
 {
  stc("You have yet to recover from your last breath.\n\r", ch);
  return;
 }
 if( ch->move < cost)
 {
  stc("You are too tired to use a breath weapon.\n\r", ch);
  return;
 }
 if( ch->mana < cost)
 {
  stc("You lack the magical energies to power your breath weapon.\n\r", ch);
  return;
 }
 if( arg2[0] == '\0')
 {
  stc("Breathe what?\n\r", ch);
  return;
 } 
 if( arg1[0] == '\0')
 {
  stc("Breathe onto whom?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg1)) == NULL)
 {
  stc("They don't seem to be here.\n\r", ch);
  return;
 }
 if( ch == victim)
 {
  stc("Not on yourself.\n\r", ch);
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
 if( !str_prefix( arg2, "fire"))
 {
  if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FIRE))
  {
   stc("You dont have the power to breathe fire.\n\r", ch);
   return;
  }
  else
   type = DRAGON_TYPE_FIRE;
 }
 else if( !str_prefix( arg2, "lightning"))
 {
  if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_LIGHTNING))
  {
   stc("You dont have the power to breathe lightning.\n\r", ch);
   return;
  }
  else
   type = DRAGON_TYPE_LIGHTNING;
 }
 else if( !str_prefix( arg2, "acid"))
 {
  if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ACID))
  {
   stc("You dont have the power to breathe acid.\n\r", ch);
   return;
  }
  else
   type = DRAGON_TYPE_ACID;
 }
 else if( !str_prefix( arg2, "poison"))
 {
  if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_POISON))
  {
   stc("You dont have the power to breathe poison.\n\r", ch);
   return;
  }
  else
   type = DRAGON_TYPE_POISON;
 }
 else if( !str_prefix( arg2, "ice"))
 {
  if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ICE))
  {
   stc("You dont have the power to breathe ice.\n\r", ch);
   return;
  }
  else
   type = DRAGON_TYPE_ICE;
 }
 else
 {
  stc("That is not a valid breath type.\n\r", ch);
  return;
 }
 switch( type)
 {
  case DRAGON_TYPE_FIRE:
   act2("You surround $N in a cone of searing flame. [$D]", ch, NULL, victim, TO_CHAR, dam);
   act2("$n surrounds you in a cone of searing flame. [$D]", ch, NULL, victim, TO_VICT, dam);
   act2("$n surrounds $N in a cone of searing flame. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
   break;
  case DRAGON_TYPE_LIGHTNING:
   act2("You blast $N with a bolt of lightning. [$D]", ch, NULL, victim, TO_CHAR, dam);
   act2("$n blasts you with a bolt of lightning. [$D]", ch, NULL, victim, TO_VICT, dam);
   act2("$n blasts $N with a bolt of lightning. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
   break;
  case DRAGON_TYPE_ACID:
   act2("You blast $N with a stream of bubbling acid. [$D]", ch, NULL, victim, TO_CHAR, dam);
   act2("$n blasts you with a stream of bubbling acid. [$D]", ch, NULL, victim, TO_VICT, dam);
   act2("$n blasts $N with a stream of bubbling acid. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
   break;
  case DRAGON_TYPE_POISON:
   act2("You spray $N with a noxious green gas. [$D]", ch, NULL, victim, TO_CHAR, dam);
   act2("$n sprays you with a noxious green gas. [$D]", ch, NULL, victim, TO_VICT, dam);
   act2("$n sprays $N with a noxious green gas. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
   break;
  case DRAGON_TYPE_ICE:
   act2("You cover $N with a layer of frigid ice. [$D]", ch, NULL, victim, TO_CHAR, dam);
   act2("$n covers you with a layer of frigid ice. [$D]", ch, NULL, victim, TO_VICT, dam);
   act2("$n covers $N with a layer of frigid ice. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
   break;
  default:
   break;
 }
 hurt_person( ch, victim, dam);
 dragon_breath_affect(ch, victim, type);
 WAIT_STATE(ch, 15);
 GET_POWERS(ch, DRAGON_BTIME) = 5;
 if( ch->fighting == NULL)
  set_fighting( ch, victim);
 if( victim->fighting == NULL)
  set_fighting( victim, ch);
 return;
}

void do_dragon_shift( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Shift into dragon or human form?\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, DRAGON_STIME) > 0)
 {
  stc("You are still tired from your last shift in form.\n\r", ch);
  return;
 }
 if( !str_cmp(arg, "human"))
 {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_HUMAN)
   {
    stc("You are already in human form.\n\r", ch);
    return;
   }
   if( ch->pcdata->held != NULL
    && ch->pcdata->held[0] != '\0')
   {
    if( (victim = get_char_world( ch, ch->pcdata->held)) != NULL)
     if( !IS_NPC(victim))
      victim->pcdata->held_by = str_dup( "" );
    ch->pcdata->held = str_dup( "" );
   }
   stc("Your body shifts back into human form.\n\r", ch);
   GET_POWERS(ch, DRAGON_FORM) = DRAGON_FORM_HUMAN;
   GET_POWERS(ch, DRAGON_STIME) += 20;
   return;
 }
 else if( !str_cmp(arg, "dragon"))
 {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   {
    stc("You are already in dragon form.\n\r", ch);
    return;
   }
   stc("Your body shifts into dragon form.\n\r", ch);
   if( ch->stance[0] != -1 )
    do_stance(ch,"");
   ch->hit += (ch->max_hit * 0.10);
   if( ch->hit > ch->max_hit)
     ch->hit = ch->max_hit;
   GET_POWERS(ch, DRAGON_FORM) = DRAGON_FORM_DRAGON;
   if( IS_SET(ch->act, PLR_HOLYLIGHT) )
    do_remove( ch, "all");
   else
   {
    SET_BIT(ch->act, PLR_HOLYLIGHT);
    do_remove( ch, "all");
    REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
   }
   return;
 }
 else
 {
  stc("Shift into a human or dragon form?\n\r", ch);
  return;
 }
 return;
}

void do_dragon_fly( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh( ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to fly.\n\r", ch);
  return;
 }
 if( !IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_WINGS))
 {
  stc("You can't fly without wings.\n\r", ch);
  return;
 }
 if( ch->fighting != NULL)
 {
  stc("You can't take flight while fighting.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY))
 {
  REMOVE_BIT(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY);
  stc("You land gracefully and tuck your wings in.\n\r", ch);
  return;
 }
 else
 {
  if( IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
  {
   stc("You can't take flight while your indoors.\n\r", ch);
   return;
  }
  SET_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_FLY);
  stc("You spread your wings and take to the skys.\n\r", ch);
  return;
 }
 return;
}

void do_dscan( CHAR_DATA *ch, char *argument)
{
 ROOM_INDEX_DATA *room;
 AREA_DATA *area;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to scan.\n\r", ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY))
 {
  stc("You must be flying to scan the world.\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("You cant seem to find them.\n\r", ch);
  return;
 }
 if( (area = victim->in_room->area) == NULL)
 {
  stc("They are in an area that doesnt exist.\n\r", ch);
  return;
 }
 if( ch->move < 3000 )
 {
  stc("You are too tired to scan for your prey.\n\r",ch);
  return;
 }
 if( IS_SET(victim->in_room->room_flags, ROOM_INDOORS) )
 {
   sprintf( arg, "You pick up the tracks of $N who is hiding indoors in %s.", area->name);
   act( arg, ch, NULL, victim, TO_CHAR);
   ch->move -= 1500;
 }
 else
 {
   room = ch->in_room;
   char_from_room(ch);
   char_to_room(ch, victim->in_room);
   if( IS_SET(ch->affected_by, AFF_SHADOWPLANE) )
   {
	REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	do_look(ch, "auto");
	SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
   }
   else
	do_look(ch, "auto");
   char_from_room(ch);
   char_to_room(ch, room);
   ch->move -= 3000;
 }
 if( !IS_NPC(victim) )      
 {
  if( IS_CLASS(victim,CLASS_MONK) && HAS_TECH(victim,TECH_CLAIR) )
   stc("You sense that someone is watching you.\n\r",victim);
  else if( IS_CLASS(victim,CLASS_JEDI) )
   stc("You sense movement in the force.\n\r",victim);
  else if( IS_CLASS(victim,CLASS_APOCRYPHIC) && standard_calls(ch,SCHOOL_ARCANE,8) )
   stc("You feel a magical presence watching you.\n\r",victim);
 }
 WAIT_STATE(ch, 8);
 return;
} 

void do_snatch( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh( ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to snatch someone.\n\r", ch);
  return;
 }
 argument = one_argument( argument, arg);
 if( arg[0] == '\0')
 {
  stc("Snatch who?\n\r", ch);
  return;
 }
 if( ch->fighting != NULL)
 {
  stc("You can't snatch someone while fighting.\n\r", ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY))
 {
  stc("You can't snatch if you're not flying.\n\r", ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_GRAB))
 {
  stc("You have yet to learn how to snatch someone from the ground.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("You can't seem to find them.\n\r", ch);
  return;
 }
 if( ch == victim)
 {
  stc("Not on yourself.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( victim->pcdata->held_by == NULL)
  victim->pcdata->held_by = str_dup( "" ); 
 if( victim->pcdata->held_by[0] != '\0')
 {
  stc("They are already being held.\n\r", ch);
  return;
 }
 if( ch->pcdata->held == NULL)
   ch->pcdata->held = str_dup( "" );
 if( ch->pcdata->held[0] != '\0')
 {
  stc("You already have someone held in your hands.\n\r", ch);
  return;
 }
 if( is_ds_range(ch) ) return;
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
// char_from_room(ch);
// char_to_room( ch, victim->in_room);
 act("You swoop down upon $N and snatch $M into the air.", ch, NULL, victim, TO_CHAR);
 act("$n swoops down upon you and snatches you into the air.", ch, NULL, victim, TO_VICT);
 act("$n swoops down upon $N and snatches $M into the air.", ch, NULL, victim, TO_NOTVICT);
 free_string(ch->pcdata->held);
 ch->pcdata->held = str_dup( victim->name);
 free_string(victim->pcdata->held_by);
 victim->pcdata->held_by = str_dup( ch->name);
 ch->pcdata->pulse_timer[PULSE_GRAB] += 12;
 ch->fight_timer += 15;
 if( ch->fight_timer > 30 )
	ch->fight_timer = 30;
 WAIT_STATE(ch, 10);
 stop_fighting( ch, TRUE);
 stop_fighting( victim, TRUE);
 return;
}

void do_grab( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh( ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to grab someone.\n\r", ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_GRAB))
 {
  stc("You have yet to learn how to grab an opponent.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY) )
 {
  stc("While you're flying, you must snatch your prey.\n\r", ch);
  return;
 }
 argument = one_argument( argument, arg);
 if( arg[0] == '\0')
 {
  stc("Grab who?\n\r", ch);
  return;
 }
 if( ch->pcdata->held[0] != '\0')
 {
  stc("You already have someone clutched in your claws.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("You can't seem to find them.\n\r", ch);
  return;
 }
 if( ch == victim)
 {
  stc("Not on yourself.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( victim->pcdata->held_by == NULL)
  victim->pcdata->held_by = str_dup( "" ); 
 if( victim->pcdata->held_by[0] != '\0')
 {
  stc("They are already being held.\n\r", ch);
  return;
 }
 if( ch->pcdata->held[0] != '\0')
 {
  stc("You already have someone held in your hands.\n\r", ch);
  return;
 }
 if( is_ds_range(ch) ) return;
 if( is_safe(ch, victim))
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 act("You grab $N in your powerful claws.", ch, NULL, victim, TO_CHAR);
 act("$n reaches down and grabs you with $s poweful claws.", ch, NULL, victim, TO_VICT);
 act("$n reaches down and grabs $N with $s powerful claws.", ch, NULL, victim, TO_NOTVICT);
 free_string(ch->pcdata->held);
 ch->pcdata->held = str_dup( victim->name);
 free_string( victim->pcdata->held_by);
 victim->pcdata->held_by = str_dup( ch->name);
 ch->pcdata->pulse_timer[PULSE_GRAB] += 10;
 WAIT_STATE(ch, 10);
 return;
}

void do_plummet( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *vict;
 CHAR_DATA *v_ch;
 CHAR_DATA *v_next;
 char arg[MAX_INPUT_LENGTH];
 int dam = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to plummet.\n\r", ch);
  return;
 }
 if( !IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY))
 {
  stc("You must be flying to plummet.\n\r", ch);
  return;
 }
 if( ch->fighting != NULL)
 {
  stc("You can't plummet while fighting.\n\r", ch);
  return;
 }
 argument = one_argument( argument, arg);
 if( arg[0] == '\0')
 {
  stc("Plummet onto who?\n\r", ch);
  return;
 }
 if( (vict = get_char_area(ch, arg)) == NULL)
 {
  stc("You can't seem to find them.\n\r", ch);
  return;
 }
 if( vict == ch )
 {
  stc("Not on yourself.\n\r",ch);
  return;
 }
 if( IS_SET(vict->in_room->room_flags, ROOM_INDOORS))
 {
  stc("You can't plummet on someone indoors.\n\r", ch);
  return;
 }
 if( IS_SET(ch->in_room->room_flags, ROOM_INDOORS))
 {
  stc("You can't plummet while your indoors.\n\r", ch);
  return;
 }
 char_from_room(ch);
 char_to_room(ch, vict->in_room);
 for( v_ch = vict->in_room->people; v_ch; v_ch = v_next)
 {
 v_next = v_ch->next_in_room;
  if( ch == v_ch)
   continue;
  if( is_safe(ch, v_ch))
   continue;
  dam = number_range(1500, 2500);
  act2("$n plummets from the sky and lands on you! [$D]", ch, NULL, v_ch, TO_VICT, dam);
  act2("$n plummets from the sky and lands on $N! [$D]", ch, NULL, v_ch, TO_NOTVICT, dam);
  act2("You plummet from the sky and land on $N! [$D]", ch, NULL, v_ch, TO_CHAR, dam);
  hurt_person( ch, v_ch, dam);
  WAIT_STATE(v_ch, 10);
 }
 FIGHT_TIMER(ch, 10);
 dam = number_range( 500, 1500);
 act2("Your plummet from the skies rattles your bones.[$D]", ch, NULL, NULL, TO_CHAR, dam);
 act2("$n looks stunned and hurt from $s plummet. [$D]", ch, NULL, v_ch, TO_ROOM, dam);
 hurt_person( ch, ch, dam);
 REM_BIT_POWERS(ch, DRAGON_FLAGS, DRAGON_FLY);
 WAIT_STATE(ch, 20);
 return;
}

void do_ddrop( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam = number_range(1000,2000);
 
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh( ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to drop someone.\n\r", ch);
  return;
 }
 if( ch->pcdata->held == NULL || ch->pcdata->held[0] == '\0')
 {
  stc("Your not holding anyone to drop.\n\r", ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY))
 {
  stc("You must be flying to drop someone.\n\r", ch);
  return;
 }
 victim = get_char_world( ch, ch->pcdata->held);
 if( victim == NULL)
 {
   stc("BUG, you are holding a null character, fixing.\n\r", ch);
   ch->pcdata->held = str_dup( "");
   return;
 }
 if( IS_NPC(victim))
 {
   stc("Can't drop npc's.\n\r", ch);
   ch->pcdata->held = str_dup( "" );
   return;
 }
 if( victim->in_room != ch->in_room)
 {
   char_from_room( victim);
   char_to_room(victim, ch->in_room);
 }
 act2("You drop $N and watch $M flail $S arms in panic. [$D]", ch, NULL, victim, TO_CHAR, dam);
 act2("You flail your arms in panic as $n drops you from a lofty height. [$D]", ch, NULL, victim,
   TO_VICT, dam);
 act2("$N flails $S arms in panic as $n drops $M from a lofty height. [$D]", ch, NULL, victim,
   TO_NOTVICT, dam);
 hurt_person( ch, victim, dam);
 FIGHT_TIMER(ch, 10);
 ch->pcdata->pulse_timer[PULSE_GRAB] = 0;
 ch->pcdata->held = str_dup(  "" );
 victim->pcdata->held_by = str_dup( "" );
 WAIT_STATE(ch, 10);
 WAIT_STATE(victim, 10);
 return;
}

void do_dthrow( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA       *victim;
 int dam = number_range( 1500, 2500);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to throw someone.\n\r", ch);
  return;
 }
 if( ch->pcdata->held == NULL || ch->pcdata->held[0] == '\0')
 {
  stc("You must be holding someone to throw them.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY))
 {
  stc("You can't throw while flying.\n\r", ch);
  return;
 }
 victim = get_char_world(ch, ch->pcdata->held);
 if( victim == NULL)
 {
  stc("You seem to be holding a NULL character.\n\r", ch);
  ch->pcdata->held = str_dup( "" );
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  ch->pcdata->held = str_dup( "" );
  return;
 }
 if( victim->in_room != ch->in_room)
 {
  char_from_room(victim);
  char_to_room(victim, ch->in_room);
 }
 act2("You throw $N to the ground with concussive force. [$D]", ch, NULL, victim, TO_CHAR, dam);
 act2("$n throws you to the ground with concussive force. [$D]", ch, NULL, victim, TO_VICT, dam);
 act2("$n throws $N to the ground with concussive force. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
 ch->pcdata->pulse_timer[PULSE_GRAB] = 0;
 hurt_person(ch, victim, dam);
 victim->pcdata->held_by = str_dup( "" );
 ch->pcdata->held = str_dup( "" );
 WAIT_STATE(ch, 10);
 WAIT_STATE(victim, 8);
 return;
}

void do_drelease( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to release someone.\n\r", ch);
  return;
 }
 if( ch->pcdata->held == NULL || ch->pcdata->held[0] == '\0')
 {
  if( ch->hit > 0 && ch->fighting == NULL )
  	stc("You're not holding anyone to set down.\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, ch->pcdata->held)) == NULL)
 {
  stc("You're holding a NULL character.\n\r", ch);
  ch->pcdata->held = str_dup( "" );
  return;
 }
 if( IS_NPC(victim))
 {
  stc("How are you holding an npc?\n\r", ch);
  ch->pcdata->held = str_dup( "" );
  return;
 }
 if( ch->in_room->vnum >= 26000 && ch->in_room->vnum <= 26206 )
 {
  char_from_room(ch);
  char_to_room(ch, victim->in_room);
  stc("Everything gets screwed up in your subconscious and... what the hell?\n\r", ch);
 }
 if( victim->in_room != ch->in_room)
 {
  char_from_room(victim);
  char_to_room(victim, ch->in_room);
 }
 if( ch->pcdata->pulse_timer[PULSE_GRAB] > 1 && ch->fighting == NULL && victim->fighting == NULL )
 {
   act("You gently set $N back onto the ground.", ch, NULL, victim, TO_CHAR);
   act("$n gently sets you back onto the ground.", ch, NULL, victim, TO_VICT);
   act("$n gently sets $N back onto the ground.", ch, NULL, victim, TO_NOTVICT);
   ch->pcdata->pulse_timer[PULSE_GRAB] = 0;
 }
 else
 {
   act("$N has escaped your grasp!", ch, NULL, victim, TO_CHAR);
   act("You have escaped $n's grasp!", ch, NULL, victim, TO_VICT);
   act("$N has escaped $n's grasp!", ch, NULL, victim, TO_NOTVICT);
   ch->pcdata->pulse_timer[PULSE_GRAB] = 0;
 }
 WAIT_STATE(ch,8);
 ch->pcdata->held = str_dup( "" );
 victim->pcdata->held_by = str_dup( "" );
 return;
}

void do_highclaw( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh( ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to claw someone.\n\r", ch);
  return;
 }
 if( ch->fighting == NULL)
 {
  stc("You must be fighting to highclaw.\n\r", ch);
  return;
 }
 if( (victim = ch->fighting) == NULL)
 {
  stc("You are fighting a NULL pointer.\n\r", ch);
  return;
 }
 dam = GET_POWERS(ch, DRAGON_MIGHT) * number_range( 150, 225);
 act2("You slash deep gashes into $N with your sharp claws. [$D]", ch, 
NULL, victim, TO_CHAR, dam);
 act2("$n slashes deep gashes into you with $s sharp claws. [$D]", ch, 
NULL, victim, TO_VICT, dam);
 act2("$n slashes deep gashes into $N with $s sharp claws. [$D]", ch, 
NULL, victim, TO_NOTVICT, dam);
 hurt_person(ch, victim, dam);
 WAIT_STATE(ch, 12);
 return;
}

void do_tailslap( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 int dam = 0;

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_DRAGON))
 {
  do_huh( ch, "");
  return;
 }
 if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
 {
  stc("You must be in dragonform to tailslap someone.\n\r", ch);
  return;
 }
 if( ch->fighting == NULL)
 {
  stc("You must be fighting to tailslap.\n\r", ch);
  return;
 }
 if( (victim = ch->fighting) == NULL)
 {
  stc("You are fighting a NULL pointer.\n\r", ch);
  return;
 }
 dam = GET_POWERS(ch, DRAGON_MIGHT) * number_range( 200, 300);
 act2("You knock the air from $N's lungs with your powerful tail. [$D]", 
   ch, NULL, victim, TO_CHAR, dam);
 act2("$n knocks the air from your lungs with $s powerful tail. [$D]", 
   ch, NULL, victim, TO_VICT, dam);
 act2("$n knocks the air from $N's lungs with $s powerful tail. [$D]", 
   ch, NULL, victim, TO_NOTVICT, dam);
 hurt_person(ch, victim, dam);
 WAIT_STATE(ch, 18);
 return;
}

void do_wingbuffet( CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];

	if( IS_NPC(ch))
 		return;
 	if( !IS_CLASS(ch, CLASS_DRAGON))
 	{
  		do_huh( ch, "");
  		return;
 	}
 	if( !IS_SET( GET_POWERS(ch, DRAGON_FLAGS), DRAGON_WINGS))
 	{
  		stc("You can't wingbuffet without wings.\n\r", ch);
  		return;
 	}
	if( ch->fighting == NULL )
	{
		argument = one_argument( argument, arg);
		if( arg[0] == '\0')
		{
			stc("Wingbuffet who?\n\r",ch);
			return;
		}
		if( (victim = get_char_room( ch, arg)) == NULL)
		{
			stc("They are not here.\n\r", ch);
			return;
		}
	}
	else
		victim = ch->fighting;
	if( is_safe(ch,victim) )
	{
		stc("Not while safe.\n\r",ch);
		return;
	}/*
	if( IS_NPC(victim))
	{
		stc("You can't wingbuffet an NPC.\n\r", ch);
		return;
	}
	if( number_range(1,100) >= 61 )
	{
		sprintf( arg, "Your wingbuffet failed to hit %s.\n\r", victim->name);
		stc(arg, ch);
		sprintf( arg, "%s attempts to hit you with a wingbuffet and failed.\n\r", ch->name);
		stc(arg, victim);
		sprintf( arg, "%s attempts to hit %s with a wingbuffet and failed.\n\r", ch->name, victim->name);
		act(arg, ch, NULL, victim, TO_NOTVICT);
		WAIT_STATE(ch, 10);
		return;
	}
	else
	{*/
		sprintf( arg, "Your wingbuffet disrupts %s's concentration of the fight.\n\r", victim->name);
		stc(arg, ch);
		sprintf( arg, "%s's wingbuffet disrupts your concentration of the fight.\n\r", ch->name);
		stc(arg, victim);
		sprintf( arg, "%s's winbuffet disrupts %s's concentration of the fight.\n\r", ch->name, victim->name);
		act( arg, ch, NULL, victim, TO_NOTVICT);
		victim->stance[0] = STANCE_NONE;
		WAIT_STATE(ch, 10);
		WAIT_STATE(victim, 8);
		return;
	//}
	return;
}
