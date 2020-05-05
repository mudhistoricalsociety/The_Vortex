#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_affect( CHAR_DATA *ch, char *argument )

{

	if ( IS_NPC(ch) ) return;

	

	send_to_char("You are affected by:-\n\r", ch);
	send_to_char("-----------------------------------\n\r", ch);
	send_to_char("#1SPELLS#n\n\r", ch);
	send_to_char("\n\r", ch);

	if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
	send_to_char("You are invisible to normal sight.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_HIDE) )
	send_to_char("You are keeping yourself hidden.\n\r", ch );
	if ( IS_AFFECTED(ch, AFF_CHARM) ) 
	send_to_char("You are charmed!\n\r", ch);
      if ( IS_AFFECTED(ch, AFF_PASS_DOOR) )
      send_to_char("You can pass through doors.\n\r", ch );
      if ( IS_AFFECTED(ch, AFF_ETHEREAL) )
      send_to_char("You are in the ethereal plane.\n\r", ch);
      if ( IS_AFFECTED(ch, AFF_FAERIE_FIRE) )
      send_to_char("You are surrounded by fairie Fire.\n\r", ch);
      if ( IS_AFFECTED(ch, AFF_SANCTUARY) ) 
      send_to_char("You are protected by a magical sanctuary.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_BLIND) )
	send_to_char("You are blinded!\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	send_to_char("You are in the shadowplane.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_CURSE) )
	send_to_char("You are cursed!\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_FLYING) )
	send_to_char("You are flying.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	send_to_char("You can see invisible things.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC) )
	send_to_char("You can sense magic.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_DETECT_HIDDEN) )
	send_to_char("You can see hidden things.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_TENDRILS) )
	send_to_char("You are entangled by tendrils!\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_INFIRMITY) )
	send_to_char("You feel slow, weak and less healthy.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_POISON) )
	send_to_char("You are poisoned!\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_PROTECT) )
	send_to_char("You are protected.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	send_to_char("You are sleeping.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_WEBBED) )
	send_to_char("You are covered in a sticky web.\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_DROWFIRE) )
	send_to_char("You are surrounded by Drowfire!\n\r", ch);
	if ( IS_AFFECTED(ch, AFF_POLYMORPH) )
	send_to_char("You are polymorphed.\n\r", ch);
	send_to_char("\n\r", ch);
	send_to_char("-----------------------------------\n\r", ch);
	send_to_char("#1SHIELDS#n\n\r", ch);

	send_to_char("\n\r", ch);
	if ( has_power_level(ch, SCHOOL_ORDER, 7)) 
      send_to_char("Electrical Shield\n\r", ch);
	if ( has_power_level(ch, SCHOOL_FIRE, 6)) 
	send_to_char("Fire Shield\n\r", ch);
	if ( has_power_level(ch, SCHOOL_WATER, 5) ) 
	send_to_char("Ice Shield\n\r", ch);
	if ( has_power_level(ch, SCHOOL_EARTH, 4) ) 
	send_to_char("Acid Shield\n\r", ch);
	if ( has_power_level(ch, SCHOOL_CHAOS, 4)) 
	send_to_char("Chaos Shield\n\r", ch);
        if( has_power_level(ch, SCHOOL_PLANT, 5))
        send_to_char("Thorn Shield\n\r", ch);
	send_to_char("\n\r", ch);
	send_to_char("-----------------------------------\n\r", ch);
        stc("#1Warps#n\n\r",ch);
        if( IS_SET(ch->warp,WARP_CRYSTAL))
         stc("Your body is composed entirely out of crystal.\n\r", ch);
        if( IS_SET(ch->warp, WARP_STEEL))
         stc("Your body is made of solid steel.\n\r", ch);
        if( IS_SET(ch->warp,WARP_VENOM))
         stc("You are coated in a noxious venom.\n\r", ch);
        if( IS_SET(ch->warp,WARP_SPIKE))
         stc("You are covered in sharp spikes.\n\r", ch);
        if( IS_SET(ch->warp,WARP_QUICK))
         stc("You move faster than the eye can see.\n\r", ch);
        if( IS_SET(ch->warp,WARP_SLOW))
         stc("You are stricken with lethargy.\n\r", ch);
        if( IS_SET(ch->warp,WARP_TERROR))
         stc("Your face strikes terror in all who see you.\n\r", ch);
        if( IS_SET(ch->warp,WARP_REGEN))
         stc("Your wounds close faster than normal.\n\r", ch);
        if( IS_SET(ch->warp,WARP_WEAPON))
         stc("You are a master of every weapon you hold.\n\r", ch);
        if( IS_SET(ch->warp,WARP_INEPT))
         stc("You barely know how to use your weapon.\n\r", ch);
        if( IS_SET(ch->warp,WARP_WEAK))
         stc("Your muscles are weak and flabby.\n\r", ch);
        if( IS_SET(ch->warp,WARP_VULN))
         stc("Your skin is extremely soft and delicate.\n\r", ch);
        if( IS_SET(ch->warp,WARP_SHARDS))
         stc("Sharp shards of glass break off when you are attacked.\n\r", ch);
        if( IS_SET(ch->warp,WARP_CLUMSY))
         stc("Your are clumsy and oafish.\n\r", ch);
        if( IS_SET(ch->warp,WARP_TENT))
         stc("Long tentacles spring from your back.\n\r", ch);
        if( IS_SET(ch->warp,WARP_SLOWHEAL))
         stc("Your wounds take a long time to heal.\n\r", ch);
        if( IS_SET(ch->warp,WARP_ARCANE))
         stc("Magic pulses in your blood.\n\r", ch);
        if( IS_SET(ch->warp,WARP_NOMAGE))
         stc("Magic runs thin in your veins.\n\r", ch);
        if( IS_SET(ch->warp,WARP_STRONG))
         stc("Your muscles explode with strength and power.\n\r", ch);
        if( IS_SET(ch->warp, WARP_SMELLY))
         stc("You smell very bad.\n\r", ch);
        if( IS_SET(ch->warp, WARP_WART))
         stc("You ae covered in warts.\n\r", ch);
        if( IS_SET(ch->warp, WARP_FEATHER))
         stc("You are covered in feathers.\n\r", ch);
        if( IS_SET(ch->warp, WARP_HAIRY))
         stc("You are extremely hairy.\n\r", ch);
        if( IS_SET(ch->warp, WARP_BIGFEET))
         stc("You have large feet.\n\r", ch);
        if( IS_SET(ch->warp, WARP_BALD))
         stc("Your head is completely bald.\n\r", ch);
        if( IS_SET(ch->warp, WARP_TAIL))
         stc("You have a cute, fuzzy tail.\n\r", ch);
      return;
}

