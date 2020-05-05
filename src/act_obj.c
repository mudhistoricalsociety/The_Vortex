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
#define CD CHAR_DATA
void	get_obj		args( ( CHAR_DATA *ch, OBJ_DATA *obj,
			    OBJ_DATA *container ) );
CD *	find_keeper	args( ( CHAR_DATA *ch ) );
int	get_cost	args( ( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
void	sheath		args( ( CHAR_DATA *ch, bool right ) );
void	draw		args( ( CHAR_DATA *ch, bool right ) );
char	*special_item_name args( ( OBJ_DATA *obj ) );
void 	call_all	args( ( CHAR_DATA *ch ) );
#undef	CD
void set_forge_val(OBJ_DATA *obj, int type, bool armor, int value);
int set_armor_count(int value);
int set_forge_value( char arg[MAX_STRING_LENGTH]);
void copy_object(OBJ_DATA *obj, OBJ_DATA *copy);
bool check_quest(CHAR_DATA *ch, int value);

void do_rename( CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 if( IS_NPC(ch))
  return;
 if( arg1[0] == '\0')
 {
  stc("Rename what?\n\r", ch);
  return;
 }
 
 if( arg2[0] == '\0')
 {
  stc("Change the short long or name?\n\r", ch);
  return;
 }
 if( (obj = get_obj_carry(ch, arg1)) == NULL)
 {
  stc("You are not carrying that.\n\r", ch);
  return;
 }
 if( IS_SET(obj->extra_flags, ITEM_ONE_RING) )
 {
   stc("You are not powerful enough to change #oThe One Ring!#n\n\r", ch);
   return;
 }
 if( !str_cmp(obj->name, "A Champion's Sigil of Blood") && (ch->level < 12 || ch->trust < 12) )
 {
   stc("You aren't powerful enough to change eq created by the Gods!\n\r", ch);
   return;
 }
 if( (obj->item_type == ITEM_METAL)
  || (obj->item_type == ITEM_GEM)
  || (obj->item_type == ITEM_ORE))
 {
  stc("No.\n\r", ch);
  return;
 }
 if( strlen(argument) < 3)
 {
  stc("New names must be longer than 3 characters.\n\r", ch);
  return;
 }
 smash_tilde(argument);
 if( !str_cmp( arg2, "name"))
 {
  if( strlen(argument) > 75)
  {
   stc("Names can not exceed 75 characters.(This includes color codes).\n\r", ch);
   return;
  }
  free_string(obj->name);
  obj->name = str_dup(argument);
 }
 else if( !str_cmp( arg2, "short"))
 {
  if( strlen(argument) > 75)
  {
   stc("Short descriptions can not exceed 75 characters.(This includes color codes).\n\r", ch);
   return;
  }
  free_string(obj->short_descr);
  obj->short_descr = str_dup(argument);
 }
 else if( !str_cmp( arg2, "long"))
 {
  if( strlen(argument) > 75)
  {
   stc("Long descriptions can not exceed 75 characters.(This includes color codes).\n\r", ch);
   return;
  }
  free_string(obj->description);
  obj->description = str_dup(argument);
 }
 else
 {
  stc("Valid options are short long and name.\n\r", ch);
  return;
 }
 stc("Done.\n\r", ch);
 return;
}

void do_cutgem( CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 int chance = number_range(1,100);
 int chance2;
 int bit1;
  int bit2;

 one_argument( argument, arg);
 if( IS_NPC(ch))
  return;
 if( arg[0] =='\0')
 {
  stc("Cut what?\n\r", ch);
  return;
 }
 if( (obj = get_obj_carry(ch, arg)) == NULL)
 {
  stc("You aren't carrrying that.\n\r", ch);
  return;
 }
 if( obj->item_type != ITEM_GEM)
 {
  stc("That is not a gem.\n\r", ch);
  return;
 }
 if( IS_SET(obj->value[0], GEM_MASTER))
 {
  stc("That gem is already masterfuly cut.\n\r", ch);
  return;
 }
 if( IS_SET(obj->value[0], GEM_UNCUT))
 {
  chance2 = 10;
  bit1 = GEM_UNCUT;
  bit2 = GEM_SIMPLE;
 }
 else if( IS_SET(obj->value[0], GEM_SIMPLE))
 {
  chance2 = 15;
  bit1 = GEM_SIMPLE;
  bit2 = GEM_COMPLEX;
 }
 else if( IS_SET(obj->value[0], GEM_COMPLEX))
 {
  chance2 = 20;
  bit1 = GEM_COMPLEX;
  bit2 = GEM_MASTER;
 }
 if( chance < chance2)
 {
  stc("You set to work, cutting your gem but you shatter it!\n\r", ch);
  extract_obj(obj);
  return;
 }
 REMOVE_BIT(obj->value[0], bit1);
 SET_BIT(obj->value[0], bit2);
 stc("You have cut the gem to a higher precision.\n\r", ch);
 return;
}

void do_polish( CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 int chance = number_range(1,100);

 one_argument( argument, arg);
 if( IS_NPC(ch))
  return;
 if( arg[0] =='\0')
 {
  stc("Polish what?\n\r", ch);
  return;
 }
 if( (obj = get_obj_carry(ch, arg)) == NULL)
 {
  stc("You aren't carrrying that.\n\r", ch);
  return;
 }
 if( obj->item_type != ITEM_GEM)
 {
  stc("That is not a gem.\n\r", ch);
  return;
 }
 if( IS_SET(obj->value[0], GEM_POLISHED))
 {
  stc("That gem is already polished.\n\r",ch);
  return;
 }
 if( chance > 90)
 {
  stc("You fumble your attempts to polish the gem and cause it to crumble.\n\r", ch);
  extract_obj(obj);
  return;
 }
 REMOVE_BIT(obj->value[0], GEM_UNPOLISHED);
 SET_BIT(obj->value[0], GEM_POLISHED);
 stc("You have polished the gem to a high shine.\n\r", ch);
 return;
}

void do_inlay( CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj = NULL;
 OBJ_DATA *gem = NULL;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];
 int hit = 0;
 int dam = 0;
 int ac = 0;
 int hp = 0;
 int mana = 0;
 int move = 0;
 int multi = 0;
 int affect = 0;
 int con = 0;
 int str = 0;
 int dex = 0;
 int intel = 0;
 bool done = FALSE;
 bool nospell = FALSE;

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);
 argument = one_argument( argument, arg3);

 if( IS_NPC(ch))
  return;
 if( arg1[0] == '\0')
 {
  stc("Inlay which gem?\n\r", ch);
  return;
 }
 if( arg2[0] == '\0')
 {
  stc("Inlay the gem into what?\n\r", ch);
  return;
 }
 if( (arg3[0] != '\0') && !str_cmp(arg3, "nospell"))
  nospell = TRUE;
 if( (gem = get_obj_carry(ch, arg1)) == NULL)
 {
  stc("But you dont have that gem.\n\r", ch);
  return;
 }
 if( (obj = get_obj_carry(ch, arg2)) == NULL)
 {
  stc("You aren't carrying that object.\n\r", ch);
  return;
 }
 if( gem->item_type != ITEM_GEM)
 {
  stc("That is not a gem.\n\r", ch);
  return;
 }
 if( (obj->item_type != ITEM_ARMOR)
  && (obj->item_type != ITEM_WEAPON))
 {
  stc("That is not a weapon or piece of armor.\n\r", ch);
  return;
 }
 if( !IS_SET(obj->extra_flags, ITEM_PREPARED))
 {
  stc("That item has yet to be prepared for inlaying.\n\r", ch);
  return;
 }
 if( obj->gem_count == 0)
 {
  stc("You can't inlay anymore gems in that.\n\r", ch);
  return;
 }
 if( IS_SET(gem->value[0], GEM_UNCUT))
  multi -= 1;
 else if( IS_SET(gem->value[0], GEM_SIMPLE))
  multi = multi;
 else if( IS_SET(gem->value[0], GEM_COMPLEX))
  multi += 1 ;
 else if( IS_SET(gem->value[0], GEM_MASTER))
  multi += 2;
 if( IS_SET(gem->value[0], GEM_UNPOLISHED))
  multi = multi;
 else if( IS_SET(gem->value[0], GEM_POLISHED))
  multi += 1;
 if( IS_SET(gem->value[0], GEM_CRACKED))
  multi -= 1;
 else if( IS_SET(gem->value[0], GEM_FLAWED))
  multi = multi;
 else if( IS_SET(gem->value[0], GEM_HAZY))
  multi += 1;
 else if( IS_SET(gem->value[0], GEM_CLEAR))
  multi += 2;
 else if( IS_SET(gem->value[0], GEM_FLAWLESS))
  multi += 3;
 if( IS_SET(gem->value[0], GEM_MASTER)
  && IS_SET(gem->value[0], GEM_POLISHED)
  && IS_SET(gem->value[0], GEM_FLAWLESS))
 {
  switch(gem->value[1])
  {
   case 1:
    affect = 8;
    break;
   case 2:
    affect = 7;
    break;
   case 3:
    affect = 6;
    break;
   case 4:
    affect = 2;
    break;
   case 5:
    affect = 5;
    break;
   case 6:
    affect = 3;
    break;
   default:
    affect = 0;
    break;
  }
 }
  if( multi < 1)
   multi = 1;
 switch(gem->value[1])
 {
  case 1:
   hit = multi * 3;
   dam = multi * 3;
   ac  = multi * -5;
   break;
  case 2:
   hit = multi * 3;
   dam = multi * 3;
   break;
  case 3:
   dam = multi * 3;
   ac =  multi * -5;
   break;
  case 4:
   hit = multi * 3;
   ac = multi * -5;
  case 5:
   hit = multi * 3;
   break;
  case 6:
   hit = multi * 3;
   hp = multi * 10;
   mana = multi * 10;
   break;
  case 7:
   str = multi;
   mana = multi * 10;
   move = multi * 10;
   break;
  case 8:
   ac = multi * -5;
   break;
  case 9:
   hp = multi * 10;
   move = multi * 10;
   dex = multi;
   break;
  case 10:
   con = multi * .5;
   ac = multi * -3;
   intel = multi * .5;
  default:
   break;
 }
 if( !nospell && (affect != 0 ) && (obj->item_type == ITEM_WEAPON))
 { 
    if(obj->value[0] - ((obj->value[0] / 1000) * 1000) != 0)
     done = TRUE;
    if( !done)
    {    
     if(obj->value[0] >= 1000)
      obj->value[0] -= ((obj->value[0] / 1000) * 1000);     
     obj->value[0] += (affect*1000);
    }
 }
 if( !nospell && (affect !=0) && (obj->item_type == ITEM_ARMOR))
 {
  if( obj->value[3] == 0)
  {
   obj->value[3] = affect;
   done = TRUE;
  }
 }
 if( !done)
 {
 if( str != 0)
  oset_affect(ch, obj,str, APPLY_STR,TRUE);
 if( con !=0 )
  oset_affect(ch, obj,con, APPLY_CON,TRUE);
 if( dex != 0)
  oset_affect(ch, obj, dex, APPLY_DEX, TRUE);
 if( intel != 0)
  oset_affect(ch,obj,intel,APPLY_INT,TRUE);
 if( hit != 0)
  oset_affect(ch,obj,hit,APPLY_HITROLL,TRUE);
 if( dam != 0)
  oset_affect(ch,obj,dam,APPLY_DAMROLL,TRUE);
 if( ac != 0)
  oset_affect(ch,obj,ac,APPLY_AC, TRUE);
 if( hp != 0)
  oset_affect(ch,obj,hp,APPLY_HIT,TRUE);
 if( mana != 0)
  oset_affect(ch,obj,mana,APPLY_MANA,TRUE);
 if( move != 0)
  oset_affect(ch,obj,move,APPLY_MOVE,TRUE);
 }
 sprintf( arg1, "You inlay %s with %s.\n\r", obj->short_descr,
   gem->short_descr);
 stc(arg1, ch);
 extract_obj(gem);
 obj->gem_count--;
 return;
}

bool has_metal(CHAR_DATA *ch, int count, int type)
{
 OBJ_DATA *obj;
 bool ret_val = TRUE;

 for( obj = ch->carrying; obj != NULL; obj = obj->next_content)
 {
  if( obj->item_type != ITEM_METAL) /* is it a bar of metal */
   continue;
  if( obj->value[0] != type) /* is it the right type of metal*/
   continue;
  if( count == 0) /* if we have taken what we need lets not take more*/
  {
   ret_val = TRUE;
   continue;
  }
  count--;
 }
 if( count != 0)
 {
  stc("You dont have enough metal to forge that.\n\r", ch);
  return FALSE;
 }
 return ret_val;
}

void set_forge_val(OBJ_DATA *obj, int type, bool armor, int value)
{
 if( !armor)
 {
  SET_BIT(obj->wear_flags, ITEM_WIELD);
  obj->item_type = ITEM_WEAPON;   
  if( type == METAL_COPPER)
  {
   SET_BIT(obj->extra_flags, FORGE_COPPER);
   obj->value[1] = 10;
   obj->value[2] = 20;  
  }
  if( type == METAL_BRONZE)
  {
   SET_BIT(obj->extra_flags, FORGE_BRONZE);
   obj->value[1] = 15;
   obj->value[2] = 25;
  }
  if( type == METAL_IRON)
  {
   SET_BIT(obj->extra_flags, FORGE_IRON);
   obj->value[1] = 25;
   obj->value[2] = 35;
  }
  if( type == METAL_STEEL) 
  {
   SET_BIT(obj->extra_flags, FORGE_STEEL);
   obj->value[1] = 40;
   obj->value[2] = 45;
  }
  obj->value[3] = value;
  free_string(obj->name);
  free_string(obj->short_descr);
  free_string(obj->description);
  obj->name = str_dup("forged weapon");
  obj->short_descr = str_dup("A newly forged weapon");
  obj->description = str_dup("A newly forged weapon lies in the dust.");
 }
 else
 {
  obj->item_type = ITEM_ARMOR;
  if( type == METAL_COPPER)
  {
   obj->value[0] = 15; 
  SET_BIT(obj->extra_flags, FORGE_COPPER);
  }
  if( type == METAL_BRONZE)
  {
   obj->value[0] = 20;
   SET_BIT(obj->extra_flags, FORGE_BRONZE);
  }
  if( type == METAL_IRON)
  {
   obj->value[0] = 25;
   SET_BIT(obj->extra_flags, FORGE_IRON);
  }
  if( type == METAL_STEEL)
  {
   obj->value[0] = 30;
   SET_BIT(obj->extra_flags, FORGE_STEEL);
  }
  SET_BIT(obj->wear_flags, value);
  free_string(obj->name);
  free_string(obj->short_descr);
  free_string(obj->description);
  obj->name = str_dup("forged armor");
  obj->short_descr = str_dup("A newly forged piece of armor");
  obj->description = str_dup("A newly forged peice of armor lies in the dust.");
 }
 return;
}

void remove_bars(CHAR_DATA *ch, int count,int  type)
{
 OBJ_DATA *obj;
 OBJ_DATA *obj_next;
 char buf[MAX_STRING_LENGTH];
 
 for( obj = ch->carrying; obj != NULL; obj = obj_next)
 {
  obj_next = obj->next_content;
  if( obj->item_type != ITEM_METAL)
   continue;
  if( obj->value[0] != type)
   continue;
  if( count == 0) 
   continue;
  sprintf(buf,"You begin forging %s\n\r", obj->short_descr);
  stc(buf, ch);
  extract_obj(obj);
  count = count - 1;
 }
 return;
}
int set_forge_value( char arg[MAX_STRING_LENGTH])
{
  if( !str_cmp( arg, "pound"))
   return 7;
  else if( !str_cmp( arg, "slice"))
   return 1;
  else if( !str_cmp( arg, "crush"))
   return 8;
  else if( !str_cmp( arg, "stab"))
    return 2;
  else if( !str_cmp( arg, "grep"))
   return 9;
  else if( !str_cmp( arg, "slash"))
   return 3;
  else if( !str_cmp( arg, "bite"))
   return 10;
  else if( !str_cmp( arg, "whip"))
   return 4;
  else if( !str_cmp( arg, "pierce"))
   return 11;
  else if( !str_cmp( arg, "claw"))
   return 5;
  else if( !str_cmp( arg, "suck"))
   return 12;
  else if( !str_cmp( arg, "blast"))
   return 6;
  else if( !str_cmp( arg, "neck"))
   return ITEM_WEAR_NECK;
  else if( !str_cmp( arg, "ring"))
   return ITEM_WEAR_FINGER;
  else if( !str_cmp( arg, "body"))
   return ITEM_WEAR_BODY;
  else if( !str_cmp( arg, "head"))
   return ITEM_WEAR_HEAD;
  else if( !str_cmp( arg, "legs"))
   return ITEM_WEAR_LEGS;
  else if( !str_cmp( arg, "feet"))
   return ITEM_WEAR_FEET;
  else if( !str_cmp( arg, "hands"))
   return ITEM_WEAR_HANDS;
  else if( !str_cmp( arg, "arms"))
   return ITEM_WEAR_ARMS;
  else if( !str_cmp( arg, "about"))
   return ITEM_WEAR_ABOUT;
  else if( !str_cmp( arg, "waist"))
   return ITEM_WEAR_WAIST;
  else if( !str_cmp( arg, "wrist"))
   return ITEM_WEAR_WRIST;
  else if( !str_cmp( arg, "face"))
   return ITEM_WEAR_FACE;
  else
   return 0;
}

int set_armor_count(int value)
{
 if( value == ITEM_WEAR_FINGER)
  return 1;
 else if( value == ITEM_WEAR_NECK)
  return 2;
 else if( value == ITEM_WEAR_BODY)
  return 3;
 else if( value == ITEM_WEAR_HEAD)
  return 2;
 else if( value == ITEM_WEAR_LEGS)
  return 3;
 else if( value == ITEM_WEAR_FEET)
  return 2;
 else if( value == ITEM_WEAR_HANDS)
  return 1;
 else if( value == ITEM_WEAR_ARMS)
  return 3;
 else if( value == ITEM_WEAR_ABOUT)
  return 3;
 else if( value == ITEM_WEAR_WAIST)
  return 2;
 else if( value == ITEM_WEAR_WRIST)
  return 1;
 else if( value == ITEM_WEAR_FACE)
  return 1;
 else
  return 3;
}

void do_new_forge( CHAR_DATA *ch, char *argument)
{
 int count = 0;
 bool armor = FALSE;
 int value = 0;
 int type = 0;
 OBJ_DATA *obj;
 OBJ_INDEX_DATA *pObjIndex;

 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];

 argument = one_argument( argument, arg1);
 argument = one_argument( argument, arg2);
 argument = one_argument( argument, arg3);

 if( IS_NPC(ch))
  return;
 if( (arg1[0] == '\0') || (arg2[0] == '\0') || (arg3[0] == '\0'))
 {
  stc("Syntax: forge <weapon/armor> <type> <metal type>.\n\r", ch);
  return;
 }
 if( !str_cmp( arg3, "copper"))
  type = METAL_COPPER;
 else if( !str_cmp( arg3, "bronze"))
  type = METAL_BRONZE;
 else if( !str_cmp( arg3, "iron"))
  type = METAL_IRON;
 else if( !str_cmp( arg3, "steel"))
  type = METAL_STEEL;
 else
 {
  stc("That is not a valid metal type.\n\r", ch);
  return;
 }
 value = set_forge_value(arg2);

 if( value == 0)
 {
  stc("That is not a valid armor/weapon type.\n\r", ch);
  return;
 } 
 if( !str_cmp( arg1, "weapon"))
 {
  armor = FALSE;
  count = 3;
 }
 else if( !str_cmp( arg1, "armor"))
 {
  armor = TRUE;
  count = set_armor_count(value);
 }
 else
 {
  stc("Weapon or armor?\n\r", ch);
  return;
 }
 if( !has_metal(ch, count, type))
  return;
 remove_bars(ch, count, type);
 if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
 {
  log_string( "Error...missing object, please inform Kale.\n\r");
  return;
 }
 obj = create_object( pObjIndex, 25 );
 SET_BIT(obj->extra_flags, ITEM_FORGED);
 set_forge_val(obj, type, armor, value);
 sprintf(arg3, "You have created %s\n\r", obj->short_descr);
 stc(arg3, ch);       
 obj_to_char(obj, ch);
 return;
} 

void copy_object(OBJ_DATA *obj, OBJ_DATA *copy)
{
  free_string(copy->name);
  copy->name = strdup(obj->name);
  free_string(copy->short_descr);
  copy->short_descr = strdup(obj->short_descr);
  free_string(copy->description);
  copy->description = strdup(obj->description);
  copy->value[0] = obj->value[0];  
  copy->value[1] = obj->value[1];  
  copy->value[2] = obj->value[2];
  copy->value[3] = obj->value[3];
  copy->item_type = obj->item_type;
  return;
}
void do_bet(CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 
 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( ch->fighting != NULL)
 {
  stc("Not while your fighting.\n\r", ch);
  return;
 }
 if( arg[0] == '\0')
 {
  stc("Bet what?\n\r", ch);
  return;
 }
 if( (obj = get_obj_carry(ch, arg)) == NULL)
 {
  stc("You arent carrying that.\n\r", ch);
  return;
 }
 if( (obj->item_type != ITEM_GEM) && (obj->item_type != ITEM_ORE)
   && (obj->item_type != ITEM_METAL))
 {
  stc("You can only bet gems, ore and bars of metal.\n\r", ch);
  return;
 }
 sprintf(arg, "You bet %s.\n\r", obj->short_descr);
 stc(arg, ch);
 extract_obj(obj);
 return;
}

void do_hilo(CHAR_DATA *ch, char *argument)
{
 int dealer_card = number_range(1,15);
 int better_card = number_range(1,15);
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 bool win = FALSE;
 int bet = 0;

 one_argument(argument, arg);
 
 if( arg[0] == '\0')
 {
  stc("You must bet an item.\n\r", ch);
  return;
 }
 if( !is_number( arg))
 {
  stc("That is not a number.\n\r", ch);
  return;
 }
 bet = atoi(arg);
 if( (bet < 50) || (bet > 100))
 {
  stc("All bets must be between 50 and a 100 qps.\n\r", ch);
  return;
 } 
 if(ch->pcdata->quest < bet)
 {
  stc("You cant bet what you dont have.\n\r", ch);
  return;
 }
 while( dealer_card == better_card)
  dealer_card = number_range(1,15);
 sprintf( buf, "You drew a %d and the dealer drew a %d.\n\r",
  better_card, dealer_card);
 stc(buf, ch);
 if( dealer_card < better_card)
   win = TRUE;
 if( win)
 {
  stc("YOU WIN!!\n\r", ch);
  ch->pcdata->quest += bet;
  return;
 }
 else
 {
  stc("YOU LOSE!!\n\r", ch);
  ch->pcdata->quest -= bet;
  return;
 }
 return;
}

void do_smelt( CHAR_DATA *ch, char *argument)
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 int cop_count = 0;
 int tin_count = 0;
 int iron_count = 0;
 int i;
 OBJ_DATA *obj;
 OBJ_INDEX_DATA *pObjIndex;
 bool used = FALSE;

 static const char *metal_name[4] = {"copper", "bronze", "iron", "steel"}; 
 argument = one_argument( argument, arg1);
 argument = one_argument( argument, arg2);

 if( IS_NPC(ch))
  return;
 if( arg1[0] == '\0')
 {
  stc("Syntax: smelt <add/show/combine> <ore>.\n\r",ch);
  return;
 }
 if( !str_cmp(arg1, "combine"))
 {
  for(i = 0; i != 4; i++)
  {
   if( ch->smelt[i] == ORE_COPPER)
   {
    ch->smelt[i] = 0;
    cop_count++;
   }
   if( ch->smelt[i] == ORE_TIN)
   {
    ch->smelt[i] = 0;
    tin_count++;
   }
   if( ch->smelt[i] == ORE_IRON)
   {
    ch->smelt[i] = 0;
    iron_count++;
  }
   else
    continue;
  }
  if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
  {
   log_string( "Error...missing object, please inform Kale.\n\r");
   return;
  }
  obj = create_object( pObjIndex, 25 );
  obj->item_type = ITEM_METAL;
  if( (cop_count == 3) && (tin_count == 0) && (iron_count == 0))
  {
   sprintf(arg2, "metal bar %s", metal_name[0]);
   free_string(obj->name);
   obj->name = str_dup(arg2);
   sprintf( arg2, "A bar of %s", metal_name[0]);
   free_string(obj->short_descr);
   obj->short_descr = str_dup(arg2);
   sprintf(arg2, "A bar of %s lies forgotten in the dust.",
    metal_name[0]);
   free_string(obj->description);
   obj->description = str_dup(arg2);
   obj->value[0] = METAL_COPPER;
   sprintf(arg2,"You have made %s.\n\r", obj->short_descr);
   stc(arg2, ch);
   obj_to_char(obj, ch);
   return;
  }
  else if( (cop_count == 2) && (tin_count == 1) && (iron_count == 0))
  {
   sprintf(arg2, "metal bar %s", metal_name[1]);
   free_string(obj->name);
   obj->name = str_dup(arg2);
   sprintf( arg2, "A bar of %s", metal_name[1]);
   free_string(obj->short_descr);
   obj->short_descr = str_dup(arg2);
   sprintf(arg2, "A bar of %s lies forgotten in the dust.",
    metal_name[1]);
   free_string(obj->description);
   obj->description = str_dup(arg2);
   obj->value[0] = METAL_BRONZE;
   sprintf(arg2,"You have made %s.\n\r", obj->short_descr);
   stc(arg2, ch);
   obj_to_char(obj, ch);
   return;
  }
  else if( (cop_count == 0) && (tin_count == 0) && (iron_count == 3))
  {
   if( number_percent() > 25)
   {
   sprintf(arg2, "metal bar %s", metal_name[2]);
   free_string(obj->name);
   obj->name = str_dup(arg2);
   sprintf( arg2, "A bar of %s", metal_name[2]);
   free_string(obj->short_descr);
   obj->short_descr = str_dup(arg2);
   sprintf(arg2, "A bar of %s lies forgotten in the dust.",
    metal_name[2]);
   free_string(obj->description);
   obj->description = str_dup(arg2);
   obj->value[0] = METAL_IRON;
   }
   else
   {
   sprintf(arg2, "metal bar %s", metal_name[3]);
   free_string(obj->name);
   obj->name = str_dup(arg2);
   sprintf( arg2, "A bar of %s", metal_name[3]);
   free_string(obj->short_descr);
   obj->short_descr = str_dup(arg2);
   sprintf(arg2, "A bar of %s lies forgotten in the dust.",
    metal_name[3]);
   free_string(obj->description);
   obj->description = str_dup(arg2);
   obj->value[0] = METAL_STEEL;
   }
   sprintf(arg2,"You have made %s.\n\r", obj->short_descr);
   stc(arg2, ch); 
   obj_to_char(obj, ch);
   return;
  }
  return;
 }
 else if( !str_cmp(arg1, "add"))
 {
  if( arg2[0] == '\0')
  {
   stc("Add what?\n\r", ch);
   return;
  }
  if( (obj = get_obj_carry(ch, arg2)) == NULL)
  {
   stc("Your not carrying that item.\n\r", ch);
   return;
  }
  if( obj->item_type != ITEM_ORE)
  {
   stc("You can only smelt ore.\n\r", ch);
   return;
  }
  for(i =0; i !=4; i++)
  {
   if( ch->smelt[i] != 0)
    continue;
   if( used)
    continue;
   else
   {
    ch->smelt[i] = obj->value[0];
    used = TRUE;
   }
  }
  extract_obj(obj);
  stc("You have added some ore to your smelting bowl.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg1, "show"))
 {
  for( i = 0; i != 4; i++)
  {
   if( ch->smelt[i] == 0)
    continue;
   if( ch->smelt[i] == ORE_COPPER)
    stc("A lump of copper ore.\n\r", ch);
   if( ch->smelt[i] == ORE_TIN)
    stc("A lump of tin ore.\n\r", ch);
   if( ch->smelt[i] == ORE_IRON)
    stc("A lump of iron ore.\n\r", ch);
  }
  return;
 }
 else
 {
  stc("Not a valid option.\n\r", ch);
  return;
 }
 return;
}
void do_mine( CHAR_DATA *ch, char *argument)
{
 int chance = number_range(1,100);
 bool gem = FALSE;
 AFFECT_DATA af;
 OBJ_INDEX_DATA *pObjIndex;
 OBJ_DATA *obj;
 char buf[MAX_STRING_LENGTH];
 int sn;

 if( IS_NPC(ch))
  return;
 if( IS_AFFECTED(ch, AFF_SHADOWPLANE))
 {
  stc("Not while in shadowplane.\n\r", ch);
  return;
 }
 if( IS_SET(ch->in_room->affected_by, AFF_ROOM_MINED))
 {
  stc("This room has already been mined.\n\r",ch);
  return;
 }
 if( IS_AFFECTED(ch, AFF_ETHEREAL) )
 {
  if( ch->mana < 750 || ch->move < 750 )
  {
   stc("You lack the strength to mine from the ether!.\n\r", ch);
   return;
  }
  else
  {
   ch->mana -= 750;
   ch->move -= 750;
  }
 }
 if( ch->trust > 8)
  chance = 1;
 if( chance > 40)
 {
  stc("You mine the area, but find nothing but a peeved gopher.\n\r", ch);
 }
 else
 {
   if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )   
   {
    log_string( "Error...missing object, please inform Kale.\n\r"); 
    return;
   }
   obj = create_object( pObjIndex, 25 );
  if( number_range(1,100) < 30)
   gem = TRUE;
  if( gem )
   make_gem(obj);
  else
   make_ore(obj);
  sprintf(buf,"Your mining efforts have brought forth  %s.", obj->short_descr);
  stc(buf, ch);
  obj_to_char(obj,ch);
 }
 if( (sn = skill_lookup( "mined" )) < 0 )
   return;
 af.type = sn;
 af.bitvector = AFF_ROOM_MINED;
 af.modifier = 0;
 af.location = APPLY_NONE;
 af.duration = number_range(10, 24);
 affect_to_room(ch->in_room, &af);
 return;
}

void do_call( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim = NULL;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "What object do you wish to call?\n\r", ch );
	return;
    }

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if (!IS_HEAD(ch, LOST_HEAD))
    {
    	act("Your eyes flicker with yellow energy.",ch,NULL,NULL,TO_CHAR);
    	act("$n's eyes flicker with yellow energy.",ch,NULL,NULL,TO_ROOM);
    }

    if (!str_cmp(arg,"all"))
    {
	call_all(ch);
	return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    if (obj->questowner == NULL || strlen(obj->questowner) < 2 ||
	str_cmp(obj->questowner,ch->pcdata->switchname) || obj->item_type == ITEM_PAGE)
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if (obj->carried_by != NULL && obj->carried_by != ch)
    {
	victim = obj->carried_by;
	if (!IS_NPC(victim) && victim->desc != NULL && victim->desc->connected != CON_PLAYING) return;
    	act("$p suddenly vanishes from your hands!",victim,obj,NULL,TO_CHAR);
    	act("$p suddenly vanishes from $n's hands!",victim,obj,NULL,TO_ROOM);
	obj_from_char(obj);
    }
    else if (obj->in_room != NULL)
    {
    	chroom = ch->in_room;
    	objroom = obj->in_room;
    	char_from_room(ch);
    	char_to_room(ch,objroom);
    	act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
	if (chroom == objroom) act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
    	char_from_room(ch);
    	char_to_room(ch,chroom);
	obj_from_room(obj);
    }
    else if (obj->in_obj != NULL) obj_from_obj(obj);
    else
    {
	if (!IS_HEAD(ch, LOST_HEAD))
	    send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    obj_to_char(obj,ch);
    if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
	REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
    act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
    do_autosave(ch,"");
    if (victim != NULL) do_autosave(victim,"");
    return;
}

void call_all( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *victim = NULL;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;
    bool found = FALSE;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->questowner == NULL || strlen(obj->questowner) < 2 || 
	    str_cmp( ch->pcdata->switchname, obj->questowner ) || obj->item_type == ITEM_PAGE)
	    continue;
        if( ch->pcdata->familiar != NULL && ch->pcdata->familiar == obj )
         continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if (in_obj->carried_by != NULL)
	{
	    if (in_obj->carried_by == ch) continue;
	}

	if (obj->carried_by != NULL)
	{
	    if (obj->carried_by == ch || obj->carried_by->desc == NULL || 
		obj->carried_by->desc->connected != CON_PLAYING) 
	    {
		if (!IS_NPC(obj->carried_by)) return;
	    }
	    act("$p suddenly vanishes from your hands!",obj->carried_by,obj,NULL,TO_CHAR);
	    act("$p suddenly vanishes from $n's hands!",obj->carried_by,obj,NULL,TO_ROOM);
	    SET_BIT(obj->carried_by->extra, EXTRA_CALL_ALL);
	    obj_from_char(obj);
	}
	else if (obj->in_room != NULL)
	{
	    chroom = ch->in_room;
	    objroom = obj->in_room;
	    char_from_room(ch);
	    char_to_room(ch,objroom);
	    act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
	    if (chroom == objroom) act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
	    char_from_room(ch);
	    char_to_room(ch,chroom);
	    obj_from_room(obj);
	}
	else if (obj->in_obj != NULL) obj_from_obj(obj);
	else continue;
	obj_to_char(obj,ch);
	if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
	    REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
	if (!IS_HEAD(ch, LOST_HEAD))
	{
	    act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
	    act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
	}
    }

    if ( !found && !IS_HEAD(ch, LOST_HEAD) )
	send_to_char( "Nothing happens.\n\r", ch );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected != CON_PLAYING ) continue;
	if ( (victim = d->character) == NULL ) continue;
	if ( IS_NPC(victim) ) continue;
      if (ch->position == POS_FIGHTING) continue;
	if ( ch != victim && !IS_EXTRA(victim,EXTRA_CALL_ALL) ) continue;
	REMOVE_BIT(victim->extra, EXTRA_CALL_ALL);
	do_autosave(victim,"");
    }
    return;
}

void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    OBJ_DATA *obj2;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *objroom;
    bool move_ch = FALSE;

    /* Objects should only have a shadowplane flag when on the floor */
    if  ( IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
	obj->in_room != NULL &&
	(!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
    {
	send_to_char( "Your hand passes right through it!\n\r", ch );
	return;
    }
    if  (!IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
	obj->in_room != NULL &&
	( IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
    {
	send_to_char( "Your hand passes right through it!\n\r", ch );
	return;
    }
    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if ( ch->carry_number + 1 > can_carry_n( ch ) )
    {
	act( "$d: you can't carry that many items.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if ( container != NULL )
    {

    	if  ( IS_AFFECTED(ch,AFF_SHADOWPLANE) && !IS_SET(container->extra_flags, ITEM_SHADOWPLANE) &&  (container->carried_by == NULL || container->carried_by != ch) )
    	{
			send_to_char( "Your hand passes right through it!\n\r", ch );
			return;
    	}
    	if  (!IS_AFFECTED(ch,AFF_SHADOWPLANE) && IS_SET(container->extra_flags, ITEM_SHADOWPLANE) && (container->carried_by == NULL || container->carried_by != ch) )
    	{
		    send_to_char( "Your hand passes right through it!\n\r", ch );
		    return;
        }
		if ((obj->pIndexData->vnum >=  29500 && obj->pIndexData->vnum <= 29599) && ch->level < 3)
		{
			send_to_char("You cannot seem to get this object.\n\r", ch);
			return;
		}
		act( "You get $p from $P.", ch, obj, container, TO_CHAR );
		act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
		for ( obj2 = container->contains; obj2 != NULL; obj2 = obj_next )
		{
			obj_next = obj2->next_content;
			if ( obj2->chobj != NULL )
			{act( "A hand reaches inside $P and takes $p out.", obj2->chobj, obj, container, TO_CHAR );
			move_ch = TRUE;}
		}
		obj_from_obj( obj );
    }
    else
    {
		if ((obj->pIndexData->vnum >=  29500 && obj->pIndexData->vnum <= 29599) && ch->level < 3)
		{
			send_to_char("You cannot seem to pick this up\n\r", ch);
			return;
		}
		act( "You pick up $p.", ch, obj, container, TO_CHAR );
		act( "$n picks $p up.", ch, obj, container, TO_ROOM );
		if (obj != NULL)
			obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY )
		extract_obj( obj );
    else
    {
		obj_to_char( obj, ch );
		if ( move_ch && obj->chobj != NULL )
		{
			if (obj->carried_by != NULL && obj->carried_by != obj->chobj)
	    		objroom = get_room_index(obj->carried_by->in_room->vnum);
			else objroom = NULL;
			if (objroom != NULL && get_room_index(obj->chobj->in_room->vnum) != objroom)
			{
		    	char_from_room(obj->chobj);
	    		char_to_room(obj->chobj,objroom);
				do_look(obj->chobj,"auto");
			}
		}
    	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
			REMOVE_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
    }

    return;
}



void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int nest = 0;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_AFFECTED(ch,AFF_ETHEREAL) )
    {
	send_to_char( "You cannot pick things up while ethereal.\n\r", ch );
	return;
    }
    
        /* Get type. */
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Get what?\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL )
	    {
		act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
		return;
	    }

	    get_obj( ch, obj, NULL );
	}
	else
	{
	    /* 'get all' or 'get all.obj' */
	    found = FALSE;
	    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    get_obj( ch, obj, NULL );
		}
	    }

	    if ( !found ) 
	    {
		if ( arg1[3] == '\0' )
		    send_to_char( "I see nothing here.\n\r", ch );
		else
		    act( "I see no $T here.", ch, NULL, &arg1[4], TO_CHAR );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	    return;
	}

	switch ( container->item_type )
	{
	default:
	    send_to_char( "That's not a container.\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {
		char name[MAX_INPUT_LENGTH];
		char *pd;

		if ( IS_NPC(ch) )
		{
		    send_to_char( "You can't do that.\n\r", ch );
		    return;
		}

		pd = container->short_descr;
		pd = one_argument( pd, name );
		pd = one_argument( pd, name );
		pd = one_argument( pd, name );

	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	    return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj container' */
	    obj = get_obj_list( ch, arg1, container->contains );
	    if ( obj == NULL)
	    {
		act( "I see nothing like that in the $T.",
		    ch, NULL, arg2, TO_CHAR );
		return;
	    }
	    get_obj( ch, obj, container );
	}
	else
	{
	    /* 'get all container' or 'get all.obj container' */
	    found = FALSE;
	    for ( obj = container->contains; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
                if( nest >= 20)
                 continue;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    get_obj( ch, obj, container );
		}
                nest++;
               
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
		    act( "I see nothing in the $T.",
			ch, NULL, arg2, TO_CHAR );
		else
		    act( "I see nothing like that in the $T.",
			ch, NULL, arg2, TO_CHAR );
	    }
	}
    }
    do_autosave(ch,"");
    return;
}



void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj_next2;
    ROOM_INDEX_DATA *objroom = get_room_index(ROOM_VNUM_IN_OBJECT);
    int count = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Put what in what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	return;
    }

    if ( container->item_type != ITEM_CONTAINER )
    {
	send_to_char( "That's not a container.\n\r", ch );
	return;
    }

    if ( IS_SET(container->value[1], CONT_CLOSED) )
    {
	act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	return;
    }

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( obj == container )
	{
	    send_to_char( "You can't fold it into itself.\n\r", ch );
	    return;
	}

	if ( IS_SET( obj->quest, QUEST_ARTIFACT) )
	{
	    send_to_char("You cannot put artifacts in a container.\n\r",ch);
	    return;
	}
        if (obj->item_type == ITEM_CORPSE_NPC)
        {
            send_to_char("You cannot put corpses in a container.\n\r",ch);
            return;
        }
        if (obj->item_type == ITEM_CORPSE_PC)
        {
            send_to_char("You cannot put corpses in a container.\n\r",ch);
            return;
        }
	if (obj->points > 500)
	{
		send_to_char("You may not put such a precious item in a container.\n\r", ch);
		return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

	if ( get_obj_weight( obj ) + get_obj_weight( container )
	     > container->value[0] )
	{
	    send_to_char( "It won't fit.\n\r", ch );
	    return;
	}

	for ( obj2 = container->contains; obj2 != NULL; obj2 = obj_next2 )
	{
	    obj_next2 = obj2->next_content;
	    if ( obj2->chobj != NULL && obj != obj2)
		act( "A hand reaches inside $P and drops $p.", obj2->chobj, obj, container, TO_CHAR );
	}
	obj_from_char( obj );
	obj_to_obj( obj, container );
	act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
	act( "You put $p in $P.", ch, obj, container, TO_CHAR );
    }
    else
    {
	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

            if( count >= 10)
              continue;
	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == WEAR_NONE
	    &&   obj != container
	    &&   !IS_SET( obj->quest, QUEST_ARTIFACT)
	    &&   can_drop_obj( ch, obj )
	    &&   get_obj_weight( obj ) + get_obj_weight( container )
		 <= container->value[0] )
	    {
		for ( obj2 = container->contains; obj2 != NULL; obj2 = obj_next2 )
		{
		    obj_next2 = obj2->next_content;
		    if ( obj2->chobj != NULL && obj2->chobj->in_room != NULL)
		    {
			if (objroom != get_room_index(obj2->chobj->in_room->vnum))
			{
		    	    char_from_room(obj2->chobj);
		    	    char_to_room(obj2->chobj,objroom);
			    do_look(obj2->chobj,"auto");
			}
			if (obj != obj2)
			    act( "A hand reaches inside $P and drops $p.", obj2->chobj, obj, container, TO_CHAR );
		    }
		}
		obj_from_char( obj );
		obj_to_obj( obj, container );
		act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
		act( "You put $p in $P.", ch, obj, container, TO_CHAR );
                count++;
	    }
	}
    }
    do_autosave(ch,"");
    return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Drop what?\n\r", ch );
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}
      if ( IS_SET( obj->quest, QUEST_ARTIFACT))
      {
          send_to_char( "You cannot drop an artifact.\n\r", ch);
          return;
      } 
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	/* Objects should only have a shadowplane flag when on the floor */
	if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
	    (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
	    SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
	act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
	act( "You drop $p.", ch, obj, NULL, TO_CHAR );
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == WEAR_NONE
	    &&   can_drop_obj( ch, obj ) 
          &&   !IS_SET( obj->quest, QUEST_ARTIFACT)) 
	    {
		found = TRUE;
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
		/* Objects should only have a shadowplane flag when on the floor */
		if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
		    (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
		    SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
		act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
		act( "You drop $p.", ch, obj, NULL, TO_CHAR );
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "You are not carrying anything.",
		    ch, NULL, arg, TO_CHAR );
	    else
		act( "You are not carrying any $T.",
		    ch, NULL, &arg[4], TO_CHAR );
	}
    }

    do_autosave(ch,"");
    return;
}



void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Give what to whom?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
	send_to_char( "You must remove it first.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if (IS_AFFECTED(victim,AFF_ETHEREAL) )
    {
	send_to_char( "You cannot give things to ethereal people.\n\r", ch );
	return;
    }

    if ( victim->carry_number + 1 > can_carry_n( victim ) )
    {
	act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( victim->carry_weight + get_obj_weight( obj ) > can_carry_w( victim ) )
    {
	act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
	act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
	return;
    }
	if (obj->pIndexData->vnum >=  29500 && obj->pIndexData->vnum <= 29599 && ch->level < 12)
	{
		send_to_char("You cannot seem to do that.\n\r", ch);
		return;
	}
    obj_from_char( obj );
    obj_to_char( obj, victim );
    act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
    act( "$n gives you $p.",   ch, obj, victim, TO_VICT    );
    act( "You give $p to $N.", ch, obj, victim, TO_CHAR    );
    return;
}




void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
	fountain = fountain->next_content )
    {
	if ( fountain->item_type == ITEM_FOUNTAIN )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char( "There is no fountain here!\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
	fountain->in_room != NULL &&
	!IS_SET(fountain->extra_flags, ITEM_SHADOWPLANE) )
    {
	send_to_char( "You are too insubstantual.\n\r", ch );
	return;
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
	fountain->in_room != NULL &&
	IS_SET(fountain->extra_flags, ITEM_SHADOWPLANE) )
    {
	send_to_char( "It is too insubstantual.\n\r", ch );
	return;
    }
    else if (IS_AFFECTED(ch,AFF_ETHEREAL) )
    {
	send_to_char( "You cannot fill containers while ethereal.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "You can't fill that.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( "Your container is already full.\n\r", ch );
	return;
    }

    if ( (obj->value[2] != fountain->value[2]) && obj->value[1] > 0)
    {
	send_to_char( "You cannot mix two different liquids.\n\r", ch );
	return;
    }

    act( "$n dips $p into $P.", ch, obj, fountain, TO_ROOM );
    act( "You dip $p into $P.", ch, obj, fountain, TO_CHAR );
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];
    liquid = obj->value[2];
    act( "$n fills $p with $T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
    act( "You fill $p with $T.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );
    return;
}



void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int amount;
    int liquid;

    one_argument( argument, arg );


    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_FOUNTAIN )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( "Drink what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
    {
	send_to_char( "You fail to reach your mouth.  *Hic*\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't drink from that.\n\r", ch );
	break;

    case ITEM_POTION:
	do_quaff(ch,obj->name);
	return;
    case ITEM_FOUNTAIN:
	if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

	if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
		obj->in_room != NULL &&
		!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
	{
	    send_to_char( "You are too insubstantual.\n\r", ch );
	    break;
	}
	else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
		obj->in_room != NULL &&
		IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
	{
	    send_to_char( "It is too insubstantual.\n\r", ch );
	    break;
	}
    	else if (IS_AFFECTED(ch,AFF_ETHEREAL) )
    	{
	    send_to_char( "You can only drink from things you are carrying while ethereal.\n\r", ch );
	    return;
    	}
	act( "$n drinks $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
	act( "You drink $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );

	amount = number_range(3, 10);
	amount = UMIN(amount, obj->value[1]);
	
	gain_condition( ch, COND_DRUNK,
	    amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
	gain_condition( ch, COND_FULL,
	    amount * liq_table[liquid].liq_affect[COND_FULL   ] );
	gain_condition( ch, COND_THIRST,
	    amount * liq_table[liquid].liq_affect[COND_THIRST ] );
        if( IS_CLASS(ch,CLASS_VAMPIRE) )
        {
         if( !str_cmp(liq_table[liquid].liq_name,"blood") )
          GET_POWERS(ch,VAMP_BLOOD) += number_range(5,15);
         if( GET_POWERS(ch,VAMP_BLOOD) >= 100 )
          GET_POWERS(ch,VAMP_BLOOD) = 100;
        }
	if ( obj->value[3] != 0 && !IS_NPC(ch))
	{
	    /* The shit was poisoned ! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );
	    af.type      = gsn_poison;
	    af.duration  = 3 * amount;
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

	act( "$n drinks $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
	act( "You drink $T from $p.",
	    ch, obj, liq_table[liquid].liq_name, TO_CHAR );

	amount = number_range(3, 10);
	amount = UMIN(amount, obj->value[1]);
	
	gain_condition( ch, COND_DRUNK,
	    amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
	gain_condition( ch, COND_FULL,
	    amount * liq_table[liquid].liq_affect[COND_FULL   ] );
	gain_condition( ch, COND_THIRST,
	    amount * liq_table[liquid].liq_affect[COND_THIRST ] );

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	    send_to_char( "You feel drunk.\n\r", ch );
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   > 50 )
	    send_to_char( "You are full.\n\r", ch );
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 50 )
	    send_to_char( "You do not feel thirsty.\n\r", ch );
	
	if ( obj->value[3] != 0 && !IS_NPC(ch))
	{
	    /* The shit was poisoned ! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );
	    af.type      = gsn_poison;
	    af.duration  = 3 * amount;
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	
	obj->value[1] -= amount;
	if ( obj->value[1] <= 0 )
	{
		obj->value[1] = 0;
	}
	break;
    }

    return;
}


void do_empty( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Empty what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( "You cannot empty that.\n\r", ch );
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

	act( "$n empties $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
	act( "You empty $T from $p.",
	    ch, obj, liq_table[liquid].liq_name, TO_CHAR );
	
	obj->value[1] = 0;
	break;
    }

    return;
}



void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int level, i, x;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Eat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }
  
    if ( !IS_IMMORTAL(ch) )
    {   
        if (IS_NPC(ch))
        return;
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL &&
	     obj->item_type != ITEM_EGG && obj->item_type != ITEM_QUEST )
	{
	    if (IS_NPC(ch) || obj->item_type != ITEM_TRASH)
	    {
		send_to_char( "That's not edible.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 50 &&
	    obj->item_type != ITEM_TRASH && obj->item_type != ITEM_QUEST &&
	    obj->item_type != ITEM_PILL)
	{
	    send_to_char( "You are too full to eat more.\n\r", ch );
	    return;
	}
    }

    act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
    act( "You eat $p.", ch, obj, NULL, TO_CHAR );

    switch ( obj->item_type )
    {
    default:
	break;

    case ITEM_FOOD:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_FULL];
	    gain_condition( ch, COND_FULL, obj->value[0] );
	    if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 10 )
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] > 50 )
		send_to_char( "You are full.\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The shit was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );

	    af.type      = gsn_poison;
	    af.duration  = 2 * obj->value[0];
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;

    case ITEM_PILL:
	level = obj->value[0];
	if (level < 1) level = 1;
	if (level > MAX_SPELL) level = MAX_SPELL;
    if( obj->carried_by != NULL && has_power_level(obj->carried_by,SCHOOL_ARCANE,4) )
     level *= 4;

/**************** Code for multi-cast pills ***************/
	if (level < 25)
	    x = 1;
     	if (level >= 25 && level < 50)
	    x = 2;
	if (level >= 50 && level < 75)
	    x = 3;
	if (level >= 75)
	    x = 4;

	for (i = 0; i < x; i++)
	{
	obj_cast_spell( obj->value[1], level, ch, ch, NULL );
	}	

/***************** End multi-cast loop ****************/

	if (ch->position == POS_FIGHTING) 
	{
	    if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch, 6 );
	}
	break;

    case ITEM_QUEST:
	if ( !IS_NPC(ch) ) ch->pcdata->quest += obj->value[0];
	break;

    case ITEM_EGG:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_FULL];
	    gain_condition( ch, COND_FULL, obj->value[1] );
	    if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 10 )
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] > 50 )
		send_to_char( "You are full.\n\r", ch );
	}

	/* Note to myself...remember to set v2 for mobiles that hatch within
	 * the player (like aliens ;).  KaVir.
	 */

	if ( obj->value[3] != 0 )
	{
	    /* The shit was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );

	    af.type      = gsn_poison;
	    af.duration  = 2 * obj->value[0];
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;
    }

    if (obj != NULL) extract_obj( obj );
    return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
	act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    unequip_char( ch, obj );
    act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p.", ch, obj, NULL, TO_CHAR );
    return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    bool wolf_ok = FALSE;


    if( obj == NULL)
    {
     log_string("BUG: Wear_obj: Null object pointer");
     return;
    }
    if ( CAN_WEAR( obj, ITEM_WIELD ) || CAN_WEAR( obj, ITEM_HOLD ) ||
	CAN_WEAR( obj, ITEM_WEAR_SHIELD ) || obj->item_type == ITEM_LIGHT )
    {
	if ( get_eq_char( ch, WEAR_WIELD    ) != NULL
	&&   get_eq_char( ch, WEAR_HOLD     ) != NULL
	&&   get_eq_char( ch, WEAR_LIGHT    ) != NULL
	&&   get_eq_char( ch, WEAR_SHIELD   ) != NULL
	&&   !remove_obj( ch, WEAR_LIGHT , fReplace )
	&&   !remove_obj( ch, WEAR_SHIELD, fReplace )
	&&   !remove_obj( ch, WEAR_WIELD , fReplace )
	&&   !remove_obj( ch, WEAR_HOLD  , fReplace ) )
	    return;
	if ( !IS_NPC(ch) &&  IS_OBJ_STAT(obj,ITEM_LOYAL) )
	{
	 if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
	 {
          stc("You are unable to use an item that has been claimed and is loyal to another.\n\r", ch);
          return;
         }
	}
	if (get_eq_char( ch, WEAR_WIELD ) == NULL && is_ok_to_wear(ch,wolf_ok,"right_hand"))
	{
	    if ( obj->item_type == ITEM_LIGHT )
	    {
	    	act( "$n lights $p and clutches it in $s right hand.", ch, obj, NULL, TO_ROOM );
	    	act( "You light $p and clutch it in your right hand.",  ch, obj, NULL, TO_CHAR );
	    }
	    else
	    {
	    	act( "$n clutches $p in $s right hand.",    ch, obj, NULL, TO_ROOM );
	    	act( "You clutch $p in your right hand.",  ch, obj, NULL, TO_CHAR );
	    }	
	    if (obj->item_type == ITEM_WEAPON)
	    {
		equip_char( ch, obj, WEAR_WIELD );
		if (!IS_NPC(ch)) do_skill(ch, ch->name);
		return;
  		
	    }
	    equip_char( ch, obj, WEAR_WIELD );
	    return;
	}
	else if (get_eq_char( ch, WEAR_HOLD ) == NULL && is_ok_to_wear(ch,wolf_ok,"left_hand"))
	{
	    if ( obj->item_type == ITEM_LIGHT )
	    {
	    	act( "$n lights $p and clutches it in $s left hand.", ch, obj, NULL, TO_ROOM );
	    	act( "You light $p and clutch it in your left hand.",  ch, obj, NULL, TO_CHAR );
	    }
	    else
	    {
	    	act( "$n clutches $p in $s left hand.",    ch, obj, NULL, TO_ROOM );
	    	act( "You clutch $p in your left hand.",  ch, obj, NULL, TO_CHAR );
	    }
	    if (obj->item_type == ITEM_WEAPON)
	    {
		if ( !IS_NPC(ch) && (obj->pIndexData->vnum == 30000 || IS_OBJ_STAT(obj,ITEM_LOYAL)))
		{
		    if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
		    {
			act( "$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM );
			act( "$p leaps out of your hand.", ch, obj, NULL, TO_CHAR );
			obj_from_char(obj);
			obj_to_room(obj,ch->in_room);
			return;
		    }
		} 
		equip_char( ch, obj, WEAR_HOLD );
		if (!IS_NPC(ch)) do_skill(ch, ch->name);
		return;
	    }
	    equip_char( ch, obj, WEAR_HOLD );
	    return;
	}
        else if (get_eq_char( ch, WEAR_THIRD ) == NULL && is_ok_to_wear(ch,wolf_ok,"third_hand"))
	{
	    if ( obj->item_type == ITEM_LIGHT )
	    {
	    	act( "$n lights $p and clutches it in $s right hand.", ch, obj, NULL, TO_ROOM );
	    	act( "You light $p and clutch it in your right hand.", ch, obj, NULL, TO_CHAR );
	    }
	    else
	    {
	    	act( "$n clutches $p in $s third hand.",    ch, obj, NULL, TO_ROOM );
	    	act( "You clutch $p in your third hand.",  ch, obj, NULL, TO_CHAR );
	    }	
	    if (obj->item_type == ITEM_WEAPON)
	    {
		equip_char( ch, obj, WEAR_THIRD );
		if (!IS_NPC(ch)) do_skill(ch, ch->name);
		return;
	    }
	    equip_char( ch, obj, WEAR_THIRD );
	    return;
	}
	else if (get_eq_char( ch, WEAR_FOURTH ) == NULL && is_ok_to_wear(ch,wolf_ok,"fourth_hand"))
	{
	    if ( obj->item_type == ITEM_LIGHT )
	    {
	    	act( "$n lights $p and clutches it in $s fourth hand.", ch, obj, NULL, TO_ROOM );
	    	act( "You light $p and clutch it in your fourth hand.", ch, obj, NULL, TO_CHAR );
	    }
	    else
	    {
	    	act( "$n clutches $p in $s fourth hand.",    ch, obj, NULL, TO_ROOM );
	    	act( "You clutch $p in your fourth hand.",  ch, obj, NULL, TO_CHAR );
	    }
	    if (obj->item_type == ITEM_WEAPON)
	    {
		if ( !IS_NPC(ch) && ( obj->pIndexData->vnum == 30000 || IS_OBJ_STAT(obj,ITEM_LOYAL)))
		{
		    if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
		    {
			act( "$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM );
			act( "$p leaps out of your hand.", ch, obj, NULL, TO_CHAR );
			obj_from_char(obj);
			obj_to_room(obj,ch->in_room);
			return;
		    }
		}
		equip_char( ch, obj, WEAR_FOURTH );
		if (!IS_NPC(ch)) do_skill(ch, ch->name);
		return;
	    }
	    equip_char( ch, obj, WEAR_FOURTH );
	    return;
	}
	if (!is_ok_to_wear(ch,wolf_ok,"left_hand") && !is_ok_to_wear(ch,wolf_ok,"right_hand"))
	    send_to_char( "You cannot use anything in your hands.\n\r", ch );
	else
	    send_to_char( "You have no free hands.\n\r", ch );
	return;
    }

    if ( obj->item_type == ITEM_LIGHT )
    {
	if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
	    return;
	act( "$n lights $p and holds it.", ch, obj, NULL, TO_ROOM );
	act( "You light $p and hold it.",  ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LIGHT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
	if ( get_eq_char( ch, WEAR_FINGER_L ) != NULL
	&&   get_eq_char( ch, WEAR_FINGER_R ) != NULL
	&&   !remove_obj( ch, WEAR_FINGER_L, fReplace )
	&&   !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_FINGER_L ) == NULL && is_ok_to_wear(ch,wolf_ok,"left_finger"))
	{
	    act( "$n wears $p on $s left finger.",    ch, obj, NULL, TO_ROOM );
	    act( "You wear $p on your left finger.",  ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_L );
	    return;
	}
	else if ( get_eq_char( ch, WEAR_FINGER_R ) == NULL && is_ok_to_wear(ch,wolf_ok,"right_finger"))
	{
	    act( "$n wears $p on $s right finger.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p on your right finger.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_R );
	    return;
	}
	if (!is_ok_to_wear(ch,wolf_ok,"left_finger") && !is_ok_to_wear(ch,wolf_ok,"right_finger"))
	    send_to_char( "You cannot wear any rings.\n\r", ch );
	else
	    send_to_char( "You cannot wear any more rings.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
	if ( get_eq_char( ch, WEAR_NECK_1 ) != NULL
	&&   get_eq_char( ch, WEAR_NECK_2 ) != NULL
	&&   !remove_obj( ch, WEAR_NECK_1, fReplace )
	&&   !remove_obj( ch, WEAR_NECK_2, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
	{
	    act( "$n slips $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You slip $p around your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_1 );
	    return;
	}

	if ( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
	{
	    act( "$n slips $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You slip $p around your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_2 );
	    return;
	}
	bug( "Wear_obj: no free neck.", 0 );
	send_to_char( "You are already wearing two things around your neck.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
	if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
	    return;

	act( "$n fits $p on $s body.",   ch, obj, NULL, TO_ROOM );
	act( "You fit $p on your body.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
	if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
	    return;
	if (!is_ok_to_wear(ch,wolf_ok,"head"))
	{
	    send_to_char("You have no head to wear it on.\n\r",ch);
	    return;
	}
	act( "$n places $p on $s head.",   ch, obj, NULL, TO_ROOM );
	act( "You place $p on your head.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FACE ) )
    {
	if ( !remove_obj( ch, WEAR_FACE, fReplace ) )
	    return;
	if (!is_ok_to_wear(ch,wolf_ok,"face"))
	{
	    send_to_char("You have no face to wear it on.\n\r",ch);
	    return;
	}
	act( "$n places $p on $s face.",   ch, obj, NULL, TO_ROOM );
	act( "You place $p on your face.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FACE );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
	    return;
	if (!is_ok_to_wear(ch,wolf_ok,"legs"))
	{
	    send_to_char("You have no legs to wear them on.\n\r",ch);
	    return;
	}
	act( "$n slips $s legs into $p.",   ch, obj, NULL, TO_ROOM );
	act( "You slip your legs into $p.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {
	if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
	    return;
	if (!is_ok_to_wear(ch,wolf_ok,"feet"))
	{
	    send_to_char("You have no feet to wear them on.\n\r",ch);
	    return;
	}
	act( "$n slips $s feet into $p.",   ch, obj, NULL, TO_ROOM );
	act( "You slip your feet into $p.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
	    return;
	if (!is_ok_to_wear(ch,wolf_ok,"hands"))
	{
	    send_to_char("You have no hands to wear them on.\n\r",ch);
	    return;
	}
	act( "$n pulls $p onto $s hands.",   ch, obj, NULL, TO_ROOM );
	act( "You pull $p onto your hands.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
	    return;
	if (!is_ok_to_wear(ch,wolf_ok,"arms"))
	{
	    send_to_char("You have no arms to wear them on.\n\r",ch);
	    return;
	}
	act( "$n slides $s arms into $p.",   ch, obj, NULL, TO_ROOM );
	act( "You slide your arms into $p.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
	    return;
	act( "$n pulls $p about $s body.",   ch, obj, NULL, TO_ROOM );
	act( "You pull $p about your body.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
	    return;
	act( "$n ties $p around $s waist.",   ch, obj, NULL, TO_ROOM );
	act( "You tie $p around your waist.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WAIST );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
	if ( get_eq_char( ch, WEAR_WRIST_L ) != NULL
	&&   get_eq_char( ch, WEAR_WRIST_R ) != NULL
	&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
	&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_WRIST_L ) == NULL && is_ok_to_wear(ch,wolf_ok,"right_wrist"))
	{
	    act( "$n slides $s left wrist into $p.",
		ch, obj, NULL, TO_ROOM );
	    act( "You slide your left wrist into $p.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_L );
	    return;
	}
	else if ( get_eq_char( ch, WEAR_WRIST_R ) == NULL && is_ok_to_wear(ch,wolf_ok,"left_wrist"))
	{
	    act( "$n slides $s left wrist into $p.",
		ch, obj, NULL, TO_ROOM );
	    act( "You slide your right wrist into $p.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_R );
	    return;
	}
	if (!is_ok_to_wear(ch,wolf_ok,"left_wrist") && !is_ok_to_wear(ch,wolf_ok,"right_wrist"))
	    send_to_char( "You cannot wear anything on your wrists.\n\r", ch );
	else
	    send_to_char( "You cannot wear any more on your wrists.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
    {
	if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
	    return;
	act( "$n straps $p onto $s shield arm.", ch, obj, NULL, TO_ROOM );
	act( "You strap $p onto your shield arm.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_SHIELD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WIELD ) )
    {
	if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
	    return;

	if ( get_obj_weight( obj ) > str_app[get_curr_str(ch)].wield )
	{
	    send_to_char( "It is too heavy for you to wield.\n\r", ch );
	    return;
	}

	act( "$n wields $p.", ch, obj, NULL, TO_ROOM );
	act( "You wield $p.", ch, obj, NULL, TO_CHAR );

	if ( !IS_NPC(ch) && ( obj->pIndexData->vnum == 30000 || IS_OBJ_STAT(obj,ITEM_LOYAL)))
	{
	    if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
	    {
		act( "$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM );
		act( "$p leaps out of your hand.", ch, obj, NULL, TO_CHAR );
		obj_from_char(obj);
		obj_to_room(obj,ch->in_room);
		return;
	    }
	}
	equip_char( ch, obj, WEAR_WIELD );
	if (!IS_NPC(ch)) do_skill(ch, ch->name);
	return;
    }

    if ( fReplace )
	send_to_char( "You can't wear, wield or hold that.\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;


    one_argument( argument, arg );

     if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_NPC(ch))
    {
	send_to_char( "You cannot wear anything in this form.\n\r", ch );
	return;
    }
    if( !IS_NPC(ch) && ch->pcdata->pulse_timer[PULSE_HEATMETAL] > 0)
    {
     stc("It's MUCH to hot for you to wear that now!\n\r", ch);
     return;
    }
    if( !IS_NPC(ch) 
     && IS_CLASS(ch, CLASS_DRAGON)
     && GET_POWERS(ch, DRAGON_FORM) == DRAGON_FORM_DRAGON)
    {
     stc("You can't wear anything in dragon form.\n\r", ch);
     return;
    }
    if ( arg[0] == '\0' )
    {
	send_to_char( "Wear, wield, or hold what?\n\r", ch );
	return;
    }
    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if( wearing_one_ring(ch) )
	    {
		stc("You cannot wear anything else whilst under the power of #oThe One Ring#n!\n\r", ch);
		return;
	    }
            if( !IS_NPC(ch) && IS_OBJ_STAT(obj, ITEM_LOYAL) && str_cmp(obj->questowner,ch->pcdata->switchname) )
	    {
	     stc("You can not wear an object that is loyal to another.\n\r",ch);
	     continue;
	    }   

	    if( !IS_NPC(ch) && IS_OBJ_STAT(obj, ITEM_ONE_RING) && str_cmp(obj->questowner,ch->pcdata->switchname) )
	    {
	     stc("#oThe One Ring #nchooses it's wearer.. and you are not it's owner!\n\r", ch);
	     continue;
	    }
	    if( IS_OBJ_STAT(obj, ITEM_ONE_RING) && !is_naked(ch) )
	    {
	     stc("#oThe One Ring #ncan only be worn alone.\n\r", ch);
	     continue;
	    }
	    if( IS_OBJ_STAT(obj,ITEM_ONE_RING) && ((ch->level < 3) || (ch->race < 5)) )
	    {
	     stc("#oYou lack the willpower to safely use the ring!#n\n\r",ch);
	     return;
	    }
	    if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
		wear_obj( ch, obj, FALSE );
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}
        if( !IS_NPC(ch) && IS_OBJ_STAT(obj, ITEM_LOYAL) && str_cmp(obj->questowner,ch->pcdata->switchname) )
	{
	 stc("You can not wear an object that is loyal to another.\n\r",ch);
         return;
	}
	if( !IS_NPC(ch) && IS_OBJ_STAT(obj, ITEM_ONE_RING) && str_cmp(obj->questowner,ch->pcdata->switchname) )
	{
	 stc("#oThe One Ring #nchooses it's wearer.. and you are not it's owner!\n\r", ch);
	 return;
	}
	if( IS_OBJ_STAT(obj, ITEM_ONE_RING) )
	{
	  if( is_naked(ch) )
	  {
	     wear_obj( ch, obj, TRUE );
	     return;
	  }
	  else
	   {
	     stc("#oThe One Ring #ncan only be worn alone.\n\r", ch);
	     return;
	   }
	}
	if( !wearing_one_ring(ch) )
	 wear_obj( ch, obj, TRUE );
	else
	{
	 stc("#oThe One Ring #ncan only be worn alone.\n\r", ch);
	 return;
	}
    }

    return;
}



void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ) )
		remove_obj( ch, obj->wear_loc, TRUE );
	}
	return;
    }
    if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }
    remove_obj( ch, obj->wear_loc, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    char buf[MAX_INPUT_LENGTH];
    int expgain;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char("Sacrifice what?\n\r",ch);
	return;
    }

    if( !str_cmp(arg,"all") )
    {
     OBJ_DATA *obj_next;
     int i = 0;
     for( obj = ch->in_room->contents;obj != NULL; obj = obj_next)
     {
      obj_next = obj->next_content;
      i++;
      if( i <= 15 && obj !=NULL )
       do_sacrifice(ch,obj->name);
      else
       return;
     }
    }

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }

    if ( !CAN_WEAR(obj, ITEM_TAKE) || obj->item_type == ITEM_QUEST ||
	 obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE ||
	 obj->item_type == ITEM_QUESTCARD || IS_SET(obj->quest, QUEST_ARTIFACT) ||
	( obj->questowner != NULL && strlen(obj->questowner) > 1 && str_cmp(ch->name,obj->questowner)))
    {
	act( "You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR );
	return;
    }
    else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
	obj->chobj->pcdata->obj_vnum != 0)
    {
	act( "You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR );
	return;
    }
    expgain = obj->cost / 100;
    if (expgain < 100)
	expgain = 100;
    if (expgain > 5000)
	expgain = 5000;
    act( "$p disintegrates into a fine powder.", ch, obj, NULL, TO_CHAR );
    act( "$n drains the energy from $p.", ch, obj, NULL, TO_ROOM );
    act( "$p disintegrates into a fine powder.", ch, obj, NULL, TO_ROOM );
    sprintf(buf, "You gain %d experience from $p.", expgain);
    ch->exp += expgain;
    act(buf, ch, obj, NULL, TO_CHAR);
    if (obj->points > 0 && !IS_NPC(ch) && obj->item_type != ITEM_PAGE)
    {
	sprintf( buf, "You receive a refund of %d quest points from $p.", (obj->points/3));
	act( buf, ch, obj, NULL, TO_CHAR );
	ch->pcdata->quest += (obj->points/3);
    }
    extract_obj( obj );
    return;
}



void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int level;

    one_argument( argument, arg );

	if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Quaff what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that potion.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "You can quaff only potions.\n\r", ch );
	return;
    }
	if (IS_NPC(ch)) return;

    act( "$n quaffs $p.", ch, obj, NULL, TO_ROOM );
    act( "You quaff $p.", ch, obj, NULL ,TO_CHAR );

    level = obj->value[0];
    if (level < 1) level = 1;
    if (level > MAX_SPELL) level = MAX_SPELL;
    if( obj->carried_by != NULL && has_power_level(obj->carried_by,SCHOOL_ARCANE,4) )
     level *= 4;

    obj_cast_spell( obj->value[1], level, ch, ch, NULL );
    obj_cast_spell( obj->value[2], level, ch, ch, NULL );
    obj_cast_spell( obj->value[3], level, ch, ch, NULL );

    extract_obj( obj );
    if (ch->position == POS_FIGHTING) 
    {
	if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch, 6 );
    }
    return;
}



void  do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;
    int level, i, x;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if (IS_NPC(ch)) return;

    if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
    {
		send_to_char( "You do not have that scroll.\n\r", ch );
		return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
		send_to_char( "You can recite only scrolls.\n\r", ch );
		return;
    }
	
    obj = NULL;
    if ( arg2[0] == '\0' )
    {
		victim = ch;
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL &&  ( obj = get_obj_here  ( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }
	if (victim != NULL)
	{
		if ( is_safe( ch, victim ) )
			return;
	}
	if (IS_NPC(ch)) return;

    act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
    act( "You recite $p.", ch, scroll, NULL, TO_CHAR );

    level = scroll->value[0];
    if (level < 1) level = 1;
    if (level > MAX_SPELL) level = MAX_SPELL;
    if( scroll->carried_by != NULL && has_power_level(scroll->carried_by,SCHOOL_ARCANE,4) )
     level *= 4;
    
/**************** Code for multi-cast scrolls ***********/
    if (level < 25)
	x = 1;
    if (level >= 25 && level < 50)
	x = 2;
    if (level >= 50 && level < 75)
	x = 3;
    if (level >= 75)
	x = 4;
    if( x > 3 ) x = 3;
    for (i = 0; i < x; i++)
    {
    	obj_cast_spell( scroll->value[1], level, ch, victim, obj );
    }

/**************** End code for multi-cast scrolls ************/


    extract_obj( scroll );
    if (ch->position == POS_FIGHTING) 
    {
    	if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch, 12 );
    }
    return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *temp;
    OBJ_DATA *staff;
    int sn;
    int level;

    staff = get_eq_char( ch, WEAR_WIELD );
    temp = get_eq_char( ch, WEAR_HOLD );

	if (IS_NPC(ch)) return;

    if ( staff == NULL && temp == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( staff == NULL ) staff = temp;
    if ( temp == NULL  ) temp = staff;

    if ( staff->item_type != ITEM_STAFF ) staff=temp;

    if ( staff->item_type != ITEM_STAFF )
    {
	send_to_char( "You can brandish only with a staff.\n\r", ch );
	return;
    }

    if ( ( sn = staff->value[3] ) < 0
    ||   sn >= MAX_SKILL
    ||   skill_table[sn].spell_fun == 0 )
    {
	bug( "Do_brandish: bad sn %d.", sn );
	return;
    }
	if (IS_NPC(ch)) return;

    if (!IS_IMMORTAL( ch))
	WAIT_STATE( ch, PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    {
	act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
	act( "You brandish $p.",  ch, staff, NULL, TO_CHAR );
	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next	= vch->next_in_room;

	    switch ( skill_table[sn].target )
	    {
	    default:
		bug( "Do_brandish: bad target for sn %d.", sn );
		return;

	    case TAR_IGNORE:
		if ( vch != ch )
		    continue;
		break;

	    case TAR_CHAR_OFFENSIVE:
		if ( IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch) )
		    continue;
		break;
		
	    case TAR_CHAR_DEFENSIVE:
		if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
		    continue;
		break;

	    case TAR_CHAR_SELF:
		if ( vch != ch )
		    continue;
		break;
	    }

	    level = staff->value[0];
	    if (level < 1) level = 1;
	    if (level > MAX_SPELL) level = MAX_SPELL;
    if( staff->carried_by != NULL && has_power_level(staff->carried_by,SCHOOL_ARCANE,4) )
     level *= 4;

	    obj_cast_spell( staff->value[3], level, ch, vch, NULL );
	}
    }

    if ( --staff->value[2] <= 0 )
    {
	act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
	act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *temp;
    OBJ_DATA *wand;
    OBJ_DATA *obj;
    int level;

    one_argument( argument, arg );

	if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Zap whom or what?\n\r", ch );
	return;
    }

    wand = get_eq_char( ch, WEAR_WIELD );
    temp = get_eq_char( ch, WEAR_HOLD );

    if ( wand == NULL && temp == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( wand == NULL ) wand = temp;
    if ( temp == NULL ) temp = wand;

    if ( wand->item_type != ITEM_WAND ) wand=temp;

    if ( wand->item_type != ITEM_WAND )
    {
	send_to_char( "You can zap only with a wand.\n\r", ch );
	return;
    }
	if (IS_NPC(ch)) return;

    obj = NULL;
    if ( arg[0] == '\0' )
    {
		if ( ch->fighting != NULL )
		{
			victim = ch->fighting;
		}
		else
		{
			send_to_char( "Zap whom or what?\n\r", ch );
			return;
		}
    }
    else
    {
		if ( ( victim = get_char_room ( ch, arg ) ) == NULL	&&  ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
		{
			send_to_char( "You can't find it.\n\r", ch );
			return;
		}
    }
	if (victim != NULL)

	{
		if ( is_safe( ch, victim ) )
			return;
	}


    if( wand->pIndexData->vnum == 26057 )
    {
	stc("You cannot zap with the Necronomicon. You must wear it\n\r", ch);
	stc("and chant <itemname> <wear location> \n\r", ch);
	return;
    }


    if (!IS_IMMORTAL( ch))
		WAIT_STATE( ch, PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
	if ( victim != NULL )
	{
	    act( "$n zaps $N with $p.", ch, wand, victim, TO_ROOM );
	    act( "You zap $N with $p.", ch, wand, victim, TO_CHAR );
	}
	else
	{
	    act( "$n zaps $P with $p.", ch, wand, obj, TO_ROOM );
	    act( "You zap $P with $p.", ch, wand, obj, TO_CHAR );
	}

	level = wand->value[0];
	if (level < 1) level = 1;
	if (level > MAX_SPELL) level = MAX_SPELL;
    if( wand->carried_by != NULL && has_power_level(wand->carried_by,SCHOOL_ARCANE,4) )
     level *= 4;

	obj_cast_spell( wand->value[3], level, ch, victim, obj );
    }

    if ( --wand->value[2] <= 0 )
    {
	act( "$n's $p explodes into fragments.", ch, wand, NULL, TO_ROOM );
	act( "Your $p explodes into fragments.", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}



void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

	if (IS_NPC(ch))
		return;
	if (ch->level < 3)
	{
		send_to_char ("You must be avatar to steal.\n\r", ch);
		return;
	}

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Steal what from whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

	if (victim->level < 3)
	{
		send_to_char("You must steal from another avatar.\n\r", ch);
		return;
	}
    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL( victim ))
    {
	send_to_char("Steal from an immortal are you crasy!\n\r", ch);
	return;
    }

    if (!IS_IMMORTAL( ch))
    	WAIT_STATE( ch, skill_table[gsn_steal].beats );

    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );

    if ( ( ch->level + number_range(1,20) < victim->level          )
    ||   ( !IS_NPC(ch) && !IS_NPC(victim) && ch->level < 3         )
    ||   ( !IS_NPC(ch) && !IS_NPC(victim) && victim->level < 3     )
    ||   ( victim->position == POS_FIGHTING                        )
    ||   ( !IS_NPC(victim) && IS_IMMUNE(victim,IMM_STEAL)          )
    ||   ( !IS_NPC(victim) && IS_IMMORTAL(victim)                  )
    ||   ( !IS_NPC(ch) && percent > ch->pcdata->learned[gsn_steal] ) )
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
	sprintf( buf, "%s is a bloody thief!", ch->name );
	do_shout( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	    else
	    {
		save_char_obj( ch );
	    }
	}

	return;
    }

    if ( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }
	
    if ( !can_drop_obj( ch, obj )
    ||   IS_SET(obj->extra_flags, ITEM_LOYAL)
    ||   IS_SET(obj->extra_flags, ITEM_INVENTORY) )
    {
	send_to_char( "You can't pry it away.\n\r", ch );
	return;
    }

    if ( ch->carry_number + 1 > can_carry_n( ch ) )
    {
	send_to_char( "You have your hands full.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	send_to_char( "You can't carry that much weight.\n\r", ch );
	return;
    }

    obj_from_char( obj );
    obj_to_char( obj, ch );
    send_to_char( "You got it!\n\r", ch );
    do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}

void do_activate( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if( IS_NPC(ch))
     return;
    if ( arg1[0] == '\0' )
    {
		send_to_char( "Which item do you wish to activate?\n\r", ch );
		return;
    }
	if ( (obj = get_obj_wear(ch,arg1)) == NULL ) 
    {
		if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
		{
			send_to_char( "You can't find that item.\n\r", ch );
			return;
		}
	/* You should only be able to use nontake items on floor */
    }
      if (IS_AFFECTED(ch, AFF_CURSE) ) 
      {
            send_to_char("#1-=You Failed.=-\n\r", ch );
            return;
      }
/*        if (ch->fight_timer > 0)
        {
                send_to_char("You cannot use a transporter with a fight timer.\n\r", ch);
                sprintf(buf,"You have %d rounds left on your fight timer.\n\r", ch->fight_timer);
                send_to_char(buf,ch);
                return;
        }
*/	if(ch->in_room->vnum == 26112)
	{
	   	send_to_char("Vertigo's power keeps your transporter from" 
			     "activating!\n\r", ch);
		return;
	}
        if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_ACTIVATE) )
    {
		send_to_char( "This item cannot be activated.\n\r", ch );
		return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
		send_to_char( "Who do you wish to activate it on?\n\r", ch );
		return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
		if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
		{
			send_to_char( "Nobody here by that name.\n\r", ch );
			return;
		}
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
	&& str_cmp(obj->chpoweruse,"(null)") )
	kavitem(str_dup(obj->chpoweruse),ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
	&& str_cmp(obj->victpoweruse,"(null)") )
	kavitem(str_dup(obj->victpoweruse),ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
	int castlevel = obj->level;
	if (castlevel < 1) castlevel = 1;
	else if (castlevel > 60) castlevel = 60;
	obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
    	if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,6);
	if ( IS_SET(obj->spectype, SITEM_DELAY1)) WAIT_STATE(ch,6);
	if ( IS_SET(obj->spectype, SITEM_DELAY2)) WAIT_STATE(ch,12);
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
//	obj->specpower = ch->in_room->vnum;
	if ( (pRoomIndex == NULL) || (pRoomIndex->vnum >= 1200 && pRoomIndex->vnum <= 1207) ) 
	{
	   stc("Your transporter explodes in your hands!\n\r", ch);
	   ch->hit -= 10000;
	   extract_obj(obj);
	   return;
	}
      if ( pRoomIndex->vnum >= 26000 && pRoomIndex->vnum <= 26206)
      {
        send_to_char("\n\nYou attempt to transport into the realm of\n" 
                     "Dreamscapes, and doom yourself to\n" 
                     "insanity!\n\n\r",ch);
        pRoomIndex = get_room_index(26112);
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	return;
      }
      if( pRoomIndex->vnum >= 50 && pRoomIndex->vnum <= 68 )
      {
       stc("No transporters to arena.\n\r",ch);
       char_from_room(ch);
       char_to_room(ch,get_room_index(3001));
       do_paradox(ch,"self");
       ch->stuntimer = 30;
       sprintf(buf,"%s just tried to transport into the arena...Didn't work out too well for them.",ch->pcdata->switchname);
       do_info(ch,buf);
       return;
      }
      if( pRoomIndex->vnum >= 1200 && pRoomIndex->vnum <= 1207 )  
      {
       stc("No transporters to imm areas.\n\r",ch);
       char_from_room(ch);
       char_to_room(ch,get_room_index(3001));
       do_paradox(ch,"self");
       ch->stuntimer = 30;
       sprintf(buf,"%s just tried to cheat and trans to an imm areas, punish away.",ch->pcdata->switchname);
       do_info(ch,buf);
       return;
      }
	if( pRoomIndex->vnum == 26 )
	{
		stc("Your transporter merely crumbles in your hands!\n\r", ch);
		extract_obj(obj);
		return;
	}

  	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
       if( (ch->in_room->vnum >= 500 && ch->in_room->vnum <= 599) || (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
           ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || ch->in_room->vnum == 30000) && CAN_WEAR(obj,ITEM_TAKE)) )
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            set_stun(ch,2);
            char_from_room(ch);
            char_to_room(ch,get_room_index(3001));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	if ( pRoomIndex == NULL ) return;
      if ( pRoomIndex->vnum >= 26000 && pRoomIndex->vnum <= 26206)
      {
        send_to_char("\n\nYou attempt to transport into the realm of\n"
                     "Dreamscapes, and doom yourself to\n"
                     "insanity!\n\n\r",ch);
        pRoomIndex = get_room_index(26112);
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	return;
      }
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
	if ( get_obj_index(obj->specpower) == NULL ) return;
	obj2 = create_object(get_obj_index(obj->specpower), ch->level);
	if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
	else                           obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
	if ( get_mob_index(obj->specpower) == NULL ) return;
	mob = create_mobile(get_mob_index(obj->specpower));
	char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
	interpret(ch,obj->victpoweron);
	if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
	    obj->victpoweroff != '\0')
	{
	    for ( victim = char_list; victim != NULL; victim = victim_next )
	    {
		victim_next	= victim->next;
		if ( victim->in_room == NULL ) continue;
		if ( victim == ch ) continue;
		if ( victim->in_room == ch->in_room )
		{
		    interpret(victim,obj->victpoweroff);
		    continue;
		}
	    }
	}
    }
    return;
}

void do_press( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;
//    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Which item do you wish to press?\n\r", ch );
	return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL ) 
    {
	if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You can't find that item.\n\r", ch );
	    return;
	}
	/* You should only be able to use nontake items on floor */
	if (CAN_WEAR(obj, ITEM_TAKE))
	{
	    send_to_char( "But you are not wearing it!\n\r", ch );
	    return;
	}
    }
      if (IS_AFFECTED(ch, AFF_CURSE) )
      {
            send_to_char("#1-=You Failed.=-\n\r", ch );
            return;
      }
/*        if (ch->fight_timer > 0)
        {
                send_to_char("You cannot use a transporter with a fight timer.\n\r", ch);
                sprintf(buf,"You have %d rounds left on your fight timer.\n\r", ch->fight_timer);
                send_to_char(buf,ch);
                return;
        }
*/        if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
        {
                send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
                return;
        }
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_PRESS) )
    {
	send_to_char( "There is nothing on this item to press.\n\r", ch );
	return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
	send_to_char( "Who do you wish to use it on?\n\r", ch );
	return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "Nobody here by that name.\n\r", ch );
	    return;
	}
      if (IS_SET (ch->in_room->room_flags, ROOM_SAFE) && !IS_NPC(victim)) {
	send_to_char ("You cannot fight in a safe room.\n\r", ch);
	return;
      }

    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
	&& str_cmp(obj->chpoweruse,"(null)") )
	kavitem(str_dup(obj->chpoweruse),ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
	&& str_cmp(obj->victpoweruse,"(null)") )
	kavitem(str_dup(obj->victpoweruse),ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
	int castlevel = obj->level;
	if (castlevel < 1) castlevel = 1;
	else if (castlevel > 60) castlevel = 60;
	obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
 	if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,6);
	if ( IS_SET(obj->spectype, SITEM_DELAY1)) 
        {
	    if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,6);
	}
	if ( IS_SET(obj->spectype, SITEM_DELAY2)) 
	{
	    if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,12);
	}
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	obj->specpower = ch->in_room->vnum;
	if ( pRoomIndex == NULL ) return;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
       if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	if ( pRoomIndex == NULL ) return;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
	if ( get_obj_index(obj->specpower) == NULL ) return;
	obj2 = create_object(get_obj_index(obj->specpower), ch->level);
	if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
	else                           obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
	if ( get_mob_index(obj->specpower) == NULL ) return;
	mob = create_mobile(get_mob_index(obj->specpower));
	char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
	interpret(ch,obj->victpoweron);
	if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
	    obj->victpoweroff != '\0')
	{
	    for ( victim = char_list; victim != NULL; victim = victim_next )
	    {
		victim_next	= victim->next;
		if ( victim->in_room == NULL ) continue;
		if ( victim == ch ) continue;
		if ( victim->in_room == ch->in_room )
		{
		    interpret(victim,obj->victpoweroff);
		    continue;
		}
	    }
	}
    }
    return;
}

void do_twist( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;
//    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Which item do you wish to twist?\n\r", ch );
	return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL ) 
    {
	if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You can't find that item.\n\r", ch );
	    return;
	}
      if (IS_AFFECTED(ch, AFF_CURSE) )
      {
            send_to_char("#1-=You Failed.=-\n\r", ch );
            return;
      }
/*        if (ch->fight_timer > 0)
        {
                send_to_char("You cannot use a transporter with a fight timer.\n\r", ch);
                sprintf(buf,"You have %d rounds left on your fight timer.\n\r", ch->fight_timer);
                send_to_char(buf,ch);
                return;
        }
*/        if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
        {
                send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
                return;
        }
	/* You should only be able to use nontake items on floor */
	if (CAN_WEAR(obj, ITEM_TAKE))
	{
	    send_to_char( "But you are not wearing it!\n\r", ch );
	    return;
	}
    }
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_TWIST) )
    {
	send_to_char( "This item cannot be twisted.\n\r", ch );
	return;
    }
	if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
	{
		send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
		return;
	}
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
	send_to_char( "Who do you wish to use it on?\n\r", ch );
	return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "Nobody here by that name.\n\r", ch );
	    return;
	}
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
	&& str_cmp(obj->chpoweruse,"(null)") )
	kavitem(str_dup(obj->chpoweruse),ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
	&& str_cmp(obj->victpoweruse,"(null)") )
	kavitem(str_dup(obj->victpoweruse),ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
	int castlevel = obj->level;
	if (castlevel < 1) castlevel = 1;
	else if (castlevel > 60) castlevel = 60;
	obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
    	if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,6);
	if ( IS_SET(obj->spectype, SITEM_DELAY1)) 
	{
	    if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,6);
	}
	if ( IS_SET(obj->spectype, SITEM_DELAY2))
	{
     		if (!IS_IMMORTAL( ch))
			WAIT_STATE(ch,12);
	}
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	obj->specpower = ch->in_room->vnum;
	if ( pRoomIndex == NULL ) return;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
       if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	if ( pRoomIndex == NULL ) return;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
       if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
	if ( get_obj_index(obj->specpower) == NULL ) return;
	obj2 = create_object(get_obj_index(obj->specpower), ch->level);
	if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
	else                           obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
	if ( get_mob_index(obj->specpower) == NULL ) return;
	mob = create_mobile(get_mob_index(obj->specpower));
	char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
	interpret(ch,obj->victpoweron);
	if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
	    obj->victpoweroff != '\0')
	{
	    for ( victim = char_list; victim != NULL; victim = victim_next )
	    {
		victim_next	= victim->next;
		if ( victim->in_room == NULL ) continue;
		if ( victim == ch ) continue;
		if ( victim->in_room == ch->in_room )
		{
		    interpret(victim,obj->victpoweroff);
		    continue;
		}
	    }
	}
    }
    return;
}

void do_pull( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;
//    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "What do you wish to pull?\n\r", ch );
	return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL ) 
    {
	if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You can't find that item.\n\r", ch );
	    return;
	}
      if (IS_AFFECTED(ch, AFF_CURSE) )
      {
            send_to_char("#1-=You Failed.=-\n\r", ch );
            return;
      }
/*        if (ch->fight_timer > 0)
        {
                send_to_char("You cannot use a transporter with a fight timer.\n\r", ch);
                sprintf(buf,"You have %d rounds left on your fight timer.\n\r", ch->fight_timer);
                send_to_char(buf,ch);
                return;
        }
*/        if ((ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68))
        {
                send_to_char("You cannot activate a transporter out whilst you are in the arena.\n\r", ch);
                return;
        }
	/* You should only be able to use nontake items on floor */
	if (CAN_WEAR(obj, ITEM_TAKE))
	{
	    send_to_char( "But you are not wearing it!\n\r", ch );
	    return;
	}
    }
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_PULL) )
    {
	send_to_char( "This item cannot be pulled.\n\r", ch );
	return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
	send_to_char( "Who do you wish to use it on?\n\r", ch );
	return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "Nobody here by that name.\n\r", ch );
	    return;
	}
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
	&& str_cmp(obj->chpoweruse,"(null)") )
	kavitem(str_dup(obj->chpoweruse),ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
	&& str_cmp(obj->victpoweruse,"(null)") )
	kavitem(str_dup(obj->victpoweruse),ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
	int castlevel = obj->level;
	if (castlevel < 1) castlevel = 1;
	else if (castlevel > 60) castlevel = 60;
	obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
    	if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,6);
	if ( IS_SET(obj->spectype, SITEM_DELAY1))
	{
		if (!IS_IMMORTAL( ch))
			WAIT_STATE(ch,6);
	}
	if ( IS_SET(obj->spectype, SITEM_DELAY2)) 
	{
	    if (!IS_IMMORTAL( ch))
		WAIT_STATE(ch,12);
	}
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	obj->specpower = ch->in_room->vnum;
	if ( pRoomIndex == NULL ) return;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
       if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
    	if (obj->chpoweron != NULL && obj->chpoweron != '\0'
	    && str_cmp(obj->chpoweron,"(null)") )
	    kavitem(str_dup(obj->chpoweron),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweron != NULL && obj->victpoweron != '\0'
	    && str_cmp(obj->victpoweron,"(null)") )
	    kavitem(str_dup(obj->victpoweron),ch,obj,NULL,TO_ROOM);
	pRoomIndex     = get_room_index(obj->specpower);
	if ( pRoomIndex == NULL ) return;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
    	if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
	    && str_cmp(obj->chpoweroff,"(null)") )
	    kavitem(str_dup(obj->chpoweroff),ch,obj,NULL,TO_CHAR);
    	if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
	    && str_cmp(obj->victpoweroff,"(null)") )
	    kavitem(str_dup(obj->victpoweroff),ch,obj,NULL,TO_ROOM);
       if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_LIMBO));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
    	if ( (mount = ch->mount) == NULL) return;
    	char_from_room( mount );
    	char_to_room( mount, ch->in_room );
    	do_look( mount, "auto" );
	return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
	if ( get_obj_index(obj->specpower) == NULL ) return;
	obj2 = create_object(get_obj_index(obj->specpower), ch->level);
	if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
	else                           obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
	if ( get_mob_index(obj->specpower) == NULL ) return;
	mob = create_mobile(get_mob_index(obj->specpower));
	char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
	interpret(ch,obj->victpoweron);
	if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
	    obj->victpoweroff != '\0')
	{
	    for ( victim = char_list; victim != NULL; victim = victim_next )
	    {
		victim_next	= victim->next;
		if ( victim->in_room == NULL ) continue;
		if ( victim == ch ) continue;
		if ( victim->in_room == ch->in_room )
		{
		    interpret(victim,obj->victpoweroff);
		    continue;
		}
	    }
	}
    }
    return;
}

bool is_ok_to_wear( CHAR_DATA *ch, bool wolf_ok, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int count;
    argument = one_argument( argument, arg );

    if (!str_cmp(arg,"head"))
    {
	if (IS_HEAD(ch,LOST_HEAD)) return FALSE;
    }
    else if (!str_cmp(arg,"face"))
    {
	if (IS_HEAD(ch,LOST_HEAD)) return FALSE;
    }
    else if (!str_cmp(arg,"left_hand"))
    {
	if (IS_ARM_L(ch,LOST_ARM)) return FALSE;
	else if (IS_ARM_L(ch,BROKEN_ARM)) return FALSE;
	else if (IS_ARM_L(ch,LOST_HAND)) return FALSE;
	else if (IS_ARM_L(ch,BROKEN_THUMB)) return FALSE;
	else if (IS_ARM_L(ch,LOST_THUMB)) return FALSE;
	count = 0;
	if (IS_ARM_L(ch,LOST_FINGER_I)||IS_ARM_L(ch,BROKEN_FINGER_I)) count+= 1;
	if (IS_ARM_L(ch,LOST_FINGER_M)||IS_ARM_L(ch,BROKEN_FINGER_M)) count+= 1;
	if (IS_ARM_L(ch,LOST_FINGER_R)||IS_ARM_L(ch,BROKEN_FINGER_R)) count+= 1;
	if (IS_ARM_L(ch,LOST_FINGER_L)||IS_ARM_L(ch,BROKEN_FINGER_L)) count+= 1;
	if (count > 2) return FALSE;
    }
    else if (!str_cmp(arg,"right_hand"))
    {
	if (IS_ARM_R(ch,LOST_ARM)) return FALSE;
	else if (IS_ARM_R(ch,BROKEN_ARM)) return FALSE;
	else if (IS_ARM_R(ch,LOST_HAND)) return FALSE;
	else if (IS_ARM_R(ch,BROKEN_THUMB)) return FALSE;
	else if (IS_ARM_R(ch,LOST_THUMB)) return FALSE;
	count = 0;
	if (IS_ARM_R(ch,LOST_FINGER_I)||IS_ARM_R(ch,BROKEN_FINGER_I)) count+= 1;
	if (IS_ARM_R(ch,LOST_FINGER_M)||IS_ARM_R(ch,BROKEN_FINGER_M)) count+= 1;
	if (IS_ARM_R(ch,LOST_FINGER_R)||IS_ARM_R(ch,BROKEN_FINGER_R)) count+= 1;
	if (IS_ARM_R(ch,LOST_FINGER_L)||IS_ARM_R(ch,BROKEN_FINGER_L)) count+= 1;
	if (count > 2) return FALSE;
    }
    else if (!str_cmp(arg,"third_hand"))
    {
      if (!IS_SET(ch->newbits, THIRD_HAND)) return FALSE;

    }
    else if (!str_cmp(arg,"fourth_hand"))
    {
      if (!IS_SET(ch->newbits, FOURTH_HAND)) return FALSE;

    }
    else if (!str_cmp(arg,"left_wrist"))
    {
	if (IS_ARM_L(ch,LOST_ARM)) return FALSE;
	else if (IS_ARM_L(ch,LOST_HAND)) return FALSE;
    }
    else if (!str_cmp(arg,"right_wrist"))
    {
	if (IS_ARM_R(ch,LOST_ARM)) return FALSE;
	else if (IS_ARM_R(ch,LOST_HAND)) return FALSE;
    }
    else if (!str_cmp(arg,"left_finger"))
    {
	if (IS_ARM_L(ch,LOST_ARM)) return FALSE;
	else if (IS_ARM_L(ch,LOST_HAND)) return FALSE;
	else if (IS_ARM_L(ch,LOST_FINGER_R)) return FALSE;
    }
    else if (!str_cmp(arg,"right_finger"))
    {
	if (IS_ARM_R(ch,LOST_ARM)) return FALSE;
	else if (IS_ARM_R(ch,LOST_HAND)) return FALSE;
	else if (IS_ARM_R(ch,LOST_FINGER_R)) return FALSE;
    }
    else if (!str_cmp(arg,"arms"))
    {
	if (IS_ARM_L(ch,LOST_ARM) && IS_ARM_R(ch,LOST_ARM)) return FALSE;
    }
    else if (!str_cmp(arg,"hands"))
    {
	if (IS_ARM_L(ch,LOST_ARM) && IS_ARM_R(ch,LOST_ARM)) return FALSE;
	if (IS_ARM_L(ch,LOST_HAND) || IS_ARM_R(ch,LOST_HAND)) return FALSE;
    }
    else if (!str_cmp(arg,"legs"))
    {
	if (IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG)) return FALSE;
    }
    else if (!str_cmp(arg,"feet"))
    {
	if (IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG)) return FALSE;
	if (IS_LEG_L(ch,LOST_FOOT) || IS_LEG_R(ch,LOST_FOOT)) return FALSE;
    }
    return TRUE;
}

void do_qmake( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (arg[0] == '\0')
    {
	send_to_char("Do you wish to qmake a MACHINE or a CARD?\n\r",ch);
	return;
    }
    if (!str_cmp(arg,"card"))
    {
    	if ( (pObjIndex = get_obj_index( OBJ_VNUM_QUESTCARD )) == NULL)
    	{
	    send_to_char("Missing object, please inform KaVir.\n\r",ch);
	    return;
    	}
        if (ch->in_room == NULL) return;
        obj = create_object(pObjIndex, 0);
        obj_to_char(obj, ch);
	quest_object(ch,obj);
    }
    else if (!str_cmp(arg,"machine"))
    {
    	if ( (pObjIndex = get_obj_index( OBJ_VNUM_QUESTMACHINE )) == NULL)
    	{
	    send_to_char("Missing object, please inform KaVir.\n\r",ch);
	    return;
    	}
        if (ch->in_room == NULL) return;
        obj = create_object(pObjIndex, 0);
        obj_to_room(obj, ch->in_room);
    }
    else
    {
	send_to_char("You can only qmake a MACHINE or a CARD.\n\r",ch);
	return;
    }
    send_to_char("Ok.\n\r",ch);
    return;
}
bool check_quest(CHAR_DATA *ch, int value)
{
  OBJ_INDEX_DATA *obj;
  char buf[MAX_STRING_LENGTH];
  if( (obj = get_obj_index(value)) == NULL)
  {
   sprintf(buf, "INVALID VNUM: %d", value);
   send_to_char(buf, ch);
   log_string(buf);
   return FALSE;
  }
  else return TRUE;
}
void do_recharge( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *qobj;
    int count = 0;
    int value = 1;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: recharge <quest card> <quest machine>\n\r", ch );
	return;
    }
    if ( (obj = get_obj_carry(ch,arg1)) == NULL )
    {
	send_to_char( "You are not carrying that object.\n\r", ch );
	return;
    }
    if (obj->item_type != ITEM_QUESTCARD)
    {
	send_to_char( "That is not a quest card.\n\r", ch );
	return;
    }
    if ( (qobj = get_obj_here(ch,arg2)) == NULL )
    {
	send_to_char( "There is nothing for you to recharge it with.\n\r", ch );
	return;
    }
    if (qobj->item_type != ITEM_QUESTMACHINE)
    {
	send_to_char( "That is not a quest machine.\n\r", ch );
	return;
    }
    if (obj->value[0] == -1) count += 1;
    if (obj->value[1] == -1) count += 1;
    if (obj->value[2] == -1) count += 1;
    if (obj->value[3] == -1) count += 1;
    if (count == 4) quest_object( ch, obj );
    else
    {
	send_to_char( "You have not yet completed the current quest.\n\r", ch );
	return;
    }
    act("You place $p into a small slot in $P.",ch,obj,qobj,TO_CHAR);
    act("$n places $p into a small slot in $P.",ch,obj,qobj,TO_ROOM);
    act("$P makes a few clicks and returns $p.",ch,obj,qobj,TO_CHAR);
    act("$P makes a few clicks and returns $p.",ch,obj,qobj,TO_ROOM);
    value = obj->level;
    if (value < 1) value = 1; else if (value > 250) value = 250;

    obj = create_object(get_obj_index( OBJ_VNUM_PROTOPLASM ), 0);
    obj->value[0] = value;
    obj->level = value;
    obj->cost = value*1000;
    obj->item_type = ITEM_QUEST;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "quest token" );
    free_string( obj->short_descr );
    sprintf(buf,"a %d point quest token",value);
    obj->short_descr = str_dup( buf );
    free_string( obj->description );
    sprintf(buf,"A %d point quest token lies on the floor.",value);
    obj->description = str_dup( buf );
    act("You take $p from $P.",ch,obj,qobj,TO_CHAR);
    act("$n takes $p from $P.",ch,obj,qobj,TO_ROOM);
    do_autosave(ch,"");
    return;
}

void quest_object( CHAR_DATA *ch, OBJ_DATA *obj )
{
    static const sh_int quest_selection[] = 
    {
	 102,
	 9201, 9225,  605, 1329, 2276, 5112, 6513, 6517, 5001,14159,
	 5005, 5011, 5012, 5013, 1510, 1352, 2348, 2361, 3005, 9328,
	 2902,  300,  303,  307,14101, 2286, 7224, 8306, 3414, 2005,
	 7216, 1100,  100,30315, 5110, 6001, 3050,  301, 5230,30302,
	  663, 7303, 2915, 2275, 8600, 8601, 8602, 8603, 5030, 9321,
	 6010, 1304, 1307, 1332, 1333, 1342, 1356, 1361, 2304, 2322,
	 2331, 2382, 8003, 8005, 5300, 5302, 5309, 5310, 5311, 4000,
	  601,  664,  900,  906,  923,  311, 7203, 7206, 1101, 5214,
	 5223, 5228, 2804, 1612, 5207, 9302, 5301, 5224, 7801, 9313,
	 6304, 2003, 3425, 3423, 608,  1109,30319, 8903, 9317, 9307,
	 4050,  911, 2204, 4100, 3428,  310, 5113, 3402, 5319, 6512,
	 5114,  913,30316, 2106, 8007,15012, 2333, 3610, 2015, 5022,
	 1394, 2202, 1401, 6005,  647, 1388, 9311, 3604, 4701,
	30325, 6106, 9322, 1384, 3412, 2342, 1374, 2210, 2332, 2901,
	 7200, 7824, 3410, 2013, 2261,19110,19104, 9320,16062, 8501
    };
    int object;
    int holder;
    if (obj == NULL || obj->item_type != ITEM_QUESTCARD) return;

    object = number_range(1, 150);
    holder = quest_selection[object];
    while(!check_quest( ch, holder))
    {
     object = number_range(1,150);
     holder = quest_selection[object];
    }
    if (object < 1 || object > 150) object = 0;
    obj->value[0] = quest_selection[object];

    object = number_range(1, 150);
    if (object < 1 || object > 150) object = 0;
    obj->value[1] = quest_selection[object];

    object = number_range(1, 150);
    if (object < 1 || object > 150) object = 0;
    obj->value[2] = quest_selection[object];

    object = number_range(1, 150);
    if (object < 1 || object > 150) object = 0;
    obj->value[3] = quest_selection[object];
    return;
}

void do_complete( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *qobj;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int count = 0;
    int count2 = 0;
	int complete = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: complete <quest card> <object>\n\r", ch );
	return;
    }

    if ( (qobj = get_obj_carry(ch,arg1)) == NULL )
    {
	send_to_char( "You are not carrying that object.\n\r", ch );
	return;
    }
    else if (qobj->item_type != ITEM_QUESTCARD)
    {
	send_to_char( "That is not a quest card.\n\r", ch );
	return;
    }
    if( qobj->value[0] > 200000) qobj->value[0] = -1;
    if( qobj->value[1] > 200000) qobj->value[1] = -1;
    if( qobj->value[2] > 200000) qobj->value[2] = -1;
    if( qobj->value[3] > 200000) qobj->value[3] = -1;
    if (qobj->value[0] == -1) count += 1;
    if (qobj->value[1] == -1) count += 1;
    if (qobj->value[2] == -1) count += 1;
    if (qobj->value[3] == -1) count += 1;

    if ( arg2[0] == '\0' )
    {
	if (count == 4) {send_to_char("This quest card has been completed.\n\r",ch);return;}
	send_to_char("You still need to find the following:\n\r",ch);
	if (qobj->value[0] != -1)
	    {pObjIndex = get_obj_index( qobj->value[0] );
	    if ( pObjIndex != NULL )
		sprintf(buf,"     %s.\n\r",pObjIndex->short_descr);
		buf[5] = UPPER(buf[5]);send_to_char(buf,ch);}
	if (qobj->value[1] != -1)
	    {pObjIndex = get_obj_index( qobj->value[1] );
	    if ( pObjIndex != NULL )
		sprintf(buf,"     %s.\n\r",pObjIndex->short_descr);
		buf[5] = UPPER(buf[5]);send_to_char(buf,ch);}
	if (qobj->value[2] != -1)
	    {pObjIndex = get_obj_index( qobj->value[2] );
	    if ( pObjIndex != NULL )
		sprintf(buf,"     %s.\n\r",pObjIndex->short_descr);
		buf[5] = UPPER(buf[5]);send_to_char(buf,ch);}
	if (qobj->value[3] != -1)
	    {pObjIndex = get_obj_index( qobj->value[3] );
	    if ( pObjIndex != NULL )
		sprintf(buf,"     %s.\n\r",pObjIndex->short_descr);
		buf[5] = UPPER(buf[5]);send_to_char(buf,ch);}
	return;
    }

    if (count == 4)
    {
	act( "But $p has already been completed!",ch,qobj,NULL,TO_CHAR );
	return;
    }

    if ( (obj = get_obj_carry(ch,arg2)) == NULL )
    {
	send_to_char( "You are not carrying that object.\n\r", ch );
	return;
    }
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	send_to_char( "You cannot use that item.\n\r", ch );
	return;
    }
    if ( (obj->timer > 1) )
    {
	send_to_char( "You can not use a cloned item to complete a quest.\n\r", ch);
	return;
    }
    if (obj->pIndexData->vnum == 30037 || obj->pIndexData->vnum == 30041)
    {
	send_to_char( "That item has lost its quest value, you must collect a new one.\n\r", ch );
	return;
    }
	while (complete < 4)
	{
		if (qobj->value[complete] != -1)
		{
			pObjIndex = get_obj_index(qobj->value[complete]);
			if (pObjIndex != NULL && !str_cmp(obj->short_descr, pObjIndex->short_descr))
			{
				qobj->value[complete] = -1;
				//complete = 4;
			}
		}
		complete++;
	}
    if (qobj->value[0] == -1) count2 += 1;
    if (qobj->value[1] == -1) count2 += 1;
    if (qobj->value[2] == -1) count2 += 1;
    if (qobj->value[3] == -1) count2 += 1;
    if (count == count2)
    {
		send_to_char( "That item is not required.\n\r", ch );
		return;
    }

    act("You touch $p to $P, and $p vanishes!",ch,obj,qobj,TO_CHAR);
    act("$n touches $p to $P, and $p vanishes!",ch,obj,qobj,TO_ROOM);
    obj_from_char(obj);
    extract_obj(obj);
    if (count >= 3)
	{
		act("$p has been completed!",ch,qobj,NULL,TO_CHAR);
	}
    else if (count == 2)
	{
		act("$p now requires one more object!",ch,qobj,NULL,TO_CHAR);
	}
    else if (count == 1)
	{
		act("$p now requires two more objects!",ch,qobj,NULL,TO_CHAR);
	}
    else if (count == 0)
	{
		act("$p now requires three more objects!",ch,qobj,NULL,TO_CHAR);
	}
    return;
}

void do_sheath( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);

    if (arg[0] == '\0') send_to_char("Which hand, left or right?\n\r",ch);
    else if (!str_cmp(arg,"all") || !str_cmp(arg,"both"))
    {
	sheath(ch,TRUE );
	sheath(ch,FALSE);
    }
    else if (!str_cmp(arg,"l") || !str_cmp(arg,"left" )) sheath(ch,FALSE);
    else if (!str_cmp(arg,"r") || !str_cmp(arg,"right")) sheath(ch,TRUE );
    else send_to_char("Which hand, left or right?\n\r",ch);
    return;
}

void do_draw( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);

    if (arg[0] == '\0') send_to_char("Which hand, left or right?\n\r",ch);
    else if (!str_cmp(arg,"all") || !str_cmp(arg,"both"))
    {
	draw(ch,TRUE );
	draw(ch,FALSE);
    }
    else if (!str_cmp(arg,"l") || !str_cmp(arg,"left" )) draw(ch,FALSE);
    else if (!str_cmp(arg,"r") || !str_cmp(arg,"right")) draw(ch,TRUE );
    else send_to_char("Which hand, left or right?\n\r",ch);
    return;
}

void sheath( CHAR_DATA *ch, bool right )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int scabbard;

    if (right)
    {
	scabbard = WEAR_SCABBARD_R;
	if ( (obj = get_eq_char(ch,WEAR_WIELD)) == NULL )
	{
	    send_to_char("You are not holding anything in your right hand.\n\r",ch);
	    return;
	}
	else if ( (obj2 = get_eq_char(ch,scabbard)) != NULL)
	{
	    act("You already have $p in your right scabbard.",ch,obj2,NULL,TO_CHAR);
	    return;
	}
    	act("You slide $p into your right scabbard.",ch,obj,NULL,TO_CHAR);
    	act("$n slides $p into $s right scabbard.",ch,obj,NULL,TO_ROOM);
    }
    else
    {
	scabbard = WEAR_SCABBARD_L;
	if ( (obj = get_eq_char(ch,WEAR_HOLD)) == NULL )
	{
	    send_to_char("You are not holding anything in your left hand.\n\r",ch);
	    return;
	}
	else if ( (obj2 = get_eq_char(ch,scabbard)) != NULL)
	{
	    act("You already have $p in your left scabbard.",ch,obj2,NULL,TO_CHAR);
	    return;
	}
    	act("You slide $p into your left scabbard.",ch,obj,NULL,TO_CHAR);
    	act("$n slides $p into $s left scabbard.",ch,obj,NULL,TO_ROOM);
    }
    if (obj->item_type != ITEM_WEAPON)
    {
	act("$p is not a weapon.",ch,obj,NULL,TO_CHAR);
	return;
    }
    unequip_char(ch,obj);
    obj->wear_loc = scabbard;
    return;
}

void draw( CHAR_DATA *ch, bool right )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int scabbard;
    int worn;
    if (right)
    {
	scabbard = WEAR_SCABBARD_R;
	worn = WEAR_WIELD;
	if ( (obj = get_eq_char(ch,scabbard)) == NULL )
	{
	    send_to_char("Your right scabbard is empty.\n\r",ch);
	    return;
	}
	else if ( (obj2 = get_eq_char(ch,WEAR_WIELD)) != NULL)
	{
	    act("You already have $p in your right hand.",ch,obj2,NULL,TO_CHAR);
	    return;
	}
    	act("You draw $p from your right scabbard.",ch,obj,NULL,TO_CHAR);
    	act("$n draws $p from $s right scabbard.",ch,obj,NULL,TO_ROOM);
    }
    else
    {
	scabbard = WEAR_SCABBARD_L;
	worn = WEAR_HOLD;
	if ( (obj = get_eq_char(ch,scabbard)) == NULL )
	{
	    send_to_char("Your left scabbard is empty.\n\r",ch);
	    return;
	}
	else if ( (obj2 = get_eq_char(ch,WEAR_HOLD)) != NULL)
	{
	    act("You already have $p in your left hand.",ch,obj2,NULL,TO_CHAR);
	    return;
	}
    	act("You draw $p from your left scabbard.",ch,obj,NULL,TO_CHAR);
    	act("$n draws $p from $s left scabbard.",ch,obj,NULL,TO_ROOM);
    }
    obj->wear_loc = -1;
    equip_char(ch,obj,worn);
    return;
}

char *special_item_name( OBJ_DATA *obj )
{
    static char buf[MAX_INPUT_LENGTH];
    int dice = number_range(1,4);
    switch ( dice )
    {
    default:
	strcpy(buf,"golden");
	break;
    case 1:
	strcpy(buf,"golden");
	break;
    case 2:
	strcpy(buf,"silver");
	break;
    case 3:
	strcpy(buf,"brass");
	break;
    case 4:
	strcpy(buf,"copper");
	break;
    }
    return buf;
}


/********* FUNCTION TO MAKE RANDOM EQ **********/

	void do_make_eq(CHAR_DATA *ch, char *argument)
	{
		OBJ_DATA *obj = NULL;
		OBJ_INDEX_DATA *pObjIndex = NULL;
		char arg[MAX_INPUT_LENGTH];
		int wear_location, weap_type, att_type, att_value;
		int min, max, affect;
		char name[MAX_STRING_LENGTH];
		char sdesc[MAX_STRING_LENGTH];
		char ldesc[MAX_STRING_LENGTH];

      if ( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
    	{	    send_to_char( "Error...missing object, please inform Kale.\n\r", ch );	    return;		}

/******** Create Object and Set Initial Values *********/

		argument = one_argument(argument, arg);

		obj = create_object(pObjIndex, 25);
		obj->weight = 0;
		obj->cost = 1000;
		obj->level = 50;
     	        SET_BIT( obj->extra_flags, ITEM_FORGED );
		wear_location = number_range(1, 13);
		free_string(obj->name);
		free_string(obj->short_descr);
		free_string(obj->description);
		if( arg[0] == '\0' 
		||  arg == NULL 
		||  (str_cmp(arg, "DSMOB")
		&&   str_cmp(arg, "DMWINNER")
		&&   str_cmp(arg, "REWARD")) )

		{
		   sprintf(name, "Immortal EQ");
		   sprintf(sdesc, "#6EQ #7Of #0An #6Im#7mo#0rt#6al#n");
		   stc("You chant words which are unknown to you, creating.. you don't know what the hell you've created!\n\r", ch);
		   act("$n chants words which are unknown to you, creating.. you don't know what the hell was created!", ch,NULL,NULL,TO_ROOM);
		}
		if( !str_cmp(arg, "DSMOB") )
		{
			sprintf(name, "Dream Essence");
			sprintf(sdesc, "#6Es#7se#0nc#6e O#7f D#0re#6am#7s#n");
		}
		if( !str_cmp(arg, "DMWINNER") )
		{
			sprintf(name, "A Champion's Sigil of Blood");
			sprintf(sdesc, "#7A #0C#nh#7a#rm#1pi#ro#7n#n'#0s #lS#4i#lg#4i#ll #7of #rBl#1oo#rd#n");
		}
		if( !str_cmp(arg, "REWARD") )
		{
			sprintf(name, "Worthless Trash");
			sprintf(sdesc, "#bA Worthless Piece of Trash#n");
		}
		obj->name = str_dup(name);
		obj->short_descr = str_dup(sdesc);
		sprintf(ldesc, "#0The Unknown lies within#n");
		obj->description = str_dup(ldesc);
/******** Get Random Wear Location *************/

		if( wear_location == 1 )
		{
			obj->item_type = ITEM_WEAPON;
			obj->value[0] = 0;
		}
		else
		{
			obj->item_type = ITEM_ARMOR;
			obj->value[0] = 30;
		}
		switch (wear_location)
		{
			case 1 :
				obj->wear_flags = ITEM_WIELD;
				weap_type = number_range(1, 12);
				obj->value[3] = weap_type;
				min = number_range(10, 30);
				max = number_range(40, 60);
				obj->value[1] = min;
				obj->value[2] = max;
				break;

			case 2 :
				obj->wear_flags = ITEM_WEAR_FINGER;
				break;

			case 3 :
				obj->wear_flags = ITEM_WEAR_NECK;
				break;

			case 4:
				obj->wear_flags = ITEM_WEAR_FACE;
				break;

			case 5:
				obj->wear_flags = ITEM_WEAR_BODY;
				break;

			case 6:
				obj->wear_flags = ITEM_WEAR_HEAD;
				break;

			case 7:
				obj->wear_flags = ITEM_WEAR_LEGS;
				break;

			case 8:
				obj->wear_flags = ITEM_WEAR_FEET;
				break;

			case 9:
				obj->wear_flags = ITEM_WEAR_HANDS;
				break;

			case 10:
				obj->wear_flags = ITEM_WEAR_ARMS;
				break;

			case 11:
				obj->wear_flags = ITEM_WEAR_ABOUT;
				break;

			case 12:
				obj->wear_flags = ITEM_WEAR_WAIST;
				break;

			case 13:
				obj->wear_flags = ITEM_WEAR_WRIST;
				break;

		}

/********** Set Random Affects ***********/

		if( number_percent() >= 95 )
		{
			affect = number_range(150, 225);
			affect = affect * -1;
			mob_oset_affect( ch, obj, affect, APPLY_AC, TRUE );
		}
		else
		{
			affect = number_range(90, 150);
			affect = affect * -1;
			mob_oset_affect( ch, obj, affect, APPLY_AC, TRUE );
		}

		if( number_percent() >= 95 )
		{
			affect = number_range(90, 140);
			mob_oset_affect( ch, obj, affect, APPLY_HITROLL, TRUE );
		}
		else
		{
			affect = number_range(54, 90);
			mob_oset_affect( ch, obj, affect, APPLY_HITROLL, TRUE );
		}

		if( number_percent() >= 95 )
		{
			affect = number_range(90, 140);
			mob_oset_affect( ch, obj, affect, APPLY_DAMROLL, TRUE );
		}
		else
		{
			affect = number_range(54, 90);
			mob_oset_affect( ch, obj, affect, APPLY_DAMROLL, TRUE );
		}

/********** 20% Chance of Adding Attributes and HP, Mana, or Move **********/

		if( number_percent() > 20 )
		{
			obj_to_char(obj, ch);
			if( IS_NPC(ch) )
			  wear_obj(ch, obj, FALSE);

			return;
		}

		att_value = number_range(1, 7);
		att_type = number_range(1, 3);
		affect = number_range(15, 75);
		switch (att_type)
		{
			case 1 :
				mob_oset_affect( ch, obj, att_value, APPLY_INT, TRUE );
				mob_oset_affect( ch, obj, att_value, APPLY_CON, TRUE );
				mob_oset_affect( ch, obj, affect, APPLY_HIT, TRUE );
				break;

			case 2:
				mob_oset_affect( ch, obj, att_value, APPLY_DEX, TRUE );
				mob_oset_affect( ch, obj, att_value, APPLY_WIS, TRUE );
				mob_oset_affect( ch, obj, affect, APPLY_MOVE, TRUE );
				break;

			case 3:
				mob_oset_affect( ch, obj, att_value, APPLY_STR, TRUE );
				mob_oset_affect( ch, obj, affect, APPLY_MANA, TRUE );
				break;
		}
		obj_to_char(obj, ch);
		if( IS_NPC(ch) )
			wear_obj(ch, obj, FALSE);

		return;

	}

/******** END FUNCTION do_make_eq **********/

/******** FUNCTION TO FORGE ONE RING ********/

void do_one_ring(CHAR_DATA *ch, char *argument)
{

   OBJ_DATA *obj;
   int  count_imprisonment = 0;
   int  count_damnation = 0;
   int  count_ignorance = 0;
   int  count_sets;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char arg3[MAX_INPUT_LENGTH];

/******** CHECK FOR INSCRIPTION *******/

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);
   argument = one_argument(argument, arg3);


   if( (arg1[0] == '\0' || str_cmp("rule",arg1))
   ||  (arg2[0] == '\0' || str_cmp("them",arg2))
   ||  (arg3[0] == '\0' || str_cmp("all", arg3)) )
   {
	stc("The inscription must be complete to form the ring!\n\r", ch);
	return;
   }

/******** CHECK FOR SAURON'S ROOM ********/

   if( ch->in_room->vnum != 27 )
   {
	stc("You do not have the power to inscribe outside of Mount Doom.\n\r", ch);
	return;
   }

/******** CHECK FOR DREAMSCAPE EQ ********/

   for(obj = ch->carrying; obj != NULL; obj = obj->next_content)
   {
     if( obj->pIndexData->vnum == 26001 )
	count_imprisonment++;
     if( obj->pIndexData->vnum == 26002 )
	count_damnation++;
     if( obj->pIndexData->vnum == 26003 )
	count_ignorance++;
   }


/********* EXTRACT DREAMSCAPE EQ IF ENOUGH IS COUNTED **********/

   if( count_imprisonment < 17 || count_damnation < 17 || count_ignorance < 17 )
   {
	stc("You do not have the right items in the minimum quantity to inscribe the ring.\n\r", ch);
	return;
   }

   count_imprisonment = 17;
   count_damnation    = 17;
   count_ignorance    = 17;

   for( count_sets = 0; count_sets < 51; count_sets++ )
   {
      for( obj = ch->carrying; obj != NULL; obj = obj->next_content )
      {
	if( obj->pIndexData->vnum == 26001 && count_imprisonment > 0 )
      	{
	  extract_obj(obj);
  	  count_imprisonment--;
      	}
      	if( obj->pIndexData->vnum == 26002 && count_damnation > 0 )
      	{
	  extract_obj(obj);
	  count_damnation--;
      	}
      	if( obj->pIndexData->vnum == 26003 && count_ignorance > 0 )
      	{
	  extract_obj(obj);
	  count_ignorance--;
      	}
      }
   }
   do_make_ring(ch, "");
   return;
}
/********* END FUNCTION FOR FORGING ONE RING *******/



/********* FUNCTION TO SET RING ATTRIBUTES *********/

void do_make_ring(CHAR_DATA *ch, char *argument)
{

   OBJ_DATA *obj = NULL;
   OBJ_INDEX_DATA *pObjIndex = NULL;
   char name[MAX_STRING_LENGTH];
   char sdesc[MAX_STRING_LENGTH];
   char ldesc[MAX_STRING_LENGTH];
   int affect = -8500;

/*********** MAKE OBJECT AND SET INITIAL VALUES ************/
   if( ( pObjIndex = get_obj_index( OBJ_VNUM_PROTOPLASM ) ) == NULL )
   {
	stc("Error...missing object, please inform Kale.\n\r", ch);
	return;
   }
   obj = create_object(pObjIndex, 25);
   obj->weight = 0;
   obj->cost = 1000;
   obj->level = 50;
   obj->value[0] = 2000;
   obj->value[3] = 5;
   obj->item_type = ITEM_ARMOR;
   obj->wear_flags = ITEM_WEAR_FINGER;
   SET_BIT(obj->extra_flags, ITEM_ONE_RING);

/************* SET OBJECT NAMES *************/
   free_string(obj->name);
   free_string(obj->short_descr);
   free_string(obj->description);
   sprintf(name, "The One Ring");
   sprintf(sdesc, "#oThe One Ring#n");
   sprintf(ldesc, "#0A #oGolden Ring #0of absolute power wills you to pick it up#n");
   obj->name = str_dup(name);
   obj->short_descr = str_dup(sdesc);
   obj->description = str_dup(ldesc);

/************ SET HP, MANA AND MOVE VALUES ************/
   mob_oset_affect( ch, obj, affect, APPLY_HIT, TRUE );
   mob_oset_affect( ch, obj, affect, APPLY_MANA, TRUE );
   mob_oset_affect( ch, obj, affect, APPLY_MOVE, TRUE );

/*********** TRANSFER OBJECT TO INVENTORY ***********/
   obj_to_char(obj, ch);
   stc("Items in your hands begin to vibrate violently...\n\r", ch);
   stc("Suddenly they explode, leaving a brilliant ring in your hand!\n\r", ch);
   stc("For a brief moment, the magic surrounding you allows you to\n\r", ch);
   stc("make out the ancient Elvish letters, the language which is that\n\r", ch);
   stc("of Mordor, which should never be uttered. The words read:\n\r", ch);
   stc("\n\rOne Ring to rule them all, One Ring to find them,\n\r", ch);
   stc("One Ring to bring them all and in the darkness bind them.\n\r", ch);
   do_save(ch, "");
   if( !IS_IMMORTAL(ch) )
   {
      ch->hit -= 5000;
      ch->mana -= 5000;
      ch->move -= 5000;
      stc("\n\rThe events leave you devasted!\n\r", ch);
      set_stun(ch, 10);
      update_pos(ch);
   }
   return;
}
/**************** END FUNCTION FOR RING ATTRIBUTES ********************/


bool wearing_one_ring(CHAR_DATA *ch)
{

   OBJ_DATA *obj;
   int i;

   for( i = 0; i < MAX_WEAR; i++ )
   {
	if( ( obj = get_eq_char(ch, i) ) == NULL )
	   continue;
	if( IS_OBJ_STAT(obj, ITEM_ONE_RING ) )
	   return TRUE;
   }
   return FALSE;
}


bool is_naked(CHAR_DATA *ch)
{

   OBJ_DATA *obj;
   int i;

   for( i = 0; i < MAX_WEAR; i++ )
   {
	if( ( obj = get_eq_char(ch, i) ) != NULL )
	  return FALSE;
   }
   return TRUE;
}

/******** FUNCTION FOR NEWBIE TRANSPORTER ********/
void newbie_trans(OBJ_DATA *obj)
{

   SET_BIT(obj->spectype, SITEM_ACTIVATE);
   SET_BIT(obj->spectype, SITEM_TRANSPORTER);
   obj->specpower = 3001;
   free_string(obj->chpoweron);
   obj->chpoweron = str_dup("You transform into a fine mist and seep into the ground.");
   free_string(obj->victpoweron);
   obj->victpoweron = str_dup("$n transforms into a fine mist and seeps into the ground.");
   free_string(obj->chpoweroff);
   obj->chpoweroff = str_dup("You seep up from the ground and reform your body.");
   free_string(obj->victpoweroff);
   obj->victpoweroff = str_dup("A fine mist seeps up from the ground and reforms into $n.");
   free_string(obj->chpoweruse);
   obj->chpoweruse = str_dup("You activate $p.");
   free_string(obj->victpoweruse);
   obj->victpoweruse = str_dup("$n activates $p.");
}
/******* END NEWBIE TRANSPORTER FUNCTION *******/

/******* FUNCTION TO OBTAIN NEWBIE TRANSPORTER ******/
void do_pray(CHAR_DATA *ch, char *argument)
{

   OBJ_DATA *obj = NULL;
   char arg[MAX_INPUT_LENGTH];
   

   argument = one_argument(argument, arg); 
   if( arg[0] == '\0' )
   {
      stc("Who do you wish to pray to?\n\r", ch);
      return;
   }
   if( str_cmp("loimos", arg) )
   {
      stc("You must pray to Loimos.\n\r", ch);
      return;
   }

   obj = create_object(get_obj_index(3390),0);
   newbie_trans(obj);
   act("Loimos hears your prayer, $p appears in your hands in an explosion of energy!\n\r",ch, obj, NULL, TO_CHAR);
   act("$n begins praying when suddenly you see $p appear in their hands in an explosion of energy!\n\r", ch, obj, NULL, TO_ROOM); 
   obj_to_char(obj, ch);
   return;
}

void do_quicken( CHAR_DATA *ch, char *argument )
{

   OBJ_DATA *obj;
   int i;

/*   if( ch->level < 10 && ch->trust < 10 )
   {
	stc("The power of the #7Quickening#n does not surround you.\n\r", ch);
	return;
   } */
   if( ch->in_room->vnum != 3054 )
   {
	stc("You must be at the temple altar to endure the power of the Quickening.\n\r", ch);
	return;
   }
   for( i = 0; i < MAX_WEAR; i++ )
   {
	if( (obj = get_eq_char(ch, i)) == NULL )
	   continue;
/*	mob_oset_affect(ch, obj, 10, APPLY_HITROLL, TRUE);
	mob_oset_affect(ch, obj, 10, APPLY_DAMROLL, TRUE);
	mob_oset_affect(ch, obj, -25, APPLY_AC, TRUE);  */
	total_affects(ch, obj);
   }

   stc("#7Lightning#n strikes your body as you kneel before the altar.\n\r", ch);
   stc("Your #4equipment begins to glow blue as the power of the\n\r", ch);
   stc("#7Quickening#n flows from your #1blood#n into it.\n\r", ch);
   stc("It then ceases from glowing and returns to it's original\n\r", ch);
   stc("color... yet you feel a great power surging through it!\n\r", ch);
   stc("The power bestowed upon it from the #1blood#n of your #0victims!#n\n\r\n\r", ch);
   do_clearstats(ch, "");
   do_save(ch, "self");
   return;
}   

void do_reward(CHAR_DATA *ch, char *argument)
{
   
   CHAR_DATA *recipient;
   char arg1[MAX_INPUT_LENGTH];
   char arg2[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int i, choice;

   argument = one_argument(argument, arg1);
   argument = one_argument(argument, arg2);

   if( arg1[0] == '\0' || arg1 == NULL 
   ||  arg2[0] == '\0' || arg2 == NULL )
   {
	stc("Syntax is: reward <playername> <reward number>\n\r\n\r", ch);
	do_help(ch, "reward");
	return;
   }

   if( (recipient = get_char_world(ch, arg1)) == NULL )
   {
	stc("They are not here.\n\r", ch);
	return;
   }

   if( str_cmp(arg2, "1") && str_cmp(arg2, "2") && str_cmp(arg2, "3")
   &&  str_cmp(arg2, "4")  )
   {
	stc("Invalid reward number: \n\r\n\r", ch);
	do_help(ch, "reward");
   	return;
   }

   choice = atoi(arg2);

   switch(choice)
   {
	case 1:
	   sprintf(buf, "You reward %s with 8 items.\n\r", recipient->name);
	   stc(buf, ch);
	   sprintf(buf, "%s has deemed you worthy...\n\r", ch->name);
	   stc(buf, recipient);
	   stc("8 items appear in your hands!\n\r", recipient);
	   for( i = 0; i < 8; i++ )
		do_make_eq(recipient, "REWARD");
	   break;
	case 2:
	   sprintf(buf, "You increase %s's max hp.\n\r", recipient->name);
	   stc(buf, ch);
	   sprintf(buf, "%s has deemed you worthy...\n\r", ch->name);
	   stc(buf, recipient);
	   stc("Your ability to gain hp increases!\n\r", recipient);
	   recipient->max_hit += 200;
	   break;
	case 3:
	   sprintf(buf, "You increase %s's max mana.\n\r", recipient->name);
	   stc(buf, ch);
	   sprintf(buf, "%s has deemed you worthy...\n\r", ch->name);
	   stc(buf, recipient);
	   stc("Your ability to gain mana increases!\n\r", recipient);
	   recipient->max_mana += 200;
	   break;
	case 4:
	   sprintf(buf, "You increase %s's max move.\n\r", recipient->name);
	   stc(buf, ch);
	   sprintf(buf, "%s has deemed you worthy...\n\r", ch->name);
	   stc(buf, recipient);
	   stc("Your ability to gain move increases!\n\r", recipient);
	   recipient->max_move += 200;
	   break;


	default:
	   stc("Invalid reward number: \n\r\n\r", ch);
	   do_help(ch, "reward");
   }

   return;

}
