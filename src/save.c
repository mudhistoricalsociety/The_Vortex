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

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];



/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest ) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );
void 	save_char_obj_finger 	args( ( CHAR_DATA *ch ) );

void    convert_old_new args( ( CHAR_DATA *ch ) );

/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj( CHAR_DATA *ch )
{
    char strsave[MAX_STRING_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) || ch->level < 2 )
	return;

    if ( ch->desc != NULL && ch->desc->connected != CON_PLAYING )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;
/*
    save_char_obj_backup( ch );
*/

    ch->save_time = current_time;
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->pcdata->switchname ) );
    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
		fwrite_char( ch, fp );
		
	if ( ch->carrying != NULL )
	    fwrite_obj( ch, ch->carrying, fp, 0 );
	fprintf( fp, "#END\n" );
    }
    fflush( fp );
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    save_char_obj_finger( ch );
    return;
}

void save_char_obj_backup( CHAR_DATA *ch )
{
    char chlevel [15];
    char buf [MAX_INPUT_LENGTH];
    char strsave[MAX_STRING_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) || ch->level < 2 )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    ch->save_time = current_time;
    fclose( fpReserve );
    sprintf( strsave, "%sstore/%s", PLAYER_DIR, capitalize(ch->pcdata->switchname) );
    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
		fwrite_char( ch, fp );

	if ( ch->carrying != NULL )
	    fwrite_obj( ch, ch->carrying, fp, 0 );
	fprintf( fp, "#END\n" );
	if      (ch->level >= 11) sprintf(chlevel,"<Implementor>");
	else if (ch->level == 10) sprintf(chlevel,"<High Judge>");
	else if (ch->level == 9 ) sprintf(chlevel,"<Judge>");
	else if (ch->level == 8 ) sprintf(chlevel,"<Enforcer>");
	else if (ch->level == 7 ) sprintf(chlevel,"<Quest Maker>");
	else if (ch->level == 6 ) sprintf(chlevel,"<Builder>");
	else if (ch->level == 5 ) sprintf(chlevel,"<Avatar>");
	else if (ch->level == 4 ) sprintf(chlevel,"<Avatar>");
	else if (ch->level == 3 ) sprintf(chlevel,"<Avatar>");
	else                      sprintf(chlevel,"<Mortal>");
	if (strlen(ch->lasttime) > 1)
	    sprintf(buf,"%s Last logged in on %s.\n\r", chlevel, ch->lasttime);
	else
	    sprintf(buf,"%s New player logged in on %s.\n\r", chlevel, ch->createtime);
	fprintf( fp, buf);
    }
    fflush( fp );
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void save_char_obj_finger( CHAR_DATA *ch )
{
    char strsave[MAX_STRING_LENGTH];
    FILE *fp;

    if ( IS_NPC(ch) || ch->level < 2 )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    ch->save_time = current_time;
    fclose( fpReserve );
    sprintf( strsave, "%sbackup/%s", PLAYER_DIR, capitalize(ch->pcdata->switchname) );
    if ( ( fp = fopen( strsave, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
    	fprintf( fp, "%s~\n",	ch->pcdata->switchname		);
	fprintf( fp, "%s~\n",	ch->pcdata->title	);
    	fprintf( fp, "%s~\n",	ch->lasthost		);
    	fprintf( fp, "%s~\n",	ch->lasttime		);
    	fprintf( fp, "%s~\n",	ch->createtime		);
    	fprintf( fp, "%d\n",	ch->level		);
    	fprintf( fp, "%d\n",	ch->race		);
        fprintf( fp, "%d\n",    ch->generation          );
        fprintf( fp, "%d\n",    ch->pcdata->souls       );
    	fprintf( fp, "%d\n",	ch->pkill );
    	fprintf( fp, "%d\n",	ch->pdeath );
	fprintf( fp, "%d\n",    ch->pcdata->awins       );
	fprintf( fp, "%d\n",    ch->pcdata->alosses	);
    	fprintf( fp, "%d\n",	ch->mkill );
    	fprintf( fp, "%d\n",	ch->mdeath );
	fprintf( fp, "%s~\n",	ch->clan);
    }
    fflush( fp );
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}



/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYERS"		);

    fprintf( fp, "Name         %s~\n",	ch->name		);
     if( ch->pcdata->version != CURRENT_PFILE_VERSION)
      ch->pcdata->version = CURRENT_PFILE_VERSION;
    fprintf( fp, "Version      %d\n",   ch->pcdata->version     );
    fprintf( fp, "Setlog       %s~\n",  ch->setlog		);
    fprintf( fp, "Setexit      %s~\n",  ch->setexit		);
    fprintf( fp, "Setdecap     %s~\n",  ch->pcdata->decap_mess  );
    fprintf( fp, "Switchname   %s~\n",  ch->pcdata->switchname  );
    fprintf( fp, "ShortDescr   %s~\n",	ch->short_descr		);
    fprintf( fp, "LongDescr    %s~\n",	ch->long_descr		);
    fprintf( fp, "Remort       %d\n",   ch->pcdata->remort      );
    fprintf( fp, "Clan         %s~\n",	ch->clan		);
    fprintf( fp, "Class        %d\n",	ch->class		);
    fprintf( fp, "Createtime   %s~\n",	ch->createtime		);
    fprintf( fp, "Lasttime     %s~\n",	ch->lasttime		);
    fprintf( fp, "Lasthost     %s~\n",	ch->lasthost		);
    fprintf( fp, "Prompt       %s~\n",	ch->prompt		);
    fprintf( fp, "Cprompt      %s~\n",	ch->cprompt		);
    fprintf( fp, "Who          %s~\n",  ch->pcdata->who         );
    fprintf( fp, "Sex          %d\n",	ch->sex			);
    fprintf( fp, "Race         %d\n",	ch->race		);
    fprintf( fp, "Immune       %d\n",	ch->immune		);
    fprintf( fp, "Rage         %d\n",   ch->rage                );
    fprintf( fp, "Generation   %d\n",   ch->generation          );
    fprintf( fp, "Souls	      %d\n",   ch->pcdata->souls	);
    fprintf( fp, "Itemaffect   %d\n",	ch->itemaffect		);
    fprintf( fp, "Home         %d\n",	ch->home		);
    fprintf( fp, "Recall2      %d\n",   ch->recall2             );
    fprintf( fp, "Level        %d\n",	ch->level		);
    fprintf( fp, "Trust        %d\n",	ch->trust		);
    fprintf( fp, "Powerblock   %d %d %d %d %d %d\n", ch->ABlock[POWER_TYPE],
     ch->ABlock[POWER_LEVEL], ch->BBlock[POWER_TYPE], ch->BBlock[POWER_LEVEL],
     ch->CBlock[POWER_TYPE], ch->CBlock[POWER_LEVEL]);
    fprintf( fp, "Paradox      %d %d %d\n", ch->paradox[0], ch->paradox[1], 
                                             ch->paradox[2] );
    fprintf( fp, "Played       %d\n",
	ch->played + (int) (current_time - ch->logon)		);
    fprintf( fp, "Room         %d\n",
	(  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
	&& ch->was_in_room != NULL )
	    ? ch->was_in_room->vnum
	    : ch->in_room->vnum );

    fprintf( fp, "PkPdMkMd     %d %d %d %d\n",
	ch->pkill, ch->pdeath, ch->mkill, ch->mdeath );
    fprintf( fp, "Alos         %d\n", ch->pcdata->alosses);
    fprintf( fp, "Awin         %d\n", ch->pcdata->awins); 
    fprintf( fp, "Weapons      %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	ch->wpn[0], ch->wpn[1], ch->wpn[2], ch->wpn[3], ch->wpn[4], 
	ch->wpn[5], ch->wpn[6], ch->wpn[7], ch->wpn[8], ch->wpn[9], 
	ch->wpn[10], ch->wpn[11], ch->wpn[12] );
    fprintf( fp, "Bounty       %d %d\n", ch->pcdata->bounty[0], ch->pcdata->bounty[1]);
    fprintf( fp, "Spells       %d %d %d %d %d\n",
	ch->spl[0], ch->spl[1], ch->spl[2], ch->spl[3], ch->spl[4] );
    fprintf( fp, "Combat       %d %d %d %d %d %d %d %d\n",
	ch->cmbt[0], ch->cmbt[1], ch->cmbt[2], ch->cmbt[3],
	ch->cmbt[4], ch->cmbt[5], ch->cmbt[6], ch->cmbt[7] );
    fprintf( fp, "Stance       %d %d %d %d %d %d %d %d %d %d %d %d\n",
	ch->stance[0], ch->stance[1], ch->stance[2], ch->stance[3],
	ch->stance[4], ch->stance[5], ch->stance[6], ch->stance[7],
	ch->stance[8], ch->stance[9], ch->stance[10], ch->stance[11] );
    fprintf( fp, "Locationhp   %d %d %d %d %d %d %d\n",
	ch->loc_hp[0], ch->loc_hp[1], ch->loc_hp[2], ch->loc_hp[3],
	ch->loc_hp[4], ch->loc_hp[5], ch->loc_hp[6] );
    fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n",
	ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move );
    fprintf( fp, "Warps        %d\n",   ch->warp                );
    fprintf( fp, "WarpCount    %d\n",   ch->warpcount           );
    fprintf( fp, "Explevel     %d\n",   ch->explevel            );
    fprintf( fp, "Expgained    %d\n",   ch->expgained           );
    fprintf( fp, "Exp          %d\n",	ch->exp			);
    fprintf( fp, "Levelexp     %d\n",   ch->levelexp		);
    fprintf( fp, "Act          %d\n",   ch->act			);
    fprintf( fp, "Special      %d\n",   ch->special		);
    fprintf( fp, "Newbits      %d\n",   ch->newbits             );
    fprintf( fp, "Clanbits     %d\n",   ch->clanbits            );
    fprintf( fp, "Extra        %d\n",   ch->extra		);
    fprintf( fp, "AffectedBy   %d\n",	ch->affected_by		);
    /* Bug fix from Alander */
    fprintf( fp, "Position     %d\n",
        ch->position == POS_FIGHTING ? POS_STANDING : ch->position );

    fprintf( fp, "Practice     %d\n",	ch->practice		);
    fprintf( fp, "SavingThrow  %d\n",	ch->saving_throw	);
    fprintf( fp, "Alignment    %d\n",	ch->alignment		);
    fprintf( fp, "Hitroll      %d\n",	ch->hitroll		);
    fprintf( fp, "Damroll      %d\n",	ch->damroll		);
    fprintf( fp, "Armor        %d\n",	ch->armor		);
    fprintf( fp, "Wimpy        %d\n",	ch->wimpy		);
    fprintf( fp, "Deaf         %d\n",	ch->deaf		);

    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum         %d\n",	ch->pIndexData->vnum	);
    }
    else
    {
    fprintf( fp, "Racename     %s~\n",  ch->pcdata->race->name  );
    fprintf( fp, "Raceg1       %d\n",   ch->pcdata->race->group1); 
    fprintf( fp, "Raceg2       %d\n",   ch->pcdata->race->group2); 
    fprintf( fp, "Racecom      %d\n",   ch->pcdata->race->complete);
    fprintf( fp, "Password     %s~\n",	ch->pcdata->pwd		);
    fprintf( fp, "Bamfin       %s~\n",	ch->pcdata->bamfin	);
    fprintf( fp, "Bamfout      %s~\n",	ch->pcdata->bamfout	);
    fprintf( fp, "Title        %s~\n",	ch->pcdata->title	);
    fprintf( fp, "AttrPerm     %d %d %d %d %d\n",
	    ch->pcdata->perm_str,
	    ch->pcdata->perm_int,
	    ch->pcdata->perm_wis,
	    ch->pcdata->perm_dex,
	    ch->pcdata->perm_con );
    fprintf( fp, "AttrMod      %d %d %d %d %d\n",
	    ch->pcdata->mod_str, 
	    ch->pcdata->mod_int, 
	    ch->pcdata->mod_wis,
	    ch->pcdata->mod_dex, 
	    ch->pcdata->mod_con );
    fprintf( fp, "Quest        %d\n", ch->pcdata->quest );
	fprintf( fp, "Power        ");
	    for (sn=0; sn < 35; sn++)
	    {
		fprintf(fp, "%d ", ch->pcdata->powers[sn]);
	    }
	    fprintf(fp, "\n");

	fprintf( fp, "Stats        ");
	    for (sn=0; sn < 12; sn++)
	    {
		fprintf(fp, "%d ", ch->pcdata->stats[sn]);
	    }
	    fprintf(fp, "\n");

	if (ch->pcdata->obj_vnum != 0)
	    fprintf( fp, "Objvnum      %d\n", ch->pcdata->obj_vnum );

	fprintf( fp, "Condition    %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2] );

	fprintf( fp, "APoints	   %d\n", ch->pcdata->apoints );
        fprintf( fp, "ASkills       ");
            for (sn=0; sn < 20; sn++)
            {
                fprintf(fp, "%d ", ch->pcdata->askills[sn]);
            }
            fprintf(fp, "\n");
	fprintf( fp, "Pulsetimer    ");
         for( sn = 0; sn < MAX_PULSE_TIMER; sn++ )
         {
          fprintf(fp, "%d ", ch->pcdata->pulse_timer[sn]);
         }
         fprintf(fp, "\n");
	fprintf( fp, "Radio	%d\n",ch->pcdata->radio);
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	    {
		fprintf( fp, "Skill        %d '%s'\n",
		    ch->pcdata->learned[sn], skill_table[sn].name );
	    }
	}
    }

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	/* Thx Alander */
	if ( paf->type < 0 || paf->type >= MAX_SKILL )
	    continue;

	fprintf( fp, "AffectData   '%s' %3d %3d %3d %10d\n",
	    skill_table[paf->type].name,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }

    fprintf( fp, "End\n\n" );
    return;
}



/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
/*
    int vnum = obj->pIndexData->vnum;

     *
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content != NULL )
	fwrite_obj( ch, obj->next_content, fp, iNest );

    /*
     * Castrate storage characters.
     */

    if ( (obj->chobj != NULL && (!IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)) || obj->item_type == ITEM_KEY)
	return;

    fprintf( fp, "#OBJECT\n" );
    fprintf( fp, "Nest         %d\n",	iNest			     );
    fprintf( fp, "Name         %s~\n",	obj->name		     );
    fprintf( fp, "ShortDescr   %s~\n",	obj->short_descr	     );
    fprintf( fp, "Description  %s~\n",	obj->description	     );
    if (obj->chpoweron != NULL && strlen(obj->chpoweron) > 1 && str_cmp(obj->chpoweron,"(null)") )
    	fprintf( fp, "Poweronch    %s~\n",	obj->chpoweron	     );
    if (obj->chpoweroff != NULL && strlen(obj->chpoweroff) > 1 && str_cmp(obj->chpoweroff,"(null)"))
    	fprintf( fp, "Poweroffch   %s~\n",	obj->chpoweroff      );
    if (obj->chpoweruse != NULL && strlen(obj->chpoweruse) > 1 && str_cmp(obj->chpoweruse,"(null)"))
    	fprintf( fp, "Powerusech   %s~\n",	obj->chpoweruse      );
    if (obj->victpoweron != NULL && strlen(obj->victpoweron) > 1 && str_cmp(obj->victpoweron,"(null)"))
    	fprintf( fp, "Poweronvict  %s~\n",	obj->victpoweron     );
    if (obj->victpoweroff != NULL && strlen(obj->victpoweroff) > 1 && str_cmp(obj->victpoweroff,"(null)"))
    	fprintf( fp, "Poweroffvict %s~\n",	obj->victpoweroff    );
    if (obj->victpoweruse != NULL && strlen(obj->victpoweruse) > 1 && str_cmp(obj->victpoweruse,"(null)"))
    	fprintf( fp, "Powerusevict %s~\n",	obj->victpoweruse    );
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    	fprintf( fp, "Questmaker   %s~\n",	obj->questmaker      );
    if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    	fprintf( fp, "Questowner   %s~\n",	obj->questowner      );

    fprintf( fp, "Vnum         %d\n",	obj->pIndexData->vnum	     );
    fprintf( fp, "Gemcount     %d\n",   obj->gem_count               );
    fprintf( fp, "ExtraFlags   %d\n",	obj->extra_flags	     );
    fprintf( fp, "ExtraFlags2  %d\n",   obj->extra_flags2	     );
    fprintf( fp, "WeapFlags    %d\n",   obj->weapflags		     );
    fprintf( fp, "WearFlags    %d\n",	obj->wear_flags		     );
    fprintf( fp, "WearLoc      %d\n",	obj->wear_loc		     );
    fprintf( fp, "ItemType     %d\n",	obj->item_type		     );
    fprintf( fp, "Weight       %d\n",	obj->weight		     );
    if (obj->spectype != 0)
    	fprintf( fp, "Spectype     %d\n",	obj->spectype	     );
    if (obj->specpower != 0)
    	fprintf( fp, "Specpower    %d\n",	obj->specpower	     );
    fprintf( fp, "Condition    %d\n",	obj->condition		     );
    fprintf( fp, "Toughness    %d\n",	obj->toughness		     );
    fprintf( fp, "Resistance   %d\n",	obj->resistance		     );
    if (obj->quest != 0)
    	fprintf( fp, "Quest        %d\n",	obj->quest	     );
    if (obj->points != 0)
    	fprintf( fp, "Points       %d\n",	obj->points	     );
    fprintf( fp, "Level        %d\n",	obj->level		     );
    fprintf( fp, "Timer        %d\n",	obj->timer		     );
    fprintf( fp, "Cost         %d\n",	obj->cost		     );
    fprintf( fp, "Values       %d %d %d %d\n",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3]	     );
    fprintf( fp, "Durability   %d\n", obj->durability		     );
    switch ( obj->item_type )
    {
    case ITEM_POTION:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1      '%s'\n", 
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2      '%s'\n", 
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_SCROLL:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1      '%s'\n", 
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2      '%s'\n", 
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3      '%s'\n", 
		skill_table[obj->value[3]].name );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
/*
	if ( paf->type < 0 || paf->type >= MAX_SKILL ) continue;
*/
	fprintf( fp, "AffectData   %d %d %d\n",
	    paf->duration, paf->modifier, paf->location );
    }

    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fprintf( fp, "ExtraDescr   %s~ %s~\n",
	    ed->keyword, ed->description );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1 );

    return;
}



/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    static PC_DATA pcdata_zero;
    static RACE_DATA racedata_zero;
    char strsave[MAX_STRING_LENGTH];
    char *strtime;
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int sn;
    extern bool PWipe;

    if ( char_free == NULL )
    {
	ch				= alloc_perm( sizeof(*ch) );
    }
    else
    {
	ch				= char_free;
	char_free			= char_free->next;
    }
    clear_char( ch );

    if ( pcdata_free == NULL )
    {
	ch->pcdata			= alloc_perm( sizeof(*ch->pcdata) );
    }
    else
    {
	ch->pcdata			= pcdata_free;
	pcdata_free			= pcdata_free->next;
    }
    *ch->pcdata				= pcdata_zero;

    if( racedata_free == NULL)
    {
     ch->pcdata->race                   = alloc_perm( sizeof(*ch->pcdata->race) );
    }
    else
    {
       ch->pcdata->race                 = racedata_free;
       racedata_free                    = racedata_free->next;
    }
    *ch->pcdata->race                   = racedata_zero;
    d->character			= ch;
    ch->desc				= d;
    ch->name				= str_dup( name );
    ch->setlog				= str_dup( "" );
    ch->setexit				= str_dup( "" );
    ch->pcdata->held                    = str_dup( "" );
    ch->pcdata->held_by                 = str_dup( "" );
    ch->pcdata->decap_mess              = str_dup( "" );
    ch->pcdata->switchname		= str_dup( name );
    ch->pcdata->race->name              = str_dup( "" );
    ch->pcdata->version                 = 1;
    ch->act				= PLR_BLANK
					| PLR_COMBINE
					| PLR_PROMPT;
    ch->extra				= 0;
    ch->generation                      = 1;
    ch->rage                            = 0;
    ch->stasis                          = FALSE;
    ch->special                         = 0;
    ch->newbits                         = 0;
    ch->clanbits                        = 0;
    ch->class                           = 0;
    ch->pcdata->radio                   = -1;
    ch->pcdata->familiar		= NULL;
    ch->pcdata->chobj			= NULL;
    ch->pcdata->who                     = str_dup( "" );
    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->title			= str_dup( "" );
    ch->clan				= str_dup( "" );
    ch->morph				= str_dup( "" );
    ch->pload				= str_dup( "" );
    ch->prompt				= str_dup( "" );
    ch->cprompt				= str_dup( "" );
    ch->ABlock[POWER_TYPE]  = 0;
    ch->ABlock[POWER_LEVEL] = 0;
    ch->BBlock[POWER_TYPE]  = 0;
    ch->BBlock[POWER_LEVEL] = 0;
    ch->CBlock[POWER_TYPE]  = 0;
    ch->CBlock[POWER_LEVEL] = 0;
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    free_string(ch->lasttime);
    ch->createtime			= str_dup( strtime );
    ch->lasttime			= str_dup( "" );
    ch->lasthost			= str_dup( "" );
    ch->hunting				= str_dup( "" );
    ch->pcdata->followers               = 0;
    ch->mounted				= 0;
    ch->home				= 3001;
    ch->recall2                         = 3001;
    ch->paradox[0]		        = 0;	/* Total paradox */
    ch->paradox[1]		        = 0;	/* Current Paradox */
    ch->paradox[2]	          	= 0;	/* Paradox Ticker */
    ch->itemaffect			= 0;
    ch->immune				= 0;
    for( sn=0 ; sn < 7 ; sn ++ )
    	ch->loc_hp[sn]			= 0;
    for( sn=0 ; sn < 13 ; sn++ )
   	 ch->wpn[sn]			= 0;
    for( sn=0 ; sn < 5 ; sn++ )
   	 ch->spl[sn]			= 0;
    for( sn=0 ; sn < 8 ; sn++ )
   	 ch->cmbt[sn]			= 0;
    for( sn=0 ; sn < 11 ; sn++ )
   	 ch->cmbt[sn]			= 0;

    ch->pkill				= 0;
    ch->pdeath				= 0;
    ch->mkill				= 0;
    ch->mdeath				= 0;
    ch->class				= -1;
    ch->pcdata->followers               = 0;
    ch->pcdata->perm_str		= 13;
    ch->pcdata->perm_int		= 13; 
    ch->pcdata->perm_wis		= 13;
    ch->pcdata->perm_dex		= 13;
    ch->pcdata->perm_con		= 13;
    ch->pcdata->quest			= 0;
    ch->pcdata->wolf			= 0;
    ch->pcdata->rank			= 0;

    for ( sn=0 ; sn < 35 ; sn++)
    	ch->pcdata->powers[sn]		= 0;

    for ( sn=0 ; sn < 12 ; sn++)
	ch->pcdata->stats[sn]		= 0;

    ch->pcdata->obj_vnum		= 0;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->apoints			= 0;
    for ( sn= 0 ; sn < 20 ; sn++)
        ch->pcdata->askills[sn]          = 0;
    for( sn = 0; sn < MAX_PULSE_TIMER; sn++)
     ch->pcdata->pulse_timer[sn] = 0;

    found = FALSE;
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );

	    if ( !str_cmp( word, "PLAYERS" ) ) fread_char    ( ch, fp );
            else if ( PWipe ) return 0;
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj      ( ch, fp );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}

bool load_char_short( DESCRIPTOR_DATA *d, char *name )
{
    static PC_DATA pcdata_zero;
    static RACE_DATA racedata_zero;
    char strsave[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    char *strtime;
    FILE *fp;
    bool found;

    if ( char_free == NULL )
    {
	ch				= alloc_perm( sizeof(*ch) );
    }
    else
    {
	ch				= char_free;
	char_free			= char_free->next;
    }
    clear_char( ch );

    if ( pcdata_free == NULL )
    {
	ch->pcdata			= alloc_perm( sizeof(*ch->pcdata) );
    }
    else
    {
	ch->pcdata			= pcdata_free;
	pcdata_free			= pcdata_free->next;
    }
    *ch->pcdata				= pcdata_zero;

    if ( racedata_free == NULL )
    {
        ch->pcdata->race                  = alloc_perm( sizeof(*ch->pcdata->race) );
    }
    else
    {
        ch->pcdata->race                = racedata_free;
        racedata_free                   = racedata_free->next;
    }
    *ch->pcdata->race                   = racedata_zero;

    d->character			= ch;
    ch->desc				= d;
    ch->name				= str_dup( name );
    ch->setlog				= str_dup( "" );
    ch->setexit				= str_dup( "" );
    ch->pcdata->held                    = str_dup( "" );
    ch->pcdata->held_by                 = str_dup( "" );
    ch->pcdata->decap_mess              = str_dup( "" );
    ch->pcdata->switchname		= str_dup( name );
    ch->act				= PLR_BLANK
					| PLR_COMBINE
					| PLR_PROMPT;
    ch->pcdata->remort                  = 0;
    ch->extra				= 0;
    ch->pcdata->radio                   = -1;
    ch->pcdata->familiar		= NULL;
    ch->pcdata->chobj			= NULL;
    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->title			= str_dup( "" );
    ch->clan				= str_dup( "" );
    ch->pload				= str_dup( "" );
    ch->prompt				= str_dup( "" );
    ch->cprompt				= str_dup( "" );
    ch->pcdata->race->name              = str_dup( "" );
    strtime = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    free_string(ch->lasttime);
    ch->createtime			= str_dup( strtime );
    ch->lasttime			= str_dup( "" );
    ch->lasthost			= str_dup( "" );
    ch->hunting				= str_dup( "" );
    ch->mounted				= 0;
    ch->home				= 3001;
    ch->recall2                         = 3001;
    ch->paradox[0]		        = 0;	/* Total paradox */
    ch->paradox[1]		        = 0;	/* Current Paradox */
    ch->paradox[2]	          	= 0;	/* Paradox Ticker */
    ch->itemaffect			= 0;
    ch->immune				= 0;
    ch->loc_hp[0]			= 0;
    ch->loc_hp[1]			= 0;
    ch->loc_hp[2]			= 0;
    ch->loc_hp[3]			= 0;
    ch->loc_hp[4]			= 0;
    ch->loc_hp[5]			= 0;
    ch->loc_hp[6]			= 0;
    ch->wpn[0]				= 0;
    ch->wpn[1]				= 0;
    ch->wpn[2]				= 0;
    ch->wpn[3]				= 0;
    ch->wpn[4]				= 0;
    ch->wpn[5]				= 0;
    ch->wpn[6]				= 0;
    ch->wpn[7]				= 0;
    ch->wpn[8]				= 0;
    ch->wpn[9]				= 0;
    ch->wpn[10]				= 0;
    ch->wpn[11]				= 0;
    ch->wpn[12]				= 0;
    ch->spl[0]				= 4;
    ch->spl[1]				= 4;
    ch->spl[2]				= 4;
    ch->spl[3]				= 4;
    ch->spl[4]				= 4;
    ch->cmbt[0]				= 0;
    ch->cmbt[1]				= 0;
    ch->cmbt[2]				= 0;
    ch->cmbt[3]				= 0;
    ch->cmbt[4]				= 0;
    ch->cmbt[5]				= 0;
    ch->cmbt[6]				= 0;
    ch->cmbt[7]				= 0;
    ch->stance[0]			= 0;
    ch->stance[1]			= 0;
    ch->stance[2]			= 0;
    ch->stance[3]			= 0;
    ch->stance[4]			= 0;
    ch->stance[5]			= 0;
    ch->stance[6]			= 0;
    ch->stance[7]			= 0;
    ch->stance[8]			= 0;
    ch->stance[9]			= 0;
    ch->stance[10]			= 0;
    ch->stance[11]                      = 0;
    ch->pkill				= 0;
    ch->pdeath				= 0;
    ch->mkill				= 0;
    ch->mdeath				= 0;
    ch->class				= 0;
    ch->pcdata->souls                   = 0;
    ch->pcdata->followers               = 0;
    ch->pcdata->perm_str		= 13;
    ch->pcdata->perm_int		= 13; 
    ch->pcdata->perm_wis		= 13;
    ch->pcdata->perm_dex		= 13;
    ch->pcdata->perm_con		= 13;
    ch->pcdata->quest			= 0;
    ch->pcdata->wolf			= 0;
    ch->pcdata->rank			= 0;
    ch->pcdata->powers[0]		= 0;
    ch->pcdata->powers[1]		= 0;
    ch->pcdata->obj_vnum		= 0;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->apoints			= 0;
    ch->level                           = 0;

    found = FALSE;
    fclose( fpReserve );
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYERS" ) ) fread_char ( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) break;
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )	if ( !str_cmp( word, literal ) ){field  = value;fMatch = TRUE;break;}


void fread_obj( CHAR_DATA *ch, FILE *fp )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    char buf[MAX_INPUT_LENGTH];

    if ( obj_free == NULL )
    {
	obj		= alloc_perm( sizeof(*obj) );
    }
    else
    {
	obj		= obj_free;
	obj_free	= obj_free->next;
    }

    *obj		= obj_zero;
    obj->name		= str_dup( "" );
    obj->short_descr	= str_dup( "" );
    obj->description	= str_dup( "" );
    obj->chpoweron	= str_dup( "(null)" );
    obj->chpoweroff	= str_dup( "(null)" );
    obj->chpoweruse	= str_dup( "(null)" );
    obj->victpoweron	= str_dup( "(null)" );
    obj->victpoweroff	= str_dup( "(null)" );
    obj->victpoweruse	= str_dup( "(null)" );
    obj->questmaker	= str_dup( "" );
    obj->questowner	= str_dup( "" );
    obj->spectype	= 0;
    obj->specpower	= 0;
    obj->condition	= 100;
    obj->toughness	= 0;
    obj->resistance	= 100;
    obj->quest		= 0;
    obj->points		= 0;
    obj->gem_count      = 0;
    obj->durability	= 0;
    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	    {
		AFFECT_DATA *paf;

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
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= 0;
		paf->next	= obj->affected;
		obj->affected	= paf;
		fMatch		= TRUE;
		break;
	    }
	    break;

	case 'C':
	    KEY( "Condition",	obj->condition,		fread_number( fp ) );
	    KEY( "Cost",	obj->cost,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	obj->description,	fread_string( fp ) );
	    KEY( "Durability",	obj->durability,	fread_number( fp ) );
	    break;

	case 'E':
	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
            KEY( "ExtraFlags2", obj->extra_flags2,      fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) )
	    {
		EXTRA_DESCR_DATA *ed;

		if ( extra_descr_free == NULL )
		{
		    ed			= alloc_perm( sizeof(*ed) );
		}
		else
		{
		    ed			= extra_descr_free;
		    extra_descr_free	= extra_descr_free->next;
		}

		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum )
		{
		    bug( "Fread_obj: incomplete object.", 0 );
		    free_string( obj->name        );
		    free_string( obj->description );
		    free_string( obj->short_descr );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
		    obj->next	= object_list;
		    object_list	= obj;
		    obj->pIndexData->count++;
		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
			obj_to_char( obj, ch );
		    else
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    return;
		}
	    }
	    break;
        case 'G':
            KEY("Gemcount",     obj->gem_count,        fread_number(fp));
            break;
	case 'I':
	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Level",	obj->level,		fread_number( fp ) );
	    break;

	case 'N':

	    KEY( "Name",	obj->name,		fread_string( fp ) );

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    break;

	case 'P':
	    KEY( "Points",	obj->points,		fread_number( fp ) );
	    KEY( "Poweronch",	obj->chpoweron,		fread_string( fp ) );
	    KEY( "Poweroffch",	obj->chpoweroff,	fread_string( fp ) );
	    KEY( "Powerusech",	obj->chpoweruse,	fread_string( fp ) );
	    KEY( "Poweronvict",	obj->victpoweron,	fread_string( fp ) );
	    KEY( "Poweroffvict",obj->victpoweroff,	fread_string( fp ) );
	    KEY( "Powerusevict",obj->victpoweruse,	fread_string( fp ) );
	    break;

	case 'Q':
	    KEY( "Quest",	obj->quest,		fread_number( fp ) );
	    KEY( "Questmaker",	obj->questmaker,	fread_string( fp ) );
	    KEY( "Questowner",	obj->questowner,	fread_string( fp ) );
	    break;

	case 'R':
	    KEY( "Resistance",	obj->resistance,	fread_number( fp ) );
	    break;

	case 'S':
	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( "Spectype",	obj->spectype,		fread_number( fp ) );
	    KEY( "Specpower",	obj->specpower,		fread_number( fp ) );

	    if ( !str_cmp( word, "Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 3 )
		{
		    bug( "Fread_obj: bad iValue %d.", iValue );
		}
		else if ( sn < 0 )
		{
		    bug( "Fread_obj: unknown skill.", 0 );
		}
		else
		{
		    obj->value[iValue] = sn;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Toughness",	obj->toughness,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Values" ) )
	    {
		obj->value[0]	= fread_number( fp );
		obj->value[1]	= fread_number( fp );
		obj->value[2]	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		fMatch		= TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Vnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		{
		    vnum = 30041;
		    if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
			bug( "Fread_obj: bad vnum %d.", vnum );
		    else
			fVnum = TRUE;
		}
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "WeapFlags",   obj->weapflags,		fread_number( fp ) );
	    KEY( "WearFlags",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "WearLoc",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Weight",	obj->weight,		fread_number( fp ) );
	    break;
	}
	if ( !fMatch )
	{
            sprintf( buf, "Fread_obj: no match->%s", word);
	    bug( buf, 0 );
	    fread_to_eol( fp );
	}
    }

}



void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int sn;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "Act",		ch->act,		fread_number( fp ) );
	    KEY( "AffectedBy",	ch->affected_by,	fread_number( fp ) );
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Armor",	ch->armor,		fread_number( fp ) );
	    KEY( "Awin",        ch->pcdata->awins,      fread_number( fp ) );
            KEY( "Alos",        ch->pcdata->alosses,    fread_number( fp ) );
	    KEY( "APoints",	ch->pcdata->apoints,	fread_number( fp ) );
            if ( !str_cmp( word, "ASkills" ) )
            {
                for ( sn = 0; sn < 20 ; sn++ )
                        ch->pcdata->askills[sn] = fread_number( fp );
                
                fMatch = TRUE;
                break;
            }
	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	    {
		AFFECT_DATA *paf;

		if ( affect_free == NULL )
		{
		    paf		= alloc_perm( sizeof(*paf) );
		}
		else
		{
		    paf		= affect_free;
		    affect_free	= affect_free->next;
		}

		if ( !str_cmp( word, "Affect" ) )
		{
		    /* Obsolete 2.0 form. */
		    paf->type	= fread_number( fp );
		}
		else
		{
		    int sn;

		    sn = skill_lookup( fread_word( fp ) );
		    if ( sn < 0 )
			bug( "Fread_char: unknown skill.", 0 );
		    else
			paf->type = sn;
		}

		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= ch->affected;
		ch->affected	= paf;
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrMod"  ) )
	    {
		ch->pcdata->mod_str  = fread_number( fp );
		ch->pcdata->mod_int  = fread_number( fp );
		ch->pcdata->mod_wis  = fread_number( fp );
		ch->pcdata->mod_dex  = fread_number( fp );
		ch->pcdata->mod_con  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) )
	    {
		ch->pcdata->perm_str = fread_number( fp );
		ch->pcdata->perm_int = fread_number( fp );
		ch->pcdata->perm_wis = fread_number( fp );
		ch->pcdata->perm_dex = fread_number( fp );
		ch->pcdata->perm_con = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
            if( !str_cmp( word, "Bounty") )
            {
             ch->pcdata->bounty[0] = fread_number(fp);
             ch->pcdata->bounty[1] = fread_number(fp);
             fMatch =TRUE;
             break;
            }
	    break;

	case 'C':
	    KEY( "Clan",	ch->clan,		fread_string( fp ) );
            KEY( "Clanbits",    ch->clanbits,           fread_number( fp ) );
	    KEY( "Class",	ch->class,       	fread_number( fp ) );
	    if ( !str_cmp( word, "Combat" ) )
	    {
		ch->cmbt[0]	= fread_number( fp );
		ch->cmbt[1]	= fread_number( fp );
		ch->cmbt[2]	= fread_number( fp );
		ch->cmbt[3]	= fread_number( fp );
		ch->cmbt[4]	= fread_number( fp );
		ch->cmbt[5]	= fread_number( fp );
		ch->cmbt[6]	= fread_number( fp );
		ch->cmbt[7]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Condition" ) )
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    KEY( "Cprompt",	ch->cprompt,		fread_string( fp ) );
	    KEY( "Createtime",	ch->createtime,		fread_string( fp ) );
	    break;

	case 'D':
	    KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	    KEY( "Deaf",	ch->deaf,		fread_number( fp ) );
	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
		return;
	    KEY( "Exp",		ch->exp,		fread_number( fp ) );
            KEY( "Explevel",	ch->explevel,		fread_number( fp ) );
	    KEY( "Expgained",	ch->expgained,		fread_number( fp ) );
	    KEY( "Extra",	ch->extra,		fread_number( fp ) );
	    break;

	case 'G':
	KEY( "Generation", ch->generation, fread_number( fp ) )
	    break;

	case 'H':
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );
	    KEY( "Home",        ch->home,		fread_number( fp ) );

	    if ( !str_cmp( word, "HpManaMove" ) )
	    {
		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->max_mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'I':
	    KEY( "Immune",        ch->immune,		fread_number( fp ) );
	    KEY( "Itemaffect",    ch->itemaffect,	fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Lasthost",	ch->lasthost,		fread_string( fp ) );
	    KEY( "Lasttime",	ch->lasttime,		fread_string( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Levelexp",    ch->levelexp,		fread_number( fp ) );
	    if ( !str_cmp( word, "Locationhp" ) )
	    {
		ch->loc_hp[0]	= fread_number( fp );
		ch->loc_hp[1]	= fread_number( fp );
		ch->loc_hp[2]	= fread_number( fp );
		ch->loc_hp[3]	= fread_number( fp );
		ch->loc_hp[4]	= fread_number( fp );
		ch->loc_hp[5]	= fread_number( fp );
		ch->loc_hp[6]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );
	    break;

	case 'N':
	    KEY( "Newbits",	ch->newbits,	fread_number( fp ) );

	    if ( !str_cmp( word, "Name" ) )
	    {
		/*
		 * Name already set externally.
		 */
		fread_to_eol( fp );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'O':
	    KEY( "Objvnum",     ch->pcdata->obj_vnum,	fread_number( fp ) );
	    break;

	case 'P':
	    if ( !str_cmp( word, "Paradox" ) )
	    {
		ch->paradox[0]	= fread_number( fp );
		ch->paradox[1]	= fread_number( fp );
		ch->paradox[2]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if( !str_cmp(word, "Pulsetimer"))
	    {
             for( sn = 0; sn < MAX_PULSE_TIMER; sn++ )
	      ch->pcdata->pulse_timer[sn] = fread_number(fp);
             fMatch = TRUE;
	     break;
	    }
            if( !str_cmp(word, "Powerblock"))
            {
              ch->ABlock[POWER_TYPE] = fread_number(fp);
              ch->ABlock[POWER_LEVEL] = fread_number(fp);
              ch->BBlock[POWER_TYPE] = fread_number(fp);
              ch->BBlock[POWER_LEVEL] = fread_number(fp);
              ch->CBlock[POWER_TYPE] = fread_number(fp);
              ch->CBlock[POWER_LEVEL] = fread_number(fp);
              fMatch = TRUE;
              break;
            }
	    KEY( "Password",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Played",	ch->played,		fread_number( fp ) );

	    if ( !str_cmp( word, "Power" ) )
	    {
                if( ch->pcdata->version == 1)
                {
		 for ( sn = 0; sn < 20; sn++ )
		 	ch->pcdata->powers[sn] = fread_number( fp );
                }
                else if( ch->pcdata->version == 3 )
                {
                 for( sn = 0; sn < 25; sn++)
                    ch->pcdata->powers[sn] = fread_number(fp);
                }
                else if( ch->pcdata->version == 4 || ch->pcdata->version == 5 )
                {
                 for( sn = 0; sn < 30; sn++)
                    ch->pcdata->powers[sn] = fread_number(fp);
                }
                else if( ch->pcdata->version >= 6)
                {
                 for( sn = 0; sn < 35; sn++)
                    ch->pcdata->powers[sn] = fread_number(fp);
                }
                else
                 fread_to_eol(fp);
		fMatch = TRUE;
		break;
	    }

	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    if ( !str_cmp( word, "PkPdMkMd" ) )
	    {
		ch->pkill	= fread_number( fp );
		ch->pdeath	= fread_number( fp );
		ch->mkill	= fread_number( fp );
		ch->mdeath	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Prompt",	ch->prompt,		fread_string( fp ) );
	    break;

	case 'Q':
	    KEY( "Quest",        ch->pcdata->quest,	fread_number( fp ) );
	    break;

	case 'R':
            KEY( "Remort",      ch->pcdata->remort,     fread_number( fp ));
            KEY( "Racename",    ch->pcdata->race->name, fread_string(fp));
            KEY( "Raceg1",      ch->pcdata->race->group1, fread_number(fp));
            KEY( "Raceg2",      ch->pcdata->race->group2, fread_number(fp));
            KEY( "Racecom",     ch->pcdata->race->complete, fread_number(fp));
	    KEY( "Race",        ch->race,		fread_number( fp ) );
            KEY( "Radio",	ch->pcdata->radio,	fread_number( fp ) );
	    KEY( "Rage",	ch->rage,               fread_number( fp ) );
	    KEY( "Rank",        ch->pcdata->rank,	fread_number( fp ) );
            KEY( "Recall2",     ch->recall2,            fread_number(fp)); 
	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Setlog",	ch->setlog, 		fread_string( fp ) );
	    KEY( "Setexit",	ch->setexit,		fread_string( fp ) );
            KEY( "Setdecap",    ch->pcdata->decap_mess, fread_string( fp ) );
	    KEY( "Switchname",  ch->pcdata->switchname, fread_string( fp ) );
	    KEY( "Souls",  ch->pcdata->souls, fread_number( fp ) );
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );

	    if ( !str_cmp( word, "Skill" ) )
	    {
		int sn;
		int value;

		value = fread_number( fp );
		sn    = skill_lookup( fread_word( fp ) );
		if ( sn >= 0 )
		    ch->pcdata->learned[sn] = value;
		fMatch = TRUE;
	    }

	    KEY( "Special",	ch->special,	fread_number( fp ) );


	    if ( !str_cmp( word, "Spells" ) )
	    {
		ch->spl[0]	= fread_number( fp );
		ch->spl[1]	= fread_number( fp );
		ch->spl[2]	= fread_number( fp );
		ch->spl[3]	= fread_number( fp );
		ch->spl[4]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Stance" ) )
	    {
		ch->stance[0]	= fread_number( fp );
		ch->stance[1]	= fread_number( fp );
		ch->stance[2]	= fread_number( fp );
		ch->stance[3]	= fread_number( fp );
		ch->stance[4]	= fread_number( fp );
		ch->stance[5]	= fread_number( fp );
		ch->stance[6]	= fread_number( fp );
		ch->stance[7]	= fread_number( fp );
		ch->stance[8]	= fread_number( fp );
		ch->stance[9]	= fread_number( fp );
		ch->stance[10]	= fread_number( fp );
                ch->stance[11]  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Stats" ) )
	    {
		for ( sn = 0; sn < 12 ; sn++ )
			ch->pcdata->stats[sn] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );

	    if ( !str_cmp( word, "Title" ) )
	    {
		ch->pcdata->title = fread_string( fp );
		if ( isalpha(ch->pcdata->title[0])
		||   isdigit(ch->pcdata->title[0]) )
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    free_string( ch->pcdata->title );
		    ch->pcdata->title = str_dup( buf );
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
            KEY( "Version",    ch->pcdata->version,     fread_number( fp ) );
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
            KEY( "Who",      ch->pcdata->who,   fread_string( fp ) );
	    KEY( "Warps",        ch->warp,       fread_number( fp ) );
            KEY( "WarpCount",   ch->warpcount,  fread_number( fp ) );
	    if ( !str_cmp( word, "Weapons" ) )
	    {
		ch->wpn[0]	= fread_number( fp );
		ch->wpn[1]	= fread_number( fp );
		ch->wpn[2]	= fread_number( fp );
		ch->wpn[3]	= fread_number( fp );
		ch->wpn[4]	= fread_number( fp );
		ch->wpn[5]	= fread_number( fp );
		ch->wpn[6]	= fread_number( fp );
		ch->wpn[7]	= fread_number( fp );
		ch->wpn[8]	= fread_number( fp );
		ch->wpn[9]	= fread_number( fp );
		ch->wpn[10]	= fread_number( fp );
		ch->wpn[11]	= fread_number( fp );
		ch->wpn[12]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    sprintf(buf, "Fread_char: no match. WORD: %s", word); 
	    bug( buf, 0 );
	    fread_to_eol( fp );
	}
    }
}
