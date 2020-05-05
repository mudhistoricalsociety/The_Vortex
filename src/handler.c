
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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"







AFFECT_DATA *		affect_free;






/*

 * Local functions.

 */




void config_who_exp()
{
 DESCRIPTOR_DATA *d;
 float value = 0.0;

 value = 1.0;

 for( d = descriptor_list; d != NULL; d = d->next)
 {
  if( d->connected != CON_PLAYING)
   continue;
  if( !d->character)
   continue;
  if( d->character->trust > 6)
   continue;
  value += 0.05;
 }
 exp_who = value;
 return;
}

int count_mobs( CHAR_DATA *ch)
{
 int count = 0;
 CHAR_DATA *mob;

 if( ch->in_room == NULL)
  return count;
 for( mob = ch->in_room->people; mob; mob = mob->next_in_room)
 {
  if( mob == ch)
   continue;
  if( !IS_NPC(mob))
   continue;
  if(mob->pIndexData->vnum != 26016)
   continue;
  count++;
 }
 return count;
}
 
bool safe_room(CHAR_DATA *ch)
{
 if(IS_NPC(ch))
  return FALSE;
 if( ch->fight_timer > 0)
  return FALSE;
 if( (ch->race >= 5) && (ch->level >= 3))
  return FALSE;
 if( ch->pkill >= 5 && ch->level >= 3 )
  return FALSE;
 if( ch->level < 3)
  return TRUE;
 if( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_SAFE) && (ch->race <= 4) && (ch->level >= 3))
  return TRUE;
 else 
  return FALSE;
}
/* 

 * Retrieve a character's trusted level for permission checking.

 */

int get_trust( CHAR_DATA *ch )

{

    if ( ch->desc != NULL && ch->desc->original != NULL )

	ch = ch->desc->original;



    if ( ch->trust != 0 )

	return ch->trust;



    if ( IS_NPC(ch) && ch->level >= LEVEL_HERO )

	return LEVEL_HERO - 1;

    else

	return ch->level;

}

void save_commands()
{
 FILE *fp;
 int i = 0;
 char code[MAX_STRING_LENGTH];

 fp = fopen(COMMAND_FILE, "w");
 if( !fp)
 {
  bug("Cannote open command file for saving.", 0);
  return;
 }
 for(i= 0; cmd_table[i].name[0] != '\0';i++)
 {
  code[0] = '\0';
  strcat(code, "do_");
  strcat(code, cmd_table[i].name);
  fprintf( fp, "%s\n", "#COMMAND");
  fprintf( fp, "%s\n", cmd_table[i].name);
  fprintf( fp, "%s\n", code);
  fprintf( fp, "%d\n", cmd_table[i].position);
  fprintf( fp, "%d\n", cmd_table[i].level);
  fprintf( fp, "%d\n", cmd_table[i].log);
 }
 fprintf( fp, "#END");
 fclose(fp);
 return;
}

void perfect_diamond(CHAR_DATA *ch)
{
  OBJ_INDEX_DATA *pObjIndex;
  OBJ_DATA *obj;
  int gem_type = number_range(1, MAX_GEM);
  char buf[MAX_STRING_LENGTH];

  if( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
  {
   log_string("Error... missing object, please inform Kale.\n\r");
   return;
  }

  obj = create_object( pObjIndex, 25 );
  obj->item_type = ITEM_GEM;
  SET_BIT(obj->value[0], GEM_MASTER);
  SET_BIT(obj->value[0], GEM_POLISHED);
  SET_BIT(obj->value[0], GEM_FLAWLESS);
  SET_BIT(obj->value[1], gem_type);
  set_gem_name(obj);
  obj_to_char(obj, ch);
  sprintf(buf, "A perfect #7%s#n appears in your hands!\n\r", obj->name);
  stc(buf, ch);
  return;
}

void make_gem(OBJ_DATA *obj)
{
  int cut = number_range(1,100);
  int polish = number_range(1,100);
  int status = number_range(1,100);
  int gem_type = number_range(1, MAX_GEM);

  obj->item_type = ITEM_GEM;

  if( cut <= 40)
   SET_BIT(obj->value[0], GEM_UNCUT);
  else if( (cut > 40) && (cut <= 65))
   SET_BIT(obj->value[0], GEM_SIMPLE);
  else if( (cut > 65) && (cut <= 90))
   SET_BIT(obj->value[0], GEM_COMPLEX);
  else
   SET_BIT(obj->value[0], GEM_MASTER);

  if( polish <= 55)
   SET_BIT(obj->value[0], GEM_UNPOLISHED);
  else
   SET_BIT(obj->value[0], GEM_POLISHED);

  if( status <= 25)
   SET_BIT(obj->value[0], GEM_CRACKED);
  else if( (status > 25) && (status <= 55))
   SET_BIT(obj->value[0], GEM_FLAWED);
  else if( (status > 55) && (status <= 65))
   SET_BIT(obj->value[0], GEM_HAZY);
  else if( (status > 65) && (status <= 90))
   SET_BIT(obj->value[0], GEM_CLEAR);
  else
   SET_BIT( obj->value[0], GEM_FLAWLESS);

  SET_BIT(obj->value[1], gem_type);
  set_gem_name(obj);
  return;
}

void set_gem_name( OBJ_DATA *obj)
{

  char cutname[MAX_STRING_LENGTH];
  char polname[MAX_STRING_LENGTH];
  char staname[MAX_STRING_LENGTH];
  char typname[MAX_STRING_LENGTH];
  char sh_name[MAX_STRING_LENGTH];
  char lo_name[MAX_STRING_LENGTH];
  char name[MAX_STRING_LENGTH];

  static const char *cut[5] = {"", "An uncut,", "A simply cut,", "A complex cut,",
                               "A masterfully cut,"};
  static const char *polish[3] = {"", "unpolished,", "polished,"};
  static const char *status[6] = {"", "cracked", "flawed", "hazy","clear", "flawless"};
  static const char *type[11] = {"", "A diamond", "A ruby", "An emerald","A sapphire", "An onyx", 
                               "A tourmaline", "A jade", "A quartz", "An opal","A fire opal"};

  if( IS_SET(obj->value[0], GEM_UNCUT))
   sprintf(cutname, cut[1]);
  else if( IS_SET(obj->value[0], GEM_SIMPLE))
   sprintf( cutname, cut[2]);
  else if( IS_SET(obj->value[0], GEM_COMPLEX))
   sprintf( cutname, cut[3]);
  else
   sprintf( cutname, cut[4]);

  if( IS_SET(obj->value[0], GEM_UNPOLISHED))
   sprintf( polname, polish[1]);
  else
   sprintf( polname, polish[2]);
  
  if( IS_SET(obj->value[0], GEM_CRACKED))
   sprintf( staname, status[1]);
  else if( IS_SET(obj->value[0], GEM_FLAWED))
   sprintf( staname, status[2]);
  else if( IS_SET(obj->value[0], GEM_HAZY))
   sprintf( staname, status[3]);
  else if( IS_SET(obj->value[0], GEM_CLEAR))
   sprintf( staname, status[4]);
  else
   sprintf( staname, status[5]);

  sprintf( typname, type[obj->value[1]]);

  sprintf( name, "gem %s %s %s %s", cutname, staname, polname,typname);
  sprintf( sh_name, "%s", typname);
  free_string( obj->name);
  obj->name = str_dup(name);
  free_string(obj->short_descr);
  obj->short_descr = str_dup(sh_name);
  sprintf( lo_name, "%s lies in the dust at your feet.",typname);
  free_string( obj->description);
  obj->description = str_dup(lo_name);
  return;
}

void make_ore(OBJ_DATA *obj)
{
 static const char *name[3] = {"copper", "tin", "iron"};
 int type = number_range(1,100);
 sh_int ore;
 char buf[MAX_STRING_LENGTH];

 obj->item_type = ITEM_ORE;
 
 if( type < 50)
 {
  ore = 0;
  obj->value[0] = ORE_COPPER;
 }
 else if( (type >= 50) && (type < 90))
 {
  ore = 1;
  obj->value[0] = ORE_TIN;
 }
 else
 {
  ore = 2;
  obj->value[0] = ORE_IRON;
 }
 sprintf(buf, "ore %s", name[ore]);
 free_string(obj->name);
 obj->name = str_dup(buf);
 sprintf(buf, "A lump of %s ore.", name[ore]);
 free_string(obj->short_descr);
 obj->short_descr = str_dup(buf);
 sprintf(buf, "A lump of %s ore lies in the dust.", name[ore]);
 free_string(obj->description);
 obj->description = str_dup(buf);
 return;
}

/*

 * Retrieve a character's age.

 */

int get_age( CHAR_DATA *ch )

{

    return 17 + ( ch->played + (int) (current_time - ch->logon) ) / 7200;

}







/*

 * Retrieve character's current strength.

 */

int get_curr_str( CHAR_DATA *ch )

{

    int max;



    if ( IS_NPC(ch) )

	return 13;



    max = 25;



    return URANGE( 3, ch->pcdata->perm_str + ch->pcdata->mod_str, max );

}







/*

 * Retrieve character's current intelligence.

 */

int get_curr_int( CHAR_DATA *ch )

{

    int max;



    if ( IS_NPC(ch) )

	return 13;



    max = 25;



    return URANGE( 3, ch->pcdata->perm_int + ch->pcdata->mod_int, max );

}







/*

 * Retrieve character's current wisdom.

 */

int get_curr_wis( CHAR_DATA *ch )

{

    int max;



    if ( IS_NPC(ch) )

	return 13;



    max = 25;



    return URANGE( 3, ch->pcdata->perm_wis + ch->pcdata->mod_wis, max );

}







/*

 * Retrieve character's current dexterity.

 */

int get_curr_dex( CHAR_DATA *ch )

{

    int max;



    if ( IS_NPC(ch) )

	return 13;



    max = 25;



    return URANGE( 3, ch->pcdata->perm_dex + ch->pcdata->mod_dex, max );

}







/*

 * Retrieve character's current constitution.

 */

int get_curr_con( CHAR_DATA *ch )

{

    int max;



    if ( IS_NPC(ch) )

	return 13;



    max = 25;



    return URANGE( 3, ch->pcdata->perm_con + ch->pcdata->mod_con, max );

}

void put_wall(ROOM_INDEX_DATA *room, int dir, int wall)
{
    int timer;   

    timer = dice(2, 2);

    switch (dir)
    {
        case DIR_NORTH :
            SET_RTIMER(room, RTIMER_WALL_NORTH, timer);
            SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_SOUTH, timer);
        break;
        case DIR_EAST :
            SET_RTIMER(room, RTIMER_WALL_EAST, timer);
            SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_WEST, timer);
        break;
        case DIR_SOUTH :
            SET_RTIMER(room, RTIMER_WALL_SOUTH, timer);
            SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_NORTH, timer);
        break;
        case DIR_WEST :
            SET_RTIMER(room, RTIMER_WALL_WEST, timer);
            SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_EAST, timer);
        break;
        case DIR_UP :
            SET_RTIMER(room, RTIMER_WALL_UP, timer);
            SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_DOWN, timer);
        break;
        case DIR_DOWN :
            SET_RTIMER(room, RTIMER_WALL_DOWN, timer);
            SET_RTIMER(room->exit[dir]->to_room, RTIMER_WALL_UP, timer);
        break;
    }
    room->exit[dir]->exit_info = wall;   
}

const sh_int  revdir [6] =
{
    2, 3, 0, 1, 5, 4
};
void make_wall(ROOM_INDEX_DATA *room, int dir, int wall)
{   
    int was;
    bool makewall = TRUE;   
    if (dir < 0 || dir > 5)
    {
        bug("Invalid direction (create wall)", 0);
        return;
    }   
    if (room->exit[dir] == NULL)
    {
        bug ("Cant make wall!!!!", 0);
        return;
    }
    was = room->exit[dir]->exit_info;       
    if (was == EX_ICE_WALL && wall == EX_FIRE_WALL)
    {
        room_message(room, "The wall of ice bursts into flames.");
        room_message(room->exit[dir]->to_room, "The wall of ice bursts into flames.");
    }   
    if (was == EX_FIRE_WALL && wall == EX_ICE_WALL)
    {
        room_message(room, "The wall of flames flickers blue for a second.");
        room_message(room->exit[dir]->to_room, "The wall of flames flickers blue for a second.");
        makewall = FALSE;
    }
    if (makewall) 
     put_wall(room, dir, wall);
    if (room->exit[dir]->to_room == NULL)
    {
        bug("Exit doesnt lead anywhere (create wall).", 0);
        return;
    }
    if (room->exit[dir]->to_room->exit[revdir[dir]] == NULL)
    {
        bug("Reverse Exit doesnt lead anywhere (create wall).", 0);
        return;
    }    
    if (makewall) put_wall(room->exit[dir]->to_room, revdir[dir], wall);
}

/*

 * Retrieve a character's carry capacity.

 */

int can_carry_n( CHAR_DATA *ch )

{

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )

	return 1000;



    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )

	return 0;



    return MAX_WEAR + 2 * get_curr_dex( ch ) / 3;

}







/*

 * Retrieve a character's carry capacity.

 */

int can_carry_w( CHAR_DATA *ch )

{

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )

	return 1000000;



    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )

	return 0;



    return str_app[get_curr_str(ch)].carry;

}









/*

 * See if a string is one of the names of an object.

 */

bool is_name( const char *str, char *namelist )

{

    char name[MAX_INPUT_LENGTH];



    for ( ; ; )

    {

	namelist = one_argument( namelist, name );

	if ( name[0] == '\0' )

	    return FALSE;

        if ( !str_prefix( str, name ) )

            return TRUE;

    }

}


/*
 * Apply or remove an affect to a character.
 */

void affect_to_char2( CHAR_DATA *ch, AFFECT_DATA *paf, int level)
{
    AFFECT_DATA *paf_new;

    if ( affect_free == NULL )
    {
        paf_new         = alloc_perm( sizeof(*paf_new) );
    }
    else
    {
        paf_new         = affect_free;
        affect_free     = affect_free->next;
    }
    *paf_new            = *paf;
    paf_new->next       = ch->affected;
    ch->affected        = paf_new;
    affect_modify2( ch, paf_new, TRUE, level);
    return;
}

void affect_modify2( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd, int level)
{
    OBJ_DATA *wield;
    int mod;
         
    mod = paf->modifier;
     
    if ( fAdd )
    {
      if( level == 1)
        SET_BIT( ch->affected_by, paf->bitvector );
      else
      {
       bug("Affect_modify2: Unknown level %d\n\r", level);
       return;
      }
    }
    else
    {
      if( level == 1)
        REMOVE_BIT( ch->affected_by, paf->bitvector );
      else
      {
       bug("Affect_modify2: unkown level %d\n\r", level);
       return;
      }
      mod = 0 - mod;
    }
    if ( IS_NPC(ch) )
    {
        switch ( paf->location )
        {
            default: break;
            case APPLY_NONE:                                            break;
            case APPLY_MANA:          ch->max_mana              += mod; break;
            case APPLY_HIT:           ch->max_hit               += mod; break;
            case APPLY_MOVE:          ch->max_move              += mod; break;
            case APPLY_AC:            ch->armor                 += mod; break;
            case APPLY_HITROLL:       ch->hitroll               += mod; break;
            case APPLY_DAMROLL:       ch->damroll               += mod; break;
            case APPLY_SAVING_PARA:   ch->saving_throw          += mod; break;
            case APPLY_SAVING_ROD:    ch->saving_throw          += mod; break;
            case APPLY_SAVING_PETRI:  ch->saving_throw          += mod; break;
            case APPLY_SAVING_BREATH: ch->saving_throw          += mod; break;
            case APPLY_SAVING_SPELL:  ch->saving_throw          += mod; break;
        }
        return;
    }
    switch ( paf->location )
    {
    default:
        bug( "Affect_modify2: unknown location %d.", paf->location );
        return;
     
    case APPLY_NONE:                                            break;
    case APPLY_STR:           ch->pcdata->mod_str       += mod; break;
    case APPLY_DEX:           ch->pcdata->mod_dex       += mod; break;
    case APPLY_INT:           ch->pcdata->mod_int       += mod; break;
    case APPLY_WIS:           ch->pcdata->mod_wis       += mod; break;
    case APPLY_CON:           ch->pcdata->mod_con       += mod; break;
    case APPLY_SEX:                                             break;
    case APPLY_CLASS:                                           break;
    case APPLY_LEVEL:                                           break;
    case APPLY_AGE:                                             break;
    case APPLY_HEIGHT:                                          break;
    case APPLY_WEIGHT:                                          break;
    case APPLY_MANA:          ch->max_mana              += mod; break;
    case APPLY_HIT:           ch->max_hit               += mod; break;
    case APPLY_MOVE:          ch->max_move              += mod; break;
    case APPLY_GOLD:                                            break;
    case APPLY_EXP:                                             break;
    case APPLY_AC:            ch->armor                 += mod; break;
    case APPLY_HITROLL:       ch->hitroll               += mod; break;
    case APPLY_DAMROLL:       ch->damroll               += mod; break;
    case APPLY_SAVING_PARA:   ch->saving_throw          += mod; break;
    case APPLY_SAVING_ROD:    ch->saving_throw          += mod; break;
    case APPLY_SAVING_PETRI:  ch->saving_throw          += mod; break;
    case APPLY_SAVING_BREATH: ch->saving_throw          += mod; break;
    case APPLY_SAVING_SPELL:  ch->saving_throw          += mod; break;
    }
        
    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   wield->item_type == ITEM_WEAPON
    &&   get_obj_weight(wield) > str_app[get_curr_str(ch)].wield )
    {
        static int depth;
    
        if ( depth == 0 )
        {
            depth++;
            act( "You drop $p.", ch, wield, NULL, TO_CHAR );
            act( "$n drops $p.", ch, wield, NULL, TO_ROOM );
            obj_from_char( wield );
            obj_to_room( wield, ch->in_room );
            depth--;
        }
    }
    return;
}

void affect_remove2( CHAR_DATA *ch, AFFECT_DATA *paf, int level)
{
   if( level == 1)
   {
    if ( ch->affected == NULL )
    {
        bug( "Affect_remove2: no affect.", 0 );
        return;
    }
    affect_modify2( ch, paf, FALSE, level);
   }     
    if( (paf == ch->affected) && (level == 1) )
    {
        ch->affected    = paf->next;
    }
    else
    {   
        AFFECT_DATA *prev;
       if( level == 1)
       {
        for ( prev = ch->affected; prev != NULL; prev = prev->next )
        {
            if ( prev->next == paf )   
            {
                prev->next = paf->next;
                break;
            }
        }
       }
        if ( prev == NULL )
        {
            bug( "Affect_remove: cannot find paf.", 0 );
            return;
        }
    }
    paf->next   = affect_free;
    affect_free = paf->next;   
    return;
}

void affect_strip2( CHAR_DATA *ch, int sn, int level)
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    
   if( level == 1)        
    for( paf = ch->affected; paf != NULL; paf = paf_next )
    {
        paf_next = paf->next; 
        if ( paf->type == sn ) 
          affect_remove2( ch, paf, level);
    }
   else
   {
    bug("Affect_strip2: unkown level %d\n\r.", level);
    return;
   }
    return;
}

void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
    OBJ_DATA *wield;
    int mod;

    mod = paf->modifier;

    if ( fAdd )
    {
	SET_BIT( ch->affected_by, paf->bitvector );
    }
    else
    {
	REMOVE_BIT( ch->affected_by, paf->bitvector );
	mod = 0 - mod;
    }
    if ( IS_NPC(ch) )
    {
    	switch ( paf->location )
    	{
	    default: break;
	    case APPLY_NONE:						break;
	    case APPLY_MANA:          ch->max_mana		+= mod;	break;
	    case APPLY_HIT:           ch->max_hit		+= mod;	break;
	    case APPLY_MOVE:          ch->max_move		+= mod;	break;
	    case APPLY_AC:            ch->armor			+= mod;	break;
	    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
	    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
	    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
	    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
	}
	return;
    }
    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->pcdata->mod_str	+= mod;	break;
    case APPLY_DEX:           ch->pcdata->mod_dex	+= mod;	break;
    case APPLY_INT:           ch->pcdata->mod_int	+= mod;	break;
    case APPLY_WIS:           ch->pcdata->mod_wis	+= mod;	break;
    case APPLY_CON:           ch->pcdata->mod_con	+= mod;	break;
    case APPLY_SEX:						break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:            ch->armor			+= mod;	break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SAVING_PARA:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_POLY:						break;
    }

    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   wield->item_type == ITEM_WEAPON
    &&   get_obj_weight(wield) > str_app[get_curr_str(ch)].wield )
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You drop $p.", ch, wield, NULL, TO_CHAR );
	    act( "$n drops $p.", ch, wield, NULL, TO_ROOM );
	    obj_from_char( wield );
	    obj_to_room( wield, ch->in_room );
	    depth--;
	}
    }
    return;
}

void area_affect_remove( AREA_DATA *area, AFFECT_DATA *paf)
{
    if ( area->affected == NULL )
    {
        bug( "Affect_remove: no affect.", 0 );
        return;
    }
    area_affect_modify( area, paf, FALSE);

    if ( paf == area->affected )  
    {
        area->affected    = paf->next;
    }
    else
    {
     AFFECT_DATA *prev;

     for ( prev = area->affected; prev != NULL; prev = prev->next )
        {
         if ( prev->next == paf )
         {
          prev->next = paf->next;
          break;
         }
        }
        if ( prev == NULL )
        {
            bug( "Area_affect_remove: cannot find paf.", 0 );
            return;
        }
    }
    paf->next   = affect_free;
    affect_free = paf->next;
    return;
}

void area_affect_modify( AREA_DATA *area, AFFECT_DATA *paf, bool fAdd )
{

    if( fAdd )
     SET_BIT( area->affected_by, paf->bitvector );
    else
     REMOVE_BIT( area->affected_by, paf->bitvector );
    return;
}

void room_affect_remove( ROOM_INDEX_DATA *room, AFFECT_DATA *paf)
{
    if ( room->affected == NULL )
    {
        bug( "Affect_remove: no affect.", 0 );
        return;
    }
    room_affect_modify( room, paf, FALSE);

    if ( paf == room->affected )  
    {
        room->affected    = paf->next;
    }
    else
    {
     AFFECT_DATA *prev;

     for ( prev = room->affected; prev != NULL; prev = prev->next )
        {
         if ( prev->next == paf )
         {
          prev->next = paf->next;
          break;
         }
        }
        if ( prev == NULL )
        {
            bug( "Room_affect_remove: cannot find paf.", 0 );
            return;
        }
    }
    paf->next   = affect_free;
    affect_free = paf->next;
    return;
}

void room_affect_modify( ROOM_INDEX_DATA *room, AFFECT_DATA *paf, bool fAdd )
{

    if( fAdd )
     SET_BIT( room->affected_by, paf->bitvector );
    else
     REMOVE_BIT( room->affected_by, paf->bitvector );
    return;
}

void affect_to_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;
 
    if ( affect_free == NULL )
    {
        paf_new         = alloc_perm( sizeof(*paf_new) );
    }
    else
    {
        paf_new         = affect_free;
        affect_free     = affect_free->next;
    }
    *paf_new            = *paf;
    paf_new->next       = room->affected;   
    room->affected        = paf_new;
   
    room_affect_modify( room, paf_new, TRUE);
    return;
}

void affect_to_area(AREA_DATA *area, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;
 
    if ( affect_free == NULL )
    {
        paf_new         = alloc_perm( sizeof(*paf_new) );
    }
    else
    {
        paf_new         = affect_free;
        affect_free     = affect_free->next;
    }
    *paf_new            = *paf;
    paf_new->next       = area->affected;   
    area->affected        = paf_new;
   
    area_affect_modify( area, paf_new, TRUE);
    return;
}  

/*
 * Remove an exit from a room					-Thoric
 */

void extract_exit( ROOM_INDEX_DATA *room, EXIT_DATA *pexit )
{
    UNLINK( pexit, room->first_exit, room->last_exit, next, prev );
    if ( pexit->rexit )
      pexit->rexit->rexit = NULL;
    STRFREE( pexit->keyword );
    STRFREE( pexit->description );
    DISPOSE( pexit );
}


/*

 * Give an affect to a char.

 */

void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    if ( affect_free == NULL )
    {
	paf_new		= alloc_perm( sizeof(*paf_new) );
    }
    else
    {
	paf_new		= affect_free;
	affect_free	= affect_free->next;
    }
    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE);
    return;
}

/*
 * Remove an affect from a char.
 */

void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf)
{
    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }
    affect_modify( ch, paf, FALSE);

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}
	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }
    paf->next	= affect_free;
    affect_free	= paf->next;
    return;
}

/*
 * Strip all affects of a given sn.
 */

void affect_strip( CHAR_DATA *ch, int sn)
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf);
    }
    return;
}


/*

 * Return true if a char is affected by a spell.

 */

bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }
    return FALSE;
}

/*
 * Add or enhance an affect.
 */

void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf)
{
    AFFECT_DATA *paf_old;
    bool found;

    found = FALSE;
    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old);
	    break;
	}
    }
    affect_to_char( ch, paf);
    return;

}

/*
 * Move a char out of a room.
 */

void char_from_room( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    if ( ch->in_room == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }

 //   if ( !IS_NPC(ch) )
//	--ch->in_room->area->nplayer;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;
    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;
    if ( !IS_NPC(ch)  &&  IS_SET(ch->newbits, NEW_DARKNESS))
    {
	if (ch->in_room != NULL)
	    REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS); 
    }
    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;
	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}
	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }
    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    return;
}







/*

 * Move a char into a room.

 */

void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )

{

    OBJ_DATA *obj;
/*    AFFECT_DATA *af;
    AFFECT_DATA *af_next;
    AFFECT_DATA afc;
    int dam = 0;
    int sn = 0;
    char buf[MAX_STRING_LENGTH];
*/ 

    if ( !pRoomIndex )

    {

	char buf[MAX_STRING_LENGTH];



	sprintf( buf, "Char_to_room: %s -> NULL room!  Putting char in limbo (%d)", ch->name, ROOM_VNUM_LIMBO );

	bug( buf, 0 );

        /* This used to just return, but there was a problem with crashing

           and I saw no reason not to just put the char in limbo. -Narn */

        pRoomIndex = get_room_index( ROOM_VNUM_LIMBO );

    }



    ch->in_room		= pRoomIndex;

    ch->next_in_room	= pRoomIndex->people;

    pRoomIndex->people	= ch;

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL

    &&   obj->item_type == ITEM_LIGHT

    &&   obj->value[2] != 0 )

	++ch->in_room->light;

    else if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL

    &&   obj->item_type == ITEM_LIGHT

    &&   obj->value[2] != 0 )

	++ch->in_room->light;

  /*  if( IS_SET(ch->in_room->affected_by, AFF_ROOM_DART) )
    {
     if( !IS_NPC(ch) && (ch->trust >= 3) )
     {
      dam = number_range(1000,3000);
      ch->hit -= dam;
      sprintf(buf, "A soft *thump* is heard as a dart flies out of nowhere striking $n! [%d]", dam );
      act(buf, ch, NULL, NULL, TO_ROOM);
      sprintf(buf, "You hear a soft *thump* and then a sudden pain as you are hit by a dart! [%d]", dam );
      stc(buf,ch);
      update_pos(ch);
      for( af = ch->in_room->affected; af != NULL; af = af_next)
      {
       af_next = af->next;
       if( af->bitvector == AFF_ROOM_DART)
        room_affect_remove(ch->in_room,af);
      }
     }
    }

   if( IS_SET(ch->in_room->affected_by, AFF_ROOM_FIRETRAP))
    {
      if(!IS_NPC(ch) && (ch->trust >= 3) && (number_range(1,100) < 70) && !has_power(ch, SCHOOL_FIRE))
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
     }*/

    if (ch->loc_hp[6] > 0 && ch->in_room->blood < 1000) ch->in_room->blood += 1;



    if ( !IS_NPC(ch)  &&  IS_SET(ch->newbits, NEW_DARKNESS))

        SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);  

    return;

}







/*

 * Give an obj to a char.

 */

void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )

{

    if (obj == NULL) return;

    obj->next_content	 = ch->carrying;

    ch->carrying	 = obj;

    obj->carried_by	 = ch;

    obj->in_room	 = NULL;

    obj->in_obj		 = NULL;

    ch->carry_number	+= 1;

    ch->carry_weight	+= get_obj_weight( obj );


 if( IS_OBJ_STAT(obj,ITEM_NOREMOVE) )
  REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
 if( !IS_OBJ_STAT(obj,ITEM_FIXED_DUR) )
 {
  SET_BIT(obj->extra_flags,ITEM_FIXED_DUR); 
  obj->durability = 100;
 }
 if( obj->item_type == ITEM_WEAPON )
 {
  if( obj->value[1] > 45 )
   obj->value[1] = 45;
  if( obj->value[2] > 55 )
   obj->value[2] = 55;
 }

}

/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{

    CHAR_DATA *ch;
    if (obj == NULL) return;
    if ( ( ch = obj->carried_by ) == NULL )
    {
	bug( "Obj_from_char: null ch.", 0 );
	return;
    }
    if ( obj->wear_loc != WEAR_NONE )
	unequip_char( ch, obj );
    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;
	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}
	if ( prev == NULL )
	    bug( "Obj_from_char: obj not in list.", 0 );
    }
    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= 1;
    ch->carry_weight	-= get_obj_weight( obj );
    return;
}

/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear )
{
    if ( obj->item_type != ITEM_ARMOR )
	return 0;

    switch ( iWear )
    {
    case WEAR_BODY:	return 3 * obj->value[0] * 100 / obj->durability;
    case WEAR_HEAD:	return 2 * obj->value[0] * 100 / obj->durability;
    case WEAR_LEGS:	return 2 * obj->value[0] * 100 / obj->durability;
    case WEAR_FEET:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_HANDS:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_ARMS:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_SHIELD:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_FINGER_L:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_FINGER_R: return     obj->value[0] * 100 / obj->durability;
    case WEAR_NECK_1:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_NECK_2:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_ABOUT:	return 2 * obj->value[0] * 100 / obj->durability;
    case WEAR_WAIST:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_WRIST_L:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_WRIST_R:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_HOLD:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_FACE:	return     obj->value[0] * 100 / obj->durability;
    case WEAR_SCABBARD_L:return 0;
    case WEAR_SCABBARD_R:return 0;
    }
    return 0;
}


/*
 * Find a piece of eq on a character.
 */

OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == iWear )
	    return obj;
    }
    return NULL;

}

/*
 * Equip a char with an obj.
 */

void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{

    CHAR_DATA   *chch;
    AFFECT_DATA *paf;
    int sn;

    if ( get_eq_char( ch, iWear ) != NULL )
    {
	bug( "Equip_char: already equipped (%d).", iWear );
	return;
    }

    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
	act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
	act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	do_autosave( ch, "" );
	return;
    }
    if (!IS_NPC(ch) && (ch->level < 3) && IS_SET(obj->quest, QUEST_ARTIFACT))
    {
      act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );
      act( "$n is zapped by $p and drops it.",  ch, obj, NULL, TO_ROOM );
      obj_from_char( obj );
      obj_to_room( obj, ch->in_room );
      do_autosave( ch, "" );
      return;
    }

    if ( iWear == WEAR_SCABBARD_L ||
    	iWear == WEAR_SCABBARD_R ) 
    {
	obj->wear_loc = iWear;
	return;
    }
    //The below check looks rediculous...removeing till i know why this is--Kale
    //if ( IS_NPC(ch)) 
	ch->armor -= apply_ac( obj, iWear );
    obj->wear_loc = iWear;
    
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
     affect_modify(ch,paf,TRUE);
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
     affect_modify(ch,paf,TRUE);

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL )
	++ch->in_room->light;

    if (!IS_NPC(ch))
    {
    	if ( (chch = get_char_world(ch,ch->name)) == NULL ) return;
    	if ( chch->desc != ch->desc) return;
    }

    if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	&& str_cmp(obj->chpoweron,"(null)")
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	{
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
	    if (IS_SET(obj->spectype, SITEM_ACTION))
		kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_ROOM);
	}
    if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	&& str_cmp(obj->victpoweron,"(null)")
	&& !IS_SET(obj->spectype, SITEM_ACTION)
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);

    if ( obj->wear_loc == WEAR_NONE )
	return;
    if (   ((obj->item_type == ITEM_ARMOR ) && (obj->value[3] >= 1   ))
	|| ((obj->item_type == ITEM_WEAPON) && (obj->value[0] >= 1000))
	|| IS_SET(obj->spectype, SITEM_SILVER)
	|| IS_SET(obj->spectype, SITEM_DEMONIC)
	|| IS_SET(obj->quest, QUEST_ARTIFACT) )
    {
	/* It would be so much easier if weapons had 5 values *sigh*.  
	 * Oh well, I'll just have to use v0 for two.  KaVir.
	 */
	if (obj->item_type == ITEM_ARMOR)
	    sn = obj->value[3];
	else
	    sn = obj->value[0] / 1000;

        if ( IS_SET(obj->extra_flags, ITEM_FORGED) 
	||   IS_SET(obj->extra_flags, ITEM_ONE_RING)
	|| ( ch->level < 3 && ( obj->pIndexData->vnum > 3373 && obj->pIndexData->vnum < 3390 ) ) )
        {
	if      ((sn ==  4) && (IS_AFFECTED(ch, AFF_BLIND)))        return;
	else if ((sn == 27) && (IS_AFFECTED(ch, AFF_DETECT_INVIS))) return;
	else if ((sn == 39) && (IS_AFFECTED(ch, AFF_FLYING)))       return;
	else if ((sn == 45) && (IS_AFFECTED(ch, AFF_INFRARED)))     return;
	else if ((sn == 46) && (IS_AFFECTED(ch, AFF_INVISIBLE)))    return;
	else if ((sn == 52) && (IS_AFFECTED(ch, AFF_PASS_DOOR)))    return;
	else if ((sn == 54) && (IS_AFFECTED(ch, AFF_PROTECT)))      return;
	else if ((sn == 57) && (IS_AFFECTED(ch, AFF_SANCTUARY)))    return;
	else if ((sn == 2 ) && (IS_AFFECTED(ch, AFF_DETECT_INVIS))) return;
	else if ((sn == 3 ) && (IS_AFFECTED(ch, AFF_FLYING)))       return;
	else if ((sn == 1 ) && (IS_AFFECTED(ch, AFF_INFRARED)))     return;
	else if ((sn == 5 ) && (IS_AFFECTED(ch, AFF_INVISIBLE)))    return;
	else if ((sn == 6 ) && (IS_AFFECTED(ch, AFF_PASS_DOOR)))    return;
	else if ((sn == 7 ) && (IS_AFFECTED(ch, AFF_PROTECT)))      return;
	else if ((sn == 8 ) && (IS_AFFECTED(ch, AFF_SANCTUARY)))    return;
	else if ((sn == 9 ) && (IS_AFFECTED(ch, AFF_SNEAK)))        return;
	else if ((sn == 10) && (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD))) return;
	else if ((sn == 11) && (IS_ITEMAFF(ch, ITEMA_FIRESHIELD)))  return;
	else if ((sn == 12) && (IS_ITEMAFF(ch, ITEMA_ICESHIELD)))   return;
	else if ((sn == 13) && (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD)))  return;
	else if ((sn == 14) && (IS_ITEMAFF(ch, ITEMA_DBPASS)))      return;
	else if ((sn == 15) && (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD))) return;
	else if ((sn == 16) && (IS_ITEMAFF(ch, ITEMA_REGENERATE)))  return;
	else if ((sn == 17) && (IS_ITEMAFF(ch, ITEMA_SPEED)))       return;
	else if ((sn == 18) && (IS_ITEMAFF(ch, ITEMA_VORPAL)))      return;
	else if ((sn == 19) && (IS_ITEMAFF(ch, ITEMA_PEACE)))       return;
	else if ((sn == 20) && (IS_ITEMAFF(ch, ITEMA_REFLECT)))     return;
	else if ((sn == 21) && (IS_ITEMAFF(ch, ITEMA_RESISTANCE)))  return;
	else if ((sn == 22) && (IS_ITEMAFF(ch, ITEMA_VISION)))      return;
	else if ((sn == 23) && (IS_ITEMAFF(ch, ITEMA_STALKER)))     return;
	else if ((sn == 24) && (IS_ITEMAFF(ch, ITEMA_VANISH)))      return;
	else if ((sn == 25) && (IS_ITEMAFF(ch, ITEMA_RAGER)))       return;
	else if ((sn == 60) && (IS_AFFECTED(ch, AFF_DETECT_HIDDEN)))return;
        }

        if( IS_SET(obj->extra_flags, ITEM_FORGED)
	||  IS_SET(obj->extra_flags, ITEM_ONE_RING) 
	|| ( ch->level < 3 && ( obj->pIndexData->vnum > 3373 && obj->pIndexData->vnum < 3390 ) ) )
        {
	if (sn == 4)
	{
	    SET_BIT(ch->affected_by, AFF_BLIND);
	    send_to_char( "You cannot see a thing!\n\r", ch );
	    act("$n seems to be blinded!",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 27 || sn == 2)
	{
	    SET_BIT(ch->affected_by, AFF_DETECT_INVIS);
	    send_to_char( "Your eyes tingle.\n\r", ch );
	    act("$n's eyes flicker with light.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 39 || sn == 3)
	{
	    SET_BIT(ch->affected_by, AFF_FLYING);
	    send_to_char( "Your feet rise off the ground.\n\r", ch );
	    act("$n's feet rise off the ground.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 45 || sn == 1)
	{
	    SET_BIT(ch->affected_by, AFF_INFRARED);
	    send_to_char( "Your eyes glow red.\n\r", ch );
	    act("$n's eyes glow red.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 46 || sn == 5)
	{
	    SET_BIT(ch->affected_by, AFF_INVISIBLE);
	    send_to_char( "You fade out of existance.\n\r", ch );
	    act("$n fades out of existance.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 52 || sn == 6)
	{
	    SET_BIT(ch->affected_by, AFF_PASS_DOOR);
	    send_to_char( "You turn translucent.\n\r", ch );
	    act("$n turns translucent.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 60)
	{
	    SET_BIT(ch->affected_by, AFF_DETECT_HIDDEN);
	    send_to_char( "You awarenes improves.\n\r", ch );
	    act("$n eyes tingle.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 54 || sn == 7)
	{
	    SET_BIT(ch->affected_by, AFF_PROTECT);
	    send_to_char( "You are surrounded by a divine aura.\n\r", ch );
	    act("$n is surrounded by a divine aura.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 57 || sn == 8)
	{
	    SET_BIT(ch->affected_by, AFF_SANCTUARY);
	    send_to_char( "You are surrounded by a white aura.\n\r", ch );
	    act("$n is surrounded by a white aura.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 9)
	{
	    SET_BIT(ch->affected_by, AFF_SNEAK);
	    send_to_char( "Your footsteps stop making any sound.\n\r", ch );
	    act("$n's footsteps stop making any sound.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 10)
	{
	    SET_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);
	    send_to_char( "You are surrounded by a crackling shield of lightning.\n\r", ch );
	    act("$n is surrounded by a crackling shield of lightning.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 11)
	{
	    SET_BIT(ch->itemaffect, ITEMA_FIRESHIELD);
	    send_to_char( "You are surrounded by a burning shield of flames.\n\r", ch );
	    act("$n is surrounded by a burning shield of flames.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 12)
	{
	    SET_BIT(ch->itemaffect, ITEMA_ICESHIELD);
	    send_to_char( "You are surrounded by a shimmering shield of ice.\n\r", ch );
	    act("$n is surrounded by a shimmering shield of ice.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 13)
        {	
	    SET_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);
	    send_to_char( "You are surrounded by a bubbling shield of acid.\n\r", ch );
	    act("$n is surrounded by a bubbling shield of acid.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 15)
	{
	    SET_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);
	    send_to_char( "You are surrounded by a swirling shield of chaos.\n\r", ch );
	    act("$n is surrounded by a swirling shield of chaos.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 16)
	    SET_BIT(ch->itemaffect, ITEMA_REGENERATE);
	else if (sn == 17)
	{
	    SET_BIT(ch->itemaffect, ITEMA_SPEED);
	    send_to_char( "You start moving faster than the eye can follow.\n\r", ch );
	    act("$n starts moving faster than the eye can follow.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 18)
	    SET_BIT(ch->itemaffect, ITEMA_VORPAL);
	else if (sn == 19)
	    SET_BIT(ch->itemaffect, ITEMA_PEACE);
	else if (sn == 20)
	{
	    SET_BIT(ch->itemaffect, ITEMA_REFLECT);
	    send_to_char( "You are surrounded by flickering shield of darkness.\n\r", ch );
	    act("$n is surrounded by a flickering shield of darkness.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 21)
	    SET_BIT(ch->itemaffect, ITEMA_RESISTANCE);
	else if (sn == 22)
	{
	    SET_BIT(ch->itemaffect, ITEMA_VISION);
	    send_to_char( "Your eyes begin to glow bright white.\n\r", ch );
	    act("$n's eyes begin to glow bright white.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 23)
	    SET_BIT(ch->itemaffect, ITEMA_STALKER);
	else if (sn == 24)
	{
	    SET_BIT(ch->itemaffect, ITEMA_VANISH);
	    send_to_char( "You blend into the shadows.\n\r", ch );
	    act("$n gradually fades into the shadows.",ch,NULL,NULL,TO_ROOM);
	}
	else if (sn == 25 && !IS_NPC(ch))
	{
	    SET_BIT(ch->itemaffect, ITEMA_RAGER);
	}
	if (IS_SET(obj->quest, QUEST_ARTIFACT))
	    SET_BIT(ch->itemaffect, ITEMA_ARTIFACT);
	if (IS_SET(obj->spectype, SITEM_SILVER) && obj->wear_loc == WEAR_WIELD)
	    SET_BIT(ch->itemaffect, ITEMA_RIGHT_SILVER);
	if (IS_SET(obj->spectype, SITEM_SILVER) && obj->wear_loc == WEAR_HOLD)
	    SET_BIT(ch->itemaffect, ITEMA_LEFT_SILVER);
       }
    }
    return;
}



/*

 * Unequip a char with an obj.

 */

void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )

{

    CHAR_DATA   *chch;

    AFFECT_DATA *paf;

    int sn;

    int oldpos = obj->wear_loc;



    if ( obj->wear_loc == WEAR_NONE )

    {

	bug( "Unequip_char: already unequipped.", 0 );

	return;

    }



    if ( obj->wear_loc == WEAR_SCABBARD_L ||

    	obj->wear_loc == WEAR_SCABBARD_R ) {obj->wear_loc = -1;return;}



    //if ( IS_NPC(ch)) 

	ch->armor += apply_ac(obj, obj->wear_loc);

    obj->wear_loc	 = -1;

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE);
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	affect_modify( ch, paf, FALSE);

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if ( (chch = get_char_world(ch,ch->name)) == NULL ) 
      return;
    if ( chch->desc != ch->desc) 
      return;
    if (!IS_NPC(ch) && (ch->desc != NULL && ch->desc->connected != CON_PLAYING)) 
      return;

    if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	&& str_cmp(obj->chpoweroff,"(null)")
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	{
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
	    if (IS_SET(obj->spectype, SITEM_ACTION))
		kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_ROOM);
	}

    if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	&& str_cmp(obj->victpoweroff,"(null)")
	&& !IS_SET(obj->spectype, SITEM_ACTION)
	&& !IS_SET(obj->spectype, SITEM_TELEPORTER)
	&& !IS_SET(obj->spectype, SITEM_TRANSPORTER) )
	kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);

    if ( ((obj->item_type == ITEM_ARMOR ) && (obj->value[3] >= 1   ))
        || ((obj->item_type == ITEM_WEAPON) && (obj->value[0] >= 1000))
	|| IS_SET(obj->spectype, SITEM_SILVER)
	|| IS_SET(obj->spectype, SITEM_DEMONIC)
	|| IS_SET(obj->quest, QUEST_ARTIFACT) )
    {

	if (obj->item_type == ITEM_ARMOR)

	    sn = obj->value[3];

	else

	    sn = obj->value[0] / 1000;
     if( IS_SET(obj->extra_flags, ITEM_FORGED) 
     ||  IS_SET(obj->extra_flags, ITEM_ONE_RING) )
     {

	if (IS_AFFECTED(ch, AFF_BLIND) && (sn == 4)

		&& !is_affected(ch, 4))

	{

	    REMOVE_BIT(ch->affected_by, AFF_BLIND);

	    send_to_char( "You can see again.\n\r", ch );

	    act("$n seems to be able to see again.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_DETECT_INVIS) && (sn == 27 || sn == 2)

		&& !is_affected(ch, 27))

	{

	    REMOVE_BIT(ch->affected_by, AFF_DETECT_INVIS);

	    send_to_char( "Your eyes stop tingling.\n\r", ch );

	    act("$n's eyes stop flickering with light.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_FLYING) && (sn == 39 || sn == 3)

		&& !is_affected(ch, 39))

	{

	    REMOVE_BIT(ch->affected_by, AFF_FLYING);

	    send_to_char( "You slowly float to the ground.\n\r", ch );

	    act("$n slowly floats to the ground.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_INFRARED) && (sn == 45 || sn == 1)

		&& !is_affected(ch, 45))

	{

	    REMOVE_BIT(ch->affected_by, AFF_INFRARED);

	    send_to_char( "Your eyes stop glowing red.\n\r", ch );

	    act("$n's eyes stop glowing red.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_INVISIBLE) && (sn == 46 || sn == 5)

		&& !is_affected(ch, 46))

	{

	    REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);

	    send_to_char( "You fade into existance.\n\r", ch );

	    act("$n fades into existance.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_PASS_DOOR) && (sn == 52 || sn == 6)

		&& !is_affected(ch, 52))

	{

	    REMOVE_BIT(ch->affected_by, AFF_PASS_DOOR);

	    send_to_char( "You feel solid again.\n\r", ch );

	    act("$n is no longer translucent.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_PROTECT) && (sn == 54 || sn == 7)

		&& !is_affected(ch, 54))

	{

	    REMOVE_BIT(ch->affected_by, AFF_PROTECT);

	    send_to_char( "The divine aura around you fades.\n\r", ch );

	    act("The divine aura around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_SANCTUARY) && (sn == 57 || sn == 8)

		&& !is_affected(ch, 57))

	{

	    REMOVE_BIT(ch->affected_by, AFF_SANCTUARY);

	    send_to_char( "The white aura around your body fades.\n\r", ch );

	    act("The white aura about $n's body fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_SNEAK) && (sn == 9)

		&& !is_affected(ch, 80))

	{

	    REMOVE_BIT(ch->affected_by, AFF_SNEAK);

	    send_to_char( "You are no longer moving so quietly.\n\r", ch );

	    act("$n is no longer moving so quietly.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_AFFECTED(ch, AFF_DETECT_HIDDEN) && (sn == 60))

	{

	    REMOVE_BIT(ch->affected_by, AFF_DETECT_HIDDEN);

	    send_to_char( "You feel less aware of your surrondings.\n\r",ch );

	    act("$n eyes tingle.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_ITEMAFF(ch, ITEMA_SHOCKSHIELD) && (sn == 10))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_SHOCKSHIELD);

	    send_to_char( "The crackling shield of lightning around you fades.\n\r", ch );

	    act("The crackling shield of lightning around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_ITEMAFF(ch, ITEMA_FIRESHIELD) && (sn == 11))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_FIRESHIELD);

	    send_to_char( "The burning shield of fire around you fades.\n\r", ch );

	    act("The burning shield of fire around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_ITEMAFF(ch, ITEMA_ICESHIELD) && (sn == 12))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_ICESHIELD);

	    send_to_char( "The shimmering shield of ice around you fades.\n\r", ch );

	    act("The shimmering shield of ice around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_ITEMAFF(ch, ITEMA_ACIDSHIELD) && (sn == 13))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_ACIDSHIELD);

	    send_to_char( "The bubbling shield of acid around you fades.\n\r", ch );

	    act("The bubbling shield of acid around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (IS_ITEMAFF(ch, ITEMA_DBPASS) && (sn == 14))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_DBPASS);

	    send_to_char( "You are no longer safe from the DarkBlade clan guardians.\n\r", ch );

	}

	else if (IS_ITEMAFF(ch, ITEMA_CHAOSSHIELD) && (sn == 15))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_CHAOSSHIELD);

	    send_to_char( "The swirling shield of chaos around you fades.\n\r", ch );

	    act("The swirling shield of chaos around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (sn == 16)

	    REMOVE_BIT(ch->itemaffect, ITEMA_REGENERATE);

	else if (IS_ITEMAFF(ch, ITEMA_SPEED) && (sn == 17))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_SPEED);

	    send_to_char( "Your actions slow down to normal speed.\n\r", ch );

	    act("$n stops moving at supernatural speed.",ch,NULL,NULL,TO_ROOM);

	}

	else if (sn == 18)

	    REMOVE_BIT(ch->itemaffect, ITEMA_VORPAL);

	else if (sn == 19)

	    REMOVE_BIT(ch->itemaffect, ITEMA_PEACE);

	else if (IS_ITEMAFF(ch, ITEMA_REFLECT) && sn == 20)

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_REFLECT);

	    send_to_char( "The flickering shield of darkness around you fades.\n\r", ch );

	    act("The flickering shield of darkness around $n fades.",ch,NULL,NULL,TO_ROOM);

	}

	else if (sn == 21)

	    REMOVE_BIT(ch->itemaffect, ITEMA_RESISTANCE);

	else if (IS_ITEMAFF(ch, ITEMA_VISION) && sn == 22)

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_VISION);

	    send_to_char( "Your eyes stop glowing bright white.\n\r", ch );

	    act("$n's eyes stop glowing bright white.",ch,NULL,NULL,TO_ROOM);

	}

	else if (sn == 23)

	    REMOVE_BIT(ch->itemaffect, ITEMA_STALKER);

	else if (IS_ITEMAFF(ch, ITEMA_VANISH) && sn == 24)

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_VANISH);

	    send_to_char( "You emerge from the shadows.\n\r", ch );

	    act("$n gradually fades out of the shadows.",ch,NULL,NULL,TO_ROOM);

	}

	else if (sn == 25 && !IS_NPC(ch) && IS_ITEMAFF(ch, ITEMA_RAGER))

	{

	    REMOVE_BIT(ch->itemaffect, ITEMA_RAGER);

	    ch->rage = 0;

	}

	if (IS_SET(obj->quest, QUEST_ARTIFACT)) 

	    REMOVE_BIT(ch->itemaffect, ITEMA_ARTIFACT);

	if (IS_SET(obj->spectype, SITEM_SILVER) && oldpos == WEAR_WIELD)

	    REMOVE_BIT(ch->itemaffect, ITEMA_RIGHT_SILVER);

	if (IS_SET(obj->spectype, SITEM_SILVER) && oldpos == WEAR_HOLD)

	    REMOVE_BIT(ch->itemaffect, ITEMA_LEFT_SILVER);

      }
    }

    return;

}







/*

 * Count occurrences of an obj in a list.

 */

int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )

{

    OBJ_DATA *obj;

    int nMatch;



    nMatch = 0;

    for ( obj = list; obj != NULL; obj = obj->next_content )

    {

	if ( obj->pIndexData == pObjIndex )

	    nMatch++;

    }



    return nMatch;

}







/*

 * Move an obj out of a room.

 */

void obj_from_room( OBJ_DATA *obj )

{

    ROOM_INDEX_DATA *in_room;



    if (obj == NULL) return;

    if ( ( in_room = obj->in_room ) == NULL )

    {

	bug( "obj_from_room: NULL.", 0 );

	return;

    }



    if ( obj == in_room->contents )

    {

	in_room->contents = obj->next_content;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = in_room->contents; prev; prev = prev->next_content )

	{

	    if ( prev->next_content == obj )

	    {

		prev->next_content = obj->next_content;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Obj_from_room: obj not found.", 0 );

	    return;

	}

    }



    obj->in_room      = NULL;

    obj->next_content = NULL;

    return;

}







/*

 * Move an obj into a room.

 */

void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )

{

    if (obj == NULL) return;
    if( pRoomIndex == NULL) return;

    obj->next_content		= pRoomIndex->contents;

    pRoomIndex->contents	= obj;

    obj->in_room		= pRoomIndex;

    obj->carried_by		= NULL;

    obj->in_obj			= NULL;

    return;

}







/*

 * Move an object into an object.

 */

void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )

{

    if (obj == NULL) return;

    obj->next_content		= obj_to->contains;

    obj_to->contains		= obj;

    obj->in_obj			= obj_to;

    obj->in_room		= NULL;

    obj->carried_by		= NULL;



    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )

    {

	if ( obj_to->carried_by != NULL && !IS_SET(obj_to->spectype,SITEM_MORPH))

	    obj_to->carried_by->carry_weight += get_obj_weight( obj );

    }



    return;

}







/*

 * Move an object out of an object.

 */

void obj_from_obj( OBJ_DATA *obj )

{

    OBJ_DATA *obj_from;



    if (obj == NULL) return;

    if ( ( obj_from = obj->in_obj ) == NULL )

    {

	bug( "Obj_from_obj: null obj_from.", 0 );

	return;

    }



    if ( obj == obj_from->contains )

    {

	obj_from->contains = obj->next_content;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = obj_from->contains; prev; prev = prev->next_content )

	{

	    if ( prev->next_content == obj )

	    {

		prev->next_content = obj->next_content;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Obj_from_obj: obj not found.", 0 );

	    return;

	}

    }



    obj->next_content = NULL;

    obj->in_obj       = NULL;



    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )

    {

	if ( obj_from->carried_by != NULL && !IS_SET(obj_from->spectype,SITEM_MORPH))

	    obj_from->carried_by->carry_weight -= get_obj_weight( obj );

    }



    return;

}







/*

 * Extract an obj from the world.

 */

void extract_obj( OBJ_DATA *obj )

{

    CHAR_DATA *ch;

    OBJ_DATA *obj_content;

    OBJ_DATA *obj_next;
    DESCRIPTOR_DATA *d;


    if ( obj == NULL ) return;

    if ( obj->in_room != NULL )

	obj_from_room( obj );

    else if ( obj->carried_by != NULL )

	obj_from_char( obj );

    else if ( obj->in_obj != NULL )

	obj_from_obj( obj );



    if ( ( ch = obj->chobj ) != NULL

    &&   !IS_NPC(ch)

    &&   ch->pcdata->chobj == obj

    &&   IS_HEAD(ch,LOST_HEAD))

    {

	REMOVE_BIT(ch->loc_hp[0],LOST_HEAD);

	REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);

	free_string(ch->morph);

	ch->morph = str_dup("");

	ch->hit = 1;

	char_from_room(ch);

	char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));

	ch->pcdata->chobj = NULL;

	obj->chobj = NULL;

	send_to_char("You have been KILLED!!\n\r",ch);

	do_look(ch,"auto");

	ch->position = POS_RESTING;
        do_fixpoly(ch,"");

    }

    else if ( ( ch = obj->chobj ) != NULL

    &&   !IS_NPC(ch)

    &&   ch->pcdata->chobj == obj

    &&   (IS_EXTRA(ch,EXTRA_OSWITCH) || ch->pcdata->obj_vnum != 0))

    {

	if (ch->pcdata->obj_vnum != 0)

	{

	    send_to_char("You have been destroyed!\n\r",ch);

	    ch->pcdata->chobj = NULL;

	    obj->chobj = NULL;

	}

	else

	{

	    REMOVE_BIT(ch->extra,EXTRA_OSWITCH);

	    REMOVE_BIT(ch->affected_by,AFF_POLYMORPH);

	    free_string(ch->morph);

	    ch->morph = str_dup("");

	    char_from_room(ch);

	    char_to_room(ch,get_room_index(ROOM_VNUM_ALTAR));

	    ch->pcdata->chobj = NULL;

	    obj->chobj = NULL;

	    send_to_char("You return to your body.\n\r",ch);

	}

    }

    else if ( obj->chobj != NULL )

    {

	if (!IS_NPC(obj->chobj)) obj->chobj->pcdata->chobj = NULL;

	obj->chobj = NULL;

	bug( "Extract_obj: obj %d chobj invalid.", obj->pIndexData->vnum );

    }

    for( d = descriptor_list; d; d = d->next)
    {
     if( d->connected != CON_PLAYING)
      continue;
     if( d->character == NULL)
      continue;
     if( d->character->pcdata == NULL)
      continue;
     if( d->character->pcdata->familiar == NULL)
      continue;
     if( d->character->pcdata->familiar == obj)
     {
      d->character->pcdata->familiar = NULL;
      write_to_buffer(d,"Your third eye has been destroyed.\n\r", 0);
     }
    }
    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )

    {

	obj_next = obj_content->next_content;

	if (obj->contains != NULL) extract_obj( obj->contains );

    }



    if ( object_list == obj )

    {

	object_list = obj->next;

    }

    else

    {

	OBJ_DATA *prev;



	for ( prev = object_list; prev != NULL; prev = prev->next )

	{

	    if ( prev->next == obj )

	    {

		prev->next = obj->next;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    //bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );

	    return;

	}

    }



    {

	AFFECT_DATA *paf;

	AFFECT_DATA *paf_next;



	for ( paf = obj->affected; paf != NULL; paf = paf_next )

	{

	    paf_next    = paf->next;

	    paf->next   = affect_free;

	    affect_free = paf;

	}

    }



    {

	EXTRA_DESCR_DATA *ed;

	EXTRA_DESCR_DATA *ed_next;



	for ( ed = obj->extra_descr; ed != NULL; ed = ed_next )

	{

	    ed_next		= ed->next;

	    free_string( ed->description );

	    free_string( ed->keyword     );

	    extra_descr_free	= ed;

	}

    }



    free_string( obj->name        );

    free_string( obj->description );

    free_string( obj->short_descr );

    if (obj->chpoweron    != NULL) free_string( obj->chpoweron   );

    if (obj->chpoweroff   != NULL) free_string( obj->chpoweroff  );

    if (obj->chpoweruse   != NULL) free_string( obj->chpoweruse  );

    if (obj->victpoweron  != NULL) free_string( obj->victpoweron );

    if (obj->victpoweroff != NULL) free_string( obj->victpoweroff);

    if (obj->victpoweruse != NULL) free_string( obj->victpoweruse);

    if (obj->questmaker   != NULL) free_string( obj->questmaker  );

    if (obj->questowner   != NULL) free_string( obj->questowner  );

    --obj->pIndexData->count;

    obj->next	= obj_free;

    obj_free	= obj;

    return;

}







/*

 * Extract a char from the world.

 */

void extract_char( CHAR_DATA *ch, bool fPull )

{

    CHAR_DATA *wch;

/*    CHAR_DATA *familiar;

    CHAR_DATA *wizard;
*/
    OBJ_DATA *obj;

    OBJ_DATA *obj_next;



    if ( ch == NULL ) return;



    if ( ch->in_room == NULL )

    {

	bug( "Extract_char: NULL.", 0 );

	return;

    }



    if ( fPull )

	die_follower( ch );



    stop_fighting( ch, TRUE );



    for ( obj = ch->carrying; obj != NULL; obj = obj_next )

    {

	obj_next = obj->next_content;

	extract_obj( obj );

    }

    

    char_from_room( ch );



    if ( IS_NPC(ch) )

	--ch->pIndexData->count;

    else if ( ch->pcdata->chobj != NULL )

    {

	ch->pcdata->chobj->chobj = NULL;

	ch->pcdata->chobj = NULL;

    }



    if ( !fPull )

    {

	char_to_room( ch, get_room_index( ROOM_VNUM_ALTAR ) );

	return;

    }



    if ( ch->desc != NULL && ch->desc->original != NULL )

	do_return( ch, "" );



    for ( wch = char_list; wch != NULL; wch = wch->next )

    {

	if ( wch->reply == ch )

	    wch->reply = NULL;

    }



    if ( ch == char_list )

    {

       char_list = ch->next;

    }

    else

    {

	CHAR_DATA *prev;



	for ( prev = char_list; prev != NULL; prev = prev->next )

	{

	    if ( prev->next == ch )

	    {

		prev->next = ch->next;

		break;

	    }

	}



	if ( prev == NULL )

	{

	    bug( "Extract_char: char not found.", 0 );

	    return;

	}

    }



    if ( ch->desc )

	ch->desc->character = NULL;



/*    if ( (wizard = ch->wizard) != NULL)

    {

	if (!IS_NPC(wizard)) wizard->pcdata->familiar = NULL;

	ch->wizard = NULL;

    }

    if ( !IS_NPC(ch) )

    {

	if ((familiar = ch->pcdata->familiar) != NULL)

	{

	    familiar->wizard = NULL;

	    ch->pcdata->familiar = NULL;

	    if (IS_NPC(familiar))

	    {

		act("$n slowly fades away to nothing.",familiar,NULL,NULL,TO_ROOM);

		extract_char(familiar,TRUE);

	    }

	}

	if ((familiar = ch->pcdata->partner) != NULL)

	    ch->pcdata->partner = NULL;

	if ((familiar = ch->pcdata->propose) != NULL)

	    ch->pcdata->propose = NULL;

	for ( familiar = char_list; familiar != NULL; familiar = familiar->next)

	{

	    if ( !IS_NPC(familiar) && familiar->pcdata->propose != NULL && 

		familiar->pcdata->propose == ch )

		familiar->pcdata->propose = NULL;

	    if ( !IS_NPC(familiar) && familiar->pcdata->partner != NULL && 

		familiar->pcdata->partner == ch )

		familiar->pcdata->partner = NULL;

	}

    }
*/


    free_char( ch );

    return;

}







/*

 * Find a char in the room.

 */

CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if ( !str_cmp( arg, "self" ) && (IS_NPC(ch) || ch->pcdata->chobj == NULL))
		return ch;
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
		if ( !IS_NPC(rch) && IS_HEAD(rch, LOST_HEAD) )
			continue;
		else if ( !IS_NPC(rch) && IS_EXTRA(rch, EXTRA_OSWITCH) )
			continue;
		/*else if ( !can_see( ch, rch ) || ( !is_name( arg, rch->name ) && ( IS_NPC(rch) || !is_name( arg, rch->morph )) && !is_name( arg, rch->pcdata->switchname )))*/
		else if ( !can_see( ch, rch ) || ( !is_name( arg, rch->name ) && ( IS_NPC(rch) || !is_name( arg, rch->morph ))))
			continue;
		if ( ++count == number )
			return rch;
    }
    return NULL;
}
/*
 * Find a char in the world.
 */

CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
		return wch;
    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
		if ( !IS_NPC(wch) && IS_HEAD(wch, LOST_HEAD) ) 
			continue;
		else if ( !IS_NPC(wch) && IS_EXTRA(wch, EXTRA_OSWITCH) )
			continue;
		/*else if ( !can_see( ch, wch ) || ( !is_name( arg, wch->name ) && ( IS_NPC(wch) || !is_name( arg, wch->morph )) && !is_name( arg, wch->pcdata->switchname )))*/
		else if ( !can_see( ch, wch ) || ( !is_name( arg, wch->name ) && ( IS_NPC(wch) || !is_name( arg, wch->morph ))))
			continue;
		if ( ++count == number )
			return wch;
	}
    return NULL;
}

CHAR_DATA *get_char_area( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *wch = NULL;

  argument = one_argument(argument, arg);
  
  wch = get_char_world(ch, arg);
  if( wch == NULL)
   return NULL;
  if( wch->in_room->area != ch->in_room->area)
   return NULL;
  else
   return wch;
}





/*

 * Find some object with a given index data.

 * Used by area-reset 'P' command.

 */

OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )

{

    OBJ_DATA *obj;



    for ( obj = object_list; obj != NULL; obj = obj->next )

    {

	if ( obj->pIndexData == pObjIndex )

	    return obj;

    }



    return NULL;

}





/*

 * Find an obj in a list.

 */

OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = list; obj != NULL; obj = obj->next_content )

    {

	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Find an object within the object you are in.

 */

OBJ_DATA *get_obj_in_obj( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    OBJ_DATA *obj2;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj->in_obj == NULL)

	return NULL;

    obj = ch->pcdata->chobj;

    if (obj->in_obj->item_type != ITEM_CONTAINER && obj->in_obj->item_type != ITEM_CORPSE_NPC &&

    	obj->in_obj->item_type != ITEM_CORPSE_PC ) return NULL;



    for ( obj2 = obj->in_obj->contains; obj2 != NULL; obj2 = obj2->next_content )

    {

	if (obj != obj2

	&&  is_name( arg, obj2->name ) )

	{

	    if ( ++count == number )

		return obj2;

	}

    }



    return NULL;

}







/*

 * Find an obj in player's inventory.

 */

OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )

    {

	if ( obj->wear_loc == WEAR_NONE

	&&   can_see_obj( ch, obj )

	&&   is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Find an obj in player's equipment.

 */

OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )

    {

	if ( obj->wear_loc != WEAR_NONE

	&&   can_see_obj( ch, obj )

	&&   is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Find an obj in the room or in inventory.

 */

OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )

{

    OBJ_DATA *obj;



    obj = get_obj_list( ch, argument, ch->in_room->contents );

    if ( obj != NULL )

	return obj;



    if ( ( obj = get_obj_carry( ch, argument ) ) != NULL )

	return obj;



    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )

	return obj;



    if ( ( obj = get_obj_in_obj( ch, argument ) ) != NULL )

	return obj;



    return NULL;

}







/*

 * Find an obj in the room or in inventory.

 */

OBJ_DATA *get_obj_room( CHAR_DATA *ch, char *argument )

{

    OBJ_DATA *obj;



    obj = get_obj_list( ch, argument, ch->in_room->contents );

    if ( obj != NULL )

	return obj;



    return NULL;

}







/*

 * Find an obj in the world.

 */

OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )

{

    char arg[MAX_INPUT_LENGTH];

    OBJ_DATA *obj;

    int number;

    int count;



    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )

	return obj;



    number = number_argument( argument, arg );

    count  = 0;

    for ( obj = object_list; obj != NULL; obj = obj->next )

    {

	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )

	{

	    if ( ++count == number )

		return obj;

	}

    }



    return NULL;

}







/*

 * Create a 'money' obj.

 */

OBJ_DATA *create_money( int amount )

{

    char buf[MAX_STRING_LENGTH];

    OBJ_DATA *obj;



    if ( amount <= 0 )

    {

	bug( "Create_money: zero or negative money %d.", amount );

	amount = 1;

    }



    if ( amount == 1 )

    {

	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_ONE ), 0 );

    }

    else

    {

	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ), 0 );

	sprintf( buf, obj->short_descr, amount );

	free_string( obj->short_descr );

	obj->short_descr	= str_dup( buf );

	obj->value[0]		= amount;

    }



    return obj;

}







/*

 * Return weight of an object, including weight of contents.

 */

int get_obj_weight( OBJ_DATA *obj )

{

    int weight;



    weight = obj->weight;

    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )

	weight += get_obj_weight( obj );



    return weight;

}







/*

 * True if room is dark.

 */

bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex )

{

    if( pRoomIndex == NULL)
     return FALSE;
    if ( pRoomIndex->light > 0 )

	return FALSE;



    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )

	return TRUE;



    if ( pRoomIndex->sector_type == SECT_INSIDE

    ||   pRoomIndex->sector_type == SECT_CITY )

	return FALSE;



    if ( weather_info.sunlight == SUN_SET

    ||   weather_info.sunlight == SUN_DARK )

	return TRUE;



    return FALSE;

}







/*

 * True if room is private.

 */

bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )

{

    CHAR_DATA *rch;

    int count;


    if( pRoomIndex == NULL)
     return TRUE;

    count = 0;

    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )

	count++;



    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )

	return TRUE;



    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )

	return TRUE;



    return FALSE;

}


/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( ch == victim )
	return TRUE;
    if( IS_NPC(ch) && IS_SET(ch->affected_by,AFF_CHARM) )
     return TRUE;
    if ( !IS_NPC(victim)
    &&   IS_SET(victim->act, PLR_WIZINVIS)
    &&   get_trust( ch ) < get_trust( victim )
    &&   victim->level != 4 )
	return FALSE;

    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_INCOG)
    &&   get_trust( ch ) < get_trust( victim ) 
    &&   ch->in_room != victim->in_room
    &&   victim-> level != 4 )
	return FALSE;
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_WOLF) && IS_SET(GET_POWERS(victim,WOLF_BITS),BIT_VANISH) &&
     IS_SET(GET_POWERS(victim,WOLF_BITS),BIT_GONE) && !IS_IMMORTAL(ch) )
        return FALSE;
    if( !IS_NPC(victim) && IS_CLASS(victim,CLASS_JEDI) && IS_SET(GET_POWERS(victim,JEDI_BITS),JEDI_MIND_TRICK) )
    {
     if( IS_IMMORTAL(ch) )
      return TRUE;
     else if( !IS_NPC(ch) && IS_CLASS(ch,CLASS_JEDI) )
     {
      if( IS_SET(GET_POWERS(ch,JEDI_BITS),JEDI_SIGHT) && GET_POWERS(ch,JEDI_FORCE_SIGHT) >=
          GET_POWERS(victim,JEDI_FORCE_MIND_TRICK) )
       return TRUE;
     }
     else
      return FALSE;
    }
    if ( IS_EXTRA(ch, BLINDFOLDED))
	return FALSE;
     if ( IS_ITEMAFF(ch, ITEMA_VISION) )
        return TRUE;
    if( IS_CLASS(victim, CLASS_ALIEN) && IS_SET(GET_POWERS(victim, ALIEN_BITS), ALIEN_CLOAK)
        && !IS_IMMORTAL(ch))
        return FALSE;
    if( IS_CLASS(victim, CLASS_SHADOW) && IS_SET(GET_POWERS(victim, SHADOW_BITS), SHADOW_INVIS)
        && !IS_IMMORTAL(ch))
        return FALSE;
    if( IS_SET(ch->in_room->room_flags,ROOM_TOTAL_DARKNESS) || IS_SET(victim->newbits,NEW_DARKNESS) )
    {
     if( IS_CLASS(ch,CLASS_VAMPIRE) && GET_POWERS(ch,DISC_AUSPEX) == 10 )
      return TRUE;
     else if( IS_CLASS(ch,CLASS_DROW) || IS_SET(ch->newbits,NEW_DARKNESS) )
      return TRUE;
     else if( !IS_IMMORTAL(ch) )
      return FALSE;
    }
    if( ch->in_room == NULL)
      return TRUE;
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
        return TRUE;
    if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && !IS_AFFECTED(victim,AFF_SHADOWPLANE))
	return TRUE;
    if (!IS_AFFECTED(ch,AFF_SHADOWPLANE) && IS_AFFECTED(victim,AFF_SHADOWPLANE)
	&& !IS_AFFECTED(ch,PLR_SHADOWSIGHT))
	return FALSE;
    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
	return FALSE;
    if ( IS_SET(ch->affected_by, AFF_BLIND) && !IS_AFFECTED(ch, PLR_SHADOWSIGHT))
	return FALSE;
    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED))
	return FALSE;
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	return FALSE;
    if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_HIDDEN) )
	return FALSE;
    if ( !IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD) )
	return TRUE;
    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) )
	return TRUE;
    if ( !IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;
    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    return TRUE;
}

/*
 * True if char can see obj.
 */

bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) )
	return TRUE;

    if (( IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)
    &&   obj->carried_by == NULL)
    &&   !IS_AFFECTED(ch, PLR_SHADOWSIGHT)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE))
	return FALSE;

    if (( !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)
    &&   obj->carried_by == NULL)
    &&   !IS_AFFECTED(ch, PLR_SHADOWSIGHT)
    &&   IS_AFFECTED(ch, AFF_SHADOWPLANE))
	return FALSE;

    if ( obj->item_type == ITEM_POTION )
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
	return FALSE;

    if ( IS_EXTRA(ch, BLINDFOLDED) )
	return FALSE;

    if ( IS_AFFECTED( ch, AFF_BLIND ) && !IS_AFFECTED(ch, PLR_SHADOWSIGHT) )
	return FALSE;

    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    if ( room_is_dark( ch->in_room ) && !IS_AFFECTED(ch, AFF_INFRARED))
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !IS_AFFECTED(ch, AFF_DETECT_INVIS) )
	return FALSE;

    if ( !IS_NPC(ch) && IS_HEAD(ch, LOST_HEAD) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_OSWITCH) &&
	ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT)
	return TRUE;

    return TRUE;
}

/*
 * True if char can drop obj.
 */

bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}

/*
 * Return ascii name of an item type.
 */

char *item_type_name( OBJ_DATA *obj )

{

    switch ( obj->item_type )

    {

    case ITEM_LIGHT:		return "light";

    case ITEM_SCROLL:		return "scroll";

    case ITEM_WAND:		return "wand";

    case ITEM_STAFF:		return "staff";

    case ITEM_WEAPON:		return "weapon";

    case ITEM_TREASURE:		return "treasure";

    case ITEM_ARMOR:		return "armor";

    case ITEM_POTION:		return "potion";

    case ITEM_FURNITURE:	return "furniture";

    case ITEM_TRASH:		return "trash";

    case ITEM_CONTAINER:	return "container";

    case ITEM_DRINK_CON:	return "drink container";

    case ITEM_KEY:		return "key";

    case ITEM_FOOD:		return "food";

    case ITEM_MONEY:		return "money";

    case ITEM_BOAT:		return "boat";

    case ITEM_CORPSE_NPC:	return "npc corpse";

    case ITEM_CORPSE_PC:	return "pc corpse";

    case ITEM_FOUNTAIN:		return "fountain";

    case ITEM_PILL:		return "pill";

    case ITEM_PORTAL:		return "portal";

    case ITEM_EGG:		return "egg";

    case ITEM_VOODOO:		return "voodoo doll";

    case ITEM_STAKE:		return "stake";

    case ITEM_MISSILE:		return "missile";

    case ITEM_AMMO:		return "ammo";

    case ITEM_QUEST:		return "quest token";

    case ITEM_QUESTCARD:	return "quest card";

    case ITEM_QUESTMACHINE:	return "quest generator";

    case ITEM_SYMBOL:		return "magical symbol";

    case ITEM_BOOK:		return "book";

    case ITEM_PAGE:		return "page";

    case ITEM_TOOL:		return "tool";

    case ITEM_COPPER:           return "copper";    

    case ITEM_IRON:             return "iron";

    case ITEM_STEEL:            return "steel";

    case ITEM_ADAMANTITE:       return "adamantite";

    case ITEM_GEMSTONE:         return "gemstone";

    case ITEM_HILT:             return "hilt";

    case ITEM_BOMB:             return "bomb";

    case ITEM_GEM:              return "gem";
    case ITEM_ORE:              return "ore";
    case ITEM_METAL:            return "metal bar";
}



    bug( "Item_type_name: unknown type %d.", obj->item_type );

    return "(unknown)";

}







/*

 * Return ascii name of an affect location.

 */

char *affect_loc_name( int location )

{

    switch ( location )

    {

    case APPLY_NONE:		return "none";

    case APPLY_STR:		return "strength";

    case APPLY_DEX:		return "dexterity";

    case APPLY_INT:		return "intelligence";

    case APPLY_WIS:		return "wisdom";

    case APPLY_CON:		return "constitution";

    case APPLY_SEX:		return "sex";

    case APPLY_CLASS:		return "class";

    case APPLY_LEVEL:		return "level";

    case APPLY_AGE:		return "age";

    case APPLY_MANA:		return "mana";

    case APPLY_HIT:		return "hp";

    case APPLY_MOVE:		return "moves";

    case APPLY_GOLD:		return "gold";

    case APPLY_EXP:		return "experience";

    case APPLY_AC:		return "armor class";

    case APPLY_HITROLL:		return "hit roll";

    case APPLY_DAMROLL:		return "damage roll";

    case APPLY_SAVING_PARA:	return "save vs paralysis";

    case APPLY_SAVING_ROD:	return "save vs rod";

    case APPLY_SAVING_PETRI:	return "save vs petrification";

    case APPLY_SAVING_BREATH:	return "save vs breath";

    case APPLY_SAVING_SPELL:	return "save vs spell";

    case APPLY_POLY:		return "polymorph form";

    }



    bug( "Affect_location_name: unknown location %d.", location );

    return "(unknown)";

}







/*

 * Return ascii name of an affect bit vector.

 */
char *affect_bit_name2( int vector )
{
   static char buf[512];
   buf[0] = '\0';
   if( vector & AFF2_AURA           ) strcat(buf, " aura"    );
   if( vector & AFF2_REGEN          ) strcat(buf, " regeneration");
   if( vector & AFF2_CONTAGION      ) strcat(buf, " contagion");
   if( vector & AFF2_SLOW           ) strcat(buf, " slow");
   if( vector & AFF2_MANASHIELD	    ) strcat(buf, " manashield");
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';

    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) strcat( buf, " invisible"     );
    if ( vector & AFF_DETECT_EVIL   ) strcat( buf, " detect_evil"   );
    if ( vector & AFF_DETECT_INVIS  ) strcat( buf, " detect_invis"  );
    if ( vector & AFF_DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & AFF_SHADOWPLANE   ) strcat( buf, " shadowplane"   );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_FLAMING       ) strcat( buf, " flaming"       );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_PROTECT       ) strcat( buf, " protect"       );
    if ( vector & AFF_ETHEREAL      ) strcat( buf, " ethereal"      );
    if ( vector & AFF_SLEEP         ) strcat( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CHARM         ) strcat( buf, " charm"         );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) strcat( buf, " pass_door"     );
    if ( vector & AFF_POLYMORPH     ) strcat( buf, " polymorph"     );
    if ( vector & PLR_SHADOWSIGHT   ) strcat( buf, " shadowsight"   );
    if ( vector & AFF_WEBBED        ) strcat( buf, " web"           );
    if ( vector & AFF_TENDRILS )     strcat(buf, " tendrils" );
    if ( vector & AFF_INFIRMITY     ) strcat( buf, " infirmity" );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}







/*

 * Return ascii name of extra flags vector.

 */

char *extra_bit_name( int extra_flags )

{

    static char buf[512];



    buf[0] = '\0';

    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );

    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );

    if ( extra_flags & ITEM_INVIS        ) strcat( buf, " invis"        );

    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );

    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );

    if ( extra_flags & ITEM_ANTI_GOOD    ) strcat( buf, " anti-good"    );

    if ( extra_flags & ITEM_ANTI_EVIL    ) strcat( buf, " anti-evil"    );

    if ( extra_flags & ITEM_ANTI_NEUTRAL ) strcat( buf, " anti-neutral" );

    if ( extra_flags & ITEM_BLESS        ) strcat( buf, " bless"        );

    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     );

    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );

    if ( extra_flags & ITEM_LOYAL        ) strcat( buf, " loyal"        );

    if ( extra_flags & ITEM_ONE_RING	 ) strcat( buf, " One Ring"	);

    if ( extra_flags & ITEM_SHADOWPLANE  ) strcat( buf, " shadowplane"  );

    if ( extra_flags & ITEM_THROWN       ) strcat( buf, " thrown"       );

    if ( extra_flags & ITEM_KEEP         ) strcat( buf, " keep"         );

    if ( extra_flags & ITEM_VANISH       ) strcat( buf, " vanish"       );

    return ( buf[0] != '\0' ) ? buf+1 : "none";

}



CHAR_DATA *get_char_world2( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;

    if (argument[0] == '\0') return NULL;

    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( IS_NPC(wch) && !str_cmp( argument, wch->short_descr ) ) return wch;
    }
    return NULL;
}



OBJ_DATA *get_obj_world2( CHAR_DATA *ch, char *argument )

{

    OBJ_DATA *obj;

    int vnum = 0;



    if (argument[0] == '\0') return NULL;



    for ( obj = object_list; obj != NULL; obj = obj->next )

    {

	if ( !str_cmp( argument, obj->short_descr ) )

	{

	    if ((vnum = obj->pIndexData->vnum) == 30037 || vnum == 30041)

		continue;

	    return obj;

	}

    }



    return NULL;

}

void total_affects( CHAR_DATA *ch, OBJ_DATA *obj )
{

   AFFECT_DATA *paf;
   int hr_total = 0;
   int dr_total = 0;
   int ac_total = 0;
   int str_total = 0;
   int wis_total = 0;
   int dex_total = 0;
   int int_total = 0;
   int con_total = 0;
   int hit_total = 0;
   int mana_total = 0;
   int move_total = 0;

   for( paf = obj->affected; paf != NULL; paf = paf->next )
   {
	switch( paf->location )
	{
	   case APPLY_HITROLL:
		hr_total += paf->modifier;
		break;
	   case APPLY_DAMROLL:
		dr_total += paf->modifier;
		break;
	   case APPLY_AC:
		ac_total += paf->modifier;
		break;
	   case APPLY_HIT:
		hit_total += paf->modifier;
		break;
	   case APPLY_MANA:
		mana_total += paf->modifier;
		break;
	   case APPLY_MOVE:
		move_total += paf->modifier;
		break;
	   case APPLY_STR:
		str_total += paf->modifier;
		break;
	   case APPLY_WIS:
		wis_total += paf->modifier;
		break;
	   case APPLY_DEX:
		dex_total += paf->modifier;
		break;
	   case APPLY_INT:
		int_total += paf->modifier;
		break;
	   case APPLY_CON:
		con_total += paf->modifier;
		break;
	}
   }

   obj->affected = NULL;

   if( ac_total > 225 )
	ac_total = 225;

   if( hr_total != 0 )
	mob_oset_affect(ch, obj, hr_total, APPLY_HITROLL, TRUE);
   if( dr_total != 0 )
	mob_oset_affect(ch, obj, dr_total, APPLY_DAMROLL, TRUE);
   if( ac_total != 0 )
	mob_oset_affect(ch, obj, ac_total, APPLY_AC, TRUE);
   if( hit_total != 0 )
	mob_oset_affect(ch, obj, hit_total, APPLY_HIT, TRUE);
   if( mana_total != 0 )
	mob_oset_affect(ch, obj, mana_total, APPLY_MANA, TRUE);
   if( move_total != 0 )
	mob_oset_affect(ch, obj, move_total, APPLY_MOVE, TRUE);
   if( str_total != 0 )
	mob_oset_affect(ch, obj, str_total, APPLY_STR, TRUE);
   if( int_total != 0 )
	mob_oset_affect(ch, obj, int_total, APPLY_INT, TRUE);
   if( wis_total != 0 )
	mob_oset_affect(ch, obj, wis_total, APPLY_WIS, TRUE);
   if( dex_total != 0 )
	mob_oset_affect(ch, obj, dex_total, APPLY_DEX, TRUE);
   if( con_total != 0 )
	mob_oset_affect(ch, obj, con_total, APPLY_CON, TRUE);

   return;
}


