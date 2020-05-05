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

CLASS_DATA *alie;
CLASS_DATA *apoc;
CLASS_DATA *drag;
CLASS_DATA *drow;
CLASS_DATA *jedi;
CLASS_DATA *monk;
CLASS_DATA *shad;
CLASS_DATA *vamp;
CLASS_DATA *were;

// parses all the improvemtn fields for a class
void display_class_stats( CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];
 
  if(IS_NPC(ch))
   return;
  if(IS_CLASS(ch, CLASS_NONE))
  {
   do_huh(ch, "");
   return;
  }
  if( IS_CLASS(ch,CLASS_DROW) )
  {
   stc("#6The powers Lloth has bestowed upon you...#n\n\r",ch);
   stc("#4-----------------------------------------#n\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_SPEED) )
    stc("You move quite quickly.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_TOUGHSKIN) )
    stc("Your skin has been toughened.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWFIRE) )
    stc("You can call Drowfire upon your enemies.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DARKNESS) )
    stc("You can summon a globe of darkness.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_LEVITATION) )
    stc("You can levitate in the air.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWSHIELD) )
    stc("You can shield your aura.\n\r",ch);  
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWPOISON) )
    stc("You have control over deadly poisons.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_GAROTTE) )
    stc("You have mastered the art of garotte.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DGAROTTE) )
    stc("You have dark-enhanced garotte.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_ARMS) )
    stc("You have extra spidery arms.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_DROWHATE) )
    stc("You can invoke the hatred of the Drow.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_SPIDERFORM) )
    stc("You can mutate into a large spider.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_WEB) )
    stc("You can shoot a spidery web at opponents.\n\r",ch);
   if (IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_EARTHSHATTER) )
    stc("You can shatter the earth under your feet.\n\r",ch);
   stc("\n\r", ch);
   if( ch->generation == 1 )
    stc("You are Lloth's Avatar.\n\r",ch);
   if( ch->generation == 2 )
    stc("You are a Drow Matron.\n\r",ch);
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) )
    stc("You are a Drow Warrior.\n\r",ch);
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    stc("You are a Drow Mage.\n\r", ch );
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    stc("You are a Drow Cleric.\n\r", ch );
   sprintf(buf,"You have %d drow power points.\n\r",GET_POWERS(ch,DROW_POINTS) );
    stc(buf,ch);
   sprintf(buf,"You have %d points of magic resistance.\n\r",GET_POWERS(ch,DROW_MAGIC) );
    stc(buf,ch);
   sprintf(buf,"You have %d pulses of darkness remaining.\n\r",ch->pcdata->pulse_timer[PULSE_DARKNESS]);
    stc(buf,ch);
   if( weather_info.sunlight == SUN_DARK )
    stc("You feel strong in the night.\n\r", ch );
   stc("\n\r",ch);
   return;
  }
  if( IS_CLASS(ch,CLASS_JEDI) )
  {
   char rank[MAX_STRING_LENGTH];

   switch( GET_POWERS(ch,JEDI_RANK) )
   {
    case JEDI_RANK_NONE:       sprintf(rank,"BUGGED");     break;
    case JEDI_RANK_APPRENTICE: sprintf(rank,"Apprentice"); break;
    case JEDI_RANK_INITIATE:   sprintf(rank,"Initiate");   break;
    case JEDI_RANK_GUARDIAN:   sprintf(rank,"Guardian");   break;
    case JEDI_RANK_KNIGHT:     sprintf(rank,"Knight");     break;
    case JEDI_RANK_MASTER:     sprintf(rank,"Master");     break;
    case JEDI_RANK_LORD:       sprintf(rank,"Lord");       break;
   }
   banner2_to_char("Jedi Stats",ch);
   sprintf(buf,"     Stance    [%2d]    Attack    [%2d]     Defense    [%2d]\n\r",
    GET_POWERS(ch,JEDI_STANCE),GET_POWERS(ch,JEDI_ATTACK),GET_POWERS(ch,JEDI_DEFENSE));
   stc(buf,ch);
   if( GET_POWERS(ch,JEDI_LIGHT) < 5 && GET_POWERS(ch,JEDI_DARK) < 5 )
    sprintf(buf,"                       Rank [%s]\n\r",rank);
   else if( GET_POWERS(ch,JEDI_LIGHT) >= 5 )
    sprintf(buf,"                       Rank [Jedi %s]\n\r",rank);
   else if( GET_POWERS(ch,JEDI_DARK) >= 5 )
    sprintf(buf,"                       Rank [Sith %s]\n\r",rank);
   stc(buf,ch);
   stc("\n\r",ch);
   banner2_to_char("Force Stats",ch);
   sprintf(buf,"     Light    [%2d]\n\r",GET_POWERS(ch,JEDI_LIGHT));
   stc(buf,ch);
   sprintf(buf,"       Force Heal      [%2d]     Force Absorb    [%2d]     Force Shield   [%2d]\n\r",
    GET_POWERS(ch,JEDI_FORCE_HEAL),GET_POWERS(ch,JEDI_FORCE_ABSORB),GET_POWERS(ch,JEDI_FORCE_SHIELD));
   stc(buf,ch);
   sprintf(buf,"       Jedi Mind Trick [%2d]     Jedi Sight      [%2d]\n\r",
    GET_POWERS(ch,JEDI_FORCE_MIND_TRICK),GET_POWERS(ch,JEDI_FORCE_SIGHT));
   stc(buf,ch);
   sprintf(buf,"     Neutral  [%2d]\n\r",GET_POWERS(ch,JEDI_NEUTRAL));
   stc(buf,ch);
   sprintf(buf,"       Force Speed     [%2d]     Force Jump      [%2d]\n\r",
    GET_POWERS(ch,JEDI_FORCE_SPEED),GET_POWERS(ch,JEDI_FORCE_JUMP));
   stc(buf,ch);
   sprintf(buf,"       Force Push      [%2d]     Force Pull      [%2d]\n\r",
    GET_POWERS(ch,JEDI_FORCE_PUSH),GET_POWERS(ch,JEDI_FORCE_PULL));
   stc(buf,ch);
   sprintf(buf,"     Dark     [%2d]\n\r",GET_POWERS(ch,JEDI_DARK));
   stc(buf,ch);
   sprintf(buf,"       Force Grip      [%2d]     Force Rage      [%2d]\n\r",
    GET_POWERS(ch,JEDI_FORCE_SPEED),GET_POWERS(ch,JEDI_FORCE_JUMP));
   stc(buf,ch);
   sprintf(buf,"       Force Drain     [%2d]     Force Lightning [%2d]\n\r",
    GET_POWERS(ch,JEDI_FORCE_PUSH),GET_POWERS(ch,JEDI_FORCE_PULL));
   stc(buf,ch);
   sprintf(buf,"                       Force Pool [%d of %d]\n\r",GET_POWERS(ch,JEDI_FORCE_POOL),
    GET_POWERS(ch,JEDI_FORCE_POOL_MAX));
   stc(buf,ch);
  }
  if( IS_CLASS(ch, CLASS_DRAGON))
  {
   banner2_to_char("Dragon stats", ch);
   if( GET_POWERS(ch, DRAGON_MIGHT) >= 10)
    sprintf( buf, "Might: %d\n\r", GET_POWERS(ch, DRAGON_MIGHT));
   else
    sprintf( buf, "Might: %d     Points to increase: %d\n\r", GET_POWERS(ch, DRAGON_MIGHT),
     (GET_POWERS(ch, DRAGON_MIGHT)+1)*POINT_STEP);
   stc(buf, ch);
   if( GET_POWERS(ch, DRAGON_TOUGH) >= 10)
    sprintf( buf, "Toughness: %d\n\r", GET_POWERS(ch, DRAGON_TOUGH));
   else
    sprintf( buf, "Toughness: %d     Points to increase: %d\n\r",GET_POWERS(ch, DRAGON_TOUGH), 
     (GET_POWERS(ch, DRAGON_TOUGH)+1)*POINT_STEP);
   stc(buf, ch);
   if( GET_POWERS(ch, DRAGON_SPEED) >= 10)
    sprintf( buf, "Speed: %d\n\r", GET_POWERS(ch, DRAGON_SPEED));
   else
    sprintf( buf, "Speed: %d     Points to increase: %d\n\r",GET_POWERS(ch, DRAGON_SPEED), 
     (GET_POWERS(ch, DRAGON_SPEED)+1)*POINT_STEP);
   stc(buf, ch);
   if( GET_POWERS(ch, DRAGON_MAGIC) >= 10)
    sprintf( buf, "Magic: %d\n\r", GET_POWERS(ch, DRAGON_MAGIC));
   else
    sprintf( buf, "Magic: %d     Points to increase: %d\n\r",GET_POWERS(ch, DRAGON_MAGIC), 
     (GET_POWERS(ch, DRAGON_MAGIC)+1)*POINT_STEP);
   stc(buf, ch);
   if( GET_POWERS(ch, DRAGON_AGE) >= 10000)
    sprintf( buf, "Years: %d\n\r", GET_POWERS(ch, DRAGON_AGE));
   else
    sprintf( buf, "Years: %d     Points to increase: %d\n\r",GET_POWERS(ch, DRAGON_AGE),
     (GET_POWERS(ch, DRAGON_AGE)+1)*2);
   stc(buf, ch);
   sprintf( buf, "Months: %d\n\r", GET_POWERS(ch, DRAGON_DAYS));
   stc(buf, ch);
   sprintf( buf, "Points: %d\n\r", GET_POWERS(ch, DRAGON_POINTS));
   stc(buf, ch);
   sprintf( buf, "You have %d pulses left on your breath timer.\n\r", GET_POWERS(ch, DRAGON_BTIME));
   stc( buf, ch);
   sprintf( buf, "You have %d pulses left on your shift timer.\n\r", GET_POWERS(ch, DRAGON_STIME));
   stc( buf, ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FIRE))
    stc("#1You have fire breath.\n\r#n", ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ICE))
    stc("#7You have ice breath.\n\r#n", ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_ACID))
    stc("#0You have acid breath.\n\r#n", ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_POISON))
    stc("#2You have poison breath.\n\r#n", ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_LIGHTNING))
    stc("#4You have lightning breath.\n\r#n", ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_GRAB))
    stc("You can grasp your victims with your enormous claws.\n\r", ch);
   if( IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_WINGS))
    stc("Your wings have grown.\n\r#n", ch);
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
    stc("You are in Dragon form.\n\r", ch);
   else
    stc("You are in Human form.\n\r", ch);
  }
  if( IS_CLASS(ch, CLASS_MONK))
  {
   banner2_to_char("TECHNIQUES", ch);
   if( HAS_TECH(ch, TECH_ELBOW))
    stc("You have learned the combat technique of the elbow.\n\r", ch);
   if( HAS_TECH(ch, TECH_BACKFIST))
    stc("You have learned the combat technique of the backfast.\n\r", ch);
   if ( HAS_TECH(ch, TECH_SWEEP))
    stc("You have learned the combat technique of sweeping.\n\r", ch);
   if( HAS_TECH(ch, TECH_REVERSE))
    stc("You have learned the combat technique of reverse sweeping.\n\r", ch);
   if( HAS_TECH(ch, TECH_KNEETHRUST))
    stc("You have learned the combat technique of the kneethrust.\n\r", ch);
   if( HAS_TECH(ch, TECH_THRUSTKICK))
    stc("You have learned the combat technique of the thrustkick.\n\r", ch);
   if( HAS_TECH(ch, TECH_ROUNDHOUSE))
    stc("You have learned the combat technique of the roundhouse.\n\r", ch);
   if( HAS_TECH(ch, TECH_HEARTPUNCH))
    stc("You have learned the combat technique of the heartpunch.\n\r", ch);
   if( HAS_TECH(ch, TECH_PALMSTRIKE))
    stc("You have learned how to palmstrike your enemies.\n\r", ch);
   if( HAS_TECH(ch, TECH_DENJIN))
    stc("You have learned how to hurl fireballs at your enemies.\n\r", ch);
   if( HAS_TECH(ch, TECH_CUSTOM))
    stc("You have learned how to use your attacks at lighting speed.\n\r", ch);
   if( HAS_TECH(ch, TECH_VOICE))
    stc("You have learned to call the spirits to aid you.\n\r", ch);
   if( HAS_TECH(ch, TECH_BLOCK))
    stc("You have learned how to block your enemies attacks.\n\r", ch);
   if( HAS_TECH(ch, TECH_CELEST))
    stc("You have learned to walk to planes.\n\r", ch);
   if( HAS_TECH(ch, TECH_STANCE))
    stc("You have mastered the arts of the combat stance.\n\r", ch);
   if( HAS_TECH(ch, TECH_UNARMED))
    stc("You fight with precision while unarmed.\n\r", ch);
   if( HAS_TECH(ch, TECH_HEALTOUCH))
    stc("You have been granted a healing touch.\n\r", ch);
   if( HAS_TECH(ch, TECH_DEATHTOUCH))
    stc("You have been granted the knowledge of a killing touch.\n\r", ch);
   if( HAS_TECH(ch, TECH_HEATMETAL))
    stc("You know how to heat an opponents armor with a thought.\n\r", ch);
   if( HAS_TECH(ch, TECH_CLAIR))
    stc("You can sense your foes anywhere in the world.\n\r", ch);
   sprintf( buf, "Current Chi: %d          Max Chi: %d\n\r", GET_POWERS(ch, MONK_CURR_CHI), 
            GET_POWERS(ch, MONK_MAX_CHI));
   stc( buf, ch);
   sprintf( buf, "Current Focus: %d        Max Focus: %d\n\r", GET_POWERS(ch, MONK_CURR_FOC), 
            GET_POWERS(ch, MONK_MAX_FOC));
   stc( buf, ch);
   sprintf( buf, "Block counter %d         Custom Combo: %d\n\r", GET_POWERS(ch, MONK_BLOCK),
           GET_POWERS(ch, MONK_CUSTOM));
   stc( buf, ch);
   sprintf( buf, "Cost for new technique: %d\n\r", ((1+count_techniques(ch)) * CHI_COST));
   stc(buf, ch);
   if( GET_POWERS(ch, MONK_MAX_CHI) < MAX_CHI)
   {
    sprintf( buf, "Cost to gain chi: %d\n\r", ((1+GET_POWERS(ch, MONK_MAX_CHI)) * (2 * CHI_COST)));
    stc(buf, ch);
   }
   if( GET_POWERS(ch, MONK_MAX_FOC) < MAX_FOCUS)
   {
    sprintf( buf, "Cost to gain focus: %d\n\r", ((1+ GET_POWERS(ch, MONK_MAX_FOC)) * (CHI_COST / 10)));
    stc( buf, ch);
   }
   switch( ch->pcdata->powers[MONK_CLASS] )
   {
      case PATH_NONE:
        sprintf( buf, "Path: None\n\r");
	break;
      case PATH_GAIA:
        sprintf( buf, "Path: Gaia\n\r");
        break;
      case PATH_INFERNO:
        sprintf( buf, "Path: Inferno\n\r");
        break;
      case PATH_PEARL:
        sprintf( buf, "Path: Pearl\n\r");
        break;
      case PATH_TSUNAMI:
        sprintf( buf, "Path: Tsunami\n\r");
	break;
      case PATH_TEMPEST:
	sprintf( buf, "Path: Tempest\n\r");
        break;
      default:
	sprintf( buf, "BUGGED: auto correcting.  Setting to None\n\r");
	GET_POWERS(ch, MONK_CLASS) = PATH_NONE;
	break;
   }
	stc( buf, ch);
   return;
  }
  if( IS_CLASS(ch, CLASS_ALIEN))
  {
   banner2_to_char("Evolutions", ch);
   sprintf( buf, "Nervous System: %d\n\r", GET_POWERS(ch, ALIEN_SPEED));
   stc( buf, ch);
   sprintf( buf, "Muscular System: %d\n\r", GET_POWERS(ch, ALIEN_STR));
   stc( buf, ch);
   sprintf( buf, "Carapace: %d\n\r", GET_POWERS(ch, ALIEN_ARMOR));
   stc( buf, ch);
   sprintf(buf, "Size: %d\n\r", GET_POWERS(ch, ALIEN_SIZE));
   stc( buf, ch);
   sprintf(buf, "Senses: %d\n\r", GET_POWERS(ch, ALIEN_SENSE));
   stc( buf, ch);
   sprintf( buf, "DNA: %d\n\r", GET_POWERS(ch, ALIEN_DNA));
   stc(buf, ch);
   sprintf( buf, "Primal urge: %d\n\r", GET_POWERS(ch, ALIEN_PRIMAL));
   stc(buf, ch);
   sprintf( buf, "Pain Tolerance: %d\n\r", GET_POWERS(ch, ALIEN_PAIN));
   stc(buf, ch);
   switch(GET_POWERS(ch, ALIEN_CLASS))
   {
    default:
    break;
    case ALIEN_DRONE:
     stc("You are a drone.\n\r", ch);
     break;
    case ALIEN_RUN:
     stc("You are a runner.\n\r", ch);
     break;
    case ALIEN_WAR:
     stc("You are a warrior.\n\r", ch);
     break;
    case ALIEN_PRAE:
     stc("You are a royal praetorian guardian.\n\r", ch);
     break;
    case ALIEN_GOR:
     stc("You are a gorilla hybrid.\n\r", ch);
     break;
    case ALIEN_SCORP:
     stc("You are a scorpion hybrid.\n\r", ch);
     break;
   }
   return;
  }
 if( IS_CLASS(ch, CLASS_SHADOW) )
 {
  char buf[MAX_STRING_LENGTH];
  int i;

  banner2_to_char("Masteries", ch);
  sprintf( buf, "Backstab     [#1" );
  for( i = 1; i < GET_POWERS(ch, SHADOW_BACKSTAB)+1; i++ )
   sprintf( buf+strlen(buf), "*" );
  for( i = 1; i < (5 - GET_POWERS(ch, SHADOW_BACKSTAB)+1); i++ )
   sprintf( buf+strlen(buf), " ");
  if( GET_POWERS(ch, SHADOW_BACKSTAB) < 5 )
   sprintf( buf+strlen(buf), "#n]  You have attained [#1%d#n] of [#1%d#n] points to your next level.\n\r", GET_POWERS(ch, SHADOW_LEV_BS), ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_BACKSTAB) ) );
  else
   sprintf( buf+strlen(buf), "#n]\n\r" );
  stc(buf,ch);
  sprintf( buf, "Trap         [#1" );
  for( i = 1; i < GET_POWERS(ch, SHADOW_TRAP)+1; i++ )
   sprintf( buf+strlen(buf), "*" );
  for( i = 1; i < (5 - GET_POWERS(ch, SHADOW_TRAP)+1); i++ )
   sprintf( buf+strlen(buf), " ");
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
   sprintf( buf+strlen(buf), "#n]  You have attained [#1%d#n] of [#1%d#n] points to your next level.\n\r", GET_POWERS(ch, SHADOW_LEV_TRAP), ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRAP) ) );
  else
   sprintf( buf+strlen(buf), "#n]\n\r" );
  stc(buf,ch);
  sprintf( buf, "Rune         [#1" );
  for( i = 1; i < GET_POWERS(ch, SHADOW_RUNE)+1; i++ )
   sprintf( buf+strlen(buf), "*" );
  for( i = 1; i < (5 - GET_POWERS(ch, SHADOW_RUNE)+1); i++ )
   sprintf( buf+strlen(buf), " ");
  if( GET_POWERS(ch, SHADOW_RUNE) < 5 )
   sprintf( buf+strlen(buf), "#n]  You have attained [#1%d#n] of [#1%d#n] points to your next level.\n\r", GET_POWERS(ch, SHADOW_LEV_RUNE), ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_RUNE) ) );
  else
   sprintf( buf+strlen(buf), "#n]\n\r" );
  stc(buf,ch);
  sprintf( buf, "Tracking     [#1" );
  for( i = 1; i < GET_POWERS(ch, SHADOW_TRACK)+1; i++ )
   sprintf( buf+strlen(buf), "*" );
  for( i = 1; i < (5 - GET_POWERS(ch, SHADOW_TRACK)+1); i++ )
   sprintf( buf+strlen(buf), " ");
  if( GET_POWERS(ch, SHADOW_TRACK) < 5 )
   sprintf( buf+strlen(buf), "#n]  You have attained [#1%d#n] of [#1%d#n] points to your next level.\n\r", GET_POWERS(ch, SHADOW_LEV_TRK), ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRACK) ) );
  else
   sprintf( buf+strlen(buf), "#n]\n\r" );
  stc(buf,ch);
  return;
 }
 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  char buf[MAX_STRING_LENGTH];

  banner2_to_char("Disciplines", ch);
  sprintf(buf,"         Bear        [%2d]     Lynx         [%2d]     Boar         [%2d]\n\r",
   GET_POWERS(ch,DISC_BEAR),GET_POWERS(ch,DISC_LYNX),GET_POWERS(ch,DISC_BOAR));
  stc(buf,ch);
  sprintf(buf,"         Owl         [%2d]     Spider       [%2d]     Wolf         [%2d]\n\r",
   GET_POWERS(ch,DISC_OWL),GET_POWERS(ch,DISC_SPIDER),GET_POWERS(ch,DISC_WOLF));
  stc(buf,ch);
  sprintf(buf,"         Hawk        [%2d]     Mantis       [%2d]     Raptor       [%2d]\n\r",
   GET_POWERS(ch,DISC_HAWK),GET_POWERS(ch,DISC_MANTIS),GET_POWERS(ch,DISC_RAPTOR));
  stc(buf,ch);
  sprintf(buf,"         Luna        [%2d]     Pain         [%2d]     Congregation [%2d]\n\r",
   GET_POWERS(ch,DISC_LUNA),GET_POWERS(ch,DISC_PAIN),GET_POWERS(ch,DISC_CONG));
  stc(buf,ch);
  sprintf(buf,"                            Discipline Points [%8d]\n\r",GET_POWERS(ch,WOLF_POINTS));
  stc(buf,ch);
  return;
 }
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  char buf[MAX_STRING_LENGTH];
  char age[MAX_STRING_LENGTH];
  
  switch( GET_POWERS(ch,VAMP_AGE) )
  {
   case AGE_NONE:       sprintf(age,"Childe");     break;
   case AGE_CAITIFF:    sprintf(age,"Caitiff");    break;
   case AGE_ANCILLA:    sprintf(age,"Ancilla");    break;
   case AGE_ELDER:      sprintf(age,"Elder");      break;
   case AGE_METHUSELAH: sprintf(age,"Methuselah"); break;
   default:             sprintf(age,"BUGGED");     break;
  }
  banner2_to_char("Disciplines", ch);
  sprintf(buf,"         Obtenebration  [%2d]     Presence  [%2d]     Quietus     [%2d]\n\r",
   GET_POWERS(ch,DISC_OBTEN),GET_POWERS(ch,DISC_PRESENCE),GET_POWERS(ch,DISC_QUIETUS));
  stc(buf,ch);
  sprintf(buf,"         Thaumaturgy    [%2d]     Auspex    [%2d]     Dominate    [%2d]\n\r",
   GET_POWERS(ch,DISC_THAUM),GET_POWERS(ch,DISC_AUSPEX),GET_POWERS(ch,DISC_DOMINATE));
  stc(buf,ch);
  sprintf(buf,"         Obfuscate      [%2d]     Protean   [%2d]     Serpentis   [%2d]\n\r",
   GET_POWERS(ch,DISC_OBFUSCATE),GET_POWERS(ch,DISC_PROTEAN),GET_POWERS(ch,DISC_SERPENTIS));
  stc(buf,ch);
  sprintf(buf,"         Vicissitude    [%2d]     Daimoinon [%2d]     Animalism   [%2d]\n\r",
   GET_POWERS(ch,DISC_VICIS),GET_POWERS(ch,DISC_DAIMOINON),GET_POWERS(ch,DISC_ANIMALISM));
  stc(buf,ch);
  sprintf(buf,"         Celerity       [%2d]     Potence   [%2d]     Fortitude   [%2d]\n\r",
   GET_POWERS(ch,DISC_CELERITY),GET_POWERS(ch,DISC_POTENCE),GET_POWERS(ch,DISC_FORTITUDE));
  stc(buf,ch);
  sprintf(buf,"                            Discipline Points [%10d]\n\r",GET_POWERS(ch,VAMP_POINTS));
  stc(buf,ch);
  sprintf(buf,"                                          Age [%10s]\n\r",age);
  stc(buf,ch);
  return;
 }
 /* add new classes here*/
  else
   return;
}

//use this as a pass through to do all the work for advancing in a class

void improve_class(CHAR_DATA *ch, int field)
{
 int cost;
 char buf[MAX_STRING_LENGTH];
 int (*handle)();

 if( IS_NPC(ch) || IS_CLASS(ch, CLASS_NONE))
  return; 
 cost = 0;
 if( IS_CLASS(ch,CLASS_JEDI) )
 {
  if( field == JEDI_STANCE )
  {
   cost = 25000000;
   if( GET_POWERS(ch,field) >= 1 )
   {
    stc("You have already learned all that is possible without combat.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_DEFENSE )
  {
   cost = 25000000;
   if( GET_POWERS(ch,field) >= 1 )
   {
    stc("You have already learned all that is possible without combat.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_ATTACK )
  {
   cost = 25000000;
   if( GET_POWERS(ch,field) >= 1 )
   {
    stc("You have already learned all that is possible without combat.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_HEAL )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_LIGHT)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;   
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;                     
   return;
  }
  else if( field == JEDI_FORCE_ABSORB )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000); 
   if( (GET_POWERS(ch,JEDI_LIGHT)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_SHIELD )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_LIGHT)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_MIND_TRICK )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000); 
   if( (GET_POWERS(ch,JEDI_LIGHT)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_SIGHT )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_LIGHT)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_SPEED )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_NEUTRAL)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_JUMP )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_NEUTRAL)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_PUSH )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_NEUTRAL)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_PULL )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_NEUTRAL)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_GRIP )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_DARK)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_RAGE )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_DARK)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_DRAIN )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_DARK)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_LIGHTNING )
  {
   if( (handle = dlsym(jedilib,"snarf_jedi_total_force")) == NULL )
   {
    sprintf(buf,"%s",dlerror());
    log_string(buf);
   }
   else
    cost = (((*handle)(ch) + 1) * 150000);
   if( (GET_POWERS(ch,JEDI_DARK)/2) < GET_POWERS(ch,field) )
   {
    stc("You lack proper discipline with the force to learn this.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_FORCE_POOL_MAX )
  {
   cost = 100000;
   if( GET_POWERS(ch,field) >= 1000 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  }
  else if( field == JEDI_LIGHT )
  {
   cost = 500000;
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,JEDI_DARK) >= 5 )
   {
    stc("The force prevents you from furthering your knowledge, worshipper of darkness.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,JEDI_LIGHT) + GET_POWERS(ch,JEDI_DARK) + GET_POWERS(ch,JEDI_NEUTRAL) >= 20 )
   {
    stc("You are already as mastered with the force as you can become.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {   
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  } 
  else if( field == JEDI_NEUTRAL )
  {
   cost = 500000;
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,JEDI_LIGHT) + GET_POWERS(ch,JEDI_DARK) + GET_POWERS(ch,JEDI_NEUTRAL) >= 20 )
   {
    stc("You are already as mastered with the force as you can become.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {   
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  } 
  else if( field == JEDI_DARK )
  {
   cost = 500000;
   if( GET_POWERS(ch,field) >= 10 )
   {
    stc("You have already learned all that is possible.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,JEDI_LIGHT) >= 5 )
   {
    stc("The force prevents you from furthering your knowledge, worshipper of light.\n\r",ch);
    return;
   }
   if( GET_POWERS(ch,JEDI_LIGHT) + GET_POWERS(ch,JEDI_DARK) + GET_POWERS(ch,JEDI_NEUTRAL) >= 20 )
   {
    stc("You are already as mastered with the force as you can become.\n\r",ch);
    return;
   }
   if( ch->exp < cost )
   {   
    act2("You lack the $D exp to further your knowledge.",ch,NULL,NULL,TO_CHAR,cost);
    return;
   }
   stc("Your knowledge increases.\n\r",ch);
   GET_POWERS(ch,field)++;
   ch->exp -= cost;
   return;
  } 
  return;
 }
 if( IS_CLASS(ch, CLASS_DRAGON))
 {
  if( field == DRAGON_MIGHT)
  {
   if( GET_POWERS(ch, DRAGON_MIGHT) >= 10)
   {
    stc("You have already as mighty as you can get.\n\r", ch);
    return;
   }
   cost = (GET_POWERS(ch, DRAGON_MIGHT)+1) * POINT_STEP;
   if( GET_POWERS(ch, DRAGON_POINTS) < cost)
   {
    stc("You lack the power to incease your might.\n\r", ch);
    return;
   }
   GET_POWERS(ch, DRAGON_MIGHT)++;
   GET_POWERS(ch, DRAGON_POINTS) -= cost;
   stc("Your might has increased.\n\r", ch);
   return;
  }
  if( field == DRAGON_TOUGH)
  {
   if( GET_POWERS(ch, DRAGON_TOUGH) >= 10)
   {
    stc("You have already as tough as you can get.\n\r", ch);
    return;
   }
   cost = (GET_POWERS(ch, DRAGON_TOUGH)+1) * POINT_STEP;
   if( GET_POWERS(ch, DRAGON_POINTS) < cost)
   {
    stc("You lack the power to incease your toughness.\n\r", ch);
    return;
   }
   GET_POWERS(ch, DRAGON_TOUGH)++;
   GET_POWERS(ch, DRAGON_POINTS) -= cost;
   stc("Your toughness has increased.\n\r", ch);
   return;
  }
  if( field == DRAGON_SPEED)
  {
   if( GET_POWERS(ch, DRAGON_SPEED) >= 10)
   {
    stc("You have already as fast as you can get.\n\r", ch);
    return;
   }
   cost = (GET_POWERS(ch, DRAGON_SPEED)+1) * POINT_STEP;
   if( GET_POWERS(ch, DRAGON_POINTS) < cost)
   {
    stc("You lack the power to incease your speed.\n\r", ch);
    return;
   }
   GET_POWERS(ch, DRAGON_SPEED)++;
   GET_POWERS(ch, DRAGON_POINTS) -= cost;
   stc("Your speed has increased.\n\r", ch);
   return;
  }
  if( field == DRAGON_MAGIC)
  {
   if( GET_POWERS(ch, DRAGON_MAGIC) >= 10)
   {
    stc("You have already mastered the magical arts.\n\r", ch);
    return;
   }
   cost = (GET_POWERS(ch, DRAGON_MAGIC)+1) * POINT_STEP;
   if( GET_POWERS(ch, DRAGON_POINTS) < cost)
   {
    stc("You lack the power to incease your magic.\n\r", ch);
    return;
   }
   GET_POWERS(ch, DRAGON_MAGIC)++;
   GET_POWERS(ch, DRAGON_POINTS) -= cost;
   stc("Your magical powers have increased.\n\r", ch);
   return;
  }
  if( field == DRAGON_AGE)
  { 
   if( GET_POWERS(ch, DRAGON_AGE) >= 10000)
   {
    stc("Your age already surpasses all time.\n\r", ch);
    return;
   }
   cost = (GET_POWERS(ch, DRAGON_AGE)+1)*2;
   if( GET_POWERS(ch, DRAGON_POINTS) < cost)
   {
    stc("You lack the power to incease your age.\n\r", ch);
    return;
   }
   GET_POWERS(ch, DRAGON_AGE)++;
   GET_POWERS(ch, DRAGON_POINTS) -= cost;
   stc("You are one dragon year older.\n\r", ch);
   return;
  }
  return;
 }
 if( IS_CLASS(ch, CLASS_MONK))
 {
  if( field == TECH_CHI)
  {
   cost = ((1 + GET_POWERS(ch, MONK_MAX_CHI)) * (CHI_COST*2));
   if( ch->exp < cost)
   {
    sprintf( buf, "You need %d experience points for that.\n\r", cost);
    stc(buf, ch);
    return;
   }
   if( GET_POWERS(ch, MONK_MAX_CHI) >= MAX_CHI)
   {
    stc("Your spirit is already fully attunded.\n\r", ch);
    return;
   } 
   ch->exp -= cost;
   GET_POWERS(ch, MONK_MAX_CHI)++;
   stc("Your spirit is more attuned with it's surroundings.\n\r", ch);
   return;
  }
  else if( field == MONK_MAX_FOC)
  {
   cost = ((1 + GET_POWERS(ch, field)) * (CHI_COST/10));
   if( ch->exp < cost)
   {
    sprintf( buf, "You need %d experience points for that.\n\r", cost);
    stc(buf, ch);
    return;
   }
   if( GET_POWERS(ch, field) >= MAX_FOCUS)
   {
    stc("Your mind is fully focused.\n\r", ch);
    return;
   }
   ch->exp -= cost;
   GET_POWERS(ch, field)++;
   stc("Your mind has become more focused.\n\r", ch);
   return;
  }   
  else
  {
   if( HAS_TECH(ch, field))
   {
    stc("But you already have that power.\n\r", ch);
    return;
   }
   cost = (1+count_techniques(ch)) * 1000000;
   if( ch->exp < cost)
   {
    sprintf(buf, "You cant afford the %d experience cost.\n\r", cost);
    stc(buf, ch);
    return;
   }
   SET_BIT(GET_POWERS(ch,MONK_TECH), field);
   ch->exp -= cost;
   stc("You have learned a new skill.\n\r", ch);
   return;
  }
  return;
 }
 if( IS_CLASS(ch, CLASS_ALIEN))
 {
  if( GET_POWERS(ch, field) >= 10)
  {
   stc("You have fully evolved.\n\r", ch);
   return;
  }
  cost = (GET_POWERS(ch, field)+1) * 500;
  if( GET_POWERS(ch, ALIEN_DNA) < cost)
  {
   stc("You lack the genetic material to evolve that further.\n\r", ch);
   return;
  }
  ch->pcdata->powers[field]++;
  ch->pcdata->powers[ALIEN_DNA] -= cost;
  stc("You absorb the DNA of others and have evolved.\n\r", ch);
  return;
 }
 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  if( GET_POWERS(ch,field) >= 10)
  {
   if( GET_POWERS(ch, field) > 10 )
	GET_POWERS(ch, field) = 10;
   stc("You have fully mastered that discipline.\n\r",ch);
   return;
  }
  cost = (GET_POWERS(ch,field)+1) * 300;
  if( GET_POWERS(ch,WOLF_POINTS) < cost)
  {
   stc("You lack the discipline points to further learn that.\n\r",ch);
   return;
  }
  GET_POWERS(ch,field)++;
  GET_POWERS(ch,WOLF_POINTS) -= cost;
  stc("You further your knowledge of your discipline.\n\r",ch);
  return;
 }
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  if( GET_POWERS(ch,field) >= 10)
  {
   stc("You have fully mastered that discipline.\n\r",ch);
   return;   
  }
  cost = (GET_POWERS(ch,field)+1) * 300;
  if( GET_POWERS(ch,VAMP_POINTS) < cost)
  {
   stc("You lack the discipline points to further learn that.\n\r",ch);
   return;
  }
  GET_POWERS(ch,field)++;
  GET_POWERS(ch,VAMP_POINTS) -= cost;
  stc("You further your knowledge of your discipline.\n\r",ch);
  return;
 }
 /* add new class stuff here */
 else
  return;
}

int class_parry(CHAR_DATA *ch)
 {
  int chance;
  int (*function)();

  chance = 0;
  if( IS_CLASS(ch,CLASS_DROW) )
  {
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_SPEED) )
    chance += 6;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SPIDERFORM) )
    chance += 6;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    chance -= 4;
   if( weather_info.sunlight == SUN_DARK )
    chance++;
   else
    chance--;
   if( IS_SET(ch->newbits,NEW_DARKNESS) )
    chance++;
   else
    chance--;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
    chance += 2;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    chance++;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    chance++;
   else
    chance--;
   chance += drow->parr;
  }
  if( IS_CLASS(ch, CLASS_APOCRYPHIC))
  {
   chance += 4;
   if( has_power_level(ch, SCHOOL_EARTH, 5))
    chance += 10;
   if( has_power_level(ch, SCHOOL_WIND, 2))
    chance += 7;
   if( has_power_level(ch, SCHOOL_WIND, 7))
    chance += 7;
   if( has_power_level(ch, SCHOOL_LUCK, 2))
    chance += number_range(-5, 20);
   if( has_power_level(ch, SCHOOL_ORDER, 5))
    chance += 10;
   chance += apoc->parr;
  }
  if( IS_CLASS(ch, CLASS_DRAGON))
  {
    if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
      chance += GET_POWERS(ch, DRAGON_SPEED);
   chance += drag->parr;
  }
  if( IS_CLASS(ch,CLASS_JEDI) )
  {
   chance += (GET_POWERS(ch,JEDI_DEFENSE)/2.5);
   chance += (GET_POWERS(ch,JEDI_RANK)/3);
   function = dlsym(jedilib,"jedi_eq_counter");
   if( (*function)(ch,TRUE) > GET_POWERS(ch,JEDI_RANK) )
    chance /= 2;
   else
    chance *= 1.5;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
    chance -= 2;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
    chance -= 4;
   chance += jedi->parr;
  }
  if( IS_CLASS(ch, CLASS_MONK))
  {
   chance += GET_POWERS(ch, MONK_CURR_CHI);
   chance += (GET_POWERS(ch, MONK_CURR_FOC)/10);
   chance += number_range( 2, 5);
   chance += monk->parr;
  } 
  if( IS_CLASS(ch, CLASS_ALIEN))
  {
   chance += GET_POWERS(ch, ALIEN_SPEED)*1.10;
   chance += (GET_POWERS(ch,ALIEN_SIZE)/2);
   chance += alie->parr;
  }
  if( IS_SET(ch->warp, WARP_CLUMSY))
   chance -= 10;
  if( IS_SET(ch->in_room->affected_by, AFF_ROOM_ICED))
  {
    if( has_power(ch, SCHOOL_WATER))
     chance += 10;
    else
     chance -= 10;
  }  
  if((ch->in_room->area != NULL) && IS_SET(ch->in_room->area->affected_by, AFF_AREA_FLOOD))
  {
    if( has_power(ch, SCHOOL_WATER))
     chance += 10;
    else      
     chance -= 10;
  }
  if( IS_CLASS(ch, CLASS_SHADOW) )
  {
   chance += ( (GET_POWERS(ch, SHADOW_BACKSTAB) + GET_POWERS(ch, SHADOW_TRACK)
          + GET_POWERS(ch, SHADOW_TRAP) + GET_POWERS(ch, SHADOW_RUNE) )/2 );
   chance += shad->parr;
  }
  if( IS_CLASS(ch, CLASS_WOLF) )
  {
   chance += GET_POWERS(ch,DISC_LYNX)/5;
   chance += GET_POWERS(ch,DISC_MANTIS)/5;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_VANISH))
    chance += 1;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    chance += 1;
   else
    chance -= 4;
   if( GET_POWERS(ch,WOLF_FORM) < 1 )
    chance -= 2;
   if( GET_POWERS(ch,WOLF_FORM) == 3 )
    chance += 2;
   chance += were->parr;
  }
  if( IS_CLASS(ch,CLASS_VAMPIRE) )
  { 
   chance += GET_POWERS(ch,DISC_CELERITY)/5;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLASHING))
    chance++;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    chance += 1;
   else if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) && !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
    chance -= 10;
   else
    chance -= 6;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) )
    chance ++;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
    chance ++;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BEAST) )
    chance -= 3;
   switch( GET_POWERS(ch,VAMP_FORM) )
   {
    case FORM_NORMAL:  chance += 1; break;
    case FORM_SERPENT: chance += 2; break;
    case FORM_ZULO:    chance -= 3; break;
    case FORM_DRAGON:  chance -= 4; break;
    case FORM_BAT:     chance += 3; break;
    case FORM_WOLF:    chance += 2; break;
    case FORM_OBJECT:  chance += 1; break;
    case FORM_MIST:    chance -= 4; break;
   }
   switch( GET_POWERS(ch,VAMP_AGE) )
   {
    default:                          break;
    case AGE_NONE:       chance -= 3; break;
    case AGE_CAITIFF:    chance -= 2; break;
    case AGE_ANCILLA:                 break;
    case AGE_ELDER:      chance += 1; break;
    case AGE_METHUSELAH: chance += 2; break;
   }
   chance += vamp->parr;
  }
  if( ASKILL(ch,AS_DEFENSE) == 100 )
   chance += 1;
  return chance;
}

int class_dodge(CHAR_DATA *ch)
{
 int chance;
 int (*function)();

  chance = 0;
  if( IS_CLASS(ch,CLASS_DROW) )
  {
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_SPEED) )
    chance += 6;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SPIDERFORM) )
    chance += 6;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    chance -= 4;
   if( weather_info.sunlight == SUN_DARK )
    chance++;  
   else
    chance--;   
   if( IS_SET(ch->newbits,NEW_DARKNESS) )
    chance++;
   else
    chance--;   
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
    chance += 2;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    chance++;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    chance++;
   else
    chance--;
   chance += drow->dodg;
  }
  if( IS_CLASS(ch, CLASS_APOCRYPHIC))
  {
   chance += 4;
   if( has_power_level(ch, SCHOOL_WATER, 6))    
    chance += 10;  
   if( has_power_level(ch, SCHOOL_WIND, 1))  
    chance += 7;  
   if( has_power_level(ch, SCHOOL_WIND, 6))  
    chance += 7;  
   if( has_power_level(ch, SCHOOL_LUCK, 1))  
    chance += number_range(-5, 20);  
   if( has_power_level(ch, SCHOOL_ORDER, 4))  
    chance += 10;  
   if( has_power_level(ch, SCHOOL_LIFE, 4))  
    chance += 10;
   chance += apoc->dodg;
  }
  if( IS_CLASS(ch,CLASS_JEDI) )
  {
   chance += (GET_POWERS(ch,JEDI_DEFENSE)/2.5);
   chance += (GET_POWERS(ch,JEDI_RANK)/3);
   function = dlsym(jedilib,"jedi_eq_counter");
   if( (*function)(ch,TRUE) > GET_POWERS(ch,JEDI_RANK) )
    chance /= 2;
   else
    chance *= 1.5;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )  
    chance -= 2; 
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
    chance -= 4;
   chance += jedi->dodg;
  }
  if( IS_CLASS(ch, CLASS_DRAGON))
  {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_HUMAN)
   {
    chance += GET_POWERS(ch, DRAGON_SPEED);
   }
   chance += drag->dodg;
  }
  if( IS_CLASS(ch, CLASS_MONK))
  { 
   chance += GET_POWERS(ch, MONK_CURR_CHI);
   chance += (GET_POWERS(ch, MONK_CURR_FOC)/10);
   chance += number_range( 2, 5);
   chance += monk->dodg;
  }
  if( IS_CLASS(ch, CLASS_ALIEN))
  {
   chance += GET_POWERS(ch, ALIEN_SPEED)*1.10;;
   chance += (GET_POWERS(ch,ALIEN_SIZE)/2);
   chance += alie->dodg;
  }
  if( (ch->in_room->area != NULL) && IS_SET( ch->in_room->affected_by, AFF_ROOM_DEFILE))     
  {   
    if( has_power(ch, SCHOOL_EVIL))   
     chance += 10;   
    else   
     chance -= 10;   
  }
  if( (ch->in_room->area != NULL) && IS_SET(ch->in_room->area->affected_by, AFF_AREA_FLOOD))
  {
    if( has_power(ch, SCHOOL_WATER))
     chance += 10;
    else
     chance -= 10;
  }
  if( IS_CLASS(ch, CLASS_SHADOW) ) 
  {
   chance += ( (GET_POWERS(ch, SHADOW_BACKSTAB) + GET_POWERS(ch, SHADOW_TRACK)
          + GET_POWERS(ch, SHADOW_TRAP) + GET_POWERS(ch, SHADOW_RUNE) )/2 );
   chance += shad->dodg;
  }
  if( IS_CLASS(ch,CLASS_WOLF) )
  {
   chance += GET_POWERS(ch,DISC_LYNX)/5;
   chance += GET_POWERS(ch,DISC_OWL)/5;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_VANISH))
    chance += 1;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    chance += 1;
   else
    chance -= 4;
   if( GET_POWERS(ch,WOLF_FORM) < 1 )
    chance -= 2;
   if( GET_POWERS(ch,WOLF_FORM) == 4 )
    chance += 2;
   chance += were->dodg;
  }
    if( IS_CLASS(ch,CLASS_VAMPIRE) )
  {
   chance += GET_POWERS(ch,DISC_CELERITY)/5;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLASHING))
    chance++;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    chance += 1;
   else if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) && !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
    chance -= 10;
   else
    chance -= 6;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) )
    chance ++;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
    chance ++;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BEAST) )
    chance -= 3;
   switch( GET_POWERS(ch,VAMP_FORM) )
   {
    case FORM_NORMAL:  chance += 1; break;  
    case FORM_SERPENT: chance += 2; break;
    case FORM_ZULO:    chance -= 3; break;
    case FORM_DRAGON:  chance -= 4; break;
    case FORM_BAT:     chance += 3; break;
    case FORM_WOLF:    chance += 2; break;
    case FORM_OBJECT:  chance += 1; break;
    case FORM_MIST:    chance -= 4; break;
   }
   switch( GET_POWERS(ch,VAMP_AGE) )
   {
    default:                          break;
    case AGE_NONE:       chance -= 3; break;
    case AGE_CAITIFF:    chance -= 2; break;
    case AGE_ANCILLA:                 break;
    case AGE_ELDER:      chance += 1; break;
    case AGE_METHUSELAH: chance += 2; break;
   }
   chance += vamp->dodg;
  }
  if( ASKILL(ch,AS_DEFENSE) == 100 )
   chance += 1;
 return chance;
}

int class_num_attack(CHAR_DATA *ch)
{
 int count = 0;

 if( ch->max_move >= 10000 )
  count += (ch->max_move / 10000);
 if( IS_CLASS(ch,CLASS_DROW) )
 {
  if( weather_info.sunlight == SUN_DARK )
   count++;
  else
   count--;
  if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
   count += 2;
  else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
   count++;
  else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
   count++;
  else
   count--;
  if( IS_SET(ch->newbits,NEW_DARKNESS) )
   count++;
  else
   count--;
  if( IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_SPEED) )
   count++;
  if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SPIDERFORM) )
   count++;
  count += drow->atta;
 }  
 if( IS_CLASS(ch, CLASS_APOCRYPHIC))
 {
  count++;
  if( has_power_level(ch, SCHOOL_TIME, 1))
   count += 1;
  if( has_power_level(ch, SCHOOL_TIME, 4))
   count += 1;
  if( has_power_level(ch, SCHOOL_TIME, 6))
   count += 1;
  if( number_range(1,5) == 2)
   count += 1;   
  if( number_range(1,10) == 2)
   count += 1;
  if( number_range(1,20) == 2)
   count += 1;
  count += apoc->atta;
 }
 if( IS_CLASS(ch,CLASS_JEDI) )
 {
  count += (GET_POWERS(ch,JEDI_ATTACK)/2.5);
  count += (GET_POWERS(ch,JEDI_RANK)/3);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
   count++;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   if( number_range(1,6) == 4 )
    count++;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
   count += 2;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
   count -= 4;
  count += jedi->atta;
 }
 if( IS_CLASS(ch, CLASS_DRAGON))
 {
  if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   count += (GET_POWERS(ch, DRAGON_SPEED) / 3);
  else
   count += (GET_POWERS(ch, DRAGON_SPEED) / 1.25);
  count += drag->atta;
 }
 if( IS_CLASS(ch, CLASS_MONK))
 {
  count += GET_POWERS(ch, MONK_CURR_CHI);
  count += monk->atta;
 }
 if( IS_CLASS(ch, CLASS_ALIEN))
 {
  count += (GET_POWERS(ch, ALIEN_SPEED) / 5);
  count += GET_POWERS(ch, ALIEN_PRIMAL) / 2;
  count += alie->atta;
 }
 if( IS_CLASS(ch, CLASS_SHADOW) )
 {
  count += 2;
  count += shad->atta;
 }
 if( IS_CLASS(ch,CLASS_WOLF) )
 {
  count += GET_POWERS(ch,DISC_SPIDER)/5;
  count += GET_POWERS(ch,DISC_HAWK)/5;
  if( GET_POWERS(ch,WOLF_FORM) < 1 )
   count -= 1;
  if( GET_POWERS(ch,WOLF_FORM) == 2 )
   count -= 3;
  if( GET_POWERS(ch,WOLF_FORM) == 5 )
   count += 1;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    count += 2;
   else
    count -= 4;
  count += were->atta;
 }
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  count += GET_POWERS(ch,DISC_CELERITY)/6;
  if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLASHING) )
   count++;
  if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BAAL) )
   count++;
  if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
   count++;
  else
   count -= 10;
  if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) )
   count -= 5;
  if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
   count += 1;
  if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BEAST) )
   count += 3;
  switch( GET_POWERS(ch,VAMP_FORM) )
  {
   case FORM_NORMAL:              break;
   case FORM_SERPENT: count += 2; break;
   case FORM_ZULO:    count -= 3; break;
   case FORM_DRAGON:  count -= 4; break;
   case FORM_BAT:     count += 1; break;
   case FORM_WOLF:    count += 1; break;
   case FORM_OBJECT:              break;
   case FORM_MIST:    count -= 4; break;
  }
  switch( GET_POWERS(ch,VAMP_AGE) )
  {
   default:                         break;
   case AGE_NONE:       count -= 2; break;
   case AGE_CAITIFF:    count -= 1; break;
   case AGE_ANCILLA:                break;
   case AGE_ELDER:      count += 1; break;
   case AGE_METHUSELAH: count += 2; break;
  }
  count += vamp->atta;
 }
 if( is_affected(ch,skill_lookup("haste")) )
  count += 2;
 if( is_affected(ch,skill_lookup("slow")) )
  count -= 2;
 if( ASKILL(ch,AS_ATTACK) >= 75 )
  count++;
 if( ASKILL(ch,AS_ATTACK) == 100 )
  count++;
 if( IS_SET(ch->warp,WARP_QUICK) )
  count++;
 if( IS_CLASS(ch, CLASS_DRAGON) && GET_POWERS(ch,DRAGON_FORM) == DRAGON_FORM_DRAGON)
  if( count > 3 )
   count = 3;
 if( !IS_NPC(ch) 
  &&  IS_CLASS(ch, CLASS_DRAGON)
  && ch->pcdata->held != NULL
  && ch->pcdata->held[0] != '\0')
  count = 1;
 if( !IS_NPC(ch)
  && ch->pcdata->held_by != NULL
  && ch->pcdata->held_by[0] != '\0')
  count /= 2; 
 if( count < 1 )
  count = 1;
 return count;
}

int class_mod_damage(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
 int i;
  if( !IS_NPC(ch) )
   for( i = 0; i < ch->race; i++ )
    dam *= 1.01;
  if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DROW) )
  {
   if( weather_info.sunlight == SUN_DARK )
    dam *= 1.025;
   else
    dam *= 0.975;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
    dam *= 1.05;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    dam *= 1.025;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    dam *= 1.025;
   else
    dam *= 0.95;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    dam *= 1.05;
   if( IS_SET(ch->newbits,NEW_DARKNESS) )
    dam *= 1.10;
   else
    dam *= 0.90;
   dam *= drow->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_DROW) )
  {
   if( weather_info.sunlight == SUN_DARK )
    dam *= 0.975;
   else
    dam *= 1.025;
   if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_WAR) || victim->generation == 1 )
    dam *= 0.95;
   else if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_MAG) )
    dam *= 0.975;
   else if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_CLE) )
    dam *= 0.975;
   else
    dam *= 1.05;
   if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_HATE) )
    dam *= 1.05;
   if( IS_SET(victim->newbits,NEW_DARKNESS) )
    dam *= 0.90;
   else
    dam *= 1.10;
   if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_SHIELD) )
    dam *= 0.975;
   if( IS_SET(GET_POWERS(victim,DROW_BITS),DROW_TOUGHSKIN) )
    dam *= 0.95;
   dam *= drow->toug;
  }
  if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_APOCRYPHIC))
  {
    dam *= 1.05;
    if( has_power_level(ch, SCHOOL_EARTH, 5))
     dam *= 1.10;
    if( has_power_level(ch, SCHOOL_LUCK, 4))
     dam += number_range( -300, 500);
    if( has_power_level(ch, SCHOOL_TIME, 2))
     dam *= 1.10;
   dam *= apoc->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim, CLASS_APOCRYPHIC))
  {
    dam *= 0.95;
    if( has_power_level(victim, SCHOOL_LIFE, 5))
     dam *= 0.90;   
    if( has_power_level(victim, SCHOOL_DEATH, 5))
     dam *= 0.90;
    if( has_power_level(victim, SCHOOL_PLANT, 1))
     dam *= 0.90;
   dam *= apoc->toug;
  }
  if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON))
  {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   {
    for( i = 0; i < GET_POWERS(ch,DRAGON_AGE); i += 250 )
     dam *= 1.01;
    switch( GET_POWERS(ch, DRAGON_MIGHT))
    {
     case 0: dam *= 0.925; break;
     case 1: dam *= 0.95;  break;
     case 2: dam *= 0.975; break;
     case 3:              break;
     case 4: dam *= 1.025; break;
     case 5: dam *= 1.05;  break;
     case 6: dam *= 1.075; break;
     case 7: dam *= 1.10;  break;
     case 8: dam *= 1.125; break;
     case 9: dam *= 1.15;  break;
     case 10: dam *= 1.175; break;
     default: break;
    }
   }
   else
   {
    switch( GET_POWERS(ch, DRAGON_MIGHT))
    {
     case 0: dam *= 0.70;  break;
     case 1: dam *= 0.75; break;
     case 2: dam *= 0.8;  break;
     case 3: dam *= 0.85; break;
     case 4: dam *= 0.9;  break;
     case 5: dam *= 0.95; break;
     case 6:              break;
     case 7: dam *= 1.025; break;
     case 8: dam *= 1.05;  break;
     case 9: dam *= 1.075; break;
     case 10: dam *= 1.10; break;
     default: break;
    }
   }
   dam *= drag->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS( victim, CLASS_DRAGON))
  {
   if( GET_POWERS(victim, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   {
    for( i = 0; i < GET_POWERS(victim,DRAGON_AGE); i += 250 )
     dam *= 0.99;
    switch( GET_POWERS(victim, DRAGON_TOUGH))
    {
     case 0: dam *= 1.10;  break;
     case 1: dam *= 1.05;  break;
     case 2:              break;
     case 3: dam *= 0.975; break;
     case 4: dam *= 0.95;  break;
     case 5: dam *= 0.85; break;
     case 6: dam *= 0.8;  break;
     case 7: dam *= 0.75; break;
     case 8: dam *= 0.7;  break;
     case 9: dam *= 0.65; break;
     case 10: dam *= 0.6; break;
     default:             break;
    }
   }
   else
   {
    switch( GET_POWERS(victim, DRAGON_SPEED))
    {
     case 0: dam *= 1.5; break;
     case 1: dam *= 1.4; break;
     case 3: dam *= 1.3; break;
     case 4: dam *= 1.2; break;
     case 5: dam *= 1.1; break;
     case 6:             break;
     case 7: dam *= 0.95; break;
     case 8: dam *= 0.9; break;
     case 9: dam *= 0.85; break;
     case 10: dam *= 0.8; break;
     default:            break;
    }
   }
   dam *= drag->toug;
  }
  if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK))
  {
    dam *= 1 + (GET_POWERS(ch, MONK_CURR_CHI)/10);
   dam *= monk->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK))
  {
    if( dam > 0 && GET_POWERS(victim, MONK_CURR_CHI) > 0)
    dam /= GET_POWERS(victim, MONK_CURR_CHI);
   dam *= monk->toug;
  }
  if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ALIEN))
  {
   dam *= ((float) (1 + GET_POWERS(ch, ALIEN_STR) * 0.045));
   dam *= ((float) (1 + GET_POWERS(ch, ALIEN_SIZE) * 0.045));
   if( GET_POWERS(ch, ALIEN_CLASS) == ALIEN_DRONE)
    dam *= 0.9;
   if( GET_POWERS(ch, ALIEN_CLASS) == ALIEN_RUN)
    dam *= 1.0;
   if( GET_POWERS(ch, ALIEN_CLASS) == ALIEN_PRAE)
    dam *= 1.10;
   if( (GET_POWERS(ch, ALIEN_CLASS) == ALIEN_SCORP)
    || (GET_POWERS(ch, ALIEN_CLASS) == ALIEN_GOR))
    dam *= 1.15; 
   dam *= alie->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim, CLASS_ALIEN))
  {
   dam *= ( (float) (1 - (GET_POWERS(victim, ALIEN_ARMOR) * 0.020)));
   dam *= ( (float) (1 - (GET_POWERS(victim, ALIEN_PAIN) * 0.020)));
   if( GET_POWERS(victim, ALIEN_CLASS) == ALIEN_DRONE)
    dam *= 1.10;  
   if( GET_POWERS(victim, ALIEN_CLASS) == ALIEN_RUN)
    dam *= 1.05;
   if( GET_POWERS(victim, ALIEN_CLASS) == ALIEN_PRAE)
    dam *= 0.95;
   if( (GET_POWERS(victim, ALIEN_CLASS) == ALIEN_SCORP)
    || (GET_POWERS(victim, ALIEN_CLASS) == ALIEN_GOR))
    dam *= 0.90;
   dam *= alie->toug;
  }
  if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) )
  {
   dam *= ( (float)( ch->max_move / 100000 ) + 1.3 );
   dam *= shad->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim, CLASS_SHADOW) )
  {/*
   if( ( (float)victim->hit / (float)victim->max_hit ) < 0.08 )
    dam *= 0.08;
   else*/
    dam *= ( (float)victim->hit / (float)victim->max_hit );
   dam *= shad->toug;
  }
  if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_WOLF) )
  {
   if( GET_POWERS(ch,DISC_BEAR) > 0 )
    dam *= 1.015;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_REND) )
    dam *= 1.015;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SENSES) )
    dam *= 1.015;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SCLAW) )
    dam *= 1.015;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW) )
    dam *= 1.015;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    dam *= 1.015; 
   else
    dam *= .925;
   if( GET_POWERS(ch,WOLF_FORM) < 1 )
    dam *= .85;
   if( GET_POWERS(ch,WOLF_FORM) == 1 )
    dam *= 1.05;
   dam *= were->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_WOLF) )
  {
   if( IS_SET(GET_POWERS(victim,WOLF_BITS),BIT_TOUGH) )
    dam *= .975;
   if( GET_POWERS(victim,DISC_BOAR) > 0 )
    dam *= .95;
   if( GET_POWERS(victim,DISC_PAIN) > 0 )
    dam *= ( (float) (1 - (GET_POWERS(victim,DISC_PAIN) * 0.01)));
   if( IS_SET(GET_POWERS(victim,WOLF_BITS),BIT_SHIELD) )
    dam *= .975;
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    dam *= .95;
   else
    dam *= 1.015;
   if( GET_POWERS(victim,WOLF_FORM) < 1 )
    dam *= 1.015;
   if( GET_POWERS(victim,WOLF_FORM) == 2 )
    dam *= .95;
   dam *= were->toug;
  }
  if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
  {
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) )
    dam *= .95;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BEAST) )
    dam *= 1.05;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
    dam *= 1.025;
   if( GET_POWERS(ch,DISC_POTENCE) > 0 )
    dam *= ( (float) (1 + (GET_POWERS(ch,DISC_POTENCE) * 0.01)));
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    dam *= 1.025;
   else if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) || IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
    dam *= .85;
   else
    dam *= .75;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BAAL) )
    dam *= 1.015;
   switch( GET_POWERS(ch,VAMP_FORM) )
   {
    case FORM_NORMAL:  dam *= 1.00;  break;
    case FORM_DRAGON:  dam *= 1.015; break;
    case FORM_ZULO:    dam *= 1.005; break;
    case FORM_BAT:     dam *= 0.90;  break;
    case FORM_SERPENT: dam *= 0.98;  break;
    case FORM_OBJECT:  dam *= 1.00;  break;
    case FORM_MIST:    dam *= .90;   break;
    case FORM_WOLF:    dam *= 1.015; break;
   }
   switch( GET_POWERS(ch,VAMP_AGE) )
   {
    case AGE_NONE:       dam *= 0.95;  break;
    case AGE_CAITIFF:    dam *= 0.97;  break;
    case AGE_ANCILLA:    dam *= 0.99;  break;
    case AGE_ELDER:      dam *= 1.01;  break;
    case AGE_METHUSELAH: dam *= 1.03; break;
   }
   switch( ch->generation )
   {
    default: dam *= 1.00; break;
    case 4:  dam *= 1.01; break;
    case 3:  dam *= 1.02; break;
    case 2:  dam *= 1.03; break;
    case 1:  dam *= 1.05; break;
   }
   dam *= vamp->powe;
  }
  if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_VAMPIRE) )
  {
   if( IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_SCALES) )
    dam *= .98;
   if( IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_GOLCANDA) )
    dam *= .95;
   if( IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_BEAST) )
    dam *= 1.05;
   if( IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_DARK_GOLC) )
    dam *= .975;
   if( GET_POWERS(victim,DISC_FORTITUDE) > 0 )
    dam *= ( (float) (1 - (GET_POWERS(victim,DISC_FORTITUDE) * 0.01)));
   if( weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK )
    dam *= .95;
   else if( !IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_GOLCANDA) && !IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_DARK_GOLC) )
    dam *= 1.05;
   switch( GET_POWERS(victim,VAMP_FORM) )
   {
    case FORM_NORMAL:  dam *= 1.00;  break;
    case FORM_DRAGON:  dam *= .98;   break;
    case FORM_ZULO:    dam *= .99;   break;
    case FORM_BAT:     dam *= 1.10;  break;
    case FORM_SERPENT: dam *= 1.015; break;
    case FORM_OBJECT:  dam *= 1.00;  break;
    case FORM_MIST:    dam *= .90;   break;
    case FORM_WOLF:    dam *= 1.015; break;
   }
   switch( GET_POWERS(victim,VAMP_AGE) )
   {
    case AGE_NONE:       dam *= 1.05; break;
    case AGE_CAITIFF:    dam *= 1.03; break;
    case AGE_ANCILLA:    dam *= 1.01; break;
    case AGE_ELDER:      dam *= 0.99; break;
    case AGE_METHUSELAH: dam *= 0.97; break;
   }
   switch( victim->generation )
   {
    default: dam *= 1.00; break; 
    case 4:  dam *= 1.03; break; 
    case 3:  dam *= 1.02; break; 
    case 2:  dam *= 0.97; break; 
    case 1:  dam *= 0.95; break; 
   }
   dam *= vamp->toug;
  }
 if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_JEDI) )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_ATTACK); i++ )
   dam *= 1.05;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
   dam *= 1.15;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= 1.05;
  switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
  {
   case JEDI_STYLE_NONE:                break;
   case JEDI_STYLE_LIGHT:  dam *= 0.95; break;
   case JEDI_STYLE_MEDIUM:              break;
   case JEDI_STYLE_HEAVY:  dam *= 1.05; break;
  }
   dam *= jedi->powe;
 }
 if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_JEDI) )
 {
  for( i = 0; i < GET_POWERS(victim,JEDI_DEFENSE); i++ )
   dam *= 0.975;
  if( IS_SET(GET_POWERS(victim,JEDI_BITS),JEDI_RAGE) )
   dam *= 0.975;
  if( IS_SET(GET_POWERS(victim,JEDI_BITS),JEDI_SHIELD) )
   dam *= 0.975;
  if( IS_SET(GET_POWERS(victim,JEDI_BITS),JEDI_UBER) )
   dam *= 0.975;
  if( IS_SET(GET_POWERS(victim,JEDI_BITS),JEDI_TOUGH) )
   dam *= 0.95;
  if( IS_SET(GET_POWERS(victim,JEDI_BITS),JEDI_UNRAGE) )
   dam *= 1.15;
  switch( GET_POWERS(victim,JEDI_STANCE_STYLE) )
  {
   case JEDI_STYLE_NONE:                break;
   case JEDI_STYLE_LIGHT:  dam *= 0.95; break;
   case JEDI_STYLE_MEDIUM:              break;
   case JEDI_STYLE_HEAVY:  dam *= 1.05; break;
  }
   dam *= jedi->toug;
 }
 return dam;
}

void get_points(CHAR_DATA *ch, CHAR_DATA *victim)
{
 char buf[MAX_STRING_LENGTH];
 int gain = 0;
 if( IS_NPC(ch))
  return;

 if( !IS_NPC(victim) && IS_HERO(ch) )
 {
  gain = ( victim->race * 15000 );
  if( gain <= 0 )
   gain = 5000;
  gain *= global_exp_mult;
  gain *= exp_who;
  ch->pcdata->apoints += gain;
  sprintf(buf, "You recieve %d ability points.\n\r", gain );   
  stc(buf,ch);
 }
 if( IS_CLASS(ch,CLASS_DROW))
 {
  if( IS_NPC(victim))
  {
   if( victim->level < 10 )
    return;
   gain = number_range( 10, victim->level);
   gain *= global_exp_mult;
   gain *= exp_who;
   sprintf(buf,"You have gained %d drow points.\n\r", gain);
   stc( buf, ch);
   GET_POWERS(ch, DROW_POINTS) += gain;
   return;
  }
  else
  {   
   gain = victim->race;
   gain *= global_exp_mult;
   gain *= exp_who;
   sprintf( buf, "You have gained %d drow points.\n\r", gain);
   stc( buf, ch);
   GET_POWERS(ch, DROW_POINTS) += gain;
   return;
  }
  return;
 }
 if( IS_CLASS(ch, CLASS_DRAGON))
 {
  if( IS_NPC(victim))
  {
   if( victim->level < 10 )
    return;
   gain = number_range( 10, victim->level);
   gain *= global_exp_mult;
   gain *= exp_who;
   sprintf(buf,"You have gained %d dragon points.\n\r", gain);
   stc( buf, ch);
   GET_POWERS(ch, DRAGON_POINTS) += gain;
   return;
  }
  else
  {
   gain = victim->race;
   gain *= global_exp_mult;
   gain *= exp_who;
   sprintf( buf, "You have gained %d dragon points.\n\r", gain);
   stc( buf, ch);
   GET_POWERS(ch, DRAGON_POINTS) += gain;
   return;
  }
  return;
 }
 if( IS_CLASS(ch, CLASS_ALIEN))
 {
  if( IS_NPC(victim))
  {
   if( victim->level < 10)
    return;
   gain = number_range( 10, victim->level);
   gain *= global_exp_mult;
   gain *= exp_who;
   GET_POWERS(ch, ALIEN_DNA) += gain;
   sprintf(buf,"You have absorbed %d points of DNA from your victim.\n\r",gain);
   stc(buf,ch);
   return;
  }
  else
  {
   gain = victim->race;                          
   gain *= global_exp_mult;
   gain *= exp_who;
   sprintf(buf,"You have absorbed %d points of DNA from your victim.\n\r",gain);
   stc(buf,ch);
   return;
  }
  return;
 }
 if( IS_CLASS(ch,CLASS_WOLF) || IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  if( IS_NPC(victim) )
  {
   if( victim->level < 10 )
    return;
   gain = number_range( 10, victim->level);
   gain *= global_exp_mult;
   gain *= exp_who; 
   if( IS_CLASS(ch,CLASS_WOLF) )
    GET_POWERS(ch,WOLF_POINTS) += gain;
   else if( IS_CLASS(ch,CLASS_VAMPIRE) )
    GET_POWERS(ch,VAMP_POINTS) += gain;
   sprintf(buf,"You have gained %d discipline points.\n\r",gain);
   stc(buf,ch);
   return;
  }
  else
  {
   gain = victim->race;
   gain *= global_exp_mult;
   gain *= exp_who; 
   if( IS_CLASS(ch,CLASS_WOLF) )
    GET_POWERS(ch,WOLF_POINTS) += gain;
   if( IS_CLASS(ch,CLASS_VAMPIRE) )
    GET_POWERS(ch,VAMP_POINTS) += gain;
   sprintf(buf,"You have gained %d discipline points.\n\r",gain);
   stc(buf,ch);
   return;
  }
 }
 /* add other classes here*/
 return;
}

void class_attributes(CHAR_DATA *ch)
{

   int age = 0;
   int value = 0;

   age = GET_POWERS(ch, DRAGON_AGE);

   if( age < 10 ) 				value = 0;
   else if( age >=  10 && age < 100 )		value = 1;
   else if( age >= 100 && age < 150 )		value = 2;
   else if( age >= 150 && age < 200 )		value = 3;
   else if( age >= 200 && age < 250 )		value = 4; 
   else if( age >= 250 && age < 300 )		value = 5;
   else if( age >= 300 && age < 350 )		value = 6;
   else if( age >= 350 )			value = 7; 

   ch->pcdata->mod_str = value;
   ch->pcdata->mod_dex = value;
   ch->pcdata->mod_con = value;
   ch->pcdata->mod_int = value;
   ch->pcdata->mod_wis = value;
   return;
}

int class_hitroll(CHAR_DATA *ch)
{
 int value = 0;

 value += ch->hitroll;
 if( !IS_NPC(ch) )
 {
  value += ASKILL(ch,AS_ATTACK);
  value += str_app[get_curr_str(ch)].tohit;
  value += ch->rage;
  value += ch->pkill;

  if( IS_CLASS(ch,CLASS_DROW) )
  {
   if( weather_info.sunlight == SUN_DARK )
    value += 300;
   else
    value -= 300;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
    value += 100;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    value += 50;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    value += 50;
   else
    value -= 150;
   value += drow->hitr;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    value += 300;
  }  
  if( IS_CLASS(ch, CLASS_DRAGON) )
  {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   {
    value += (GET_POWERS(ch,DRAGON_AGE) / 20 );
    value += (GET_POWERS(ch, DRAGON_SPEED)*300);
   }
   value += drag->hitr;
  }
  if( IS_CLASS(ch,CLASS_JEDI) )
  {
   value += (GET_POWERS(ch,JEDI_ATTACK) * 5);
   value += (GET_POWERS(ch,JEDI_RANK) * 5);
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
    value += 100;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
    value += 200;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
    value -= 400;
   value += jedi->hitr;
  }
  if( IS_CLASS(ch,CLASS_APOCRYPHIC) )
  {
   if( has_power_level(ch, SCHOOL_FIRE, 1))
    value += 150;
   if( has_power_level(ch, SCHOOL_WIND, 3))
    value += 150;
   if( has_power_level(ch, SCHOOL_WIND, 8))
    value += 150;
   if( has_power_level(ch, SCHOOL_SPACE, 6))
    value += 150;
   if( has_power_level(ch, SCHOOL_ANIMAL, 3))
    value += 150;
   if( has_power_level(ch, SCHOOL_ANIMAL, 6))
    value += 150;
   if( has_power_level(ch, SCHOOL_LUCK, 5))
    value += number_range(-10, 250);
   if( has_power_level(ch, SCHOOL_ORDER, 6))
    value += 150;
   value += apoc->hitr;
  }
  if( IS_CLASS(ch, CLASS_ALIEN))
  {
   value += GET_POWERS(ch, ALIEN_SENSE) * 8;
   value += GET_POWERS(ch, ALIEN_SPEED) * 8;
   value += alie->hitr;
  }
  if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DEFILE))
  {
   if( !has_power(ch, SCHOOL_EVIL))
    value -= 100;
   else
    value += 50;
  }
  if( IS_CLASS(ch, CLASS_MONK))
  {
   value += (GET_POWERS(ch, MONK_CURR_CHI) * 150);
   value += monk->hitr;
  }
  if( IS_CLASS(ch, CLASS_SHADOW) )
  {
   value += ( ch->max_move / 200 );
   value += shad->hitr;
  }
  if( IS_CLASS(ch,CLASS_WOLF) )
  {
   value += 3*(GET_POWERS(ch,DISC_BEAR) + GET_POWERS(ch,DISC_LYNX) + GET_POWERS(ch,DISC_BOAR) + GET_POWERS(ch,DISC_OWL)
    + GET_POWERS(ch,DISC_SPIDER) + GET_POWERS(ch,DISC_WOLF) + GET_POWERS(ch,DISC_HAWK) + GET_POWERS(ch,DISC_MANTIS)
    + GET_POWERS(ch,DISC_RAPTOR) + GET_POWERS(ch,DISC_LUNA) + GET_POWERS(ch,DISC_PAIN) + GET_POWERS(ch,DISC_CONG) );
   value += were->hitr;
  }
  if( IS_CLASS(ch,CLASS_VAMPIRE) )
  {
   value += (GET_POWERS(ch,DISC_CELERITY) * 12);
   value += vamp->hitr;
  }
 }
 if( value < 0 )
  value = 1;
 return value;
}

int class_damroll(CHAR_DATA *ch)
{
 int value = 0;

 value += ch->damroll;
 if( !IS_NPC(ch) )
 {
  value += ASKILL(ch,AS_ATTACK);
  value += str_app[get_curr_str(ch)].todam;
  value += ch->rage;
  value += ch->pkill;

  if( IS_CLASS(ch,CLASS_DROW) )
  {
   if( weather_info.sunlight == SUN_DARK )
    value += 300;
   else
    value -= 300;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
    value += 100;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    value += 50;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    value += 50;
   else
    value -= 150;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    value += 300;
   value += drow->damr;
  }
  if( IS_CLASS(ch, CLASS_DRAGON))
  {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   {
    value += (GET_POWERS(ch,DRAGON_AGE) / 20 );
    value += (GET_POWERS(ch, DRAGON_MIGHT) * 300);
   }
   value += drag->damr;
  }
  if( IS_CLASS(ch,CLASS_JEDI) )
  {
   value += (GET_POWERS(ch,JEDI_ATTACK) * 5);
   value += (GET_POWERS(ch,JEDI_RANK) * 5);
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )  
    value += 100;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
    value += 200;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
    value -= 400;
   value += jedi->damr;
  }
  if( IS_CLASS(ch, CLASS_APOCRYPHIC))
  {
   if( has_power_level( ch, SCHOOL_FIRE, 3))
    value += 150;
   if( has_power_level( ch, SCHOOL_WATER, 3))
    value += 150;
   if( has_power_level(ch, SCHOOL_EARTH, 1))
    value += 150;
   if( has_power_level(ch, SCHOOL_EVIL, 5))
    value += 150;
   if( has_power_level(ch, SCHOOL_ANIMAL, 4))
    value += 150;
   if( has_power_level(ch, SCHOOL_LUCK, 4))
    value += number_range(-10, 250);
   if( has_power_level(ch, SCHOOL_CHAOS, 3))
    value += 150;
   if( has_power_level(ch, SCHOOL_DEATH, 3))
    value += 150;
   value += apoc->damr;
  }
  if( IS_CLASS(ch, CLASS_ALIEN))
  {
   value += GET_POWERS(ch, ALIEN_STR) * 8;
   value += GET_POWERS(ch, ALIEN_SIZE) * 8;
   value += GET_POWERS(ch, ALIEN_PRIMAL) *8;
   value += alie->damr;
  }
  if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DEFILE))
  {
   if( !has_power(ch, SCHOOL_EVIL))
    value -= 100;
   else
    value += 50;
  }
  if( IS_CLASS(ch, CLASS_SHADOW) )
  {
   value += ( ch->max_hit / 200 );
   value += shad->damr;
  }
  if( IS_CLASS(ch,CLASS_WOLF) )
  {
   value += 3*(GET_POWERS(ch,DISC_BEAR) + GET_POWERS(ch,DISC_LYNX) + GET_POWERS(ch,DISC_BOAR) + GET_POWERS(ch,DISC_OWL)
    + GET_POWERS(ch,DISC_SPIDER) + GET_POWERS(ch,DISC_WOLF) + GET_POWERS(ch,DISC_HAWK) + GET_POWERS(ch,DISC_MANTIS)
    + GET_POWERS(ch,DISC_RAPTOR) + GET_POWERS(ch,DISC_LUNA) + GET_POWERS(ch,DISC_PAIN) + GET_POWERS(ch,DISC_CONG) );
   value += were->damr;
  }
  if( IS_CLASS(ch,CLASS_VAMPIRE) )
  {
   value += (GET_POWERS(ch,DISC_POTENCE) * 12);
   value += vamp->damr;
  }
 }
 if( value < 0 )
  value = 1;
 return value;
}

int class_ac(CHAR_DATA *ch)
{
 int value = 0;

 value -= ch->armor;
 if( !IS_NPC(ch) )
 {
  value += (ASKILL(ch,AS_DEFENSE)*3);  
  value -= dex_app[get_curr_dex(ch)].defensive;
  value -= ch->rage;
  value += ch->pkill;

  if( IS_CLASS(ch,CLASS_DROW) )
  {
   if( weather_info.sunlight == SUN_DARK )
    value += 300;
   else
    value -= 300;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_WAR) || ch->generation == 1 )
    value += 100;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_MAG) )
    value += 50;
   else if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_CLE) )
    value += 50;
   else
    value -= 150;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_SHIELD) )
    value += 100;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_TOUGHSKIN) )
    value += 100;
   if( IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    value -= 150;
   value -= drow->armo;
  }
  if( IS_CLASS(ch, CLASS_DRAGON))
  {
   if( GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
   {
    value += (GET_POWERS(ch,DRAGON_AGE) / 10 );
    value += (GET_POWERS( ch, DRAGON_TOUGH) * 250);
   }
   value -= drag->armo;
  }
  if( IS_CLASS(ch,CLASS_JEDI) )
  {
   value += (GET_POWERS(ch,JEDI_DEFENSE) * 5);
   value += (GET_POWERS(ch,JEDI_RANK) * 5);
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SHIELD) )
    value += 200;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )  
    value += 100;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
    value += 400;
   if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
    value -= 800;
   value -= jedi->armo;
  }
  if( IS_CLASS(ch, CLASS_APOCRYPHIC))
  {
   if( has_power_level(ch, SCHOOL_SPACE, 5))
    value += 150;
   if( has_power_level(ch, SCHOOL_PLANT, 1))
    value += 150;
   if( has_power_level(ch, SCHOOL_LIFE, 3))
    value += 150;
   if( has_power_level(ch, SCHOOL_DEATH, 4))
    value += 150;
   value -= apoc->armo;
  }
  if( IS_CLASS(ch, CLASS_ALIEN))
  {
   value += GET_POWERS(ch, ALIEN_SPEED) * 10;
   value += GET_POWERS(ch, ALIEN_ARMOR) * 10;
   value -= alie->armo;
  }
  if( IS_CLASS(ch, CLASS_MONK))
  {
   value += GET_POWERS(ch, MONK_CURR_CHI) * 200;
   value -= monk->armo;
  }
  if( IS_CLASS(ch, CLASS_SHADOW) )
  {
   value += ( ( ch->max_hit + ch->max_move ) / 400 );
   value -= shad->armo;
  }
  if( IS_CLASS(ch,CLASS_WOLF) )
  {
   value += 1.25*(GET_POWERS(ch,DISC_BEAR) + GET_POWERS(ch,DISC_LYNX) + GET_POWERS(ch,DISC_BOAR) + GET_POWERS(ch,DISC_OWL)
    + GET_POWERS(ch,DISC_SPIDER) + GET_POWERS(ch,DISC_WOLF) + GET_POWERS(ch,DISC_HAWK) + GET_POWERS(ch,DISC_MANTIS)
    + GET_POWERS(ch,DISC_RAPTOR) + GET_POWERS(ch,DISC_LUNA) + GET_POWERS(ch,DISC_PAIN) + GET_POWERS(ch,DISC_CONG) );
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SHIELD) )
    value += 100;
   if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_VANISH) )
    value += 100;
   value -= were->armo;
  }
  if( IS_CLASS(ch,CLASS_VAMPIRE) )
  {
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_SCALES) )
    value += 100;
   if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_EXO) )
    value += 100;
   value += (GET_POWERS(ch,DISC_FORTITUDE) * 12);
   value -= vamp->armo;
  }
 }
 value *= -1;
 return value;
}

void class_regen(CHAR_DATA *ch)
{
 int gain = 0;

 if( IS_NPC(ch))
  return;
 if( ch->fighting != NULL)
  return;
 gain = (ch->max_hit / number_range(150,190));
 switch ( ch->position )
 {
   case POS_MEDITATING: gain *= 1.3;  break;
   case POS_SLEEPING:   gain *= 1.5;  break;
   case POS_RESTING:    gain *= 1.3;  break;
 }  
 if(is_affected(ch, skill_lookup("regeneration")))
   gain *= 1.5;
 if(ch->stasis)
   gain *= 2;
 if( IS_SET(ch->warp, WARP_REGEN))
   gain *= 1.25;   
 if( IS_SET(ch->warp, WARP_SLOWHEAL))
   gain /= 1.25;
 if( IS_CLASS(ch,CLASS_DROW) )
 {
  gain *= 1.15;
  gain *= drow->rege;
 }
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  switch( GET_POWERS(ch,VAMP_AGE) )
  {
   default:                          break;
   case AGE_NONE:                    break;
   case AGE_CAITIFF:    gain *= 1.1; break;
   case AGE_ANCILLA:    gain *= 1.2; break;
   case AGE_ELDER:      gain *= 1.4; break;
   case AGE_METHUSELAH: gain *= 1.8; break;
  }
  if( GET_POWERS(ch,VAMP_BLOOD) > blood_max(ch) )
   GET_POWERS(ch,VAMP_BLOOD) = blood_max(ch);
  gain *= vamp->rege;
 }
 if( IS_CLASS(ch, CLASS_DRAGON))
 {
  if( GET_POWERS(ch, DRAGON_FORM) != DRAGON_FORM_DRAGON)
   gain *= 1.5;
  else
   gain *= 1.1;
  gain *= drag->rege;
 }
 if( IS_CLASS(ch,CLASS_JEDI) )
 {
  switch( GET_POWERS(ch,JEDI_RANK) )
  {
   default:                   gain *= 1.0; break;
   case JEDI_RANK_APPRENTICE: gain *= 1.0; break;
   case JEDI_RANK_INITIATE:   gain *= 1.1; break;
   case JEDI_RANK_GUARDIAN:   gain *= 1.2; break;
   case JEDI_RANK_KNIGHT:     gain *= 1.4; break;
   case JEDI_RANK_MASTER:     gain *= 1.6; break;
   case JEDI_RANK_LORD:       gain *= 1.8; break;
  }
  GET_POWERS(ch,JEDI_COMBO_OLD) = 0;
  GET_POWERS(ch,JEDI_COMBO_NEW) = 0;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_UBER);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_TOUGH) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_TOUGH);
  if( GET_POWERS(ch,JEDI_FORCE_POOL) < GET_POWERS(ch,JEDI_FORCE_POOL_MAX) )
   GET_POWERS(ch,JEDI_FORCE_POOL) += number_range(10,25);
  if( GET_POWERS(ch,JEDI_FORCE_POOL) > GET_POWERS(ch,JEDI_FORCE_POOL_MAX) )
   GET_POWERS(ch,JEDI_FORCE_POOL) = GET_POWERS(ch,JEDI_FORCE_POOL_MAX);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UNRAGE) )
   if( GET_POWERS(ch,JEDI_FORCE_POOL) >= GET_POWERS(ch,JEDI_FORCE_POOL_MAX) )
   {
    stc("You feel recovered from your rage.\n\r",ch);
    REM_BIT_POWERS(ch,JEDI_BITS,JEDI_UNRAGE);
   }
  gain *= jedi->rege;
 }
 if( IS_CLASS(ch, CLASS_MONK))
 {
   if( ch->position == POS_MEDITATING )
   {
    if( GET_POWERS(ch, MONK_CURR_FOC) < GET_POWERS(ch, MONK_MAX_FOC))
    {
     GET_POWERS(ch, MONK_CURR_FOC) += number_range(1,GET_POWERS(ch, MONK_MAX_CHI));
     if(  GET_POWERS(ch, MONK_CURR_FOC) > GET_POWERS(ch, MONK_MAX_FOC))
        GET_POWERS(ch, MONK_CURR_FOC) = GET_POWERS(ch, MONK_MAX_FOC);
    }
   }
   GET_POWERS(ch,MONK_COMBO) = 0;
   GET_POWERS(ch,MONK_CHAIN) = 0;
   gain *= 3;
  gain *= monk->rege;
 }
 if( IS_CLASS(ch, CLASS_ALIEN))
 {
  if( GET_POWERS(ch,ALIEN_SIZE) < 2 )
   gain *= 1;
  else
   gain *= (GET_POWERS(ch, ALIEN_SIZE) / 2);
  gain *= alie->rege;
 }
 if( IS_CLASS(ch, CLASS_SHADOW) )
 {
  gain *= ( ( ( (float)ch->max_hit / 100000 ) / 2 ) + 1 );
  gain *= ( ( (float)ch->max_mana / 100000 ) + 1 );
  gain *= ( ( ( (float)ch->max_move / 100000 ) * 2 ) + 1 );
  gain *= shad->rege;
 }
 if( !IS_CLASS(ch, CLASS_SHADOW) && ch->in_room != NULL && IS_SET(ch->in_room->affected_by, AFF_ROOM_NEGATIVITY) )
  gain /= 2;
 if( IS_CLASS(ch, CLASS_SHADOW) && ch->in_room != NULL && IS_SET(ch->in_room->affected_by, AFF_ROOM_NEGATIVITY) )
  gain *= 2;
 if( ch->fight_timer > 0)
  gain /= 2;
 if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_ALIEN) )         
  if( IS_SET(GET_POWERS(ch,ALIEN_BITS),ALIEN_CLOAK) )
    gain /= 2;
 if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_SHADOW) )
  if( IS_SET(GET_POWERS(ch,SHADOW_BITS),SHADOW_INVIS) )
    gain /= 2;
 if( ch->hit < 0)
 gain = number_range(1,2);
 if( !IS_CLASS(ch, CLASS_SHADOW ) )
 {
  if( ch->hit < ch->max_hit)
   ch->hit += gain;
  if( ch->mana < ch->max_mana)
   ch->mana += gain;
  if( ch->move < ch->max_move)
   ch->move += gain;
 }
 else if( IS_CLASS(ch, CLASS_SHADOW) )
 {
  if( ch->hit < ch->max_hit )
   ch->hit += ( gain / 2 );
  if( ch->mana < ch->max_mana )
   ch->mana += ( gain * 1.25 );
  if( ch->move < ch->max_move )
   ch->move += ( gain * 1.5 );
 }
 if( ch->level >= 3 )
 {
  if( (ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
       ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5] + ch->loc_hp[6]) != 0)
   reg_mend(ch);
 }
 update_pos(ch);
 return;
}

/*int class_damage(CHAR_DATA *ch, CHAR_DATA *vict, int cost, bool type, int cost_loc)
{
 int dam = 0;
 int wait = 0;

 if( !ch || !vict)
  return 0;
 if( cost == 0)
  return 0;
 dam = (cost*2) + number_range(-500, 500);
 dam = randomize_damage(dam, number_range(1,100))
 wait = cost/21;
 switch(cost_loc)
 {
  default:
   return 0;
   break
  case 0:
   if( ch->hit < cost)
    return 0;
   else
    ch->hit -= cost;
   break;
  case 1:
   if( ch->mana < cost)
    return 0;
   else
    ch->mana -= cost;
   break;
  case 2:
   if( ch->move < cost);
    return 0;
   else
    ch->move -= cost;
   break;
 }
 if( type)
   vict->hit -= dam;
 else
 {
   vict->hit += dam;
   if( vict->hit > victim->max_hit)
     vict->hit = victim->max_hit;
 }
 update_pos(vict);
 update_pos(ch);
 WAIT_STATE(ch, wait);
 return dam;
}
*/
