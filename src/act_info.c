/**************************************************************************
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
#include<types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

static int max_logged;

int blood_max( CHAR_DATA *ch );
void do_new_who( CHAR_DATA *ch, char *argument);
void save_config();
CLAN_DATA *find_clan_name(char *argument);
char *	const	where_name	[] =
{
    "[Light         ] ",
    "[On Finger     ] ",
    "[On Finger     ] ",
    "[Around Neck   ] ",
    "[Around Neck   ] ",
    "[On Body       ] ",
    "[On Head       ] ",
    "[On Legs       ] ",
    "[On Feet       ] ",
    "[On Hands      ] ",
    "[On Arms       ] ",
    "[Off Hand      ] ",
    "[Around Body   ] ",
    "[Around Waist  ] ",
    "[Around Wrist  ] ",
    "[Around Wrist  ] ",
    "[Right Hand    ] ",
    "[Left Hand     ] ",
    "[Third Hand    ] ",
    "[Fourth Hand   ] ",
    "[On Face       ] ",
    "[Left Scabbard ] ",
    "[Right Scabbard] ",
};

const char * exitname [6] =
{
	"north",
	"east",
	"south",
	"west",
	"up",
	"down"
};

const char * exitname2 [6] =
{
	"to the north",
	"to the east",
	"to the south",
	"to the west",
	"above you",
	"below you"
};



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );
void	askill_gain		args( ( CHAR_DATA *ch, int field ) );
void	evil_eye		args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	check_left_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_left_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	obj_score		args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';
    ADD_COLOUR(ch, buf, D_CYAN);
    if( (obj->carried_by == ch || IS_IMMORTAL(ch)) && (obj->item_type == ITEM_WEAPON || obj->item_type == ITEM_ARMOR) )
     sprintf(buf,"#0(#7%d%%#0)#n ",obj->durability);
    if ( IS_SET(obj->quest, QUEST_ARTIFACT)) strcat(buf, "(Artifact) " );
    else if ( IS_SET(obj->quest, QUEST_RELIC)) strcat(buf, "(Relic) " );
    else if ( obj->points < 750 && !IS_SET(obj->extra_flags, ITEM_FORGED) && obj->points != 0)
         strcat(buf,"(Legendary) " );
    else if ( obj->points < 1250 && !IS_SET(obj->extra_flags, ITEM_FORGED) && obj->points != 0 )
         strcat(buf, "(Mythical) " );
    else if ( obj->points < 3000 && !IS_SET(obj->extra_flags, ITEM_FORGED) && obj->points !=0)
         strcat(buf, "(Priceless) " );
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "(Invis) "     );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "(Blue Aura) "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "(Red Aura) "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "(Yellow Aura) "  );
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "(Magical) "   );
        if (IS_SET(obj->spectype, SITEM_COPPER))  strcat(buf,"(Copper) ");
    if (IS_SET(obj->spectype, SITEM_IRON))  strcat(buf,"(Iron) ");
    if (IS_SET(obj->spectype, SITEM_STEEL))  strcat(buf,"(Steel) ");
    if (IS_SET(obj->spectype, SITEM_ADAMANTITE))  strcat(buf,"(Adamantite) ");
    if (IS_SET(obj->spectype, SITEM_HILT))  strcat(buf,"(Hilted) ");
    if (IS_SET(obj->spectype, SITEM_GEMSTONE)) strcat(buf,"(Gemstoned) ");
/*    if (obj->value[0] == 18000 && obj->item_type == ITEM_WEAPON)
       strcat(buf,"(Sharpened) ");
  */  if ( IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	!IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "(Shadowplane) " );
    if (!IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	 IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "(Normal plane) " );
    if(obj->item_type == ITEM_GEM)
    {
     if( IS_SET(obj->value[0], GEM_UNCUT)) strcat(buf, "(Uncut) ");
     if( IS_SET(obj->value[0], GEM_SIMPLE)) strcat(buf, "(Simple cut) ");
     if( IS_SET(obj->value[0], GEM_COMPLEX)) strcat(buf, "(Complex cut) ");
     if( IS_SET(obj->value[0], GEM_MASTER)) strcat(buf, "(Master cut) ");
     if( IS_SET(obj->value[0], GEM_UNPOLISHED)) strcat(buf, "(Unpolished) ");
     if( IS_SET(obj->value[0], GEM_POLISHED)) strcat(buf, "(Polished) ");
     if( IS_SET(obj->value[0], GEM_CRACKED)) strcat(buf, "(Cracked) ");
     if( IS_SET(obj->value[0], GEM_FLAWED)) strcat(buf, "(Flawed) ");
     if( IS_SET(obj->value[0], GEM_HAZY)) strcat(buf, "(Hazy) ");
     if( IS_SET(obj->value[0], GEM_CLEAR)) strcat(buf, "(Clear) ");
     if( IS_SET(obj->value[0], GEM_FLAWLESS)) strcat(buf, "(Flawless) ");
    }
    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
	if ( obj->condition < 100) strcat(buf, " (Damaged)");
    }
    else
    {
	if ( obj->description != NULL )
	    strcat( buf, obj->description );
    }
    strcat(buf, "#n");

    return buf;
}

/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "#c(%2d) ", prgnShow[iShow] );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "#c     ", ch );
	    }
	}
	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "#n\n\r", ch );
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return;
}

void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
 /* This hunk'o'shit was entirely redone
  * by the one, the only...The Kline.
  */
 char buf[MAX_STRING_LENGTH]; /* Main buffer */
 char hld[MAX_STRING_LENGTH]; /* Holder for additional sprintf's */

 buf[0] = '\0';
 hld[0] = '\0';

 if( !IS_NPC(victim) && victim->pcdata->chobj != NULL )
  return;
 if( !IS_NPC(victim) && victim->pcdata->race->name != NULL && strlen(victim->pcdata->race->name) > 0 )
  sprintf(buf,"[%s] ",up_case(victim->pcdata->race->name));
 else if( !IS_NPC(victim) )
  sprintf(buf,"[None] ");
 if( !IS_NPC(victim) && victim->desc==NULL )
  strcat(buf,"(Link-Dead) ");
 if( IS_AFFECTED(victim,AFF_INVISIBLE) )
  strcat(buf,"(Invis) ");
 if( IS_AFFECTED(victim,AFF_HIDE) )
  strcat(buf,"(Hide) ");
 if( IS_AFFECTED(victim, AFF_CHARM) )
  strcat(buf,"(Charmed) ");
 if( IS_AFFECTED(victim,AFF_PASS_DOOR) )
  strcat(buf,"(Translucent) ");
 if( IS_AFFECTED(victim,AFF_FAERIE_FIRE) )
  strcat(buf,"(Pink Aura) ");
 if( IS_EVIL(victim) && IS_AFFECTED(ch,AFF_DETECT_EVIL) )
  strcat(buf,"(Red Aura) ");
 if( IS_AFFECTED(victim,AFF_SANCTUARY) )
  strcat(buf,"(White Aura) ");
 if( IS_AFFECTED(ch,AFF_ETHEREAL) && !IS_AFFECTED(victim,AFF_ETHEREAL) )
  strcat(buf,"(Normal Plane) ");
 if( !IS_AFFECTED(ch,AFF_ETHEREAL) && IS_AFFECTED(victim,AFF_ETHEREAL) )
  strcat(buf,"(Ethereal) ");
 if( IS_IMMORTAL(victim) )
 {
  if( IS_SET(victim->act,PLR_INCOG) )
   strcat(buf,"(Wiz Incog) ");
  if( IS_SET(victim->act,PLR_WIZINVIS) )
   strcat(buf,"(Wiz Invis) ");
 }
 if( IS_AFFECTED(ch,AFF_SHADOWPLANE) && !IS_AFFECTED(victim,AFF_SHADOWPLANE) )
  strcat(buf,"(Normal Plane) ");
 if( !IS_AFFECTED(ch,AFF_SHADOWPLANE) && IS_AFFECTED(victim,AFF_SHADOWPLANE) )
  strcat(buf,"(Shadowplane) ");
 if( strlen(victim->morph) > 0 )
  strcat(buf,up_case(victim->morph));
 else if( !IS_NPC(victim) )
  strcat(buf,up_case(victim->pcdata->switchname));
 else if( IS_NPC(victim) && !IS_AFFECTED(victim,AFF_FLYING) && victim->fighting == NULL )
  strcat(buf,up_case(victim->long_descr));
 else if( IS_NPC(victim) && (victim->fighting != NULL || IS_AFFECTED(victim,AFF_FLYING)) )
  strcat(buf,up_case(victim->short_descr));
 switch( victim->position )
 {
  case POS_STANDING:
   if( !IS_AFFECTED(victim,AFF_FLYING) )
   {
    if( !IS_NPC(victim) )
    {
     if( victim->stance[0] == STANCE_NORMAL )
      strcat(buf," is here, crouched in a fighting stance.\n\r");
     if( victim->stance[0] == STANCE_VIPER )
      strcat(buf," is here, crouched in a viper fighting stance.\n\r");
     if( victim->stance[0] == STANCE_CRANE )
      strcat(buf," is here, crouched in a crane fighting stance.\n\r");
     if( victim->stance[0] == STANCE_CRAB )
      strcat(buf," is here, crouched in a crab fighting stance.\n\r");
     if( victim->stance[0] == STANCE_MONGOOSE )
      strcat(buf," is here, crouched in a mongoose fighting stance.\n\r");
     if( victim->stance[0] == STANCE_BULL )
      strcat(buf," is here, crouched in a bull fighting stance.\n\r");
     if( victim->stance[0] == STANCE_MANTIS )
      strcat(buf," is here, crouched in a mantis fighting stance.\n\r");
     if( victim->stance[0] == STANCE_DRAGON )
      strcat(buf," is here, crouched in a dragon fighting stance.\n\r");
     if( victim->stance[0] == STANCE_TIGER )
      strcat(buf," is here, crouched in a tiger fighting stance.\n\r");
     if( victim->stance[0] == STANCE_MONKEY )
      strcat(buf," is here, crouched in a monkey fighting stance.\n\r");
     if( victim->stance[0] == STANCE_SWALLOW )
      strcat(buf," is here, crouched in a swallow fighting stance.\n\r");
     if( victim->stance[0] == STANCE_NONE )
      strcat(buf," is here.\n\r");
    }
   }
   else
    strcat(buf," is hovering here.\n\r");
   break;
  case POS_DEAD:
   strcat(buf," is lying dead on the floor.\n\r");
   break;
  case POS_MORTAL:
   strcat(buf," is lying here, mortally wounded.\n\r");
   break;
  case POS_INCAP:
   strcat(buf," is lying here incapacitated.\n\r");
   break;
  case POS_STUNNED:
   strcat(buf," is lying here stunned.\n\r");
   break;
  case POS_SLEEPING:
   strcat(buf," is sleeping here.\n\r");
   break;
  case POS_RESTING:
   strcat(buf," is resting here.\n\r");
   break;
  case POS_SITTING:
   strcat(buf," is sitting here.\n\r");
   break;
  case POS_MEDITATING:
   strcat(buf," is meditating here.\n\r");
   break;
  case POS_FIGHTING:
   strcat(buf," is here, fighting ");
   if( victim->fighting == NULL )
    strcat(buf, "thin air??\n\r");
   else if( victim->fighting == ch )
    strcat(buf,"YOU!\n\r");
   else if( victim->in_room == victim->fighting->in_room )
   {
    strcat(buf,PERS(victim->fighting,ch));
    strcat(buf,".\n\r");
   }
   else
    strcat(buf,"someone who left??\n\r");
    break;
 }
 if( IS_AFFECTED(victim,AFF_FLAMING) )
 {
  if( IS_NPC(victim) )
   sprintf(hld,"...%s is engulfed in blazing flames!\n\r",up_case(victim->short_descr));
  else
   sprintf(hld,"...%s is engulfed in blazing flames!\n\r",(strlen(victim->morph) > 0)?
    up_case(victim->morph):up_case(victim->pcdata->switchname));
  strcat(buf,hld);
 }
 if( IS_EXTRA(victim,TIED_UP) )
 {
  sprintf(hld,"...%s is tied up",(strlen(victim->morph) > 0)?
   up_case(victim->morph):up_case(victim->pcdata->switchname));
  if( IS_EXTRA(victim,GAGGED) )
   strcat(hld," and gagged!\n\r");
  else if( IS_EXTRA(victim,BLINDFOLDED) )
   strcat(hld," and blindfolded!\n\r");
  else if( IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED) )
   strcat(hld,", gagged, and blindfolded!\n\r");
  else
   strcat(hld,"!\n\r");
  strcat(buf,hld);
 }
 if( IS_AFFECTED(victim,AFF_WEBBED) )
 {
  if( IS_NPC(victim) )
   sprintf(hld,"...%s is coated in a sticky web.\n\r",up_case(victim->short_descr));
  else
   sprintf(hld,"...%s is coated in a sticky web.\n\r",(strlen(victim->morph) > 0)?
    up_case(victim->morph):up_case(victim->pcdata->switchname));
  strcat(buf,hld);
 }
 stc(buf,ch);
 if( !IS_SET(ch->act,PLR_BRIEF) )
 {
  if( has_power_level(victim,SCHOOL_PLANT,5) )
   act("...$N is surrounded by a sharp shield of thorns.",ch,NULL,victim,TO_CHAR);
  if( has_power_level(victim,SCHOOL_ORDER,7) )
   act("...$N is surrounded by a crackling shield of lightning.",ch,NULL,victim,TO_CHAR);
  if( has_power_level(victim,SCHOOL_FIRE,6) )
   act("...$N is surrounded by a burning shield of fire.",ch,NULL,victim,TO_CHAR);
  if( has_power_level(victim,SCHOOL_WATER,5) )
   act("...$N is surrounded by a shimmering shield of ice.",ch,NULL,victim,TO_CHAR);
  if( has_power_level(victim,SCHOOL_EARTH,4) )
   act("...$N is surrounded by a bubbling shield of acid.",ch,NULL,victim,TO_CHAR);
  if( has_power_level( victim, SCHOOL_CHAOS, 4) )
   act("...$N is surrounded by a swirling shield of chaos.",ch,NULL,victim,TO_CHAR);
  if( IS_ITEMAFF(victim,ITEMA_REFLECT) )
   act("...$N is surrounded by a flickering shield of darkness.", ch,NULL,victim,TO_CHAR);
 }
 return;
}

void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
    }

    if (!IS_NPC(ch) && IS_HEAD(victim,LOST_HEAD))
    {
	act( "$N is lying here.", ch, NULL, victim, TO_CHAR );
	return;
    }
    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, PERS(victim, ch) );

         if ( percent >= 100 ) strcat( buf, " is in perfect health.\n\r"  );
    else if ( percent >=  90 ) strcat( buf, " is slightly scratched.\n\r" );
    else if ( percent >=  80 ) strcat( buf, " has a few bruises.\n\r"     );
    else if ( percent >=  70 ) strcat( buf, " has some cuts.\n\r"         );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has many nasty wounds.\n\r" );
    else if ( percent >=  40 ) strcat( buf, " is bleeding freely.\n\r"    );
    else if ( percent >=  30 ) strcat( buf, " is covered in blood.\n\r"   );
    else if ( percent >=  20 ) strcat( buf, " is leaking guts.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is almost dead.\n\r"        );
    else                       strcat( buf, " is DYING.\n\r"              );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if (!IS_NPC( victim ))
    {
    	if (IS_AFFECTED(victim, AFF_INFRARED)) act("$N's eyes are glowing bright red.",ch,NULL,victim,TO_CHAR);
    	if (IS_AFFECTED(victim, AFF_FLYING)) act("$N is hovering in the air.",ch,NULL,victim,TO_CHAR);
    }
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj != obj)
	    {
	    	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    	send_to_char( "\n\r", ch );
	    }
	    else
		send_to_char( "you\n\r", ch);
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   !IS_HEAD(victim, LOST_HEAD)
    &&   number_percent( ) < ch->pcdata->learned[gsn_peek] )
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( IS_IMMORTAL(ch) || IS_IMMORTAL(rch) )
	{
	    if ( !IS_NPC(rch)
	    &&   IS_SET(rch->act, PLR_WIZINVIS)
	    &&   get_trust( ch ) < get_trust( rch ) )
		continue;
	}
	else
	{
	    if ( !IS_NPC(rch)
	    && ( IS_SET(rch->act, PLR_WIZINVIS)
	    ||   IS_ITEMAFF(rch, ITEMA_VANISH) )
	    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
	    &&   !IS_ITEMAFF(ch, ITEMA_VISION) )
		continue;
	}

	if ( !IS_NPC(rch) && IS_HEAD(rch,LOST_HEAD))
	    continue;

	if ( !IS_NPC(rch) && IS_EXTRA(rch,EXTRA_OSWITCH))
	    continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room )
	&& ( IS_AFFECTED(rch, AFF_INFRARED)))
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
    }

    return;
}



bool check_blind( CHAR_DATA *ch )
{

    if ( IS_EXTRA(ch, BLINDFOLDED) )
    {
        send_to_char( "You can't see a thing through the blindfold!\n\r", ch );
        return FALSE;
    }
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
    {
	send_to_char( "You have no eyes!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, PLR_SHADOWSIGHT))
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    return TRUE;
}

void do_set_deca( CHAR_DATA *ch, char *argument )
{
 stc("If you wish to set your decap you must spell out SETDECAP in full.\n\r",ch);
 return;
}

void do_set_decap( CHAR_DATA *ch, char *argument )
{
    if( IS_NPC(ch))
    return;
    if( ch->pcdata->quest < 100)
    {
     send_to_char("You need at least 100 qps.\n\r", ch);
     return;
    }
    if( argument[0] == '\0')
    {
     send_to_char("Set it to what?\n\r", ch);
     return;
    }
    if( !str_cmp(argument,"off") )
    {
     stc("Custom decap message removed.  Decap reset to normal.\n\r",ch);
     if( IS_SET( ch->newbits, NEW_SETDECAP))
      REMOVE_BIT( ch->newbits, NEW_SETDECAP);
     return;
    }
    if( IS_SET( ch->newbits, NEW_SETDECAP))
     REMOVE_BIT( ch->newbits, NEW_SETDECAP);
    smash_tilde(argument);
    //smash_color( argument);
    free_string( ch->pcdata->decap_mess);
    ch->pcdata->decap_mess = str_dup( argument);
    SET_BIT(ch->newbits, NEW_SETDECAP);
    send_to_char("Done.\n\r", ch);
    ch->pcdata->quest -= 100;
    return;
}

void do_setlog( CHAR_DATA *ch, char *argument )
{

        if (IS_NPC(ch))
        return;

        if (argument[0] == '\0')
        {
        send_to_char("Set your login to what?\n\r",ch);
	return;
         }

        if (ch->pcdata->quest < 100 )
        {
        send_to_char("You need 100 quest points to set your login.\n\r",ch);
        return;
        }


        smash_tilde( argument );
         //smash_color( argument);
       free_string( ch->setlog );
        ch->setlog = str_dup( argument );
        SET_BIT(ch->newbits, NEW_SETLOG);
        send_to_char("Ok.\n\r",ch);
        ch->pcdata->quest -= 100;
}

void do_setexit( CHAR_DATA *ch, char *argument )
{

        if (IS_NPC(ch))
        return;

        if (argument[0] == '\0')
        {
        send_to_char("Set your exit message to what?\n\r",ch);
        return;
        }

        if (!IS_SET(ch->newbits, NEW_SETLOG))
        {
        send_to_char("You need to set your login first.\n\r",ch);
        return;
	return;
        }

        smash_tilde( argument );
        //smash_color( argument );
        free_string( ch->setexit );
        ch->setexit = str_dup( argument );
        SET_BIT(ch->newbits, NEW_SETEXIT);
        send_to_char("Ok.\n\r",ch);
}

void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char *pdesc;
    int door;
    bool found;
    int checkdoor = 0;
    bool aff_found = FALSE;

    if ( ch->desc == NULL && (wizard = ch->wizard) == NULL) return;

    if (ch->in_room == NULL) return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch) && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) &&
	!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_IMMORTAL(ch) &&
       ( !IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,DISC_AUSPEX) != 10 ) && !IS_CLASS(ch,CLASS_DROW) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	return;
    }

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_ITEMAFF(ch, ITEMA_VISION)
    &&   !IS_AFFECTED(ch, AFF_INFRARED)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE)
    &&   !(ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
    &&   !IS_NPC(ch) && ch->pcdata->chobj != NULL
    &&   ch->pcdata->chobj->in_obj != NULL)
    &&   ch->race < 15
    &&   room_is_dark( ch->in_room ) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_ITEMAFF(ch, ITEMA_VISION)
    &&   IS_AFFECTED(ch, AFF_ETHEREAL)
    &&   !(ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
    &&   !IS_NPC(ch) && ch->pcdata->chobj != NULL
    &&   ch->pcdata->chobj->in_obj != NULL))
    {
        send_to_char( "Everything is translucent... \n\r", ch );
        show_char_to_char( ch->in_room->people, ch );
        return;
    }
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	    act( "$p",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
    	else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "The shadow plane\n\r", ch );
        else if ( IS_AFFECTED(ch, AFF_ETHEREAL) )
            send_to_char( "The ethereal plane\n\r", ch);
	else if( (ch->in_room != NULL) && (ch->in_room->area != NULL))
	{
            buf[0] = '\0';
	    if( IS_MXP(ch) )
	     stc("\e[1z<RName>",ch);
            sprintf(buf,ch->in_room->name);
	    stc(buf,ch);
	    if( IS_MXP(ch) )
	     stc("</RName>",ch);
	    stc("\n\r", ch);
            buf[0] = '\0';
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DEFILE))
            {
             sprintf(buf+strlen(buf), "#2(Defiled)#n ");
             aff_found = TRUE;
            }
            if( IS_SET(ch->in_room->area->affected_by, AFF_AREA_FLOOD))
            {
             sprintf(buf+strlen(buf), "#5{Flooded}#n ");
             aff_found = TRUE;
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_MINED))
            {
             sprintf(buf+strlen(buf),"#3(Mined)#n ");
             aff_found = TRUE;
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_WHIRLWIND))
            {
             sprintf(buf+strlen(buf), "#4(Whirlwind)#n ");
             aff_found = TRUE;
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_FIRETRAP))
            {
             sprintf(buf+strlen(buf), "#1(Firetrap)#n ");
             aff_found = TRUE;
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_ICED))
            {
             sprintf(buf+strlen(buf), "#6(Iced)#n ");
             aff_found = TRUE;
            }
	    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_TEARGAS) )
	    {
	     sprintf(buf+strlen(buf), "#g(#2Teargas#g)#n ");
	     aff_found = TRUE;
	    }
	    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DART) )
            {
	     if( ch->in_room->trapped_by != NULL )
             {
	      if( !str_cmp(ch->in_room->trapped_by,ch->name) )
              {
               sprintf(buf+strlen(buf), "#7(#0Dart Trap#7)#n ");
               aff_found = TRUE;
              }
	     }
             else if( number_percent() < 20 )
             {
              sprintf(buf+strlen(buf), "#7(#0Dart Trap#7)#n ");
              aff_found = TRUE;
             }
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_POISON) )
            {
             if( ch->in_room->trapped_by != NULL )
	     {
              if( !str_cmp(ch->in_room->trapped_by,ch->name) )
              {
               sprintf(buf+strlen(buf), "#7(#0Poison Trap#7)#n ");
               aff_found = TRUE;
              }
	     }
             else if( number_percent() < 20 )
             {
              sprintf(buf+strlen(buf), "#7(#0Poison Trap#7)#n ");
              aff_found = TRUE;
             }
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_EXPLOSION) )
            {
             if( ch->in_room->trapped_by != NULL )
	     {
              if( !str_cmp(ch->in_room->trapped_by,ch->name) )
              {
               sprintf(buf+strlen(buf), "#7(#0Explosion Trap#7)#n ");
               aff_found = TRUE;
              }
	     }
             else if( number_percent() < 20 )
             {
              sprintf(buf+strlen(buf), "#7(#0Explosion Trap#7)#n ");
              aff_found = TRUE;
             }
            }
            if( IS_SET(ch->in_room->affected_by, AFF_ROOM_SLEEP) )
            {
             if( ch->in_room->trapped_by != NULL )
	     {
              if( !str_cmp(ch->in_room->trapped_by,ch->name) )
              {
               sprintf(buf+strlen(buf), "#7(#0Sleep Trap#7)#n ");
               aff_found = TRUE;
              }
	     }
             else if( number_percent() < 20 )
             {
              sprintf(buf+strlen(buf), "#7(#0Sleep Trap#7)#n ");
              aff_found = TRUE;
             }
            }
	    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_NEGATIVITY) )
	    {
	     sprintf(buf+strlen(buf), "#7(#3Negativity Rune#7)#n ");
	     aff_found = TRUE;
	    }
	    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DRAIN) )
	    {
	     sprintf(buf+strlen(buf), "#7(#1Drain Rune#7)#n ");
	     aff_found = TRUE;
	    }
            if( IS_SET(ch->in_room->affected_by,ROOM_TOTAL_DARKNESS) )
            {
             sprintf(buf+strlen(buf),"#7(#0Darkness#7)#n ");
             aff_found = TRUE;
            }
            if(aff_found)
            {
             sprintf(buf+strlen(buf), "\n\r");
	     send_to_char( buf, ch );
            }
	}

	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
	    do_exits( ch, "auto" );

	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	{
	    act( "You are inside $p.",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
	    show_list_to_char( ch->pcdata->chobj->in_obj->contains, ch, FALSE, FALSE );
	}
	else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "You are standing in complete darkness.\n\r", ch );
        else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_ETHEREAL) )
            send_to_char( "You are standing in a translucent nothingness.\n\r", ch );

	else if ( ( !IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF) ) &&
		( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) )
	{
	    if( IS_MXP(ch) )
	     stc("\e[1z<RDesc>",ch);
	    send_to_char( ch->in_room->description, ch );
	    if( IS_MXP(ch) )
	     stc("</RDesc>",ch);
	    if (ch->in_room->blood == 1000)
		sprintf(buf,"You notice that the room is completely drenched in blood.\n\r");
	    else if (ch->in_room->blood > 750)
		sprintf(buf,"You notice that there is a very large amount of blood around the room.\n\r");
	    else if (ch->in_room->blood > 500)
		sprintf(buf,"You notice that there is a large quantity of blood around the room.\n\r");
	    else if (ch->in_room->blood > 250)
		sprintf(buf,"You notice a fair amount of blood on the floor.\n\r");
	    else if (ch->in_room->blood > 100)
		sprintf(buf,"You notice several blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 50)
		sprintf(buf,"You notice a few blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 25)
		sprintf(buf,"You notice a couple of blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 0)
		sprintf(buf,"You notice a few drops of blood on the floor.\n\r");
	    else sprintf(buf,"You notice nothing special in the room.\n\r");
	    ADD_COLOUR(ch, buf, L_RED);
	    if (ch->in_room->blood > 0) send_to_char(buf,ch);
	}
            if (IS_SET(ch->in_room->room_flags, ROOM_FLAMING))
            send_to_char("..This room is engulfed in flames!\n\r",ch);

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );

        for (door=0 ; door < 6 ; door++)
	{
	    if (ch->in_room == NULL) continue;
	    if (ch->in_room->exit[door] == NULL) continue;

	    if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ICE_WALL))
	    {
		sprintf(buf, "     You see a glacier of ice %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_CALTROP_WALL))
	    {
		sprintf(buf, "     You see a wall of caltrops %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_FIRE_WALL))
	    {
		sprintf(buf, "     You see a blazing wall of fire %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_SWORD_WALL))
	    {
		sprintf(buf, "     You see a spinning wall of swords %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_MUSHROOM_WALL))
	    {
		sprintf(buf, "     You see a vibrating mound of mushrooms %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info,EX_PRISMATIC_WALL))
	    {
		sprintf(buf, "     You see a shimmering wall of many colours %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_IRON_WALL))
	    {
		sprintf(buf, "    You see a solid wall of iron %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ASH_WALL))
	    {
		sprintf(buf, "    You see a deadly wall of ash %s.\n\r", exitname2[door]);
	        send_to_char(buf, ch);
	    }
	    else if (IS_SET(ch->in_room->exit[door]->exit_info, EX_ENERGYNET_WALL))
	    {
		sprintf(buf, "    You see a barely visible enery web %s.\n\r", exitname2[door]);
		send_to_char(buf, ch);
	    }
	}
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "It doesn't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    if (obj->value[2] == 1 || obj->value[2] == 3)
	    {
		send_to_char( "It seems to be closed.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in it.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "It contains a small amount of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "It's about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "It's about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "It is almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "It's completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow it is MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p contains:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "e" ) ||
	!str_cmp( arg1, "s" ) || !str_cmp( arg1, "w" ) ||
	!str_cmp( arg1, "u" ) || !str_cmp( arg1, "d" ) )
	    checkdoor = 1;

  if (checkdoor != 1) {
    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	return;
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !str_cmp(arg1,vch->morph))
	    {
		show_char_to_char_1( vch, ch );
		return;
	    }
	    continue;
	}
    }

    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
    {
	obj = get_obj_in_obj(ch,arg1);
	if (obj != NULL)
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
	return;
    }
  }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if( is_ds_range(ch) )
    {
	stc("You cannot see that far into your subconscious.\n\r", ch);
	return;
    }
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );
*/

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    if( IS_SET(ch->in_room->room_flags, ROOM_DARK ))
	    {
		stc("\n\rYour vision cannot penetrate the darkness "
		    "shrouding that room.\n\r", ch);
		char_from_room(ch);
		char_to_room(ch,location);
		return;
	    }
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
	else
	{
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    if (IS_SET(ch->in_room->room_flags, ROOM_DARK))
	    {
	 	send_to_char("\n\rYour Vision cannot penetrate the"
			     "darkness shrouding that room.\n\r",ch);
	    	char_from_room(ch);
		char_to_room(ch,location);
		return;
	    }
	    else
                do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
    }
    else
    {
	if ((pexit = ch->in_room->exit[door]) == NULL) return;
	if ((pRoomIndex = pexit->to_room) == NULL) return;
	location = ch->in_room;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
	char_from_room(ch);
	char_to_room(ch,location);
    }

    return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	     if (obj->condition >= 100)
	    sprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
	else if (obj->condition >= 75 )
	    sprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
	else if (obj->condition >= 50 )
	    sprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
	else if (obj->condition >= 25 )
	    sprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
	else
	    sprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
	send_to_char(buf,ch);
	switch ( obj->item_type )
	{
	default:
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    sprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;


    if( fAuto )
    {
     if( IS_MXP(ch) )
      sprintf(buf,"\e[1z<RExits>[Exits:");
     else
      sprintf(buf,"[Exits:");
    }
    else 
     sprintf(buf,"Obvious exits:\n\r");


    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
                if( IS_MXP(ch) )
                 strcat(buf,"<Ex>");
		strcat( buf, dir_name[door] );
                if( IS_MXP(ch) )
                 strcat(buf,"</Ex>");
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s\n\r",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->to_room )
			?  "Too dark to tell"
			: pexit->to_room->name
		    );
	    }
	}
    }
    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );
    if ( fAuto )
    {
     if( IS_MXP(ch) )
      strcat(buf,"]</RExits>\n\r");
     else
     {
      strcat( buf, "]\n\r" );
      ADD_COLOUR(ch, buf, D_CYAN);
     }
    }
    send_to_char( buf, ch );
    return;
}

void do_weaplist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    if (IS_NPC(ch)) return;


send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char( "                 Weapon, Stance, Quest and Spell Skill           \n\r",ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char( "                            Weapon\n\r",ch);
sprintf( buf, "Unarm: %3d Slice: %3d Stab: %3d Slash: %3d Whip  : %3d Claw: %3d Blast: %3d\n\r", ch->wpn[0],ch->wpn[1],ch->wpn[2],ch->wpn[3],ch->wpn[4],ch->wpn[5],ch->wpn[6]);
send_to_char(buf,ch);
sprintf( buf, "Pound: %3d Crush: %3d Grep: %3d  Bite: %3d Pierce: %3d Suck: %3d\n\r",ch->wpn[7],ch->wpn[8],ch->wpn[9],ch->wpn[10], ch->wpn[11],ch->wpn[12]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char( "                            Stances\n\r",ch);
sprintf( buf, "Viper   : %3d Crane   : %3d Crab    : %3d Mongoose: %3d Bull     : %3d\n\r",ch->stance[1],ch->stance[2],ch->stance[3],ch->stance[4],ch->stance[5]);
send_to_char(buf,ch);
sprintf( buf,"Mantis  : %3d Dragon  : %3d Tiger   : %3d Monkey  : %3d Swallow  : %3d\n\r",ch->stance[6], ch->stance[7],ch->stance[8],ch->stance[9],ch->stance[10]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);
send_to_char( "                            Magic\n\r",ch);
sprintf( buf, "Purple: %3d  Red: %3d  Blue: %3d  Green: %3d  Yellow: %3d\n\r",ch->spl[0],ch->spl[1],ch->spl[2],ch->spl[3],ch->spl[4]);
send_to_char(buf,ch);
send_to_char(

"------------------------------------------------------------------------------\n\r",ch);


return;
}

void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int a_c = class_ac(ch);

	if (IS_NPC(ch)) return;

    if (!IS_NPC(ch) && (IS_EXTRA(ch,EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD)))
	{
		obj_score(ch,ch->pcdata->chobj);
		return;
	}
    sprintf( buf,"You are %s%s.  You have been playing for %d hours.\n\r",ch->pcdata->switchname,IS_NPC(ch) ? "" : ch->pcdata->title,	(get_age(ch) - 17) * 2 );
    send_to_char( buf, ch );
    if (!IS_NPC(ch))
		birth_date(ch, TRUE);
    sprintf( buf,
	"You have %d/%d hit, %d/%d mana, %d/%d movement, %d primal energy.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->practice );
    send_to_char( buf, ch );

    sprintf( buf,
	"You are carrying %d/%d items with weight %d/%d kg.\n\r",
	ch->carry_number, can_carry_n(ch),
	ch->carry_weight, can_carry_w(ch) );
    send_to_char( buf, ch );

    sprintf( buf,
	"Str: %d  Int: %d  Wis: %d  Dex: %d  Con: %d.\n\r",
	get_curr_str(ch),
	get_curr_int(ch),
	get_curr_wis(ch),
	get_curr_dex(ch),
	get_curr_con(ch) );
    send_to_char( buf, ch );

    if( get_hours_played(ch) <= 4 )
     stc("Your experience is increased as a new player!\n\r", ch);

    sprintf( buf,
	"You have scored %d experience.\n\r",
	ch->exp);
    send_to_char( buf, ch );

	if (IS_IMMORTAL(ch) && (IS_SET(ch->act, PLR_WIZINVIS)) )
		send_to_char("You are Wiz Invis\n\r", ch);
	if (IS_IMMORTAL(ch) && (IS_SET(ch->act, PLR_INCOG)) )
		send_to_char("You are Wiz Incog\n\r", ch);
    sprintf( buf,
	"Autoexit: %s.  Autoloot: %s.  Autosac: %s.\n\r",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOLOOT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOSAC) ) ? "yes" : "no" );
    send_to_char( buf, ch );

    sprintf( buf, "Wimpy set to %d hit points.\n\r", ch->wimpy );
    if ( ch->pcdata->condition[COND_DRUNK]   > 10 )
		send_to_char( "You are drunk.\n\r",   ch );
    if (ch->level < 3 && ch->pcdata->condition[COND_THIRST] ==  0 )
		send_to_char( "You are thirsty.\n\r", ch );
    if (ch->level < 3 && ch->pcdata->condition[COND_FULL]   ==  0 )
		send_to_char( "You are hungry.\n\r",  ch );

    else switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",		ch );
	break;
    case POS_MEDITATING:
	send_to_char( "You are meditating.\n\r",	ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",		ch );
	break;
    }

    if ( ch->level >= 0 )
    {
	sprintf( buf, "AC: %d.  ", a_c );
	send_to_char( buf, ch );
    }

    send_to_char( "You are ", ch );
         if ( a_c >=  101 ) send_to_char( "naked!\n\r", ch );
    else if ( a_c >=   80 ) send_to_char( "barely clothed.\n\r",   ch );
    else if ( a_c >=   60 ) send_to_char( "wearing clothes.\n\r",  ch );
    else if ( a_c >=   40 ) send_to_char( "slightly armored.\n\r", ch );
    else if ( a_c >=   20 ) send_to_char( "somewhat armored.\n\r", ch );
    else if ( a_c >=    0 ) send_to_char( "armored.\n\r",          ch );
    else if ( a_c >=  -50 ) send_to_char( "well armored.\n\r",     ch );
    else if ( a_c >= -100 ) send_to_char( "strongly armored.\n\r", ch );
    else if ( a_c >= -250 ) send_to_char( "heavily armored.\n\r",  ch );
    else if ( a_c >= -500 ) send_to_char( "superbly armored.\n\r", ch );
    else if ( a_c >= -749 ) send_to_char( "divinely armored.\n\r", ch );
    else                    send_to_char( "ultimately armored!\n\r",ch );

	if (!IS_NPC(ch))
	{
		sprintf( buf, "Hitroll: %d.  Damroll: %d. ", class_hitroll(ch),class_damroll(ch));
		send_to_char( buf, ch );

	}
    else if (ch->level >= 0)
	send_to_char( "\n\r", ch );

    if ( ch->level >= 0 )
    {
	sprintf( buf, "Alignment: %d.  ", ch->alignment );
	send_to_char( buf, ch );
    }

    send_to_char( "You are ", ch );
         if ( ch->alignment >  900 ) send_to_char( "angelic.\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_char( "saintly.\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_char( "good.\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "kind.\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "neutral.\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_char( "mean.\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "evil.\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "demonic.\n\r", ch );
    else                             send_to_char( "satanic.\n\r", ch );

    if ( !IS_NPC(ch) && ch->level >= 0 )
    {
	sprintf( buf, "Status: %d.  ", ch->race );
	send_to_char( buf, ch );
    }


    if (!IS_NPC(ch)) send_to_char( "You are ", ch );

         if (!IS_NPC(ch) && ch->level == 1 ) send_to_char( "a Newbie.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 2 && ch->max_hit < 3000) send_to_char( "a Mortal.\n\r", ch);
	else if (!IS_NPC(ch) && ch->level == 2 && ch->max_hit < 8000) send_to_char( "a Mortal with public channel access.\n\r", ch);
	else if (!IS_NPC(ch) && ch->level == 2 ) send_to_char( "a Mortal who may train avatar.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 7 ) send_to_char( "a Builder.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 8 ) send_to_char( "a Quest Maker.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 9 ) send_to_char( "an Enforcer.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 10) send_to_char( "a Judge.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 11) send_to_char( "a High Judge.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 12) send_to_char( "an Implementor.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 0 ) send_to_char( "an Avatar.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 4 ) send_to_char( "Immortal.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 9 ) send_to_char( "Godling.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 14) send_to_char( "Demigod.\n\r",ch);
    else if (!IS_NPC(ch) && ch->race <= 19) send_to_char( "Lesser God.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 24) send_to_char( "Greater God.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 29) send_to_char( "Supreme God.\n\r", ch);
	else if (!IS_NPC(ch) && ch->race <= 34) send_to_char( "Ultimate God.\n\r", ch);
	else if (!IS_NPC(ch) && ch->race > 34)  send_to_char( "Master.\n\r", ch);
    else if (!IS_NPC(ch)) send_to_char( "a Bugged Character!\n\r", ch);
    sprintf(buf, "Rage: %d  Stasis: %d pulses  Shadowplane: %d pulses\n\r", ch->rage, ch->pcdata->pulse_timer[PULSE_STASIS],
ch->pcdata->pulse_timer[PULSE_SHADOWPLANE]);
    stc(buf,ch);
    if ( !IS_NPC(ch) )
    {
		/*if (ch->pkill  == 0)
			sprintf(ss1,"no players");
		else if (ch->pkill  == 1)
			sprintf(ss1,"%d player",ch->pkill);
		else
			sprintf(ss1,"%d players",ch->pkill);
		if
			(ch->pdeath == 0) sprintf(ss2,"no players");
		else if (ch->pdeath == 1)
			sprintf(ss2,"%d player",ch->pdeath);
		else
			sprintf(ss2,"%d players",ch->pdeath);*/
		sprintf(buf,"[Player] Kills : %-6d  Deaths : %-6d\n\r", ch->pkill, ch->pdeath );
		send_to_char( buf, ch );

		sprintf(buf,"[Arena]  Wins  : %-6d  Losses : %-6d\n\r", ch->pcdata->awins,
ch->pcdata->alosses );
		send_to_char( buf, ch);

		sprintf(buf,"[Mobile] Kills : %-6d  Deaths : %-6d\n\r", ch->mkill, ch->mdeath );
		send_to_char( buf, ch );

		if(ch->challenger != NULL)
		{
		  sprintf(buf,"[Arena] You have been challenged by %s.\n\r",
		  ch->challenger->name);
		  send_to_char( buf, ch);
		}
		if(ch->challenged != NULL)
		{
		  sprintf(buf,"[Arena] You have challenged %s.\n\r",
		  ch->challenged->name);
		  send_to_char( buf, ch);
		}
    }

    if ( !IS_NPC(ch) && ch->pcdata->quest > 0)
    {
	if (ch->pcdata->quest == 1)
	    sprintf( buf, "You have a single quest point.\n\r" );
	else
	    sprintf( buf, "You have %d quest points.\n\r", ch->pcdata->quest );
	send_to_char( buf, ch );
    }

    if( !IS_NPC(ch) )
    {
     sprintf( buf, "You have %d ability points.\n\r", ch->pcdata->apoints );
     stc(buf,ch);
    }

    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "You are keeping yourself hidden from those around you.\n\r", ch );

	if (ch->fight_timer > 0)
        {
	sprintf(buf,"You have %d rounds left on your fight timer.\n\r", ch->fight_timer);
	send_to_char(buf,ch);
	}
        if( ch->login_timer > 0)
        {
        sprintf( buf, "You have %d rounds left on your login timer.\n\r", ch->login_timer);
        send_to_char(buf, ch);
        }
    if( IS_CLASS(ch,CLASS_VAMPIRE) )
    {
     sprintf(buf,"Blood: %d[%d]\n\r",GET_POWERS(ch,VAMP_BLOOD),blood_max(ch));
     stc(buf,ch);
     sprintf(buf,"Beast: %d\n\r",GET_POWERS(ch,VAMP_BEAST));
     stc(buf,ch);
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) )
      stc("You have attained Golcanda!\n\r",ch);
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BEAST) )
      stc("Your beast has taken complete control!\n\r",ch);
     if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
      stc("You have mutated into a Dark Golcanda!\n\r",ch);
    }
    if ( ch->affected != NULL )
    {
	send_to_char( "You are affected by:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    sprintf( buf, "Spell: '%s'", skill_table[paf->type].name );
	    send_to_char( buf, ch );

	    if ( ch->level >= 0 )
	    {  if(paf->level != 1)
               {
		sprintf( buf,
		    " modifies %s by %d for %d hours with bits %s.\n\r",
		    affect_loc_name( paf->location ),
		    paf->modifier,
		    paf->duration,
		    affect_bit_name( paf->bitvector ) );
               }
               else
               {
                sprintf( buf,
                " modifies %s by %d for %d hours with bits %s.\n\r",
                 affect_loc_name(paf->location),
                 paf->modifier,
                 paf->duration,
                 affect_bit_name2(paf->bitvector));
               }
		send_to_char( buf, ch );
	    }
	}
    }
    return;
}



char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\rGod Wars started up at %s\rThe system time is %s\r",

	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    sprintf( buf, "The sky is %s and %s.\n\r",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);
    send_to_char( buf, ch );
    return;
}


void do_bounty_list( CHAR_DATA *ch, char *argument)
{
   DESCRIPTOR_DATA *d;
   char buf[MAX_STRING_LENGTH];
   bool found = FALSE;
   send_to_char("Characters currently with a price on their heads:\n\r", ch);
   send_to_char("-------------------------------------------------\n\r", ch);
   send_to_char("\n\r", ch);
   for ( d = descriptor_list; d != NULL; d = d->next )
   {
     if( (d->character != NULL) && (has_bounty(d->character)))
      {
       found = TRUE;
       sprintf( buf, "%s is wanted for a bounty of %d.\n\r", d->character->name,
               d->character->pcdata->bounty[1]);
       send_to_char(buf, ch);
      }
    }
    if( found == FALSE)
     send_to_char("There are no open bounty's.\n\r", ch);
    send_to_char("\n\r", ch);
  return;
}

#define MSL MAX_STRING_LENGTH

void do_who2(CHAR_DATA *ch, char *argument)
{
 char buffer[MSL];
 char imp[MSL];
 char hj[MSL];
 char judge[MSL];
 char enforcer[MSL];
 char qm[MSL];
 char builder[MSL];
 char entity[MSL];
 char ult[MSL];
 char sup[MSL];
 char ggd[MSL];
 char lg[MSL];
 char dmg[MSL];
 char gdl[MSL];
 char imm[MSL];
 char ava[MSL];
 char mor[MSL];
 char name[MAX_STRING_LENGTH];
 char clan[MAX_STRING_LENGTH];
 char status[MAX_STRING_LENGTH];
 char title[MAX_STRING_LENGTH];
 char gen[MAX_STRING_LENGTH];
 char soul[MAX_STRING_LENGTH];
 char stat[MSL];
 char rank[MSL];
 DESCRIPTOR_DATA *d;
 CLAN_DATA *pclan;
 sh_int count=0;
 sh_int blah;
 sh_int holder;
 int exp = 0;
 int i = 0;
 bool morfound = FALSE;
 bool avafound = FALSE;
 bool immfound = FALSE;

 buffer[0] = '\0';
 imp[0] = '\0';
 hj[0] = '\0';
 judge[0] ='\0';
 enforcer[0] = '\0';
 qm[0] = '\0';
 builder[0] = '\0';
 entity[0] = '\0';
 ult[0] = '\0';
 sup[0] = '\0';
 ggd[0] = '\0';
 lg[0] = '\0';
 dmg[0] = '\0';
 gdl[0] = '\0';
 imm[0] = '\0';
 ava[0] = '\0';
 mor[0] = '\0';

 exp += (int) ((exp_who - 1) * 100);
 exp += ((global_exp_mult - 1) * 100);
 stc("#7*-------------------------------------#6[#1   Vortex  #6 ]#7------------------------------------*#n\n\r", ch);
 stc("#7|                                                                                       |#n\n\r",
ch);
 stc("#7|  #3Status     Clan Rank               G    S    U     Player    Title #7                  |#n\n\r", ch);

stc("#7|---------------------------------------------------#4+#7-----------------------------------|#n\n\r",
ch);
 for( d = descriptor_list ; d != NULL; d = d->next)
 {
  if( !d->character || (d->character->name == NULL) ||
      (d->connected != CON_PLAYING))
   continue;
  if( !d->character->pcdata)
   continue;
  if( !can_see(ch,d->character) && (d->character->trust > 6))
   continue;
  sprintf( name, d->character->name);
  if((d->character->clan == NULL) || strlen(d->character->clan) < 5)
   sprintf( clan, "None");
  else
   sprintf( clan, d->character->clan);
  sprintf(gen, "%d", d->character->generation);
  if( !IS_NPC(d->character))
   sprintf(soul, "%d", d->character->pcdata->souls);
  else
   sprintf(soul, "0");

  blah = d->character->race;

  if( (pclan = find_clan_name(clan)) != NULL)
   sprintf(rank, pclan->rank_name[d->character->clan_rank]);
  else
   sprintf( rank, "None");

  sprintf(stat, "%d", blah);
  if( d->character->level <= 2)
  {
    if( d->character->mkill < 5)
    {
     sprintf(status, "#3New#n");
     holder = 1;
    }
    else if( (d->character->mkill >= 5) && (d->character->max_hit < 8000))
    {
     sprintf( status, "#2Mo+#n");
     holder = 2;
    }
    else
    {
     sprintf( status, "#1M++#n");
     holder = 3;
    }
   morfound = TRUE;
  }
  else if( blah == 0)
  {
   sprintf( status, "#1Ava#n");
   avafound = TRUE;
   holder = 4;
  }
  else if( (blah >= 1) && (blah < 5))
  {
   sprintf( status, "#2Imm#n");
   avafound = TRUE;
   holder = 5;
  }
  else if( (blah >= 5) && (blah < 10))
  {
   sprintf( status, "#3Gdl#n");
   avafound = TRUE;
   holder = 6;
  }
  else if( (blah >= 10) && (blah < 15))
  {
   sprintf( status, "#4Dem#n");
   avafound = TRUE;
   holder = 7;
  }
  else if( (blah >= 15) && (blah < 20))
  {
   sprintf( status, "#5LGd#n");
   avafound = TRUE;
   holder = 8;
  }
  else if( (blah >= 20) && (blah < 25))
  {
   sprintf( status, "#6GGd#n");
   avafound = TRUE;
   holder = 9;
  }
  else if( (blah >= 25) && (blah < 30))
  {
   sprintf( status, "#7Sup#n");
   avafound = TRUE;
   holder = 10;
  }
  else if( (blah >= 30) && (blah < 35))
  {
   sprintf( status, "#1Ult#n");
   avafound = TRUE;
   holder = 11;
  }
  else if( blah >= 40)
  {
   sprintf( status, "#0Ent#n");
   avafound = TRUE;
   holder = 12;
  }
  if( d->character->level == 7)
  {
   sprintf( status, "#1Bui#n");
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 13;
  }
  else if( d->character->level == 8)
  {
   sprintf( status, "#2QM#n ");
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 14;
  }
  else if( d->character->level == 9)
  {
   sprintf( status, "#4Enf#n");
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 15;
  }
  else if( d->character->level == 10)
  {
   sprintf( status, "#5Jud#n");
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 16;
  }
  else if( d->character->level == 11)
  {
   sprintf( status, "#6HiJ#n");
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 17;
  }
  else if( d->character->level == 12)
  {
   sprintf( status, "#7Imp#n");
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 18;
  }
  if( !IS_NPC(d->character))
  sprintf( title, d->character->pcdata->title);
  for( i = 0; title[i] != '\0'; i++)
  {
   if( (title[i] == '#'))
     sprintf(title+strlen(title), " ");
  }
  sprintf( buffer, "#7|#n  %6s  %10s %-15s  %3s   %3s   %3s #7|#n %12s%-21s#7 |#n\n\r", status,clan,
      rank, gen,stat,soul,name,title);
  switch(holder)
  {
   case 1:
   case 2:
   case 3:
   strcat(mor, buffer);
   break;
   case 4:
   strcat(ava, buffer);
   break;
   case 5:
   strcat(imm, buffer);
   break;
   case 6:
   strcat(gdl, buffer);
   break;
   case 7:
   strcat(dmg, buffer);
   break;
   case 8:
   strcat(lg, buffer);
   break;
   case 9:
   strcat(ggd, buffer);
   break;
   case 10:
   strcat(sup, buffer);
   break;
   case 11:
   strcat(ult, buffer);
   break;
   case 12:
   strcat(entity, buffer);
   break;
   case 13:
   strcat(qm, buffer);
   break;
   case 14:
   strcat(builder, buffer);
   break;
   case 15:
   strcat(enforcer,buffer);
   break;
   case 16:
   strcat(judge, buffer);
   break;
   case 17:
   strcat(hj, buffer);
   break;
   case 18:
   strcat(imp, buffer);
   break;
   default:
   break;
  }
  count++;
 }
 if( immfound)
 {

stc("#7|---------------------------------------------------#4+#7------------#1Immortals#7--------------|#n\n\r",
ch);
 stc(imp, ch);
 stc(hj, ch);
 stc(judge, ch);
 stc(enforcer, ch);
 stc(builder, ch);
 stc(qm, ch);
 }
 if(avafound)
 {

stc("#7|---------------------------------------------------#4+#7-------------#1Avatars#7---------------|#n\n\r",
ch);
 stc(entity, ch);
 stc(ult, ch);
 stc(sup, ch);
 stc(ggd, ch);
 stc(lg, ch);
 stc(dmg, ch);
 stc(gdl, ch);
 stc(imm,ch);
 stc(ava, ch);
 }
 if(morfound)
 {

stc("#7|---------------------------------------------------#4+#7-------------#1Mortals#7---------------|#n\n\r",
ch);
 stc(mor, ch);
 }

stc("#7|---------------------------------------------------#4+#7-----------------------------------|#n\n\r",
ch);
 stc("#7|                                                                                       |#n\n\r",
ch);
 stc("#7|#n                     Key: (G) Generation  (S) Status   (U) Souls                      #7 |#n\n\r", ch);
 sprintf(buffer,
  "#7|#n            New players this reboot: %3d   Old Players this reboot: %3d             #7   |#n\n\r",
new_players, old_players);
 stc(buffer, ch);
 sprintf(buffer,
"#7|#n                         Current Experience modifier: %3d percent                      #7|#n\n\r",
exp);
 stc(buffer, ch);
 sprintf(buffer,
     "#7|--------------------------[#n  %d visible players connected #7 ]----------------------------|#n\n\r", count);
 stc(buffer, ch);
 return;
}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    bool found;

    if (!IS_NPC(ch) && IS_HEAD(ch,LOST_HEAD))
        {send_to_char( "You are not a container.\n\r", ch ); return;}
    else if (!IS_NPC(ch) && IS_EXTRA(ch,EXTRA_OSWITCH))
    {
        if ( !IS_NPC(ch) && (obj = ch->pcdata->chobj) == NULL)
	    {send_to_char( "You are not a container.\n\r", ch ); return;}
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "You are not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "You don't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "You are empty.\n\r", ch );
		break;
	    }
	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in you.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "You contain a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "You're about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "You're about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "You are almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "You're completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow you are MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    act( "$p contain:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	for ( obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content )
	{
	    if ( obj2->wear_loc != WEAR_NONE
	    &&   can_see_obj( ch, obj2 )
	    &&   obj1->item_type == obj2->item_type
	    && ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if ( obj2 == NULL )
	{
	    send_to_char( "You aren't wearing anything comparable.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0];
	    value2 = obj2->value[0];
	    break;

	case ITEM_WEAPON:
	    value1 = obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}

void do_wizlist( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "wizlist" );
    return;
}

/*
void do_wizlist( CHAR_DATA *ch, char *argument )
{
 WIZ_DATA *list;
 char imm12[MAX_STRING_LENGTH];
 char imm11[MAX_STRING_LENGTH];
 char imm10[MAX_STRING_LENGTH];
 char imm09[MAX_STRING_LENGTH];
 char imm08[MAX_STRING_LENGTH];
 char imm07[MAX_STRING_LENGTH];
 bool a = FALSE;
 bool b = FALSE;
 bool c = FALSE;
 bool d = FALSE;
 bool e = FALSE;
 bool f = FALSE;

 imm12[0] = '\0';
 imm11[0] = '\0';
 imm10[0] = '\0';
 imm09[0] = '\0';
 imm08[0] = '\0';
 imm07[0] = '\0';

 if( IS_NPC(ch) )
  return;
 for( list = wiz_first; list; list = list->next)
 {
  switch( list->level )
  {
   default: break;
   case 12:
   {
    sprintf(imm12,list->name);
    a = TRUE;
    break;
   }
   case 11:
   {
    sprintf(imm11,list->name);
    b = TRUE;
    break;
   }
   case 10:
   {
    sprintf(imm10,list->name);
    c = TRUE;
    break;
   }
   case 9:
   {
    sprintf(imm09,list->name);
    d = TRUE;
    break;
   }
   case 8:
   {
    sprintf(imm08,list->name);
    e = TRUE;
    break;
   }
   case 7:
   {
    sprintf(imm07,list->name);
    f = TRUE;
    break;
   }
  }
  if( a )
  {
   stc(config_info->imm12,ch);
   stc("\n\r",ch);
   stc(imm12,ch);
   stc("\n\r",ch);
  }
  if( b )
  {
   stc(config_info->imm11,ch);
   stc("\n\r",ch);
   stc(imm11,ch);
   stc("\n\r",ch);
  }
  if( c )
  {
   stc(config_info->imm10,ch);
   stc("\n\r",ch);
   stc(imm10,ch);
   stc("\n\r",ch);
  }
  if( d )
  {
   stc(config_info->imm09,ch);
   stc("\n\r",ch);
   stc(imm09,ch);
   stc("\n\r",ch);
  }
  if( e )
  {
   stc(config_info->imm08,ch);
   stc("\n\r",ch);
   stc(imm08,ch);
   stc("\n\r",ch);
  }
  if( f )
  {
   stc(config_info->imm07,ch);
   stc("\n\r",ch);
   stc(imm07,ch);
   stc("\n\r",ch);
  }
 }
 return;
}
*/



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
	ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    bool found;
	char *area_name;




    one_argument( argument, arg );
	one_argument( argument, arg2);
	location = ( arg2[0] == '\0' ) ? ch->in_room : find_location( ch, arg2 );
    if ( location == NULL )
    {
		send_to_char( "No such location.\n\r", ch );
		return;
    }


	if ( location->area == NULL )

	{

		area_name = "No area";

	}

	else if ( location->area->name == NULL )

	{

		area_name = "Unnamed area";

	}

	else

	{

		area_name = location->area->name;

	}


    if ( arg[0] == '\0' )
    {
		send_to_char("Players near you.\n\r", ch);
		sprintf( buf2, "Local area : %s\n\r", area_name);
		send_to_char( buf2, ch );
		found = FALSE;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( ( d->connected == CON_PLAYING
	    ||   d->connected == CON_EDITING)
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   victim->pcdata->chobj == NULL
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;
    int overall;
    int con_hit;
    int con_dam;
    int con_ac;
    int con_hp;

    one_argument( argument, arg );
    overall = 0;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    act( "You examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_CHAR );
    act( "$n examines $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_NOTVICT );
    act( "$n examines you closely, looking for your weaknesses.", ch, NULL, victim, TO_VICT );

    if (!IS_NPC(victim))
      do_skill(ch,victim->name);

	con_hit = class_hitroll(victim);
	con_dam = class_damroll(victim);
	con_ac = class_ac(victim);
	con_hp = victim->hit;
    if (con_hp < 1) con_hp = 1;

    diff = victim->level - ch->level + con_hit - class_hitroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more skilled than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as skilled as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as skilled as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as skilled as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more skilled than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more skilled than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more skilled than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = victim->level - ch->level + con_dam - class_damroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more powerful than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as powerful as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as powerful as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as powerful as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more powerful than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more powerful than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more powerful than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = ch->hit * 100 / con_hp;
         if ( diff <=  10 ) {msg = "$E is currently FAR healthier than you are."; overall = overall - 3;}
    else if ( diff <=  50 ) {msg = "$E is currently much healthier than you are."; overall = overall - 2;}
    else if ( diff <=  75 ) {msg = "$E is currently slightly healthier than you are."; overall = overall - 1;}
    else if ( diff <= 125 ) {msg = "$E is currently about as healthy as you are.";}
    else if ( diff <= 200 ) {msg = "You are currently slightly healthier than $M."; overall = overall + 1;}
    else if ( diff <= 500 ) {msg = "You are currently much healthier than $M."; overall = overall + 2;}
    else                    {msg = "You are currently FAR healthier than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = con_ac + class_ac(ch);
         if ( diff <= -100) {msg = "$E is FAR better armoured than you."; overall = overall - 3;}
    else if ( diff <= -50 ) {msg = "$E looks much better armoured than you."; overall = overall - 2;}
    else if ( diff <= -25 ) {msg = "$E looks better armoured than you."; overall = overall - 1;}
    else if ( diff <=  25 ) {msg = "$E seems about as well armoured as you.";}
    else if ( diff <=  50 ) {msg = "You are better armoured than $M."; overall = overall + 1;}
    else if ( diff <=  100) {msg = "You are much better armoured than $M."; overall = overall + 2;}
    else                    {msg = "You are FAR better armoured than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = overall;
         if ( diff <= -11 ) msg = "Conclusion: $E would kill you in seconds.";
    else if ( diff <=  -7 ) msg = "Conclusion: You would need a lot of luck to beat $M.";
    else if ( diff <=  -3 ) msg = "Conclusion: You would need some luck to beat $N.";
    else if ( diff <=   2 ) msg = "Conclusion: It would be a very close fight.";
    else if ( diff <=   6 ) msg = "Conclusion: You shouldn't have a lot of trouble defeating $M.";
    else if ( diff <=  10 ) msg = "Conclusion: $N is no match for you.  You can easily beat $M.";
    else                    msg = "Conclusion: $E wouldn't last more than a few seconds against you.";
    act( msg, ch, NULL, victim, TO_CHAR );

    return;
}



void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{

    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;
	if (ch->level < 3 && ch->max_hit < 3000)
	{
		send_to_char("You may not change your title until you have 3000 hp.\n\r", ch);
		return;
	}

    if( IS_DAMNED(ch) ) return;

/* Commented...For NOW :P --Kline
    if( !str_cmp(ch->pcdata->switchname, "Kalfyre") )
    {
     stc("Never! BUAUAHHAHA!!\n\r", ch);
     return;
    }
*/
    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }
    if( argument[0] == 'D' && argument[1] == 'A' && argument[2] == 'M'
    &&  argument[3] == 'N' && argument[4] == 'E' && argument[5] == 'D' )
	argument = str_dup("DAMNED");
    if( !str_cmp("DAMNED",  argument) || !str_cmp("#DAMNED", argument)
    ||  !str_cmp(".DAMNED", argument) || !str_cmp("DAMNED ", argument) )
    {
	stc("If you wish to be damned, commit suicide.\n\r", ch);
	stc("If you have the title damned in any form, when you\n\r", ch);
	stc("are not damned, you will be paradoxed.\n\r", ch);
	return;
    }

    if ( strlen(argument) > 17 )
	argument[17] = '\0';

    smash_tilde( argument );
    sprintf(buf,"#n");
    strcpy(buf,argument);
    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}


void do_report( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    char hit_str[MAX_INPUT_LENGTH];
    char mana_str[MAX_INPUT_LENGTH];
    char move_str[MAX_INPUT_LENGTH];
    char mhit_str[MAX_INPUT_LENGTH];
    char mmana_str[MAX_INPUT_LENGTH];
    char mmove_str[MAX_INPUT_LENGTH];
    char exp_str[MAX_INPUT_LENGTH];
    sprintf(hit_str, "%d", ch->hit);
    COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
    sprintf(mana_str, "%d", ch->mana);
    COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
    sprintf(move_str, "%d", ch->move);
    COL_SCALE(move_str, ch, ch->move, ch->max_move);
    sprintf(exp_str, "%d", ch->exp);
    COL_SCALE(exp_str, ch, ch->exp, 1000);
    sprintf(mhit_str, "%d", ch->max_hit);
    ADD_COLOUR(ch, mhit_str, L_CYAN);
    sprintf(mmana_str, "%d", ch->max_mana);
    ADD_COLOUR(ch, mmana_str, L_CYAN);
    sprintf(mmove_str, "%d", ch->max_move);
    ADD_COLOUR(ch, mmove_str, L_CYAN);
    sprintf( buf,
	"You report: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	hit_str,  mhit_str,
	mana_str, mmana_str,
	move_str, mmove_str,
	exp_str   );

    send_to_char( buf, ch );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch == NULL ) continue;
	if ( vch == ch ) continue;
	if ( vch->in_room == NULL ) continue;
	if ( vch->in_room != ch->in_room ) continue;
    	sprintf(hit_str, "%d", ch->hit);
    	COL_SCALE(hit_str, vch, ch->hit, ch->max_hit);
    	sprintf(mana_str, "%d", ch->mana);
    	COL_SCALE(mana_str, vch, ch->mana, ch->max_mana);
    	sprintf(move_str, "%d", ch->move);
    	COL_SCALE(move_str, vch, ch->move, ch->max_move);
    	sprintf(exp_str, "%d", ch->exp);
    	COL_SCALE(exp_str, vch, ch->exp, 1000);
    	sprintf(mhit_str, "%d", ch->max_hit);
    	ADD_COLOUR(vch, mhit_str, L_CYAN);
    	sprintf(mmana_str, "%d", ch->max_mana);
    	ADD_COLOUR(vch, mmana_str, L_CYAN);
    	sprintf(mmove_str, "%d", ch->max_move);
    	ADD_COLOUR(vch, mmove_str, L_CYAN);
    	if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    ch->morph,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
        else
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    IS_NPC(ch) ? capitalize(ch->short_descr) : ch->name,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
	buf[0] = UPPER(buf[0]);
    	send_to_char( buf, vch );
    }
    return;
}

/* Klinemark, fix the memory overflows to properly sort
void do_practice(CHAR_DATA * ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  int sn;

  if (IS_NPC(ch))
    return;

  if (argument[0] == '\0')
  {
    int col, i;

    col = 0;
    for (i = 0; i < 6; i++)
    {
      for (sn = 0; sn < MAX_SKILL; sn++)
      {
        if (skill_table[sn].name == NULL)
          break;
        if (ch->level < skill_table[sn].skill_level)
          continue;
        //if (skill_table[sn].target != i)
        //  continue;
        switch (i)
        {
          case 0:
	    if (skill_table[sn].target == i)
             sprintf(buf, "#p%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 1:
            if (skill_table[sn].target == i)
             sprintf(buf, "#R%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 2:
            if (skill_table[sn].target == i)
             sprintf(buf, "#L%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 3:
            if (skill_table[sn].target == i)
            sprintf(buf, "#G%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 4:
            if (skill_table[sn].target == i)
            sprintf(buf, "#y%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
	  case 5:
            if( skill_table[sn].spell_fun == spell_null )
             sprintf(buf, "#6%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
          default:
            sprintf(buf, " ");
            break;
        }
     //   if( skill_table[sn].spell_fun == spell_null )
     //    sprintf(buf, "#6%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
        send_to_char(buf, ch);
        if (++col % 3 == 0)
          send_to_char("\n\r", ch);
      }
    }
    send_to_char("#n\n\r", ch);

    if (col % 3 != 0)
      send_to_char("\n\r", ch);

    sprintf(buf, "You have %d exp left.\n\r", ch->exp);
    send_to_char(buf, ch);
  }
  else if (!strcmp(argument, "all"))
  {
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if (ch->exp < 5000)
        break;
      if ((skill_table[sn].name != NULL) && (ch->level >= skill_table[sn].skill_level) && ch->pcdata->learned[sn] < 100)
      {
        ch->pcdata->learned[sn] = 100;
        ch->exp -= 5000;
      }
    }
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else
  {
    if (!IS_AWAKE(ch))
    {
      send_to_char("In your dreams, or what?\n\r", ch);
      return;
    }

    if (ch->exp <= 0)
    {
      send_to_char("You have no exp left.\n\r", ch);
      return;
    }

    if ((sn = skill_lookup(argument)) < 0 || (!IS_NPC(ch) && ch->level < skill_table[sn].skill_level))
    {
      send_to_char("You can't practice that.\n\r", ch);
      return;
    }

    if (ch->pcdata->learned[sn] >= SKILL_ADEPT)
    {
      sprintf(buf, "You are already an adept of %s.\n\r", skill_table[sn].name);
      send_to_char(buf, ch);
    }
    else if (ch->pcdata->learned[sn] > 0 && ch->exp < 5000)
    {
      sprintf(buf, "You need 5000 exp to increase %s any more.\n\r", (skill_table[sn].name));
      send_to_char(buf, ch);
    }
    else if (ch->pcdata->learned[sn] == 0 && ch->exp < 5000)
    {
      sprintf(buf, "You need 5000 exp to increase %s.\n\r", skill_table[sn].name);
      send_to_char(buf, ch);
    }
    else
    {
      if (ch->pcdata->learned[sn] == 0)
      {
        ch->exp -= 5000;
        ch->pcdata->learned[sn] += 100;
      }
      else
      {
        ch->exp -= (ch->pcdata->learned[sn] / 2);
        ch->pcdata->learned[sn] += int_app[get_curr_int(ch)].learn;
      }
      if (ch->pcdata->learned[sn] < SKILL_ADEPT)
      {
        act("You practice $T.", ch, NULL, skill_table[sn].name, TO_CHAR);
      }
      else
      {
        ch->pcdata->learned[sn] = SKILL_ADEPT;
        act("You are now an adept of $T.", ch, NULL, skill_table[sn].name, TO_CHAR);
      }
    }
  }
  return;
}
*/

void do_practice(CHAR_DATA * ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  int sn;

  if (IS_NPC(ch))
    return;

  if (argument[0] == '\0')
  {
    int col, i;

    col = 0;
    for (i = 0; i < 5; i++)
    {
      for (sn = 0; sn < MAX_SKILL; sn++)
      {
        if (skill_table[sn].name == NULL)
          break;
        if (ch->level < skill_table[sn].skill_level)
          continue;
        if (skill_table[sn].target != i)
          continue;
        switch (i)
        {
          case 0:
            sprintf(buf, "#p%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 1:
            sprintf(buf, "#R%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 2:
            sprintf(buf, "#L%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 3:
            sprintf(buf, "#G%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          case 4:
            sprintf(buf, "#y%18s #9%3d%%  ", skill_table[sn].name, ch->pcdata->learned[sn]);
            break;
          default:
            sprintf(buf, " ");
            break;
        }
        send_to_char(buf, ch);
        if (++col % 3 == 0)
          send_to_char("\n\r", ch);
      }
    }
    send_to_char("#n\n\r", ch);

    if (col % 3 != 0)
      send_to_char("\n\r", ch);

    sprintf(buf, "You have %d exp left.\n\r", ch->exp);
    send_to_char(buf, ch);
  }
  else if (!strcmp(argument, "all"))
  {
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if (ch->exp < 5000)
        break;
      if ((skill_table[sn].name != NULL) && (ch->level >= skill_table[sn].skill_level) && ch->pcdata->learned[sn] < 100)
      {
        ch->pcdata->learned[sn] = 100;
        ch->exp -= 5000;
      }
    }
    send_to_char("Ok.\n\r", ch);
    return;
  }
  else
  {
    if (!IS_AWAKE(ch))
    {
      send_to_char("In your dreams, or what?\n\r", ch);
      return;
    }

    if (ch->exp <= 0)
    {
      send_to_char("You have no exp left.\n\r", ch);
      return;
    }

    if ((sn = skill_lookup(argument)) < 0 || (!IS_NPC(ch) && ch->level < skill_table[sn].skill_level))
    {
      send_to_char("You can't practice that.\n\r", ch);
      return;
    }

    if (ch->pcdata->learned[sn] >= SKILL_ADEPT)
    {
      sprintf(buf, "You are already an adept of %s.\n\r", skill_table[sn].name);
      send_to_char(buf, ch);
    }
    else if (ch->pcdata->learned[sn] > 0 && ch->exp < 5000)
    {
      sprintf(buf, "You need 5000 exp to increase %s any more.\n\r", (skill_table[sn].name));
      send_to_char(buf, ch);
    }
    else if (ch->pcdata->learned[sn] == 0 && ch->exp < 5000)
    {
      sprintf(buf, "You need 5000 exp to increase %s.\n\r", skill_table[sn].name);
      send_to_char(buf, ch);
    }
    else
    {
      if (ch->pcdata->learned[sn] == 0)
      {
        ch->exp -= 5000;
        ch->pcdata->learned[sn] += 100;
      }
      else
      {
        ch->exp -= (ch->pcdata->learned[sn] / 2);
        ch->pcdata->learned[sn] += int_app[get_curr_int(ch)].learn;
      }
      if (ch->pcdata->learned[sn] < SKILL_ADEPT)
      {
        act("You practice $T.", ch, NULL, skill_table[sn].name, TO_CHAR);
      }
      else
      {
        ch->pcdata->learned[sn] = SKILL_ADEPT;
        act("You are now an adept of $T.", ch, NULL, skill_table[sn].name, TO_CHAR);
      }
    }
  }
  return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( arg1, ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password, Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    /*
     *pwdnew = crypt( arg2, ch->pcdata->switchname );
     *for ( p = pwdnew; *p != '\0'; p++ )
     */

	pwdnew = arg2;
	for ( p = pwdnew; *p != '\0'; p++ )

     {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    if (!IS_EXTRA(ch,EXTRA_NEWPASS)) SET_BIT(ch->extra,EXTRA_NEWPASS);
    save_char_obj( ch );
    if (ch->desc != NULL && ch->desc->connected == CON_PLAYING )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if( (handle = dlsym(sociallib,"list_socials")) == NULL )
 {
  stc("An error has occurred and been logged, please inform Kline.\n\r",ch);
  log_string(dlerror());
  return;
 }
 else
  (*handle)(ch,argument);
 return;
}


void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    col = 0;
    for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name != NULL; sn++ )
    {
	sprintf( buf, "%-12s", skill_table[sn].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level == 0
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
	{
	    send_to_char( "You are silenced.\n\r", ch );
	    return;
	}

	send_to_char( "Channels:", ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_NEWTALK)
	    ? " +NEWTALK"
	    : " -newtalk",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_CHAT)
	    ? " +CHAT"
	    : " -chat",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_CLAN)
	    ? " +CLAN"
	    : " -clan",
	    ch );


	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_IMMTALK)
		? " +IMMTALK"
		: " -immtalk",
		ch );
	}

	send_to_char( !IS_SET(ch->deaf, CHANNEL_MUSIC)
	    ? " +MUSIC"
	    : " -music",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_QUESTION)
	    ? " +QUESTION"
	    : " -question",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_SHOUT)
	    ? " +SHOUT"
	    : " -shout",
	    ch );


	if (IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_LOG)
		? " +LOG"
		: " -log",
		ch );
	}

	send_to_char( !IS_SET(ch->deaf, CHANNEL_INFO)
	    ? " +INFO"
	    : " -info",
	    ch );


	send_to_char( !IS_SET(ch->deaf, CHANNEL_TELL)
	    ? " +TELL"
	    : " -tell",
	    ch );

	send_to_char( ".\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

           if ( !str_cmp( arg+1, "chat"     ) )
            bit = CHANNEL_CHAT;
	else if ( !str_cmp( arg+1, "immtalk"  ) )
		bit = CHANNEL_IMMTALK;
      else if ( !str_cmp( arg+1, "newtalk"  ) )
            bit = CHANNEL_NEWTALK;
      else if ( !str_cmp( arg+1, "music"    ) )
		bit = CHANNEL_MUSIC;
	else if ( !str_cmp( arg+1, "question" ) )
		bit = CHANNEL_QUESTION;
	else if ( !str_cmp( arg+1, "shout"    ) )
		bit = CHANNEL_SHOUT;
	else if ( !str_cmp( arg+1, "yell"     ) )
		bit = CHANNEL_YELL;
/*	else if ( !str_cmp( arg+1, "howl"     ) )
		bit = CHANNEL_HOWL;
*/	else if (IS_IMMORTAL(ch) && !str_cmp( arg+1, "log") )
		bit = CHANNEL_LOG;
	else if ( !str_cmp( arg+1, "info"     ) )
		bit = CHANNEL_INFO;
	else if ( !str_cmp( arg+1, "tell"     ) )
		bit = CHANNEL_TELL;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    REMOVE_BIT (ch->deaf, bit);
	else
	    SET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "[ Keyword  ] Option\n\r", ch );

	send_to_char(  IS_SET(ch->act, PLR_ANSI)
            ? "[+ANSI     ] You have ansi colour on.\n\r"
	    : "[-ansi     ] You have ansi colour off.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOEXIT)
            ? "[+AUTOEXIT ] You automatically see exits.\n\r"
	    : "[-autoexit ] You don't automatically see exits.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOLOOT)
	    ? "[+AUTOLOOT ] You automatically loot corpses.\n\r"
	    : "[-autoloot ] You don't automatically loot corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOSAC)
	    ? "[+AUTOSAC  ] You automatically sacrifice corpses.\n\r"
	    : "[-autosac  ] You don't automatically sacrifice corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BLANK)
	    ? "[+BLANK    ] You have a blank line before your prompt.\n\r"
	    : "[-blank    ] You have no blank line before your prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BRIEF)
	    ? "[+BRIEF    ] You see brief descriptions.\n\r"
	    : "[-brief    ] You see long descriptions.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_COMBINE)
	    ? "[+COMBINE  ] You see object lists in combined format.\n\r"
	    : "[-combine  ] You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PROMPT)
	    ? "[+PROMPT   ] You have a prompt.\n\r"
	    : "[-prompt   ] You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_TELNET_GA)
	    ? "[+TELNETGA ] You receive a telnet GA sequence.\n\r"
	    : "[-telnetga ] You don't receive a telnet GA sequence.\n\r"
	    , ch );
	send_to_char(  IS_SET(ch->act, PLR_COL_SCALE)
	    ? "[+COLSCALE ] Your prompt uses the color scale.\n\r"
	    : "[-colscale ] Your prompt does not use the color scale.\n\r"
	    , ch );
        send_to_char(  IS_SET(ch->act, PLR_NEW_HELP)
            ? "[+NEWHELP  ] You are listening to the newbie ticker.\n\r"
            : "[-newhelp  ] You are not listening to the newbie ticker.\n\r"
            , ch );
	send_to_char(  IS_SET(ch->act, PLR_SILENCE)
	    ? "[+SILENCE  ] You are silenced.\n\r"
	    : ""
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "[-tell     ] You can't use 'tell'.\n\r"
	    , ch );
        send_to_char( IS_SET(ch->act, PLR_MSP)
            ? "[+MSP      ] You have MSP enabled.\n\r"
            : "[-msp      ] You have MSP disabled.\n\r"
            , ch);
        send_to_char( IS_MXP(ch)
            ? "[+MXP      ] You have MXP enabled.\n\r"
            : "[-mxp      ] You have MXP disabled.\n\r"
            , ch); 

    }
    else
    {
	bool fSet;
	int bit;

	     if ( arg[0] == '+' ) fSet = TRUE;
	else if ( arg[0] == '-' ) fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}

             if ( !str_cmp( arg+1, "ansi"     ) ) bit = PLR_ANSI;
        else if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
	else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
	else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
	else if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
	else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
	else if ( !str_cmp( arg+1, "colscale" ) ) bit = PLR_COL_SCALE;
	else if ( !str_cmp( arg+1, "newhelp"  ) ) bit = PLR_NEW_HELP;
        else if ( !str_cmp( arg+1, "msp"      ) ) bit = PLR_MSP;
        else if ( !str_cmp( arg+1, "nogroup"  ) ) bit = PLR_NO_GROUP;
        else if ( !str_cmp( arg+1, "mxp"      ) ) bit = PLR_MXP;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
	    SET_BIT    (ch->act, bit);
        if ( fSet && bit == PLR_MXP )
         do_help(ch,"MXP_DEF");
	else if( !fSet && bit != PLR_NO_GROUP )
	    REMOVE_BIT (ch->act, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}

void do_mxp( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_MXP(ch)) do_config(ch,"-mxp");
    else do_config(ch,"+mxp");
    return;
}

void do_msp( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_MSP)) do_config(ch,"-msp");
    else do_config(ch,"+msp");
    return;
}

void do_ansi( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_ANSI)) do_config(ch,"-ansi");
    else do_config(ch,"+ansi");
    return;
}

void do_autoexit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOEXIT)) do_config(ch,"-autoexit");
    else do_config(ch,"+autoexit");
    return;
}

void do_autoloot( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOLOOT)) do_config(ch,"-autoloot");
    else do_config(ch,"+autoloot");
    return;
}

void do_autosac( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOSAC)) do_config(ch,"-autosac");
    else do_config(ch,"+autosac");
    return;
}

void do_blank( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BLANK)) do_config(ch,"-blank");
    else do_config(ch,"+blank");
    return;
}

void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF)) do_config(ch,"-brief");
    else do_config(ch,"+brief");
    return;
}
void do_colscale( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_COL_SCALE)) do_config(ch,"-colscale");
    else do_config(ch,"+colscale");
    return;
}

void do_newhelp( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_NEW_HELP)) do_config(ch,"-newhelp");
    else do_config(ch,"+newhelp");
    return;
}

void do_diagnose( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg  [MAX_INPUT_LENGTH];
    int teeth = 0;
    int ribs = 0;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (arg == '\0')
    {
	send_to_char("Who do you wish to diagnose?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("Nobody here by that name.\n\r",ch);
	return;
    }
    act("$n examines $N carefully, diagnosing $S injuries.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you carefully, diagnosing your injuries.",ch,NULL,victim,TO_VICT);
    act("Your diagnoses of $N reveals the following...",ch,NULL,victim,TO_CHAR);
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] +
	   victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
	   victim->loc_hp[6] ) == 0 )
    {
	act("$N has no apparent injuries.",ch,NULL,victim,TO_CHAR);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	return;
    }
    /* Check head */
    if (IS_HEAD(victim,LOST_EYE_L) && IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost both of $S eyes.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_L))
	act("$N has lost $S left eye.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost $S right eye.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_EAR_L) && IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost both of $S ears.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_L))
	act("$N has lost $S left ear.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost $S right ear.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_NOSE))
	act("$N has lost $S nose.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,BROKEN_NOSE))
	act("$N has got a broken nose.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,BROKEN_JAW))
	act("$N has got a broken jaw.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
	act("$N has had $S head cut off.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HEAD))
	    act("...Blood is spurting from the stump of $S neck.",ch,NULL,victim,TO_CHAR);
    }
    else
    {
	if (IS_BODY(victim,BROKEN_NECK))
	    act("$N has got a broken neck.",ch,NULL,victim,TO_CHAR);
	if (IS_BODY(victim,CUT_THROAT))
	{
	    act("$N has had $S throat cut open.",ch,NULL,victim,TO_CHAR);
	    if (IS_BLEEDING(victim,BLEEDING_THROAT))
		act("...Blood is pouring from the wound.",ch,NULL,victim,TO_CHAR);
	}
    }
    if (IS_HEAD(victim,BROKEN_SKULL))
	act("$N has got a broken skull.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_TOOTH_1 )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2 )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4 )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8 )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16)) teeth += 16;
    if (teeth > 0)
    {
	sprintf(buf,"$N has had %d teeth knocked out.",teeth);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_HEAD(victim,LOST_TONGUE))
	act("$N has had $S tongue ripped out.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
    	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    	return;
    }
    /* Check body */
    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;
    if (ribs > 0)
    {
	sprintf(buf,"$N has got %d broken ribs.",ribs);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_BODY(victim,BROKEN_SPINE))
	act("$N has got a broken spine.",ch,NULL,victim,TO_CHAR);
    /* Check arms */
    check_left_arm(ch,victim);
    check_right_arm(ch,victim);
    check_left_leg(ch,victim);
    check_right_leg(ch,victim);
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

void check_left_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S arms.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L) && IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_ARM))
    {
	act("$N has lost $S left arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N arms are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,BROKEN_ARM))
	act("$N's left arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND) &&
	!IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S hands.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L) && IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_HAND))
    {
	act("$N has lost $S left hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	return;
    if (IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost $S right arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND))
	return;
    if (IS_ARM_R(victim,LOST_HAND))
    {
	act("$N has lost $S right hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_left_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost both of $S legs.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L) && IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_LEG))
    {
	act("$N has lost $S left leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N legs are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_LEG_L(victim,BROKEN_LEG))
	act("$N's left leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost both of $S feet.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L) && IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_FOOT))
    {
	act("$N has lost $S left foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_L(victim,BROKEN_TOE_A) && !IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_B) && !IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_C) && !IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_D) && !IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	return;
    if (IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost $S right leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N's right leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
	return;
    if (IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost $S right foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_R(victim,BROKEN_TOE_A) && !IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_B) && !IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_C) && !IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_D) && !IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void obj_score( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    sprintf( buf,"You are %s.\n\r",obj->short_descr);
    send_to_char( buf, ch );

    sprintf( buf,"Type %s, Extra flags %s.\n\r",item_type_name(obj),
	extra_bit_name(obj->extra_flags));
    send_to_char( buf, ch );

    sprintf( buf,"You weigh %d pounds and are worth %d gold coins.\n\r",obj->weight,obj->cost);
    send_to_char( buf, ch );

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
	obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You were created by %s, and are owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	sprintf( buf, "You were created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You are owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }

    switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
	sprintf( buf, "You contain level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_QUEST:
	sprintf( buf, "Your quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	sprintf( buf, "You have %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_WEAPON:
	sprintf( buf, "You inflict %d to %d damage in combat (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		sprintf(buf,"You are a minor spell weapon.\n\r");
	    else if (obj->level < 20)
		sprintf(buf,"You are a lesser spell weapon.\n\r");
	    else if (obj->level < 30)
		sprintf(buf,"You are an average spell weapon.\n\r");
	    else if (obj->level < 40)
		sprintf(buf,"You are a greater spell weapon.\n\r");
	    else if (obj->level < 50)
		sprintf(buf,"You are a major spell weapon.\n\r");
	    else
		sprintf(buf,"You are a supreme spell weapon.\n\r");
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    sprintf (buf, "You are dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "You are the bane of all evil.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "You drink the souls of your victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "You have been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "You crackle with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "You are dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "You have been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4 || itemtype == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "You allow your wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "You grant your wielder the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 4)
	    sprintf (buf, "You allow your wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "You render your wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "You allow your wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "You protect your wielder from evil.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "You protect your wielder in combat.\n\r");
	else if (itemtype == 9)
	    sprintf (buf, "You allow your wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    sprintf (buf, "You surround your wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    sprintf (buf, "You surround your wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    sprintf (buf, "You surround your wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    sprintf (buf, "You surround your wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    sprintf (buf, "You protect your wielder from attacks from DarkBlade clan guardians.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "You enable your wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "You protect your wielder from player attacks.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "You surround your wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "You grant your wielder superior protection.\n\r");
	else if (itemtype == 22)
	    sprintf (buf, "You grant your wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "You make your wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "You conceal your wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "You invoke the power of your wielders beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	sprintf( buf, "Your armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4 || obj->value[3] == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "You allow your wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "You grant your wearer the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 4)
	    sprintf (buf, "You allow your wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "You render your wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "You allow your wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "You protect your wearer from evil.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "You protect your wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "You allow your wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    sprintf (buf, "You surround your wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    sprintf (buf, "You surround your wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    sprintf (buf, "You surround your wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    sprintf (buf, "You surround your wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    sprintf (buf, "You protect your wearer from attacks from DarkBlade clan guardians.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "You enable your wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "You protect your wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "You surround your wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "You grant your wearer superior protection.\n\r");
	else if (obj->value[3] == 22)
	    sprintf (buf, "You grant your wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "You make your wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "You conceal your wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "You invoke the power of your wearers beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "prompt" );
       return;
   }

   if( !strcmp( argument, "on" ) )
   {
      if (IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt on!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         SET_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "off" ) )
   {
      if (!IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt off!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         REMOVE_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->prompt);
      ch->prompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 100 )
	  argument[100] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_cprompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "cprompt" );
       return;
   }

   if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->cprompt);
      ch->cprompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 100 )
	  argument[100] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->cprompt );
   ch->cprompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
}

void do_finger( CHAR_DATA *ch, char *argument)
{
  FILE *fp;
  char strsave[MAX_STRING_LENGTH];
  char *name;
  char *title;
  char *host;
   char *time;
  char *created;
  int status, gen,soul,level,pkill,pdeath,awin,aloss,mkill,mdeath;
  int name_size, pre_space, x, i;
  const int SPACE_SIZE = 24;
  char new_name[SPACE_SIZE - 1];
  char *clan;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  char divider[MSL] = "#4o#6--------------------------------------------------#4o#n\n\r";
  one_argument(argument, arg);


  new_name[0] = '\0';
  if(arg[0] == '\0')
  {
   stc("Finger who?\n\r", ch);
   return;
  }
  if( !check_parse_name(argument))
  {
   stc("That is not a valid name.\n\r", ch);
   return;
  }
  if( !str_cmp(argument, "apostasy") )
  {
	stc("That character doesn't exist.\n\r", ch);
	return;
  }
  if( !char_exists(TRUE, argument))
  {
   stc("That character doesn't exist.\n\r", ch);
   return;
  }
  fclose( fpReserve );
  sprintf( strsave, "%sbackup/%s", PLAYER_DIR, capitalize(argument) );
  if( (fp = fopen( strsave, "r")) == NULL)
  {
   stc("That file doesnt exist.\n\r", ch);
   return;
  }
  name = fread_string(fp);
  title = fread_string(fp);
  host = fread_string(fp);
  time = fread_string(fp);
  created = fread_string(fp);
  level = fread_number(fp);
  status = fread_number(fp);
  gen = fread_number(fp);
  soul = fread_number(fp);
  pkill = fread_number(fp);
  pdeath = fread_number(fp);
  awin = fread_number(fp);
  aloss = fread_number(fp);
  mkill = fread_number(fp);
  mdeath = fread_number(fp);
  clan = fread_string(fp);

  name_size = strlen(arg);
  pre_space = (SPACE_SIZE - name_size) / 2;
  for( i = 0; i < pre_space; i++)
	new_name[i] = ' ';
  for( i = pre_space; i < pre_space + name_size; i++ )
  {
	x = i - pre_space;
	new_name[i] = arg[x];
	new_name[pre_space] = toupper(new_name[pre_space]);
  }
  for( i = pre_space + name_size; i < SPACE_SIZE; i++ )
	new_name[i] = ' ';
  new_name[SPACE_SIZE] = '\0';
  sprintf(buf, "#4O#6==========#7+#6o#4{#7%s#4}#6o#7+#6==========#4O#n\n\r", new_name);
  stc(buf, ch);
  stc("#6|                                                  |#n\n\r", ch);
  stc(divider, ch);
  sprintf(buf, "#6| #7Generation: #n%5d   #7Status: #n%5d   #7Souls: #n%5d #6|#n\n\r", gen, status, soul);
  stc(buf, ch);
  stc(divider, ch);


  switch(level)
  {
   case 1:
    sprintf(buf, "#5  Newbie#n");
    break;
   case 2:
    sprintf(buf, "#7  Mortal#n");
    break;
   case 3:
    sprintf(buf, "#1  Avatar#n");
    break;
   case 4:
    sprintf(buf, "#0  DAMNED#n");
    break;
   case 7:
    sprintf(buf, "#4Immortal#n");
    break;
   case 8:
    sprintf(buf, "#4Immortal#n");
    break;
   case 9:
    sprintf(buf, "#4Immortal#n");
    break;
   case 10:
    sprintf(buf, "#4Immortal#n");
    break;
   case 11:
    sprintf(buf, "#4Immortal#n");
    break;
   case 12:
    sprintf(buf, "#4Immortal#n");
    break;
   default:
    sprintf(buf, "BUGGED\n\r");
    break;
  }
  stc("#6| #7Level: ", ch);
  stc(buf, ch);


/*  sprintf(buf, "  #7Class:  #n%16s#6|#n\n\r", class);
    stc(buf, ch);
*/

  stc("  #7Class: #nINCOMPLETE               #6|#n\n\r", ch);

  sprintf(buf, "#6| #7Clan:  #n%8s", clan);
  stc(buf, ch);
  stc("  #7Rank:  #nINCOMPLETE               #6|#n\n\r", ch);

/*
  sprintf(buf, "  #7Rank:  #n%16s#6|#n\n\r", rank);
  stc(buf, ch);
*/

  stc(divider, ch);
  if( (pkill + pdeath) > 0 )
    sprintf(buf, "#6| #7PKills: #n%6d   #7PDeaths: #1%4d  #7PK Ratio: #n%3.0f", pkill, pdeath, (float)pkill/(pkill+pdeath)*100);
  else
    sprintf(buf, "#6| #7PKills: #n%6d   #7PDeaths: #1%4d  #7PK Ratio: #n  0", pkill, pdeath);
  stc(buf, ch);
  stc("%   #6|\n\r", ch);
  if( (awin + aloss) > 0 )
    sprintf(buf, "#6| #7AWins:  #n%6d   #7ALosses: #1%4d  #7AW Ratio: #n%3.0f", awin, aloss, (float)awin/(awin + aloss)* 100);
  else
    sprintf(buf, "#6| #7AWins:  #n%6d   #7ALosses: #1%4d  #7AW Ratio: #n  0", awin, aloss);
  stc(buf, ch);
  stc("%   #6|\n\r", ch);
  sprintf(buf, "#6| #7MKills: #n%6d   #7MDeaths: #1%4d                   #6|\n\r", mkill, mdeath);
  stc(buf, ch);
  stc(divider, ch);
  sprintf(buf, "#6| #7Last Seen On:  #n%14s          #6|#n\n\r", time);
  stc(buf, ch);
  sprintf(buf, "#6| #7Created On:    #n%14s          #6|#n\n\r", created);
  stc(buf, ch);
  if( ch->level > 10 )
  {
	stc(divider, ch);
	stc("#6| #7Last Connected From:                             #6|#n\n\r", ch);
	sprintf(buf, "#6| #n%-48s #6|#n\n\r", host);
     	stc(buf,ch);
  }
  stc("#4O#6==================================================#4O#n\n\r", ch);
  stc("\n\r", ch);
  new_name[0] = '\0';
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );

    return;
}

void do_ethereal(CHAR_DATA *ch, char *argument)
{
 if(IS_NPC(ch))
  return;
 if(ch->race < 18)
  return;
 if(ch->fight_timer != 0 && !IS_AFFECTED(ch, AFF_ETHEREAL) )
 {
  stc("You can not transcend the ether while blood is still on your hands!\n\r", ch);
  return;
 }
 if (!IS_AFFECTED(ch, AFF_ETHEREAL))
 {
  if(ch->mana < 10000)
  {
   stc("You lack the 10,000 mana required to fade into the ether.\n\r", ch);
   return;
  }
  ch->mana -= 10000;
  stc("You vanish into the ether.\n\r",ch);
  act("$n vanishes into the ether.",ch,NULL,NULL,TO_ROOM);
  SET_BIT(ch->affected_by, AFF_ETHEREAL);
  do_look(ch,"auto");
  return;
 }

 else
 {
  REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
  stc("You rematerialize from the ether.\n\r",ch);
  act("$n rematerializes from the ether.",ch,NULL,NULL,TO_ROOM);
  do_look(ch,"auto");
  return;
 }
 return;
}

void do_who( CHAR_DATA *ch, char *argument )
{
 char buffer[MSL];
 char imm12[MSL];
 char imm11[MSL];
 char imm10[MSL];
 char imm09[MSL];
 char imm08[MSL];
 char imm07[MSL];
 char title50[MSL];
 char title45[MSL];
 char title40[MSL];
 char title35[MSL];
 char title30[MSL];
 char title25[MSL];
 char title20[MSL];
 char title15[MSL];
 char title10[MSL];
 char title05[MSL];
 char title00[MSL];
 char titlenone[MSL];
 char mor[MSL];
 char name[MSL];
 char ratio[MSL];
 char status[MSL];
 char title[MSL];
 char gen[MSL];
 char soul[MSL];
 char stat[MSL];
 char class[MSL];
 DESCRIPTOR_DATA *d;
 sh_int count=0;
 sh_int blah;
 sh_int holder;
 int exp = 0;
 bool morfound = FALSE;
 bool avafound = FALSE;
 bool immfound = FALSE;

 if( !IS_NPC(ch) && (ch->pcdata->who == NULL || strlen(ch->pcdata->who) > 1))
 {
  do_new_who(ch, argument);
  return;
 }
 buffer[0] = '\0';
 imm12[0] = '\0';
 imm11[0] = '\0';
 imm10[0] = '\0';
 imm09[0] = '\0';
 imm08[0] = '\0';
 imm07[0] = '\0';
 title50[0] = '\0';
 title45[0] = '\0';
 title40[0] = '\0';
 title35[0] = '\0';
 title30[0] = '\0';
 title25[0] = '\0';
 title20[0] = '\0';
 title15[0] = '\0';
 title10[0] = '\0';
 title05[0] = '\0';
 title00[0] = '\0';
 titlenone[0] = '\0';
 mor[0] = '\0';
 class[0] = '\0';

 exp += (int) ((exp_who - 1) * 100);
 exp += ((global_exp_mult - 1) * 100);
 stc("#0(#g.o#20#go.#0)#n-----------------------#0(#g.o#20#go.#0)     #6Vortex     #0(#g.o#20#go.#0)#n-----------------------#0(#g.o#20#go.#0)#n\n\r", ch);
 stc("#0:                                                                                      #0:#n\n\r", ch);
 stc("#0:       #7Rank         Ratio       G  S  U       Class       Player        Title         #0:#n\n\r", ch);
 stc("#0:                                                                                      #0:#n\n\r", ch);
 for( d = descriptor_list ; d != NULL; d = d->next)
 {
  if( !d->character || (d->character->name == NULL) ||
      (d->connected != CON_PLAYING))
   continue;
  if( !d->character->pcdata)
   continue;
  if( !can_see(ch,d->character) && (d->character->trust > 6))
   continue;
  sprintf( name, d->character->name);
  if( d->character->pkill == 0 )
   sprintf( ratio, "0.000");
  else
   sprintf( ratio, "%4.3f", ((float)d->character->pkill / ((float)d->character->pkill + (float)d->character->pdeath)) );
  if( !IS_NPC(d->character) && d->character->generation < 10 )
   sprintf(gen, "0%d", d->character->generation);
  else
   sprintf(gen, "%d", d->character->generation);
  if( !IS_NPC(d->character) && d->character->pcdata->souls < 10)
   sprintf(soul, "0%d", d->character->pcdata->souls);
  else
   sprintf(soul, "%d", d->character->pcdata->souls);

  blah = d->character->race;
  if( !IS_NPC(d->character) && d->character->race < 10)
   sprintf(stat, "0%d", blah);
  else
   sprintf(stat, "%d", blah);
  if( !IS_NPC(d->character) )
  {
   if( !str_cmp( d->character->pcdata->switchname, "Sluggo") )
    sprintf(class, "Sandworm");
   else if( !str_cmp( d->character->pcdata->switchname, "Kline") )
    sprintf(class, "Eternal");
   else if( !str_cmp( d->character->pcdata->switchname, "Loimos") )
    sprintf(class, "Enigma");
   else if( !str_cmp( d->character->pcdata->switchname, "Kale") )
    sprintf(class, "Master");
   else if( !str_cmp( d->character->pcdata->switchname, "Corruption") )
    sprintf(class, "Dark Elf");
   else if( !str_cmp( d->character->pcdata->switchname, "Vixen") )
    sprintf(class, "Kitten");
   else if( IS_CLASS(d->character, CLASS_NONE) )
    sprintf(class, "None");
   else if( IS_CLASS(d->character, CLASS_DRAGON) )
    sprintf(class, "Dragon");
   else if( IS_CLASS(d->character, CLASS_ALIEN) )
    sprintf(class, "Alien");
   else if( IS_CLASS(d->character, CLASS_APOCRYPHIC) )
    sprintf(class, "Apocryphic");
   else if( IS_CLASS(d->character, CLASS_MONK) )
    sprintf(class, "Monk");
   else if( IS_CLASS(d->character, CLASS_SHADOW) )
    sprintf(class, "Shadow");
   else if( IS_CLASS(d->character, CLASS_WOLF) )
    sprintf(class, "Werewolf");
   else if( IS_CLASS(d->character, CLASS_JEDI) )
    sprintf(class, "Jedi");
   else if( IS_CLASS(d->character, CLASS_VAMPIRE) )
    sprintf(class, "Vampire");
   else if( IS_CLASS(d->character, CLASS_DROW) )
    sprintf(class, "Drow");
   else
    sprintf(class, "Unknown");
  }
  if( d->character->level <= 2)
  {
    if( d->character->mkill < 5)
    {
     sprintf(status, "#3Newbie#n");
     holder = 1;
    }
    else if( (d->character->mkill >= 5) && (d->character->max_hit < 8000))
    {
     sprintf( status, "#2Mortal+#n");
     holder = 2;
    }
    else
    {
     sprintf( status, "#1Mortal++#n");
     holder = 3;
    }
   morfound = TRUE;
  }
  if( d->character->level >= 3 && d->character->level <= 6 )
  {
   if( blah == 0)
   {
    sprintf( status, config_info->title_none);
    avafound = TRUE;
    holder = 4;
   }
   else if( (blah >= 1) && (blah < 5))
   {
    sprintf( status, config_info->title00);
    avafound = TRUE;
    holder = 5;
   }
   else if( (blah >= 5) && (blah < 10))
   {
    sprintf( status, config_info->title05);
    avafound = TRUE;
    holder = 6;
   }
   else if( (blah >= 10) && (blah < 15))
   {
    sprintf( status, config_info->title10);
    avafound = TRUE;
    holder = 7;
   }
   else if( (blah >= 15) && (blah < 20))
   {
    sprintf( status, config_info->title15);
    avafound = TRUE;
    holder = 8;
   }
   else if( (blah >= 20) && (blah < 25))
   {
    sprintf( status, config_info->title20);
    avafound = TRUE;
    holder = 9;
   }
   else if( (blah >= 25) && (blah < 30))
   {
    sprintf( status, config_info->title25);
    avafound = TRUE;
    holder = 10;
   }
   else if( (blah >= 30) && (blah < 35))
   {
    sprintf( status, config_info->title30);
    avafound = TRUE;
    holder = 11;
   }
   else if( (blah >= 35) && (blah < 40))
   {
    sprintf( status, config_info->title35);
    avafound = TRUE;
    holder = 12;
   }
   else if( (blah >= 40) && (blah <45))
   {
    sprintf( status, config_info->title40);
    avafound = TRUE;
    holder = 13;
   }
   else if( (blah >= 45) && (blah < 50))
   {
    sprintf(status, config_info->title45);
    avafound = TRUE;
    holder = 14;
   }
   else if( blah >= 50 )
   {
    sprintf(status, config_info->title50);
    avafound = TRUE;
    holder = 15;
   }
  }
  if( d->character->level == 7)
  {
   sprintf( status, config_info->imm07);
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 16;
  }
  else if( d->character->level == 8)
  {
   sprintf( status, config_info->imm08);
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 17;
  }
  else if( d->character->level == 9)
  {
   sprintf( status, config_info->imm09);
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 18;
  }
  else if( d->character->level == 10)
  {
   sprintf( status, config_info->imm10);
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 19;
  }
  else if( d->character->level == 11)
  {
   sprintf( status, config_info->imm11);
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 20;
  }
  else if( d->character->level == 12)
  {
   sprintf( status, config_info->imm12);
   immfound = TRUE;
   sprintf(gen, "**");
   sprintf(stat,"**");
   sprintf(soul, "**");
   holder = 21;
  }
  if( !IS_NPC(d->character) )
  sprintf( title, d->character->pcdata->title);
  smash_color(title);
  sprintf( buffer, "#0:#n %16s #7%10s#n #0%8s #0%2s #0%2s#n #7%10s #7%12s%-21s#0  :#n\n\r",
      status,ratio,gen,stat,soul,class,name,title);
  switch(holder)
  {
   case 1:
   strcat(mor, buffer);
   break;
   case 2:
   strcat(mor, buffer);
   break;
   case 3:
   strcat(mor, buffer);
   break;
   case 4:
   strcat(titlenone, buffer);
   break;
   case 5:
   strcat(title00, buffer);
   break;
   case 6:
   strcat(title05, buffer);
   break;
   case 7:
   strcat(title10, buffer);
   break;
   case 8:
   strcat(title15, buffer);
   break;
   case 9:
   strcat(title20, buffer);
   break;
   case 10:
   strcat(title25, buffer);
   break;
   case 11:
   strcat(title30, buffer);
   break;
   case 12:
   strcat(title35, buffer);
   break;
   case 13:
   strcat(title40, buffer);
   break;
   case 14:
   strcat(title45, buffer);
   break;
   case 15:
   strcat(title50, buffer);
   break;
   case 16:
   strcat(imm07, buffer);
   break;
   case 17:
   strcat(imm08, buffer);
   break;
   case 18:
   strcat(imm09,buffer);
   break;
   case 19:
   strcat(imm10, buffer);
   break;
   case 20:
   strcat(imm11, buffer);
   break;
   case 21:
   strcat(imm12, buffer);
   break;
   default:
   break;
  }
  count++;
 }
 if(immfound)
 {
 stc("#0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)    #6Immortal    #0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)#n\n\r", ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 stc(imm12, ch);
 stc(imm11, ch);
 stc(imm10, ch);
 stc(imm09, ch);
 stc(imm08, ch);
 stc(imm07, ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 }
 if(avafound)
 {
 stc("#0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)     #6Avatar     #0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)#n\n\r", ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 stc(title50, ch);
 stc(title45, ch);
 stc(title40, ch);
 stc(title35, ch);
 stc(title30, ch);
 stc(title25, ch);
 stc(title20, ch);
 stc(title15, ch);
 stc(title10, ch);
 stc(title05, ch);
 stc(title00,ch);
 stc(titlenone, ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 }
 if(morfound)
 {
 stc("#0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)     #6Mortal     #0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)#n\n\r", ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 stc(mor, ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 }
 stc("#0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)      #6Info      #0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)#n\n\r", ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 stc("#0:                           #7[#0G#7]#0eneration   #7[#0S#7]#0tatus  So#7[#0u#7]#0ls                           #0:#n\n\r", ch);
 sprintf(buffer,"#0:     New Players This Reboot:   #7[#0%2d#7]               #0Old Players This Reboot:  #7[#0%2d#7]     #0:#n\n\r",new_players, old_players);
 stc(buffer, ch);
 sprintf(buffer,"#0:                                #0Most This Boot   #7[#0%3d#7]                                #0:\n\r", max_logged);
 stc(buffer, ch);
 sprintf(buffer,"#0:                                #0Experience Bonus #7[#0%3d#7]                                #0:#n\n\r",exp);
 stc(buffer, ch);
 sprintf(buffer,"#0:               #0Ap XP Bonus  #7[#0%3.2f#7]  #0Ap till next bonus #6[#0%7d#7]                      #0:#n\n\r" 
,config_info->ap_exp_mult, config_info->ap_exp_points);
 stc( buffer, ch);
 if( deathmatch )
  stc("#0:                                #0Deathmatch Mode  #7[#0 On#7]                                #0:#n\n\r",ch);
 else
  stc("#0:                                #0Deathmatch Mode  #7[#0Off#7]                                #0:#n\n\r",ch);
 stc("#0:                                                                                      :#n\n\r", ch);
 sprintf(buffer,
     "#0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)  #0Players #7[#0%2d#7]  #0(#g.o#20#go.#0)#n----------------------#0(#g.o#20#go.#0)#n\n\r", count);
 stc(buffer, ch);
 return;
}

void do_askills( CHAR_DATA *ch, char *argument )
{
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];

 int cost, i = 0;

 argument = one_argument(argument,arg);
 argument = one_argument(argument,arg2);
 argument = one_argument( argument, arg3);
 if( IS_NPC(ch))
  return;

 if( arg[0] == '\0' )
 {
  stc("(-------------------[Ability Skills]-------------------)\n\r", ch);
  stc("|                                                      |\n\r", ch);
  if( ASKILL(ch, AS_TOTAL) < 1 )
   sprintf(buf, "| You have no ability skills!                          |\n\r");
  else
   sprintf(buf, "| You have %2d ability skills!                          |\n\r", ASKILL(ch, AS_TOTAL));
   stc(buf,ch);
   sprintf(buf, "| Remaining Ability Points: [%7d]                  |\n\r", ch->pcdata->apoints);
   stc(buf,ch);
   stc("|                                                      |\n\r", ch);
  if( ASKILL(ch, AS_FINAL) > 0 )
  {
   cost = 1000;
   for( i = 0; i < ASKILL(ch,AS_FINAL); i++ )
    cost *= 1.05;
   if( ASKILL(ch,AS_FINAL) < ASKILL_MAX_LEV )
    sprintf(buf,"| Desperation  :-   Level [%3d]   Cost [%5d]         |\n\r",ASKILL(ch,AS_FINAL),cost);
   else
    sprintf(buf,"| Desperation  :-   Level [%3d]   Cost [*****]         |\n\r",ASKILL(ch,AS_FINAL));
   stc(buf,ch);
  }
  if( ASKILL(ch, AS_ATTACK) > 0 )
  {
   cost = 10000;
   for( i = 0; i < ASKILL(ch,AS_ATTACK); i++ )
    cost += 50;
   if( ASKILL(ch,AS_ATTACK) < ASKILL_MAX_LEV )
    sprintf(buf,"| Attack       :-   Level [%3d]   Cost [%5d]         |\n\r",ASKILL(ch,AS_ATTACK),cost);
   else
    sprintf(buf,"| Attack       :-   Level [%3d]   Cost [*****]         |\n\r",ASKILL(ch,AS_ATTACK));
   stc(buf,ch);
  }
  if( ASKILL(ch, AS_DEFENSE) > 0 )
  {
   cost = 10000;
   for( i = 0; i < ASKILL(ch,AS_DEFENSE); i++ )
    cost += 50;
   if( ASKILL(ch,AS_DEFENSE) < ASKILL_MAX_LEV )
    sprintf(buf,"| Defense      :-   Level [%3d]   Cost [%5d]         |\n\r",ASKILL(ch,AS_DEFENSE),cost);
   else
    sprintf(buf,"| Defense      :-   Level [%3d]   Cost [*****]         |\n\r",ASKILL(ch,AS_DEFENSE));
   stc(buf,ch);
  }
  if( IS_SET(ch->pcdata->askills[AS_BITS],ABIT_TRUESIGHT) || ASKILL(ch,AS_TRUESIGHT) > 0 )
    sprintf(buf,"| Truesight    :-   Level [***]   Cost [*****]         |\n\r");
  else
  {
   cost = 250000;
    sprintf(buf,"| Truesight    :-   Level [---]   Cost [%d]         |\n\r",cost);
  }
  stc(buf,ch);
  if( ASKILL(ch, AS_SWORDSMAN) > 0 )
  {
   cost = 25000;
   for( i = 0; i < ASKILL(ch,AS_SWORDSMAN); i++ )
    cost += 100;
   if( ASKILL(ch,AS_SWORDSMAN) < ASKILL_MAX_LEV )
    sprintf(buf,"| Swordsmanship:-   Level [%3d]   Cost [%5d]         |\n\r",ASKILL(ch,AS_SWORDSMAN),cost);
   else
    sprintf(buf,"| Swordsmanship:-   Level [%3d]   Cost [*****]         |\n\r",ASKILL(ch,AS_SWORDSMAN));
   stc(buf,ch);
  }
  if( ASKILL(ch, AS_STANCES) > 0 )
  {
   cost = 12000;
   for( i = 0; i < ASKILL(ch,AS_STANCES); i++ )
    cost += 100;
   if( ASKILL(ch,AS_STANCES) < ASKILL_MAX_LEV )
    sprintf(buf,"| Stance       :-   Level [%3d]   Cost [%5d]         |\n\r",ASKILL(ch,AS_STANCES),cost);
   else
    sprintf(buf,"| Stance       :-   Level [%3d]   Cost [*****]         |\n\r",ASKILL(ch,AS_STANCES));
   stc(buf,ch);
  }
  if( ASKILL(ch, AS_SPELLS) > 0 )
  {
   cost = 12000;
   for( i = 0; i < ASKILL(ch,AS_SPELLS); i++ )
    cost += 100;
   if( ASKILL(ch,AS_SPELLS) < ASKILL_MAX_LEV )
    sprintf(buf,"| Magic        :-   Level [%3d]   Cost [%5d]         |\n\r",ASKILL(ch,AS_SPELLS),cost);
   else
    sprintf(buf,"| Magic        :-   Level [%3d]   Cost [*****]         |\n\r",ASKILL(ch,AS_SPELLS));
   stc(buf,ch);
  }
  if( IS_SET(ch->pcdata->askills[AS_BITS],ABIT_READAURA) || ASKILL(ch,AS_READAURA) > 0 )
    sprintf(buf,"| Readaura     :-   Level [***]   Cost [*****]         |\n\r");
  else
  {
   cost = 500000;
    sprintf(buf,"| Readaura     :-   Level [---]   Cost [%d]         |\n\r",cost);
  }
  stc(buf,ch);
  stc("|                                                      |\n\r", ch);
  stc("(------------------------------------------------------)\n\r", ch);
  return;
 }
 if( !str_cmp(arg,"learn") )
 {
  if( arg2[0] == '\0' )
  {
   stc("Learn what?\n\r",ch);
   return;
  }
  if( !str_cmp(arg2,"experience"))
  {
   if( arg3[0] == '\0' || !is_number( arg3) || atoi(arg3) < 0)
   {
 	stc("You must supply a numeric value grater than zero.\n\r", ch);
	return;
   }
   i = atoi( arg3 );
   if( i > ch->pcdata->apoints)
   {
    	stc("You can't spend more than you have.\n\r", ch);
        return;
   }
   ch->pcdata->apoints -= i;
   config_info->ap_exp_points -= i;
   sprintf( buf, "%s has just contributed %d ap to the global exp multiplier.", ch->pcdata->switchname, i);
   do_info(ch, buf);
   while( config_info->ap_exp_points <= 0)
   {
	config_info->ap_exp_points += 5000000;
        config_info->ap_exp_mult += 0.01;
        sprintf( buf, "Global exp multiplier has gone up 1 percent thanks to %s.", ch->pcdata->switchname);
        do_info( ch, buf);
   }
   save_config();
   return;
  }
  if( !str_cmp(arg2,"desperation") )
  {
   askill_gain(ch,AS_FINAL);
   return;
  }
  if( !str_cmp(arg2,"attack") )
  {
   askill_gain(ch,AS_ATTACK);
   return;
  }
  if( !str_cmp(arg2,"defense") )
  {
   askill_gain(ch,AS_DEFENSE);
   return;
  }
  if( !str_cmp(arg2,"truesight") )
  {
   askill_gain(ch,AS_TRUESIGHT);
   return;
  }
  if( !str_cmp(arg2, "swordsmanship") )
  {
   askill_gain(ch,AS_SWORDSMAN);
   return;
  }
  if( !str_cmp(arg2, "stance") )
  {
   askill_gain(ch,AS_STANCES);
   return;
  }
  if( !str_cmp(arg2, "magic") )
  {
   askill_gain(ch,AS_SPELLS);
   return;
  }
  if( !str_cmp(arg2, "readaura") )
  {
   askill_gain(ch,AS_READAURA);
   return;
  }
  else
  {
   stc("Learn what?\n\r",ch);
   return;
  }
  return;
 }
 return;
}

void askill_gain( CHAR_DATA *ch, int field )
{
 int cost, i = 0;
 char buf[MAX_STRING_LENGTH];

 if( field == AS_FINAL )
 {
  cost = 1000;
  for( i = 0; i < ASKILL(ch,field); i++ )
   cost *= 1.05;
  if( ASKILL(ch,field) > 99 )
  {
   stc("You have already mastered the desperation technique.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( ASKILL(ch,field) < 1 )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_ATTACK )
 {
  cost = 10000;
  for( i = 0; i < ASKILL(ch,field); i++ )
   cost += 50;
  if( ASKILL(ch,field) > 99 )
  {
   stc("You have already mastered your attacking ability.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( ASKILL(ch,field) < 1 )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_DEFENSE )
 {
  cost = 10000;
  for( i = 0; i < ASKILL(ch,field); i++ )
   cost += 50;
  if( ASKILL(ch,field) > 99 )
  {
   stc("You have already mastered your defending ability.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( ASKILL(ch,field) < 1 )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_TRUESIGHT )
 {
  cost = 250000;
  if( IS_SET(ch->pcdata->askills[AS_BITS],ABIT_TRUESIGHT) )
  {
   stc("You already know how to expand your vision.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( !IS_SET(ch->pcdata->askills[AS_BITS],ABIT_TRUESIGHT) )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  SET_BIT(ch->pcdata->askills[AS_BITS],ABIT_TRUESIGHT);
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_SWORDSMAN )
 {
  cost = 25000;
  for( i = 0; i < ASKILL(ch,field); i++ )
   cost += 100;
  if( ASKILL(ch,field) > 99 )
  {
   stc("You have already mastered your swordsmanship ability.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( ASKILL(ch,field) < 1 )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_STANCES )
 {
  cost = 12000;
  for( i = 0; i < ASKILL(ch,field); i++ )
   cost += 100;
  if( ASKILL(ch,field) > 99 )
  {
   stc("You have already mastered your stancing ability.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( ASKILL(ch,field) < 1 )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_SPELLS )
 {
  cost = 12000;
  for( i = 0; i < ASKILL(ch,field); i++ )
   cost += 100;
  if( ASKILL(ch,field) > 99 )
  {
   stc("You have already mastered your magical abilities.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( ASKILL(ch,field) < 1 )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 if( field == AS_READAURA )
 { 
  cost = 500000;
  if( IS_SET(ch->pcdata->askills[AS_BITS],ABIT_READAURA) )
  {
   stc("You already know how to view another's aura.\n\r",ch);
   return;
  }
  if( ch->pcdata->apoints < cost )
  {
   sprintf(buf,"You lack the required %d ability points.\n\r",cost);
   stc(buf,ch);
   return;
  }
  if( !IS_SET(ch->pcdata->askills[AS_BITS],ABIT_READAURA) )
   ASKILL(ch,AS_TOTAL)++;
  ASKILL(ch,field)++;
  SET_BIT(ch->pcdata->askills[AS_BITS],ABIT_READAURA);
  ch->pcdata->apoints -= cost;
  stc("You further your knowledge.\n\r",ch);
  return;
 }
 return;
}

void send_newhelp_msg( CHAR_DATA *ch )
{
 int blah = number_range(1,20);

 switch(blah)
 {
  default:
   log_string("*** send_newhelp_msg returned NULL ***");
   break;
  case 1:
   stc("#0[#7NEWHELP#0] #7Two good places to start exping as a newbie are 2su4ed or 2su4n from recall.#n\n\r",ch);
   break;
  case 2:
   stc("#0[#7NEWHELP#0] #7You should first practice gas breath (prac gas) and then cast it while exping.#n\n\r",ch);
   break;
  case 3:
   stc("#0[#7NEWHELP#0] #7Be sure to read notes and help changes daily to keep ontop of the latest changes!#n\n\r",ch);
   break;
  case 4:
   stc("#0[#7NEWHELP#0] #7You need to be an avatar to use or learn any class powers unless you are an Apocryphic!#n\n\r",ch);
   break;
  case 5:
   stc("#0[#7NEWHELP#0] #7To save time, you can 'prac all' or 'sac all' instead of using them one at a time.#n\n\r",ch);
   break;
  case 6:
   stc("#0[#7NEWHELP#0] #7You can view some useful information about yourself with the commands score and level.#n\n\r",ch);
   break;
  case 7:
   stc("#0[#7NEWHELP#0] #7You can disable these messages at any time by typing newhelp or config -newhelp.#n\n\r",ch);
   break;
  case 8:
   stc("#0[#7NEWHELP#0] #7When ready to class, read help class and use the autoclass command.#n\n\r",ch);
   break;
  case 9:
   stc("#0[#7NEWHELP#0] #7Beware!  Once you train avatar you are no longer safe from dieing to anybody!#n\n\r",ch);
   break;
  case 10:
   stc("#0[#7NEWHELP#0] #7You can recieve information on certain things by typing help <subject>.#n\n\r",ch);
   break;
  case 11:
   stc("#0[#7NEWHELP#0] #7Seek out the forger 2 north of recall, and seek his knowledge in mining.#n\n\r",ch);
   break;
  case 12:
   stc("#0[#7NEWHELP#0] #7Gas breath becomes stronger as your hp increases.#n\n\r",ch);
   break;
  case 13:
   stc("#0[#7NEWHELP#0] #7If you need some help getting started in killing, try reading help pk.#n\n\r",ch);
   break;
  case 14:
   stc("#0[#7NEWHELP#0] #7Be sure to read notes and help changes daily to keep ontop of the latest changes!#n\n\r",ch);
   break;
  case 15:
   stc("#0[#7NEWHELP#0] #7As a mortal, you can stance and it won't come undone as you walk!#n\n\r",ch);
   break;
  case 16:
   stc("#0[#7NEWHELP#0] #7As a newbie, you recieve increased experience gains for the first 4 hours of your mudlife.#n\n\r",ch);
   break;
  case 17:
   stc("#0[#7NEWHELP#0] #7Seek out an old dingy scroll in midgaard.  It will help you in training weapons and stances!#n\n\r",ch);
   break;
  case 18:
   stc("#0[#7NEWHELP#0] #7Two good places to start exping as a newbie are 2su4ed or 2su4n from recall.#n\n\r",ch);
   break;
  case 19:
   stc("#0[#7NEWHELP#0] #7You can disable these messages at any time by typing newhelp or config -newhelp.#n\n\r",ch);
   break;
  case 20:
   stc("#0[#7NEWHELP#0] #7You can become a soul of the damned while avatar, allowing you to advance your class without fear of dieing!#n\n\r",ch);
   break;
 }
 return;
}

void do_apconv( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_STRING_LENGTH];

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;
 if( IS_DAMNED(ch) ) return;
 if( arg[0] == '\0' )
 {
  stc("How much exp would you like to convert?\n\r",ch);
  return;
 }
 if( !is_number(arg) )
 {
  stc("That's not a number!\n\r",ch);
  return;
 }
 if( ch->exp < atoi(arg) )
 {
  stc("You don't even have that much exp!\n\r",ch);
  return;
 }
 if( atoi(arg) < 1000 )
 {
  stc("1000 minimum.\n\r",ch);
  return;
 }
 ch->exp -= atoi(arg);
 ch->pcdata->apoints += (atoi(arg)/1000);
 stc("Exp converted successfully!\n\r",ch);
 return;
}

void do_regroup( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_SET(ch->act,PLR_NO_GROUP) )
 {
  stc("You're already a group fighter.\n\r",ch);
  return;
 }
 if( ch->pcdata->apoints < 100000 || ch->pcdata->quest < 10000 )
 {
  stc("You require 100,000 ability points and 10,000 quest points to change into a group fighter.\n\r",ch);
  return;
 }
 ch->pcdata->apoints -= 100000;
 ch->pcdata->quest -= 10000;
 REMOVE_BIT(ch->act,PLR_NO_GROUP);
 stc("Solo flag removed, you are free to group once more.\n\r",ch);
 return;
}

void do_newwho( CHAR_DATA *ch, char *argument)
{
 BUFFER *title40 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title35 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title30 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title25 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title20 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title15 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title10 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title05 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title00 = buffer_new( MAX_STRING_LENGTH);
 BUFFER *titlenone = buffer_new( MAX_STRING_LENGTH);
 BUFFER *imp     = buffer_new( MAX_STRING_LENGTH);
 BUFFER *hj      = buffer_new( MAX_STRING_LENGTH);
 BUFFER *judge   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *enf     = buffer_new( MAX_STRING_LENGTH);
 BUFFER *qm      = buffer_new( MAX_STRING_LENGTH);
 DESCRIPTOR_DATA *d;

 buffer_clear(imp);
 buffer_clear(hj);
 buffer_clear(judge);
 buffer_clear(enf);
 buffer_clear(qm);
 buffer_clear(title40);
 buffer_clear(title35);
 buffer_clear(title30);
 buffer_clear(title25);
 buffer_clear(title20);
 buffer_clear(title15);
 buffer_clear(title10);
 buffer_clear(title05);
 buffer_clear(title00);
 buffer_clear(titlenone);
 for( d = descriptor_list; d; d = d->next)
 {
  if( d->connected != CON_PLAYING)
   continue;
  if( d->character == NULL)
   continue;
  if( IS_NPC(d->character))
   continue;
  if( !can_see(ch, d->character))
   continue;
  if( IS_IMMORTAL(d->character))
  {
   switch( d->character->trust)
   {
    case LEVEL_IMPLEMENTOR:
     bprintf( imp, "Implementor  %10s\n\r", d->character->pcdata->switchname);
     break;
    case LEVEL_HIGHJUDGE:
     bprintf( hj,  "High Judge   %10s\n\r", d->character->pcdata->switchname);
     break;
    case LEVEL_JUDGE:
     bprintf(judge,"Judge        %10s\n\r", d->character->pcdata->switchname);
     break;
    case LEVEL_ENFORCER:
     bprintf( enf, "Enforcer     %10s\n\r",  d->character->pcdata->switchname);
     break;
    case LEVEL_QUESTMAKER:
     bprintf( qm,  "Quest Maker  %10s\n\r",  d->character->pcdata->switchname);
     break;
    default:
     bprintf( qm,  "Builder      %10s\n\r", d->character->pcdata->switchname);
     break;
   }
  }
  else
  {
   switch( d->character->race)
   {
    case 0:
     bprintf( titlenone,"%10s     %10s\n\r", config_info->title_none,
      d->character->pcdata->switchname);
     break;
    case 1:
    case 2:
    case 3:
    case 4:
     bprintf( title00, "%10s     %10s\n\r", config_info->title00,
      d->character->pcdata->switchname);
     break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
     bprintf( title05, "%10s     %10s\n\r", config_info->title05,
      d->character->pcdata->switchname);
     break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
     bprintf( title10, "%10s     %10s\n\r", config_info->title10,
      d->character->pcdata->switchname);
     break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
     bprintf( title15, "%10s     %10s\n\r", config_info->title15,
      d->character->pcdata->switchname);
     break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
     bprintf( title20, "%10s     %10s\n\r", config_info->title20,
      d->character->pcdata->switchname);
     break;
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
     bprintf( title25, "%10s     %10s\n\r", config_info->title25,
      d->character->pcdata->switchname);
     break;
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
     bprintf( title30, "%10s     %10s\n\r", config_info->title30,
      d->character->pcdata->switchname);
     break;
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
     bprintf( title35, "%10s     %10s\n\r", config_info->title35,
      d->character->pcdata->switchname);
     break;
    default:
     bprintf( title40, "%10s     %10s\n\r", config_info->title40,
      d->character->pcdata->switchname);
     break;
   }
  }
 }
 if( imp->data[0] != '\0')
   stc( imp->data, ch);
 if( hj->data[0] != '\0')
   stc( hj->data, ch);
 if( judge->data[0] != '\0')
   stc( judge->data, ch);
 if( enf->data[0] != '\0')
   stc( enf->data, ch);
 if( qm->data[0] != '\0')
   stc( qm->data, ch);
 if( title40->data[0] != '\0')
   stc( title40->data, ch);
 if( title35->data[0] != '\0')
   stc( title35->data, ch);
 if( title30->data[0] != '\0')
   stc( title30->data, ch);
 if( title25->data[0] != '\0')
   stc( title25->data, ch);
 if( title20->data[0] != '\0')
   stc( title20->data, ch);
 if( title15->data[0] != '\0')
   stc( title15->data, ch);
 if( title10->data[0] != '\0')
   stc( title10->data, ch);
 if( title05->data[0] != '\0')
   stc( title05->data, ch);
 if( title00->data[0] != '\0')
   stc( title00->data, ch);
 if( titlenone->data[0] != '\0')
   stc( titlenone->data, ch);
 buffer_free(imp);
 buffer_free(hj);
 buffer_free(judge);
 buffer_free(enf);
 buffer_free(qm);
 buffer_free(title40);
 buffer_free(title35);
 buffer_free(title30);
 buffer_free(title25);
 buffer_free(title20);
 buffer_free(title15);
 buffer_free(title10);
 buffer_free(title05);
 buffer_free(title00);
 buffer_free(titlenone);
 return;
}

void do_testwho( CHAR_DATA *ch, char *argument )
{
 char buf[MSL];
 char imm12[MSL];
 char imm11[MSL];
 char imm10[MSL];
 char imm09[MSL];
 char imm08[MSL];
 char imm07[MSL];
 char title40[MSL];
 char title35[MSL];
 char title30[MSL];
 char title25[MSL];
 char title20[MSL];
 char title15[MSL];
 char title10[MSL];
 char title05[MSL];
 char title00[MSL];
 char titlenone[MSL];
 char mor[MSL];
 char name[MSL];
 char clan[MSL];
 char status[MSL];
 char title[MSL];
 char gen[MSL];
 char soul[MSL];
 char stat[MSL];
 char class[MSL];
 char class_rank[MSL];
 char clan_rank[MSL];
 DESCRIPTOR_DATA *d;
 int count = 0;
 int exp = 0;
 int blah, holder;
 bool morfound = FALSE;
 bool avafound = FALSE;
 bool immfound = FALSE;

 exp += (int) ((exp_who - 1) * 100);
 exp += ((global_exp_mult - 1) * 100);
 buf[0] = '\0';
 imm12[0] = '\0';
 imm11[0] = '\0';
 imm10[0] = '\0';
 imm09[0] = '\0';
 imm08[0] = '\0';
 imm07[0] = '\0';
 title40[0] = '\0';
 title35[0] = '\0';
 title30[0] = '\0';
 title25[0] = '\0';
 title20[0] = '\0';
 title15[0] = '\0';
 title10[0] = '\0';
 title05[0] = '\0';
 title00[0] = '\0';
 titlenone[0] = '\0';
 mor[0] = '\0';
 name[0] = '\0';
 clan[0] = '\0';
 status[0] = '\0';
 title[0] = '\0';
 gen[0] = '\0';
 soul[0] = '\0';
 stat[0] = '\0';
 class[0] = '\0';
 sprintf(clan_rank,"ClanRank");
 sprintf(class_rank,"ClassRank");

 stc("()                                             Vortex                                             ()\n\r",ch);
 stc("()================================================================================================()\n\r",ch);
 stc("() Rank             Player  Title             G  S  U   Class     Class Rank  Clan      Clan Rank ()\n\r",ch);
 for( d = descriptor_list; d != NULL; d = d->next)
 {
  if( !d->character || (d->character->name == NULL) ||
      (d->connected != CON_PLAYING))
   continue;
  if( !d->character->pcdata )
   continue;
  if( !can_see(ch,d->character) )
   continue;
  sprintf(name,d->character->name);
  sprintf(title,d->character->pcdata->title);
  smash_color(title);
  if( IS_IMMORTAL(d->character) )
   sprintf(gen,"**");
  else
  {
   if( d->character->generation < 10 )
    sprintf(gen,"0%d",d->character->generation);
   else
    sprintf(gen,"%d",d->character->generation);
  }
  if( IS_IMMORTAL(d->character) )
   sprintf(stat,"**");
  else
  {
   if( d->character->race < 10 )
    sprintf(stat,"0%d",d->character->race);
   else
    sprintf(stat,"%d",d->character->race);
  }
  if( IS_IMMORTAL(d->character) )
   sprintf(soul,"**");
  else
  {
   if( d->character->pcdata->souls < 10 )
    sprintf(soul,"0%d",d->character->pcdata->souls);
   else
    sprintf(soul,"%d",d->character->pcdata->souls);
  }
  if( !str_cmp(d->character->pcdata->switchname,"Sluggo") )
   sprintf(class,"Sandworm");
  else if( !str_cmp(d->character->pcdata->switchname,"Kline") )
   sprintf(class,"Eternal");
  else if( !str_cmp(d->character->pcdata->switchname, "Loimos") )
   sprintf(class, "Enigma");
  else if( IS_CLASS(d->character, CLASS_NONE) )
   sprintf(class,"None");
  else if( IS_CLASS(d->character, CLASS_ALIEN) )
   sprintf(class,"Alien");
  else if( IS_CLASS(d->character, CLASS_APOCRYPHIC) )
   sprintf(class,"Apocryphic");
  else if( IS_CLASS(d->character, CLASS_CYBORG) )
   sprintf(class,"Cyborg");
  else if( IS_CLASS(d->character, CLASS_MONK) )
   sprintf(class,"Monk");
  else if( IS_CLASS(d->character, CLASS_SHADOW) )
   sprintf(class,"Shadow");
  else if( IS_CLASS(d->character, CLASS_WOLF) )
   sprintf(class,"Werewolf");
  else if( IS_CLASS(d->character, CLASS_JEDI) )
   sprintf(class,"Jedi");
  else if( IS_CLASS(d->character, CLASS_VAMPIRE) )
   sprintf(class,"Vampire");
  else
   sprintf(class,"Unknown");
  if((d->character->clan == NULL) || strlen(d->character->clan) < 5)
   sprintf(clan,"None");
  else
   sprintf(clan,d->character->clan);
  if( d->character->level <= 2)
  {
   if( d->character->mkill < 5)
    sprintf(status,"#3Newbie#n");
   else if( (d->character->mkill >= 5) && (d->character->max_hit < 8000))
    sprintf(status,"#2Mortal+#n");
   else
    sprintf(status,"#1Mortal++#n");
   holder = 1;
   morfound = TRUE;
  }
  if( d->character->level >= 3 && d->character->level <= 6 )
  {
   blah = d->character->race;
   if( blah == 0)
   {
    sprintf(status,config_info->title_none);
    holder = 2;
    avafound = TRUE;
   }
   else if( (blah >= 1) && (blah < 5))
   {
    sprintf(status,config_info->title00);
    holder = 3;
    avafound = TRUE;
   }
   else if( (blah >= 5) && (blah < 10))
   {
    sprintf(status,config_info->title05);
    holder = 4;
    avafound = TRUE;
   }
   else if( (blah >= 10) && (blah < 15))
   {
    sprintf(status,config_info->title10);
    holder = 5;
    avafound = TRUE;
   }
   else if( (blah >= 15) && (blah < 20))
   {
    sprintf(status,config_info->title15);
    holder = 6;
    avafound = TRUE;
   }
   else if( (blah >= 20) && (blah < 25))
   {
    sprintf(status,config_info->title20);
    holder = 7;
    avafound = TRUE;
   }
   else if( (blah >= 25) && (blah < 30))
   {
    sprintf(status,config_info->title25);
    holder = 8;
    avafound = TRUE;
   }
   else if( (blah >= 30) && (blah < 35))
   {
    sprintf(status,config_info->title30);
    holder = 9;
    avafound = TRUE;
   }
   else if( (blah >= 35) && (blah < 40))
   {
    sprintf(status,config_info->title35);
    holder = 10;
    avafound = TRUE;
   }
   else if( (blah >= 40))
   {
    sprintf(status,config_info->title40);
    holder = 11;
    avafound = TRUE;
   }
  }
  if( d->character->level == 7)
  {
   sprintf(status,config_info->imm07);
   holder = 12;
   immfound = TRUE;
  }
  else if( d->character->level == 8)
  {
   sprintf(status,config_info->imm08);
   holder = 13;
   immfound = TRUE;
  }
  else if( d->character->level == 9)
  {
   sprintf(status,config_info->imm09);
   holder = 14;
   immfound = TRUE;
  }
  else if( d->character->level == 10)
  {
   sprintf(status,config_info->imm10);
   holder = 15;
   immfound = TRUE;
  }
  else if( d->character->level == 11)
  {
   sprintf(status,config_info->imm11);
   holder = 16;
   immfound = TRUE;
  }
  else if( d->character->level == 12)
  {
   sprintf(status,config_info->imm12);
   holder = 17;
   immfound = TRUE;
  }
  count++;
  sprintf(buf,"   %-16s %10s %-18s %2s %2s %2s  %-10s %-10s %-10s %-10s\n\r"
   ,status,name,title,gen,stat,soul,class,class_rank,clan,clan_rank);
  switch( holder )
  {
   case 1:  strcat(mor,buf);       break;
   case 2:  strcat(titlenone,buf); break;
   case 3:  strcat(title00,buf);   break;
   case 4:  strcat(title05,buf);   break;
   case 5:  strcat(title10,buf);   break;
   case 6:  strcat(title15,buf);   break;
   case 7:  strcat(title20,buf);   break;
   case 8:  strcat(title25,buf);   break;
   case 9:  strcat(title30,buf);   break;
   case 10: strcat(title35,buf);   break;
   case 11: strcat(title40,buf);   break;
   case 12: strcat(imm07,buf);     break;
   case 13: strcat(imm08,buf);     break;
   case 14: strcat(imm09,buf);     break;
   case 15: strcat(imm10,buf);     break;
   case 16: strcat(imm11,buf);     break;
   case 17: strcat(imm12,buf);     break;
  }
 }
 if( immfound )
 {
  stc("()IMMORTALS()\n\r",ch);
  stc(imm12,ch);
  stc(imm11,ch);
  stc(imm10,ch);
  stc(imm09,ch);
  stc(imm08,ch);
  stc(imm07,ch);
 }
 if( avafound )
 {
  stc("() AVATARS ()\n\r",ch);
  stc(title40,ch);
  stc(title35,ch);
  stc(title30,ch);
  stc(title25,ch);
  stc(title20,ch);
  stc(title15,ch);
  stc(title10,ch);
  stc(title05,ch);
  stc(title00,ch);
  stc(titlenone,ch);
 }
 if( morfound )
 {
  stc("() MORTALS ()\n\r",ch);
  stc(mor,ch);
 }
 stc("()================================================================================================()\n\r",ch);
 stc("    [G]eneration    New Players This Reboot: [10]    Deathmatch Mode  [Off]    Players     [ 7]\n\r",ch);
 stc("    [S]tatus        Old Players This Reboot: [30]    Experience Bonus [ 14]    Top Players [14]\n\r",ch);
 stc("    So[u]ls         Kill Attempts           [100]    Valid Kills      [ 34]    Avg Players [10]\n\r",ch);
 return;
}

void do_testscore( CHAR_DATA *ch, char *argument )
{
 char name[MAX_STRING_LENGTH];
 char title[MAX_STRING_LENGTH];
 char age[MAX_STRING_LENGTH];
 char class[MAX_STRING_LENGTH];
 char buf[MAX_STRING_LENGTH];
 int gen, hours, chp, mhp, cmp, mmp, cmv, mmv;
 int cstr, cint, cwis, cdex, ccon, xp, ap, qp, pr, citem, cweight, mitem, mweight;
 int hr, dr, ac, dc, pk, pd, mk, md, ak, ad, mstr, mint, mwis, mdex, mcon;
 int stasis, shadowplane, status, soul, rage, attack;
 int prat = 0.00;
 int mrat = 0.00;
 int arat = 0.00;
 name[0] = '\0';
 title[0] = '\0';
 age[0] = '\0';
 class[0] = '\0';
 buf[0] = '\0';

 sprintf(name,"%s",ch->pcdata->switchname); sprintf(title,"%s",get_status_name(ch));
/* sprintf(age,"%s",birth_date(ch,TRUE));*/     sprintf(class,"%s",get_class_name(ch));
 gen     = ch->generation;         hours  = get_hours_played(ch);
 chp     = ch->hit;                mhp    = ch->max_hit;
 cmp     = ch->mana;               mmp    = ch->max_mana;
 cmv     = ch->move;               mmv    = ch->max_move;
 cstr    = get_curr_str(ch);       cint   = get_curr_int(ch);
 cwis    = get_curr_wis(ch);       cdex   = get_curr_dex(ch);
 ccon    = get_curr_con(ch);       xp     = ch->exp;
 ap      = ch->pcdata->apoints;    qp     = ch->pcdata->quest;
 pr      = ch->practice;           citem  = ch->carry_number;
 cweight = ch->carry_weight;       mitem  = can_carry_n(ch);
 mweight = can_carry_w(ch);        hr     = class_hitroll(ch);
 dr      = class_damroll(ch);      pk     = ch->pkill;
 pd      = ch->pdeath;             mk     = ch->mkill;
 md      = ch->mdeath;             ak     = ch->pcdata->awins;
 ad      = ch->pcdata->alosses;    status = ch->race;
 soul    = ch->pcdata->souls;      rage   = ch->rage;
 attack  = class_num_attack(ch);   ac     = class_ac(ch);
 mstr    = ch->pcdata->perm_str;   mint   = ch->pcdata->perm_int;
 mwis    = ch->pcdata->perm_wis;   mdex   = ch->pcdata->perm_dex;
 mcon    = ch->pcdata->perm_con;
 stasis  = ch->pcdata->pulse_timer[PULSE_STASIS];
 shadowplane = ch->pcdata->pulse_timer[PULSE_SHADOWPLANE];

 switch( pk+pd )
 {
  default:  prat = ( (float)(pk/(pk+pd)*100) );  break;
  case 0:   prat = 0.00;  break;
 }
 switch( mk+md )
 {
  default:  mrat = ( (float)(mk/(mk+md)*100) );  break;
  case 0:   mrat = 0.00;  break;
 }
 switch( ak+ad )
 {
  default:  arat = ( (float)(ak/(ak+ad)*100) );  break;
  case 0:   arat = 0.00;  break;
 }

 if( IS_NPC(ch) )
  return;
 stc("()---[Information]\n\r",ch);
 sprintf(buf,"| You are %s, a %s %s.\n\r",name,title,class);
 stc(buf,ch);
 sprintf(buf,"| You are <age>, and have been playing for %d.\n\r",hours);
 stc(buf,ch);
 stc("()---[Vitals]\n\r",ch);
 sprintf(buf,"| Health: %d[%d]  Str: %d[%d]  XP: %d\n\r",chp,mhp,cstr,mstr,xp);
 stc(buf,ch);
 sprintf(buf,"| Mana:   %d[%d]  Int: %d[%d]  AP: %d\n\r",cmp,mmp,cint,mint,ap);
 stc(buf,ch);
 sprintf(buf,"| Move:   %d[%d]  Wis: %d[%d]  QP: %d\n\r",cmv,mmv,cwis,mwis,qp);
 stc(buf,ch);
 sprintf(buf,"| Items:  %d[%d]      Dex: %d[%d]  PR: %d\n\r",citem,mitem,cdex,mdex,pr);
 stc(buf,ch);
 sprintf(buf,"| Weight: %d[%d]    Con: %d[%d]\n\r",cweight,mweight,ccon,mcon);
 stc(buf,ch);
 stc("()---[Combat]\n\r",ch);
 sprintf(buf,"| HR: %d  AC: %d  PK: %d  MK: %d  AK: %d\n\r",hr,ac,pk,mk,ak);
 stc(buf,ch);
 sprintf(buf,"| DR: %d  DC: %d   PD: %d   MD: %d      AD: %d\n\r",dr,dc,pd,md,ad);
 stc(buf,ch);
 stc("()---[Misc]\n\r",ch);
 sprintf(buf,"| Stasis:      %d pulses  Status: %d     Souls: %d\n\r",stasis,status,soul);
 stc(buf,ch);
 sprintf(buf,"| Shadowplane: %d pulses  Generation: %d  Rage: %d\n\r",shadowplane,gen,rage);
 stc(buf,ch);
 sprintf(buf,"| Attacks:     %d\n\r",attack);
 stc(buf,ch);
 stc("()---[Ratio]\n\r",ch);
 sprintf(buf,"| PK: %d  MK: %d  AK: %d\n\r",prat,mrat,arat);
 stc(buf,ch);
 stc("()\n\r",ch);
 return;
}

void do_morph_check( CHAR_DATA *ch, char *argument )
{
 char buf[MAX_STRING_LENGTH];
 stc("Checking ch->morph...\n\r",ch);
 sprintf(buf,"Length is %d.\n\r",strlen(ch->morph));
 stc(buf,ch);
 if( strlen(ch->morph) > 0 )
 {
  sprintf(buf,"Content is %s.\n\r",ch->morph);
  stc(buf,ch);
 }
 else
  stc("Content is NULL.\n\r",ch);
 return;
}

void do_dpconv( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) && !IS_CLASS(ch,CLASS_VAMPIRE) && !IS_CLASS(ch,CLASS_DRAGON) && !IS_CLASS(ch,CLASS_ALIEN) )
 {
  stc("Your class has no points to convert!\n\r",ch);
  return;
 }
 switch( ch->class )
 {
  default:  break;
  case CLASS_WOLF:
   ch->exp += (GET_POWERS(ch,WOLF_POINTS) * 5);
   GET_POWERS(ch,WOLF_POINTS) = 0;
   break;
  case CLASS_VAMPIRE:
   ch->exp += (GET_POWERS(ch,VAMP_POINTS) * 5);
   GET_POWERS(ch,VAMP_POINTS) = 0;
   break;
  case CLASS_DRAGON:
   ch->exp += (GET_POWERS(ch,DRAGON_POINTS) * 5);
   GET_POWERS(ch,DRAGON_POINTS) = 0;
   break;
  case CLASS_ALIEN:
   ch->exp += (GET_POWERS(ch,ALIEN_DNA) * 5);
   GET_POWERS(ch,ALIEN_DNA) = 0;
   break;
 }
 stc("All class points have been converted successfully!\n\r",ch);
 return;
}
/*
void assign_class_name( CHAR_DATA *ch )
{

   if( !IS_NPC(ch) )
   {
    if( !str_cmp(ch->pcdata->switchname, "Sluggo") )
	ch->class->class_name = str_dup("Sandworm");
    else if( !str_cmp(ch->pcdata->switchname, "Kline") )
	ch->class->class_name = str_dup("Eternal");
    else if( !str_cmp(ch->pcdata->switchname, "Loimos") )
	ch->class->class_name = str_dup("Enigma");
    else if( IS_CLASS(ch, CLASS_NONE) )
	ch->class->class_name = str_dup("None");
    else if( IS_CLASS(ch, CLASS_DRAGON) )
	ch->class->class_name = str_dup("Dragon");
    else if( IS_CLASS(ch, CLASS_ALIEN) )
	ch->class->class_name = str_dup("Alien");
    else if( IS_CLASS(ch, CLASS_APOCRYPHIC) )
	ch->class->class_name = str_dup("Apocryphic");
    else if( IS_CLASS(ch, CLASS_CYBORG) )
	ch->class->class_name = str_dup("Cyborg");
    else if( IS_CLASS(ch, CLASS_MONK) )
	ch->class->class_name = str_dup("Monk");
    else if( IS_CLASS(ch, CLASS_SHADOW) )
	ch->class->class_name = str_dup("Shadow");
    else if( IS_CLASS(ch, CLASS_WOLF) )
	ch->class->class_name = str_dup("Werewolf");
    else if( IS_CLASS(ch, CLASS_JEDI) )
	ch->class->class_name = str_dup("Jedi");
    else if( IS_CLASS(ch, CLASS_VAMPIRE) )
	ch->class->class_name = str_dup("Vampire");
    else
	ch->class->class_name = str_dup("Unknown");
   }

   return;
}
*/

void set_switchname( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_switchname: NPC.", 0 );
	return;
    }
    strcpy( buf, title );
    free_string( ch->pcdata->switchname );
    ch->pcdata->switchname = str_dup( buf );
    return;
}

void set_pc_name( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_pc_name: NPC.", 0 );
	return;
    }
    strcpy( buf, title );
    free_string( ch->name);
    ch->name = str_dup( buf );
    return;
}

void do_fixpoly( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_SET(ch->affected_by,AFF_POLYMORPH) )
 {
  free_string(ch->morph);
  ch->morph = strdup("");
 }
 else if( IS_SET(ch->affected_by,AFF_POLYMORPH) )
 {
  REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
  free_string(ch->morph);
  ch->morph = strdup("");
 }
 return;
}

void do_readaura( CHAR_DATA *ch, char *argument )
{
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 if( IS_NPC(ch) )
  return;
 argument = one_argument(argument,arg);

 if( !IS_SET(ch->pcdata->askills[AS_BITS],ABIT_READAURA) && ASKILL(ch,AS_READAURA) < 1 )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Whom do you wish to do this to?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg)) == NULL )  
 {
  stc("They're not here.\n\r",ch);
  return;
 }

 act("$n intently examines $N...",ch,NULL,victim,TO_NOTVICT);
 act("You intently examine $N...",ch,NULL,victim,TO_CHAR);
 act("$n intently examines you...",ch,NULL,victim,TO_VICT);

 if( IS_NPC(victim) )
  sprintf(buf,"Ac: [%5d]         Dr: [%5d]         Hr: [%5d]\n\r",victim->armor,victim->damroll,victim->hitroll);
 else if( !IS_NPC(victim) )
  sprintf(buf,"Ac: [%5d]         Dr: [%5d]         Hr: [%5d]\n\r",class_ac(victim),class_damroll(victim),class_hitroll(victim));
 stc(buf,ch);
 sprintf(buf,"Hp: [%5d/%5d]   Mp: [%5d/%5d]   Mv: [%5d/%5d]\n\r",
  victim->hit,victim->max_hit,victim->mana,victim->max_mana,victim->move,victim->max_move);
 stc(buf,ch);

 return;
}

void do_beep( CHAR_DATA *ch, char *argument )
{
   int pos, i;
   char arg1[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;

   one_argument( argument, arg1 );
   if ( IS_NPC(ch) )
    return;

   if ( arg1[0] =='\0' )
   {
      stc( "Usage: BEEP <victim>\n\r", ch );
      return;
   }

   if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
   {
      stc( "Couldn't find the victim.\n\r", ch );
      return;
   }
   
   if ( IS_NPC( victim ) )
   {
      stc( "Not on NPCs.\n\r", ch );
      return;
   }
      
   if ( ch == victim )
   {
      stc( "Beep yourself?  Sure, ok... beep! beep! beep!\n\r", ch );
      return;
   }
   if( !str_cmp(victim->pcdata->switchname,"Kline") )
   {
    stc("Foolish mortal.  Now you suffer.\n\r",ch);
    for( i = 0; i < 10; i++ )
     stc("\a",ch);
    return;
   }   
   pos = victim->position;
   victim->position = POS_STANDING;
      
   act( "You beep $N...",         ch, NULL, victim, TO_CHAR );
   act( "\a$n is beeping you...", ch, NULL, victim, TO_VICT );
   
   victim->position = pos;
   return;
}

void do_gocial( CHAR_DATA *ch, char *argument )
{
   char command[MAX_INPUT_LENGTH];
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   int cmd;
   const char GOCIAL_TAG[] = "#4[#6>>#7-#1GOCIAL#7-#6<<#4]#n  ";
   int (*handle)();

   argument = one_argument(argument, command);
   argument = one_argument(argument, arg);

   if(sociallib == NULL)
    sociallib = dlopen(SOCIAL_LIB,RTLD_NOW | RTLD_GLOBAL);

   if( (handle = dlsym(sociallib,"get_social")) == NULL )
    log_string(dlerror());
   if( (cmd = ((*handle)(command))) == -1 )
   {
	stc("What is it you're trying to do?\n\r", ch);
	return;
   }

   switch (ch->position)
   {
	case POS_DEAD:
	   stc("You can't do that, you're dead!\n\r", ch);
	   return;
	case POS_INCAP:
	case POS_MORTAL:
	   stc("You are hurt far too bad for that.\n\r", ch);
	   return;
	case POS_STUNNED:
	   stc("You cannot do that while stunned.\n\r", ch);
	   return;
	case POS_SLEEPING:
	   if( (*handle)("snore") )
		break;
	   stc("In your dreams, or what?\n\r", ch);
	   return;
   }


   if( arg[0] == '\0' )
   {
	act(social_table[cmd]->others_no_arg, ch, NULL, victim, GOCIAL);
	stc(GOCIAL_TAG, ch);
	act(social_table[cmd]->char_no_arg, ch, NULL, victim, TO_CHAR);
	return;
   }
   else if( (victim = get_char_world(ch, arg)) == NULL )
   {
   	stc("They aren't here.\n\r", ch);
	return;
   }

   else if( victim == ch )
   {
	act(social_table[cmd]->others_auto, ch, NULL, victim, GOCIAL);
	stc(GOCIAL_TAG, ch);
	act(social_table[cmd]->char_auto, ch, NULL, victim, TO_CHAR);
	return;
   }
   else
   {
	stc(GOCIAL_TAG, ch);
	act(social_table[cmd]->char_found, ch, NULL, victim, TO_CHAR);
	stc("\n\r", victim);
	stc(GOCIAL_TAG, victim);
	act(social_table[cmd]->vict_found, ch, NULL, victim, TO_VICT);
	act(social_table[cmd]->others_found, ch, NULL, victim, GOCIAL);
	return;
   }

   return;

}

void do_display_social( CHAR_DATA *ch, char *argument )
{
 void (*handle)();
    
 if( (handle = dlsym(sociallib,"display_social")) == NULL )
 {  
  stc("An error has occurred and been logged, please inform Kline.\n\r",ch);
  log_string(dlerror());
  return;
 }  
 else
  (*handle)(ch,argument);
 return;
} 

void do_save_socials( CHAR_DATA *ch, char *argument )
{
 void (*handle)();

 if( (handle = dlsym(sociallib,"save_socials")) == NULL )
 {
  stc("An error has occurred and been logged, please inform Kline.\n\r",ch);
  log_string(dlerror());
  return;
 }
 else
  (*handle)(ch,argument);
 return;
}
void update_players_logged(void)
{

   DESCRIPTOR_DATA *d;
   int players_found = 0;

   for( d = descriptor_list; d != NULL; d = d->next )
   {
	if( !d->character 
	|| (d->character->name == NULL) 
	|| (d->connected != CON_PLAYING) )
	   continue;
	else
	   players_found++;
   }

   max_logged = players_found > max_logged ? players_found : max_logged; 

}
