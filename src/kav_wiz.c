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



void quest_clone args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int random_group(CHAR_DATA *ch);

void do_damn ( CHAR_DATA *ch, char *argument )
{

	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument, arg);
	if( arg[0] == '\0' )
	{
	  stc("Damn who?\n\r", ch);
	  return;
	}
	if( (victim = get_char_world(ch, arg) ) == NULL )
	{
	  stc("They are not here.\n\r", ch);
	  return;
	}
	if( IS_NPC(victim) )
	{
	  stc("Not on mobs!\n\r", ch);
	  return;
	}
	victim->level = 4;
	victim->trust = 4;
	stc("They are now DAMNED.\n\r", ch);
	stc("Judgment of the gods has left you DAMNED!\n\r", victim);
	set_title(victim, "DAMNED");
	STRFREE(victim->clan);
	victim->clan = str_dup("DAMNED");
	do_save(victim, "");
	return;
}
void do_autoclass ( CHAR_DATA *ch, char* argument )
{
	/* Kline Wuz Here */
	char arg [MAX_INPUT_LENGTH];
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];
	
	smash_tilde( argument );
	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( ch->pcdata->race->complete > 0 )
	{
		stc( "You already have a class!\n\r", ch );
		return;
	}

	if ( arg[0] == '\0' )
	{
		stc( "Syntax: autoclass <class>\n\r", ch );
		stc( "Classes: Apocryphic, Alien, Shadow, Monk, Werewolf, Dragon, Vampire, Drow\n\r", ch );
		stc( "Apocryphics must use a different syntax.\n\r", ch );
		stc( "Apocryphic Syntax: autoclass apocryphic <RaceName> <BannedGroup>.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg,"apocryphic") && ( arg1 == '\0' || arg2 == '\0' ) )
	{
		stc( "Apocryphic Syntax: autoclass apocryphic <RaceName> <BannedGroup>.\n\r", ch );
		return;
	}

	if ( !str_cmp( arg, "apocryphic" ) )
	{
		if( atoi(arg2) < 1 || arg1 == NULL )
		{
			stc( "Invalid selection.\n\r", ch );
			return;
		}

		stc( "You are now an Apocryphic!\n\r", ch );
		ch->class = CLASS_APOCRYPHIC;
		free_string(ch->pcdata->race->name);
		ch->pcdata->race->name = str_dup(arg1);
		ch->pcdata->race->group1 = atoi(arg2);
		ch->pcdata->race->group2 = atoi(arg2);
		ch->pcdata->race->complete = TRUE;
		return;
	}
        else if( !str_cmp( arg, "dragon"))
        {
         stc("You are now a Dragon!\n\r", ch);
         ch->class = CLASS_DRAGON;
         free_string( ch->pcdata->race->name);
         ch->pcdata->race->name = str_dup("Dragon");
         ch->pcdata->race->group1 = -1;
         ch->pcdata->race->group2 = -1;
         ch->pcdata->race->complete = TRUE;
         return;
        }
        else if( !str_cmp(arg, "alien"))
        {
         stc("You are now an Alien!\n\r", ch);
         ch->class = CLASS_ALIEN;
	 free_string(ch->pcdata->race->name);
         ch->pcdata->race->name = str_dup("Alien");
         ch->pcdata->race->group1 = -1;
         ch->pcdata->race->group2 = -1;
         ch->pcdata->race->complete = TRUE;
         return;
        }
        else if( !str_cmp(arg, "monk"))
        {
         stc("You are now a Monk!\n\r", ch);
         ch->class = CLASS_MONK;
         free_string(ch->pcdata->race->name);
	 ch->pcdata->race->name = str_dup("Monk");
         ch->pcdata->race->group1 = -1;
         ch->pcdata->race->group2 = -1;
         ch->pcdata->race->complete = TRUE;
         return;
        }
	else if( !str_cmp(arg, "shadow") )
	{
	 stc("You are now a Shadow!\n\r", ch);
	 ch->class = CLASS_SHADOW;
	 free_string(ch->pcdata->race->name);
	 ch->pcdata->race->name = str_dup("Shadow");
	 ch->pcdata->race->group1 = -1;
	 ch->pcdata->race->group2 = -1;
	 ch->pcdata->race->complete = TRUE;
	 return;
	}
	else if( !str_cmp(arg, "werewolf") )
	{
	 stc("You are now a Werewolf!\n\r",ch);
	 ch->class = CLASS_WOLF;
	 free_string(ch->pcdata->race->name);
	 ch->pcdata->race->name = str_dup("Werewolf");
	 ch->pcdata->race->group1 = -1;
	 ch->pcdata->race->group2 = -1;
	 ch->pcdata->race->complete = TRUE;
	 return;
	}
        else if( !str_cmp(arg, "vampire") ) 
        {
         stc("You are now a Vampire!\n\r",ch);
         ch->class = CLASS_VAMPIRE;
         free_string(ch->pcdata->race->name);
         ch->pcdata->race->name = str_dup("Vampire");
         ch->pcdata->race->group1 = -1;
         ch->pcdata->race->group2 = -1;
         ch->pcdata->race->complete = TRUE;
         GET_POWERS(ch,VAMP_BEAST) = 50;
         return;
        }
	else if( !str_cmp(arg,"drow") )
	{
	 stc("You are now a Drow!\n\r",ch);
	 ch->class = CLASS_DROW;
	 free_string(ch->pcdata->race->name);
	 ch->pcdata->race->name = str_dup("Drow");
	 ch->pcdata->race->group1 = -1;
	 ch->pcdata->race->group2 = -1;
	 ch->pcdata->race->complete = TRUE;
	 return;
	}
        else
	 do_autoclass(ch,"");

	return;
}

void do_redeem( CHAR_DATA *ch, char *argument)
{
	int cost = 500;
	char arg[MAX_INPUT_LENGTH];

	smash_tilde(argument);
	argument = one_argument( argument, arg );
	
	if( IS_NPC(ch) )
		return;
	
	if( arg[0] == '\0' )
	{
		stc( "Syntax: redeem all or redeem <warp>.\n\r", ch );
		stc( "To see a list of valid warps, type redeem list.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "list" ) )
	{
		stc( "Valid redeemable warps are:\n\r", ch );
		stc( "	Crystal	Steel	Vemon	Spikes	Quick\n\r", ch );
		stc( "	Slow	Terror	FastR	WpnMst	Inept\n\r", ch );
		stc( "	Weak	Soft	Glass	Clumsy	Tenta\n\r", ch );
		stc( "	SlowR	SMagic	WMagic	Strong	Smelly\n\r", ch );
		stc( "	Warts	Feather	Hairy	BigFeet	Bald\n\r", ch );
		stc( "	Tail\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "all" ) )
	{
		if( ch->warpcount < 3 )
		{
			stc( "You must have at least 3 warps.\n\r", ch );
			return;
		}

		if( ch->practice < cost )
		{
			stc( "You need 500 primal.\n\r", ch );
			return;
		}
		
		ch->warpcount = 0;
		ch->warp = 0;
		ch->practice -= cost;
		stc( "Your warps have beem redeemed.\n\r", ch );
		return;
	}

	/* Row 1 */

	if ( !str_cmp(arg, "crystal" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_CRYSTAL) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_CRYSTAL;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "steel" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_STEEL) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_STEEL;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "venom" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_VENOM) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_VENOM;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "spikes" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_SPIKE) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_SPIKE;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "quick" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_QUICK) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_QUICK;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	/* Row 2 */

	if ( !str_cmp(arg, "slow" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_SLOW) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_SLOW;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "terror" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_TERROR) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_TERROR;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "fastr" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_REGEN) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_REGEN;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "wpnmst" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_WEAPON) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_WEAPON;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "inept" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_INEPT) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_INEPT;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	/* Row 3 */

	if ( !str_cmp(arg, "weak" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_WEAK) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_WEAK;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "soft" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_VULN) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_VULN;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "glass" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_SHARDS) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_SHARDS;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "clumsy" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_CLUMSY) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_CLUMSY;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "tenta" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_TENT) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_TENT;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}
	
	/* Row 4 */

	if ( !str_cmp(arg, "slowr" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_SLOWHEAL) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_SLOWHEAL;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "smagic" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_ARCANE) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_ARCANE;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "wmagic" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_NOMAGE) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_NOMAGE;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "strong" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_STRONG) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_STRONG;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "smelly" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_SMELLY) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_SMELLY;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	/* Row 5 */

	if ( !str_cmp(arg, "warts" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_WART) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_WART;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "feather" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_FEATHER) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_FEATHER;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "hairy" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_HAIRY) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_HAIRY;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "bigfeet" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_BIGFEET) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_BIGFEET;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	if ( !str_cmp(arg, "bald" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_BALD) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_BALD;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	/* Row 6 */

	if ( !str_cmp(arg, "tail" ) )
	{
		if( ch->practice < (cost*2) )
		{
			stc( "You need 1000 primal.\n\r", ch );
			return;
		}

		if( !IS_SET(ch->warp, WARP_TAIL) )
		{
			stc( "You don't have that warp!\n\r", ch );
			return;
		}

		ch->warpcount--;
		ch->warp -= WARP_TAIL;
		ch->practice -= (cost*2);
		stc( "Your warp has been redeemed.\n\r", ch );
		return;
	}

	else
	{
		stc( "Valid redeemable warps are:\n\r", ch );
		stc( "	Crystal	Steel	Vemon	Spikes	Quick\n\r", ch );
		stc( "	Slow	Terror	FastR	WpnMst	Inept\n\r", ch );
		stc( "	Weak	Soft	Glass	Clumsy	Tenta\n\r", ch );
		stc( "	SlowR	SMagic	WMagic	Strong	Smelly\n\r", ch );
		stc( "	Warts	Feather	Hairy	BigFeet	Bald\n\r", ch );
		stc( "	Tail\n\r", ch );
		return;
	}

	return;
}

void do_setrace( CHAR_DATA *ch, char *argument)
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 int value;

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if( IS_NPC(ch))
  return;
 if( ch->pcdata->race->complete)
 {
  stc("But your race is already complete.\n\r", ch);
  return;
 }
 if( arg1[0] == '\0')
 {
  stc("Set what on your race.\n\r", ch);
  stc("Syntax: setrace <name/group/complete>\n\r", ch);
  return;
 }
 if( !str_cmp( arg1, "name"))
 {
  if( arg2[0] == '\0')
  {
   stc("Set the name to what?\n\r", ch);
   return;
  }
  if( (strlen(arg2) > 10) || (strlen(arg2) < 3))
  {
   stc("The name of a race must be between 3 and 10 characters long.\n\r",ch);
   return;
  }
  free_string(ch->pcdata->race->name);
  ch->pcdata->race->name = str_dup(arg2);
  stc("Race name set.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg1, "group"))
 {
  if( arg2[0] == '\0')
  {
   stc("Bar which group from your race.\n\r", ch);
   return;
  }
  if( !is_number(arg2))
  {
   stc("You must supply a number from 1 to 8.\n\r", ch);
   return;
  }
  value = atoi(arg2);
  if( (value < 1) || (value > 8))
  {
   stc("You must supply a number from 1 to 8.\n\r", ch);
   return;
  }
  ch->pcdata->race->group1 = value;
  stc("Banned group is set.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "complete"))
 {
  if(ch->pcdata->race->name == NULL)
  {
   stc("You must set the name first.\n\r", ch);
   return;
  }
  if( ch->pcdata->race->group1 == 0)
  {
   stc("You must first set a power group that will be banned for you race.\n\r", ch);
   return;
  }
  ch->pcdata->race->group2 = random_group(ch);
  ch->pcdata->race->complete = TRUE;
  stc("Your race is complete.\n\r", ch);
  return;
 }
 else
 {
  stc("That is not a valid option.\n\r", ch);
  return;
 }
 return;
}

void do_showrace(CHAR_DATA *ch, char *argument)
{
 char buf[MAX_STRING_LENGTH];

 if(ch->pcdata->race->name)
  sprintf(buf, "Race name: %s\n\r", up_case(ch->pcdata->race->name));
 else
  sprintf(buf, "Race name: %s\n\r", ch->pcdata->race->name);
 stc(buf, ch);
 sprintf(buf, "Banned group: %d\n\r", ch->pcdata->race->group1);
 stc(buf, ch);
/*  sprintf(buf, "Second banned group: %d\n\r", ch->pcdata->race->group2);
 stc(buf, ch); */
 if( ch->pcdata->race->complete)
  stc("Your race is completed.\n\r", ch);
 else
  stc("Your race has not been completed.\n\r", ch);
 return;
}

int random_group(CHAR_DATA *ch)
{
 int used = ch->pcdata->race->group1;
 int value = number_range(1,8);
 int ret_value = 0;

 if( value == used)
  ret_value = random_group(ch);
 else
  ret_value = value;

 return ret_value;
}
void do_setpaintball(CHAR_DATA *ch, char *argument)
{
  /* syntax "paintball victim <on/off> optional -> <max deaths>"*/
  CHAR_DATA *victim;
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  char arg3[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int deaths = 5;
  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);

  if( IS_NPC(ch))
   return;
  if( !IS_IMMORTAL(ch))
   return;
  if( (victim = get_char_world(ch, arg1)) == NULL)
  {
   stc("They are not logged in.\n\r", ch);
   return;
  }
  /*if( victim->level < 3)
  {
   stc("Avatars only.\n\r", ch);
   return;
  }*/
  if( (arg3[0] != '\0') && !is_number(arg3))
  {
   stc("HOW many deaths...numbers only.\n\r", ch);
   return;
  }
  if( (arg3[0] != '\0') && is_number(arg3))
   deaths = atoi(arg3);
  if( deaths > 10)
  {
   stc("Max deaths is can no higher than 10..setting to 10.\n\r", ch);
   deaths = 10;
  }
  if( !str_cmp(arg2, "on"))
  {
   victim->paintball[PAINT_ON] = 1;
   stc("You have been entered into paintball wars!\n\r", victim);
   victim->paintball[PAINT_DEATHS] = deaths;
   sprintf(buf, "%s has joined the #1p#2a#3i#4n#5t#6b#1a#2l#3l#n wars!\n\r",victim->name);
   do_info(victim, buf);
   return;
  }
  else if( !str_cmp(arg2,"off"))
  {
   victim->paintball[PAINT_ON] = 0;
   stc("You have been removed from paintball wars!\n\r", victim);
   victim->paintball[PAINT_KILLS] = 0;
   victim->paintball[PAINT_DEATHS] = 0;
   sprintf( buf, "%s has been removed from paintball wars!\n\r", victim->name);
   do_info(victim,buf);
   return;
  }
  else
  {
   stc("On or off?\n\r", ch);
   return;
  }
 return;
}

void do_forge( CHAR_DATA *ch, char *argument )
{    

    char arg1 [MAX_STRING_LENGTH];

    char arg2 [MAX_STRING_LENGTH];

    char arg3 [MAX_STRING_LENGTH];

    char buf[MAX_STRING_LENGTH]; 

    OBJ_DATA *obj = NULL;

    OBJ_DATA *obj2 = NULL;

    OBJ_INDEX_DATA *pObjIndex = NULL;

    int value;

    int add;

 

    send_to_char("Please use quest.\n\r",ch);

    return;



    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

 

    smash_tilde( argument );

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    strcpy( arg3, argument );

 

    if ( arg1[0] != '\0' && !str_cmp( arg1, "create" ) && ch->exp > 0 )

    {

        if ( arg2[0] == '\0' )

	{

            send_to_char("Syntax: forge create <element>:\n\rPick from Copper, Iron, Steel, Adamantite.\n\r ",ch);

	    return;

	}

             if (!str_cmp(arg2,"copper"    )) {add = ITEM_COPPER;

value = 250000; pObjIndex = get_obj_index(OBJ_VNUM_COPPER);}

        else if (!str_cmp(arg2,"iron"      )) {add = ITEM_IRON;

value = 1000000; pObjIndex = get_obj_index(30050);}

        else if (!str_cmp(arg2,"steel"     )) {add = ITEM_STEEL;

value = 2500000; pObjIndex = get_obj_index(30051);}

        else if (!str_cmp(arg2,"adamantite")) {add = ITEM_ADAMANTITE;

value = 3000000; pObjIndex = get_obj_index(30052);}

        else if (!str_cmp(arg2,"diamond")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30053);}

        else if (!str_cmp(arg2,"emerald")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30054);}

        else if (!str_cmp(arg2,"sapphire")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30055);}

        else if (!str_cmp(arg2,"ruby")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30056);}

        else if (!str_cmp(arg2,"jade")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30057);}

        else if (!str_cmp(arg2,"pearl")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30058);}

        else if (!str_cmp(arg2,"topaz")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30059);}

        else if (!str_cmp(arg2,"amethyst")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30060);}

        else if (!str_cmp(arg2,"onyx")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30061);}

        else if (!str_cmp(arg2,"opal")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30062);}

        else if (!str_cmp(arg2,"ivory")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30063);}

        else if (!str_cmp(arg2,"lazuli")) {add = ITEM_GEMSTONE;

value = 500000; pObjIndex = get_obj_index(30065);}

        else if (!str_cmp(arg2,"marble")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30067);}

        else if (!str_cmp(arg2,"crystal")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30066);}

        else if (!str_cmp(arg2,"ebony")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30064);}

        else if (!str_cmp(arg2,"gold")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30068);}

        else if (!str_cmp(arg2,"bronze")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30069);}

        else if (!str_cmp(arg2,"sandstone")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30070);}

        else if (!str_cmp(arg2,"limestone")) {add = ITEM_HILT;

value = 750000; pObjIndex = get_obj_index(30071);}



	else

	{

            send_to_char("Syntax: forge create <element>\n\rObject being one of: Copper, Iron, Steel, Adamantite\n\r",ch);

	    return;

	}

        if (ch->exp < value)

	{

            sprintf(buf,"You dont have the required %d experience points.\n\r",value);

	    send_to_char(buf,ch);

	    return;

	}

        if ( ( pObjIndex ) == NULL )

    	{

            send_to_char( "Error...missing object, please inform Infidel or Maser.\n\r", ch );

	    return;

    	}

    	obj = create_object( pObjIndex, 25 );

    	obj->weight = 1;

    	obj->cost   = 1000;

    	obj->item_type = add;

        ch->exp -= value;

    	obj_to_char(obj,ch);

        SET_BIT(obj->quest,QUEST_FREENAME);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->name);

    	if (obj->questowner != NULL) free_string(obj->questowner);

    	obj->questowner = str_dup(ch->name);

        act( "You reach up into the air and draw out $p.", ch, obj, NULL, TO_CHAR );

        act( "$n reaches up into the air and draws out $p.", ch, obj, NULL, TO_ROOM );

	return;

    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )

    {

        send_to_char( "- - - - - - - - - - ----====[  ITEM FORGING ]====---- - - - - - - - - - -\n\r",        ch );

        send_to_char( "Syntax: Forge create (metaltype)\n\r",ch);

        send_to_char( "Create: Creates an element to forge with.\n\r",ch);

        send_to_char( "Syntax: Forge (item) (slab)\n\r",ch);

        send_to_char( "Forge:  Forges the metal of choice into the item.\n\r",ch);

        send_to_char("\n\r",ch);    

        send_to_char("Primary Metals:        Secondary Gemstones:        Hilts:\n\r",ch);

        send_to_char(" Copper		        Diamond.                    Ivory\n\r",ch);

        send_to_char(" Iron		        Emerald.                    Ebony\n\r",ch);

        send_to_char(" Steel		        Sapphire.                   Crystal\n\r",ch);

        send_to_char(" Adamantite	        Ruby.                       Marble\n\r",ch);

        send_to_char("	                Jade.                       Gold\n\r",ch);

        send_to_char("	                Pearl.                      Bronze\n\r",ch);

        send_to_char("	                Topaz.                      Sandstone\n\r",ch);

        send_to_char("	                Amethyst.                   Limestone\n\r",ch);

        send_to_char("	                Onyx.\n\r",ch);

        send_to_char("	                Opal.\n\r",ch);

        send_to_char("	                Lazuli.\n\r",ch);



send_to_char( "- - - - - - - - - - ----====[  ITEM FORGING ]====---- - - - - - - - - -\n\r",        ch );

	return;

    }

   if (!str_cmp(arg3,"slab"))

   {

   send_to_char("You must identify the element name not simply slab.\n\r",ch);

   return;

   }

    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )

    {

	send_to_char( "You are not carrying that item.\n\r", ch );

	return;

    }

         if (!str_cmp( arg2, "copper" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a copper forging slab.\n\r", ch

);

                return;



            }

 if (obj2->item_type!= ITEM_COPPER)

{send_to_char("That thing isnt a slab!\n\r",ch);return;}



            else if (IS_SET(obj->spectype, SITEM_COPPER) ||

IS_SET(obj->spectype, SITEM_IRON) || IS_SET(obj->spectype, SITEM_STEEL) ||

IS_SET(obj->spectype, SITEM_ADAMANTITE))

	    {

                send_to_char("That item is already forged.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_COPPER);

	    ch->level = 12;

	    ch->trust = 12;

            oset_affect(ch,obj,3,APPLY_HITROLL,FALSE);

            oset_affect(ch,obj,3,APPLY_DAMROLL,FALSE);

	    ch->level = 3;

	    ch->trust = 3;

            obj_from_char(obj2);      

            act("You forge copper onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges copper onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

	    obj->points -= 180;    

	return;

 	}

 

         if (!str_cmp( arg2, "iron" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an iron forging slab.\n\r", ch

);

                return;



            }

  if (obj2->item_type != ITEM_IRON)

{send_to_char("That thing isnt a slab!\n\r",ch);return;}

 

            else if (IS_SET(obj->spectype, SITEM_COPPER) ||

IS_SET(obj->spectype, SITEM_IRON) || IS_SET(obj->spectype, SITEM_STEEL) ||

IS_SET(obj->spectype, SITEM_ADAMANTITE))

	    {

                send_to_char("That item is already forged.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_IRON);

            ch->level=12;

            ch->trust=12;

            oset_affect(ch,obj,6,APPLY_HITROLL,FALSE);

            oset_affect(ch,obj,6,APPLY_DAMROLL,FALSE);

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge iron onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges iron onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

	    obj->points -= 360;    

	return;

 	}

 

         if (!str_cmp( arg2, "steel" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a steel forging slab.\n\r", ch

);

                return;



            }

  if (obj2->item_type != ITEM_STEEL)

{send_to_char("That thing isnt a slab!\n\r",ch);return;}



            else if (IS_SET(obj->spectype, SITEM_COPPER) ||

IS_SET(obj->spectype, SITEM_IRON) || IS_SET(obj->spectype, SITEM_STEEL) ||

IS_SET(obj->spectype, SITEM_ADAMANTITE))

	    {

                send_to_char("That item is already forged.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_STEEL);

            ch->level=12;

            ch->trust=12;

            oset_affect(ch,obj,9,APPLY_HITROLL,FALSE);

            oset_affect(ch,obj,9,APPLY_DAMROLL,FALSE);

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge steel onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges steel onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

	    obj->points -= 540;    

	return;

 	}

 

         if (!str_cmp( arg2, "adamantite" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a adamantite forging slab.\n\r", ch);

                return;

  

          }

  if (obj2->item_type != ITEM_ADAMANTITE)

{send_to_char("That thing isnt a slab!\n\r",ch);return;}

 

            else if (IS_SET(obj->spectype, SITEM_COPPER) ||

IS_SET(obj->spectype, SITEM_IRON) || IS_SET(obj->spectype, SITEM_STEEL) ||

IS_SET(obj->spectype, SITEM_ADAMANTITE))

	    {

                send_to_char("That item is already forged.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_ADAMANTITE);

            ch->level=12;

            ch->trust=12;

            oset_affect(ch,obj,12,APPLY_HITROLL,FALSE);

            oset_affect(ch,obj,12,APPLY_DAMROLL,FALSE);

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge adamantite onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges adamantite onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

	    obj->points -= 720;    

	return;

 	}

         if (!str_cmp( arg2, "diamond" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a diamond gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a diamond onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a diamond onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 8000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 8;

           }

         return;

       }

        if (!str_cmp( arg2, "emerald" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an emerald gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge an emerald onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges an emerald onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 13000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 13;

           }

	return;

 	}

        if (!str_cmp( arg2, "sapphire" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a sapphire gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a sapphire onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a sapphire onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 12000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 12;

           }

	return;

 	}

        if (!str_cmp( arg2, "ruby" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a ruby gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a ruby onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a ruby onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 11000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 11;

           }

	return;

 	}

        if (!str_cmp( arg2, "lazuli" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a lazuli gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a lazuli onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a lazuli onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 2000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 2;

           }

	return;

 	}

        if (!str_cmp( arg2, "jade" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require some jade.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge some jade onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges some jade onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 7000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 7;

           }

	return;

 	}

            if (!str_cmp( arg2, "pearl" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a pearl.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a pearl onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a pearl onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 5000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 5;

           }

	return;

 	}

            if (!str_cmp( arg2, "topaz" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a topaz gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a topaz onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a topaz onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 10000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 10;

           }

	return;

 	}

            if (!str_cmp( arg2, "amethyst" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an amethyst gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge an amethyst onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges an amethyst onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 3000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 3;

           }

	return;

 	}

            if (!str_cmp( arg2, "onyx" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an onyx gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge an onyx onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges an onyx onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 9000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 9;

           }

	return;

 	}

            if (!str_cmp( arg2, "opal" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an opal gemstone.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_GEMSTONE)

 	    { send_to_char("That isnt a gemstone.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_GEMSTONE))

	    {

                send_to_char("That item is already forged with a gemstone.\n\r",ch);

		return;

	    }

            SET_BIT(obj->spectype, SITEM_GEMSTONE);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge an opal onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges an opal onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            if (obj->value[0] >= 1000)

		obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    obj->value[0] += 6000;

           }	        

	   else if (obj->item_type == ITEM_ARMOR)

           { 

	    obj->value[3] = 6;

           }

	return;

 	}

            if (!str_cmp( arg2, "ivory" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an ivory hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge an ivory hilt onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges an ivory hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 24;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "ebony" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require an ebony hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge an ebony hilt onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges an ebony hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 4;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "crystal" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a crystal hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a crystal hilt onto $p.\n\r",ch, obj,NULL,TO_CHAR);

            act("$n forges a crystal hilt onto $p.\n\r",ch, obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 30;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "marble" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a marble hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a marble hilt onto $p.\n\r",ch,obj,NULL,TO_CHAR);

            act("$n forges a marble hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 34;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "gold" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a gold hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a gold hilt onto $p.\n\r",ch,obj,NULL,TO_CHAR);

            act("$n forges a gold hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 48;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "bronze" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a bronze hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a bronze hilt onto $p.\n\r",ch,obj,NULL,TO_CHAR);

            act("$n forges a bronze hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 1;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "sandstone" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a sandstone hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a sandstone hilt onto $p.\n\r",ch,obj,NULL,TO_CHAR);

            act("$n forges a sandstone hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 37;

           }	        

	return;

 	}

            if (!str_cmp( arg2, "limestone" ))

   	{

            if ( ( obj2 = get_obj_carry( ch, (arg2))) == NULL )

            {

                send_to_char( "You require a limestone hilt.\n\r", ch);

                return;

            }

	    if (obj2->item_type != ITEM_HILT)

 	    { send_to_char("That isnt a hilt.\n\r",ch); return;}

 

            else if (IS_SET(obj->spectype, SITEM_HILT))

	    {

                send_to_char("That item is already forged with a hilt.\n\r",ch);

		return;

	    }

	   else if (obj->item_type != ITEM_WEAPON)

           { 

	    send_to_char("You can only add hilts to weapons.\n\r",ch);

            return;

	   }



            SET_BIT(obj->spectype, SITEM_HILT);

            ch->level=12;

            ch->trust=12;

            ch->level=3;

            ch->trust=3;

            obj_from_char(obj2);      

            act("You forge a limestone hilt onto $p.\n\r",ch,obj,NULL,TO_CHAR);

            act("$n forges a limestone hilt onto $p.\n\r",ch,obj,NULL,TO_ROOM);	

           if (obj->item_type == ITEM_WEAPON)

           {

            obj->value[0] += 53;

           }	        

	return;

 	}





    if ( arg3[0] == '\0' )

    {

      do_forge(ch,"");

    }

    return;

}



void clear_stats( CHAR_DATA *ch )

{

    OBJ_DATA *obj;

    OBJ_DATA *obj_next;



    if (IS_NPC(ch)) return;



    for ( obj = ch->carrying; obj != NULL; obj = obj_next )

    {

	obj_next = obj->next_content;

	if ( obj->wear_loc != WEAR_NONE )

	{obj_from_char(obj); obj_to_char(obj,ch);}

    }

    while ( ch->affected )

	affect_remove( ch, ch->affected );

    ch->armor			= 100;

    ch->hitroll			= 0;

    ch->damroll			= 0;

    ch->saving_throw		= 0;

    ch->pcdata->mod_str		= 0;

    ch->pcdata->mod_int		= 0;

    ch->pcdata->mod_wis		= 0;

    ch->pcdata->mod_dex		= 0;

    ch->pcdata->mod_con		= 0;

    save_char_obj( ch );

    return;

}







/* In case we need to remove our pfiles, or wanna turn mortal for a bit */
/* Old Relevel --Kline
void do_relevel( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;

    if (!str_cmp(ch->pcdata->switchname,"Kale"))
    {
        ch->level = MAX_LEVEL;
        ch->trust = MAX_LEVEL;
        send_to_char("Wacky!\n\r",ch);
    }    

    else if (!str_cmp(ch->pcdata->switchname, "SaTaN"))
    {
	ch->level = MAX_LEVEL;
	ch->trust = MAX_LEVEL;
	send_to_char("The black moon refreshes your godly state.\n\r", ch);
    }
    else if (!str_cmp(ch->pcdata->switchname,"Sluggo"))
    {
        ch->level = MAX_LEVEL;
        ch->trust = MAX_LEVEL;
        send_to_char("Implementor.\n\r",ch);
    }
    else if (!str_cmp(ch->pcdata->switchname,"Fang"))
    {
        ch->level = MAX_LEVEL;
        ch->trust = MAX_LEVEL;
        send_to_char("You feel your PiMpIn PoWeR return.\n\r",ch);
    }
    else if( !str_cmp(ch->pcdata->switchname, "Kormac"))
    {
      ch->level = LEVEL_HIGHJUDGE;
      ch->trust = LEVEL_HIGHJUDGE;
      send_to_char("AAARRRRRR MATEY!\n\r", ch);
    }
    else if( !str_cmp(ch->pcdata->switchname, "Kline"))
    {
     ch->level = LEVEL_HIGHJUDGE;
     ch->trust = LEVEL_HIGHJUDGE;
     stc("BITCHBOY!!!!\n\r", ch);
    }
    else if( !str_cmp(ch->pcdata->switchname, "Loimos"))
    {
     ch->level = LEVEL_JUDGE;
     ch->trust = LEVEL_HIGHJUDGE;
     stc("Level = Judge  Trust = High Judge\n\r", ch);
    }
    else if( !str_cmp(ch->pcdata->switchname, "Jarra"))
    {
     ch->level = LEVEL_JUDGE;
     ch->trust = LEVEL_JUDGE;
     stc("And what are we going to do tomorrow night Brain?\n\r", ch);
    }
    else if( !str_cmp(ch->pcdata->switchname, "Cataclysm"))
    {
     ch->level = LEVEL_BUILDER;
     ch->trust = LEVEL_BUILDER;
     stc("Bouncy bouncy?\n\r", ch);
    }
    else send_to_char("Huh?\n\r",ch);

    return;
}
*/
void do_relevel( CHAR_DATA *ch, char *argument)
{
 WIZ_DATA *list;
 bool found = FALSE;

 if( IS_NPC(ch))
  return;
 for( list = wiz_first; list; list = list->next)
 {
  if( str_cmp( ch->pcdata->switchname, list->name))
   continue;
  found = TRUE;
  ch->level = list->level;
  ch->trust = list->level;
  stc(list->message, ch);
  stc("\n\r",ch);
  break;
 }
 if( !found)
  do_huh( ch, "");
 return;
}

void do_quest( CHAR_DATA *ch, char *argument )

{

    char arg1 [MAX_STRING_LENGTH];

    char arg2 [MAX_STRING_LENGTH];

    char arg3 [MAX_STRING_LENGTH];

    char buf[MAX_STRING_LENGTH]; 

    char endchar[3];

    OBJ_DATA *obj = NULL;

    OBJ_INDEX_DATA *pObjIndex = NULL;

    int value;

    int add;

    int remove;



    if (IS_NPC(ch)) 

    {

        send_to_char("Not while switched.\n\r",ch);

        return;

    }



    smash_tilde( argument );

    argument = one_argument( argument, arg1 );

    argument = one_argument( argument, arg2 );

    strcpy( arg3, argument );



    if ( arg1[0] != '\0' && !str_cmp( arg1, "create" ) && ch->pcdata->quest > 0 )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to create new objects.\n\r",ch);

	    return;

	}

    	else if ( arg2[0] == '\0' )

	{

	    send_to_char("Syntax: quest create <object> <field>\n\rObject being one of: Light (10 QP), Weapon <type> (50 QP), Armor (30 QP),\n\rContainer (10 QP), Boat (10 QP), Fountain <type> (10 QP), Stake (10 QP).\n\r",ch);

	    return;

	}

    	     if (!str_cmp(arg2,"light"    )) {add = ITEM_LIGHT;     value = 10;}

    	else if (!str_cmp(arg2,"weapon"   )) {add = ITEM_WEAPON;    value = 50;}

    	else if (!str_cmp(arg2,"armor"    )) {add = ITEM_ARMOR;     value = 20;}

    	else if (!str_cmp(arg2,"armour"   )) {add = ITEM_ARMOR;     value = 20;}

    	else if (!str_cmp(arg2,"container")) {add = ITEM_CONTAINER; value = 10;}

    	else if (!str_cmp(arg2,"boat"     )) {add = ITEM_BOAT;      value = 10;}

    	else if (!str_cmp(arg2,"fountain" )) {add = ITEM_FOUNTAIN;  value = 10;}

    	else if (!str_cmp(arg2,"stake"    )) {add = ITEM_STAKE;     value = 10;}

	else

	{

	    send_to_char("Syntax: quest create <object> <field>\n\rObject being one of: Light (1 QP), Weapon <type> (5 QP), Armor (1 QP),\n\rContainer (1 QP), Boat (1 QP), Fountain <type> (1 QP), Stake (1 QP).\n\r",ch);

	    return;

	}

	if (ch->pcdata->quest < value)

	{

	    sprintf(buf,"You dont have the required %d quest points.\n\r",value);

	    send_to_char(buf,ch);

	    return;

	}

    	if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )

    	{

	    send_to_char( "Error...missing object, please inform SaTaN.\n\r", ch );

	    return;

    	}

    	obj = create_object( pObjIndex, 25 );

    	obj->weight = 1;

    	obj->cost   = 1000;

    	obj->item_type = add;

	if (add == ITEM_WEAPON)

	{

	    if (arg3[0] == '\0')

	    {

		send_to_char("Please specify weapon type: Slice, Stab, Slash, Whip, Claw, Blast, Pound, Crush, Pierce, Suck, Grep, or Bite.\n\r",ch);

		if (obj != NULL) extract_obj(obj);

		return;

	    }

	    else if (!str_cmp(arg3,"slice" )) obj->value[3] = 1;

	    else if (!str_cmp(arg3,"stab"  )) obj->value[3] = 2;

	    else if (!str_cmp(arg3,"slash" )) obj->value[3] = 3;

	    else if (!str_cmp(arg3,"whip"  )) obj->value[3] = 4;

	    else if (!str_cmp(arg3,"claw"  )) obj->value[3] = 5;

	    else if (!str_cmp(arg3,"blast" )) obj->value[3] = 6;

	    else if (!str_cmp(arg3,"pound" )) obj->value[3] = 7;

	    else if (!str_cmp(arg3,"crush" )) obj->value[3] = 8;

	    else if (!str_cmp(arg3, "grep" )) obj->value[3] = 9;

	    else if (!str_cmp(arg3, "bite" )) obj->value[3] = 10;

	    else if (!str_cmp(arg3,"pierce")) obj->value[3] = 11;

	    else if (!str_cmp(arg3,"suck"  )) obj->value[3] = 12;

	    else {

		send_to_char("Please specify weapon type: Slice, Stab, Slash, Whip, Claw, Blast, Pound, Crush, Pierce, Suck, Grep, or Bite.\n\r",ch);

		if (obj != NULL) extract_obj(obj);

		return;}

	    obj->value[1] = 10;

	    obj->value[2] = 20;

	    obj->level    = 50;

	}

	else if (add == ITEM_FOUNTAIN)

	{

	    if (arg3[0] == '\0')

	    {

		send_to_char("Please specify fountain contents: Water, Beer, Wine, Ale, Darkale, Whisky,\n\rFirebreather, Specialty, Slime, Milk, Tea, Coffee, Blood, Saltwater.\n\r",ch);

		if (obj != NULL) extract_obj(obj);

		return;

	    }

	    else if (!str_cmp(arg3,"water"        )) obj->value[2] = 0;

	    else if (!str_cmp(arg3,"beer"         )) obj->value[2] = 1;

	    else if (!str_cmp(arg3,"wine"         )) obj->value[2] = 2;

	    else if (!str_cmp(arg3,"ale"          )) obj->value[2] = 3;

	    else if (!str_cmp(arg3,"darkale"      )) obj->value[2] = 4;

	    else if (!str_cmp(arg3,"whisky"       )) obj->value[2] = 5;

	    else if (!str_cmp(arg3,"firebreather" )) obj->value[2] = 7;

	    else if (!str_cmp(arg3,"specialty"    )) obj->value[2] = 8;

	    else if (!str_cmp(arg3,"slime"        )) obj->value[2] = 9;

	    else if (!str_cmp(arg3,"milk"         )) obj->value[2] = 10;

	    else if (!str_cmp(arg3,"tea"          )) obj->value[2] = 11;

	    else if (!str_cmp(arg3,"coffee"       )) obj->value[2] = 12;

	    else if (!str_cmp(arg3,"blood"        )) obj->value[2] = 13;

	    else if (!str_cmp(arg3,"saltwater"    )) obj->value[2] = 14;

	    else {

		send_to_char("Please specify fountain contents: Water, Beer, Wine, Ale, Darkale, Whisky,\n\rFirebreather, Specialty, Slime, Milk, Tea, Coffee, Blood, Saltwater.\n\r",ch);

		if (obj != NULL) extract_obj(obj);

		return;}

	    obj->value[0] = 1000;

	    obj->value[1] = 1000;

	}

	else if (add == ITEM_CONTAINER)

	    obj->value[0] = 999;

	else if (add == ITEM_LIGHT)

	    obj->value[2] = -1;

	else if (add == ITEM_ARMOR)

	    obj->value[0] = 15;

	ch->pcdata->quest -= value;

    	obj_to_char(obj,ch);

	SET_BIT(obj->quest,QUEST_FREENAME);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

    	if (obj->questowner != NULL) free_string(obj->questowner);

    	obj->questowner = str_dup(ch->pcdata->switchname);

    	act( "You reach up into the air and draw out a ball of protoplasm.", ch, obj, NULL, TO_CHAR );

    	act( "$n reaches up into the air and draws out a ball of protoplasm.", ch, obj, NULL, TO_ROOM );

	return;

    }

/*

	send_to_char( "Clone: Clones the object for originals cost (minimum of 5 QP).\n\r",ch );

*/

    if ( arg1[0] == '\0' || arg2[0] == '\0' )

    {

	send_to_char( "- - - - - - - - - - ----====[ QUEST ITEM COSTS ]====---- - - - - - - - - - -\n\r",	 ch );

	send_to_char( "Create: Creates a new personalized object, costing between 10 and 50 QP.\n\r",ch );

	send_to_char( "Name/Short/Long: Rename the object.  1 QP for all three.\n\r",ch );

	send_to_char( "Protection: Sets AC on armour at 1 QP per point.\n\r", ch );

	send_to_char( "Min/Max: Sets min/max damage on weapon at 1 QP per point.\n\r", ch );

	send_to_char( "Weapon: Sets weapon type for 10 QP.\n\r", ch );

	send_to_char( "Extra (add/remove): Glow(1/1), Hum(1/1), Invis(1/1), Anti-Good(1/10),\n\r                    Anti-Neutral(1/10), Anti-Evil(1/10), Loyal(10/1).\n\r",ch );

	send_to_char( "Wear: Select location, costs 20 QPs.  Type 'quest <obj> wear' to see locations.\n\r",ch );

	send_to_char( "Power: Spell power for spell weapons.  Costs 1 QP per power point.\n\r",ch );

	send_to_char( "Spell: Spell weapons or affect.  Costs 50 QP.\n\r",ch );

	send_to_char( "Transporter: Future transportation to that room.  Costs 50 QP.\n\r",ch );

	send_to_char( "Special: Set activate/twist/press/pull flags.\n\r",ch );

	/*send_to_char( "You-in/You-out/Other-in/Other-out: Renames for transporter actions.\n\r",ch );

	send_to_char( "You-wear/You-remove/You-use: What you see when you wear/remove/use.\n\r",ch );

	send_to_char( "Other-wear/Other-remove/Other-use: What others see when you wear/remove/use.\n\r",ch );
        */
	send_to_char( "Weight: Set objects weight to 1.  Costs 10 QP.\n\r",ch );

	send_to_char( "Str, Dex, Int, Wis, Con... max =   3 each, at  20 quest points per +1 stat.\n\r", ch);

	send_to_char( "Hp, Mana, Move............ max =  25 each, at   5 quest points per point.\n\r", ch);

	send_to_char( "Hitroll, Damroll.......... max =   5 each, at  30 quest points per point.\n\r", ch);

	send_to_char( "Ac........................ max = -25,      at  10 points per point.\n\r", ch);

	send_to_char( "- - - - - - - - - - ----====[ QUEST ITEM COSTS ]====---- - - - - - - - - - -\n\r",	 ch );

	return;

    }



    if ( ( obj = get_obj_carry( ch, arg1) ) == NULL )

    {

	send_to_char( "You are not carrying that item.\n\r", ch );

	return;

    }


    if( IS_SET(obj->extra_flags, ITEM_FORGED))
    {
     stc("You can't quest forged items.\n\r", ch);
     return;
    }
    if( IS_SET(obj->extra_flags, ITEM_ONE_RING))
    {
     stc("You cannot improve upon Sauron's magic.\n\r", ch);
     return;
    }
    if( obj->pIndexData->vnum >= 26001 
     && obj->pIndexData->vnum <= 26003 )
    {
     stc("Only demons summoned from the Necronomicon can change that item.\n\r", ch);
     return;
    } 
    if (obj->item_type == ITEM_QUEST)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_QUEST)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_AMMO)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_AMMO)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_EGG)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_EGG)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_VOODOO)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_VOODOO)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_MONEY)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_MONEY)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_TREASURE)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_TREASURE)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_TOOL)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_TOOL)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_SYMBOL)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_SYMBOL)\n\r", ch );
		return;
    }
	else if (obj->item_type == ITEM_PAGE)
    {
		send_to_char( "You cannot quest-change that item.(ITEM_PAGE)\n\r", ch );
		return;
    }
	else if (IS_SET(obj->quest, QUEST_ARTIFACT))
    {
		send_to_char( "You cannot quest-change that item.(QUEST_ARTIFACT)\n\r", ch );
		return;
    }
/*	else if (IS_SET(obj->quest, QUEST_RELIC))
    {
		send_to_char("You cannot quest change that item. (QUEST_RELIC)\n\r", ch);
		return;
    } */


    if (!IS_IMMORTAL(ch) && (obj->questowner == NULL || str_cmp(ch->pcdata->switchname,obj->questowner)))

    {

	send_to_char("You can only change an item you own.\n\r", ch);

	return;

    }



    /*

     * Snarf the value (which need not be numeric).

     */

    if ( arg3[0] != '\0' ) value = atoi( arg3 );

    else value = 0;



    if ( !str_cmp( arg2, "protection" ) )

    {

	if ( arg3[0] == '\0' )

		{send_to_char("How much armor class?\n\r", ch); return;}

	if (obj->item_type != ITEM_ARMOR)

	{

	    send_to_char("That item is not armor.\n\r", ch);

	    return;

	}

	else if (obj->item_type == ITEM_ARMOR && (value+obj->value[0]) > 15)

	{

	    if (obj->value[0] < 15)

	    	sprintf(buf,"The armor class can be increased by %d.\n\r",(15 - obj->value[0]));

	    else

	    	sprintf(buf,"The armor class cannot be increased any further.\n\r");

	    send_to_char(buf, ch);

	    return;

	}

	else if (value > ch->pcdata->quest)

	{

	    send_to_char("You dont have enough quest points.\n\r", ch);

	    return;

	}

	else

		obj->value[0] += value;

	if (obj->value[0] < 0) obj->value[0] = 0;

	send_to_char("Ok.\n\r",ch);

	if (value < 1) value = 1;

	ch->pcdata->quest -= value;

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "min" ) )

    {
		if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
		{
			send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );
			return;
		}


		if ( arg3[0] == '\0' )
	
		{
			send_to_char("How much min damage?\n\r", ch);
			return;
		}

		if (obj->item_type != ITEM_WEAPON)
	
		{

			send_to_char("That item is not a weapon.\n\r", ch);

		    return;

		}

		else if (obj->item_type == ITEM_WEAPON && (value+obj->value[1]) > 10)
	
		{

			if (obj->value[1] < 10)
	
		    	sprintf(buf,"The minimum damage can be increased by %d.\n\r",(10 - obj->value[1]));

			else

	    		sprintf(buf,"The minimum damage cannot be increased any further.\n\r");

			send_to_char(buf, ch);
		
		    return;
		}

		else if (value > ch->pcdata->quest)

		{
	
		    send_to_char("You dont have enough quest points.\n\r", ch);

			return;

		}

		else

			obj->value[1] += value;

		if (obj->value[1] < 1) obj->value[1] = 1;

		send_to_char("Ok.\n\r",ch);

		if (value < 1) value = 1;

		ch->pcdata->quest -= value;

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "max" ) )

    {
			if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
		    {
				send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );
				return;
		    }


	else if ( arg3[0] == '\0' )

		{send_to_char("How much max damage?\n\r", ch); return;}

	if (obj->item_type != ITEM_WEAPON)

	{

	    send_to_char("That item is not a weapon.\n\r", ch);

	    return;

	}

	else if (obj->item_type == ITEM_WEAPON && (value+obj->value[2]) > 20)

	{

	    if (obj->value[2] < 20)

	    	sprintf(buf,"The maximum damage can be increased by %d.\n\r",(20 - obj->value[2]));

	    else

	    	sprintf(buf,"The maximum damage cannot be increased any further.\n\r");

	    send_to_char(buf, ch);

	    return;

	}

	else if (value > ch->pcdata->quest)

	{

	    send_to_char("You dont have enough quest points.\n\r", ch);

	    return;

	}

	else

		obj->value[2] += value;

	if (obj->value[2] < 0) obj->value[2] = 0;

	send_to_char("Ok.\n\r",ch);

	if (value < 1) value = 1;

	ch->pcdata->quest -= value;

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "weapon" ) )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to change weapon types.\n\r",ch);

	    return;

	}

	if (obj->item_type == ITEM_WEAPON)

	{

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

	    if (ch->pcdata->quest < 10)

	    {

	    	send_to_char("You dont have enough quest points.\n\r", ch);

	    	return;

	    }

	    if (arg3[0] == '\0')

	    {

		send_to_char("Please specify weapon type: Slice, Stab, Slash, Whip, Claw, Blast, Pound, Crush, Pierce, Suck, Grep, or Bite.\n\r",ch);

		return;

	    }

	    else if (!str_cmp(arg3,"slice" )) value = 1;

	    else if (!str_cmp(arg3,"stab"  )) value = 2;

	    else if (!str_cmp(arg3,"slash" )) value = 3;

	    else if (!str_cmp(arg3,"whip"  )) value = 4;

	    else if (!str_cmp(arg3,"claw"  )) value = 5;

	    else if (!str_cmp(arg3,"blast" )) value = 6;

	    else if (!str_cmp(arg3,"pound" )) value = 7;

	    else if (!str_cmp(arg3,"crush" )) value = 8;

            else if (!str_cmp(arg3, "grep" )) value = 9;
            
            else if (!str_cmp(arg3, "bite" )) value = 10;

	    else if (!str_cmp(arg3,"pierce")) value = 11;

	    else if (!str_cmp(arg3,"suck"  )) value = 12;

	    else

	    {

		send_to_char("Please specify weapon type: Slice, Stab, Slash, Whip, Claw, Blast, Pound, Crush, Pierce, Suck, Grep or Bite.\n\r",ch);

		return;

	    }

	    if (obj->value[3] == value)

	    {

		send_to_char("It is already that weapon type.\n\r",ch);

		return;

	    }

	    obj->value[3] = value;

	    ch->pcdata->quest -= 10;

	    send_to_char("Ok.\n\r",ch);

	    if (obj->questmaker != NULL) free_string(obj->questmaker);

	    obj->questmaker = str_dup(ch->pcdata->switchname);

	}

	else

	{

	    send_to_char("That item is not a weapon.\n\r", ch);

	    return;

	}

	return;

    }



    if ( !str_cmp( arg2, "extra" ) )

    {
/*
	if (IS_SET(obj->quest, QUEST_RELIC))

	{

	    send_to_char("Not on a relic.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

		else*/ if ( arg3[0] == '\0' )

	{

	    send_to_char("Enter one of: Glow, Hum, Invis, Anti-good, Anti-evil, Anti-neutral, Loyal,\n\rSilver.\n\r",ch);

	    return;

	}

	if      ( !str_cmp( arg3, "glow"         )) {value = ITEM_GLOW; add = 1; remove = 1;}

	else if ( !str_cmp( arg3, "hum"          )) {value = ITEM_HUM; add = 1; remove = 1;}

	else if ( !str_cmp( arg3, "invis"        )) {value = ITEM_INVIS; add = 1; remove = 1;}

	else if ( !str_cmp( arg3, "anti-good"    )) {value = ITEM_ANTI_GOOD; add = 1; remove = 10;}

	else if ( !str_cmp( arg3, "anti-evil"    )) {value = ITEM_ANTI_EVIL; add = 1; remove = 10;}

	else if ( !str_cmp( arg3, "anti-neutral" )) {value = ITEM_ANTI_NEUTRAL; add = 1; remove = 10;}

	else if ( !str_cmp( arg3, "loyal"        )) {value = ITEM_LOYAL; add = 10; remove = 1;}

	else if ( !str_cmp( arg3, "silver"       )) {value = SITEM_SILVER; add = 100; remove = 0;}

	else

	{

	    send_to_char("Enter one of: Glow, Hum, Invis, Anti-good, Anti-evil, Anti-neutral, Loyal,\n\rSilver.\n\r",ch);

	    return;

	}



	if (!str_cmp( arg3, "silver" ))

	{

	    if (IS_SET(obj->spectype, SITEM_SILVER))

	    {

		send_to_char("That item is already silver.\n\r",ch);

		return;

	    }
		if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
	    {

		send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

		return;
	
		}

	    else if (ch->pcdata->quest < add)

	    {

		sprintf(buf,"Sorry, you need %d quest points to set that flag.\n\r",add);

		send_to_char(buf,ch);return;

	    }

	    ch->pcdata->quest -= add;

	    SET_BIT(obj->spectype, SITEM_SILVER);

	}

	else if (IS_SET(obj->extra_flags,value))

	{

	    if (ch->pcdata->quest < remove)

	    {

		sprintf(buf,"Sorry, you need %d quest points to remove that flag.\n\r",remove);

		send_to_char(buf,ch);return;

	    }

	    ch->pcdata->quest -= remove;

	    REMOVE_BIT(obj->extra_flags, value);

	}

	else

	{

	    if (ch->pcdata->quest < add)

	    {

		sprintf(buf,"Sorry, you need %d quest points to set that flag.\n\r",add);

		send_to_char(buf,ch);return;

	    }

	    ch->pcdata->quest -= add;

	    SET_BIT(obj->extra_flags, value);

	}

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "wear" ) )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to change object wear locations.\n\r",ch);

	    return;

	}
	if(IS_SET(obj->quest, QUEST_RELIC))
	{
	send_to_char("Not on a relic.\n\r", ch);
	return;
	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }


        if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	if ( arg3[0] == '\0' )

	{

	    send_to_char("Wear location can be from: Finger, Neck, Body, Head, Legs, Hands, Arms,\n\rAbout, Waist, Wrist, Hold, Face.\n\r",ch);

	    return;

	}

	if      ( !str_cmp( arg3, "finger" )) value = ITEM_WEAR_FINGER;

	else if ( !str_cmp( arg3, "neck"   )) value = ITEM_WEAR_NECK;

	else if ( !str_cmp( arg3, "body"   )) value = ITEM_WEAR_BODY;

	else if ( !str_cmp( arg3, "head"   )) value = ITEM_WEAR_HEAD;

	else if ( !str_cmp( arg3, "legs"   )) value = ITEM_WEAR_LEGS;

	else if ( !str_cmp( arg3, "feet"   )) value = ITEM_WEAR_FEET;

	else if ( !str_cmp( arg3, "hands"  )) value = ITEM_WEAR_HANDS;

	else if ( !str_cmp( arg3, "arms"   )) value = ITEM_WEAR_ARMS;

	else if ( !str_cmp( arg3, "about"  )) value = ITEM_WEAR_ABOUT;

	else if ( !str_cmp( arg3, "waist"  )) value = ITEM_WEAR_WAIST;

	else if ( !str_cmp( arg3, "wrist"  )) value = ITEM_WEAR_WRIST;

	else if ( !str_cmp( arg3, "hold"   )) value = ITEM_WIELD;

	else if ( !str_cmp( arg3, "face"   )) value = ITEM_WEAR_FACE;

	else {

	    send_to_char("Wear location can be from: Finger, Neck, Body, Head, Legs, Hands, Arms,\n\r",ch); 

	    stc("About, Waist, Wrist, Hold, Face.\n\r",ch); return;}

	if (IS_SET(obj->wear_flags,ITEM_TAKE) ) value += 1;

	if (obj->wear_flags == value || obj->wear_flags == (value+1) )

	{

	    act("But $p is already worn in that location!",ch,obj,NULL,TO_CHAR);

	    return;

	}

	else if ( (value != ITEM_WIELD && value != (ITEM_WIELD+1) )

		&& obj->item_type == ITEM_WEAPON)

	{

	    act("You can only HOLD a weapon.",ch,obj,NULL,TO_CHAR);

	    return;

	}

	else if (ch->pcdata->quest < 20 && !(obj->pIndexData->vnum == 30037 && obj->wear_flags == 1))

	{

	    send_to_char("It costs 20 quest points to change a location.\n\r", ch);

	    return;

	}

	if (!(obj->pIndexData->vnum == 30037 && obj->wear_flags == 1)) 

	    ch->pcdata->quest -= 20;

	obj->wear_flags = value;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "replacespell" ) )

    {

	int weapon = 0;

	int affect = 0;

/*	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }*/
        if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	if ( arg3[0] == '\0' )

	{

	    send_to_char("Spell weapons: Sneak, Dark, Holy, Vampiric, Flaming, Electrified, Poisonous.\n\r",ch);

	    send_to_char("Spell affects: Sanct, Blind, Seeinvis, Fly, Infravision, Invis, Passdoor, Protection,\n\r", ch);
		send_to_char("Mages only: Shockshield, Fireshield, Iceshield, Acidshield.\n\r",ch);

	    return;

	}

	if      ( !str_cmp( arg3, "acid"        )) weapon = 1;

	else if ( !str_cmp( arg3, "dark"        )) weapon = 4;

	else if ( !str_cmp( arg3, "holy"        )) weapon = 30;

	else if ( !str_cmp( arg3, "vampiric"    )) weapon = 34;

	else if ( !str_cmp( arg3, "flaming"     )) weapon = 37;

	else if ( !str_cmp( arg3, "electrified" )) weapon = 48;

	else if ( !str_cmp( arg3, "poisonous"   )) weapon = 53;

	else if ( !str_cmp( arg3, "infravision" )) affect = 1;

	else if ( !str_cmp( arg3, "seeinvis"    )) affect = 2;

	else if ( !str_cmp( arg3, "fly"         )) affect = 3;

	else if ( !str_cmp( arg3, "blind"       )) affect = 4;

	else if ( !str_cmp( arg3, "invis"       )) affect = 5;

	else if ( !str_cmp( arg3, "passdoor"    )) affect = 6;

	else if ( !str_cmp( arg3, "protection"  )) affect = 7;

	else if ( !str_cmp( arg3, "sanct"       )) affect = 8;

	else if ( !str_cmp( arg3, "sneak"       )) affect = 9;

	else if ( !str_cmp( arg3, "shockshield" )) affect = 10;

	else if ( !str_cmp( arg3, "fireshield"  )) affect = 11;

	else if ( !str_cmp( arg3, "iceshield"   )) affect = 12;

	else if ( !str_cmp( arg3, "acidshield"  )) affect = 13;

	else

	{

	    send_to_char("Spell weapons: Sneak, Dark, Holy, Vampiric, Flaming, Electrified, Poisonous.\n\r",ch);

	    send_to_char("Spell affects: Sanct, Blind, Seeinvis, Fly, Infravision, Invis, Passdoor, Protection,\n\r", ch);
		send_to_char("Mages only: Shockshield, Fireshield, Iceshield, Acidshield.\n\r",ch);

	    return;

	}



	if (obj->item_type != ITEM_WEAPON && weapon > 0)

	{

	    send_to_char("You can only put that power on a weapon.\n\r", ch);

	    return;

	}

	else if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR && affect > 0)

	{

	    send_to_char("You can only put that power on a weapon or a piece of armour.\n\r", ch);

	    return;

	}

	else if (ch->pcdata->quest < 50 )

	{

	    send_to_char("It costs 50 quest points to create a spell weapon or affect.\n\r", ch);

	    return;

	}

	if (weapon > 0)

	{

	    if (obj->value[0] >= 1000)

		obj->value[0] = ((obj->value[0] / 1000) * 1000);

	    else obj->value[0] = 0;

	    obj->value[0] += weapon;

	}

	else if (affect > 0)

	{

	    if (obj->item_type == ITEM_WEAPON)

	    {

	    	if (obj->value[0] >= 1000)

		    obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    	obj->value[0] += (affect*1000);

	    }

	    else if (obj->item_type == ITEM_ARMOR)

	    	obj->value[3] = affect;

	}

	ch->pcdata->quest -= 50;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "spell" ) )

    {

	int weapon = 0;

	int affect = 0;

/*	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29730))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }*/ /* Need to add Hellish vnums Wibble */

        if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	if ( arg3[0] == '\0' )

	{

	    send_to_char("Spell weapons: Sneak, Dark, Holy, Vampiric, Flaming, Electrified, Poisonous.\n\r",ch);

	    send_to_char("Spell affects: Sanct, Blind, Seeinvis, Fly, Infravision, Invis, Passdoor, Protection,\n\r", ch);
		send_to_char("Mages only: Shockshield, Fireshield, Iceshield, Acidshield.\n\r",ch);

	    return;

	}

	if      ( !str_cmp( arg3, "acid"        )) weapon = 1;

	else if ( !str_cmp( arg3, "dark"        )) weapon = 4;

	else if ( !str_cmp( arg3, "holy"        )) weapon = 30;

	else if ( !str_cmp( arg3, "vampiric"    )) weapon = 34;

	else if ( !str_cmp( arg3, "flaming"     )) weapon = 37;

	else if ( !str_cmp( arg3, "electrified" )) weapon = 48;

	else if ( !str_cmp( arg3, "poisonous"   )) weapon = 53;

	else if ( !str_cmp( arg3, "infravision" )) affect = 1;

	else if ( !str_cmp( arg3, "seeinvis"    )) affect = 2;

	else if ( !str_cmp( arg3, "fly"         )) affect = 3;

	else if ( !str_cmp( arg3, "blind"       )) affect = 4;

	else if ( !str_cmp( arg3, "invis"       )) affect = 5;

	else if ( !str_cmp( arg3, "passdoor"    )) affect = 6;

	else if ( !str_cmp( arg3, "protection"  )) affect = 7;

	else if ( !str_cmp( arg3, "sanct"       )) affect = 8;

	else if ( !str_cmp( arg3, "sneak"       )) affect = 9;

	else if ( !str_cmp( arg3, "shockshield" )) affect = 10;

	else if ( !str_cmp( arg3, "fireshield"  )) affect = 11;

	else if ( !str_cmp( arg3, "iceshield"   )) affect = 12;

	else if ( !str_cmp( arg3, "acidshield"  )) affect = 13;

	else

	{

	    send_to_char("Spell weapons: Sneak, Dark, Holy, Vampiric, Flaming, Electrified, Poisonous.\n\r",ch);

	    send_to_char("Spell affects: Sanct, Blind, Seeinvis, Fly, Infravision, Invis, Passdoor, Protection,\n\r", ch);
		send_to_char("Mages only: Shockshield, Fireshield, Iceshield, Acidshield.\n\r",ch);

	    return;

	}



	if (obj->item_type != ITEM_WEAPON && weapon > 0)

	{

	    send_to_char("You can only put that power on a weapon.\n\r", ch);

	    return;

	}

	else if (obj->item_type != ITEM_WEAPON && obj->item_type != ITEM_ARMOR && affect > 0)

	{

	    send_to_char("You can only put that power on a weapon or a piece of armour.\n\r", ch);

	    return;

	}

	else if (ch->pcdata->quest < 50 )

	{

	    send_to_char("It costs 50 quest points to create a spell weapon or affect.\n\r", ch);

	    return;

	}

	if (weapon > 0)

	{

	    if ( obj->value[0] - ((obj->value[0] / 1000) * 1000) != 0 )

	    {

		send_to_char("That item already has a spell weapon power.  If you wish to replace the \n\rcurrent spell power, use the format: quest <object> replacespell <spell>.\n\r",ch);

		return;

	    }

	    if (obj->value[0] >= 1000)

		obj->value[0] = ((obj->value[0] / 1000) * 1000);

	    else obj->value[0] = 0;

	    obj->value[0] += weapon;

	}

	else if (affect > 0)

	{

	    if (obj->item_type == ITEM_WEAPON)

	    {

	    	if ( obj->value[0] >= 1000 )

	    	{

		    send_to_char("That item already has a spell affect power.  If you wish to replace the \n\rcurrent spell power, use the format: quest <object> replacespell <spell>.\n\r",ch);

		    return;

	    	}

	    	if (obj->value[0] >= 1000)

		    obj->value[0] -= ((obj->value[0] / 1000) * 1000);

	    	obj->value[0] += (affect*1000);

	    }

	    else if (obj->item_type == ITEM_ARMOR)

	    {

	    	if ( obj->value[3] > 0 )

	    	{

		    send_to_char("That item already has a spell affect power.  If you wish to replace the \n\rcurrent spell power, use the format: quest <object> replacespell <spell>.\n\r",ch);

		    return;

	    	}

	    	obj->value[3] = affect;

	    }

	}

	ch->pcdata->quest -= 50;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "power" ) )

    {

	if ( arg3[0] == '\0' )

	{

	    send_to_char("Please specify the amount of power.\n\r",ch);

	    return;

	}

	if (obj->item_type != ITEM_WEAPON)

	{

	    send_to_char("Only weapons have a spell power.\n\r",ch);

	    return;

	}
	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

	else if (obj->level >= 50)

	{

	    send_to_char("This weapon can hold no more spell power.\n\r",ch);

	    return;

	}

	else if (value + obj->level > 50)

	{

	    sprintf(buf,"You can only add %d more spell power to this weapon.\n\r",(50 - obj->level));

	    send_to_char(buf,ch);

	    return;

	}
	else if (value < 0)
	{
		send_to_char("You cant seem to do that.\n\r", ch);
		return;
	}

	else if (value > ch->pcdata->quest)

	{

	    send_to_char("You dont have enough quest points to increase the spell power.\n\r",ch);

	    return;

	}

	obj->level += value;

	if (obj->level < 0) obj->level = 0;

	if (value < 1) value = 1;

	ch->pcdata->quest -= value;

	send_to_char("Ok.\n\r",ch);

	if (obj->questmaker != NULL) free_string(obj->questmaker);

	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }

	

    if ( !str_cmp( arg2, "weight" ) )

    {

	if (obj->weight < 2)

	{

	    send_to_char("You cannot reduce the weight of this item any further.\n\r",ch);

	    return;

	}

	else if (ch->pcdata->quest < 10)

	{

	    send_to_char("It costs 10 quest point to remove the weight of an object.\n\r",ch);

	    return;

	}

	obj->weight = 1;

	ch->pcdata->quest -= 10;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "transporter" ) )

    {
	
	if( ch->in_room->vnum == 26 )
	{
	  stc("You cannot make a transporter here.\n\r", ch);
	  return;
	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
       {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

       }

        if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

    	if (IS_SET(obj->spectype, SITEM_TELEPORTER))

	{ send_to_char("This item is already a transporter.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_TRANSPORTER))

	{ send_to_char("This item is already a teleporter.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_SPELL))

	{ send_to_char("This item is already a spell caster.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_OBJECT))

	{ send_to_char("This item is already an object creator.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_MOBILE))

	{ send_to_char("This item is already a creature creator.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_ACTION))

	{ send_to_char("This item is already a commanding device.\n\r",ch);

	  return;}

	else if (ch->pcdata->quest < 50)

	{

	    send_to_char("It costs 50 quest point to create a transporter.\n\r",ch);

	    return;

	}

	SET_BIT(obj->spectype, SITEM_ACTIVATE);

	//SET_BIT(obj->spectype, SITEM_TELEPORTER);

        SET_BIT(obj->spectype, SITEM_TRANSPORTER);

	obj->specpower = ch->in_room->vnum;

	ch->pcdata->quest -= 50;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

    	if (obj->chpoweron != NULL) free_string(obj->chpoweron);

    	obj->chpoweron = str_dup("You transform into a fine mist and seep into the ground.");

    	if (obj->victpoweron != NULL) free_string(obj->victpoweron);

    	obj->victpoweron = str_dup("$n transforms into a fine mist and seeps into the ground.");

    	if (obj->chpoweroff != NULL) free_string(obj->chpoweroff);

    	obj->chpoweroff = str_dup("You seep up from the ground and reform your body.");

    	if (obj->victpoweroff != NULL) free_string(obj->victpoweroff);

    	obj->victpoweroff = str_dup("A fine mist seeps up from the ground and reforms into $n.");

    	if (obj->chpoweruse != NULL) free_string(obj->chpoweruse);

    	obj->chpoweruse = str_dup("You activate $p.");

    	if (obj->victpoweruse != NULL) free_string(obj->victpoweruse);

    	obj->victpoweruse = str_dup("$n activates $p.");

	return;

    }



    /*if ( !str_cmp( arg2, "retransporter" ) )

    {

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }
        if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

    	if (!IS_SET(obj->spectype, SITEM_TELEPORTER))

	{ send_to_char("This item is not a transporter.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_TRANSPORTER))

	{ send_to_char("This item is already a teleporter.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_SPELL))

	{ send_to_char("This item is already a spell caster.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_OBJECT))

	{ send_to_char("This item is already an object creator.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_MOBILE))

	{ send_to_char("This item is already a creature creator.\n\r",ch);

	  return;}

    	else if (IS_SET(obj->spectype, SITEM_ACTION))

	{ send_to_char("This item is already a commanding device.\n\r",ch);

	  return;}

	else if (ch->pcdata->quest < 50)

	{

	    send_to_char("It costs 50 quest point to create a transporter.\n\r",ch);

	    return;

	}

	SET_BIT(obj->spectype, SITEM_ACTIVATE);

	SET_BIT(obj->spectype, SITEM_TELEPORTER);

	obj->specpower = ch->in_room->vnum;

	ch->pcdata->quest -= 50;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->pcdata->switchname);

	return;

    }



    if ( !str_cmp( arg2, "clone" ) )

    {

	if (IS_SET(obj->quest, QUEST_RELIC))

	{

	    send_to_char("Not on a relic.\n\r",ch);

	    return;

	}

        if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	value = obj->points;

	if (value < 5 || obj->item_type == ITEM_PAGE) value = 5;

	if (obj->item_type == ITEM_QUEST || obj->item_type == ITEM_QUESTCARD)

	{

	    send_to_char("I think not...\n\r",ch);

	    return;

	}

	else if (obj->questowner == NULL || str_cmp(obj->questowner,ch->pcdata->switchname))

	{

	    send_to_char("You can only clone something you own.\n\r",ch);

	    return;

	}

	else if (ch->pcdata->quest < value)

	{

	    sprintf(buf,"It will cost you %d quest points to clone %s.\n\r",

		value,obj->short_descr);

	    send_to_char(buf,ch);

	    return;

	}

	ch->pcdata->quest -= value;

	quest_clone(ch,obj);

	return;

    }

*/

    if ( arg3[0] == '\0' )

    {

	send_to_char( "- - - - - - - - - - ----====[ QUEST ITEM COSTS ]====---- - - - - - - - - - -\n\r",	 ch );

	send_to_char( "Create: Creates a new personalized object, costing between 10 and 50 QP.\n\r",ch );

	send_to_char( "Name/Short/Long: Rename the object.  1 QP for all three.\n\r",ch );

	send_to_char( "Protection: Sets AC on armour at 1 QP per point.\n\r", ch );

	send_to_char( "Min/Max: Sets min/max damage on weapon at 1 QP per point.\n\r", ch );

	send_to_char( "Weapon: Sets weapon type for 10 QP.\n\r", ch );

	send_to_char( "Extra (add/remove): Glow(1/1), Hum(1/1), Invis(1/1), Anti-Good(1/10),\n\r                    Anti-Neutral(1/10), Anti-Evil(1/10), Loyal(10/1).\n\r",ch );

	send_to_char( "Wear: Select location, costs 20 QPs.  Type 'quest <obj> wear' to see locations.\n\r",ch );

	send_to_char( "Power: Spell power for spell weapons.  Costs 1 QP per power point.\n\r",ch );

	send_to_char( "Spell: Spell weapons or affect.  Costs 50 QP.\n\r",ch );

	send_to_char( "Transporter: Future transportation to that room.  Costs 50 QP.\n\r",ch );

	send_to_char( "Special: Set activate/twist/press/pull flags.\n\r",ch );

/*	send_to_char( "You-in/You-out/Other-in/Other-out: Renames for transporter actions.\n\r",ch );

	send_to_char( "You-wear/You-remove/You-use: What you see when you wear/remove/use.\n\r",ch );

	send_to_char( "Other-wear/Other-remove/Other-use: What others see when you wear/remove/use.\n\r",ch );
*/
	send_to_char( "Weight: Set objects weight to 1.  Costs 10 QP.\n\r",ch );

	send_to_char( "Str, Dex, Int, Wis, Con... max =   3 each, at  20 quest points per +1 stat.\n\r", ch);

	send_to_char( "Hp, Mana, Move............ max =  25 each, at   5 quest points per point.\n\r", ch);

	send_to_char( "Hitroll, Damroll.......... max =   5 each, at  30 quest points per point.\n\r", ch);

	send_to_char( "Ac........................ max = -25,      at  10 points per point.\n\r", ch);

	send_to_char( "- - - - - - - - - - ----====[ QUEST ITEM COSTS ]====---- - - - - - - - - - -\n\r",	 ch );

	return;

    }



    if (obj->item_type != ITEM_BOOK)

    {

    	if      ( !str_cmp( arg2, "hitroll" ) || !str_cmp( arg2, "hit" ) )

	    {oset_affect(ch,obj,value,APPLY_HITROLL,TRUE);return;}

    	else if ( !str_cmp( arg2, "damroll" ) || !str_cmp( arg2, "dam" ) )

	    {oset_affect(ch,obj,value,APPLY_DAMROLL,TRUE);return;}

    	else if ( !str_cmp( arg2, "armor" ) || !str_cmp( arg2, "ac" ) )

	    {oset_affect(ch,obj,value,APPLY_AC,TRUE);return;}

    	else if ( !str_cmp( arg2, "hitpoints" ) || !str_cmp( arg2, "hp" ) )
	{
	 if(value < 1)
	 {
	  stc("Value must be positive.\n\r", ch);
	  return;
	 }
	 else
	    {oset_affect(ch,obj,value,APPLY_HIT,TRUE);return;}
	}
    	else if ( !str_cmp( arg2, "mana" ) )
	{
         if(value < 1)
         {  
          stc("Value must be positive.\n\r", ch);
          return;
         }  
         else
	    {oset_affect(ch,obj,value,APPLY_MANA,TRUE);return;}
	}
    	else if ( !str_cmp( arg2, "move" ) || !str_cmp( arg2, "movement" ) )
	{
         if(value < 1)
         {  
          stc("Value must be positive.\n\r", ch);
          return;
         }  
         else
	    {oset_affect(ch,obj,value,APPLY_MOVE,TRUE);return;}
	}
    	else if ( !str_cmp( arg2, "str" ) || !str_cmp( arg2, "strength" ) )

	    {oset_affect(ch,obj,value,APPLY_STR,TRUE);return;}

    	else if ( !str_cmp( arg2, "dex" ) || !str_cmp( arg2, "dexterity" ) )

	    {oset_affect(ch,obj,value,APPLY_DEX,TRUE);return;}

    	else if ( !str_cmp( arg2, "int" ) || !str_cmp( arg2, "intelligence" ) )

	    {oset_affect(ch,obj,value,APPLY_INT,TRUE);return;}

    	else if ( !str_cmp( arg2, "wis" ) || !str_cmp( arg2, "wisdom" ) )

	    {oset_affect(ch,obj,value,APPLY_WIS,TRUE);return;}

    	else if ( !str_cmp( arg2, "con" ) || !str_cmp( arg2, "constitution" ) )

	    {oset_affect(ch,obj,value,APPLY_CON,TRUE);return;}

    }



    if ( !str_cmp( arg2, "name" ) )

    {

	value = 1;

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to rename objects.\n\r",ch);

	    return;

	}

	if(IS_SET(obj->quest, QUEST_RELIC))
	{
	send_to_char("Not on a relic.\n\r", ch);
	return;
	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

	if (!IS_SET(obj->quest,QUEST_NAME) && 

	    (IS_SET(obj->quest,QUEST_SHORT) || IS_SET(obj->quest,QUEST_LONG)))

	{

	    SET_BIT(obj->quest,QUEST_NAME);

	    value = 0;

	}

	else if (IS_SET(obj->quest,QUEST_NAME))

	{

	    REMOVE_BIT(obj->quest,QUEST_SHORT);

	    REMOVE_BIT(obj->quest,QUEST_LONG);

	}

	else

	    SET_BIT(obj->quest,QUEST_NAME);

	if (IS_SET(obj->quest,QUEST_FREENAME))

	{

	    value = 0;

	    REMOVE_BIT(obj->quest,QUEST_FREENAME);

	}

	if (ch->pcdata->quest < value)

	{

	    send_to_char("It costs 1 quest point to rename an object.\n\r",ch);

	    return;

	}

	if (strlen(arg3) < 3)

	{

	    send_to_char("Name should be at least 3 characters long.\n\r",ch);

	    return;

	}

	ch->pcdata->quest -= value;

	strcpy(arg3,capitalize(arg3));

	arg3[0] = LOWER(arg3[0]);

	endchar[0] = (arg3[strlen(arg3)-1]);

	endchar[1] = '\0';

	if (!str_cmp(endchar,".")) arg3[strlen(arg3)-1] = '\0';

	free_string( obj->name );

	obj->name = str_dup( arg3 );

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->name);

	return;

    }



    if ( !str_cmp( arg2, "short" ) )

    {

	value = 1;

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to rename objects.\n\r",ch);

	    return;

	}

	if(IS_SET(obj->quest, QUEST_RELIC))
	{
	send_to_char("Not on a relic\n\r", ch);
	return;
	}
       if( (obj->item_type == ITEM_METAL)
        || (obj->item_type == ITEM_GEM)
        || (obj->item_type == ITEM_ORE))
        {
         stc("No.\n\r", ch);
         return;
        }

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }
	if (!IS_SET(obj->quest,QUEST_SHORT) && 

	    (IS_SET(obj->quest,QUEST_NAME) || IS_SET(obj->quest,QUEST_LONG)))

	{

	    SET_BIT(obj->quest,QUEST_SHORT);

	    value = 0;

	}

	else if (IS_SET(obj->quest,QUEST_SHORT))

	{

	    REMOVE_BIT(obj->quest,QUEST_NAME);

	    REMOVE_BIT(obj->quest,QUEST_LONG);

	}

	else

	    SET_BIT(obj->quest,QUEST_SHORT);

	if (IS_SET(obj->quest,QUEST_FREENAME))

	{

	    value = 0;

	    REMOVE_BIT(obj->quest,QUEST_FREENAME);

	}

	if (ch->pcdata->quest < value)

	{

	    send_to_char("It costs 1 quest point to rename an object.\n\r",ch);

	    return;

	}

	if (strlen(arg3) < 3)

	{

	    send_to_char("Name should be at least 3 characters long.\n\r",ch);

	    return;

	}

	ch->pcdata->quest -= value;

/*

	arg3[0] = LOWER(arg3[0]);

*/

	endchar[0] = (arg3[strlen(arg3)-1]);

	endchar[1] = '\0';

	if (!str_cmp(endchar,".")) arg3[strlen(arg3)-1] = '\0';

	free_string( obj->short_descr );

	obj->short_descr = str_dup( arg3 );

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->name);

	return;

    }



    if ( !str_cmp( arg2, "long" ) )

    {

	value = 1;

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to rename objects.\n\r",ch);

	    return;

	}

	if(IS_SET(obj->quest, QUEST_RELIC))
	{
	send_to_char("Not on a relic.\n\r", ch);
	return;
	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }
	if (!IS_SET(obj->quest,QUEST_LONG) && 

	    (IS_SET(obj->quest,QUEST_NAME) || IS_SET(obj->quest,QUEST_SHORT)))

	{

	    SET_BIT(obj->quest,QUEST_LONG);

	    value = 0;

	}

	else if (IS_SET(obj->quest,QUEST_LONG))

	{

	    REMOVE_BIT(obj->quest,QUEST_NAME);

	    REMOVE_BIT(obj->quest,QUEST_SHORT);

	}

	else

	    SET_BIT(obj->quest,QUEST_LONG);

	if (IS_SET(obj->quest,QUEST_FREENAME))

	{

	    value = 0;

	    REMOVE_BIT(obj->quest,QUEST_FREENAME);

	}

	if (ch->pcdata->quest < value)

	{

	    send_to_char("It costs 1 quest point to rename an object.\n\r",ch);

	    return;

	}

	if (strlen(arg3) < 3)

	{

	    send_to_char("Name should be at least 3 characters long.\n\r",ch);

	    return;

	}

	ch->pcdata->quest -= value;

	arg3[0] = UPPER(arg3[0]);

	endchar[0] = (arg3[strlen(arg3)-1]);

	endchar[1] = '\0';

	if (str_cmp(endchar,".")) strcat(arg3,".");

	free_string( obj->description );

	obj->description = str_dup( arg3 );

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->name);

	return;

    }



    if ( !str_cmp( arg2, "ed" ) )

    {

	EXTRA_DESCR_DATA *ed;



	argument = one_argument( argument, arg3 );

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not allowed to rename objects.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }


	if ( argument == NULL )

	{

	    send_to_char( "Syntax: quest <object> ed <keyword> <string>\n\r",

		ch );

	    return;

	}



	if ( extra_descr_free == NULL )

	{

	    ed			= alloc_perm( sizeof(*ed) );

	}

	else

	{

	    ed			= extra_descr_free;

	    extra_descr_free	= ed;

	}



	strcpy(buf, argument);

	strcat(buf, "\n\r");

	buf[0] = UPPER(buf[0]);



	ed->keyword		= str_dup( arg3 );

	ed->description		= str_dup( buf  );

	ed->next		= obj->extra_descr;

	obj->extra_descr	= ed;

	send_to_char("Ok.\n\r",ch);

    	if (obj->questmaker != NULL) free_string(obj->questmaker);

    	obj->questmaker = str_dup(ch->name);

	return;

    }



    if (!str_cmp(arg2,"special"))

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }
	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	if (arg3[0] == '\0')

	{

	    send_to_char("Please enter ACTIVATE, TWIST, PRESS or PULL.\n\r",ch);

	    return;

	}

	if (str_cmp(arg3,"activate") && str_cmp(arg3,"twist") &&

	    str_cmp(arg3,"press"   ) && str_cmp(arg3,"pull" ))

	{

	    send_to_char("Please enter ACTIVATE, TWIST, PRESS or PULL.\n\r",ch);

	    return;

	}

	if (IS_SET(obj->spectype, SITEM_ACTIVATE))

	    REMOVE_BIT(obj->spectype, SITEM_ACTIVATE);

	if (IS_SET(obj->spectype, SITEM_TWIST))

	    REMOVE_BIT(obj->spectype, SITEM_TWIST);

	if (IS_SET(obj->spectype, SITEM_PRESS))

	    REMOVE_BIT(obj->spectype, SITEM_PRESS);

	if (IS_SET(obj->spectype, SITEM_PULL))

	    REMOVE_BIT(obj->spectype, SITEM_PULL);

	if (!str_cmp(arg3,"activate")) SET_BIT(obj->spectype, SITEM_ACTIVATE);

	if (!str_cmp(arg3,"twist"   )) SET_BIT(obj->spectype, SITEM_TWIST);

	if (!str_cmp(arg3,"press"   )) SET_BIT(obj->spectype, SITEM_PRESS);

	if (!str_cmp(arg3,"pull"    )) SET_BIT(obj->spectype, SITEM_PULL);

	arg3[0] = UPPER(arg3[0]);

	sprintf(buf,"%s flag set.\n\r",arg3);

	send_to_char(buf,ch);

	return;

    }


/*
    if (!str_cmp(arg2,"you-out") || !str_cmp(arg2,"you-wear"))

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}
	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }
	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"you-out") && !IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("That item is not a transporter.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"you-wear") && IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("Not on a transporter.\n\r",ch);

	    return;

	}

	if (obj->chpoweron != NULL) strcpy(buf,obj->chpoweron);

	else {send_to_char("There is a problem with this item, please inform an implementor.\n\r",ch);return;}

	if (!str_cmp(arg3,"clear"))

	{

	    free_string( obj->chpoweron );

	    obj->chpoweron = str_dup( "(null)" );

	}

	else if (obj->chpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))

	{

	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)

	    {

		send_to_char("Line too long.\n\r",ch);

		return;

	    }

	    else

	    {

	    	free_string( obj->chpoweron );

	    	strcat( buf, "\n\r" );

	    	strcat( buf, arg3 );

	    	obj->chpoweron = str_dup( buf );

	    }

	}

	else

	{

	    free_string( obj->chpoweron );

	    obj->chpoweron = str_dup( arg3 );

	}

	send_to_char("Ok.\n\r",ch);

    }

    else if (!str_cmp(arg2,"other-out") || !str_cmp(arg2,"other-wear") )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"other-out") && !IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("That item is not a transporter.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"other-wear") && IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("Not on a transporter.\n\r",ch);

	    return;

	}

	if (obj->victpoweron != NULL) strcpy(buf,obj->victpoweron);

	else {send_to_char("There is a problem with this item, please inform an implementor.\n\r",ch);return;}

	if (!str_cmp(arg3,"clear"))

	{

	    free_string( obj->victpoweron );

	    obj->victpoweron = str_dup( "(null)" );

	}

	else if (obj->victpoweron != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))

	{

	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)

	    {

		send_to_char("Line too long.\n\r",ch);

		return;

	    }

	    else

	    {

	    	free_string( obj->victpoweron );

	    	strcat( buf, "\n\r" );

	    	strcat( buf, arg3 );

	    	obj->victpoweron = str_dup( buf );

	    }

	}

	else

	{

	    free_string( obj->victpoweron );

	    obj->victpoweron = str_dup( arg3 );

	}

	send_to_char("Ok.\n\r",ch);

    }

    else if (!str_cmp(arg2,"you-in") || !str_cmp(arg2,"you-remove") )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
        {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

        }
	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"you-in") && !IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("That item is not a transporter.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"you-remove") && IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("Not on a transporter.\n\r",ch);

	    return;

	}

	if (obj->chpoweroff != NULL) strcpy(buf,obj->chpoweroff);

	else {send_to_char("There is a problem with this item, please inform an implementor.\n\r",ch);return;}

	if (!str_cmp(arg3,"clear"))

	{

	    free_string( obj->chpoweroff );

	    obj->chpoweroff = str_dup( "(null)" );

	}

	else if (obj->chpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))

	{

	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)

	    {

		send_to_char("Line too long.\n\r",ch);

		return;

	    }

	    else

	    {

	    	free_string( obj->chpoweroff );

	    	strcat( buf, "\n\r" );

	    	strcat( buf, arg3 );

	    	obj->chpoweroff = str_dup( buf );

	    }

	}

	else

	{

	    free_string( obj->chpoweroff );

	    obj->chpoweroff = str_dup( arg3 );

	}

	send_to_char("Ok.\n\r",ch);

    }

    else if (!str_cmp(arg2,"other-in") || !str_cmp(arg2,"other-remove") )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"other-in") && !IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("That item is not a transporter.\n\r",ch);

	    return;

	}

    	if (!str_cmp(arg2,"other-remove") && IS_SET(obj->spectype, SITEM_TELEPORTER))

	{

	    send_to_char("Not on a transporter.\n\r",ch);

	    return;

	}

	if (obj->victpoweroff != NULL) strcpy(buf,obj->victpoweroff);

	else {send_to_char("There is a problem with this item, please inform an implementor.\n\r",ch);return;}

	if (!str_cmp(arg3,"clear"))

	{

	    free_string( obj->victpoweroff );

	    obj->victpoweroff = str_dup( "(null)" );

	}

	else if (obj->victpoweroff != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))

	{

	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)

	    {

		send_to_char("Line too long.\n\r",ch);

		return;

	    }

	    else

	    {

	    	free_string( obj->victpoweroff );

	    	strcat( buf, "\n\r" );

	    	strcat( buf, arg3 );

	    	obj->victpoweroff = str_dup( buf );

	    }

	}

	else

	{

	    free_string( obj->victpoweroff );

	    obj->victpoweroff = str_dup( arg3 );

	}

	send_to_char("Ok.\n\r",ch);

    }

    else if (!str_cmp(arg2,"you-use") )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }
	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	if (obj->chpoweruse != NULL) strcpy(buf,obj->chpoweruse);

	else {send_to_char("There is a problem with this item, please inform an implementor.\n\r",ch);return;}

	if (!str_cmp(arg3,"clear"))

	{

	    free_string( obj->chpoweruse );

	    obj->chpoweruse = str_dup( "(null)" );

	}

	else if (obj->chpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))

	{

	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)

	    {

		send_to_char("Line too long.\n\r",ch);

		return;

	    }

	    else

	    {

	    	free_string( obj->chpoweruse );

	    	strcat( buf, "\n\r" );

	    	strcat( buf, arg3 );

	    	obj->chpoweruse = str_dup( buf );

	    }

	}

	else

	{

	    free_string( obj->chpoweruse );

	    obj->chpoweruse = str_dup( arg3 );

	}

	send_to_char("Ok.\n\r",ch);

    }

    else if (!str_cmp(arg2,"other-use") )

    {

	if (!IS_EXTRA(ch,EXTRA_TRUSTED))

	{

	    send_to_char("You are not permitted to change an object in this way.\n\r",ch);

	    return;

	}

	if ((obj->pIndexData->vnum >= 29600) && (obj->pIndexData->vnum <= 29742))
    {

	send_to_char( "You cannot quest-change that item.(QUEST_RELIC)\n\r", ch );

	return;

    }

	if (obj->item_type == ITEM_BOOK)

	{

	    send_to_char("Not on a book.\n\r",ch);

	    return;

	}

	if (obj->victpoweruse != NULL) strcpy(buf,obj->victpoweruse);

	else {send_to_char("There is a problem with this item, please inform an implementor.\n\r",ch);return;}

	if (!str_cmp(arg3,"clear"))

	{

	    free_string( obj->victpoweruse );

	    obj->victpoweruse = str_dup( "(null)" );

	}

	else if (obj->victpoweruse != NULL && buf[0] != '\0' && str_cmp(buf,"(null)"))

	{

	    if (strlen(buf)+strlen(arg3) >= MAX_STRING_LENGTH-4)

	    {

		send_to_char("Line too long.\n\r",ch);

		return;

	    }

	    else

	    {

	    	free_string( obj->victpoweruse );

	    	strcat( buf, "\n\r" );

	    	strcat( buf, arg3 );

	    	obj->victpoweruse = str_dup( buf );

	    }

	}

	else

	{

	    free_string( obj->victpoweruse );

	    obj->victpoweruse = str_dup( arg3 );

	}

	send_to_char("Ok.\n\r",ch);

    }
    */
    return;

}



void quest_clone( CHAR_DATA *ch, OBJ_DATA *obj )

{

    OBJ_INDEX_DATA *pObjIndex;

    OBJ_DATA *obj2;

    AFFECT_DATA *paf;

    AFFECT_DATA *paf2;



    pObjIndex = get_obj_index( obj->pIndexData->vnum);

    obj2 = create_object( pObjIndex, obj->level );

    /* Copy any changed parts of the object. */

    free_string(obj2->name);

    obj2->name=str_dup(obj->name);

    free_string(obj2->short_descr);

    obj2->short_descr=str_dup(obj->short_descr);

    free_string(obj2->description);

    obj2->description=str_dup(obj->description);

    if (obj2->questmaker != NULL) free_string(obj2->questmaker);

	obj2->questmaker=str_dup(ch->name);

    if (obj->chpoweron != NULL && obj->points >= 50)

    {

    	free_string(obj2->chpoweron);

    	obj2->chpoweron=str_dup(obj->chpoweron);

    }

    if (obj->chpoweroff != NULL && obj->points >= 50)

    {

    	free_string(obj2->chpoweroff);

    	obj2->chpoweroff=str_dup(obj->chpoweroff);

    }

    if (obj->chpoweruse != NULL && obj->points >= 50)

    {

    	free_string(obj2->chpoweruse);

    	obj2->chpoweruse=str_dup(obj->chpoweruse);

    }

    if (obj->victpoweron != NULL && obj->points >= 50)

    {

    	free_string(obj2->victpoweron);

    	obj2->victpoweron=str_dup(obj->victpoweron);

    }

    if (obj->victpoweroff != NULL && obj->points >= 50)

    {

    	free_string(obj2->victpoweroff);

    	obj2->victpoweroff=str_dup(obj->victpoweroff);

    }

    if (obj->victpoweruse != NULL && obj->points >= 50)

    {

    	free_string(obj2->victpoweruse);

    	obj2->victpoweruse=str_dup(obj->victpoweruse);

    }

    if (obj->points >= 50)

    {

	obj2->spectype	 = obj->spectype;

	obj2->specpower	 = obj->specpower;

    }

    obj2->item_type	 = obj->item_type;

    obj2->extra_flags	 = obj->extra_flags;

    obj2->wear_flags	 = obj->wear_flags;

    obj2->weight	 = obj->weight;

    obj2->condition	 = obj->condition;

    obj2->toughness	 = obj->toughness;

    obj2->resistance	 = obj->resistance;

    obj2->quest		 = obj->quest;

    obj2->points	 = obj->points;

    obj2->cost		 = obj->cost;

    obj2->value[0]	 = obj->value[0];

    obj2->value[1]	 = obj->value[1];

    obj2->value[2]	 = obj->value[2];

    obj2->value[3]	 = obj->value[3];

    /*****************************************/

    obj_to_char(obj2,ch);

    if (obj->affected != NULL)

    {

    	for ( paf = obj->affected; paf != NULL; paf = paf->next )

    	{

	    if (affect_free == NULL)

	        paf2 = alloc_perm( sizeof(*paf) );

	    else

	    {

		paf2 = affect_free;

		affect_free = affect_free->next;

	    }

	    paf2->type  	= 0;

	    paf2->duration	= paf->duration;

	    paf2->location	= paf->location;

	    paf2->modifier	= paf->modifier;

	    paf2->bitvector	= 0;

	    paf2->next  	= obj2->affected;

	    obj2->affected	= paf2;

    	}

    }

    act( "$p splits into two identical objects.", ch, obj, NULL, TO_CHAR );

    return;

}



void oset_affect( CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest )
{
    AFFECT_DATA *paf;
    int quest;
    int range;
    int cost;
    char buf[MAX_STRING_LENGTH];
    int max;

    if (IS_NPC(ch))
    {
	send_to_char("Switch back, smart ass.\n\r", ch);
	return;
    } 
    if (value == 0)
    {
	send_to_char("Please enter a positive or negative amount.\n\r", ch);
	return;
    }

    if (!IS_JUDGE(ch) && (obj->questowner == NULL))
    {
	send_to_char("First you must set the owners name on the object.\n\r", ch);
	return;
    }
    /*if (!IS_JUDGE(ch) && (obj->questmaker == NULL ||
	str_cmp(ch->name,obj->questmaker)) && !is_quest )
    {
	send_to_char("That item has already been oset by someone else.\n\r", ch);
	return;
    }*/

    if      ( affect == APPLY_STR     ) {range = 3;  cost = 20 ; quest = QUEST_STR;}
    else if ( affect == APPLY_DEX     ) {range = 3;  cost = 20 ; quest = QUEST_DEX;}
    else if ( affect == APPLY_INT     ) {range = 3;  cost = 20 ; quest = QUEST_INT;}
    else if ( affect == APPLY_WIS     ) {range = 3;  cost = 20 ; quest = QUEST_WIS;}
    else if ( affect == APPLY_CON     ) {range = 3;  cost = 20 ; quest = QUEST_CON;}
    else if ( affect == APPLY_HIT     ) {range = 25; cost =  5 ; quest = QUEST_HIT;}
    else if ( affect == APPLY_MANA    ) {range = 25; cost =  5 ; quest = QUEST_MANA;}
    else if ( affect == APPLY_MOVE    ) {range = 25; cost =  5 ; quest = QUEST_MOVE;}
    else if ( affect == APPLY_HITROLL ) {range = 5;  cost = 30 ; quest = QUEST_HITROLL;}
    else if ( affect == APPLY_DAMROLL ) {range = 5;  cost = 30 ; quest = QUEST_DAMROLL;}
    else if ( affect == APPLY_AC      ) {range = 25; cost = 10 ; quest = QUEST_AC;}
    else return;

    if (IS_SET(obj->quest, QUEST_IMPROVED))
	max = 1250 + (ch->race * 20);
    else if ( obj->pIndexData->vnum == OBJ_VNUM_PROTOPLASM )
	{range *= 2; max = 750 + (ch->race * 10);}
    else
	max = 400 + (ch->race * 10);
    if (obj->item_type == ITEM_WEAPON) {max *= 2;range *= 2;}
   if ( !IS_JUDGE(ch) && !IS_SET(obj->extra_flags, ITEM_FORGED) && !IS_SET(obj->extra_flags,ITEM_LIGHT_SABER) && ((value > 0 
    && value > range) || (value < 0 && value < (range-range-range))) )
    {
	send_to_char("That is not within the acceptable range...\n\r", ch);
	send_to_char("Str, Dex, Int, Wis, Con... max =   3 each, at  20 quest points per +1 stat.\n\r", ch);
	send_to_char("Hp, Mana, Move............ max =  25 each, at   5 quest point per point.\n\r", ch);
	send_to_char("Hitroll, Damroll.......... max =   5 each, at  30 quest points per point.\n\r", ch);
	send_to_char("Ac........................ max = -25,      at  10 points per point.\n\r", ch);
	send_to_char("\n\rNote: Created items can have upto 2 times the above maximum.\n\r", ch);
	send_to_char("Also: Weapons may have upto 2 (4 for created) times the above maximum.\n\r", ch);
	return;
    }

    if ( quest >= QUEST_HIT && value < 0 ) 
      cost *= (value - (value*2));
    else 
      cost *= value;

    if ( cost < 0 ) 
     cost = 0;

    if (!IS_JUDGE(ch) && !IS_SET(obj->extra_flags, ITEM_FORGED) && !IS_SET(obj->extra_flags,ITEM_LIGHT_SABER)
     && IS_SET(obj->quest, quest) )
    {
	send_to_char("That affect has already been set on this object.\n\r", ch);
	return;
    }
    if (!IS_JUDGE(ch) && !IS_SET(obj->extra_flags, ITEM_FORGED) && !IS_SET(obj->extra_flags,ITEM_LIGHT_SABER)
     && (obj->points + cost > max))
    {
	sprintf(buf,"You are limited to %d quest points per item.\n\r",max);
	send_to_char(buf, ch);
	return;
    }

    if (is_quest && !IS_SET(obj->extra_flags, ITEM_FORGED) && (ch->pcdata->quest < cost))
    {
	sprintf(buf,"That costs %d quest points, while you only have %d.\n\r",cost, ch->pcdata->quest);
	send_to_char(buf, ch);
	return;
    }
    if (!IS_SET(obj->quest, quest) ) 
       SET_BIT(obj->quest,quest);
    if( !IS_SET(obj->extra_flags,ITEM_LIGHT_SABER) )
     obj->points += cost;
    if (obj->questmaker != NULL) 
      free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);

    if ( affect_free == NULL )
    {
	paf		= alloc_perm( sizeof(*paf) );
    }
    else
    {
	paf		= affect_free;
	affect_free	= affect_free->next;
    }
    paf->type	= 0;
    paf->duration	= -1;
    paf->location	= affect;
    paf->modifier	= value;
    paf->bitvector	= 0;
    paf->next		= obj->affected;
    obj->affected	= paf;

return;

}


void mob_oset_affect(CHAR_DATA *ch, OBJ_DATA *obj, int value, int affect, bool is_quest)
{

   AFFECT_DATA *paf;
   int quest;
   

   if( value == 0 )
      return;

   if( affect == APPLY_STR )
      quest = QUEST_STR;
   else if( affect == APPLY_DEX )
      quest = QUEST_DEX;
   else if( affect == APPLY_INT )
      quest = QUEST_INT;
   else if( affect == APPLY_CON )
      quest = QUEST_CON;
   else if( affect == APPLY_WIS )
      quest = QUEST_WIS;
   else if( affect == APPLY_HIT )
      quest = QUEST_HIT;
   else if( affect == APPLY_MANA )
      quest = QUEST_MANA;
   else if( affect == APPLY_MOVE )
      quest = QUEST_MOVE;
   else if( affect == APPLY_HITROLL )
      quest = QUEST_HITROLL;
   else if( affect == APPLY_DAMROLL )
      quest = QUEST_DAMROLL;
   else if( affect == APPLY_AC )
      quest = QUEST_AC;
   else return;

   if( !IS_SET(obj->quest, quest) ) 
      SET_BIT( obj->quest, quest);

   if( obj->questmaker != NULL )
      free_string(obj->questmaker);
   obj->questmaker = str_dup(ch->name); 

   if( affect_free == NULL )
      paf = alloc_perm(sizeof(*paf));
   else
   {
      paf = affect_free;
      affect_free = affect_free->next;
   }

   paf->type = 0;
   paf->duration = -1;
   paf->location = affect;
   paf->modifier = value;
   paf->bitvector = 0; 
   paf->next = obj->affected;
   obj->affected = paf;
   SET_BIT(obj->wear_flags, ITEM_TAKE);
return;

}
