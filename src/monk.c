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

/***********************************************************************************
 ***********************************************************************************
 *                                                                                 *
 *  Monks in the style of old Godwars: Unplugged there are 9 combos with 5         *
 *  chained combos. all of which can be seen in the fucntion combo_exec().         *
 *                                                                                 *
 ***********************************************************************************
 ***********************************************************************************/

void gain_custom_combo(CHAR_DATA *ch);
void combo_exec(CHAR_DATA *ch, CHAR_DATA *victim, int combo);

/* Drow fix --Kline */
void do_drow_grant args(( CHAR_DATA *ch, char *argument ));

/***********************************
 ***********************************
 *                                 *
 * This function is used to raise  *
 * the skill level of custom combo *
 * for the monk.  Used after a     *
 * successful combo.               *
 *                                 *
 ***********************************
 ***********************************/ 
void gain_custom_combo(CHAR_DATA *ch)
{
 if( !IS_NPC(ch) 
   && IS_CLASS(ch, CLASS_MONK) 
   && GET_POWERS(ch, MONK_CUSTOM) < MAX_CUSTOM)
   {
    GET_POWERS(ch, MONK_CUSTOM)++;
    stc("#7You have become more proficient in your combat maneuvers.#n\n\r", ch);
    return;
   }  
 return;
}

/*************************************
 *************************************
 *                                   *
 * Simply counts how many techniques *
 * a monk as learned.                *
 *                                   *
 *************************************
 *************************************/
int count_techniques(CHAR_DATA *ch)
{
 int value = 0;
 
 if( IS_NPC(ch))
  return 0;
 if( !IS_CLASS(ch, CLASS_MONK))
  return 0;
 if( HAS_TECH(ch,TECH_ELBOW))
  value++;
 if( HAS_TECH(ch, TECH_BACKFIST))
  value++; 
 if( HAS_TECH(ch, TECH_SWEEP))
  value++;  
 if( HAS_TECH(ch, TECH_REVERSE))
  value++; 
 if( HAS_TECH(ch, TECH_KNEETHRUST))
  value++;  
 if( HAS_TECH(ch, TECH_THRUSTKICK))
  value++;  
 if( HAS_TECH(ch, TECH_ROUNDHOUSE))
  value++; 
 if( HAS_TECH(ch, TECH_HEARTPUNCH))
  value++; 
 if( HAS_TECH(ch, TECH_PALMSTRIKE))
  value++;
 if( HAS_TECH(ch, TECH_DENJIN))
   value++;   
 if( HAS_TECH(ch, TECH_CUSTOM))
   value++;  
 if( HAS_TECH(ch, TECH_VOICE))
   value++;    
 if( HAS_TECH(ch, TECH_BLOCK))
   value++;
 if( HAS_TECH(ch, TECH_CELEST))
   value++;
 if( HAS_TECH(ch, TECH_STANCE))
   value++; 
 if( HAS_TECH(ch,TECH_UNARMED))
   value++; 
 if( HAS_TECH(ch,TECH_HEALTOUCH))
    value++;
 if( HAS_TECH(ch,TECH_DEATHTOUCH))
   value++; 
 if( HAS_TECH(ch,TECH_HEATMETAL))
  value++;
 if( HAS_TECH(ch,TECH_CLAIR))
  value++;
 return value;
}

bool has_weapons( CHAR_DATA *ch)
{
 OBJ_DATA *obj;
 
 if( (obj = get_eq_char( ch, WEAR_WIELD)) != NULL)
  return TRUE;
 else if( (obj = get_eq_char(ch, WEAR_HOLD)) != NULL)
  return TRUE;
 else 
  return FALSE;
}

void do_voice(CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 CHAR_DATA *vict_next;
 int cost = 30;
 int dam = 0;
 static const char *message[5] =
 {"The clouds overhead begin to rumble and boil as lightning courses
   through the heavens.  The black clouds part as the enourmous dragon
   Bahamut spreads his wings for a landing to aid those that have called
   him.",
  "A thin mist begins to surround you as an a loud howl can be heard
   a few feet a way.  In a flash of white, an enourmous wolf stands before
   you, with yellow fangs showing.  The wolf lets loose another thunderous
   howl and begins his attack.",
  "The clouds part and singing can be heard from the heavens as a large
   winged figure streaks from sky.  With impossible grace, the archangel
   Michael lands before $n.  Michael raises his hand as it begins to glow
   with the power of heaven, and he touches $n on the head.  $n has been
   healed.",
  "Lightning streaks from the heavens as the Archangel Michael bolts
   from the skies to land before $N.  With a nod to $n, Michael raises
   his sword high and strikes $N.",
  "A sharp wind beings to blow from the north as the air becomes
   tainted with stench of death.  The wind born degris stings your
   eyes as a large hulking figure appears from nowhere.  With a red
   glint in his eyes, Efreet sucks the vitality our of $N and give it
   to $n."
 };

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Voice what?\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("Your mind lacks the peace and focus required.\n\r", ch);
  return;
 }
 if( !str_cmp( arg, "bahamut"))
 {
  if( !IS_SET(GET_POWERS(ch, MONK_BITS), MONK_BAHAMUT))
  {
   stc("Bahamut has not been granted to you.\n\r", ch);
   return;
  }
  if( !IS_OUTSIDE(ch))
  {
   stc("You must be outdoor.\n\r", ch);
   return;
  }
  if( ch->fighting == NULL)
  {
   stc("Bahamut will not answer your call unless you are in distress.\n\r", ch);
   return;
  }
  act(message[0], ch, NULL, NULL, TO_ALL);
  REMOVE_BIT(GET_POWERS(ch, MONK_BITS), MONK_BAHAMUT);
  for( victim = ch->in_room->people; victim; victim = vict_next)
  {
   vict_next = victim->next_in_room;
   if( victim == ch)
    continue;
   if( victim->fighting == NULL)
    continue;
   if( victim->fighting != ch)
    continue;
   dam = number_range(4000, 6000);
   act2("Bahamut breathes a gout of flame on $N. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
   act2("Bahamut breathes a gout of flame on you. [$D]", ch, NULL, victim, TO_VICT, dam);
   act2("Bahamut breathes a gout of flame on $N. [$D]", ch, NULL, victim, TO_CHAR, dam);
   hurt_person(ch, victim, dam);
  }
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  WAIT_STATE(ch, MONK_WAIT);
  return;
 }
 else if( !str_cmp( arg, "wolf"))
 {
  if( !IS_SET(GET_POWERS(ch, MONK_BITS), MONK_WOLF))
  {
   stc("You have not been granted the power to summon the spirit of the wolf.\n\r", ch);
   return;
  }
  if( ch->fighting == NULL)
  {
   stc("The spirit of the wolf refuses to aid you while not in danger.\n\r", ch);
   return;
  }
  REMOVE_BIT(GET_POWERS(ch, MONK_BITS), MONK_WOLF);
  act(message[1], ch, NULL, NULL, TO_ALL);
  dam = number_range(2500, 3500);
  victim = ch->fighting;
  act2("The Large White Wolf attacks $N. [$D]", ch, NULL, victim, TO_CHAR, dam);
  act2("The Large White Wolf attacks $N. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
  act2("The Large White Wolf attacks you. [$D]", ch, NULL, victim, TO_VICT, dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  WAIT_STATE(ch, MONK_WAIT);
  return;
 }
 else if( !str_cmp( arg, "michael"))
 {
  if( !IS_SET(GET_POWERS(ch, MONK_BITS), MONK_MICHAEL))
  {
   stc("You have not been granted the use of Michael.\n\r", ch);
   return;
  }
  dam = number_range( 2000, 3000);
  REMOVE_BIT(GET_POWERS(ch, MONK_BITS), MONK_MICHAEL);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  WAIT_STATE(ch, MONK_WAIT);
  if( ch->fighting == NULL)
  {
   act(message[2], ch, NULL, victim, TO_ALL);
   ch->hit += dam;
   if( ch->hit > ch->max_hit)
    ch->hit = ch->max_hit;
   return;
  }
  else
  {
   victim = ch->fighting;
   act(message[3], ch, NULL, victim, TO_ALL);
   act2("Michael takes a mighty slash at $n with his huge sword.", victim, NULL, NULL, TO_CHAR, dam);
   hurt_person(ch, victim, dam);
   return;
  }
 }
 else if( !str_cmp( arg, "bear"))
 {
  if( !IS_SET(GET_POWERS(ch, MONK_BITS), MONK_BEAR))
  {
   stc("You have not been granted use of the spririt of the Bear.\n\r", ch);
   return;
  }
  if( ch->fighting == NULL)
  {
   stc("The spirit of the bear refuses to aid you while you're not in danger.\n\r", ch);
   return;
  }
  REMOVE_BIT( GET_POWERS(ch, MONK_BITS), MONK_BEAR);
  act("$n begins to glow as $e is surrounded by the spirit of the bear.", ch, NULL, NULL, TO_ROOM);
  act("You begin to glow with the spirit of the bear.", ch, NULL, NULL, TO_CHAR);
  ch->pcdata->pulse_timer[PULSE_BEAR] += 5;
  WAIT_STATE(ch, MONK_WAIT);
  return;
 }
 else if( !str_cmp( arg, "efreet"))
 {
  if( !IS_SET(GET_POWERS(ch, MONK_BITS), MONK_EFREET))
  {
   stc("You have not been granted the use of Efreet.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   stc("Efreet will only come in the midst of combat.\n\r", ch);
   return;
  }
  dam = UMIN(victim->move, number_range(3000,5000));
  REMOVE_BIT(GET_POWERS(ch, MONK_BITS), MONK_EFREET);
  act(message[4], ch, NULL, victim, TO_ALL);
  victim->move -= dam;
  ch->move += dam;
  WAIT_STATE(ch, 10);
  return;
 }
 else
 {
  stc("That is not a voice.\n\r", ch);
  return;
 }
 return;
}

void do_grant( CHAR_DATA *ch, char *argument)
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int bit;
 int cost = 75;
 
 if( IS_CLASS(ch,CLASS_DROW) )
 {
  do_drow_grant(ch,argument);
  return;
 }

 argument = one_argument( argument, arg1);
 argument = one_argument( argument, arg2);
 
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( arg1[0] == '\0')
 {
  stc("Grant who what?\n\r", ch);
  return;
 }
 if( arg2[0] =='\0')
 {
  stc("Grant what?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg1)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on npc's.\n\r", ch);
  return;
 }
 if( !IS_CLASS(victim, CLASS_MONK))
 {
  stc("They are not a monk.\n\r", ch);
  return;
 }
 if( !str_cmp( arg2, "michael"))
 {
   bit = MONK_MICHAEL;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_PEARL)
  {
   stc("Only a pearl monk can grant the aid of Michael.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of Michael.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of Michael.\n\r", victim);
  stc("You have granted them the use of Michael.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return;
 }
 else if( !str_cmp( arg2, "bear"))
 {
   bit = MONK_BEAR;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_GAIA)
  {
   stc("Only a gaia monk can grant the aid of the Bear.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of the Bear.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of the Bear.\n\r", victim);
  stc("You have granted them the use of the Bear.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return; 
 }  
 else if( !str_cmp( arg2, "bahamut"))
 {
   bit = MONK_BAHAMUT;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_INFERNO)
  {
   stc("Only an inferno monk can grant the aid of Bahamut.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of Bahamut.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of Bahamut.\n\r", victim);
  stc("You have granted them the use of Bahamut.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return;
 }
 else if( !str_cmp( arg2, "fido"))
 {
   bit = MONK_FIDO;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_GAIA)
  {
   stc("Only a gaia monk can grant the aid of Fido.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of Fido.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of Fido.\n\r", victim);
  stc("You have granted them the use of Fido.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return;
 }
 else if( !str_cmp( arg2, "mistdragon"))
 {
   bit = MONK_MIST;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_TSUNAMI)
  {
   stc("Only a tsunami monk can grant the aid of the Mist Dragon.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of the Mist Dragon.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of the Mist Dragon.\n\r", victim);
  stc("You have granted them the use of the Mist Dragon.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return;
 }
 else if( !str_cmp( arg2, "efreet"))
 {
   bit = MONK_EFREET;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_TEMPEST)
  {
   stc("Only a tempest monk can grant the aid of the Efreet.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of the Efreet.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of the Efreet.\n\r", victim);
  stc("You have granted them the use of the Efreet.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return;
 }
 else if( !str_cmp( arg2, "wolf"))
 {
   bit = MONK_WOLF;
  if( GET_POWERS(ch, MONK_CLASS) != PATH_GAIA)
  {
   stc("Only a gaia monk can grant the aid of the Wolf.\n\r", ch);
   return;
  }
  if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
  {
   stc("Your mind is not focused enough.\n\r", ch);
   return;
  }
  if( IS_SET(GET_POWERS(victim, MONK_BITS), bit))
  {
   stc("They already have the aid of the Wolf.\n\r", ch);
   return;
  }
  SET_BIT(GET_POWERS(victim, MONK_BITS),bit);
  stc("You have been granted the use of the Wolf.\n\r", victim);
  stc("You have granted them the use of the Wolf.\n\r", ch);
  GET_POWERS(ch, MONK_CURR_FOC) -= cost;
  return;
 }
 else
 {
  stc("Valid options are: michael. bear, wolf, fido, mistdragon, efreet, bahamut.\n\r", ch);
  return;
 }
 return;
}

void do_path( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 
 argument = one_argument(argument, arg);
 
 if(IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, MONK_CLASS) > 0)
 {
  stc("You have already set your path.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Which path would you like to walk?\n\r", ch);
  stc("Current valid paths are:\n\r", ch);
  stc("Pearl, Gaia, Inferno and Tempest\n\r", ch);
  return;
 }
 if( !str_cmp( arg, "inferno"))
 {
  stc("You are now an inferno monk.\n\r", ch);
  GET_POWERS(ch, MONK_CLASS) = PATH_INFERNO;
  return;
 }
 else if( !str_cmp( arg, "tsunami"))
 {
  stc("You are now a tsunami monk.\n\r", ch);
  GET_POWERS(ch, MONK_CLASS) = PATH_TSUNAMI;
  return;
 }
 else if( !str_cmp( arg, "pearl"))
 {
  stc("You are now a pearl monk.\n\r", ch);
  GET_POWERS(ch, MONK_CLASS) = PATH_PEARL;
  return;
 }
 else if( !str_cmp( arg, "tempest"))
 {
  stc("You are now a tempest monk.\n\r", ch);
  GET_POWERS(ch, MONK_CLASS) = PATH_TEMPEST;
  return;
 }
 else if( !str_cmp( arg, "gaia"))
 {
  stc("You are now a gaia monk.\n\r", ch);
  GET_POWERS(ch, MONK_CLASS) = PATH_GAIA;
  return;
 }
 else
 {
  stc("Choices are: pearl, tsunami, inferno,tempest, gaia.\n\r", ch);
  return;
 }
 return;
}
 
/***********************************************
 ***********************************************
 *                                             *
 * Heatmetal sets a timer that causes small    *
 * amount of damage and randomly removes eq    *
 * from the victim.                            *
 *                                             *
 ***********************************************
 ***********************************************/
void do_heatmetal( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 30;

 argument = one_argument( argument, arg);
 
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( has_weapons(ch))
 {
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( !HAS_TECH(ch, TECH_HEATMETAL))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("You mind is not focused enough.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 5 )
 {
  stc("The adrenaline still coursing through your veins keeps you from focusing\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Heat who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They don't seem to be here.\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
 {
  stc("On avatars only.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("On players only.\n\r", ch);
  return;
 }
 victim->pcdata->pulse_timer[PULSE_HEATMETAL] = number_range(2, 5);
 act("$n concentrates and your equipment begins to heat up.", ch, NULL, victim, TO_VICT);
 act("You concentrate and $N's equipment begins to heat up.", ch, NULL, victim, TO_CHAR);
 act("$n concentrates and stares at $N's equipment.", ch, NULL, victim, TO_NOTVICT);
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 ch->fight_timer += 10;
 WAIT_STATE(ch, 10);
}

/********************************************
 ********************************************
 *                                          *
 * Celestial mov a character to the loca-   *
 * tion of their victim.                    *
 *                                          *
 ********************************************
 ********************************************/
void do_celestial( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 30;
 
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( !HAS_TECH(ch, TECH_CELEST))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("You mind lacks the proper focus.\n\r", ch);
  return;
 }
 argument = one_argument(argument, arg);
 if( arg[0] == '\0')
 {
  stc("Celestial walk to who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("Alas, you cannot locate them.\n\r", ch);
  return;
 }
 if( IS_IMMORTAL(victim) )
 {
  stc("Immortals control the celestial plane, you are not powerful enough to travel to them!\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
 {
  stc("On avatars only.\n\r", ch);
  return;
 }
 if( victim->in_room == NULL)
 { 
  stc("BUG: victim in NULL room.\n\r", ch);
  return;
 }
 if( is_ds_range(victim) )
  return;
 if( ch->in_room->vnum >= 50 && ch->in_room->vnum <= 68 )
 {
  stc("Not in arena.\n\r",ch);
  return;
 }
 if( victim->in_room->vnum >= 50 && victim->in_room->vnum <= 68 )
 {
  stc("Not while they're in arena.\n\r",ch);
  return;
 }
 act("$n's body begins to phase out of existance as the power of $s spirit flares.", ch, NULL, NULL, TO_ROOM);
 char_from_room(ch);
 char_to_room(ch, victim->in_room);
 act("A white light appears before $N as $n forms out of the nothingness.", ch, NULL, victim, TO_NOTVICT);
 act("Using the power of your spirit, you travel the plans and appear before $N.", ch, NULL, victim,TO_CHAR);
 act("A white light appears before you and $n forms out of the nothingness.", ch, NULL, victim, TO_VICT);
 do_look(ch, "");
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 WAIT_STATE(ch, 10);
 return;
}

/************************************************
 ************************************************
 *                                              *
 * Clairvoayance senses the area their victim   *
 * is located, Also the clairvoayance technique *
 * is used to detect if someone is scrying on   *
 * them.                                        *
 *                                              *
 ************************************************
 ************************************************/
void do_clairvoyance( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 10;
 char buf[MAX_STRING_LENGTH];

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( !HAS_TECH(ch, TECH_CLAIR))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("You mind lacks the proper focus.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Sense who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("Alas, you cannot locate them.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("Not on NPC's.\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
 {
  stc("On avatars only.\n\r", ch);
  return;
 }
 if( victim->in_room == NULL)
 {
  stc("BUG: victim in NULL room.\n\r", ch);
  return;
 }
 if( victim->in_room->area == NULL)
 {
  stc("BUG: victim in NULL area.\n\r", ch);
  return;
 }
 sprintf( buf, "You sense %s in %s.\n\r", victim->pcdata->switchname, victim->in_room->area->name);
 stc(buf, ch);
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 if( !IS_NPC(victim) )      
 {
  if( IS_CLASS(victim,CLASS_MONK) && HAS_TECH(victim,TECH_CLAIR) )
   stc("You sense that someone is watching you.\n\r",victim);
  else if( IS_CLASS(victim,CLASS_JEDI) )
   stc("You sense movement in the force.\n\r",victim);
  else if( IS_CLASS(victim,CLASS_APOCRYPHIC) && standard_calls(ch,SCHOOL_ARCANE,8) )
   stc("You feel a magical presence watching you.\n\r",victim);
 }
 WAIT_STATE(ch, 10);
 return;
} 
/*************************************************
 *************************************************
 *                                               *
 * Deathtouch causes a small amount of damage    *
 * every combat round until the timer is used up *
 *                                               *
 *************************************************
 *************************************************/
void do_deathtouch( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 25;
 
 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_DEATHTOUCH))
 {
  stc("You have yet to learn that.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("Your mind lack the focus.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
 {
  stc("They are not an avatar.\n\r", ch);
  return;
 }
 if( victim == ch)
 {
  stc("That would not be very wise.\n\r", ch);
  return;
 }
 act("You hand glows a sickly green as you touch $N.", ch, NULL, victim, TO_CHAR);
 act("$n's hand glows a sickly green as $e touches you.", ch, NULL, victim, TO_VICT);
 act("$n's hand glows a sickly green as $e touches $N.", ch, NULL, victim, TO_NOTVICT);
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 victim->pcdata->pulse_timer[PULSE_DEATHTOUCH] += number_range( 3, 10);
 ch->fight_timer += 15;
 WAIT_STATE(ch, 10);
 return;
}
/****************************************************
 ****************************************************
 *                                                  *
 * Healtouch gives the target a slight heal every   *
 * combat pulse until the counter runs out.         *
 *                                                  *
 ****************************************************
 ****************************************************/
void do_healtouch(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 25;

 argument = one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_HEALTOUCH))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("Your mind lacks the focus.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }

 if( !can_heal(ch, victim) ) return;

 if( !IS_HERO(victim))
 {
  stc("They are not an avatar.\n\r", ch);
  return;
 }
 if( victim != ch)
 {
  act("Your hand glows a pure white as you touch $N", ch, NULL, victim, TO_CHAR); 
  act("$n's hand glows a pure white as $e touches you.", ch, NULL, victim, TO_VICT);
  act("$n's hand glows a pure white as $e touches $N.", ch, NULL, victim, TO_NOTVICT);
 }
 else
 {
  stc("You focus the energy of your soul to heal your wounds.\n\r", ch);
  act("$n's glows a pure white briefly with an expression of peace on $s face.", ch, NULL, NULL, TO_ROOM);
 }
 victim->pcdata->pulse_timer[PULSE_HEALTOUCH] += number_range( 3, 10);
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 WAIT_STATE(ch, 10);
 return;
}

/***********************************************
 ***********************************************
 *                                             *
 * Technique is the fucntion to display and    *
 * improve a monks class. Which is in turn     *
 * is routed to display_class_stats and        *
 * improve_class_stats                         *
 *                                             *
 ***********************************************
 ***********************************************/

void do_technique( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument( argument, arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_MONK) )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0')
 {
  display_class_stats(ch);
  return;
 }
 if( !str_cmp( arg, "chi"))
 {
  improve_class(ch, TECH_CHI);
  return;
 }
 else if( !str_cmp( arg, "focus"))
 {
  improve_class(ch, MONK_MAX_FOC);
  return;
 }
 else if( !str_cmp( arg, "elbow"))
 {
  improve_class(ch, TECH_ELBOW);
  return;
 }
 else if( !str_cmp( arg, "backfist"))
 {
  improve_class(ch, TECH_BACKFIST);
  return;
 }
 else if( !str_cmp( arg, "sweep"))
 {
  improve_class(ch, TECH_SWEEP);
  return;
 }
 else if( !str_cmp( arg, "reverse"))
 {
  improve_class(ch, TECH_REVERSE);
  return;
 }
 else if( !str_cmp( arg, "kneethrust"))
 {
  improve_class(ch, TECH_KNEETHRUST);
  return;
 }
 else if( !str_cmp( arg, "thrustkick"))
 {
  improve_class(ch, TECH_THRUSTKICK);
  return;
 }
 else if( !str_cmp( arg, "roundhouse"))
 {
  improve_class(ch, TECH_ROUNDHOUSE);
  return;
 }
 else if( !str_cmp( arg, "heartpunch"))
 {
  improve_class(ch, TECH_HEARTPUNCH);
  return;
 }
 else if( !str_cmp( arg, "palmstrike"))
 {
  improve_class(ch, TECH_PALMSTRIKE);
  return;
 }
 else if( !str_cmp( arg, "denjin"))
 {
  improve_class(ch, TECH_DENJIN);
  return;
 }
 else if( !str_cmp( arg, "custom"))
 {
  improve_class(ch, TECH_CUSTOM);
  return;
 }
 else if( !str_cmp( arg, "voice"))
 {
  improve_class(ch, TECH_VOICE);
  return;
 }
 else if( !str_cmp( arg, "block"))
 {
  improve_class(ch, TECH_BLOCK);
  return;
 }
 else if( !str_cmp( arg, "celestial"))
 {
  improve_class(ch, TECH_CELEST);
  return;
 }
 else if( !str_cmp( arg, "stance"))
 {
  improve_class(ch, TECH_STANCE);
  return;
 }
 else if( !str_cmp( arg, "combat"))
 {
  improve_class(ch, TECH_UNARMED);
  return;
 }
 else if( !str_cmp( arg, "healtouch"))
 {
  improve_class(ch, TECH_HEALTOUCH);
  return;
 }
 else if( !str_cmp( arg, "deathtouch"))
 {
  improve_class(ch, TECH_DEATHTOUCH);
  return;
 }
 else if( !str_cmp( arg, "heatmetal"))
 {
  improve_class(ch, TECH_HEATMETAL);
  return;
 }
 else if( !str_cmp( arg, "clairvoyance"))
 {
  improve_class(ch, TECH_CLAIR);
  return;
 }
 else
 {
  stc("Learn what?\n\r", ch);
  return;
 }
 return;
}

/*************************************************
 *************************************************
 *                                               *
 * Custcombo is a toggle.  If its on, it should  *
 * syphon a small amount of movement every       *
 * combat pulse.  It also must be turned on to   *
 * gain proficiency in it.                       *
 *                                               *
 *************************************************
 *************************************************/
void do_customcombo( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_CUSTOM))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch, MONK_BITS), MONK_CUST))
 {
  REMOVE_BIT(GET_POWERS(ch, MONK_BITS), MONK_CUST);
  stc("You have relaxed your fighting techniqes to normal.\n\r", ch);
  return;
 }
 else
 {
  SET_BIT(GET_POWERS(ch, MONK_BITS), MONK_CUST);
  stc("Your fighting techniques are at optial.\n\r", ch);
  return;
 }
 return;
}

/************************************************
 ************************************************
 *                                              *
 * Denjin is a stun command. It does a little   *
 * damage, but mainly a stun, based on the      *
 * the current amount of chi the monk has       *
 *                                              *
 ************************************************
 ************************************************/
void do_denjin( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 50;
 int damage = 0;

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_DENJIN))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Hit who?\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("Your mind is not focused enough.\n\r", ch);
  return;
 }
 if( ch->fighting == NULL )
 {
  stc("Only in combat.\n\r",ch); 
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("On players only.\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
 {
  stc("On avatars only.\n\r", ch);
  return;
 }
 if( ch->hit > ch->max_hit/2 )
 {
  stc("You can only perform the denjin as a last resort!\n\r",ch); 
  return;
 }
 if( victim->hit < (victim->max_hit * 0.90) )
 {
  stc("They are too wounded to perform the denjin against!\n\r",ch);
  return;
 }
 do_yell(ch, "DENJIN HOUDUKEN!");
 if( number_range(0,100) < 20 )
 {
  damage = 0;
  act("You hurl a fireball of chi energy at $N and miss!",ch,NULL,victim,TO_CHAR);
  act("$n hurls a fireball of chi energy at you and misses!",ch,NULL,victim,TO_VICT);
  act("$n hurls a fireball of chi energy at $N and misses!",ch,NULL,victim,TO_NOTVICT);
 }
 else
 {
  damage = (GET_POWERS(ch, MONK_CURR_CHI) * 100) + number_range(200, 400);
  act2("You hurl a fireball of chi energy at $N. [$D]", ch, NULL, victim, TO_CHAR, damage);
  act2("$n hurls a fireball of chi energy at you. [$D]", ch, NULL, victim, TO_VICT, damage);
  act2("$n hurls a fireball of chi energy at $N. [$D]", ch, NULL, victim, TO_NOTVICT, damage);
  WAIT_STATE(victim,20+GET_POWERS(ch,MONK_CURR_CHI));
 }
 hurt_person(ch, victim, damage);
 ch->fight_timer += 10;
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 WAIT_STATE(ch,12);
 return;
}

void do_palmstrike( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 40;
 int damage = 0;
 
 argument = one_argument( argument, arg);
  
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_PALMSTRIKE))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 {
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( GET_POWERS(ch, MONK_CURR_FOC) < cost)
 {
  stc("Your mind is not focused enough.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
 {
  stc("On players only.\n\r", ch);
  return;
 }
 if( !IS_HERO(victim))
 {
  stc("On avatars only.\n\r", ch);
  return;
 }
 if( number_range(0,100) < 30 )
 {
  act("You slam your open palm into $N's chest, but it has no affect!",ch,NULL,victim,TO_CHAR);
  act("$n slams $s open palm into your chest, but it has no affect!",ch,NULL,victim,TO_VICT);
  act("$n slams $s open palm into $N's chest, but it has no affect!",ch,NULL,victim,TO_NOTVICT);
 }
 else
 {
  damage = (GET_POWERS(ch, MONK_CURR_CHI) * 100) + number_range(200, 400);
  act2("You slam your open palm into $N' chest. [$D]", ch, NULL, victim, TO_CHAR, damage);
  act2("$n slams $s open palm into your chest. [$D]", ch, NULL, victim, TO_VICT, damage);
  act2("$n slams $s open palm into $N's chest. [$D]", ch, NULL, victim, TO_NOTVICT, damage);
  hurt_person(ch, victim, damage);
  stop_fighting(ch,TRUE);
  stop_fighting(victim,TRUE);
 }
 ch->fight_timer += 10;
 GET_POWERS(ch, MONK_CURR_FOC) -= cost;
 WAIT_STATE(ch,8);
 WAIT_STATE(victim,14);
 return;
}

/****************************************************
 *                                                  *
 * Chi is used to make the monk fight harder and    *
 * better.  Syphons movement every combat round     *
 * that Chi exists. Chi must be purchased upto a    *
 * max defined in MONK_MAX_CHI.                     *
 *                                                  *
 ****************************************************/
void do_chi( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_CHI) >= GET_POWERS(ch, MONK_MAX_CHI))
 {
  stc("Your chi is as high as it can go!\n\r", ch);
  return;
 }
 GET_POWERS(ch, MONK_CURR_CHI)++;
 stc("#7You concentrate and gain power from the forces around you.\n\r#n", ch);
 act("#7You see lines of concentration cross $n's face as $e gathers strength.#n", ch,
  NULL, NULL, TO_ROOM);
 return;
}

/*****************************************************
 *                                                   *
 * Relax releases stored Chi energy so the monk can  *
 * conserve movement.                                *
 *                                                   *
 *****************************************************/
void do_relax( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch))
  return;
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( GET_POWERS(ch, MONK_CURR_CHI) == 0)
  return;
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 stc("You release your chi back into the world.\n\r", ch);
 act("The lines of stress around $n's face relax.", ch, NULL, NULL, TO_ROOM);
 GET_POWERS(ch, MONK_CURR_CHI)--;
 WAIT_STATE(ch, 6);
 return;
}

/*****************************************************
 *                                                   *
 * Combo_exec is used as a pass through for all the  *
 * combos to keep the attacks relatively clean.      *
 * If a new combo is created make a new definition   *
 * in combo_type enumerated datatype and place it in *
 * here.                                             *
 *                                                   *
 *****************************************************/
void combo_exec(CHAR_DATA *ch, CHAR_DATA *victim, int combo)
{
 int dam = 0;
 int value = 0;

 if( victim == NULL)
 {
  stc("NULL victim, please report to immortals.\n\r", ch);
  return;
 }
 if( ch == NULL)
  return;
 if( victim->hit < 0)
  return;
 if( combo == COMBO_MOVE)
 {
  dam = number_range(1200,1950);
  act("#0$n begins to radiate black.#n", ch, NULL, NULL, TO_ROOM);
  stc("#0You begin to radiate black.#n\n\r", ch);
  act2("#0You touch $N and syphon off $S endurance. [$D]#n", ch, NULL, victim, TO_CHAR, dam);
  act2("#0$n touches $N and syphons off $S endurance. [$D]#n", ch, NULL, victim, TO_NOTVICT,dam );
  act2("#0$n touches you and syphons off some of your endurance. [$D]#n", ch, NULL, victim, TO_VICT,dam);
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  ch->move += dam;
  victim->move -= dam;
  if( ch->move > ch->max_move)
   ch->move = ch->max_move;
  if( victim->move < 0)
    victim->move = 0;
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN_NONE)   
  {
   value = CHAIN2_STAGE1;
   act("#b*******CHAIN COMBO 2 STAGE 1 *******#n", ch, NULL,victim, TO_ALL);
  }
  else
   value = CHAIN_NONE;
  GET_POWERS(ch, MONK_CHAIN) = value;
  return;
 }
 else if( combo == COMBO_MANA)
 {
  dam = number_range(1200,1950);
  act("#6$n begins to glow bright cyan.#n", ch, NULL, NULL, TO_ROOM);
  stc("#6You begin to glow bright cyan.#n\n\r", ch);
  act2("#6You syphon magical energies from $N. [$D]#n", ch, NULL, victim, TO_CHAR,dam);
  act2("#6$n syphons magical energies from you. [$D]#n", ch, NULL, victim, TO_VICT,dam);
  act2("#6$n syphons magical energies from $N. [$D]#n", ch, NULL, victim, TO_NOTVICT,dam);
  ch->mana += dam;
  victim->mana -= dam;
  if( ch->mana > ch->max_mana)
   ch->mana = ch->max_mana;
  if( victim->mana < 0)
    victim->mana = 0;
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN_NONE)
  {
   value = CHAIN1_STAGE1;
   act("#b*******CHAIN COMBO 1 STAGE 1 *******#n", ch, NULL,victim, TO_ALL);
  }
  else
   value = CHAIN_NONE;
  GET_POWERS(ch, MONK_CHAIN) = value;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  return;
 }
 else if( combo == COMBO_STUN)
 {
  act("#2$n begins to glow bright green.#n", ch, NULL, NULL, TO_ROOM);
  stc("#2You begin to glow bright green.#n\n\r", ch);
  if( victim->stuntimer <= 0)
  {
   act("#2You draw your fist back and hit $N EXTREMELY HARD in the chest!#n", ch, NULL,
       victim, TO_CHAR);
   act("#2$n draws $s fist back and hits you EXTREMELY HARD in the chest!#n", ch, NULL,
       victim, TO_VICT);
   act("#2$n draws $s fist back and hits $N EXTREMELY HARD in the chest!#n", ch, NULL,
       victim, TO_NOTVICT);
   if( can_stun(ch, victim) )
       set_stun(victim, number_range(1,2));
  }
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN3_STAGE2)
  {
   act("#b****CHAIN COMBO 3*******#n", ch, NULL, victim, TO_ALL);
   combo_exec(ch, victim, COMBO_CHAIN3);    
  }
  else if( GET_POWERS(ch, MONK_CHAIN) == CHAIN4_STAGE2)
  {
   act("#b*****CHAIN COMBO 4*******#n", ch, NULL, victim, TO_ALL);
   combo_exec(ch, victim, COMBO_CHAIN4);
  }
  else
   GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  return;
 }
 else if( combo == COMBO_DAM1)
 {
  dam = number_range(950,1200);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  stc("#1You begin to glow bright red.#n\n\r", ch);
  act("#1$n begins to glow bright red.#n", ch, NULL, victim, TO_ROOM);
  act2("#1You jump high in the air and kick $N in the head. [$D]#n", ch, NULL, victim, TO_CHAR,dam);
  act2("#1$n jumps high in the air and kicks you in the head. [$D]#n", ch, NULL, victim, TO_VICT,dam);
  act2("#1$n jumps high in the air and kicks $N in the head. [$D]#n", ch, NULL, victim, TO_NOTVICT,dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN1_STAGE1)
  {
   value = CHAIN1_STAGE2;
   act("#b*******CHAIN COMBO 1 STAGE 2 *******#n", ch, NULL,victim, TO_ALL);
  }
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN3_STAGE1)
  { 
   value = CHAIN3_STAGE2;
   act("#b*******CHAIN COMBO 3 STAGE 2 *******#n", ch, NULL,victim, TO_ALL);
  }
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN_NONE)
  {
   value = CHAIN5_STAGE1;
   act("#b*******CHAIN COMBO 5 STAGE 1 *******#n", ch, NULL,victim, TO_ALL);
  }
  GET_POWERS(ch, MONK_CHAIN) = value;
 }
 else if( combo == COMBO_DAM2)
 {
  act("#1$n begins to glow bright red.#n", ch, NULL, NULL, TO_ROOM);
  stc("#1You begin to glow bright red.#n\n\r", ch);
  dam = number_range(1200,1450);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  hurt_person(ch, victim, dam);
  act2("#1$n hits you with a flurry of punches. [$D]#n", ch, NULL, victim, TO_VICT,dam);
  act2("#1You hit $N with a flurry of punches. [$D]#n", ch, NULL, victim, TO_CHAR,dam);
  act2("#1$n hits $N with a flurry of punches. [$D]#n", ch, NULL, victim, TO_NOTVICT,dam);
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN2_STAGE1)
  {
   value = CHAIN2_STAGE2;
   act("#b*******CHAIN COMBO 2 STAGE 2 *******#n", ch, NULL,victim, TO_ALL);
  }
  else if( GET_POWERS(ch, MONK_CHAIN) == CHAIN_NONE)
  {
   value = CHAIN4_STAGE1;
   act("#b*******CHAIN COMBO 4 STAGE 1 *******#n", ch, NULL,victim, TO_ALL);
  }
  else if( GET_POWERS(ch, MONK_CHAIN) == CHAIN5_STAGE1)
  {
   value = CHAIN5_STAGE2;
   act("#b*******CHAIN COMBO 5 STAGE 2 *******#n", ch, NULL,victim, TO_ALL);
  }
  else
   value = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  GET_POWERS(ch, MONK_CHAIN) = value;
  return;
 }
 else if( combo == COMBO_DAM3)
 {
  act("#1$n begins to glow bright red.#n", ch, NULL, NULL, TO_ROOM);
  stc("#1You begin to glow bright red.#n\n\r", ch);
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  dam = number_range(1450,1700);
  hurt_person(ch, victim, dam);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  act2("#1$n grabs the back of your head, pulls back $s fist and smashes it into your nose. [$D]#n", 
    ch, NULL, victim, TO_VICT,dam);
  act2("#1You grab $N by the back of the head, pull back your fist and smash it into $S nose. [$D]#n", 
    ch, NULL, victim, TO_CHAR,dam);
  act2("#1$n grabs $N by the back of the head, pulls back $s fist and smashes it into $N's nose. [$D]#n", 
    ch, NULL, victim, TO_NOTVICT,dam);
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN_NONE)
  {
   value = CHAIN3_STAGE1;
   act("#b*******CHAIN COMBO 3 STAGE 1 *******#n", ch, NULL,victim, TO_ALL);
  }
  else if( GET_POWERS(ch, MONK_CHAIN) == CHAIN5_STAGE2)
  {
   act("#b*********COMBO CHAIN 5********#n", ch, NULL, victim, TO_ALL);
   combo_exec(ch, victim, COMBO_CHAIN5);
  }
  else
   value = CHAIN_NONE;
  GET_POWERS(ch, MONK_CHAIN) = value; 
  return;
 }
 else if( combo == COMBO_HEAL1)
 {
  dam = number_range(950,1200);
  act("#4 $n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
  stc("#4 You begin to glow bright blue.#n\n\r", ch);
  act2("#4You glow with a healing radiance. [$D]#n", ch,NULL,victim, TO_CHAR, dam);
  act2("#4$n glows with a healing radiance. [$D]#n", ch, NULL, victim, TO_ROOM, dam);
  ch->hit += dam;
  if( ch->hit > ch->max_hit)
   ch->hit = ch->max_hit;
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN2_STAGE2)
  {
   act("#b********* COMBO CHAIN 2 *******#n", ch, NULL, victim, TO_ALL);
   combo_exec(ch, victim, COMBO_CHAIN2);
  }
  else
   GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;  
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  return;
 }
 else if( combo == COMBO_HEAL2)
 {
  if( victim == NULL)
    return;
  dam = number_range(1200,1450);
  act("#4$n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
  stc("#4You begin to glow bright blue.#n\n\r", ch);
  act2("#4You glow brightly as a healing light envelopes you. [$D]#n", ch, NULL, NULL, TO_CHAR, dam);
  act2("#4$n glows brightly as a healing light envelopes $m. [$D]#n", ch, NULL, victim, TO_ROOM, dam);
  ch->hit += dam;
  if( ch->hit > ch->max_hit)
   ch->hit = ch->max_hit;
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN1_STAGE2)
  {
   act("#b********COMBO CHAIN 1**********#n", ch, NULL, victim, TO_ALL);
   combo_exec(ch, victim, COMBO_CHAIN1);
  }
  else
   GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  return;
 }
 else if( combo == COMBO_HEAL3)
 {
  dam = number_range(1450,1700);
  act("#4$n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
  stc("#4You begin to glow bright blue.#n\n\r", ch);
  act2("#4Light explodes from your body as you are healed! [$D]#n", ch, NULL, NULL, TO_CHAR, dam);
  act2("#4Light explodes from $n's body as $e is healed! [$D]#n", ch, NULL, victim, TO_ROOM, dam);
  ch->hit += dam;
  if( ch->hit > ch->max_hit)
   ch->hit = ch->max_hit;
  if( GET_POWERS(ch, MONK_CHAIN) == CHAIN4_STAGE1)
  {
   value = CHAIN4_STAGE2;
   act("#b*******CHAIN COMBO 4 STAGE 2 *******#n", ch, NULL,victim, TO_ALL);
  }
  else
   value = CHAIN_NONE;
  GET_POWERS(ch, MONK_CHAIN) = value;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  return;
 }
 else if( combo == COMBO_CHAIN1)
 {
  if( victim == NULL)
   return;
  dam = number_range(1300,3300);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  act("#2$n pulses with a bright green light.#n", ch, NULL, NULL, TO_ROOM);
  stc("#2You pulse with a bright green light.#n\n\r", ch);
  act2("#2You send $N flying with a dragon uppercut!#n [$D]",ch, NULL, victim, TO_CHAR, dam);
  act2("#2$n sends you flying with a dragon uppercut!#n [$D]",ch, NULL, victim, TO_VICT, dam);
  act2("#2$n sends $N flying with a dragon uppercut!#n [$D]", ch, NULL, victim, TO_NOTVICT,dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
 }
 else if( combo == COMBO_CHAIN2)
 {
  dam = number_range(1550,3550);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  act("#1$n pulses with a bright red light.#n", ch, NULL, NULL, TO_ROOM);
  stc("#1You pulse with a bright red light.#n\n\r", ch);
  act2("#1You hit $N INSANELY HARD with a flaming dragon uppercut!#n [$D]",ch, NULL, victim, TO_CHAR, dam);
  act2("#1$n hits you INSANELY HARD with a flaming dragon uppercut!#n [$D]",ch, NULL, victim, TO_VICT, dam);
  act2("#1$n hits $N INSANELY HARD with a flaming dragon uppercut!#n [$D]", ch, NULL, victim, TO_NOTVICT,dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
 }
 else if( combo == COMBO_CHAIN3)
 {
  dam = number_range(1800,3800);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  act("#3$n pulses with a bright yellow light.#n", ch, NULL, NULL, TO_ROOM);
  stc("#3You pulse with a bright yellow light.#n\n\r", ch);
  act2("#3You jump into the air and send a barrage of fireballs at $N!#n [$D]",
    ch, NULL, victim, TO_CHAR, dam);
  act2("#3$n jumps into the air and sends a barrage of fireballs at you!#n [$D]", 
    ch, NULL, victim, TO_VICT, dam);
  act2("#3$n jumps into the air and sends a barrage of fireballs at $N!#n [$D]", 
    ch, NULL, victim, TO_NOTVICT, dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
 }
 else if( combo == COMBO_CHAIN4)
 {
  dam = number_range(2050,4050);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  act("#4$n pulses with a bright blue light.#n", ch, NULL, NULL, TO_ROOM);
  stc("#4You pulse with a bright blue light.#n\n\r", ch);
  act2("#4You jump into the air and pummel $N with a hurricane kick!#n [#D]",
    ch, NULL, victim, TO_CHAR, dam);
  act2("#4$n jumps into the air and pummels you with a hurricane kick#n [$D]", 
    ch, NULL, victim, TO_VICT,dam);
  act2("#4$n jumps into the air and pummels $N with a hurricane kick!#n [$D]", 
    ch, NULL, victim, TO_NOTVICT,dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
 }
 else if( combo == COMBO_CHAIN5)
 {
  act("#5You glow with a bright purple light.#n", ch, NULL, victim, TO_CHAR);
  act("#5$n glows with a bright purple light.#n", ch, NULL, victim, TO_ROOM);
  dam = number_range(2300,4300);
  if( ch->pcdata->pulse_timer[PULSE_BEAR] > 0)
   dam *= 2;
  act2("#5You beat $N senseless with a barrage of stonefists!#n [$D]",ch,NULL,victim,TO_CHAR, dam);
  act2("#5$n beats you senseless with a barrage of stonefists!#n [$D]", ch, NULL, victim, TO_VICT, dam);
  act2("#5$n beats $N senseless with a barrage of stonefists!#n [$D]",ch, NULL, victim, TO_NOTVICT,dam);
  hurt_person(ch, victim, dam);
  GET_POWERS(ch, MONK_CHAIN) = CHAIN_NONE;
  GET_POWERS(ch, MONK_COMBO) = COMBO_NONE;
  return;
 }
 else
  return;
}

/*********************************************
 *                                           *
 * Everything below this comment is a combat *
 * maneuver used to create a combo and a     *
 * chain.  Chains are 3 combos used one      *
 * after the other. Simple define the new    *
 * combos and chains in the appropriate area *
 * and change the values as needed.          *
 *                                           *
 *********************************************/
void do_elbow(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("You elbow $N hard in the gut. [$D]", ch, NULL, victim, TO_CHAR, dam);
 act2("$n elbows $N hard in the gut. [$D]", ch, NULL, victim, TO_NOTVICT,dam);
 act2("$n elbows you hard in the gut. [$D]", ch, NULL, victim, TO_VICT,dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO5_STAGE1:
   act("#1 $n begins to glow bright red.#n", ch, NULL, NULL, TO_ROOM);
   stc("#1 You begin to glow bright red.#n\n\r", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO5_STAGE2;
   break;
 case COMBO7_STAGE1:
   act("#4 $n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4 You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO7_STAGE2;
   break;
 case COMBO9_STAGE3:
   act("#4 $n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4 You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO9_STAGE4;
   break;
 case COMBO4_6_STAGE2:
  combo_exec(ch, victim, COMBO_DAM1);
  break;
 case COMBO_NONE:
  GET_POWERS(ch, MONK_COMBO) = COMBO2_STAGE1;
  break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim);
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT); 
 return;
}

void do_reverse(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("Your reverse sweep knocks $N off $S feet. [$D]", ch, NULL, victim, TO_CHAR, dam);
 act2("$n's reverse sweep knocks $N off $S feet. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
 act2("$n's reverse sweep knocks you off your feet. [$D]", ch, NULL, victim, TO_VICT, dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO1_STAGE2:
   combo_exec(ch, victim,COMBO_MOVE);
   gain_custom_combo(ch);
   break;
 case COMBO6_STAGE3:
   act("#4 $n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4 You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO6_STAGE4;
   break;
 case COMBO8_STAGE3:
   combo_exec(ch, victim, COMBO_HEAL2);
   gain_custom_combo(ch);
   break;
 case COMBO_NONE:
   GET_POWERS(ch, MONK_COMBO) = COMBO1_STAGE1;
   break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT);
 return;
}

void do_sweep( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("Your sweep trips up $N. [$D]", ch, NULL, victim, TO_CHAR, dam);
 act2("$n sweeps $N and trips $M up. [$D]",ch, NULL, victim, TO_NOTVICT, dam);
 act2("$n trips you up with a sweep. [$D]",ch, NULL, victim, TO_VICT, dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO1_STAGE1:
   act("#0 $n begins to radiate black.#n", ch, NULL, NULL, TO_ROOM);
   stc("#0 You begin to radiate black.#n\n\r", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO1_STAGE2;
   break;
  case COMBO6_STAGE4:
   combo_exec(ch, victim, COMBO_DAM3);
   gain_custom_combo(ch); 
   break;
  case COMBO_NONE:
   GET_POWERS(ch, MONK_COMBO) = COMBO5_STAGE1;
   break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT); 
 return;
}

void do_backfist(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("Your backfist smacks $N hard in the face. [$D]",ch, NULL, victim, TO_CHAR, dam);
 act2("$n sends a backfist at $N's face. [$D]",ch, NULL, victim, TO_NOTVICT, dam);
 act2("$n smacks you in the face with a backfist. [$D]",ch, NULL, victim, TO_VICT, dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO2_STAGE1:
   act("#6 $n begins to glow bright cyan.#n", ch, NULL, NULL, TO_ROOM);
   stc("#6 You begin to glow bright cyan.#n\n\r", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO2_STAGE2;
   break;
 case COMBO5_STAGE3:
   combo_exec(ch, victim, COMBO_DAM2);
   gain_custom_combo(ch);
   break;
 case COMBO4_6_STAGE2:
   act("#1 $n begins to glow bright red.#n", ch, NULL, NULL, TO_ROOM);
   stc("#1 You begin to glow bright red.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO6_STAGE3;
   break;
 case COMBO8_STAGE2:
   act("#4$n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO8_STAGE3;
   break;
 case COMBO9_STAGE2:
   act("#4$n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO9_STAGE3;
   break;
 case COMBO_NONE:
  GET_POWERS(ch, MONK_COMBO) = COMBO4_6_STAGE1;
  break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT); 
 return;
}

void do_kneethrust(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("You kneethrust $N hard in the gut . [$D]",ch, NULL, victim, TO_CHAR, dam);
 act2("$n kneethrusts $N hard in the gut. [$D]", ch, NULL, victim, TO_NOTVICT,dam);
 act2("$n knees you hard in the gut. [$D]", ch, NULL, victim, TO_VICT,dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO2_STAGE2:
   combo_exec(ch, victim, COMBO_MANA);
   gain_custom_combo(ch);
   break;
 case COMBO3_STAGE1: // this is actually combo 9
   act("#4 $n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4 You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO9_STAGE2;
   break;
 case COMBO_NONE:
  GET_POWERS(ch, MONK_COMBO) = COMBO3_STAGE1;
  break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT); 
 return;
}

void do_thrustkick(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("You knock the air from $N's lungs with a thrustkick. [$D]",ch, NULL, victim, TO_CHAR, dam);
 act2("$n knocks the air from $N's lungs with a thrustkick. [$D]",ch, NULL, victim, TO_NOTVICT, dam);
 act2("$n knocks the air from your lungs with a thrustkick. [$D]",ch, NULL, victim, TO_VICT, dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO5_STAGE2:
   act("#1 $n begins to glow bright red.#n", ch, NULL, NULL, TO_ROOM);
   stc("#1 You begin to glow bright red.#n\n\r", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO5_STAGE3;
   break;
 case COMBO9_STAGE4:
   combo_exec(ch, victim, COMBO_HEAL3);
   gain_custom_combo(ch);
   break;
 case COMBO_NONE:
   GET_POWERS(ch, MONK_COMBO) = COMBO3_STAGE1;
   break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT); 

 return;
}

void do_roundhouse(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("Your send a roundhouse kick at $N's head. [$D]",ch, NULL, victim, TO_CHAR, dam);
 act2("$n sends a roundhouse kick at $N's head. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
 act2("$n sends a roundhouse kick at your head. [$D]", ch, NULL, victim, TO_VICT, dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO3_STAGE1:
   act("#2 $n begins to glow bright green.#n", ch, NULL, NULL, TO_ROOM);
   stc("#2 You begin to glow bright green.#n\n\r", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO3_STAGE2;
   break;
 case COMBO4_6_STAGE1:
   act("#1 $n begins to glow bright red.#n", ch, NULL, NULL, TO_ROOM);
   stc("#1 You begin to glow bright red.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO4_6_STAGE2;
   break;
 case COMBO8_STAGE1:
   act("#4 $n begins to glow bright blue.#n", ch, NULL, NULL, TO_ROOM);
   stc("#4 You begin to glow bright blue.#n", ch);
   GET_POWERS(ch, MONK_COMBO) = COMBO8_STAGE2;
   break;
 case COMBO_NONE:
   GET_POWERS(ch, MONK_COMBO) = COMBO8_STAGE1;
   break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT); 
 return;
}

void do_heartpunch(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam =0;
 int combo = COMBO_NONE;

 argument = one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 combo = GET_POWERS(ch, MONK_COMBO);
 if( !IS_CLASS(ch, CLASS_MONK))
 {
  do_huh(ch, "");
  return;
 }
 if( !HAS_TECH(ch, TECH_ELBOW))
 {
  stc("You don't know how.\n\r", ch);
  return;
 }
 if( has_weapons(ch))
 { 
  stc("You can't while your hands are not empty.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  if( (victim = ch->fighting) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 else
 {
  if( (victim = get_char_room(ch, arg)) == NULL)
  {
   stc("Hit who?\n\r", ch);
   return;
  }
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Try as you might, you're not -that- flexible!\n\r",ch);
  return;
 }
 dam = number_range(MONK_MIN_DAM,MONK_MAX_DAM);
 act2("Your heartpunch hits $N hard in the chest. [$D]",ch, NULL, victim, TO_CHAR, dam);
 act2("$n heartpunchs $N hard in the chest. [$D]", ch, NULL, victim, TO_NOTVICT, dam);
 act2("$n heartpunchs you hard in the chest. [$D]",ch, NULL, victim, TO_VICT, dam);
 hurt_person(ch, victim, dam);
 switch( GET_POWERS(ch, MONK_COMBO))
 {
  default:
   GET_POWERS(ch, MONK_COMBO) = COMBO_NONE; break;
  case COMBO3_STAGE2:
   combo_exec(ch, victim, COMBO_STUN);
   gain_custom_combo(ch);
   break;
 case COMBO7_STAGE2:
   combo_exec(ch, victim, COMBO_HEAL1);
   gain_custom_combo(ch);
   break;
 case COMBO_NONE:
   GET_POWERS(ch, MONK_COMBO) = COMBO7_STAGE1;
   break;
 }
 if( ch->fighting == NULL)
  set_fighting( ch, victim); 
 if( MONK_SET(ch, MONK_CUST))
  WAIT_STATE(ch, (MONK_WAIT / 2));
 else
  WAIT_STATE(ch, MONK_WAIT);
 return;
}

