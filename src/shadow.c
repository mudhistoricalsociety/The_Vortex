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

void    check_killer    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
OBJ_DATA *get_eye args( (CHAR_DATA *ch));

void do_runeweave( CHAR_DATA *ch, char *argument )
{
 int cost = 0;
 int sn;
 AFFECT_DATA af;
 AFFECT_DATA *afr, *afr_next;
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 else if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_RUNE) < 1 )
 {
  stc("You require at least a level 1 knowledge of runes first.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What kind of rune do you wish to weave? (Heal, Negativity, Drain, Nullify)\n\r", ch );
  return;
 }
 if( !str_cmp(arg, "heal") )
 {
  CHAR_DATA *victim;
  int heal = 0;
  char arg2[MAX_INPUT_LENGTH];

  argument = one_argument(argument,arg2);

  if( GET_POWERS(ch, SHADOW_RUNE) < 4 )
  {
   heal = number_range(200,600);
   cost = ( heal * 3 );
  }
  else
  {
   heal = number_range(400,800);
   cost = ( heal * 2 );
  }
  if( GET_POWERS(ch, SHADOW_RUNE) < 1 )
  {
   stc("You require at least a level 1 knowledge of runes first.\n\r", ch);
   return;
  }
  if( ch->mana < cost )
  {
   stc("You are too tired to weave a rune.\n\r", ch);
   return;
  }
  if( arg2[0] != '\0' )
   victim = get_char_room(ch,arg2);
  else if( arg2[0] == '\0' )
   victim = ch;
  if( victim == NULL )
  {
   stc("They're not here.\n\r", ch);
   return;
  }
  stc("You begin to slowly weave a rune into the air...\n\r", ch);
  act("$n begins to slowly weave a rune into the air...", ch, NULL, NULL, TO_NOTVICT);
  if( victim == ch )
  {
   sprintf(buf, "You finish your weave and the rune flashes a bright #4blue#n then vanishes! [%d]\n\r", heal);
   stc(buf,ch);
  }
  else if( victim != ch )
  {
   stc("You finish your weave and the rune flashes a bright #4blue#n then vanishes!\n\r", ch);
   sprintf(buf, "$n finishes $s weave and the rune flashes a bright #4blue#n then vanishes! [%d]", heal);
   act(buf, ch, NULL, victim, TO_VICT);
  }
  act("$n finishes $s weave and the rune flashes a bright #4blue#n then vanishes!", ch, NULL, NULL, TO_NOTVICT);
  victim->hit += heal;
  ch->mana -= cost;
  if( GET_POWERS(ch, SHADOW_RUNE) < 5 )
   INC_POWERS(ch, SHADOW_LEV_RUNE);
  if( GET_POWERS(ch, SHADOW_LEV_RUNE) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_RUNE) )
   && GET_POWERS(ch, SHADOW_RUNE) < 5 )
  {
   stc("Your mastery of rune has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_RUNE) = 0;
   INC_POWERS(ch, SHADOW_RUNE);
  }
  WAIT_STATE(ch,8);
  return;
 }
 else if( !str_cmp(arg, "negativity") )
 {
  if( GET_POWERS(ch, SHADOW_RUNE) < 4 )
   cost = 8000;
  else
   cost = 4000;
  if( GET_POWERS(ch, SHADOW_RUNE) < 2 )
  {
   stc("You require at least a level 2 knowledge of runes first.\n\r", ch);
   return;
  }
  if( ch->in_room == NULL )
   return;
  if( ch->mana < cost )
  {
   stc("You are too tired to weave a rune.\n\r", ch);
   return;
  }
  if( (sn = skill_lookup("negativity_rune")) < 0)
   return;
  stc("You begin to slowly weave a rune into the air...\n\r", ch);
  act("$n begins to slowly weave a rune into the air...", ch, NULL, NULL, TO_NOTVICT);
  ch->mana -= cost;
  af.type = sn;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = 4;
  af.bitvector = AFF_ROOM_NEGATIVITY;
  affect_to_room(ch->in_room, &af);
  stc("You finish your weave and the rune flashes a bright #3yellow#n then vanishes!\n\r", ch);
  act("$n finishes $s weave and the rune flashes a bright #3yellow#n then vanishes!", ch, NULL, NULL, TO_NOTVICT);
  if( GET_POWERS(ch, SHADOW_RUNE) < 5 )
   INC_POWERS(ch, SHADOW_LEV_RUNE);
  if( GET_POWERS(ch, SHADOW_LEV_RUNE) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_RUNE) )
   && GET_POWERS(ch, SHADOW_RUNE) < 5 )
  {
   stc("Your mastery of rune has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_RUNE) = 0;
   INC_POWERS(ch, SHADOW_RUNE);
  }
  WAIT_STATE(ch,8);
  return;
 }
 else if( !str_cmp(arg, "drain") )
 {
  if( GET_POWERS(ch, SHADOW_RUNE) < 4 )
   cost = 10000;
  else
   cost = 5000;
  if( GET_POWERS(ch, SHADOW_RUNE) < 3 )
  {
   stc("You require at least a level 3 knowledge of runes first.\n\r", ch);
   return;
  }
  if( ch->in_room == NULL )
   return;
  if( ch->mana < cost )
  {
   stc("You are too tired to weave a rune.\n\r", ch);
   return;
  }
  if( (sn = skill_lookup("drain_rune")) < 0)
   return;
  stc("You begin to slowly weave a rune into the air...\n\r", ch);
  act("$n begins to slowly weave a rune into the air...", ch, NULL, NULL, TO_NOTVICT);
  ch->mana -= cost;
  ch->in_room->rune_by = str_dup(ch->name);
  af.type = sn;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = 2;
  af.bitvector = AFF_ROOM_DRAIN;
  affect_to_room(ch->in_room, &af);
  stc("You finish your weave and the rune flashes a bright #1red#n then vanishes!\n\r", ch);
  act("$n finishes $s weave and the rune flashes a bright #1red#n then vanishes!", ch, NULL, NULL, TO_NOTVICT);
  if( GET_POWERS(ch, SHADOW_RUNE) < 5 )
   INC_POWERS(ch, SHADOW_LEV_RUNE);
  if( GET_POWERS(ch, SHADOW_LEV_RUNE) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_RUNE) )
   && GET_POWERS(ch, SHADOW_RUNE) < 5 )
  {
   stc("Your mastery of rune has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_RUNE) = 0;
   INC_POWERS(ch, SHADOW_RUNE);
  }
  WAIT_STATE(ch,8);
  return;
 }
 else if( !str_cmp(arg, "nullify") )
 {
  if( GET_POWERS(ch, SHADOW_RUNE) < 4 )
   cost = 12000;
  else
   cost = 6000;
  if( GET_POWERS(ch, SHADOW_RUNE) < 5 )
  {
   stc("You first require full mastery of your art.\n\r", ch);
   return;
  }
  if( ch->in_room == NULL )
   return;
  if( ch->mana < cost )
  {
   stc("You are too tired to weave a rune.\n\r", ch);
   return;
  }
  stc("You begin to slowly weave a rune into the air...\n\r", ch);
  act("$n begins to slowly weave a rune into the air...", ch, NULL, NULL, TO_NOTVICT);
  ch->mana -= cost;
  for( afr = ch->in_room->affected; afr != NULL; afr = afr_next)
  {
   afr_next = afr->next;
   if( afr->type == skill_lookup("dart_trap") || afr->type == skill_lookup("poison_trap")
    || afr->type == skill_lookup("explosion_trap") || afr->type == skill_lookup("sleep_trap") )
     ch->in_room->trapped_by = NULL;
   if( afr->type == skill_lookup("drain_rune") )
    ch->in_room->rune_by = NULL;
   room_affect_remove(ch->in_room,afr);
  }
  stc("You finish your weave and the rune flashes a bright #2green#n then vanishes!\n\r", ch);
  act("$n finishes $s weave and the rune flashes a bright #2green#n then vanishes!", ch, NULL, NULL, TO_NOTVICT);
  WAIT_STATE(ch,8);
  return;
 }
 return;
}

void do_traproom( CHAR_DATA *ch, char *argument )
{
 int sn;
 int cost = 0;
 int i = 0;
 AFFECT_DATA af;
 CHAR_DATA *vch, *vch_next;
 char arg[MAX_INPUT_LENGTH];
 int ws = 0;

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 else if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_TRAP) < 1 )
 {
  stc("You require at least a level 1 knowledge of trapping first.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What kind of trap do you wish to set? (Dart, Poison, Explosion, Sleep)\n\r", ch );
  return;
 }
 if( ch->in_room->trapped_by != NULL )
 {
  if( !str_cmp(ch->in_room->trapped_by, ch->name) )
   stc("You already have a trap here!\n\r", ch);
  else
   stc("Somebody else already has a trap here!\n\r", ch);
  return;
 }
 for( vch = ch->in_room->people; vch != NULL; vch = vch_next )
 {
  vch_next = vch->next_in_room;
  if( !IS_NPC(vch) )
   i++;
  if( i > 1 )
  {
   stc("You can't set a trap with somebody watching!\n\r", ch);
   return;
  }
 }
 if( !str_cmp(arg, "dart") )
 {
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   cost = 6000;
   ws = 12;
  }
  else
  {
   cost = 3000;
   ws  = 6;
  }
  if( GET_POWERS(ch, SHADOW_TRAP) < 1 )
  {
   stc("You do not know how to set a dart trap!\n\r", ch);
   return;
  }
  if( ch->move < cost )
  {
   stc("You are too tired to create a dart trap.\n\r", ch);
   return;
  }
  if( (sn = skill_lookup("dart_trap")) < 0)
   return;

  ch->move -= cost;
  ch->in_room->trapped_by = str_dup(ch->name);
  af.type = sn;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = number_range(4,10);
  af.bitvector = AFF_ROOM_DART;
  affect_to_room(ch->in_room, &af);
  act("A dart trap has been set.", ch, NULL, NULL, TO_ALL);
  WAIT_STATE(ch,ws);
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
   INC_POWERS(ch, SHADOW_LEV_TRAP);
  if( GET_POWERS(ch, SHADOW_LEV_TRAP) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRAP) )
   && GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   stc("Your mastery of trap has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_TRAP) = 0;
   INC_POWERS(ch, SHADOW_TRAP);
  }
  return;
 }
 else if( !str_cmp(arg, "poison") )
 {
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   cost = 6000;
   ws = 14;
  }
  else
  {
   cost = 3000;
   ws = 7;
  }
  if( GET_POWERS(ch, SHADOW_TRAP) < 2 )
  {
   stc("You do not know how to set a poison trap!\n\r", ch);
   return;
  }
  if( ch->move < cost || ch->mana < (cost/2) )
  {
   stc("You are too tired to create a poison trap.\n\r", ch);
   return;
  }
  if( (sn = skill_lookup("poison_trap")) < 0)
   return;

  ch->move -= cost;
  ch->mana -= (cost/2);
  ch->in_room->trapped_by = str_dup(ch->name);
  af.type = sn;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = number_range(4,10);
  af.bitvector = AFF_ROOM_POISON;
  affect_to_room(ch->in_room, &af);
  act("A poison trap has been set.", ch, NULL, NULL, TO_ALL);
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
   INC_POWERS(ch, SHADOW_LEV_TRAP);
  if( GET_POWERS(ch, SHADOW_LEV_TRAP) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRAP) )
   && GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   stc("Your mastery of trap has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_TRAP) = 0;
   INC_POWERS(ch, SHADOW_TRAP);
  }
  return;
  WAIT_STATE(ch,ws);
 }
 else if( !str_cmp(arg, "explosion") )
 {
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   cost = 8000;
   ws = 16;
  }
  else
  {
   cost = 4000;
   ws = 8;
  }
  if( GET_POWERS(ch, SHADOW_TRAP) < 3 )
  {
   stc("You do not know how to set an explosion trap!\n\r", ch);
   return;
  }
  if( ch->move < cost || ch->mana < (cost/2) )
  {
   stc("You are too tired to create an explosion trap.\n\r", ch);
   return;
  }
  if( (sn = skill_lookup("explosion_trap")) < 0)
   return;
   
  ch->move -= cost;
  ch->mana -= (cost/2);
  ch->in_room->trapped_by = str_dup(ch->name);
  af.type = sn;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = number_range(4,10);
  af.bitvector = AFF_ROOM_EXPLOSION;
  affect_to_room(ch->in_room, &af);
  act("An explosion trap has been set.", ch, NULL, NULL, TO_ALL);
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
   INC_POWERS(ch, SHADOW_LEV_TRAP);
  if( GET_POWERS(ch, SHADOW_LEV_TRAP) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRAP) )
   && GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   stc("Your mastery of trap has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_TRAP) = 0;
   INC_POWERS(ch, SHADOW_TRAP);
  }
  return;
  WAIT_STATE(ch,ws);
 }
 else if( !str_cmp(arg, "sleep") )
 { 
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   cost = 10000;
   ws = 18;
  }
  else
  {
   cost = 5000;
   ws = 9;
  }
  if( GET_POWERS(ch, SHADOW_TRAP) < 4 )
  {
   stc("You do not know how to set a sleep trap!\n\r", ch);
   return;
  }
  if( ch->move < cost || ch->mana < (cost/2) )
  {
   stc("You are too tired to create a sleep trap.\n\r", ch);
   return;
  }
  if( (sn = skill_lookup("sleep_trap")) < 0)
   return;
 
  ch->move -= cost;
  ch->mana -= (cost/2);
  ch->in_room->trapped_by = str_dup(ch->name);
  af.type = sn;
  af.modifier = 0;
  af.location = APPLY_NONE;
  af.duration = number_range(4,10);
  af.bitvector = AFF_ROOM_SLEEP;
  affect_to_room(ch->in_room, &af);
  act("An sleep trap has been set.", ch, NULL, NULL, TO_ALL);
  if( GET_POWERS(ch, SHADOW_TRAP) < 5 )
   INC_POWERS(ch, SHADOW_LEV_TRAP);
  if( GET_POWERS(ch, SHADOW_LEV_TRAP) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRAP) )
   && GET_POWERS(ch, SHADOW_TRAP) < 5 )
  {
   stc("Your mastery of trap has increased!\n\r", ch);
   GET_POWERS(ch, SHADOW_LEV_TRAP) = 0;
   INC_POWERS(ch, SHADOW_TRAP);
  }
  return;
  WAIT_STATE(ch,ws);
 }
 else
 {
  stc("What kind of trap do you wish to set? (Dart, Poison, Explosion, Sleep)\n\r", ch );
  return;
 }
return;
}

OBJ_DATA *get_eye( CHAR_DATA *ch)
{
 OBJ_DATA *obj;
 bool found = FALSE;

 if( IS_NPC(ch))
  return NULL;
 if( !IS_CLASS(ch, CLASS_SHADOW))
  return NULL;
 if( ch->pcdata->familiar == NULL)
  return NULL;
 for( obj = object_list; obj && !found; obj = obj->next)
 {
  if( obj == ch->pcdata->familiar)
  {
   found = TRUE;
   return obj;
  }
 }
 return NULL;
}

void do_thirdeye( CHAR_DATA *ch, char *argument )
{
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 ROOM_INDEX_DATA *room;

 argument = one_argument( argument, arg );

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 else if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_TRACK) < 5 )
 {
  stc("You require full mastery of tracking before you can use thirdeye.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Do you wish to create, view, or destroy your thirdeye?\n\r", ch);
  return;
 }
 if( !str_cmp(arg, "create") )
 {
  if( ch->mana < 5000 )
  {
   stc("You require at least 5000 mana to create a thirdeye.\n\r", ch);
   return;
  }
  if( ch->pcdata->familiar != NULL )
  {
   stc("You already have an existing thirdeye.\n\r", ch);
   return;
  }
  stc("You focus your mental energies momentarily and create a small ball of pure thought.\n\r", ch);
  obj = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
  obj_to_char(obj,ch);
  obj->questowner = str_dup(ch->name);
  ch->pcdata->familiar = obj;
  ch->mana -= 5000;
  return;
 }
 else if( !str_cmp(arg, "view") )
 {
  if( (obj = get_eye(ch)) == NULL )
  {
   stc("You have no thirdeye to view.\n\r", ch);
   return;
  }
  if( obj->in_obj != NULL )
  {
   stc("You are unable to locate your thirdeye.\n\r", ch);
   return;
  }
  room = ch->in_room;
  char_from_room(ch);
  if( obj->in_room == NULL && obj->carried_by->in_room != NULL )
   char_to_room(ch, obj->carried_by->in_room);
  else if( obj->carried_by == NULL && obj->in_room != NULL )
   char_to_room(ch, obj->in_room);
  if( IS_SET(ch->affected_by, AFF_SHADOWPLANE))
  {
   REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
   do_look(ch, "auto");
   SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
  }
  else if( IS_SET(ch->affected_by, AFF_ETHEREAL) )
  {
   REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
   do_look(ch, "auto");
   SET_BIT(ch->affected_by, AFF_ETHEREAL);
  }
  else
   do_look(ch, "auto");
  char_from_room(ch);
  char_to_room(ch, room);
 }
 else if( !str_cmp(arg, "destroy") )
 {
  if( (obj = get_eye(ch)) == NULL )
  {
   stc("You have no thirdeye to destroy.\n\r", ch);
   return;
  }
  extract_obj(obj);
 }
return;
}

void do_tumble( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA       *victim;
 ROOM_INDEX_DATA *to_room;
 EXIT_DATA       *pexit;
 EXIT_DATA       *pexit_rev;
 char            buf       [MAX_INPUT_LENGTH];
 char            direction [MAX_INPUT_LENGTH];
 char            arg      [MAX_INPUT_LENGTH];
 int             door;
 int             rev_dir;
 int		 cost = 6000;

 argument = one_argument( argument, arg );

 if( IS_NPC(ch) )
  return;

 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;

 else if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_BACKSTAB) < 5 )
 {
  stc("You require full mastery of backstabbing before you can tumble.\n\r", ch);
  return;
 }

 if( IS_AFFECTED(ch,AFF_WEBBED) )
 {
  stc("Not with all this sticky webbing on!\n\r",ch);
  return;
 }

 if( ch->move < cost )
 {
  stc("You are too tired to tumble!\n\r", ch);
  return;
 }

 if( ch->fighting == NULL )
 {
  stc( "You need to be fighting in order to tumble.\n\r", ch );
  return;
 }

 if( ch->fighting != NULL )
  victim = ch->fighting;

 if( victim == NULL )
 {
  stc("They're not here.\n\r", ch);
  return;
 }
    
 if ( victim == ch ) 
 {
  stc( "You can't tumble yourself.\n\r", ch );
  return;
 }
     
 WAIT_STATE( ch, 6 );          

 rev_dir = 0;
                
 if ( arg[0] == '\0' )
  door = number_range(0,3);

 else
 {
  if( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) )
   door = 0;
  else if( !str_cmp( arg, "e" ) || !str_cmp( arg, "east" ) )
   door = 1;
  else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) )
   door = 2;
  else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west" ) )
   door = 3;
  else
  {
   stc("You can only tumble people north, south, east or west.\n\r", ch);
   return;
  }
 }

 if (door == 0)
 {       
  sprintf(direction,"north");
  rev_dir = 2;
 }
 if (door == 1)
 {
  sprintf(direction,"east");
  rev_dir = 3;
 }
 if (door == 2)
 {
  sprintf(direction,"south");
  rev_dir = 0;
 }
 if (door == 3)
 {   
  sprintf(direction,"west");
  rev_dir = 1;
 }

 if( number_range(1,100) > (GET_POWERS(ch,SHADOW_BACKSTAB) * 14) )
 {
  act("You attempt to tumble $N, but miss!",ch,NULL,victim,TO_CHAR);
  act("$n attempts to tumble $N, but misses!",ch,NULL,victim,TO_NOTVICT);
  act("$n attempts to tumble you, but misses!",ch,NULL,victim,TO_VICT);
  WAIT_STATE(ch,12);
  return;
 }

 if( ( pexit = ch->in_room->exit[door] ) == NULL || ( to_room = pexit->to_room ) == NULL) 
 {       
  sprintf(buf,"$n tumbles $N into the %s wall.", direction);
  act(buf,ch,NULL,victim,TO_NOTVICT);
  sprintf(buf,"You tumble $N into the %s wall.", direction);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n tumbles you into the %s wall.", direction);
  act(buf,ch,NULL,victim,TO_VICT);
  if( can_stun(ch, victim) )
  	set_stun(victim,2);
  stop_fighting(ch,TRUE);
  stop_fighting(victim,TRUE);
  update_pos(victim);
  return;
 }

 pexit = victim->in_room->exit[door];
 if( IS_SET(pexit->exit_info, EX_CLOSED) && (ch->in_room->vnum >= 26000 && ch->in_room->vnum <= 26206) )
 {
   stc("They ricochet off the closed exit and land on your dumb ass!\n\r", ch);
   ch->hit -= 5000;
   set_stun(ch,1);
   update_pos(ch);
   return;
 } 
 if( IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim, AFF_PASS_DOOR) && !IS_AFFECTED(victim, AFF_ETHEREAL) )
 {       
  if (IS_SET(pexit->exit_info, EX_LOCKED))
   REMOVE_BIT(pexit->exit_info, EX_LOCKED);
  if (IS_SET(pexit->exit_info, EX_CLOSED))
   REMOVE_BIT(pexit->exit_info, EX_CLOSED);
  sprintf(buf,"$n maneuvers around $N and tumbles $M %s.", direction);
  act(buf,ch,NULL,victim,TO_NOTVICT);
  sprintf(buf,"You maneuver around $N and tumble $M %s.", direction);
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n maneuvers around you and tumbles you %s as you smash through the $d", direction);
  act(buf,ch,NULL,victim,TO_VICT);
  sprintf(buf,"There is a loud crash as $n smashes through the $d.");
  act(buf,victim,NULL,pexit->keyword,TO_ROOM);

  if( ( to_room = pexit->to_room ) != NULL && ( pexit_rev = to_room->exit[rev_dir] ) != NULL
   && pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL )
  {
   if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
   if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
   if (door == 0)
    sprintf(direction,"south");
   if (door == 1)
    sprintf(direction,"west");
   if (door == 2)
    sprintf(direction,"north"); 
   if (door == 3)
    sprintf(direction,"east");  

   char_from_room(victim);
   char_to_room(victim,to_room);  
   sprintf(buf,"$n comes smashing in through the %s $d.", direction);
   act(buf,victim,NULL,pexit->keyword,TO_ROOM);
  }
 }

 else
 {
  sprintf(buf,"$n maneuvers around $N and tumbles $M %s.", direction);  
  act(buf,ch,NULL,victim,TO_NOTVICT);
  sprintf(buf,"You maneuver around $N and tumble $M %s.", direction);  
  act(buf,ch,NULL,victim,TO_CHAR);
  sprintf(buf,"$n maneuvers around you and tumbles you %s.", direction);
  act(buf,ch,NULL,victim,TO_VICT);

  if (door == 0)
   sprintf(direction,"south");
  if (door == 1)
   sprintf(direction,"west");
  if (door == 2)
   sprintf(direction,"north");
  if (door == 3)
   sprintf(direction,"east");

  char_from_room(victim);
  char_to_room(victim,to_room);
  sprintf(buf,"$n comes tumbling in from the %s.", direction);
  act(buf,victim,NULL,NULL,TO_ROOM);
 }
 ch->move -= cost;
 WAIT_STATE(victim,10);
 WAIT_STATE(ch,12);
 stop_fighting(ch,TRUE);
 check_trap(victim);
 return;
}

void do_circle( CHAR_DATA * ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int chance = 0;
 bool go = FALSE;
 char buf[MAX_STRING_LENGTH];
 int dam = 0;

 one_argument (argument, arg);

 if( IS_NPC (ch) )
  return;
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_BACKSTAB) < 2 )
 {
  stc("You require a level 2 mastery of backstab to use circle.\n\r", ch);
  return;
 }
 if( ch->move < 500 )
 {
  stc("You are too tired to circle.\n\r", ch);
  return;
 }
 if( IS_AFFECTED(ch,AFF_WEBBED) )
 {
  stc("Not with all this sticky webbing on!\n\r",ch);
  return;
 }
 if( ch->fighting == NULL )
 {
  stc( "You need to be fighting in order to circle.\n\r", ch );
  return;
 }
 else if( ch->fighting != NULL )
  victim = ch->fighting;
 if( victim == ch )
 {
  stc( "How can you circle yourself?\n\r", ch );
  return;
 }
 if( is_safe ( ch, victim ) )
  return;

 check_killer(ch,victim);

 if( GET_POWERS(ch, SHADOW_BACKSTAB) < 4 )
  WAIT_STATE(ch,18);
 else
  WAIT_STATE(ch,9);

 chance = number_range(5,10)+GET_POWERS(ch, SHADOW_BACKSTAB);
 dam = number_range(300,500);

 if( GET_POWERS(ch, SHADOW_BACKSTAB) == 5 )
  dam *= 2;
 if( number_range(1,100) < chance )
 {
  go = TRUE;
  dam *= 2;
 }

 sprintf( buf, "You circle behind $N and plunge your weapon deep into $S back! [%d]", dam );
 act( buf, ch, NULL, victim, TO_CHAR    );
 sprintf( buf, "$n circles behind you and plunges $s weapon deep into your back! [%d]", dam );
 act( buf, ch, NULL, victim, TO_VICT    ); 
 sprintf( buf, "$n circles behind $N and plunges $s weapon deep into $N's back! [%d]", dam );
 act( buf, ch, NULL, victim, TO_NOTVICT );

 if( go )   
 {
  sprintf(buf, "You hear a loud CRACK!");
  do_recho(ch,buf);
 }
 
 ch->move -= 500;
 hurt_person(ch,victim,dam);
 update_pos(victim);

 return;
}

void do_shadow( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 one_argument( argument, arg );

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 else if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_TRACK) < 5 )
 {
  stc("You require full mastery of tracking before you can use shadow.\n\r", ch);
  return;
 }
 if ( arg[0] == '\0' )
 {
  stc( "Shadow whom?\n\r", ch );
  return;
 }

 if ( ( victim = get_char_room( ch, arg ) ) == NULL )
 {
  stc( "They aren't here.\n\r", ch );
  return;
 }
    
 if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
 {
  act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
  return;
 }
 
 if ( victim == ch )
 {
  if ( ch->master == NULL )
  {
   if( IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_FOLLOW) )
    REM_BIT_POWERS(ch, SHADOW_BITS, SHADOW_FOLLOW);
   GET_POWERS(ch, SHADOW_COUNTER) = 0;
   stc( "You are not shadowing anyone.\n\r", ch );
   return;
  }
  stop_follower( ch );
  return;
 }

 if ( ch->master != NULL )
  stop_follower( ch );
 SET_BIT_POWERS(ch,SHADOW_BITS,SHADOW_FOLLGO);
 add_follower( ch, victim );
 return;
}

void do_mastery( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];

 argument = one_argument(argument, arg); 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 if( arg[0] == '\0' && ch->in_room->vnum != 3029 )
 {
  display_class_stats(ch);
  return;
 }
 else if( arg[0] == '\0' && ch->in_room->vnum== 3029 )
 {
  if( GET_POWERS(ch, SHADOW_BACKSTAB < 1) || GET_POWERS(ch, SHADOW_TRAP) < 1 || GET_POWERS(ch, SHADOW_RUNE) < 1 || GET_POWERS(ch, SHADOW_TRACK) < 1 )
   stc("The guildmaster says '#3What would you like for me to teach you?'.#n\n\r", ch);
  else
   stc("The guildmaster says '#3You have already learned all that I can teach.'.#n\n\r", ch);
 }
 else if( !str_cmp(arg, "learn") && ch->in_room->vnum != 3029 )
 {
  stc("You can only learn new skills in the presence of your guildmaster.\n\r", ch);
  return;
 }
 else if( !str_cmp(arg, "learn") && ch->in_room->vnum == 3029 )
 {
  char arg2[MAX_INPUT_LENGTH];
  argument = one_argument(argument,arg2);
  if( arg2[0] == '\0' )
  {
   stc("The guildmaster says '#3I can teach you the arts of backstabbing, trapping, runes, and tracking.'.#n\n\r", ch);
   return;
  }
  if( !str_cmp(arg2, "backstab") )
  {
   stc("The guildmaster says '#3So I see you wish to learn the art of backstabbing?.'.#n\n\r", ch);
   if( GET_POWERS(ch, SHADOW_BACKSTAB) > 1 )
   {
    stc("The guildmaster says '#3You already know what I can teach, and must practice it on your own.'.#n\n\r", ch);
    return;
   }
   else
   {
    if( ch->practice < SHADOW_COST )
    {
     stc("The guildmaster says '#3I'm sorry, but you lack the primal energy to learn this.'.#n\n\r", ch);
     return;
    }
    else
    {
     stc("The guildmaster says '#3Very well, I shall teach you.'.#n\n\r", ch);
     ch->practice -= SHADOW_COST;
     INC_POWERS(ch, SHADOW_BACKSTAB);
     return;
    }
   }
  }
  else if( !str_cmp(arg2, "trap") )
  {
   stc("The guildmaster says '#3So I see you wish to learn the art of trapping?.'.#n\n\r", ch);
   if( GET_POWERS(ch, SHADOW_TRAP) > 1 )
   {
    stc("The guildmaster says '#3You already know what I can teach, and must practice it on your own.'.#n\n\r", ch);
    return;
   }
   else  
   {
    if( ch->practice < SHADOW_COST )
    {
     stc("The guildmaster says '#3I'm sorry, but you lack the primal energy to learn this.'.#n\n\r", ch);
     return;
    }
    else
    {
     stc("The guildmaster says '#3Very well, I shall teach you.'.#n\n\r", ch);
     ch->practice -= SHADOW_COST;
     INC_POWERS(ch, SHADOW_TRAP);
     return;
    }
   }
  } 
  else if( !str_cmp(arg2, "rune") )
  {
   stc("The guildmaster says '#3So I see you wish to learn the art of runes?.'.#n\n\r", ch);    
   if( GET_POWERS(ch, SHADOW_RUNE) > 1 )    
   {
    stc("The guildmaster says '#3You already know what I can teach, and must practice it on your own.'.#n\n\r", ch);
    return;
   }
   else
   { 
    if( ch->practice < SHADOW_COST )
    {
     stc("The guildmaster says '#3I'm sorry, but you lack the primal energy to learn this.'.#n\n\r", ch);
     return;
    }
    else
    {
     stc("The guildmaster says '#3Very well, I shall teach you.'.#n\n\r", ch);
     ch->practice -= SHADOW_COST;
     INC_POWERS(ch, SHADOW_RUNE);    
     return;
    }
   }
  }
  else if( !str_cmp(arg2, "tracking") )
  {
   stc("The guildmaster says '#3So I see you wish to learn the art of tracking?.'.#n\n\r", ch);    
   if( GET_POWERS(ch, SHADOW_TRACK) > 1 )    
   {
    stc("The guildmaster says '#3You already know what I can teach, and must practice it on your own.'.#n\n\r", ch);
    return;
   }
   else
   { 
    if( ch->practice < SHADOW_COST )
    {
     stc("The guildmaster says '#3I'm sorry, but you lack the primal energy to learn this.'.#n\n\r", ch);
     return;
    }
    else
    {
     stc("The guildmaster says '#3Very well, I shall teach you.'.#n\n\r", ch);
     ch->practice -= SHADOW_COST;
     INC_POWERS(ch, SHADOW_TRACK);    
     return;
    }
   }
  }
 }
 if( !str_cmp( arg, "backstab" ) )
 {
  if( GET_POWERS(ch, SHADOW_BACKSTAB) < 1 )
   sprintf( buf, "You have no knowledge of backstabbing.\n\r" );
  else if( GET_POWERS(ch, SHADOW_BACKSTAB) > 0 && GET_POWERS(ch, SHADOW_BACKSTAB) < 5 )
   sprintf( buf, "You have a level %d mastery of backstabbing.\n\r", GET_POWERS(ch, SHADOW_BACKSTAB) );
  else
   sprintf( buf, "You have mastered the art of backstabbing.\n\r" );
  stc( buf, ch );
  if( GET_POWERS(ch, SHADOW_BACKSTAB) > 0 )
   stc( "   You can use the backstab technique.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_BACKSTAB) > 1 )
   stc( "   You can use the circle technique.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_BACKSTAB) > 2 )
   stc( "   You are adept in backstabbing, and perform it quicker.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_BACKSTAB) > 3 )
   stc( "   You are adept in circling, and perform it quicker.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_BACKSTAB) > 4 )
   stc( "   You have perfected your art, granting enhanced damage and the art of tumbling.\n\r", ch );
  return;
 }
 else if( !str_cmp( arg, "trap" ) )
 {
  if( GET_POWERS(ch, SHADOW_TRAP) < 1 )
   sprintf( buf, "You have no knowledge of trapping.\n\r" );
  else if( GET_POWERS(ch, SHADOW_TRAP) > 0 && GET_POWERS(ch, SHADOW_TRAP) < 5 )
   sprintf( buf, "You have a level %d mastery of trapping.\n\r", GET_POWERS(ch, SHADOW_TRAP) );
  else
   sprintf( buf, "You have mastered the art of trapping.\n\r" );
  stc( buf, ch );
  if( GET_POWERS(ch, SHADOW_TRAP) > 0 )
   stc( "   You can create a dart trap.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRAP) > 1 )
   stc( "   You can create a poison trap.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRAP) > 2 )
   stc( "   You can create an explosion trap.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRAP) > 3 )
   stc( "   You can create a sleep trap.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRAP) > 4 )
   stc( "   You have perfected your art, enabling you to perform it more swiftly with a reduced cost.\n\r", ch );
  return;
 }
 else if( !str_cmp( arg, "rune" ) )
 {
  if( GET_POWERS(ch, SHADOW_RUNE) < 1 )
   sprintf( buf, "You have no knowledge of runes.\n\r" );
  else if( GET_POWERS(ch, SHADOW_RUNE) > 0 && GET_POWERS(ch, SHADOW_RUNE) < 5 )
   sprintf( buf, "You have a level %d mastery of runes.\n\r", GET_POWERS(ch, SHADOW_RUNE) );
  else
   sprintf( buf, "You have mastered the art of runes.\n\r" );
  stc( buf, ch );
  if( GET_POWERS(ch, SHADOW_RUNE) > 0 )
   stc( "   You know how to weave a healing rune.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_RUNE) > 1 )
   stc( "   You know how to weave a negativity rune.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_RUNE) > 2 )
   stc( "   You know how to weave a drain rune.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_RUNE) > 3 )
   stc( "   You have improved your rune designs, making them cheaper and more effective.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_RUNE) > 4 )
   stc( "   You have perfected your art, and mastered the nullify rune.\n\r", ch );
  return;
 }
 else if( !str_cmp( arg, "tracking" ) )
 {
  if( GET_POWERS(ch, SHADOW_TRACK) < 1 )
   sprintf( buf, "You have no knowledge of tracking.\n\r" );
  else if( GET_POWERS(ch, SHADOW_TRACK) > 0 && GET_POWERS(ch, SHADOW_TRACK) < 5 )
   sprintf( buf, "You have a level %d mastery of tracking.\n\r", GET_POWERS(ch, SHADOW_TRACK) );
  else
   sprintf( buf, "You have mastered the art of tracking.\n\r" );
  stc( buf, ch );
  if( GET_POWERS(ch, SHADOW_TRACK) > 0 )
   stc( "   You can use the conceal technique.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRACK) > 1 )
   stc( "   You can track your enemies more swiftly.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRACK) > 2 )
   stc( "   You leave no trace when moving.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRACK) > 3 )
   stc( "   You have further mastered the art of tracking your enemies.\n\r", ch );
  if( GET_POWERS(ch, SHADOW_TRACK) > 4 )
   stc( "   You have perfected your art, granting the arts of shadow, thirdeye, and scry.\n\r", ch );
  return;
 }
return;
}

void conceal_count( CHAR_DATA *ch )
{
  stc("You slowly fade into existence.\n\r", ch);
  REM_BIT_POWERS(ch, SHADOW_BITS, SHADOW_INVIS);
  act("$n slowly fades into existence.", ch, NULL, NULL, TO_NOTVICT);
  return;
}

void do_conceal( CHAR_DATA *ch, char *argument )  
{
  
 if( IS_NPC(ch) )
  return;
 if( IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_vamp_conceal(ch,"");
  return;
 }
 if( !IS_CLASS(ch, CLASS_SHADOW) )
  return;
 if( IS_CLASS(ch, CLASS_SHADOW) && GET_POWERS(ch, SHADOW_TRACK) < 1 )
 {
  stc("You require level 1 in tracking before you can use conceal.\n\r", ch);
  return;
 }
 if( ch->mana < 1000 )
 {
  stc("You lack the magical stamina to cloak yourself.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 0)
 {
  stc("Not with a fight timer.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch, SHADOW_BITS), SHADOW_INVIS) )
 {
  conceal_count(ch);
  return;
 }
 else
 {
  stc("You slowly blend in with your surroundings.\n\r", ch);
  act("$n slowly blends in with $s surroundings.", ch, NULL, NULL, TO_NOTVICT);
  SET_BIT_POWERS(ch, SHADOW_BITS, SHADOW_INVIS);
  ch->mana -= 1000;
 }
 if( GET_POWERS(ch, SHADOW_TRACK) < 5 )
  INC_POWERS(ch, SHADOW_LEV_TRK);
 if( GET_POWERS(ch, SHADOW_LEV_TRK) >= ( SHADOW_LEV_GAIN * GET_POWERS(ch, SHADOW_TRACK) )
  && GET_POWERS(ch, SHADOW_TRACK) < 5 )
 {
  stc("Your mastery of tracking has increased!\n\r", ch);
  GET_POWERS(ch, SHADOW_LEV_TRK) = 0;
  INC_POWERS(ch, SHADOW_TRACK);
 }
 return;
} 
