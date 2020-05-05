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
#include "clantable.h"



void horn args( ( CHAR_DATA *ch ) );


char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );
void    take_key        args( ( CHAR_DATA *ch, int key) );  
int	count_imms	args( ( CHAR_DATA *ch ) );
bool	check_track	args( ( CHAR_DATA *ch, int direction ) );
void	add_tracks	args( ( CHAR_DATA *ch, int direction ) );
void	show_page	args( ( CHAR_DATA *ch, OBJ_DATA *book, int pnum, bool pagefalse ) );
void	show_runes	args( ( CHAR_DATA *ch, OBJ_DATA *page, bool endline ) );
bool	are_runes	args( ( OBJ_DATA *page ) );
int     get_primal_cost args( (int primal, int value) );

void do_convert( CHAR_DATA *ch, char *argument)
{
 int chance = number_range(1,100);
 OBJ_INDEX_DATA *pObjIndex;
 OBJ_DATA *obj;
 char buf[MAX_STRING_LENGTH];

 if( IS_NPC(ch))
  return;
 if( ch->pcdata->quest < 100)
 {
  stc("You need at least 100 qps.\n\r", ch);
  return;
 }
 if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
   {
    log_string( "Error...missing object, please inform Kale.\n\r");
    return;
   }
   obj = create_object( pObjIndex, 25 );

 if( chance < 50)
  make_gem(obj);
 else
  make_ore(obj);
 obj_to_char(obj, ch);
 sprintf(buf, "You have create %s.\n\r", obj->short_descr);
 stc(buf, ch);
 ch->pcdata->quest -= 100;
 return;
}

void move_char( CHAR_DATA *ch, int door )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    CHAR_DATA *mount;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    DESCRIPTOR_DATA *d;
    char buf  [MAX_STRING_LENGTH];
    char poly [MAX_STRING_LENGTH];
    char mount2 [MAX_INPUT_LENGTH];
    char leave [20];
    int revdoor;

    if ( door < 0 || door > 5 )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->to_room      ) == NULL )
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
	return;
    }
    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    &&   !IS_AFFECTED(ch, AFF_PASS_DOOR)
    &&   !IS_AFFECTED(ch, AFF_ETHEREAL)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE) )
    {
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	    return;
    }
    if( IS_SET( pexit->exit_info, EX_CLOSED)
    && !IS_IMMORTAL(ch)
    &&  (to_room->vnum >= 26000
     &&  to_room->vnum <= 26206))
    {
     act( "You must have the key to enter here.", ch, NULL, NULL, TO_CHAR);
     return;
    }
    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "What?  And leave your beloved master?\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && (mount=ch->mount) != NULL && IS_SET(ch->mounted,IS_MOUNT))
    {
	send_to_char( "You better wait for instructions from your rider.\n\r", ch );
	return;
    }

    if ( room_is_private( to_room ))
    {
	if ( IS_NPC( ch ) || ch->trust < LEVEL_HIGHJUDGE)
	{
		send_to_char( "That room is private right now.\n\r", ch );
		return;
	}
	else 
		send_to_char( "That room is private (Access granted).\n\r", ch);
    }

    if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_LEG)) &&
	 (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_LEG)) &&
	 (IS_ARM_L(ch,BROKEN_ARM) || IS_ARM_L(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_HOLD) != NULL) &&
	 (IS_ARM_R(ch,BROKEN_ARM) || IS_ARM_R(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	send_to_char( "You need at least one free arm to drag yourself with.\n\r", ch );
	return;
    }
    else if ( IS_BODY(ch,BROKEN_SPINE) &&
	 (IS_ARM_L(ch,BROKEN_ARM) || IS_ARM_L(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_HOLD) != NULL) &&
	 (IS_ARM_R(ch,BROKEN_ARM) || IS_ARM_R(ch,LOST_ARM) ||
	    get_eq_char(ch, WEAR_WIELD) != NULL))
    {
	send_to_char( "You cannot move with a broken spine.\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) )
    {
	int move;
	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR )
	{
	    if( (!IS_AFFECTED(ch, AFF_FLYING) && !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY) )
	    && ((mount=ch->mount) != NULL && IS_SET(ch->mounted,IS_RIDING)) 
            && IS_AFFECTED(mount, AFF_FLYING))
	    {
		send_to_char( "You can't fly.\n\r", ch );
		return;
	    }
	}

	if ( in_room->sector_type == SECT_WATER_NOSWIM
	||   to_room->sector_type == SECT_WATER_NOSWIM )
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;
	    if ( IS_AFFECTED(ch, AFF_FLYING) || IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY) )
		found = TRUE;
	    if ( !found )
	    {
	    	for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    	{
		    if ( obj->item_type == ITEM_BOAT )
		    {
		    	found = TRUE;
		    	break;
		    }
	    	}
	    	if ( !found )
	    	{
		    send_to_char( "You need a boat to go there.\n\r", ch );
		    return;
	    	}
        	else if ( (!IS_AFFECTED(ch, AFF_FLYING) && !IS_SET(GET_POWERS(ch, DRAGON_FLAGS), DRAGON_FLY)) )
        	{
            		bool from_ok = FALSE;
            		bool to_ok = FALSE;
            		if ( in_room->sector_type == SECT_WATER_NOSWIM ) from_ok = TRUE;
            		if ( in_room->sector_type == SECT_WATER_SWIM   ) from_ok = TRUE;
            		if ( to_room->sector_type == SECT_WATER_NOSWIM ) to_ok   = TRUE;
            		if ( to_room->sector_type == SECT_WATER_SWIM   ) to_ok   = TRUE;
            		if ( !from_ok || !to_ok )
           		{
               		 send_to_char("You either need a boat or have to be flying.\n\r",ch);
                	 return;   
            		}
        	}
	    }
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

	if (IS_HERO(ch)) move = 0;

	if(ch->move <= 0)
	{
		send_to_char("You are too Exhausted.\n\r",ch);
		return;
	}

	if (IS_SET(ch->mounted, IS_RIDING) && (ch->move < move || ch->move < 1))
	{
	    send_to_char( "You are too exhausted.\n\r", ch );
	    return;
	}

        if (IS_SET(pexit->exit_info, EX_IRON_WALL)
	&& !IS_AFFECTED(ch, AFF_PASS_DOOR) )
    {
	send_to_char("A towering wall of iron blocks your path.\n\r", ch);
	act("$n's path is blocked by the wall of iron.", ch, NULL, NULL, TO_ROOM);
	return;
    }

    if (IS_SET(pexit->exit_info, EX_MUSHROOM_WALL))
    {
	send_to_char("The mushrooms block your path.\n\r", ch);
	return;
    }

    if (IS_SET(pexit->exit_info, EX_FIRE_WALL)
	&& ch->class == 0)
    {
	act("$n bursts through the wall of fire.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You jump through the flames and are unaffected.\n\r", ch);
    }

    else if (IS_SET(pexit->exit_info, EX_FIRE_WALL)
	&& ch->class != 0 && !IS_NPC(ch) && ch->level > 2)
    {
	act("$n bursts through the wall of fire.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You jump through the flames.\n\r", ch);
	ch->hit -= dice(15 , 50);
      stc("The flames sear your flesh.\n\r",ch);
    }

    if (IS_SET(pexit->exit_info, EX_SWORD_WALL)
	&& ch->class == 0)
    {
	act("$n bursts through the wall of swords.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You jump through the swords and are unaffected.\n\r", ch);
    }

    else if (IS_SET(pexit->exit_info, EX_SWORD_WALL)
	&& ch->class != 0 && !IS_NPC(ch) && ch->level > 2)
    {
	act("$n jumps through the wall of swords.", ch, NULL, NULL, TO_ROOM);
	send_to_char("You jump through the swords.\n\r", ch);
	send_to_char("Aaaaaaaaarghhhhhhh! That hurt!\n\r", ch);
	ch->hit -= dice(6, 70);
    }

    if (IS_SET(pexit->exit_info, EX_ASH_WALL) )
    {
	send_to_char("You scream in agony as the wall of ash rips apart your life force.\n\r", ch);
	act("$n screams in agony as the wall of ash rips $s life force apart.", ch, NULL, NULL, TO_ROOM);
	ch->hit /= 2;
	ch->move /= 2;
    }

    if (IS_SET(pexit->exit_info, EX_ENERGYNET_WALL)
       && ( ch->level > 2 && !IS_CLASS(ch, CLASS_CYBORG) )  )
    {
	stc("A barely visible energy web stops your movement!\n\r", ch);
	return;
    }

    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_SHADOW) )
    {
     if( IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_FOLLOW) )
     {
      INC_POWERS(ch, SHADOW_COUNTER);
      if( ch->move < ( GET_POWERS(ch, SHADOW_COUNTER) * 50 ) )
       conceal_count(ch);
      else
       ch->move -= ( GET_POWERS(ch, SHADOW_COUNTER) * 50 );
     }
    }

	WAIT_STATE( ch, 1 );
	if (!IS_SET(ch->mounted, IS_RIDING)) ch->move -= move;
    }

    /* Check for mount message - KaVir */
    if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
    {
	if (IS_NPC(mount))
	    sprintf(mount2," on %s.",mount->short_descr);
	else
	    sprintf(mount2," on %s.",mount->name);
    }
    else
	sprintf(mount2,".");

    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	sprintf(leave,"rolls");
    else if ( IS_AFFECTED(ch,AFF_ETHEREAL) )
	sprintf(leave,"floats");
    else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
	sprintf(leave,"swims");
    else if ( IS_BODY(ch,BROKEN_SPINE) )
	sprintf(leave,"drags $mself");
    else if ( IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG) )
	sprintf(leave,"drags $mself");
    else if ( ch->hit < (ch->max_hit/4) )
	sprintf(leave,"crawls");
    else if ( (IS_LEG_R(ch,LOST_LEG) || IS_LEG_R(ch,LOST_FOOT)) &&
	 (!IS_LEG_L(ch,BROKEN_LEG) && !IS_LEG_L(ch,LOST_LEG) && 
	  !IS_LEG_L(ch,LOST_FOOT)) )
	sprintf(leave,"hops");
    else if ( (IS_LEG_L(ch,LOST_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (!IS_LEG_R(ch,BROKEN_LEG) && !IS_LEG_R(ch,LOST_LEG) &&
	  !IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"hops");
    else if ( (IS_LEG_L(ch,BROKEN_LEG) || IS_LEG_L(ch,LOST_FOOT)) &&
	 (!IS_LEG_R(ch,BROKEN_LEG) && !IS_LEG_R(ch,LOST_LEG) &&
	  !IS_LEG_R(ch,LOST_FOOT)) )
	sprintf(leave,"limps");
    else if ( (IS_LEG_R(ch,BROKEN_LEG) || IS_LEG_R(ch,LOST_FOOT)) &&
	 (!IS_LEG_L(ch,BROKEN_LEG) && !IS_LEG_L(ch,LOST_LEG) &&
	  !IS_LEG_L(ch,LOST_FOOT)) )
	sprintf(leave,"limps");
    else if ( ch->hit < (ch->max_hit/3) )
	sprintf(leave,"limps");
    else if ( ch->hit < (ch->max_hit/2) )
	sprintf(leave,"staggers");
    else if ( !IS_NPC(ch) )
    {
	if (ch->pcdata->condition[COND_DRUNK] > 10)
	    sprintf(leave,"staggers");
	else
	    sprintf(leave,"walks");
    }
    else
	sprintf(leave,"walks");

    if (!IS_NPC(ch) && ch->stance[0] != -1 && ((ch->trust == 3) || (ch->level == 3))) do_stance(ch,"");
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	if ( ( victim = d->character ) == NULL )
	    continue;

        if ( ch->in_room == NULL || victim->in_room == NULL ) continue;

        if ( ch == victim || ch->in_room != victim->in_room) continue;

	if ( d->connected != CON_PLAYING || !can_see( ch, victim ) )
	    continue;

	if ( !IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK) && IS_AFFECTED(ch,AFF_POLYMORPH)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) )
	&& can_see(victim,ch))
	{
	    if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"%s rides $T%s",ch->morph,mount2);
	    else
		sprintf(poly,"%s %s $T%s",ch->morph,leave,mount2);
	    act( poly, victim, NULL, dir_name[door], TO_CHAR );
	}
	else if ( !IS_AFFECTED(ch, AFF_SNEAK)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) )
	&& can_see(victim,ch))
	{
	    if( ((mount = ch->mount) != NULL) && (ch->mounted == IS_RIDING))
		sprintf(poly,"$n rides %s%s",dir_name[door],mount2);
	    else
		sprintf(poly,"$n %s %s%s",leave,dir_name[door],mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
    }
    char_from_room( ch );
    char_to_room( ch, to_room );

/******** Dreamscape DT Bit Removal   *********/

    if ( ch->in_room->vnum == 26112 && !IS_NPC(ch))
    {
     if( IS_AFFECTED(ch, AFF_SHADOWPLANE))
      REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
     if( IS_AFFECTED(ch, AFF_ETHEREAL))
      REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
     if( IS_SET(ch->newbits, NEW_DARKNESS))
      REMOVE_BIT(ch->newbits, NEW_DARKNESS);
     if( IS_SET(GET_POWERS(ch, ALIEN_BITS), ALIEN_CLOAK))
      REM_BIT_POWERS(ch, ALIEN_BITS, ALIEN_CLOAK); 
    } 

/******** Dreamscape character checks *********/

    if ( ch->in_room->vnum == 26001 ) 
    {
     ROOM_INDEX_DATA *pRoomIndex;      
     pRoomIndex = get_room_index(26112);
     if( (ch->level < 3
     ||   ch->max_hit < 15000
     ||   ch->max_mana < 15000
     ||   ch->max_move < 15000) 
     && !IS_NPC(ch) )
     {
      stc("The Dream Master exclaims 'You were warned not to enter if you were not powerful enough!" 
          " Now you shall pay with your life!'\n\r", ch);
      act("$n disappears before you to pay with his blood!\n\r", ch, NULL, NULL, TO_ROOM);
      char_from_room(ch);
      char_to_room(ch,pRoomIndex);
      return;
     }
    } 
    if      ( door == 0 ) {revdoor = 2;sprintf(buf,"the south");}
    else if ( door == 1 ) {revdoor = 3;sprintf(buf,"the west");}
    else if ( door == 2 ) {revdoor = 0;sprintf(buf,"the north");}
    else if ( door == 3 ) {revdoor = 1;sprintf(buf,"the east");}
    else if ( door == 4 ) {revdoor = 5;sprintf(buf,"below");}
    else                  {revdoor = 4;sprintf(buf,"above");}

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	if ( ( victim = d->character ) == NULL )
	    continue;

        if ( ch->in_room == NULL || victim->in_room == NULL ) continue;

        if ( ch == victim || ch->in_room != victim->in_room) continue;

	if ( d->connected != CON_PLAYING || !can_see( ch, victim ) )
	    continue;

	if ( !IS_NPC(ch) && !IS_AFFECTED(ch, AFF_SNEAK) && IS_AFFECTED(ch,AFF_POLYMORPH)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) && can_see(victim,ch))
	{
	    if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"%s rides in from %s%s",ch->morph,buf,mount2);
	    else
		sprintf(poly,"%s %s in from %s%s",ch->morph,leave,buf,mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
	else if ( !IS_AFFECTED(ch, AFF_SNEAK) && can_see(victim,ch)
	&& ( IS_NPC(ch) || !IS_SET(ch->act, PLR_WIZINVIS) ) )
	{
	    if ( (mount = ch->mount) != NULL && ch->mounted == IS_RIDING )
		sprintf(poly,"$n rides in from %s%s",buf,mount2);
	    else
		sprintf( poly, "$n %s in from %s%s",leave,buf,mount2);
	    act( poly, ch, NULL, victim, TO_VICT );
	}
    }

    do_look( ch, "auto" );

    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;
	if ( (mount = fch->mount) != NULL && mount == ch && IS_SET(fch->mounted,IS_MOUNT))
	{
	    act( "$N digs $S heels into you.", fch, NULL, ch, TO_CHAR );
	    char_from_room(fch);
	    char_to_room(fch,ch->in_room);
	}

	if ( fch->master == ch && fch->position == POS_STANDING 
		&& fch->in_room != ch->in_room)
	{
	    act( "You follow $N.", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door );
	}
    }
    room_text(ch,">ENTER<");
    if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_DROW) && IS_SET(GET_POWERS(ch,DROW_BITS),DROW_HATE) )
    {
     if( number_percent() < 2 )
     {
      stc("You scream out in hatred and attack!\n\r",ch);
      act("$n screams out in hatred and attacks!",ch,NULL,NULL,TO_ROOM);
      for( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
       vch_next = vch->next_in_room;
       if( ch == vch )
        continue;
       else
        multi_hit(ch,vch,TYPE_UNDEFINED);
      }
     }
    }
    return;
}



void do_humanform( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    if (IS_NPC(ch)) return;
    if ( ( obj = ch->pcdata->chobj ) == NULL )
    {
	send_to_char("You are already in human form.\n\r",ch);
	return;
    }

    ch->pcdata->obj_vnum = 0;
    obj->chobj = NULL;
    ch->pcdata->chobj = NULL;
    if( IS_SET(ch->affected_by,AFF_POLYMORPH) )
     REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);
    if( IS_SET(ch->extra,EXTRA_OSWITCH) )
     REMOVE_BIT(ch->extra, EXTRA_OSWITCH);
    if( IS_CLASS(ch,CLASS_VAMPIRE) )
     GET_POWERS(ch,VAMP_FORM) = FORM_NORMAL;
    free_string(ch->morph);
    ch->morph = str_dup("");
    act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
    act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
    extract_obj(obj);
    if (ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
    {
	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_HELL));
    }
    return;
}

void check_trap(CHAR_DATA *ch)
{
  AFFECT_DATA *af;
  AFFECT_DATA *af_next;
  int dam = 0;
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *vch, *vch_next;
  AFFECT_DATA afc;
  int sn = 0;

    /* add traps here */
    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DART) )
    {
     if( !IS_NPC(ch) && (ch->trust >= 3) )
     {
      if( !str_cmp(ch->in_room->trapped_by,ch->name) )
       return;   
      dam = number_range(1000,3000);
      ch->hit -= dam;
      sprintf(buf, "A soft *thump* is heard as a dart flies out of nowhere striking %s! [%d]", ch->name, dam);
      act(buf,ch,NULL,NULL,TO_NOTVICT);
      sprintf(buf, "You hear a soft *thump* and then a sudden pain as you are hit by a dart! [%d]\n\r", dam);
      stc(buf,ch);
      update_pos(ch);
      ch->in_room->trapped_by = NULL;
      for( af = ch->in_room->affected; af != NULL; af = af_next)
      {
       af_next = af->next;
       if( af->bitvector == AFF_ROOM_DART)
        room_affect_remove(ch->in_room,af);
      }
     }
    }
    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_EXPLOSION) )
    {
     if( !IS_NPC(ch) && (ch->trust >= 3) )
     {
      if( !str_cmp(ch->in_room->trapped_by,ch->name) )
       return;   
      dam = number_range(500,1500); 
      for( vch = ch->in_room->people; vch != NULL; vch = vch_next )
      {
       vch_next = vch->next_in_room;
       if( IS_NPC(vch) )
        continue;
       if( !str_cmp(vch->in_room->trapped_by,ch->name) )
        continue;
       vch->hit -= dam;
       vch->pcdata->pulse_timer[PULSE_HEATMETAL] = number_range(1,2);
       if( IS_CLASS(vch,CLASS_DRAGON) && GET_POWERS(vch,DRAGON_FORM) == DRAGON_FORM_DRAGON )
        vch->pcdata->pulse_timer[PULSE_HEATMETAL] = 0;
       update_pos(vch);   
       sprintf(buf, "A burst of flame suddenly engulfs the room! [%d]", dam );
       send_to_room(buf,ch->in_room);
      }
      ch->in_room->trapped_by = NULL;
      for( af = ch->in_room->affected; af != NULL; af = af_next)
      {
       af_next = af->next;
       if( af->bitvector == AFF_ROOM_EXPLOSION)
        room_affect_remove(ch->in_room,af);
      }
     }
    }
    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_POISON) )
    {
     if( !IS_NPC(ch) && (ch->trust >= 3) )
     {
      if( !str_cmp(ch->in_room->trapped_by,ch->name) )
       return;
      sprintf(buf, "You hear a barely audible hiss of gas, then notice %s suddenly turning a sickly green!", ch->name);
      act(buf,ch,NULL,NULL,TO_NOTVICT);
      sprintf(buf, "Your vision blurs and you become dizzy as you feel poison begin to flow through your veins!\n\r");
      stc(buf,ch);
      if( (sn = skill_lookup("poison")) < 0)
       return;
      afc.type = sn;
      afc.duration = number_range(2,6); 
      afc.location = APPLY_STR;
      afc.modifier = -3;
      afc.bitvector = AFF_POISON;
      affect_to_char(ch, &afc);
      update_pos(ch);
      ch->in_room->trapped_by = NULL;
      for( af = ch->in_room->affected; af != NULL; af = af_next)
      {
       af_next = af->next;
       if( af->bitvector == AFF_ROOM_POISON)
       room_affect_remove(ch->in_room,af);
      }
     }
    }
    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_SLEEP) )
    {
     if( !IS_NPC(ch) && (ch->trust >= 3) )
     {
      if( !str_cmp(ch->in_room->trapped_by,ch->name) )
       return;
      sprintf(buf, "You hear a barely audible hiss of gas, then notice %s stumble around and collapse on the floor!", ch->name);
      act(buf,ch,NULL,NULL,TO_NOTVICT);
      sprintf(buf, "Your eyes suddenly grow very heavy as you collapse to the floor!\n\r");
      stc(buf,ch);
      ch->position = POS_SLEEPING;
      update_pos(ch);
      WAIT_STATE(ch,16);
      ch->in_room->trapped_by = NULL;
      for( af = ch->in_room->affected; af != NULL; af = af_next)
      {
       af_next = af->next;
       if( af->bitvector == AFF_ROOM_SLEEP) 
       room_affect_remove(ch->in_room,af);
      }
     }
    }
    if( IS_SET(ch->in_room->affected_by, AFF_ROOM_FIRETRAP))
    {
      if(!IS_NPC(ch) && IS_HERO(ch) && (number_range(1,100) < 70) && !has_power(ch, SCHOOL_FIRE))
      {
       ch->hit -= number_range(1000,3000);
       act("$n has tripped a firetrap!", ch, NULL, NULL, TO_ROOM);
       stc("You have tripped a firetrap!\n\r", ch);
       update_pos(ch);
       for( af = ch->in_room->affected; af != NULL; af = af_next)
       {
        af_next = af->next;
        if( af->bitvector == AFF_ROOM_FIRETRAP)
         room_affect_remove(ch->in_room,af);
       }
      }
     } 
   return;
}

void do_notravel( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (IS_IMMUNE(ch, IMM_TRAVEL))
    {
	REMOVE_BIT(ch->immune, IMM_TRAVEL);
	send_to_char("You can now be the target of travel or burrow.\n\r", ch);
	return;
    }
    else if (!IS_IMMUNE(ch, IMM_TRAVEL)) 
    {
	SET_BIT(ch->immune, IMM_TRAVEL);
	send_to_char("You can no longer be the target of travel or burrow.\n\r", ch);
	return;
    }
    else
	send_to_char( "Excuse you?\n\r", ch );
    return;
}

void do_north( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;
   if (IS_AFFECTED(ch,AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
        if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
        send_to_char("You are unable to move tangled in these tendrils.\n\r",ch);
	return;
    }
    if( !IS_NPC(ch)
     && ch->pcdata->held_by != NULL
     && ch->pcdata->held_by[0] != '\0')
    {
     stc("You can't move while being held.\n\r", ch);
     return;
    }
    in_room = ch->in_room;
    move_char( ch, DIR_NORTH );
    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_NORTH );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
    check_trap(ch);
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
    send_to_char("You are unable to move tangled in these tendrils.\n\r",ch);
	return;
    }
    if( !IS_NPC(ch)
     && ch->pcdata->held_by != NULL
     && ch->pcdata->held_by[0] != '\0')
    {
     stc("You can't move while being held.\n\r", ch);
     return;
    }
    in_room = ch->in_room;
    move_char( ch, DIR_EAST );
    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_EAST );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
    check_trap(ch);
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
        send_to_char("You are unable to move tangled in these tendrils.\n\r",ch);
	return;
    }
    if( !IS_NPC(ch)
     && ch->pcdata->held_by != NULL
     && ch->pcdata->held_by[0] != '\0')
    {
     stc("You can't move while being held.\n\r", ch);
     return;
    }
    in_room = ch->in_room;
    move_char( ch, DIR_SOUTH );
    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_SOUTH );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
    check_trap(ch);
    return;
}

void do_west( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
        send_to_char("You are unable to move tangled in these tendrils.\n\r",ch);
	return;
    }
    if( !IS_NPC(ch)
     && ch->pcdata->held_by != NULL
     && ch->pcdata->held_by[0] != '\0')
    {
     stc("You can't move while being held.\n\r", ch);
     return;
    }
    in_room = ch->in_room;
    move_char( ch, DIR_WEST );
    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_WEST );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
    check_trap(ch);
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
        send_to_char("You are unable to move tangled in these tendrils.\n\r",ch);
	return;
    }
    if( !IS_NPC(ch)
     && ch->pcdata->held_by != NULL
     && ch->pcdata->held_by[0] != '\0')
    {
     stc("You can't move while being held.\n\r", ch);
     return;
    }
    in_room = ch->in_room;
    move_char( ch, DIR_UP );
    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_UP );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
    check_trap(ch);
    return;
}

void do_down( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;

    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
	send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
	return;
    }
    if (IS_AFFECTED(ch, AFF_TENDRILS))
    {
        send_to_char("You are unable to move tangled in these tendrils.\n\r",ch);
	return;
    }
    if( !IS_NPC(ch)
     && ch->pcdata->held_by != NULL
     && ch->pcdata->held_by[0] != '\0')
    {
     stc("You can't move while being held.\n\r", ch);
     return;
    }
    in_room = ch->in_room;
    move_char( ch, DIR_DOWN );
    if (!IS_NPC(ch) && ch->in_room != in_room)
    {
	ROOM_INDEX_DATA *old_room;
	old_room = ch->in_room;
	char_from_room(ch);
	char_to_room(ch, in_room);
	add_tracks( ch, DIR_DOWN );
	char_from_room(ch);
	char_to_room(ch, old_room);
    }
    check_trap(ch);
    return;
}



int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}

/* random room generation procedure */
ROOM_INDEX_DATA  *get_random_room(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL )
        if ( !IS_SET(room->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(room->room_flags, ROOM_SOLITARY) 
		&&   !IS_SET(room->room_flags, ROOM_SAFE) 
		&& ((room->vnum < 50) || (room->vnum > 68)) )
            break;
    }

    return room;
}

ROOM_INDEX_DATA  *get_rand_room()
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL )
        if ( !IS_SET(room->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(room->room_flags, ROOM_SOLITARY) 
	&&   !IS_SET(room->room_flags, ROOM_SAFE) 
	&&  room->vnum > 300
	&& ((room->vnum < 50) || (room->vnum > 68)) )
            break;
    }

    return room;
}

ROOM_INDEX_DATA  *get_treemeld_room()
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL
	    && room->sector_type == SECT_FOREST ) break;
    }

    return room;
}

ROOM_INDEX_DATA  *get_rand_room_by_sect( int sect )
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 32000 ) );
        if ( room != NULL
	    && room->sector_type == sect ) break;
    }

    return room;
}

/* Designed for the portal spell, but can also have other uses...KaVir
 * V0 = Where the portal will take you.
 * V1 = Number of uses (0 is infinate).
 * V2 = if 2, cannot be entered.
 * V3 = The room the portal is currently in.
 */
void do_enter( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char arg[MAX_INPUT_LENGTH];
    char poly [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    CHAR_DATA *mount;
    bool found;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Enter what?\n\r", ch );
	return;
    }

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return;
    }
    if ( obj->item_type != ITEM_PORTAL )
    {
	act( "You cannot enter that.", ch, NULL, arg, TO_CHAR );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
	!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
    {
	send_to_char( "You are too insubstantual.\n\r", ch );
	return;
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
	IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
    {
	send_to_char( "It is too insubstantual.\n\r", ch );
	return;
    }

    if ( obj->value[2] == 2 || obj->value[2] == 3 )
    {
	act( "It seems to be closed.", ch, NULL, arg, TO_CHAR );
	return;
    }

    pRoomIndex = get_room_index(obj->value[0]);
    location = ch->in_room;

    if ( pRoomIndex == NULL )
    {
	act( "You are unable to enter.", ch, NULL, arg, TO_CHAR );
	return;
    }

    act( "You step into $p.",  ch, obj, NULL, TO_CHAR );
    if ( !IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH) )
	sprintf(poly,"%s steps into $p.",ch->morph);
    else
	sprintf(poly,"$n steps into $p.");
    act( poly,  ch, obj, NULL, TO_ROOM );
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    if ( !IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH) )
	sprintf(poly,"%s steps out of $p.",ch->morph);
    else
	sprintf(poly,"$n steps out of $p.");
    act( poly,  ch, obj, NULL, TO_ROOM );
    char_from_room(ch);
    char_to_room(ch,location);
/* Leave this out for now, as it doesn't seem to work properly. KaVir
    if ((obj->value[2] == 1) && (CAN_WEAR(obj,ITEM_TAKE)))
    {
	obj->value[2] = 2;
	REMOVE_BIT(obj->wear_flags, ITEM_TAKE);
    }
*/
    if (obj->value[1] != 0)
    {
	obj->value[1] = obj->value[1] - 1;
	if (obj->value[1] < 1)
	{
	    act( "$p vanishes.",  ch, obj, NULL, TO_CHAR );
	    act( "$p vanishes.",  ch, obj, NULL, TO_ROOM );
	    extract_obj(obj);
	}
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
/* Leave this out for now, as it doesn't seem to work properly. KaVir
	    if ((portal->value[2] == 2) && (!CAN_WEAR(obj,ITEM_TAKE)))
	    {
		portal->value[2] = 1;
		SET_BIT(obj->wear_flags, ITEM_TAKE);
	    }
*/
	    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
		!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
	    {
		REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
		break;
	    }
	    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
		IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
	    {
		SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
		break;
	    }
    	    if (portal->value[1] != 0)
    	    {
		portal->value[1] = portal->value[1] - 1;
		if (portal->value[1] < 1)
		{
		    act( "$p vanishes.",  ch, portal, NULL, TO_CHAR );
		    act( "$p vanishes.",  ch, portal, NULL, TO_ROOM );
		    extract_obj(portal);
		}
	    }
    	}
    }
    do_look(ch,"auto");
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}


void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_BOOK)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) && obj->item_type != ITEM_BOOK)
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER && obj->item_type != ITEM_BOOK)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) && obj->item_type != ITEM_BOOK )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	if (obj->item_type == ITEM_BOOK) obj->value[2] = 0;
	send_to_char( "Ok.\n\r", ch );
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}



void do_turn( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: Turn <book> <forward/back>.\n\r", ch );
	return;
    }

    if (is_number(arg2)) value = atoi(arg2);

    if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
    {
	send_to_char("You don't have that book.\n\r",ch);
	return;
    }

    if ( obj->item_type != ITEM_BOOK)
    { send_to_char( "That's not a book.\n\r", ch ); return; }
    if ( IS_SET(obj->value[1], CONT_CLOSED) )
    { send_to_char( "First you should open it.\n\r",    ch ); return; }

    if (!str_cmp(arg2,"f") || !str_cmp(arg2,"forward"))
    {
	if (obj->value[2] >= obj->value[3])
	{
	    send_to_char("But you are already at the end of the book.\n\r",ch);
	    return;
	}
	obj->value[2] += 1;
	act( "You flip forward a page in $p.", ch, obj, NULL, TO_CHAR );
	act( "$n flips forward a page in $p.", ch, obj, NULL, TO_ROOM );
    }
    else if (!str_cmp(arg2,"b") || !str_cmp(arg2,"backward"))
    {
	if (obj->value[2] <= 0)
	{
	    send_to_char("But you are already at the beginning of the book.\n\r",ch);
	    return;
	}
	obj->value[2] -= 1;
	act( "You flip backward a page in $p.", ch, obj, NULL, TO_CHAR );
	act( "$n flips backward a page in $p.", ch, obj, NULL, TO_ROOM );
    }
    else if (is_number(arg2) && value >= 0 && value <= obj->value[3])
    {
	if (value == obj->value[2])
	{
	    act("$p is already open at that page.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	else if (value < obj->value[2])
	{
	    act( "You flip backwards through $p.", ch, obj, NULL, TO_CHAR );
	    act( "$n flips backwards through $p.", ch, obj, NULL, TO_ROOM );
	}
	else
	{
	    act( "You flip forwards through $p.", ch, obj, NULL, TO_CHAR );
	    act( "$n flips forwards through $p.", ch, obj, NULL, TO_ROOM );
	}
	obj->value[2] = value;
    }
    else
	send_to_char("Do you wish to turn forward or backward a page?\n\r",ch);
    return;
}

void do_read( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you wish to read?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char("You don't have that.\n\r",ch);
	return;
    }

    if ( obj->item_type == ITEM_PAGE)
    {
	if (!str_cmp(obj->victpoweruse,"(null)"))
	    sprintf(buf,"Untitled page.\n\r");
	else
	    sprintf(buf,"%s.\n\r",obj->victpoweruse);
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	if (obj->chpoweruse == NULL || obj->chpoweruse == '\0' ||
	    !str_cmp(obj->chpoweruse,"(null)"))
	{
	    if (!are_runes(obj))
		send_to_char("This page is blank.\n\r",ch);
	    else if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
		!IS_SET(obj->quest, QUEST_MASTER_RUNE))
		show_runes(ch, obj, FALSE);
	    else
		send_to_char("This page is blank.\n\r",ch);
	    return;
	}
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	send_to_char(obj->chpoweruse,ch);
	send_to_char("\n\r--------------------------------------------------------------------------------\n\r",ch);
	if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) &&
	    !IS_SET(obj->quest, QUEST_MASTER_RUNE )) show_runes(ch, obj, TRUE);
	return;
    }

    if ( obj->item_type != ITEM_BOOK)
    { do_look(ch,obj->name); return; }
    if ( IS_SET(obj->value[1], CONT_CLOSED) )
    {
	if (!str_cmp(obj->victpoweruse,"(null)"))
	    sprintf(buf,"The book is untitled.\n\r");
	else
	    sprintf(buf,"The book is titled '%s'.\n\r",obj->victpoweruse);
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	return;
    }

    if (obj->value[2] == 0)
    {
	int page;

	send_to_char("Index page.\n\r",ch);
	if (obj->value[3] <= 0)
	{
	    send_to_char("<No pages>\n\r",ch);
	    return;
	}
	for ( page = 1; page <= obj->value[3]; page ++ )
	{
	    sprintf(buf,"Page %d:",page);
	    send_to_char(buf,ch);
	    show_page(ch,obj,page,TRUE);
	}
    }
    else
    {
	sprintf(buf,"Page %d:",obj->value[2]);
	send_to_char(buf,ch);
	show_page(ch,obj,obj->value[2],FALSE);
    }
    return;
}

void show_page( CHAR_DATA *ch, OBJ_DATA *book, int pnum, bool pagefalse )
{
    OBJ_DATA *page;
    OBJ_DATA *page_next;
    char buf[MAX_STRING_LENGTH];
    bool found = FALSE;

    for ( page = book->contains; page != NULL; page = page_next )
    {
	page_next = page->next_content;
	if (page->value[0] == pnum)
	{
	    found = TRUE;
	    if (!str_cmp(page->victpoweruse,"(null)"))
		sprintf(buf,"Untitled page.\n\r");
	    else
		sprintf(buf,"%s.\n\r",page->victpoweruse);
	    buf[0] = UPPER(buf[0]);
	    send_to_char(buf,ch);
	    if (!pagefalse)
	    {
		if (page->chpoweruse == NULL || page->chpoweruse == '\0' ||
			!str_cmp(page->chpoweruse,"(null)"))
		{
		    if (!are_runes(page))
			send_to_char("This page is blank.\n\r",ch);
		    else if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
			!IS_SET(page->quest, QUEST_MASTER_RUNE))
			show_runes(ch, page, FALSE);
		    else
			send_to_char("This page is blank.\n\r",ch);
		    return;
		}
		send_to_char("--------------------------------------------------------------------------------\n\r",ch);
		send_to_char(page->chpoweruse,ch);
		send_to_char("\n\r--------------------------------------------------------------------------------\n\r",ch);
		if (IS_AFFECTED(ch, AFF_DETECT_MAGIC) && 
		    !IS_SET(page->quest, QUEST_MASTER_RUNE))
		    show_runes(ch, page, TRUE);
	    }
	}
    }
    if (!found)
	send_to_char("This page has been torn out.\n\r",ch);
    return;
}

void show_runes( CHAR_DATA *ch, OBJ_DATA *page, bool endline )
{
 return;
}

bool are_runes( OBJ_DATA *page )
{
    if (page->value[1] + page->value[2] + page->value[3] < 1) return FALSE;
    return TRUE;
}

bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}

void take_key(CHAR_DATA *ch, int key)
{
  OBJ_DATA *obj;
  bool found = FALSE;

  for( obj = ch->carrying; obj && !found; obj = obj->next_content)
  {
   if( obj->pIndexData->vnum == key)
   {
     obj_from_char(obj);
     extract_obj(obj);
     found = TRUE; 
     return;
   }
  }
  return;
}

void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;
       

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

        if ( pexit->key >= 26007 && pexit->key <= 26056 )
        {
         REMOVE_BIT(pexit->exit_info, EX_LOCKED);
         stc("The door unlocks, and suddenly the key explodes!\n\r", ch);
         act("$n unlocks the $d and suddenly the key explodes!\n\r", ch, NULL, pexit->keyword, TO_ROOM);
         take_key(ch, pexit->key);
         return;
        }
 
	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_pick_lock] )
    {
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* pick the other side */
	if ( ( to_room   = pexit->to_room               ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }

	send_to_char( "You wake and stand up.\n\r", ch );
	act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_RESTING:
    case POS_SITTING:
	send_to_char( "You stand up.\n\r", ch );
	act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_MEDITATING:
	send_to_char( "You uncross your legs and stand up.\n\r", ch );
	act( "$n uncrosses $s legs and stands up.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
    case POS_SITTING:
    case POS_STANDING:
	send_to_char( "You rest.\n\r", ch );
	act( "$n rests.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_RESTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_sit( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You are already sitting.\n\r", ch );
	break;

    case POS_STANDING:
	send_to_char( "You sit down.\n\r", ch );
	act( "$n sits down.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SITTING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}


void do_meditate( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch))
    {
	send_to_char("You are unable to attain the correct state of mind.\n\r",ch);
	return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_MEDITATING:
	send_to_char( "You are already meditating.\n\r", ch );
	break;

    case POS_SITTING:
	send_to_char( "You cross your legs and start meditating.\n\r", ch );
	act( "$n crosses $s legs and starts meditating.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_STANDING:
	send_to_char( "You sit down, cross your legs and start meditating.\n\r", ch );
	act( "$n sits down, crosses $s legs and starts meditating.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_MEDITATING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}

void do_sleep( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_SITTING:
    case POS_MEDITATING:
    case POS_RESTING:
    case POS_STANDING: 
	send_to_char( "You sleep.\n\r", ch );
	act( "$n sleeps.", ch, NULL, NULL, TO_ROOM );
	ch->position = POS_SLEEPING;
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

    if (victim->position < POS_SLEEPING)
	{ act( "$E doesn't respond!",   ch, NULL, victim, TO_CHAR );  return; }

    act( "You wake $M.", ch, NULL, victim, TO_CHAR );
    act( "$n wakes you.", ch, NULL, victim, TO_VICT );
    victim->position = POS_STANDING;
    return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    send_to_char( "You attempt to move silently.\n\r", ch );
    affect_strip( ch, gsn_sneak );

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_sneak] )
    {
	af.type      = gsn_sneak;
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
    }

    return;
}



void do_hide( CHAR_DATA *ch, char *argument )
{
    send_to_char( "You attempt to hide.\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_hide] )
	SET_BIT(ch->affected_by, AFF_HIDE);

    return;
}



/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    affect_strip ( ch, gsn_invis			);
    affect_strip ( ch, gsn_mass_invis			);
    affect_strip ( ch, gsn_sneak			);
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_unpolymorph( CHAR_DATA *ch, char *argument )
{
    if (!is_affected(ch, gsn_polymorph) )
    {
	send_to_char("But you are not polymorphed!\n\r",ch);
	return;
    }
    act("$n's body begins to distort.",ch,NULL,NULL,TO_ROOM);
    affect_strip ( ch, gsn_polymorph );
    act("$n resumes $s normal form.",ch,NULL,NULL,TO_ROOM);
    send_to_char( "You resume your normal form.\n\r", ch );
    return;
}



void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *location;
  
    act( "$n's body flickers with green energy.", ch, NULL, NULL, TO_ROOM );
    act( "Your body flickers with green energy.", ch, NULL, NULL, TO_CHAR );

    if ( ( location = get_room_index( ch->home ) ) == NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location && ch->in_room->vnum != 26)
	return;
    if( in_arena(ch))
    {
     stc("You can't recall from the arena.\n\r", ch);
     return;
    }
    if(  IS_AFFECTED(ch, AFF_CURSE) && ch->level > 2 )
    {
	send_to_char( "You are unable to recall.\n\r", ch );
	return;
    }
    if (IS_NPC(ch) && ch->level <= 500)
    {
      send_to_char( "You are unable to recall.\n\r", ch );
      return;
    }
    if ( ( victim = ch->fighting ) != NULL )
    {
	if ( number_bits( 1 ) == 0 )
	{
	    WAIT_STATE( ch, 4 );
	    sprintf( buf, "You failed!\n\r" );
	    send_to_char( buf, ch );
	    return;
	}
	sprintf( buf, "You recall from combat!\n\r");
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
    }

    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    if( ch->home != 26 )
    {
      char_from_room( ch );
      char_to_room( ch, location );
    }
    else
    {
	stc("\n\r#7The miners have kicked you out of their home!#n\n\r\n\r", 
ch);
	ch->home = 3001;
	char_from_room(ch);
	char_to_room(ch, get_room_index(3001));
    }
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if ( ( mount = ch->mount ) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, ch->in_room );
    return;
}

void do_home( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC(ch) ) return;

    if ( arg[0] == '\0' || str_cmp(arg,"here") )
    {
	send_to_char( "If you wish this to be your room, you must type 'home here'.\n\r", ch );
	return;
    }

    if ( ch->in_room->vnum == ch->home )
    {
	send_to_char( "But this is already your home!\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) ||
	 IS_SET(ch->in_room->room_flags, ROOM_SAFE) ||
	 (ch->in_room->vnum >= 26000 && ch->in_room->vnum <= 26206)
      || (ch->in_room->vnum >= 500 && ch->in_room->vnum <= 599) )  /* Reserved for clanhalls --Kline */
    {
	send_to_char( "You are unable to make this room your home.\n\r", ch );
	return;
    }

    if( ch->in_room->vnum == 26 && !IS_IMMORTAL(ch) )
    {
	stc("The miners won't stoop that low to let your kind home here!\n\r", ch);
	return;
    }
    ch->home = ch->in_room->vnum;
    send_to_char( "This room is now your home.\n\r", ch );

    return;
}


void do_escape( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch) || !IS_HERO(ch))
	return;
  
	if (ch->hit > 0)
	{
		send_to_char("You can only do this if you are wounded.\n\r", ch);
		return;
	}
    if ( ch->position >= POS_SLEEPING )
    {
		send_to_char( "You can only do this if you are dying.\n\r", ch );
		return;
    }
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
    if ( ( location = get_room_index( ROOM_VNUM_TEMPLE ) ) == NULL )
    {
		send_to_char( "You are completely lost.\n\r", ch );
		return;
    }

    if ( ch->in_room == location )
	return;

    ch->move = 0;
    ch->mana = 0;
    act( "$n fades out of existance.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n fades into existance.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    sprintf(buf,"%s has escaped defenceless from a fight.",ch->pcdata->switchname);
    do_info(ch,buf);

    return;
}


/* Old train  */

void do_oldtrain( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    sh_int *pAbility;
    char *pOutput;
    int cost;
    int magic;
    int immcost;
    int primal;
    int ancilla;
    int methuselah;
    int elder;
    int max_stat = 18;
    bool last = TRUE;
    bool is_ok = FALSE;
    bool immutest = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) )
	return;

    if ( arg1[0] == '\0' )
    {
	sprintf( buf, "You have %d experience points.\n\r", ch->exp );
	send_to_char( buf, ch );
	strcpy(arg1,"foo");
    }

         if (!str_cmp(arg1,"str")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"int")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"wis")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"dex")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"con")   ) is_ok = TRUE;
    else if (!str_cmp(arg1,"hp")    ) is_ok = TRUE;
    else if (!str_cmp(arg1,"mana")  ) is_ok = TRUE;
    else if (!str_cmp(arg1,"move")  ) is_ok = TRUE;
    else if (!str_cmp(arg1,"primal")) is_ok = TRUE;

	if ((!str_cmp(arg1,"hp") || !str_cmp(arg1,"hit")) && ch->max_hit < 30001)
	{
	  if (ch->exp < ch->max_hit * 50)
	  {
	    send_to_char("You dont have the experience required to improve hp by 50.\n\r",ch);
	    return;
	  }
		ch->exp-=ch->max_hit*50;
		ch->max_hit+= 50;
		if (ch->max_hit > 30000)
			ch->max_hit = 30000;
		return;
	}

  if ( arg2[0] != '\0' && is_ok )
    {
	int amount = 0;
         
	if (!is_number(arg2))
	{
	    send_to_char("Please enter a numeric value.\n\r",ch);
	    return;
	}
	amount = atoi( arg2 );
	if (amount < 1 || amount > 50)
	{
	    send_to_char("Please enter a value between 1 and 50.\n\r",ch);
	    return;
	}
	
	if(amount > 1)
	{
	    sprintf(buf,"%s %d",arg1,amount-1);
	    do_oldtrain(ch,buf);
	    last = FALSE;
	}
    }

    cost = 200;
    immcost = count_imms(ch);
    primal = (1+ch->practice)*500;
    magic = 0;
    ancilla = 1500000;
    elder = 7500000;
    methuselah = 15000000;

    if ( !str_cmp( arg1, "str" ) )
    {
	pAbility    = &ch->pcdata->perm_str;
	pOutput     = "strength";
    }

    else if ( !str_cmp( arg1, "int" ) )
    {
	pAbility    = &ch->pcdata->perm_int;
	pOutput     = "intelligence";
    }

    else if ( !str_cmp( arg1, "wis" ) )
    {
	pAbility    = &ch->pcdata->perm_wis;
	pOutput     = "wisdom";
    }

    else if ( !str_cmp( arg1, "dex" ) )
    {
	pAbility    = &ch->pcdata->perm_dex;
	pOutput     = "dexterity";
    }

    else if ( !str_cmp( arg1, "constitution" ) )
    {
	pAbility    = &ch->pcdata->perm_con;
	pOutput     = "constitution";
    }

	else if ( !str_cmp(arg1, "con"))
    {
	pAbility    = &ch->pcdata->perm_con;
	pOutput     = "constitution";
    }

    else if ( !IS_NPC(ch) && !str_cmp( arg1, "avatar") && (ch->level == 2) && ch->max_hit >= 8000 
           && ch->pcdata->race->complete)
    {
        cost        = 1000;
	pAbility    = &ch->level;
	pOutput     = "level";
    }

	else if ( !str_cmp( arg1, "mana") && ch->max_mana < 30000)
    {
        cost        = (ch->max_mana - ch->pcdata->perm_wis);
	pAbility    = &ch->max_mana;
	pOutput     = "mana";
    }

    else if ( !str_cmp( arg1, "move") && ch->max_move < 30000)
    {
        cost        = (ch->max_move - ch->pcdata->perm_dex);
	pAbility    = &ch->max_move;
	pOutput     = "move";
    }

    else if ( !str_cmp( arg1, "primal") && ch->practice < 999)
    {
        cost        = primal;
	pAbility    = &ch->practice;
	pOutput     = "primal";
    }
    else if ( !str_cmp( arg1, "slash") && !IS_IMMUNE(ch,IMM_SLASH) )
    
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SLASH);
	send_to_char( "You are now more resistant to slashing and slicing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "stab") && !IS_IMMUNE(ch,IMM_STAB) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_STAB);
	send_to_char( "You are now more resistant to stabbing and piercing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "smash") && !IS_IMMUNE(ch,IMM_SMASH) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SMASH);
	send_to_char( "You are now more resistant to blasting, pounding and crushing weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "beast") && !IS_IMMUNE(ch,IMM_ANIMAL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_ANIMAL);
	send_to_char( "You are now more resistant to claw and bite attacks.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "grab") && !IS_IMMUNE(ch,IMM_MISC) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_MISC);
	send_to_char( "You are now more resistant to grepping, sucking and whipping weapons.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "charm") && !IS_IMMUNE(ch,IMM_CHARM) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_CHARM);
	send_to_char( "You are now immune to charm spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "heat") && !IS_IMMUNE(ch,IMM_HEAT) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_HEAT);
	send_to_char( "You are now immune to heat and fire spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "cold") && !IS_IMMUNE(ch,IMM_COLD) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_COLD);
	send_to_char( "You are now immune to cold spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "lightning") && !IS_IMMUNE(ch,IMM_LIGHTNING) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_LIGHTNING);
	send_to_char( "You are now immune to lightning and electrical spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "acid") && !IS_IMMUNE(ch,IMM_ACID) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_ACID);
	send_to_char( "You are now immune to acid spells.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "sleep") && !IS_IMMUNE(ch,IMM_SLEEP) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_SLEEP);
	send_to_char( "You are now immune to the sleep spell.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "drain") && !IS_IMMUNE(ch,IMM_DRAIN) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_DRAIN);
	send_to_char( "You are now immune to the energy drain spell.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "voodoo") && !IS_IMMUNE(ch,IMM_VOODOO) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_VOODOO);
	send_to_char( "You are now immune to voodoo magic.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "hurl") && !IS_IMMUNE(ch,IMM_HURL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_HURL);
	send_to_char( "You are now immune to being hurled.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "backstab") && !IS_IMMUNE(ch,IMM_BACKSTAB) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_BACKSTAB);
	send_to_char( "You are now immune to being backstabbed.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "kick") && !IS_IMMUNE(ch,IMM_KICK) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_KICK);
	send_to_char( "You are now immune to being kicked.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "disarm") && !IS_IMMUNE(ch,IMM_DISARM) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_DISARM);
	send_to_char( "You are now immune to being disarmed.\n\r", ch );
	return;
    }

    else if ( !str_cmp( arg1, "steal") && !IS_IMMUNE(ch,IMM_STEAL) )
    {
	if (ch->exp < immcost)
	{
	    send_to_char( "You don't have enough exp.\n\r", ch );
	    return;
	}
	ch->exp = ch->exp - immcost;
	SET_BIT(ch->immune, IMM_STEAL);
	send_to_char( "You are now immune to being stolen from.\n\r", ch );
	return;
    }
    else
    {
	immutest = FALSE;
	if ( ( ch->pcdata->perm_str >= max_stat )
                && ( ch->pcdata->perm_wis >= max_stat )
                && ( ch->pcdata->perm_int >= max_stat )  
                && ( ch->pcdata->perm_dex >= max_stat )    
                && ( ch->pcdata->perm_con >= max_stat ) ) 
		immutest = TRUE;
	if (immutest == FALSE)
	{
	sprintf( buf, "You can train the following:\n\r" );
	send_to_char( buf, ch );

	send_to_char( "Stats:", ch );
	if ( ch->pcdata->perm_str < max_stat ) send_to_char( " Str", ch );
	if ( ch->pcdata->perm_int < max_stat ) send_to_char( " Int", ch );
	if ( ch->pcdata->perm_wis < max_stat ) send_to_char( " Wis", ch );
	if ( ch->pcdata->perm_dex < max_stat ) send_to_char( " Dex", ch );
	if ( ch->pcdata->perm_con < max_stat ) send_to_char( " Con", ch );
	if ( ( ch->pcdata->perm_str >= max_stat )
		&& ( ch->pcdata->perm_wis >= max_stat )
		&& ( ch->pcdata->perm_int >= max_stat )
		&& ( ch->pcdata->perm_dex >= max_stat )
		&& ( ch->pcdata->perm_con >= max_stat ) )
	    send_to_char( " None left to train.\n\r", ch );
	else
	    send_to_char( ".\n\r", ch );
	}
	immutest = FALSE;
	if ( !IS_NPC(ch) && ch->level == 2 && ch->max_hit >= 8000 && ch->pcdata->race->complete)
	{
	   sprintf( buf, "Become an avatar - 1000 exp.\n\r" );
	   send_to_char( buf, ch );
	}
    if ( ch->max_hit < 30000)
	{
		sprintf (buf,"Hp                - %d exp per point.\n\r", (ch->max_hit) );
		send_to_char(buf,ch);
	}
	if ( ch->max_mana      < 30000 )
	{
	    sprintf( buf, "Mana             - %d exp per point.\n\r",(ch->max_mana - ch->pcdata->perm_wis) );
	   send_to_char( buf, ch );
	}
	if ( ch->max_move      < 30000 )
	{
	   sprintf( buf, "Move             - %d exp per point.\n\r",(ch->max_move - ch->pcdata->perm_con) );
	   send_to_char( buf, ch );
	}
	if ( ch->practice        < 999 )
	{
	   sprintf( buf, "Primal           - %d exp per point of primal energy.\n\r", primal );
	   send_to_char( buf, ch );
	}
	immutest = FALSE;
	if ( IS_IMMUNE(ch, IMM_SLASH) && IS_IMMUNE(ch, IMM_STAB) &&
        IS_IMMUNE(ch, IMM_SMASH) && IS_IMMUNE(ch, IMM_ANIMAL) &&
        IS_IMMUNE(ch, IMM_MISC) &&
		IS_IMMUNE(ch, IMM_CHARM) && IS_IMMUNE(ch, IMM_HEAT) &&
        IS_IMMUNE(ch, IMM_COLD) && IS_IMMUNE(ch, IMM_LIGHTNING) &&
        IS_IMMUNE(ch, IMM_ACID) && IS_IMMUNE(ch, IMM_SLEEP) && 
        IS_IMMUNE(ch, IMM_DRAIN) && IS_IMMUNE(ch, IMM_VOODOO) &&
		IS_IMMUNE(ch, IMM_HURL) && IS_IMMUNE(ch, IMM_BACKSTAB) &&
        IS_IMMUNE(ch, IMM_KICK)      && IS_IMMUNE(ch, IMM_DISARM)   && 
        IS_IMMUNE(ch, IMM_STEAL) )
		immutest = TRUE;
	if (immutest == FALSE)
	{
	sprintf( buf, "Natural resistances and immunities - %d exp each.\n\r", immcost );
	send_to_char( buf, ch );
	}
	immutest = FALSE;
	/* Weapon resistance affects */
	if ( IS_IMMUNE(ch, IMM_SLASH) && IS_IMMUNE(ch, IMM_STAB) &&
        IS_IMMUNE(ch, IMM_SMASH) && IS_IMMUNE(ch, IMM_ANIMAL) &&
        IS_IMMUNE(ch, IMM_MISC) )
		immutest = TRUE;
	if (immutest == FALSE)
	{
	send_to_char( "Weapon resistances:", ch );
	if ( !IS_IMMUNE(ch, IMM_SLASH)  ) send_to_char( " Slash", ch );
	if ( !IS_IMMUNE(ch, IMM_STAB)   ) send_to_char( " Stab", ch );
	if ( !IS_IMMUNE(ch, IMM_SMASH)  ) send_to_char( " Smash", ch );
	if ( !IS_IMMUNE(ch, IMM_ANIMAL) ) send_to_char( " Beast", ch );
	if ( !IS_IMMUNE(ch, IMM_MISC)   ) send_to_char( " Grab", ch );
	if ( IS_IMMUNE(ch, IMM_SLASH) && IS_IMMUNE(ch, IMM_STAB) && 
	IS_IMMUNE(ch, IMM_SMASH) && IS_IMMUNE(ch, IMM_ANIMAL) && 
	IS_IMMUNE(ch, IMM_MISC) )
	   send_to_char( " None left to learn.\n\r", ch );
	else
	   send_to_char( ".\n\r", ch );
	}
	immutest = FALSE;
	/* Spell immunity affects */
	if ( IS_IMMUNE(ch, IMM_CHARM) && IS_IMMUNE(ch, IMM_HEAT) &&
        IS_IMMUNE(ch, IMM_COLD) && IS_IMMUNE(ch, IMM_LIGHTNING) &&
        IS_IMMUNE(ch, IMM_ACID) && IS_IMMUNE(ch, IMM_SLEEP) && 
        IS_IMMUNE(ch, IMM_DRAIN) && IS_IMMUNE(ch, IMM_VOODOO) )
		immutest = TRUE;
	if (immutest == FALSE)
	{
	send_to_char( "Magical immunities:", ch );
	if ( !IS_IMMUNE(ch, IMM_CHARM)     ) send_to_char( " Charm", ch );
	if ( !IS_IMMUNE(ch, IMM_HEAT)      ) send_to_char( " Heat", ch );
	if ( !IS_IMMUNE(ch, IMM_COLD)      ) send_to_char( " Cold", ch );
	if ( !IS_IMMUNE(ch, IMM_LIGHTNING) ) send_to_char( " Lightning", ch );
	if ( !IS_IMMUNE(ch, IMM_ACID)      ) send_to_char( " Acid", ch );
	if ( !IS_IMMUNE(ch, IMM_SLEEP)     ) send_to_char( " Sleep", ch );
	if ( !IS_IMMUNE(ch, IMM_DRAIN)     ) send_to_char( " Drain", ch );
	if ( !IS_IMMUNE(ch, IMM_VOODOO)    ) send_to_char( " Voodoo", ch );
	if ( IS_IMMUNE(ch, IMM_CHARM) && IS_IMMUNE(ch, IMM_HEAT) && 
	IS_IMMUNE(ch, IMM_COLD) && IS_IMMUNE(ch, IMM_LIGHTNING) && 
	IS_IMMUNE(ch, IMM_ACID) && IS_IMMUNE(ch, IMM_SLEEP) &&
	IS_IMMUNE(ch, IMM_DRAIN) && IS_IMMUNE(ch, IMM_VOODOO) )
	   send_to_char( " None left to learn.\n\r", ch );
	else
	   send_to_char( ".\n\r", ch );
	}
	immutest = FALSE;
	if ( IS_IMMUNE(ch, IMM_HURL) && IS_IMMUNE(ch, IMM_BACKSTAB) && IS_IMMUNE(ch, IMM_KICK)
	  && IS_IMMUNE(ch, IMM_DISARM) && IS_IMMUNE(ch, IMM_STEAL) )
		immutest = TRUE;
	if (immutest == FALSE)
	{
	/* Skill immunity affects */
	send_to_char( "Skill immunities:", ch );
	if ( !IS_IMMUNE(ch, IMM_HURL)     ) send_to_char( " Hurl", ch );
	if ( !IS_IMMUNE(ch, IMM_BACKSTAB) ) send_to_char( " Backstab", ch );
	if ( !IS_IMMUNE(ch, IMM_KICK)     ) send_to_char( " Kick", ch );
	if ( !IS_IMMUNE(ch, IMM_DISARM)   ) send_to_char( " Disarm", ch );
	if ( !IS_IMMUNE(ch, IMM_STEAL)    ) send_to_char( " Steal", ch );
	if ( IS_IMMUNE(ch, IMM_HURL) && IS_IMMUNE(ch, IMM_BACKSTAB) && 
	IS_IMMUNE(ch, IMM_KICK)      && IS_IMMUNE(ch, IMM_DISARM)   && 
	IS_IMMUNE(ch, IMM_STEAL) )
	   send_to_char( " None left to learn.\n\r", ch );
	else
	   send_to_char( ".\n\r", ch );
	}
	immutest = FALSE;
	return;
    }

    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "str")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "int")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "wis")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= max_stat) && (!str_cmp( arg1, "dex")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if  ((*pAbility >= max_stat) && (!str_cmp( arg1, "constitution")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if  ((*pAbility >= max_stat) && (!str_cmp( arg1, "con")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= 30000) && (!str_cmp( arg1, "mana")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= 30000) && (!str_cmp( arg1, "move")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( (*pAbility >= 999) && (!str_cmp( arg1, "primal")))
    {
	if (last) act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }
    if ( cost < 1 ) 
    cost = 1;
    else if ( cost > ch->exp )
    {
	if (last) send_to_char( "You don't have enough exp.\n\r", ch );
	return;
    }
   else
    ch->exp		-= cost;
    *pAbility		+= 1;
    if ( !str_cmp( arg1, "avatar") )
    {
	if (ch->max_hit < 8000)
	{
		send_to_char ("You may not train avatar until you have 8k hp.\n\r", ch);
		return;
	}
        if( !ch->pcdata->race->complete)
        {
         stc("You must have a race first.\n\r", ch);
         return;
        }
	act( "You become an avatar!", ch, NULL, NULL, TO_CHAR );
	sprintf( buf, "%s has become an avatar!",ch->name );
	do_info(ch,buf);
        if( ch->trust < 3)
         ch->trust = 3;
	if (ch->level < ch->trust) ch->level = ch->trust;
  }
    else if (last)
	act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    return;
}

void primal_buyback( CHAR_DATA *ch)
{
 int primal = 0;
 int count1;
 int count2;
 for( count1 = 15; count1 < 210; count1 +=15)
 {
  for( count2 = 1; count2 < count1; count2++)
   {
    primal += 500 * count2;
   }
 }
 ch->exp += primal;
}

int get_primal_cost( int primal, int value)
{
 int ret_val = 0;
 int counter = 1;
 int holder = primal+1;

 if( value < 0)
  value = 0;
 ret_val += ((holder) * 500); 
 while( (value != 1) && (value > 0))
 {
  ret_val += ((holder+counter)*500);
  counter++;
  value--;
 }
 if( ret_val < 1)
   ret_val = 500;

 return ret_val;
} 


/**************** COMMIT SUICIDE *************/

void do_commit(CHAR_DATA *ch, char *argument)
{

 char buf[MAX_STRING_LENGTH];
 char arg1[MAX_INPUT_LENGTH];
 int cost = 0;

 argument = one_argument(argument, arg1);

 if( IS_NPC(ch) )
   return;

 if( ch->pcdata->remort > 0 )
 {
  stc("Remorts can not be damned.\n\r", ch);
  return;
 }
 if( arg1[0] == '\0' || str_cmp("suicide", arg1) )
 {
   stc("If you wish to commit suicide, you must type it out fully.\n\r", ch);
   return;
 }

 if( ch->fighting != NULL || ch->fight_timer > 0 )
 {
   stc("The #0Angel of Death#n already lurks about you.\n\r", ch);
   return;
 }
 if( ch->level < 3 )
 {
   stc("Only avatars can commit suicide.\n\r", ch);
   return;
 }

 if( IS_DAMNED(ch) ) return;
 


 if( ch->max_hit < 10000 || ch->max_mana < 10000 || ch->max_move < 10000 )
 {
   stc("You must have at least 10000 hp, mana and move.\n\r", ch);
   return;
 }

 if( is_clan_leader(ch) || is_clan_coleader(ch) )
 {
   stc("Leaders of a clan cannot leave their clan that way!\n\r", ch);
   return;
 }
 stc("You kneel down and begin to pray for death.\n\r", ch);
 stc("Suddenly a figure cloaked in black appears before you,\n\r", ch);
 stc("and with a quick slash slices your head from your body!\n\r", ch);
 stc("You here a voice like thunder speak 'YOU HAVE BEEN DAMNED!'\n\r", ch);
 stc("The only way to return to the avatar realm is to repent.\n\r", ch);
 stc("The dark figure vanishes as your limp body falls to the ground.\n\r", ch);

 behead(ch);
 do_sacrifice(ch, "head");
 ch->hit = -10;
 ch->mana = -10;
 ch->move = -10;
 cost = 200 * (ch->race);
 if( cost < 1000 )
   cost = 1000;
 if( cost > 10000 )
   cost = 10000;
 ch->max_hit -= cost;
 ch->max_move -= cost;
 ch->max_mana -= cost;
 ch->level = 4;
 ch->trust = 4;
 sprintf(buf, "You feel a chill in the air as the #0Angel of Death#n answers %s's prayer for death.\n\r", ch->name);
 do_info(ch, buf);
 sprintf(buf, "%s has been damned!\n\r", ch->name);
 do_info(ch, buf);
 set_title(ch, "DAMNED");
 STRFREE(ch->clan);
 ch->clan = str_dup("DAMNED");
 do_save(ch, "");
 return;
}

/*************** END SUICIDE FUNCTION **************/

/*************** REPENT FUNCTION *******************/

void do_repent(CHAR_DATA *ch, char *argument)
{

 char buf[MAX_STRING_LENGTH];

 if( IS_NPC(ch) )
  return;

 if( ch->level != 4 && ch->trust != 4 && !str_cmp(ch->pcdata->title, "DAMNED") )
 {
  stc("You have not been damned.\n\r", ch);
  return;
 }

 stc("A figure shrouded in white appears before you.  You drop to\n\r", ch);
 stc("your knees and ask for repentance.  A voice like thunder speaks\n\r", ch);
 stc("the words 'SALVATION IS YOURS!'\n\r", ch);
 ch->level = 2;
 ch->trust = 2;
 ch->hit = -10;
 ch->mana = -10;
 ch->move = -10;
 sprintf(buf, "%s has repented of their sins!", ch->name);
 do_info(ch, buf);
 set_title(ch, "REPENTANT");
 STRFREE(ch->clan);
 ch->clan = str_dup("None");
 do_save(ch, "");
 return;
}

/*************** END REPENT ************/

/*Kale & SaTaN*/
void do_train(CHAR_DATA *ch, char *argument)
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char buf[MAX_INPUT_LENGTH];
 int cost;
 int value;
 int hp = 0;
 int move = 0;
 int mana = 0;

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if( IS_NPC(ch)) return;

  hp = (30000 + (ch->pcdata->remort * 3000));
  mana = (30000 + (ch->pcdata->remort * 3000));
  move = (30000 + (ch->pcdata->remort * 3000));

 if(arg1[0] == '\0')
 {
  sprintf(buf,"You have %d experience points.\n\r", ch->exp);
  stc(buf, ch);
  stc("You can train the following:\n\r", ch);
  sprintf(buf, "Stats: ");
  if( ch->pcdata->perm_str < 18)
   sprintf(buf+strlen(buf), "Str ");
  if( ch->pcdata->perm_int < 18)
   sprintf(buf+strlen(buf), "Int ");
  if( ch->pcdata->perm_con < 18)
   sprintf(buf+strlen(buf), "Con ");
  if( ch->pcdata->perm_wis < 18)
   sprintf(buf+strlen(buf), "Wis ");
  if( ch->pcdata->perm_dex < 18)
   sprintf(buf+strlen(buf), "Dex ");
  sprintf(buf+strlen(buf), "\n\r");
  stc(buf, ch);
  
  sprintf(buf, "Primal    - %d.\n\r", ((ch->practice+1)*500));
  stc(buf, ch);

  if(ch->max_hit < (hp + (ch->race * 50)))
  {
   sprintf(buf, "Hp        - %d exp per point.\n\r",
    ((ch->max_hit - ch->pcdata->perm_con)*(ch->pcdata->remort + 1)));
   stc(buf, ch);
  }
  if( ch->max_move < (move + (ch->race * 50)))
  {
   sprintf(buf, "Move      - %d exp per point.\n\r",
     ((ch->max_move - ch->pcdata->perm_dex)*(ch->pcdata->remort + 1)));
   stc( buf, ch);
  }
  if( ch->max_mana < (mana + (ch->race * 50)))
  {
   sprintf(buf, "Mana      - %d exp per point.\n\r",
     ((ch->max_mana - ch->pcdata->perm_int)*(ch->pcdata->remort + 1)));
   stc(buf, ch);
  }
  sprintf(buf, "Weapon immunes: ");
  if( !IS_IMMUNE(ch,IMM_SLASH))
   sprintf(buf+strlen(buf), "slash ");
  if( !IS_IMMUNE(ch,IMM_STAB))
   sprintf(buf+strlen(buf), "stab ");
  if( !IS_IMMUNE(ch,IMM_SMASH))
   sprintf(buf+strlen(buf), "smash ");
  if( !IS_IMMUNE(ch,IMM_ANIMAL))
   sprintf(buf+strlen(buf), "animal ");
  if( !IS_IMMUNE(ch,IMM_MISC))
   sprintf(buf+strlen(buf), "grab ");
  sprintf(buf+strlen(buf), "\n\r");
  stc(buf, ch);
  sprintf(buf, "Magical immunities: ");
  if( !IS_IMMUNE(ch,IMM_CHARM))
   sprintf(buf+strlen(buf), "charm ");
  if( !IS_IMMUNE(ch,IMM_HEAT))
   sprintf(buf+strlen(buf), "heat ");
  if( !IS_IMMUNE(ch,IMM_COLD))
   sprintf(buf+strlen(buf), "cold ");
  if( !IS_IMMUNE(ch,IMM_LIGHTNING))
   sprintf(buf+strlen(buf), "lightning ");
  if( !IS_IMMUNE(ch,IMM_ACID))
   sprintf(buf+strlen(buf), "acid ");
  if( !IS_IMMUNE(ch,IMM_SLEEP))
   sprintf(buf+strlen(buf), "sleep ");
  if( !IS_IMMUNE(ch,IMM_DRAIN))
   sprintf(buf+strlen(buf), "drain ");
  if( !IS_IMMUNE(ch,IMM_VOODOO))
   sprintf(buf+strlen(buf), "voodoo ");
  sprintf(buf+strlen(buf), "\n\r");
  stc(buf, ch);
  sprintf( buf, "Skill immunities: ");
  if( !IS_IMMUNE(ch,IMM_HURL))
   sprintf(buf+strlen(buf), "hurl ");
  if( !IS_IMMUNE(ch,IMM_BACKSTAB))
   sprintf(buf+strlen(buf), "backstab ");
  if( !IS_IMMUNE(ch,IMM_KICK))
   sprintf(buf+strlen(buf), "kick ");
  if( !IS_IMMUNE(ch,IMM_DISARM))
   sprintf(buf+strlen(buf), "disarm ");
  if( !IS_IMMUNE(ch,IMM_STEAL))
   sprintf(buf+strlen(buf), "steal ");
   sprintf(buf+strlen(buf), "\n\r");
  if ( !IS_NPC(ch) && ch->level == 2 && ch->max_hit >= 8000 && ch->pcdata->race->complete)
   sprintf( buf+strlen(buf), "Become an avatar - 1000 exp.\n\r");
  stc(buf, ch);
  if( IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,VAMP_AGE) < AGE_METHUSELAH )
  {
   switch( GET_POWERS(ch,VAMP_AGE) )
   {
    case AGE_NONE:    cost = 42188;  break;
    case AGE_CAITIFF: cost = 56250;  break;
    case AGE_ANCILLA: cost = 75000;  break;
    case AGE_ELDER:   cost = 100000; break;
    default: break;
   }
   sprintf(buf,"Age - %d discipline points\n\r",cost);
   stc(buf,ch);
  }
  if( IS_CLASS(ch,CLASS_DROW) && GET_POWERS(ch,DROW_MAGIC) < 100 )
  {
   cost = ((GET_POWERS(ch,DROW_MAGIC)+1)*100);
   sprintf(buf,"Magic Resistance - %d drow points\n\r",cost);
   stc(buf,ch);
   return;
  }
  return;
 }
 if( !str_cmp("hit", arg1) || !str_cmp("hp", arg1))
 {
  if( arg2[0] == '\0') value = 1;
  else
  {
   if( !is_number(arg2))
   {
    stc("You must supply a number.\n\r", ch);
    return;
   }
   value = atoi(arg2);
  } 
  if( (ch->max_hit + value) > (hp + (ch->race * 50)))
  {
   stc("You can't train your hitpoints that much.\n\r", ch);
   return;
  }
  cost = 0;
  cost = ((ch->max_hit - ch->pcdata->perm_con) * value)+(((value/2) * (value)) + (value/2));
  cost *= (ch->pcdata->remort + 1);
  if( cost > ch->exp)
  {
   stc("You cant afford it.\n\r", ch);
   return;
  }
  if( cost < 1000 )
   cost = 1000;
  ch->max_hit += value;
  ch->exp -= cost;
  return;
 }
 else if( !str_cmp("mana", arg1))
 {
  if( arg2[0] == '\0') value = 1;
  else
  {
   if( !is_number(arg2))
   {
    stc("You must supply a number.\n\r", ch);
    return;
   }
   value = atoi(arg2);
  } 
  if( (ch->max_mana + value) > (mana + (ch->race * 50)))
  {
   sprintf( buf, "mana = %d", mana);
   log_string(buf);
   stc("You can't train your mana that much.\n\r", ch);
   return;
  }
  cost = 0;
  cost = ((ch->max_mana - ch->pcdata->perm_int) * value) + (((value/2) * (value)) + (value/2));
  cost *= (ch->pcdata->remort + 1);
  if( cost > ch->exp)
  {
   stc("You cant afford it.\n\r", ch);
   return;
  }
  if( cost < 1000 )
   cost = 1000;
  ch->max_mana += value;
  ch->exp -= cost;
  return;
 }
 else if( !str_cmp("move", arg1))
 {
  if( arg2[0] == '\0') value = 1;
  else
  {
   if( !is_number(arg2))
   {
    stc("You must supply a number.\n\r", ch);
    return;
   }
   value = atoi(arg2);
  } 
  if( (ch->max_move + value) > (move + (ch->race * 50)))
  {
   sprintf( buf, "move = %d", move);
   log_string( buf ); 
   stc("You can't train your move that much.\n\r", ch);
   return;
  }
  cost = 0;
  cost = ((ch->max_move - ch->pcdata->perm_dex) * value) +(((value/2) * (value)) + (value/2));
  cost *= (ch->pcdata->remort + 1);
  if( cost > ch->exp)
  {
   stc("You cant afford it.\n\r", ch);
   return;
  }
  if( cost < 1000 )
   cost = 1000;
  ch->max_move += value;
  ch->exp -= cost;
  return;
 }
 else if( !str_cmp("primal", arg1))
 {
  if( arg2[0] == '\0')
   value = 1;
  else
  {
   if( !is_number(arg2))
   {
    stc("You must supply a number.\n\r", ch);
    return;
   }
   value = atoi(arg2);
   if( value < 1)
   {
    stc("Supply a value of at least 1.\n\r", ch);
    return;
   }
   if( value > 500)
   {
    stc("Value can not be higher than 500.\n\r", ch);
    return;
   }
  }
  cost = 0;
  cost = get_primal_cost(ch->practice, value);
  if( cost > ch->exp)
  {
   stc("You cant afford it.\n\r", ch);
   return;
  }
  ch->practice += value;
  ch->exp -= cost;
  return;
 }
 else if( !str_cmp(arg1, "slash"))
 {
  if( IS_IMMUNE(ch, IMM_SLASH))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_SLASH);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "stab"))
 {
  if( IS_IMMUNE(ch, IMM_STAB))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_STAB);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "smash"))
 {
  if( IS_IMMUNE(ch, IMM_SMASH))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_SMASH);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "animal"))
 {
  if( IS_IMMUNE(ch, IMM_ANIMAL))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_ANIMAL);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "grab"))
 {
  if( IS_IMMUNE(ch, IMM_MISC))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_MISC);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "charm"))
 {
  if( IS_IMMUNE(ch, IMM_CHARM))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_CHARM);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "heat"))
 {
  if( IS_IMMUNE(ch, IMM_HEAT))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_HEAT);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "cold"))
 {
  if( IS_IMMUNE(ch, IMM_COLD))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_COLD);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "lightning"))
 {
  if( IS_IMMUNE(ch, IMM_LIGHTNING))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_LIGHTNING);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "acid"))
 {
  if( IS_IMMUNE(ch, IMM_ACID))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_ACID);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "sleep"))
 {
  if( IS_IMMUNE(ch, IMM_SLEEP))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_SLEEP);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "drain"))
 {
  if( IS_IMMUNE(ch, IMM_DRAIN))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_DRAIN);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "voodoo"))
 {
  if( IS_IMMUNE(ch, IMM_VOODOO))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_VOODOO);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "hurl"))
 {
  if( IS_IMMUNE(ch, IMM_HURL))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_HURL);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "kick"))
 {
  if( IS_IMMUNE(ch, IMM_KICK))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_KICK);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "backstab"))
 {
  if( IS_IMMUNE(ch, IMM_BACKSTAB))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_BACKSTAB);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "disarm"))
 {
  if( IS_IMMUNE(ch, IMM_DISARM))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_DISARM);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "steal"))
 {
  if( IS_IMMUNE(ch, IMM_STEAL))
   return;
  cost = count_imms(ch);
  if( ch->exp < cost)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  SET_BIT(ch->immune, IMM_STEAL);
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg1, "int"))
 {
  cost = 300000;
  if( cost > ch->exp)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( ch->pcdata->perm_int >= 18)
  {
   stc("You have already reached the maximum.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  ch->pcdata->perm_int++;
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg1, "str"))
 {
  cost = 300000;
  if( cost > ch->exp)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( ch->pcdata->perm_str >= 18)
  {
   stc("You have already reached the maximum.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  ch->pcdata->perm_str++;
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg1, "wis"))
 {
  cost = 300000;
  if( cost > ch->exp)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( ch->pcdata->perm_wis >= 18)
  {
   stc("You have already reached the maximum.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  ch->pcdata->perm_wis++;
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg1, "con"))
 {
  cost = 300000;
  if( cost > ch->exp)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( ch->pcdata->perm_con >= 18)
  {
   stc("You have already reached the maximum.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  ch->pcdata->perm_con++;
  stc("Done.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg1, "dex"))
 {
  cost = 300000;
  if( cost > ch->exp)
  {
   stc("You can't afford it.\n\r", ch);
   return;
  }
  if( ch->pcdata->perm_dex >= 18)
  {
   stc("You have already reached the maximum.\n\r", ch);
   return;
  }
  ch->exp -= cost;
  ch->pcdata->perm_dex++;
  stc("Done.\n\r", ch);
  return;
 }
 else if ( !str_cmp( arg1, "avatar") && (!ch->pcdata->race->complete))
 {
  send_to_char ("You may not train avatar until you set your class.\n\r", ch);
  return;
 }
 else if( !str_cmp( arg1, "avatar") && (ch->level == 2) && ch->pcdata->race->complete)
 {
  if( IS_SET(ch->special, SPC_NEWBIEPASS))
  {
   stc("WARNING: if you type train avatar again you will be able to be pkilled.\n\r", ch);
   stc("DO NOT train avatar unless you are sure you are ready to fight the other players.\n\r", ch);
   REMOVE_BIT(ch->special, SPC_NEWBIEPASS);
   return;
  }
  if( deathmatch )
  {
   stc("Wait till the deathmatch is over!\n\r",ch);
   return;
  }
  cost = 1000;
  act( "You become an avatar!", ch, NULL, NULL, TO_CHAR );
  sprintf( buf, "%s has become an avatar!",ch->name );
  do_info(ch,buf);
  if( ch->trust < 3) ch->trust = 3;
  if (ch->level < ch->trust) ch->level = ch->trust;
  if( !str_cmp(ch->clan, "DAMNED") )
  {
   ch->level = 4;
   ch->trust = 4;
  }
 }
 else if( !str_cmp(arg1,"age") && IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,VAMP_AGE) < AGE_METHUSELAH )
 {
  switch( GET_POWERS(ch,VAMP_AGE) )
  {
   case AGE_NONE:    cost = 42188;  break;
   case AGE_CAITIFF: cost = 56250;  break;
   case AGE_ANCILLA: cost = 75000;  break;
   case AGE_ELDER:   cost = 100000; break;
   default: break;
  }
  if( GET_POWERS(ch,VAMP_POINTS) < cost )
  {
   sprintf(buf,"You lack the %d discipline points to further your age.\n\r",cost);
   stc(buf,ch);
   return;
  }
  GET_POWERS(ch,VAMP_AGE)++;
  GET_POWERS(ch,VAMP_POINTS) -= cost;
  stc("You have risen to a new level of vampiric power!\n\r",ch);
 }
 else if( !str_cmp(arg1,"magic") && IS_CLASS(ch,CLASS_DROW) && GET_POWERS(ch,DROW_MAGIC) < 100 )
 {
  cost = ((GET_POWERS(ch,DROW_MAGIC)+1)*100);
  if( GET_POWERS(ch,DROW_POINTS) < cost )
  {
   sprintf(buf,"You lack the %d drow points to enhance your magic resistance.\n\r",cost);
   stc(buf,ch);
   return;
  }
  GET_POWERS(ch,DROW_MAGIC)++;
  GET_POWERS(ch,DROW_POINTS) -= cost;
 }
 else
 {
  stc("Train what?\n\r", ch);
  return;
 }
 return;
}
/* /SaTaN */

void do_mount( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Mount what?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_POLYMORPH))
    {
	send_to_char( "You cannot ride in this form.\n\r",ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r",ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot ride on your own back!\n\r",ch );
	return;
    }

    if ( ch->mounted > 0 )
    {
	send_to_char( "You are already riding.\n\r",ch );
	return;
    }

    if ( !IS_NPC(victim) || victim->mounted > 0 || (IS_NPC(victim) && !IS_SET(victim->act, ACT_MOUNT) ) )
    {
	send_to_char( "You cannot mount them.\n\r",ch );
	return;
    }

    if ( victim->position < POS_STANDING )
    {
	if ( victim->position < POS_SLEEPING )
	    act( "$N is too badly hurt for that.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SLEEPING )
	    act( "First you better wake $m up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_RESTING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_MEDITATING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SITTING )
	    act( "First $e better stand up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_SLEEPING )
	    act( "First you better wake $m up.", ch, NULL, victim, TO_CHAR );
	else if ( victim->position == POS_FIGHTING )
	    act( "Not while $e's fighting.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (!IS_NPC(ch) && ch->stance[0] != -1 && ((ch->trust > 2) || (ch->level > 2))) do_stance(ch,"");

    ch->mounted     = IS_RIDING;
    victim->mounted = IS_MOUNT;
    ch->mount = victim;
    victim->mount = ch;

    act( "You clamber onto $N's back.", ch, NULL, victim, TO_CHAR );
    act( "$n clambers onto $N's back.", ch, NULL, victim, TO_ROOM );
    return;
}

void do_dismount( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( ch->mounted == 0 )
    {
	send_to_char( "But you are not riding!\n\r",ch );
	return;
    }

    if ( (victim = ch->mount) == NULL )
    {
	send_to_char( "But you are not riding!\n\r",ch );
	return;
    } 

    act( "You clamber off $N's back.", ch, NULL, victim, TO_CHAR );
    act( "$n clambers off $N's back.", ch, NULL, victim, TO_ROOM );

    ch->mounted     = IS_ON_FOOT;
    victim->mounted = IS_ON_FOOT;

    ch->mount = NULL;
    victim->mount = NULL;

    return;
}

void do_tie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

	if (ch->level < 3)
	{
		send_to_char("Can't do this as a mortal.\n\r", ch);
		return;
	}
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	if (IS_NPC(victim))
	{
		send_to_char("You cannot do that to a mob.\n\r", ch);
		return;
	}
    if (victim == ch)
    {
	send_to_char( "You cannot tie yourself up!\n\r", ch );
	return;
    }
    if (IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "But they are already tied up!\n\r", ch );
	return;
    }
    if (victim->position > POS_STUNNED || victim->hit > 0)
    {
	send_to_char( "You can only tie up a defenceless person.\n\r", ch );
	return;
    }
    act("You quickly tie up $N.",ch,NULL,victim,TO_CHAR);
    act("$n quickly ties up $N.",ch,NULL,victim,TO_ROOM);
    send_to_char("You have been tied up!\n\r",victim);
    SET_BIT(victim->extra,TIED_UP);
    sprintf(buf,"%s has been tied up by %s.",victim->name,ch->name);
    do_info(ch,buf);
    return;
}

void do_untie( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "But they are not tied up!\n\r", ch );
	return;
    }
    if (victim == ch)
    {
	send_to_char( "You cannot untie yourself!\n\r", ch );
	return;
    }
    act("You quickly untie $N.",ch,NULL,victim,TO_CHAR);
    act("$n quickly unties $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n quickly unties you.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,TIED_UP);
    return;
}

void do_gag( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch && !IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "You cannot gag yourself!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP) && !IS_EXTRA(victim,GAGGED))
    {
	send_to_char( "You can only gag someone who is tied up!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,GAGGED))
    {
    	act("You place a gag over $N's mouth.",ch,NULL,victim,TO_CHAR);
    	act("$n places a gag over $N's mouth.",ch,NULL,victim,TO_NOTVICT);
    	act("$n places a gag over your mouth.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->extra,GAGGED);
	return;
    }
    if (ch == victim)
    {
    	act("You remove the gag from your mouth.",ch,NULL,victim,TO_CHAR);
    	act("$n removes the gag from $s mouth.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->extra,GAGGED);
	return;
    }
    act("You remove the gag from $N's mouth.",ch,NULL,victim,TO_CHAR);
    act("$n removes the gag from $N's mouth.",ch,NULL,victim,TO_NOTVICT);
    act("$n removes the gag from your mouth.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,GAGGED);
    return;
}

void do_blindfold( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (victim == ch && !IS_EXTRA(victim,BLINDFOLDED) && IS_EXTRA(victim,TIED_UP))
    {
	send_to_char( "You cannot blindfold yourself!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,TIED_UP) && !IS_EXTRA(victim,BLINDFOLDED))
    {
	send_to_char( "You can only blindfold someone who is tied up!\n\r", ch );
	return;
    }
    if (!IS_EXTRA(victim,BLINDFOLDED))
    {
    	act("You place a blindfold over $N's eyes.",ch,NULL,victim,TO_CHAR);
    	act("$n places a blindfold over $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    	act("$n places a blindfold over your eyes.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->extra,BLINDFOLDED);
	return;
    }
    if (ch == victim)
    {
    	act("You remove the blindfold from your eyes.",ch,NULL,victim,TO_CHAR);
    	act("$n removes the blindfold from $s eyes.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->extra,BLINDFOLDED);
	return;
    }
    act("You remove the blindfold from $N's eyes.",ch,NULL,victim,TO_CHAR);
    act("$n removes the blindfold from $N's eyes.",ch,NULL,victim,TO_NOTVICT);
    act("$n removes the blindfold from your eyes.",ch,NULL,victim,TO_VICT);
    REMOVE_BIT(victim->extra,BLINDFOLDED);
    return;
}


int count_imms( CHAR_DATA *ch )
{
    int count = 0;

    if ( IS_IMMUNE(ch, IMM_SLASH)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_STAB)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_SMASH)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_ANIMAL)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_MISC)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_CHARM)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_HEAT)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_COLD)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_LIGHTNING) ) count += 1;
    if ( IS_IMMUNE(ch, IMM_ACID)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_SLEEP)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_DRAIN)     ) count += 1;
    if ( IS_IMMUNE(ch, IMM_VOODOO)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_HURL)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_BACKSTAB)  ) count += 1;
    if ( IS_IMMUNE(ch, IMM_KICK)      ) count += 1;
    if ( IS_IMMUNE(ch, IMM_DISARM)    ) count += 1;
    if ( IS_IMMUNE(ch, IMM_STEAL)     ) count += 1;
    return ( ( count * 10000 ) + 10000 );
}

void do_track( CHAR_DATA *ch, char *argument )
{
    bool found = FALSE;
    if (!IS_NPC(ch) && ch->position == POS_MORTAL)
    {
        send_to_char("You are too hurt to track anyone!\n\r", ch);
        return;
    }
    if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_track])
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	return;
    }
    if (check_track(ch,0)) found = TRUE;
    if (check_track(ch,1)) found = TRUE;
    if (check_track(ch,2)) found = TRUE;
    if (check_track(ch,3)) found = TRUE;
    if (check_track(ch,4)) found = TRUE;
    if (found == FALSE)
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	return;
    }
    act("$n carefully examines the ground for tracks.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_hunt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	if (strlen(ch->hunting) > 1)
	{
	    free_string(ch->hunting);
	    ch->hunting = str_dup( "" );
	    send_to_char( "You stop hunting your prey.\n\r", ch );
	}
	else send_to_char( "Who do you wish to hunt?\n\r", ch );
	return;
    }
    if (!str_cmp(arg,ch->name))
    {
	send_to_char("How can you hunt yourself?\n\r",ch);
	return;
    }
    if (ch->position == POS_MORTAL)
    {
        send_to_char("You are too hurt to hunt!\n\r", ch);
        return;
    }
    ch->hunting = str_dup(arg);
    send_to_char( "Ok.\n\r", ch );
    return;
}

void check_hunt( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    bool found = FALSE;
    int direction = 0;
    int i = 0;
    ROOM_INDEX_DATA *in_room;

    in_room = ch->in_room;
    if (!IS_NPC(ch) && number_percent() > ch->pcdata->learned[gsn_track])
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
	return;
    }
    if (check_track(ch,0)) {found = TRUE;direction = ch->in_room->track_dir[0];}
    else if (check_track(ch,1)) {found = TRUE;direction = ch->in_room->track_dir[1];}
    else if (check_track(ch,2)) {found = TRUE;direction = ch->in_room->track_dir[2];}
    else if (check_track(ch,3)) {found = TRUE;direction = ch->in_room->track_dir[3];}
    else if (check_track(ch,4)) {found = TRUE;direction = ch->in_room->track_dir[4];}
    else if ( ( victim = get_char_room( ch, ch->hunting ) ) == NULL )
    {
	send_to_char("You cannot sense any trails from this room.\n\r",ch);
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
	return;
    }
    if (strlen(ch->hunting) < 2) return;
    if ( ( victim = get_char_room( ch, ch->hunting ) ) != NULL ) return;
    act("$n carefully examines the ground for tracks.",ch,NULL,NULL,TO_ROOM);
    for( i = 0 ; i < 10 ; i++ )
     move_char(ch,direction);
    if( i == 10 )
    {
     free_string(ch->hunting);
     ch->hunting = str_dup( "" );
     i = 0;
     stc("You pause in your pursuit.\n\r", ch);
     return;
    }
    if (in_room == ch->in_room || victim != NULL)
    {
	free_string(ch->hunting);
	ch->hunting = str_dup( "" );
    }
    return;
}

void add_tracks( CHAR_DATA *ch, int direction )
{
    int loop;

    if (IS_NPC(ch)) return;
    if (IS_ITEMAFF(ch, ITEMA_STALKER)) return;
    if( IS_CLASS(ch, CLASS_SHADOW) && ( GET_POWERS(ch, SHADOW_TRACK) > 2 ) )
     return;
    for ( loop = 0; loop <= 4; loop++ )
    {
    	if (ch->in_room->track[loop] != NULL && !str_cmp(ch->in_room->track[loop],ch->name))
    	{
	    free_string(ch->in_room->track[loop]);
	    ch->in_room->track[loop] = str_dup("");
    	}
    }
    if (strlen(ch->in_room->track[0]) < 2)
    {
	free_string(ch->in_room->track[0]);
	ch->in_room->track[0]     = str_dup(ch->name);
	ch->in_room->track_dir[0] = direction;
    }
    else if (strlen(ch->in_room->track[1]) < 2)
    {
	free_string(ch->in_room->track[1]);
	ch->in_room->track[1]     = str_dup(ch->name);
	ch->in_room->track_dir[1] = direction;
    }
    else if (strlen(ch->in_room->track[2]) < 2)
    {
	free_string(ch->in_room->track[2]);
	ch->in_room->track[2]     = str_dup(ch->name);
	ch->in_room->track_dir[2] = direction;
    }
    else if (strlen(ch->in_room->track[3]) < 2)
    {
	free_string(ch->in_room->track[3]);
	ch->in_room->track[3]     = str_dup(ch->name);
	ch->in_room->track_dir[3] = direction;
    }
    else if (strlen(ch->in_room->track[4]) < 2)
    {
	free_string(ch->in_room->track[4]);
	ch->in_room->track[4]     = str_dup(ch->name);
	ch->in_room->track_dir[4] = direction;
    }
    else
    {
	free_string(ch->in_room->track[0]);
	ch->in_room->track[0]     = str_dup(ch->in_room->track[1]);
	ch->in_room->track_dir[0] = ch->in_room->track_dir[1];
	free_string(ch->in_room->track[1]);
	ch->in_room->track[1]     = str_dup(ch->in_room->track[2]);
	ch->in_room->track_dir[1] = ch->in_room->track_dir[2];
	free_string(ch->in_room->track[2]);
	ch->in_room->track[2]     = str_dup(ch->in_room->track[3]);
	ch->in_room->track_dir[2] = ch->in_room->track_dir[3];
	free_string(ch->in_room->track[3]);
	ch->in_room->track[3]     = str_dup(ch->in_room->track[4]);
	ch->in_room->track_dir[3] = ch->in_room->track_dir[4];
	free_string(ch->in_room->track[4]);
	ch->in_room->track[4]     = str_dup(ch->name);
	ch->in_room->track_dir[4] = direction;
    }
    return;
}

bool check_track( CHAR_DATA *ch, int direction )
{
    CHAR_DATA *victim;
    char buf [MAX_INPUT_LENGTH];
    char vict [MAX_INPUT_LENGTH];
    int door;

    strcpy(vict,ch->hunting);

    if (strlen(ch->hunting) > 1)
    {
	if (!str_cmp(ch->hunting,vict))
	{
	    if ( ( victim = get_char_room( ch, vict ) ) != NULL )
	    {
		act("You have found $N!",ch,NULL,victim,TO_CHAR);
		free_string(ch->hunting);
		ch->hunting = str_dup( "" );
		return TRUE;
	    }
	}
    }

    if (strlen(ch->in_room->track[direction]) < 2) return FALSE;
    if (!str_cmp(ch->in_room->track[direction],ch->name)) return FALSE;
    if (strlen(ch->hunting) > 1 && str_cmp(ch->in_room->track[direction],ch->hunting)) return FALSE;
    door = ch->in_room->track_dir[direction];
    sprintf(buf,"You sense the trail of %s leading $T from here.",ch->in_room->track[direction]);
    act( buf, ch, NULL, dir_name[door], TO_CHAR );
    return TRUE;
}

void do_autostance( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 stc("We don't offer a 'complete' autostance here as we think it is quite lame, and lacking in skill.\n\r",ch);
 stc("However, if you are mortal, you can stance and it won't come undone while moving!\n\r",ch);
 stc("Avatars, however, do not have this luxury.\n\r",ch);
 return;
}

/******** DRAG HIDING PLAYERS FROM PRIVATE ROOMS ********/


/* void do_drag( CHAR_DATA *ch, char *argument )
{

   CHAR_DATA *victim;
   ROOM_INDEX_DATA *to_room;
   EXIT_DATA *pexit;
   char arg[MAX_INPUT_LENGTH];
   int door;
   int dam;
   
   argument = one_argument(argument, arg);

   if( arg[0] == '\0' )
   {
	stc("Drag who?\n\r", ch);
	return;
   }
 
   if( (victim = get_char_world(ch, arg)) == NULL )
   {
	stc("They are not here.\n\r", ch);
	return;
   }

   if( IS_NPC(victim) )
   {
	stc("Not on mobs.\n\r", ch);
	return;
   }

   if( IS_IMMORTAL(victim) )
   {
	stc("Not on Immortals!\n\r", ch);
	return;
   }

   if( (victim = get_char_area(ch, arg)) == NULL )
   {
	stc("You're not even in the same area!\n\r", ch);
	return;
   }

   if( IS_DAMNED(ch) || IS_DAMNED(victim) ) return;

   if( ch->in_room == victim->in_room )
   {
	stc("You are already in the same room!\n\r", ch);
	return;
   }

   if( ch->move < 5000 )
   {
	stc("You are too exhausted to drag anyone.\n\r", ch);
	return;
   }


   for( door = 0; door < 6; door++ )
   {
	if( (pexit = ch->in_room->exit[door]) == NULL 
	||  (to_room = pexit->to_room)        == NULL )
	   continue;


	if( victim->in_room == pexit->to_room )
	{
	   if( !IS_SET(victim->in_room->room_flags, ROOM_PRIVATE) )
	   {
		stc("That room isn't private, just walk in!\n\r", ch);
		return;
	   }
		

	   char_from_room(victim);
	   char_to_room(victim, ch->in_room);
           dam = ( victim->race * 250 );
           if( dam < 250 )
            dam = 250;
           dam *= ( 1 + (ch->move / ch->max_move) );
           dam += number_range(1,500);
	   act2("You drag $N from $S hiding spot, catching $M by surprise! [$D]", ch, NULL, victim, TO_CHAR,dam);
	   act2("$n drags you from your hiding spot, catching you by surprise! [$D]", ch, NULL, victim, TO_VICT,dam);
	   act2("$n drags $N from $S hiding spot, catching $M by surprise! [$D]", ch, NULL, victim, TO_NOTVICT,dam);
	   victim->hit -= dam;
	   ch->move -= 5000;
	   return;
	}
   }

   stc("You aren't even standing outside their hiding spot.\n\r", ch);
   return;
}
*/
/******* END FUNCTION DO_DRAG *******/

void do_scry( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 AREA_DATA *area;
 ROOM_INDEX_DATA *room;
 char arg[MAX_INPUT_LENGTH];
 sh_int cost = 500;
 argument = one_argument( argument, arg );

 if( IS_CLASS(ch,CLASS_NONE) )
 {
  do_huh(ch,"");
  return;
 }
 else if( IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,DISC_AUSPEX) < 1 )
 {
  stc("You need Auspex 1 to scry.\n\r",ch);
  return;
 }
 else if( IS_CLASS(ch,CLASS_WOLF) && GET_POWERS(ch,DISC_OWL) < 1 )
 {
  stc("You need Owl 1 to scry.\n\r",ch);
  return;
 }
 else if( IS_CLASS(ch,CLASS_ALIEN) && GET_POWERS(ch,ALIEN_SENSE) < 1 )
 {
  stc("You need to train your senses to level 1 to scry.\n\r",ch);
  return;
 }
 else if( IS_CLASS(ch,CLASS_MONK) )
 {
  do_clairvoyance(ch,arg);
  return;
 }
 else if( IS_CLASS(ch,CLASS_SHADOW) && GET_POWERS(ch,SHADOW_TRACK) < 5 )
 {
  stc("You must first fully master the art of tracking to use scry.\n\r",ch);
  return;
 }
 else if( IS_CLASS(ch,CLASS_DRAGON) )
 {
  do_dscan(ch,arg);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You lack the strength to see through the ethers of space and time.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Scry who?\n\r",ch);
  return;
 }
 if( (victim = get_char_world(ch,arg)) == NULL )
 {
  stc("They are not here.\n\r",ch);
  return;
 }
 if( IS_IMMORTAL(victim) )
 {
  stc("You cannot scry Immortals.\n\r", ch);
  return;
 }
 if( victim->in_room == NULL)
 {
  bug("Do_scry: victim in NULL room.\n\r",0);
  return;
 }
 if( (area = victim->in_room->area) == NULL )
 {
  bug("Do_scry: victim in NULL area.\n\r",0);
  return;
 }
 room = ch->in_room;
 if( IS_CLASS(ch,CLASS_APOCRYPHIC) && has_power_level(ch,SCHOOL_ARCANE,2) )
 {
  sprintf(arg,"You sense $N's presence in %s.",area->name);
  act(arg,ch,NULL,victim,TO_CHAR);
 }
 char_from_room(ch);
 char_to_room(ch,victim->in_room);
 if( IS_SET(ch->affected_by,AFF_SHADOWPLANE) )  
 {
  REMOVE_BIT(ch->affected_by,AFF_SHADOWPLANE);
  do_look(ch,"auto");
  SET_BIT(ch->affected_by,AFF_SHADOWPLANE);
 }
 else
  do_look(ch,"auto");
 if( !IS_NPC(victim) )
 {
  if( IS_CLASS(victim,CLASS_MONK) && HAS_TECH(victim,TECH_CLAIR) )
   stc("You sense that someone is watching you.\n\r",victim);
  else if( IS_CLASS(victim,CLASS_JEDI) )
   stc("You sense movement in the force.\n\r",victim);
  else if( IS_CLASS(victim,CLASS_APOCRYPHIC) && has_power_level(ch,SCHOOL_ARCANE,7) )
   stc("You feel a magical presence watching you.\n\r",victim);
 }
 char_from_room(ch);
 char_to_room(ch,room);
 ch->mana -= cost;
 FIGHT_TIMER(ch,5);
 return;
}
