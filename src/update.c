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



/*
 * Local functions.
 */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void    room_update     args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void    embrace_update  args((void));
void	send_newhelp_msg args((CHAR_DATA *ch));

void gain_exp( CHAR_DATA *ch, int gain )
{
    if ( IS_NPC(ch)) return;
    ch->exp += gain;  
	if (ch->explevel < 1)
	ch->explevel =1 ;


    gain = gain/ch->explevel/2;

    ch->levelexp = UMAX( 1000, ch->levelexp + gain );
    while ( ch->explevel < 100 && ch->levelexp >= 100000 * (ch->explevel+1))
    {
		ch->explevel += 1;
    }

    while (ch->explevel > 1 && ch->levelexp < 100000 * (ch->explevel - 1) )
    {
		ch->explevel -=1;
    } 
    save_char_obj(ch);

    return;
}




/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int conamount;

    if( ch->hit >= ch->max_hit )
     return 0;

    if ( IS_NPC(ch) )
    {
     if( ch->hit < 0)
       ch->hit = 1;
	gain = ch->level;
    }
    else
    {

	gain = number_range( 50, 75 );

	if ((conamount = (get_curr_con(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: 
		gain *= conamount * 1.5;	break;
		case POS_SLEEPING:   gain *= conamount * 1.75;	break;
		case POS_RESTING:    gain *= conamount * 1.5;	break;
	    }
	}

    }
    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int intamount;

    if( ch->mana >= ch->max_mana )
     return 0;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = number_range( 50, 150 );

	if ((intamount = (get_curr_int(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING: gain *= intamount * 3; break;
		case POS_SLEEPING:   gain *= intamount * 5;			break;
		case POS_RESTING:    gain *= intamount * 3;		break;
	    }
	}

    }
/*    if( IS_SET(ch->aff2, AFF2_REGEN))
      gain *= 5;
    if( ch->stasis)
      gain *= 5;*/
    return UMIN( gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;
    int dexamount;

    if( ch->move >= ch->max_move )
     return 0;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = number_range(50, 150 );

	if ((dexamount = (get_curr_dex(ch)+1)) > 1)
	{
	    switch ( ch->position )
	    {
		case POS_MEDITATING:
		gain *= dexamount * 3;
		break;
		case POS_SLEEPING:   gain *= dexamount * 5;		break;
		case POS_RESTING:    gain *= dexamount * 3;	break;
	    }
	}

    }
    /*if( IS_SET(ch->aff2, AFF2_REGEN))
    gain *= 5;
    if( ch->stasis)
    gain *= 5;*/
    return UMIN(gain, ch->max_move - ch->move);
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;

    if ( value == 0 || IS_NPC(ch) )
	return;

    if (!IS_NPC(ch) && IS_HERO(ch) 
	&& iCond != COND_DRUNK)
	return;

    condition				= ch->pcdata->condition[iCond];
    if (!IS_NPC(ch) )
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 48 );
    else
	ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, 100 );

    /*if ( ch->pcdata->condition[iCond] == 0 )
    {
	switch ( iCond )
	{
         default:
           break;
	}
    }
    else if ( ch->pcdata->condition[iCond] < 10 )
    {
	switch ( iCond )
	{
	case COND_FULL:
		send_to_char( "You feel hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
		send_to_char( "You feel thirsty.\n\r", ch );
	    break;
	}
    }
*/
    return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{	
	CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;
    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
		ch_next = ch->next;

		if ( ch->in_room == NULL )
			continue;

		if ( ch->hunting != NULL && ch->hunting != '\0' && strlen(ch->hunting) > 1 )
		{
			check_hunt( ch );
			continue;
		}

		if ( !IS_NPC(ch) )
		{
			if (ch->pcdata->condition[COND_DRUNK] > 10 && number_range(1,10) == 1)
			{
				send_to_char("You hiccup loudly.\n\r",ch);
				act("$n hiccups.",ch,NULL,NULL,TO_ROOM);
			}
			if (!IS_NPC(ch) && (ch->level > 2) && ch->pcdata->condition[COND_DRUNK] > 0)
				ch->pcdata->condition[COND_DRUNK] = 0;
			if (ch->level < 3)
			{
				mortal_regen(ch);
			}
			if (!IS_NPC(ch) && (ch->level > 2) && (ch->class == 0))
			{
				if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || ch->move < ch->max_move )
					werewolf_regen(ch);
			}
			else if (IS_ITEMAFF(ch, ITEMA_REGENERATE))
			{
			if ( ch->hit < ch->max_hit || ch->mana < ch->max_mana || 
				ch->move < ch->max_move )
				werewolf_regen(ch);
			if (ch->loc_hp[6] > 0)
			{
				int sn = skill_lookup( "clot" );
				(*skill_table[sn].spell_fun) (sn,ch->level,ch,ch);
			}
			else
			{
				if ((ch->loc_hp[0] + ch->loc_hp[1] + ch->loc_hp[2] +
				ch->loc_hp[3] + ch->loc_hp[4] + ch->loc_hp[5]) != 0)
				reg_mend(ch);
			}
	    }
	    continue;
	}

	if ( IS_AFFECTED(ch, AFF_CHARM) ) continue;

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	    if (ch == NULL) continue;
	}

	/* That's all for sleeping / busy monster */
	if ( ch->position != POS_STANDING )
	    continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 2 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && obj->cost > max )
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n picks $p up.", ch, obj_best, NULL, TO_ROOM );
		act( "You pick $p up.", ch, obj_best, NULL, TO_CHAR );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL)
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)
	&& ( ch->hunting == NULL || strlen(ch->hunting) < 2 )
	&& ( (!IS_SET(ch->act, ACT_STAY_AREA) && ch->level < 900)
	||   pexit->to_room->area == ch->in_room->area ) )
	{
	    move_char( ch, door );
	}

	/* Flee */
	if ( ch->hit < ch->max_hit / 4
	&& ( door = number_bits( 3 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_AFFECTED(ch, AFF_WEBBED)
	&&   ch->level < 350
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB) 
	&&   !IS_AFFECTED(ch, AFF_TENDRILS))
        {
	    CHAR_DATA *rch;
	    bool found;

	    found = FALSE;
	    for ( rch  = pexit->to_room->people;
		  rch != NULL;
		  rch  = rch->next_in_room )
	    {
		if ( !IS_NPC(rch) )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if ( !found )
		move_char( ch, door );
	}

    }

    return;
}



/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch = NULL;
    int diff;
    bool char_up;

    buf[0] = '\0';

    switch ( ++time_info.hour )
    {
		case  6:
		weather_info.sunlight = SUN_RISE;
		strcat( buf, "The sun rises in the east.\n\r" );
                for ( d = descriptor_list; d != NULL; d = d->next )
                {
                        char_up = FALSE;
                        if (( d->connected == CON_PLAYING
                        ||    d->connected == CON_EDITING )
                        &&   (ch = d->character) != NULL
                        &&  !IS_NPC(ch))
                        {
                                if( IS_CLASS(ch,CLASS_VAMPIRE) )
                                {
                                 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_POTENCE) )
                                 {
                                  REM_BIT_POWERS(ch,VAMP_BITS,BIT_POTENCE);
                                  ch->generation++;
                                 }
                                 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLASHING) )
                                 {
                                  REM_BIT_POWERS(ch,VAMP_BITS,BIT_FLASHING);
                                  GET_POWERS(ch,DISC_CELERITY)--;
                                 }
                                 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BAAL) )
                                 {
                                  REM_BIT_POWERS(ch,VAMP_BITS,BIT_BAAL);
                                  GET_POWERS(ch,DISC_CELERITY)--;
                                  GET_POWERS(ch,DISC_POTENCE)--;
                                  GET_POWERS(ch,DISC_FORTITUDE)--;
                                 }
                                 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_TIDE) )
                                  REM_BIT_POWERS(ch,VAMP_BITS,BIT_TIDE);
                                 stc("You feel your blood powers fade away with the coming of day.\n\r",ch);
                                }
			}
		}
		break;

		case  7:
		weather_info.sunlight = SUN_LIGHT;
		strcat( buf, "The day has begun.\n\r" );
		break;

		case 18:
		weather_info.sunlight = SUN_SET;
		strcat( buf, "The sun slowly disappears in the west.\n\r" );
		break;

		case 19:
			weather_info.sunlight = SUN_DARK;
		strcat( buf, "The night has begun.\n\r" );
		break;

		case 24:
		time_info.hour = 0;
		time_info.day++;
		for ( d = descriptor_list; d != NULL; d = d->next )
		{
			char_up = FALSE;
			if (( d->connected == CON_PLAYING
			||    d->connected == CON_EDITING )
			&&   (ch = d->character) != NULL
			&&  !IS_NPC(ch))
			{
				send_to_char( "You hear a clock in the distance strike midnight.\n\r",ch);
				ch->pcdata->pulse_timer[PULSE_STASIS] = 60;
                                ch->pcdata->pulse_timer[PULSE_SHADOWPLANE] = 60;
		                ch->pcdata->pulse_timer[PULSE_DARKNESS] = 60;
                                if (IS_SET(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS)
                                && !IS_SET(ch->newbits,NEW_DARKNESS))
                                {
                                        send_to_char("The darkness shrouding the room dissipates.\n\r",ch);
                                        act("The darkness shrouding the room dissipates.",ch,NULL,NULL,TO_ROOM);
                                        REMOVE_BIT(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS);
                                }
				if (IS_SET(ch->in_room->room_flags, ROOM_SILENCE))
				{
					send_to_char("The silence leaves the room.\n\r",ch);
					act("The silence leaves the room.",ch,NULL,NULL,TO_ROOM);
					REMOVE_BIT(ch->in_room->room_flags,ROOM_SILENCE);                
				}
				if (IS_SET(ch->in_room->room_flags,ROOM_FLAMING))
				{
					send_to_char("The flames in the room die down.\n\r",ch);
					act("The flames in the room die down.",ch,NULL,NULL,TO_ROOM);
					REMOVE_BIT(ch->in_room->room_flags,ROOM_FLAMING);
				}
 				if (IS_EXTRA(ch, EXTRA_ROT))
				{
						send_to_char("Your flesh feels better.\n\r", ch);
						REMOVE_BIT(ch->extra, EXTRA_ROT);
                    
				}

		} 
                 
			
		}
		break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default: 
	bug( "Weather_update: bad sky %d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS:
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The sky is getting cloudy.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "It starts to rain.\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "The clouds disappear.\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "Lightning flashes in the sky.\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The rain stopped.\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "The lightning has stopped.\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if (( d->connected == CON_PLAYING
	    ||    d->connected == CON_EDITING )
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) )
		send_to_char( buf, d->character );
	}
    }

    return;
}


int is_wall(EXIT_DATA *ex)
{

    if (IS_SET(ex->exit_info, EX_ICE_WALL)) return 1;
    if (IS_SET(ex->exit_info, EX_FIRE_WALL)) return 2;
    if (IS_SET(ex->exit_info, EX_SWORD_WALL)) return 3;
    if (IS_SET(ex->exit_info, EX_PRISMATIC_WALL)) return 4;
    if (IS_SET(ex->exit_info, EX_IRON_WALL)) return 5;
    if (IS_SET(ex->exit_info, EX_MUSHROOM_WALL)) return 6;
    if (IS_SET(ex->exit_info, EX_CALTROP_WALL)) return 7;
    if (IS_SET(ex->exit_info, EX_ASH_WALL)) return 8;
    if (IS_SET(ex->exit_info, EX_ENERGYNET_WALL)) return 9;
    return 0;
}

const char * wall[MAX_WALL+1] = 
{
    "","wall of ice", "wall of fire", "wall of swords",
	"prismatic wall", "wall or iron", "wall of mushrooms",
	"wall of caltrops", "wall of ash", "wall of energy"
};

/*
 * Update all rooooooms, like gore, poison clouds etc....
 */
void room_update( void )
{
    int i;
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *room;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( room = room_list; room != NULL; room = room->next_room)
    {
      for ( paf = room->affected; paf != NULL; paf = paf_next )
      {
       CHAR_DATA *rch;

       paf_next    = paf->next;
       if( paf->duration > 0 )
        paf->duration--;
       else if ( paf->duration < 0 )
        continue;
       else
       {       
        char *msg_off = NULL;
        if( ( paf->type > 0 ) && ( paf->type < MAX_SKILL ) )
        {
         if ( paf_next == NULL )
          msg_off = skill_table[paf->type].msg_off;
         else if ( (paf_next->type != paf->type) || (paf_next->duration>0))
          msg_off = skill_table[paf->type].msg_off;
        }
        else if ( paf->type >= MAX_SKILL )
        {
         bug("Invalid paf->type (%d)", paf->type);
        }
        if ( msg_off != NULL )
        {
        if( paf->type > 0 && skill_table[paf->type].msg_off)
        {
         for( rch = room->people; rch != NULL; rch = rch->next_in_room)
         { 
          send_to_char( skill_table[paf->type].msg_off, rch);
          send_to_char( "\n\r", rch );
         }
        }
      }
      room_affect_remove( room, paf );
      if( paf->type == skill_lookup("dart_trap") || paf->type == skill_lookup("poison_trap")
       || paf->type == skill_lookup("explosion_trap") || paf->type == skill_lookup("sleep_trap") )
       room->trapped_by = NULL;
      if( paf->type == skill_lookup("drain_rune") )
       room->rune_by = NULL;
     }
   }
        if (RTIMER(room, RTIMER_WALL_NORTH) == 1
	    && room->exit[DIR_NORTH] != NULL
	    && is_wall(room->exit[DIR_NORTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_NORTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_NORTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_SOUTH) == 1
	    && room->exit[DIR_SOUTH] != NULL
	    && is_wall(room->exit[DIR_SOUTH])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_SOUTH])]);
	    room_message(room, buf);
	    make_wall(room, DIR_SOUTH, 0);
        }

        if (RTIMER(room, RTIMER_WALL_EAST) == 1
	    && room->exit[DIR_EAST] != NULL
	    && is_wall(room->exit[DIR_EAST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_EAST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_EAST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_WEST) == 1
	    && room->exit[DIR_WEST] != NULL
	    && is_wall(room->exit[DIR_WEST])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_WEST])]);
	    room_message(room, buf);
	    make_wall(room, DIR_WEST, 0);
        }

        if (RTIMER(room, RTIMER_WALL_UP) == 1
	    && room->exit[DIR_UP] != NULL
	    && is_wall(room->exit[DIR_UP])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_UP])]);
	    room_message(room, buf);
	    make_wall(room, DIR_UP, 0);
        }

        if (RTIMER(room, RTIMER_WALL_DOWN) == 1
	    && room->exit[DIR_DOWN] != NULL
	    && is_wall(room->exit[DIR_DOWN])!= 0)
        {
	    sprintf(buf, "The %s slowly fades away.", wall[is_wall(room->exit[DIR_DOWN])]);
	    room_message(room, buf);
	    make_wall(room, DIR_DOWN, 0);
        }

      if (RTIMER(room, RTIMER_SILENCE) == 1)
	    room_message(room, "The silence disappates.");

	for (i = 0 ; i < MAX_RTIMER ; i++)
	    room->tick_timer[i] = UMAX(room->tick_timer[i] - 1, 0);

    }

    return;

}



/*
 * Update all chars, including mobs.
 * This function is performance sensitive.
 */
void char_update( void )
{   
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_save;
    CHAR_DATA *ch_quit;
    bool is_obj;
    bool drop_out = FALSE;
    time_t save_time;

    save_time	= current_time;
    ch_save	= NULL;
    ch_quit	= NULL;
    config_who_exp();
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	int i;

	ch_next = ch->next;

	if (!IS_NPC(ch) && (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH)))
	    is_obj = TRUE;
	else if (!IS_NPC(ch) && ch->pcdata->obj_vnum != 0)
	    {is_obj = TRUE;SET_BIT(ch->extra, EXTRA_OSWITCH);}
	else
	    is_obj = FALSE;
	/*
	 * Find dude with oldest save time.
	 */
	if ( !IS_NPC(ch)
	&& ( ch->desc == NULL || ch->desc->connected == CON_PLAYING )
	&&   ch->level >= 2
	&&   ch->save_time < save_time )
	{
	    ch_save	= ch;
	    save_time	= ch->save_time;
	}

	if (ch->fighting == NULL && !IS_NPC(ch)) 

	if( ch->hit > ch->max_hit )
	 ch->hit = ch->max_hit;
	if( ch->mana > ch->max_mana )
	 ch->mana = ch->max_mana;
	if( ch->move > ch->max_move )
	 ch->move = ch->max_move;
        if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_JEDI) && GET_POWERS(ch,JEDI_RANK) < JEDI_RANK_LORD )
        {
         if( number_percent() < ((GET_POWERS(ch,JEDI_ATTACK) + GET_POWERS(ch,JEDI_DEFENSE) + GET_POWERS(ch,JEDI_STANCE))/2) )
         {
          stc("You feel yourself become more in tune with the force.\n\r",ch);
          INC_POWERS(ch,JEDI_RANK);
         }
        }
        if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ALIEN)) 
        {
          if( (GET_POWERS(ch, ALIEN_SIZE) < 10) && number_percent() == 5)
          {
           stc("You have grown in size.\n\r", ch);
           INC_POWERS(ch, ALIEN_SIZE);
          }
          if( GET_POWERS(ch, ALIEN_CLAWS) > 0)
          {
           if( GET_POWERS(ch, ALIEN_CLAWS) == 1)
            stc("Your claws have grown dull and fallen out.\n\r", ch);
           DEC_POWERS(ch, ALIEN_CLAWS);
          }
          if( GET_POWERS(ch, ALIEN_TAIL) > 0)
          {
           if( GET_POWERS(ch, ALIEN_TAIL) == 1)
            stc("Your tail grows brittle and falls off.\n\r", ch);
           DEC_POWERS(ch, ALIEN_TAIL);
          }
        }
	if( !IS_NPC(ch) && IS_SET(ch->act,PLR_NEW_HELP) )
	 send_newhelp_msg(ch);
	/* Character Tick Timers */

     	if( ch->tick_timer[TIMER_CAN_SUMMON] == 1)
            stc("You can summon the powers that be again.\n\r", ch);
        if( ch->tick_timer[TIMER_TASER] == 1)
            send_to_char("You can use your taser again.\n\r", ch);
        if( ch->tick_timer[TIMER_CAN_GUST] == 1)
            send_to_char("You can breathe a gust of wind again.\n\r", ch);

	for (i = 0; i < MAX_TIMER; i++)
	    if (ch->tick_timer[i] > 0) ch->tick_timer[i] -= 1;

	if ( ch->position == POS_STUNNED && !is_obj)
	{
            ch->hit = ch->hit + number_range(2,4);
	    update_pos( ch );

	}

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL && !is_obj)
	{
	    OBJ_DATA *obj;

	    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    || ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 ) )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    --ch->in_room->light;
		    act( "$p goes out.", ch, obj, NULL, TO_ROOM );
		    act( "$p goes out.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	    }

	    if ( ++ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		}
	    }

	    if ( ch->timer > 30 ) ch_quit = ch;

	    gain_condition( ch, COND_DRUNK,  -1 );
	    	gain_condition( ch, COND_FULL, -1 );
	    	gain_condition( ch, COND_THIRST, -1 );
	}

        for( paf = ch->affected; paf != NULL; paf = paf_next )
        { 
          paf_next    = paf->next;
          if( paf->duration > 0 )
            paf->duration--; 
          else if( paf->duration < 0 )
            continue;
          else
          {
           char *msg_off = NULL;
           if( ( paf->type > 0 ) && ( paf->type < MAX_SKILL ) )
           {
            if( paf_next == NULL )
             msg_off = skill_table[paf->type].msg_off; 
            else if ( (paf_next->type != paf->type) || (paf_next->duration > 0) )
             msg_off = skill_table[paf->type].msg_off;
           }
           else if( paf->type >= MAX_SKILL )
            bug("Invalid paf->type (%d)", paf->type);
           if( msg_off != NULL )
           {
            if( paf->type > 0 && skill_table[paf->type].msg_off && !is_obj)
            {   
             send_to_char( skill_table[paf->type].msg_off, ch );
             send_to_char( "\n\r", ch );
            }
           }
           if( paf->level == 0)
            affect_remove( ch, paf );
           else if( paf->level == 1)
            affect_remove2( ch, paf, 1);
           else
            affect_remove(ch, paf);
          }
        }
 
	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */
	if ( ch->loc_hp[6] > 0 && !is_obj && ch->in_room != NULL )
	{
	    int dam = 0;
	    int minhit = 0;
	    if (!IS_NPC(ch)) minhit = -11;
	    if (IS_BLEEDING(ch,BLEEDING_HEAD) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's neck.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your neck.\n\r", ch );
		dam += number_range(20,50);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_THROAT) && (ch->hit-dam) > minhit )
	    {
		act( "Blood pours from the slash in $n's throat.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "Blood pours from the slash in your throat.\n\r", ch );
		dam += number_range(10,20);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_ARM_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right arm.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right arm.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_HAND_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right wrist.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right wrist.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_L) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's left ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your left ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_BLEEDING(ch,BLEEDING_LEG_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right leg.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right leg.\n\r", ch );
		dam += number_range(10,20);
	    }
	    else if (IS_BLEEDING(ch,BLEEDING_FOOT_R) && (ch->hit-dam) > minhit )
	    {
		act( "A spray of blood shoots from the stump of $n's right ankle.", ch, NULL, NULL, TO_ROOM );
		send_to_char( "A spray of blood shoots from the stump of your right ankle.\n\r", ch );
		dam += number_range(5,10);
	    }
	    if (IS_HERO(ch)) { ch->hit = ch->hit - dam;
		if (ch->hit < 1) ch->hit = 1; }
	    else ch->hit = ch->hit - dam;
	    update_pos(ch);
	    ch->in_room->blood += dam;
	    if (ch->in_room->blood > 1000) ch->in_room->blood = 1000;
	    if (ch->hit <=-11 || (IS_NPC(ch) && ch->hit < 1))
	    {
		do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }
	}
	if (IS_EXTRA(ch,EXTRA_ROT) && !is_obj && !drop_out)
	{
	 int dam;
	 if (IS_NPC(ch)) break;
         act( "$n's flesh shrivels and tears.", ch, NULL, NULL, TO_ROOM);
	 send_to_char( "Your flesh shrivels and tears.\n\r", ch);
         dam = number_range(250,500);
         ch->hit = ch->hit - dam;
         update_pos(ch);
         if (ch->hit < -10)
         {
            do_killperson(ch,ch->name);
            drop_out = TRUE;
         }
        }


     if ( IS_AFFECTED(ch, AFF_FLAMING) && !is_obj && !drop_out && ch->in_room != NULL )
	{
	    int dam;
	    if (IS_NPC(ch)) break;
	    act( "$n's flesh burns and crisps.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "Your flesh burns and crisps.\n\r", ch );
	    dam = number_range(500,750);
	    ch->hit = ch->hit - dam;
	    update_pos(ch);
	    if (ch->hit <=-9)
	    {
		update_pos(ch);
		drop_out = TRUE;
	    }
	}
	else if ( IS_AFFECTED(ch, AFF_POISON) && !is_obj && !drop_out )
	{
	    act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You shiver and suffer.\n\r", ch );
	    damage( ch, ch, 2, gsn_poison );
	    if( IS_NPC(ch) && ch->hit <= 0 )
		raw_kill(ch);
	}
	else if ( ch->position == POS_INCAP && !is_obj && !drop_out )
	{
	    if (ch->level > 0)
                ch->hit = ch->hit + number_range(2,4);
	    else
                ch->hit = ch->hit - number_range(1,2);
	    update_pos( ch );
            if (ch->position > POS_INCAP)
            {
                act( "$n's wounds stop bleeding and seal up.\n\r", ch, NULL, NULL, TO_ROOM );
                send_to_char( "Your wounds stop bleeding and seal up.\n\r", ch );
            }
            if (ch->position > POS_STUNNED)
            {
                act( "$n clambers back to $s feet.\n\r", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You clamber back to your feet.\n\r", ch );
            }
	}
	else if ( ch->position == POS_MORTAL && !is_obj && !drop_out )
	{
	    drop_out = FALSE;
	   /* if (ch->level > 0)*/
                ch->hit = ch->hit + number_range(2,4);
/*	    else
	    {
                ch->hit = ch->hit - number_range(1,2);
		if (!IS_NPC(ch) && (ch->hit <=-11))
		    do_killperson(ch,ch->name);
		drop_out = TRUE;
	    }*/
	    if (!drop_out)
	    {
	    	update_pos( ch );
            	if (ch->position == POS_INCAP)
            	{
                    act( "$n's wounds begin to close, and $s bones pop back into place.", ch, NULL, NULL, TO_ROOM );
                    send_to_char( "Your wounds begin to close, and your bones pop back into place.\n\r", ch );
            	}
	    }
	}
	else if ( ch->position == POS_DEAD && !is_obj && !drop_out )
	{
	   update_pos(ch);
	   if (!IS_NPC(ch))
		do_killperson(ch,ch->name);
	}
	drop_out = FALSE;
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
    if ( ch_save != NULL || ch_quit != NULL )
    {
	for ( ch = char_list; ch != NULL; ch = ch_next )
	{
	    ch_next = ch->next;
	    if ( ch == ch_save )
		save_char_obj( ch );
	    if ( ch == ch_quit )
		do_quit( ch, "" );
	}
    }

    return;
}



/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;
	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

	switch ( obj->item_type )
	{
	default:              message = "$p vanishes.";         	break;
	case ITEM_FOUNTAIN:   message = "$p dries up.";         	break;
	case ITEM_CORPSE_NPC: message = "$p decays into dust."; 	break;
	case ITEM_CORPSE_PC:  message = "$p decays into dust."; 	break;
	case ITEM_FOOD:       message = "$p decomposes.";		break;
	case ITEM_TRASH:      message = "$p crumbles into dust.";	break;
	case ITEM_EGG:        message = "$p cracks open.";		break;
	case ITEM_WEAPON:     message = "$p turns to fine dust and blows away."; break;
        case ITEM_WALL:       message = "$p flows back into the ground.";break;
        case ITEM_BOMB:       message = "$p is now inactive.";          break;
	}

	if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    act( message, rch, obj, NULL, TO_ROOM );
	    act( message, rch, obj, NULL, TO_CHAR );
	}

	/* If the item is an egg, we need to create a mob and shell!
	 * KaVir
	 */
	if ( obj->item_type == ITEM_EGG)
	{
	    CHAR_DATA      *creature;
	    OBJ_DATA       *egg;
	    if ( get_mob_index( obj->value[0] ) != NULL )
	    {
		if (obj->carried_by != NULL && obj->carried_by->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->carried_by->in_room);
		}
		else if (obj->in_room != NULL)
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,obj->in_room);
		}
		else
		{
	    	    creature = create_mobile( get_mob_index( obj->value[0] ) );
		    char_to_room(creature,get_room_index(ROOM_VNUM_HELL));
		}
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, creature->in_room );
	    	act( "$n clambers out of $p.", creature, obj, NULL, TO_ROOM );
	    }
	    else if (obj->in_room != NULL)
	    {
	    	egg = create_object( get_obj_index( OBJ_VNUM_EMPTY_EGG ), 0 );
	    	egg->timer = 2;
	    	obj_to_room( egg, obj->in_room );
	    }
	}

	if (obj != NULL) extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't want the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim = NULL;

    OBJ_DATA *obj = NULL;
    OBJ_DATA *chobj = NULL;
    ROOM_INDEX_DATA *objroom = NULL;
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
/*
	if ( d->connected == CON_PLAYING
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata->stage[1] > 0
	&& ( victim = ch->pcdata->partner ) != NULL
	&&   !IS_NPC(victim)
	&&   ch->in_room != NULL
	&&   victim->in_room != NULL
	&&   victim->in_room != ch->in_room )
	{
	    ch->pcdata->stage[1] = 0;
	    victim->pcdata->stage[1] = 0;
	}
*/
	if (( d->connected == CON_PLAYING
	||    d->connected == CON_EDITING )
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&& ( obj = ch->pcdata->chobj ) != NULL )
	{
	    if (obj->in_room != NULL)
	    	objroom = obj->in_room;
	    else if (obj->in_obj != NULL)
	    	objroom = get_room_index(ROOM_VNUM_IN_OBJECT);
	    else if (obj->carried_by != NULL)
	    {
		if (obj->carried_by != ch && obj->carried_by->in_room != NULL)
		    objroom = obj->carried_by->in_room;
		else continue;
	    }
	    else continue;
	    if (ch->in_room != objroom && objroom != NULL)
	    {
	    	char_from_room(ch);
	    	char_to_room(ch,objroom);
		do_look(ch,"auto");
	    }
	}
	else if (( d->connected == CON_PLAYING
	||         d->connected == CON_EDITING )
	&& ( ch = d->character ) != NULL
	&&   !IS_NPC(ch)
	&&   ch->pcdata != NULL
	&&   (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0) )
	{
	    if (ch->pcdata->obj_vnum != 0)
	    {
		bind_char(ch);
		continue;
	    }
	    if (IS_HEAD(ch,LOST_HEAD))
	    {
	    	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);
	    	send_to_char("You are able to regain a body.\n\r",ch);
	    	ch->position = POS_RESTING;
	    	ch->hit = 1;
	    } else {
	    	send_to_char("You return to your body.\n\r",ch);
		REMOVE_BIT(ch->extra,EXTRA_OSWITCH);}
	    REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
	    free_string(ch->morph);
	    ch->morph = str_dup("");
	    char_from_room(ch);
	    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));
	    if ( ( chobj = ch->pcdata->chobj ) != NULL )
		chobj->chobj = NULL;
	    ch->pcdata->chobj = NULL;
	    do_look(ch,"auto");
	}
	continue;
    }

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;
	if ( IS_NPC(wch)
	|| ( wch->desc != NULL && wch->desc->connected != CON_PLAYING && wch->desc->connected != CON_EDITING )
	||   wch->position <= POS_STUNNED
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->pcdata == NULL
	|| ( ( chobj = wch->pcdata->chobj ) != NULL )
	||   wch->in_room == NULL )
	    continue;

	if (!IS_NPC(wch) && IS_CLASS(wch, CLASS_SHADOW) )
	{
	    if ( wch->hunting != NULL && wch->hunting != '\0' && 
		strlen(wch->hunting) > 1 && GET_POWERS(wch, SHADOW_TRACK) > 1)
	    {
		ROOM_INDEX_DATA *old_room = wch->in_room;
		check_hunt( wch );
		if (wch->in_room == old_room)
		{
		    free_string(wch->hunting);
		    wch->hunting = str_dup( "" );
		    continue;
		}
		continue;
	    }
            
	    if ( wch->hunting != NULL && wch->hunting != '\0' &&
                strlen(wch->hunting) > 1 && GET_POWERS(wch, SHADOW_TRACK) > 3)
            {
                ROOM_INDEX_DATA *old_room = wch->in_room;
                check_hunt( wch );
                if (wch->in_room == old_room)
                {
                    free_string(wch->hunting);
                    wch->hunting = str_dup( "" );
                    continue;
                }
		continue;
	    }
	}

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   !IS_SET(ch->act, ACT_AGGRESSIVE)
	    ||   no_attack(ch, wch)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch ) )
		continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   !no_attack(ch, vch)
		&&   vch->pcdata != NULL
		&& ( ( chobj = vch->pcdata->chobj ) == NULL )
		&&   vch->level < LEVEL_IMMORTAL
		&&   vch->position > POS_STUNNED
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch ) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
	    {
/*
		bug( "Aggr_update: null victim attempt by mob %d.", ch->pIndexData->vnum );
*/
		continue;
	    }

	    multi_hit( ch, victim, TYPE_UNDEFINED );
	}
    }

    return;
}


 

/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */
void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 );
	area_update	( );
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }
    if ( --pulse_point    <= 0 )
    {
	pulse_point     = number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 );
	weather_update	( );
	char_update	( );
	obj_update	( );
        room_update	( );
    }

    aggr_update( );
    tail_chain( );
    return;
}
