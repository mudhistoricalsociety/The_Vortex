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

/* Kline-style Jedi! WOOOT! */

/*** BEGIN UTILITY FUNCTIONS ***/
void jedi_force_upkeep( CHAR_DATA *ch )
{
 int i;
 int cost = 0;

 if( ch == NULL )
  return;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_ABSORB) )
  for( i = 0; i < GET_POWERS(ch,JEDI_FORCE_ABSORB); i++ )
   cost += 3;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SHIELD) )      
  for( i = 0; i < GET_POWERS(ch,JEDI_FORCE_SHIELD); i++ )
   cost += 3;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_MIND_TRICK) )      
  for( i = 0; i < GET_POWERS(ch,JEDI_FORCE_MIND_TRICK); i++ )
   cost += 3;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SIGHT) )      
  for( i = 0; i < GET_POWERS(ch,JEDI_FORCE_SIGHT); i++ )
   cost += 3;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )      
  for( i = 0; i < GET_POWERS(ch,JEDI_FORCE_RAGE); i++ )
   cost += 5;
 if( GET_POWERS(ch,JEDI_FORCE_POOL) < cost )
 {
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_ABSORB) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_ABSORB);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SHIELD) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_SHIELD);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_MIND_TRICK) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_MIND_TRICK);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SIGHT) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_SIGHT);
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_RAGE) )
   REM_BIT_POWERS(ch,JEDI_BITS,JEDI_RAGE);
  stc("Your force powers dissipated.\n\r",ch);
 }
 else
  GET_POWERS(ch,JEDI_FORCE_POOL) -= cost;
 return;
}

int snarf_jedi_total_force( CHAR_DATA *ch )
{
 if( ch == NULL )
  return;
 return ( GET_POWERS(ch,JEDI_FORCE_HEAL) + GET_POWERS(ch,JEDI_FORCE_ABSORB) + GET_POWERS(ch,JEDI_FORCE_SHIELD)
        + GET_POWERS(ch,JEDI_FORCE_MIND_TRICK) + GET_POWERS(ch,JEDI_FORCE_SIGHT) + GET_POWERS(ch,JEDI_FORCE_SPEED)
        + GET_POWERS(ch,JEDI_FORCE_JUMP) + GET_POWERS(ch,JEDI_FORCE_PUSH) + GET_POWERS(ch,JEDI_FORCE_PULL)
        + GET_POWERS(ch,JEDI_FORCE_GRIP) + GET_POWERS(ch,JEDI_FORCE_RAGE) + GET_POWERS(ch,JEDI_FORCE_DRAIN)
        + GET_POWERS(ch,JEDI_FORCE_LIGHTNING) );
}

int snarf_jedi_combo_dam( CHAR_DATA *ch, int dam )
{
 switch( GET_POWERS(ch,JEDI_COMBO_OLD) )
 {
  case JEDI_COMBO_NONE:
   break;
  case JEDI_COMBO_JOVER:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 0.50; break;
    case JEDI_COMBO_JFLIP:  dam *= 1.10; break;
    case JEDI_COMBO_JUPPER: dam *= 0.75; break;
    case JEDI_COMBO_JBACK:  dam *= 1.50; break;
    case JEDI_COMBO_JSLICE: dam *= 0.75; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.20; break;
    case JEDI_COMBO_JSLASH: dam *= 1.30; break;
    case JEDI_COMBO_JROLL:  dam *= 1.40; break;
   }
   break;
  case JEDI_COMBO_JFLIP:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 1.10; break;
    case JEDI_COMBO_JFLIP:  dam *= 0.50; break;
    case JEDI_COMBO_JUPPER: dam *= 0.75; break;
    case JEDI_COMBO_JBACK:  dam *= 1.50; break;
    case JEDI_COMBO_JSLICE: dam *= 0.75; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.20; break;
    case JEDI_COMBO_JSLASH: dam *= 1.30; break;
    case JEDI_COMBO_JROLL:  dam *= 1.40; break;
   }
   break;
  case JEDI_COMBO_JUPPER:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 1.40; break;
    case JEDI_COMBO_JFLIP:  dam *= 1.30; break;
    case JEDI_COMBO_JUPPER: dam *= 0.50; break;
    case JEDI_COMBO_JBACK:  dam *= 0.75; break;
    case JEDI_COMBO_JSLICE: dam *= 1.50; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.20; break;
    case JEDI_COMBO_JSLASH: dam *= 0.75; break;
    case JEDI_COMBO_JROLL:  dam *= 1.10; break;
   }
   break;
  case JEDI_COMBO_JBACK:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 0.75; break;
    case JEDI_COMBO_JFLIP:  dam *= 1.30; break;
    case JEDI_COMBO_JUPPER: dam *= 0.75; break;
    case JEDI_COMBO_JBACK:  dam *= 0.50; break;
    case JEDI_COMBO_JSLICE: dam *= 0.75; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.10; break;
    case JEDI_COMBO_JSLASH: dam *= 1.40; break;
    case JEDI_COMBO_JROLL:  dam *= 1.50; break;
   }
   break;
  case JEDI_COMBO_JSLICE:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 1.30; break;
    case JEDI_COMBO_JFLIP:  dam *= 1.40; break;
    case JEDI_COMBO_JUPPER: dam *= 1.10; break;
    case JEDI_COMBO_JBACK:  dam *= 0.75; break;
    case JEDI_COMBO_JSLICE: dam *= 0.50; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.50; break;
    case JEDI_COMBO_JSLASH: dam *= 1.20; break;
    case JEDI_COMBO_JROLL:  dam *= 0.75; break;
   }
   break;
  case JEDI_COMBO_JSWIPE:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 1.20; break;
    case JEDI_COMBO_JFLIP:  dam *= 1.10; break;
    case JEDI_COMBO_JUPPER: dam *= 1.40; break;
    case JEDI_COMBO_JBACK:  dam *= 0.75; break;
    case JEDI_COMBO_JSLICE: dam *= 1.30; break;
    case JEDI_COMBO_JSWIPE: dam *= 0.50; break;
    case JEDI_COMBO_JSLASH: dam *= 1.50; break;
    case JEDI_COMBO_JROLL:  dam *= 0.75; break;
   }
   break;
  case JEDI_COMBO_JSLASH:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 1.20; break;
    case JEDI_COMBO_JFLIP:  dam *= 0.75; break;
    case JEDI_COMBO_JUPPER: dam *= 1.30; break;
    case JEDI_COMBO_JBACK:  dam *= 0.75; break;
    case JEDI_COMBO_JSLICE: dam *= 1.50; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.40; break;
    case JEDI_COMBO_JSLASH: dam *= 0.50; break;
    case JEDI_COMBO_JROLL:  dam *= 1.10; break;
   }
   break;
  case JEDI_COMBO_JROLL:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:                break;
    case JEDI_COMBO_JOVER:  dam *= 1.10; break;
    case JEDI_COMBO_JFLIP:  dam *= 1.30; break;
    case JEDI_COMBO_JUPPER: dam *= 0.75; break;
    case JEDI_COMBO_JBACK:  dam *= 1.50; break;
    case JEDI_COMBO_JSLICE: dam *= 0.75; break;
    case JEDI_COMBO_JSWIPE: dam *= 1.20; break;
    case JEDI_COMBO_JSLASH: dam *= 1.40; break;
    case JEDI_COMBO_JROLL:  dam *= 0.50; break;
   }
   break;
 }
 return dam;
}

int snarf_jedi_combo_ws( CHAR_DATA *ch, int ws )
{
 switch( GET_POWERS(ch,JEDI_COMBO_OLD) )
 {
  case JEDI_COMBO_NONE:
   break;
  case JEDI_COMBO_JOVER:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 2.00; break;
    case JEDI_COMBO_JFLIP:  ws *= 0.90; break;
    case JEDI_COMBO_JUPPER: ws *= 1.25; break;
    case JEDI_COMBO_JBACK:  ws *= 0.50; break;
    case JEDI_COMBO_JSLICE: ws *= 1.25; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.80; break;
    case JEDI_COMBO_JSLASH: ws *= 0.70; break;
    case JEDI_COMBO_JROLL:  ws *= 0.60; break;
   }
   break;
  case JEDI_COMBO_JFLIP:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 0.90; break;
    case JEDI_COMBO_JFLIP:  ws *= 2.00; break;
    case JEDI_COMBO_JUPPER: ws *= 1.25; break;
    case JEDI_COMBO_JBACK:  ws *= 0.50; break;
    case JEDI_COMBO_JSLICE: ws *= 1.25; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.80; break;
    case JEDI_COMBO_JSLASH: ws *= 0.70; break;
    case JEDI_COMBO_JROLL:  ws *= 0.60; break;
   }
   break;
  case JEDI_COMBO_JUPPER:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 0.60; break;
    case JEDI_COMBO_JFLIP:  ws *= 0.70; break;
    case JEDI_COMBO_JUPPER: ws *= 2.00; break;
    case JEDI_COMBO_JBACK:  ws *= 1.25; break;
    case JEDI_COMBO_JSLICE: ws *= 0.50; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.80; break;
    case JEDI_COMBO_JSLASH: ws *= 1.25; break;
    case JEDI_COMBO_JROLL:  ws *= 0.90; break;
   }
   break;
  case JEDI_COMBO_JBACK:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 1.25; break;
    case JEDI_COMBO_JFLIP:  ws *= 0.70; break;
    case JEDI_COMBO_JUPPER: ws *= 1.25; break;
    case JEDI_COMBO_JBACK:  ws *= 2.00; break;
    case JEDI_COMBO_JSLICE: ws *= 1.25; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.90; break;
    case JEDI_COMBO_JSLASH: ws *= 0.60; break;
    case JEDI_COMBO_JROLL:  ws *= 0.50; break;
   }
   break;
  case JEDI_COMBO_JSLICE:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 0.70; break;
    case JEDI_COMBO_JFLIP:  ws *= 0.60; break;
    case JEDI_COMBO_JUPPER: ws *= 0.90; break;
    case JEDI_COMBO_JBACK:  ws *= 1.25; break;
    case JEDI_COMBO_JSLICE: ws *= 2.00; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.50; break;
    case JEDI_COMBO_JSLASH: ws *= 0.80; break;
    case JEDI_COMBO_JROLL:  ws *= 1.25; break;
   }
   break;
  case JEDI_COMBO_JSWIPE:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 0.80; break;
    case JEDI_COMBO_JFLIP:  ws *= 0.90; break;
    case JEDI_COMBO_JUPPER: ws *= 0.60; break;
    case JEDI_COMBO_JBACK:  ws *= 1.25; break;
    case JEDI_COMBO_JSLICE: ws *= 0.70; break;
    case JEDI_COMBO_JSWIPE: ws *= 2.00; break;
    case JEDI_COMBO_JSLASH: ws *= 0.50; break;
    case JEDI_COMBO_JROLL:  ws *= 1.25; break;
   }
   break;
  case JEDI_COMBO_JSLASH:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 0.80; break;
    case JEDI_COMBO_JFLIP:  ws *= 1.25; break;
    case JEDI_COMBO_JUPPER: ws *= 0.70; break;
    case JEDI_COMBO_JBACK:  ws *= 1.25; break;
    case JEDI_COMBO_JSLICE: ws *= 0.50; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.60; break;
    case JEDI_COMBO_JSLASH: ws *= 2.00; break;
    case JEDI_COMBO_JROLL:  ws *= 0.90; break;
   }
   break;
  case JEDI_COMBO_JROLL:
   switch( GET_POWERS(ch,JEDI_COMBO_NEW) )
   {
    case JEDI_COMBO_NONE:               break;
    case JEDI_COMBO_JOVER:  ws *= 0.90; break;
    case JEDI_COMBO_JFLIP:  ws *= 0.70; break;
    case JEDI_COMBO_JUPPER: ws *= 1.25; break;
    case JEDI_COMBO_JBACK:  ws *= 0.50; break;
    case JEDI_COMBO_JSLICE: ws *= 1.25; break;
    case JEDI_COMBO_JSWIPE: ws *= 0.80; break;
    case JEDI_COMBO_JSLASH: ws *= 0.60; break;
    case JEDI_COMBO_JROLL:  ws *= 2.00; break;
   }
   break;
 }
 return ws;
}

void find_jedi_chain_affect( CHAR_DATA *ch, CHAR_DATA *victim )
{
 int i = number_range(1,20);
 AFFECT_DATA af;
 int sn;
 CHAR_DATA *rch;
 OBJ_DATA *obj;

 switch( i )
 {
  default:
   log_string("This should NOT be happening!  find_jedi_chain_affect is b0rked!");
   break;

  case JEDI_COMBO_AFF_HASTE:          /* Haste          */
   act("#7$n eminates a strong force energy and suddenly becomes a blur!#n",ch,NULL,NULL,TO_NOTVICT);
   act("#7You eminate a strong force energy and suddenly become a blur!#n",ch,NULL,NULL,TO_CHAR);
   if( (sn = skill_lookup("haste")) < 0 )
   {
    log_string("JEDI_COMBO_AFF_HASTE: Can not find the haste spell!");
    return;
   }
   af.type = sn;
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.duration = 10;
   af.bitvector = AFF_HASTE;
   affect_to_char(ch,&af);
   break;

  case JEDI_COMBO_AFF_SLOW:           /* Slow           */
   act("#7$n eminates a strong force energy causing $N's movements to slow!#n",ch,NULL,victim,TO_NOTVICT);
   act("#7You eminate a strong force energy causing $N's movements to slow!#n",ch,NULL,victim,TO_CHAR);
   act("#7$n eminates a string force energy causing your movements to slow!#n",ch,NULL,victim,TO_VICT);
   if( (sn = skill_lookup("slow")) < 0 )
   {
    log_string("JEDI_COMBO_AFF_SLOW: Can not find the slow spell!");
    return;
   }
   af.type = sn;
   af.modifier = 0;
   af.location = APPLY_NONE;
   af.duration = 10;
   af.bitvector = AFF2_SLOW;
   affect_to_char2(victim,&af,1);
   break;

  case JEDI_COMBO_AFF_SHEAL:        /* Small Heal     */
   i = number_range(1,1000);
   act2("#7$n surrounds $mself with the force, mending $s cuts. [$D]#n",ch,NULL,NULL,TO_NOTVICT,i);
   act2("#7You surround yourself with the force, mending your cuts. [$D]#n",ch,NULL,NULL,TO_CHAR,i);
   ch->hit += i;
   update_pos(ch);
   break;

  case JEDI_COMBO_AFF_MHEAL:       /* Medium Heal    */
   i = number_range(1000,2000);
   act2("#7$n surrounds $mself with the force, mending $s bones. [$D]#n",ch,NULL,NULL,TO_NOTVICT,i);
   act2("#7You surround yourself with the force, mending your bones. [$D]#n",ch,NULL,NULL,TO_CHAR,i);
   ch->hit += i;
   update_pos(ch);
   break;

  case JEDI_COMBO_AFF_LHEAL:        /* Large Heal     */
   i = number_range(2000,3000);
   act2("#7$n surrounds $mself with the force, mending $s critical wounds. [$D]#n",ch,NULL,NULL,TO_NOTVICT,i);
   act2("#7You surround yourself with the force, mending your critical wounds. [$D]#n",ch,NULL,NULL,TO_CHAR,i);
   ch->hit += i;
   update_pos(ch);
   break;

  case JEDI_COMBO_AFF_SDAM:         /* Small Damage   */
   i = number_range(1,1000);
   act2("#7$n sends a wave of dark energies at $N! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7You send a wave of dark energies at $N! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7$n sends a wave of dark energies at you! [$D]#n",ch,NULL,victim,TO_VICT,i);
   hurt_person(ch,victim,i);
   break;

  case JEDI_COMBO_AFF_MDAM:         /* Medium Damage  */
   i = number_range(1000,2000);
   act2("#7$n concentrates $s hatred and channels it into $N! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7You concentrate your hatred and channel it into $N! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7$n concentrates $s hatred and channels it into you! [$D]#n",ch,NULL,victim,TO_VICT,i);
   hurt_person(ch,victim,i);
   break;

  case JEDI_COMBO_AFF_LDAM:         /* Large Damage   */
   i = number_range(2000,3000);
   act2("#7A coruscating ball of dark matter forms from $n's body, and absorbs itself into $N! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7A coruscating ball of dark matter forms from your body, and absorbs itself into $N! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7A coruscating ball of dark matter forms from $n's body, and absorbs itself into you! [$D]#n",ch,NULL,victim,TO_VICT,i);
   hurt_person(ch,victim,i);
   break;

  case JEDI_COMBO_AFF_TOUGHNESS:    /* Toughness      */
   act("#7A blinding light flashes, and the glint of a rainbow colored barrier is barely visible around $n!#n",ch,NULL,NULL,TO_NOTVICT);
   act("#7A blinding light flashes, and the glint of a rainbow colored barrier is barely visible around you!#n",ch,NULL,NULL,TO_CHAR);
   if( !IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_TOUGH) )
    SET_BIT(GET_POWERS(ch,JEDI_BITS),JEDI_TOUGH);
   break;

  case JEDI_COMBO_AFF_ANTI_SANCT:   /* Anti-Toughness */
   act("#7A stream of blue light eminates from $n, striking $N in $s chest!#n",ch,NULL,victim,TO_NOTVICT);
   act("#7A stream of blue light eminates from you, striking $N in $s chest!#n",ch,NULL,victim,TO_CHAR);
   act("#7A stream of blue light eminates from $n, striking you in your chest!#n",ch,NULL,victim,TO_VICT);
   if( IS_AFFECTED(victim,AFF_SANCTUARY) )
   {
    REMOVE_BIT(victim->affected_by,AFF_SANCTUARY);
    act("The white aura about $N's body fades.",NULL,NULL,victim,TO_ROOM);
    act("The white aura around your body fades.",ch,NULL,NULL,TO_VICT);
   }
   break;

  case JEDI_COMBO_AFF_STUN:         /* Stun           */
   act("#7$n pulls back and slams $s fist into $N's torso; leaving $M stunned!#n",ch,NULL,victim,TO_NOTVICT);
   act("#7You pull back and slam your fist into $N's torso; leaving $M stunned!#n",ch,NULL,victim,TO_CHAR);
   act("#7$n pulls back and slams $s fist into your torso; leaving you stunned!#n",ch,NULL,victim,TO_VICT);
   set_stun(victim,2);
   break;

  case JEDI_COMBO_AFF_STOP_FIGHT:   /* Stop Fight     */
   act("#7A strong presence of peace can be felt as a look of concentration passes over $n's face.#n",ch,NULL,NULL,TO_NOTVICT);
   act("#7A strong presence of peace can be felt as a look of concentration passes over your face.#n",ch,NULL,NULL,TO_CHAR);
   for( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
   {
    if( rch->fighting != NULL )
     stop_fighting(rch,TRUE);
   }
   break;

  case JEDI_COMBO_AFF_UNSTANCE:     /* Unstance       */
   act("#7$n attacks $N's mind with the force, causing $M to lose concentration!#n",ch,NULL,victim,TO_NOTVICT);
   act("#7You attack $N's mind with the force, causing $M to lose concentration!#n",ch,NULL,victim,TO_CHAR);
   act("#7$n attacks your mind with the force, causing you to lose concentration!#n",ch,NULL,victim,TO_VICT);
   if( victim->stance[0] > -1 )
    do_stance(victim,"");
   break;

  case JEDI_COMBO_AFF_DISARM:       /* Disarm         */
   act("#7$n quickly catches $N's weapons with $s lightsaber, flinging them from $N's hands!#n",ch,NULL,victim,TO_NOTVICT);
   act("#7You quickly catch $N's weapons with your lightsaber, flinging them from $S hands!#n",ch,NULL,victim,TO_CHAR);
   act("#7$n quickly catches your weapons with $s lightsaber, flinging them from your hands!#n",ch,NULL,victim,TO_VICT);
   if( (obj = get_eq_char(victim,WEAR_WIELD)) != NULL )
    unequip_char(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_HOLD)) != NULL )
    unequip_char(victim,obj);
   break;

  case JEDI_COMBO_AFF_UBER_JEDI:    /* Uber-Jedi      */
   act("#7$n glows with a dark aura as raw force energies flow into $s body!#n",ch,NULL,NULL,TO_NOTVICT);
   act("#7You glow with a dark aura as raw force energies flow into your body!#n",ch,NULL,NULL,TO_CHAR);
   if( !IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
    SET_BIT(GET_POWERS(ch,JEDI_BITS),JEDI_UBER);
   break;

  case JEDI_COMBO_AFF_LEECH_HEALTH: /* Leech Health   */
   i = number_range(1000,1500);
   act2("#7Raw energies eminate forth from $n, chipping away at $N's lifeforce! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7Raw energies eminate forth from you, chipping away at $N's lifeforce! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7Raw energies eminate forth from $n, chipping away at your lifeforce! [$D]#n",ch,NULL,victim,TO_VICT,i);
   hurt_person(ch,victim,i);
   ch->hit += i;
   update_pos(ch);
   break;

  case JEDI_COMBO_AFF_LEECH_MANA:   /* Leech Mana     */
   i = number_range(1000,1500);
   act2("#7Raw energies eminate forth from $n, chipping away at $N's spiritual energy! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7Raw energies eminate forth from you, chipping away at $N's spiritual energy! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7Raw energies eminate forth from $n, chipping away at your spiritual energy! [$D]#n",ch,NULL,victim,TO_VICT,i);
   victim->mana -= i;
   ch->mana += i;
   update_pos(ch);
   update_pos(victim);
   break;

  case JEDI_COMBO_AFF_LEECH_MOVE:   /* Leech Move     */
   i = number_range(1000,1500);
   act2("#7Raw energies eminate forth from $n, chipping away at $N's vitality! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7Raw energies eminate forth from you, chipping away at $N's vitality! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7Raw energies eminate forth from $n, chipping away at your vitality! [$D]#n",ch,NULL,victim,TO_VICT,i);
   victim->move -= i;
   ch->move += i;
   update_pos(ch);
   update_pos(victim);
   break;

  case JEDI_COMBO_AFF_LEECH_ALL:    /* Leech All      */
   i = number_range(500,1000);
   act2("#7Raw energies eminate forth from $n, chipping away at $N's essence! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   act2("#7Raw energies eminate forth from you, chipping away at $N's essence! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   act2("#7Raw energies eminate forth from $n, chipping away at your essence! [$D]#n",ch,NULL,victim,TO_VICT,i);
   victim->hit -= i;
   victim->mana -= i;
   victim->move -= i;
   ch->hit += i;
   ch->mana += i;
   ch->move += i;
   update_pos(ch);
   update_pos(victim);
   break;

  case JEDI_COMBO_AFF_EQUILIZER:    /* The Equilizer  */
   i = (victim->hit * 0.25);
   act2("#7$n deeply concentrates and rocks the room with an explosion of pure force energy directed at $N! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   i = (ch->hit * 0.125);
   act2("#7$n gets caught up in the blast! [$D]#n",ch,NULL,victim,TO_NOTVICT,i);
   i = (victim->hit * 0.25);
   act2("#7You deeply concentrate and rock the room with an explosion of pure force energy directed at $N! [$D]#n",ch,NULL,victim,TO_CHAR,i);
   i = (ch->hit * 0.125);
   act2("#7You get caught up in the blast! [$D]#n",ch,NULL,NULL,TO_CHAR,i);
   i = (victim->hit * 0.25);
   act2("#7$n deeply concentrates and rocks the room with an explosion of pure force energy directed at you! [$D]#n",ch,NULL,victim,TO_VICT,i);
   i = (ch->hit * 0.125);
   act2("#7$n gets caught up in the blast! [$D]#n",ch,NULL,victim,TO_VICT,i);
   ch->hit *= 0.875;
   victim->hit *= 0.75;
   update_pos(ch);
   update_pos(victim);
   break;
 }
 return;
}

int find_jedi_chain_length( CHAR_DATA *ch )
{
 int ret_val = 3;

 if( ch == NULL )
  return 0;
 if( IS_NPC(ch) )
  return 0;
 if( !IS_CLASS(ch,CLASS_JEDI) )
  return 0;
 switch( GET_POWERS(ch,JEDI_RANK) )
 {
  default:                   ret_val += 0; break;
  case JEDI_RANK_APPRENTICE: ret_val += 0; break;
  case JEDI_RANK_INITIATE:   ret_val += 1; break;
  case JEDI_RANK_GUARDIAN:   ret_val += 1; break;
  case JEDI_RANK_KNIGHT:     ret_val += 2; break;
  case JEDI_RANK_MASTER:     ret_val += 2; break;
  case JEDI_RANK_LORD:       ret_val += 3; break;
 }
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                ret_val += 0; break;
  case JEDI_STYLE_LIGHT:  ret_val += 3; break;
  case JEDI_STYLE_MEDIUM: ret_val += 0; break;
  case JEDI_STYLE_HEAVY:  ret_val -= 3; break;
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   ret_val += 2;
 if( ret_val < 0 )
  ret_val = 0;
 return ret_val;
}

int jedi_eq_counter( CHAR_DATA *ch, bool exempt )
{
 int i = 0;         /* If we use exempt, we count the equipment */
 int ret_val = 0;   /* worn, but skip over all hold, about, and */
 OBJ_DATA *obj;     /* feet items.  --Kline                     */
  
 for( i = 0; i < MAX_WEAR; i++ )
 {
  if( (obj = get_eq_char(ch,i) ) == NULL )
   continue;
  if( exempt )
  {
   if( obj->wear_loc == WEAR_FEET )
    continue;
   if( obj->wear_loc == WEAR_ABOUT )   
    continue;
   if( obj->wear_loc == WEAR_WIELD )
    continue;
   if( obj->wear_loc == WEAR_HOLD )
    continue;
  }
  ret_val++;
 }
 return ret_val;
}

/*** END UTILITY FUNCTIONS ***/

/*** BEGIN MISC COMMANDS ***/

void saber_craft( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 OBJ_DATA *obj;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What do you wish to forge into a lightsaber?\n\r",ch);
  return;
 }
 if( (obj = get_obj_carry(ch,arg)) == NULL )
 {
  stc("You're not carrying that.\n\r",ch);
  return;
 }
 if( ch->move < ch->max_move )
 {
  stc("You must be entirely rested to focus upon your crafting.\n\r",ch);
  return;
 }
 if( IS_SET(obj->quest,QUEST_ARTIFACT) || obj->item_type != ITEM_WEAPON )
 {
  stc("You can not forge a lightsaber from that item.\n\r",ch);
  return;
 }
 if( str_cmp(obj->questowner,ch->pcdata->switchname) )
 {
  stc("You can not forge a lightsaber from a weapon which is not yours.\n\r",ch);
  return;
 }
 if( IS_SET(obj->extra_flags,ITEM_LIGHT_SABER) )
 {
  stc("That has already been forged into a saber of pure energy!\n\r",ch);
  return;
 }
 ch->move = 0;
 SET_BIT(obj->extra_flags,ITEM_LOYAL);
 SET_BIT(obj->extra_flags,ITEM_LIGHT_SABER);
 obj->value[1] += 10;
 obj->value[2] += 15;
 oset_affect(ch,obj,15,APPLY_HITROLL,FALSE);
 oset_affect(ch,obj,15,APPLY_DAMROLL,FALSE);
 oset_affect(ch,obj,-50,APPLY_AC,FALSE);
 oset_affect(ch,obj,3,APPLY_STR,FALSE);
 oset_affect(ch,obj,3,APPLY_INT,FALSE);
 oset_affect(ch,obj,3,APPLY_WIS,FALSE);
 oset_affect(ch,obj,3,APPLY_DEX,FALSE);
 oset_affect(ch,obj,3,APPLY_CON,FALSE);
 stc("You concentrate deeply and craft the weapon into a saber of pure energy!\n\r",ch);
 WAIT_STATE(ch,36);
 return;
}

void jedi_show_powers( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_STRING_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0' )
 {
  display_class_stats(ch);
  sprintf(arg,"EQ EXEMPT: %d\n\r",jedi_eq_counter(ch,TRUE));
  stc(arg,ch);
  sprintf(arg,"EQ FULL  : %d\n\r",jedi_eq_counter(ch,FALSE));
  stc(arg,ch);
  return;
 }
 else if( !str_cmp(arg,"stance") )
 {
  improve_class(ch,JEDI_STANCE);
  return;
 }
 else if( !str_cmp(arg,"attack") )
 {
  improve_class(ch,JEDI_ATTACK);
  return;
 }
 else if( !str_cmp(arg,"defense") )
 {
  improve_class(ch,JEDI_DEFENSE);
  return;
 }
 else if( !str_cmp(arg,"heal") )
 { 
  improve_class(ch,JEDI_FORCE_HEAL);
  return;
 }
 else if( !str_cmp(arg,"absorb") )
 { 
  improve_class(ch,JEDI_FORCE_ABSORB);
  return;
 }
 else if( !str_cmp(arg,"shield") )
 { 
  improve_class(ch,JEDI_FORCE_SHIELD);
  return;
 }
 else if( !str_cmp(arg,"mindtrick") )
 { 
  improve_class(ch,JEDI_FORCE_MIND_TRICK);
  return;
 }
 else if( !str_cmp(arg,"sight") )
 { 
  improve_class(ch,JEDI_FORCE_SIGHT);
  return;
 }
 else if( !str_cmp(arg,"speed") )
 { 
  improve_class(ch,JEDI_FORCE_SPEED);
  return;
 }
 else if( !str_cmp(arg,"jump") )
 { 
  improve_class(ch,JEDI_FORCE_JUMP);
  return;
 }
 else if( !str_cmp(arg,"push") )
 { 
  improve_class(ch,JEDI_FORCE_PUSH);
  return;
 }
 else if( !str_cmp(arg,"pull") )
 { 
  improve_class(ch,JEDI_FORCE_PULL);
  return;
 }
 else if( !str_cmp(arg,"light") )
 { 
  improve_class(ch,JEDI_LIGHT);
  return;
 }
 else if( !str_cmp(arg,"dark") )
 { 
  improve_class(ch,JEDI_DARK);
  return;
 }
 else if( !str_cmp(arg,"neutral") )
 { 
  improve_class(ch,JEDI_NEUTRAL);
  return;
 }
 else if( !str_cmp(arg,"grip") )
 { 
  improve_class(ch,JEDI_FORCE_GRIP);
  return;
 }
 else if( !str_cmp(arg,"rage") )
 { 
  improve_class(ch,JEDI_FORCE_RAGE);
  return;
 }
 else if( !str_cmp(arg,"drain") )
 { 
  improve_class(ch,JEDI_FORCE_DRAIN);
  return;
 }
 else if( !str_cmp(arg,"lightning") )
 { 
  improve_class(ch,JEDI_FORCE_LIGHTNING);
  return;
 }
 else if( !str_cmp(arg,"forcepool") )
 {
  improve_class(ch,JEDI_FORCE_POOL_MAX);
  return;
 }
 return;
}

void combat_style( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);

 if( !IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,JEDI_STANCE) < 1 )
 {
  stc("You have know knowledge of combat styles.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Combat style can be changed with combatstyle <style>\n\r",ch);
  stc("You currently know of the following combat styles:\n\r",ch);
  if( GET_POWERS(ch,JEDI_STANCE) >= 1 )
   stc("  Medium: No bonuses, no penalties.  All around balanced.\n\r",ch);
  if( GET_POWERS(ch,JEDI_STANCE) >= 5 )
   stc("  Light : Faster attacking, less damage taken, less damage dealt.\n\r",ch);
  if( GET_POWERS(ch,JEDI_STANCE) == 10 )
   stc("  Heavy : Slower attacking, more damage taken, more damage dealt.\n\r",ch);
 }
 if( !str_cmp(arg,"medium") )
 {
  GET_POWERS(ch,JEDI_STANCE_STYLE) = JEDI_STYLE_MEDIUM;
  stc("You now fight in the medium combat style.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"light") )
 { 
  GET_POWERS(ch,JEDI_STANCE_STYLE) = JEDI_STYLE_LIGHT;
  stc("You now fight in the light combat style.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"heavy") ) 
 {
  GET_POWERS(ch,JEDI_STANCE_STYLE) = JEDI_STYLE_HEAVY; 
  stc("You now fight in the heavy combat style.\n\r",ch); 
  return;
 }
 return;
}

/*** END MISC COMMANDS ***/

/*** BEGIN COMBO COMMANDS ***/

void jover( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JOVER;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You leap above $N's head, slashing deeply into $M with your lightsaber! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n leaps above your head, slashing deeply into you with $s lightsaber! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n leaps above $N's head, slashing deeply into $M with $s lightsaber! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jflip( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JFLIP;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;   
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You flip over $N's head, slicing into $M with your lightsaber! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n flips over your head, slicing into you with $s lightsaber! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n flips over $N's head, slicing into $M with $s lightsaber! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jupper( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JUPPER;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You sweep your lightsaber in an upwards arc, cutting deep into $N's chest! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n sweeps $s lightsaber in an upwards arc, cutting deep into your chest! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n sweeps $s lightsaber in an upwards arc, cutting deep into $N's chest! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jback( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JBACK;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;   
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You maneuver behind $N and violently thrust your lightsaber into $S back! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n maneuvers behind you and violently thrusts $s lightsaber into your back! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n maneuvers behind $N and violently thrusts $s lightsaber into $S back! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jslice( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JSLICE;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;   
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You slice $N deeply across $E chest! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n slices you deeply across your chest! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n slices $N deeply across $E chest! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jswipe( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JSWIPE;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;   
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You swipe your saber at $N, catching $S arm! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n swipes $s saber at you, catching your arm! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n swipes $s saber at $N, catching $S arm! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jslash( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JSLASH;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;   
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You assault $N with a heavy slash, nearly cleaving $M in two! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n assaults you with a heavy slash, nearly cleaving you in two! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n assaults $N with a heavy slash, nearly cleaving $M in two! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

void jroll( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam = number_range(JEDI_MIN_DAM,JEDI_MAX_DAM);
 int mod = number_range(JEDI_MIN_RAND,JEDI_MAX_RAND);
 int i;
 int ws = JEDI_WAIT;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not even fighting!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_HOLD) == NULL )
 {
  stc("You're not even wielding a weapon!\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_WIELD) != NULL )
 {
  if( !IS_SET(get_eq_char(ch,WEAR_WIELD)->extra_flags,ITEM_LIGHT_SABER) )
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 if( get_eq_char(ch,WEAR_HOLD) != NULL )  
 {
  if( !IS_SET(get_eq_char(ch,WEAR_HOLD)->extra_flags,ITEM_LIGHT_SABER) ) 
  stc("You must be wielding a lightsaber.\n\r",ch);
  return;
 }
 GET_POWERS(ch,JEDI_COMBO_OLD) = GET_POWERS(ch,JEDI_COMBO_NEW);
 GET_POWERS(ch,JEDI_COMBO_NEW) = JEDI_COMBO_JROLL;
 GET_POWERS(ch,JEDI_CHAIN_LENGTH)++;
 if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) > 1 )
 {
  for( i = 0; i < GET_POWERS(ch,JEDI_CHAIN_LENGTH); i++ )
   dam += mod;   
  if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
   dam *= JEDI_DAM_MOD_UBER;
  if( GET_POWERS(ch,JEDI_CHAIN_LENGTH) == find_jedi_chain_length(ch) )
  {
   dam += (mod * JEDI_PEAK_MOD);
   stc("#7Your chain of attacks has reached it's peak!#n\n\r",ch);
   find_jedi_chain_affect(ch,victim);
   GET_POWERS(ch,JEDI_CHAIN_LENGTH) = 0;
  }
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SPEED) )
  ws /= JEDI_WS_MOD_SPEED;
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_UBER) )
  ws /= JEDI_WS_MOD_UBER;
 switch( GET_POWERS(ch,JEDI_STANCE_STYLE) )
 {
  default:                                        break;
  case JEDI_STYLE_LIGHT:  dam *= JEDI_LIGHT_DAM_MOD; ws /= JEDI_LIGHT_WS_MOD; break;
  case JEDI_STYLE_MEDIUM:                         break;
  case JEDI_STYLE_HEAVY:  dam *= JEDI_HEAVY_DAM_MOD; ws *= JEDI_HEAVY_WS_MOD; break;
 }
 snarf_jedi_combo_dam(ch,dam);
 snarf_jedi_combo_ws(ch,ws);
 act2("You nimbly roll underneath $N, slashing into $S legs! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n nimbly rolls underneath you, slashing into your legs! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n nimbly rolls underneath $N, slashing into $S legs! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,ws);
 hurt_person(ch,victim,dam);
 return;
}

/*** END COMBO COMMANDS ***/

/*** BEGIN FORCE POWERS ***/

void jedi_force_heal( CHAR_DATA *ch, char *argument )
{
 int heal = number_range(100,500);
 int i;
 int cost = 800;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,JEDI_DARK) >= 5 )
 {
  stc("A follower of the Sith can not use such a force power.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,JEDI_LIGHT) < 1 )
 {
  stc("You are not trained in the path of the Jedi.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,JEDI_FORCE_HEAL) < 1 )
 {
  stc("You are unlearned in the ways of healing yourself.\n\r",ch);
  return;
 }
 for( i = 0; i < GET_POWERS(ch,JEDI_LIGHT); i++ )
 {
  heal += number_range(100,200);
  cost += 100;
 }
 switch( GET_POWERS(ch,JEDI_FORCE_HEAL) )
 {
  default:                             break;
  case 1:  heal *= 1.07; cost *= 1.03; break;
  case 2:  heal *= 1.14; cost *= 1.06; break;
  case 3:  heal *= 1.21; cost *= 1.09; break;
  case 4:  heal *= 1.28; cost *= 1.12; break;
  case 5:  heal *= 1.35; cost *= 1.15; break;
  case 6:  heal *= 1.42; cost *= 1.18; break;
  case 7:  heal *= 1.49; cost *= 1.21; break;
  case 8:  heal *= 1.56; cost *= 1.24; break;
  case 9:  heal *= 1.63; cost *= 1.27; break;
  case 10: heal *= 1.70; cost *= 1.30; break;
 }
 if( ch->mana < cost )
 {
  act2("You lack the $D mana to heal your wounds.",ch,NULL,NULL,TO_CHAR,cost);
  return;
 }
 act2("You envelop yourself in a faint white light, and feel your pain ease away. [$D]",ch,NULL,NULL,TO_CHAR,heal);
 act2("$n envelops $mself in a faint white light, and $s pain seems to ease away. [$D]",ch,NULL,NULL,TO_ROOM,heal);
 ch->hit += heal;
 ch->mana -= cost;
 update_pos(ch);
 return;
}

void jedi_force_absorb( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_JEDI) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,JEDI_DARK) >= 5 )
 {
  stc("A follower of the Sith can not use such a force power.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,JEDI_LIGHT) < 1 )
 {
  stc("You are not trained in the path of the Jedi.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,JEDI_FORCE_ABSORB) < 1 )
 {
  stc("You are unlearned in the ways of absorbing damage.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_ABSORB) )
 {
  REM_BIT_POWERS(ch,JEDI_BITS,JEDI_ABSORB);
  stc("You relax your focus, and the force energies dissipate from your body.\n\r",ch);
  act("$n relaxes $s focus, and the force energies dissipate from $s body.",ch,NULL,NULL,TO_ROOM);
  return;
 }
 else if( !IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_ABSORB) )
 {
  SET_BIT_POWERS(ch,JEDI_BITS,JEDI_ABSORB);
  stc("You focus deeply, and force energies flow into your body!\n\r",ch);
  act("$n focuses deeply, and force energies flow into $s body!",ch,NULL,NULL,TO_ROOM);
  return;
 }
 return;
}
/*** END FORCE POWERS ***/
