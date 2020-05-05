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
void update_drag_ac( CHAR_DATA *ch);
void might_mult(CHAR_DATA *ch, int damage);
void age_mult(int amult, int dam);
/* commands for dragons
   dragongain
   frieghten
   terror
   d_high_claw
   d_low_claw
   dtaillash
   dkick
   dbite
   dragonbreath
*/
/* valid combos
   1 - high claw, high claw
   2 - high claw, low claw, high claw
   3 - low claw, low claw, bite, tail, kick
   4 - bite, high claw, low claw
   5 - kick, tail, high claw, bite
*/
#define DRAGON_LAG 6
void might_mult(CHAR_DATA *ch, int damage)
{
  switch( ch->pcdata->powers[DRAGON_MIGHT])
  {
    default: break;
    case 1: damage = damage * 1.1;
            break;
    case 2: damage = damage * 1.15;
            break;
    case 3: damage = damage * 1.2;
            break;
    case 4: damage = damage * 1.25;
            break;
    case 5: damage = damage * 1.3;
            break;
    case 6: damage = damage * 1.35;
            break;
    case 7: damage = damage * 1.4;
            break;
    case 8: damage = damage * 1.45;
            break;
    case 9: damage = damage * 1.5;
            break;
    case 10: damage = damage * 1.6;
            break;
    return;
   }
 return;
}

void age_mult(int amult, int dam)
{
  if( amult < 1) /* i would so a switch, but that would be a lot of case statements */
    dam = dam * 1.1;
  if( (amult >= 2) && (amult < 3)) /* 200 yrs */
    dam = dam * 1.2;
  if( (amult >= 3) && (amult < 4)) /* 300 yrs */
    dam = dam * 1.3;
  if( (amult >= 4) && (amult < 5)) /* 400 yrs */
    dam = dam * 1.4;
  if( (amult >= 5) && (amult < 6)) /* 500 yrs */
    dam = dam * 1.5;
  if( (amult >= 6) && (amult < 7)) /* 600 yrs */
    dam = dam * 1.7;
  if( (amult >= 7) && (amult < 8)) /* 700 yrs */
    dam = dam * 1.8;
  if( (amult >= 8) && (amult < 9)) /* 800 yrs */
    dam = dam * 1.9;
  if( (amult >= 9) && (amult < 10)) /* 900 yrs */
    dam = dam * 2;
  if( (amult >=10) && (amult < 11)) /* 1000 yrs */
    dam = dam * 2.1;
  if( (amult >=15) && (amult < 20)) /* 1500 yrs */
    dam = dam * 2.3;
  if( (amult >= 20) && (amult < 25)) /* 2000 yrs */
    dam = dam * 2.5;
  if( (amult >= 25) && (amult < 30)) /* 2500 yrs */
    dam = dam * 2.7;
  if( amult > 30)               /* 3000 yrs +*/
    dam = dam *3;
  return;
}

void update_drag_ac( CHAR_DATA *ch)
{
  int value = ch->pcdata->powers[DRAGON_AGE];
  if( (value > 0) && (value < 100))
   ch->armor -= 15;
  if( (value >= 100) && (value < 500))
   ch->armor -= 20;
  else
   ch->armor -=25;
return;
} 
 
void do_dragon_gain( CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int cost;
  int max;
  int improve;
  bool up_ac;
  bool blah = FALSE;
  argument = one_argument( argument, arg);

  if(IS_NPC(ch))
  return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->level < 3)
  {
   send_to_char("You must be avatar to do this.\n\r", ch);
   return;
  }
  if( !str_cmp( arg, "fear"))
  {
    max = 4;
    cost = (1+ch->pcdata->powers[DRAGON_FEAR]) * 20;
    improve = DRAGON_FEAR;
    blah = TRUE;
  }
  else if( !str_cmp( arg, "torment"))
  {
   max = 5;
   cost =(1+ ch->pcdata->powers[DRAGON_TORMENT]) * 20;
   improve = DRAGON_TORMENT;
   blah = TRUE;
  }
  else if( !str_cmp( arg, "flame"))
  {
   max = 10;
   cost = (1+ch->pcdata->powers[DRAGON_FLAME]) * 20;
   improve = DRAGON_FLAME;
   blah = TRUE;
  }
  else if( !str_cmp( arg, "might"))
  {
   max = 10;
   cost = (1+ch->pcdata->powers[DRAGON_MIGHT]) * 20;
   improve = DRAGON_MIGHT;
   blah = TRUE;
  }
  else if( !str_cmp( arg, "fury"))
  {
   max = 10;
   cost = (1+ch->pcdata->powers[DRAGON_FURY]) * 20;
   improve = DRAGON_FURY;
   blah = TRUE;
  }
  else if( !str_cmp( arg, "age"))
  {
   max = 3000;
   cost = 40;
   improve = DRAGON_AGE;
  up_ac = TRUE;
  blah = TRUE;
  }
  else if( arg[0] == '\0')
  {
   sprintf( buf, "Fear(%d) Torment(%d) Flame(%d) Might(%d) Fury(%d) Age(%d).\n\r",
            ch->pcdata->powers[DRAGON_FEAR], ch->pcdata->powers[DRAGON_TORMENT],
            ch->pcdata->powers[DRAGON_FLAME],ch->pcdata->powers[DRAGON_MIGHT],
            ch->pcdata->powers[DRAGON_FURY], ch->pcdata->powers[DRAGON_AGE]);
   send_to_char( buf, ch);
  }
  else
   return;
  if( blah && ch->pcdata->powers[improve] >= max)
  {
   send_to_char("You have already maxed that discipline.\n\r", ch);
   return;
  }
  if( ch->practice < cost && arg[0] != '\0' )
  {
   send_to_char("You cant afford it.\n\r", ch);
   return;
  }
  if( arg[0] != '\0' && (blah = TRUE) )
  {
//  if( up_ac)
//  update_drag_ac(ch);
  ch->pcdata->powers[improve] +=1;
  ch->practice -= cost;
  send_to_char("Your powers ascend to the next level.\n\r", ch);
  }
  return;
}

void do_frighten( CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA *was_in;
  ROOM_INDEX_DATA *now_in;
  CHAR_DATA *victim;
  int cost = 5000;
  int attempt;
 
  if(!IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if(ch->pcdata->powers[DRAGON_FEAR] < 1)
  {
   send_to_char("You need at least 1 point of Dragon Fear.\n\r", ch);
   return;
  }
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( is_safe(ch, victim))
  {
   stc("Not while safe.\n\r", ch);
   return;
  }
  if( ch->move < cost)
  {
    send_to_char("You need 5000 move.\n\r", ch);
    return;
  }
  if( number_percent() < 25)
  {
    send_to_char("You failed!\n\r", ch);
    act("$n roars loudly at $N, but fails to scare $S\n\r", ch, NULL, victim, TO_ROOM);
    return;
  }
  was_in = ch->in_room;
  if( (victim->rage > 250))
  {
   send_to_char("They are to wild to be frightened from combat.\n\r", ch);
   return;
  }
  for ( attempt = 0; attempt < 6; attempt++ )
  {  
   EXIT_DATA *pexit;
   int door;
   door = number_door( );
   if( ( pexit = was_in->exit[door] ) == 0 
   || pexit->to_room == NULL
   || (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim, AFF_PASS_DOOR)) 
   || ( IS_NPC(victim) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
       continue;
   move_char( victim, door );
   if( ( now_in = victim->in_room ) == was_in )
    continue;
   victim->in_room = was_in;
   victim->in_room = now_in;
   if( !IS_NPC(victim) )
    send_to_char( "You have been frightened from combat!\n\r", victim);
   WAIT_STATE(victim,DRAGON_LAG);
   WAIT_STATE(ch, DRAGON_LAG);
   ch->move -= cost;
   act("$n scares $N from combat.\n\r", ch, NULL, victim, TO_NOTVICT);
    stop_fighting( victim, TRUE );
   return;
  }
   return;
}

void do_terror( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *vch;
 CHAR_DATA *vch_next;
 int attempt;
 int cost = 15;
 ROOM_INDEX_DATA *was_in;
 ROOM_INDEX_DATA *now_in;
 if(IS_NPC(ch)) 
   return;
 if( ch->practice < cost )
 {
  send_to_char("You need 15 primal.\n\r", ch);
  return;
 }
 if( ch->pcdata->powers[DRAGON_FEAR] < 2)
 {
   send_to_char("You need at least 2 in Dragon Fear.\n\r", ch);
   return;
 }
 for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
 {
   vch_next = vch->next_in_room;
   was_in = ch->in_room;
   now_in = ch->in_room;
  if( vch == ch)
  continue;
  if( is_safe(ch, vch))
  continue;
  for ( attempt = 0; attempt < 6; attempt++ )
  {
    EXIT_DATA *pexit;
    int door;
    door = number_door( );
    if( ( pexit = was_in->exit[door] ) == 0
    || pexit->to_room == NULL
    || IS_SET(pexit->exit_info, EX_CLOSED)
    || (IS_NPC(vch) && IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
       continue;
   move_char( vch, door );
   if( ( now_in = vch->in_room ) == was_in )
    continue;
   vch->in_room = was_in;
   vch->in_room = now_in;
   if (!IS_NPC(vch))
    send_to_char( "You have been frightened from combat!\n\r", ch );
   WAIT_STATE(vch,DRAGON_LAG);
    stop_fighting( vch, TRUE );
  }         
 }
 WAIT_STATE(ch, DRAGON_LAG);
 ch->practice -= cost;
 return;
}

void do_dragon_high_claw( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int dam = number_range( 1200, 1550);
  int amult;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  argument = one_argument(argument, arg);
  if( (victim = ch->fighting) == NULL)
  {
   send_to_char("Claw who?\n\r", ch);
   return;
  }
  if(victim != ch->fighting)
  {
   send_to_char("They aren't here.\n\r", ch);
  return;   
  }
  if(ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( IS_NPC(ch))
   return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_TORMENT] <1)
  {
   send_to_char("You lack the skills to torment your foes.\n\r", ch);
   return;
  }
  if( ch->move < 200 )
  {
   send_to_char("You are too tired to do that.\n\r", ch);
   return;
  }
  ch->move -= 200;
  might_mult( ch, dam);
  amult = ch->pcdata->powers[DRAGON_AGE]/100;
  age_mult( amult, dam);
  sprintf( buf,"You slash $N with your claws. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_CHAR);
  sprintf( buf, "$n slashes at $N's head with their claws. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  sprintf( buf,"$n swipes your head with sharp claws. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_VICT);
  if( victim->stance[0] == STANCE_TIGER)
   dam = dam * 1.5;
  else if( victim->stance[0] == STANCE_SWALLOW)
   dam = dam * 0.8;
  victim->hit -= dam;
  update_pos(victim);
  WAIT_STATE(ch, DRAGON_LAG);
  return;
}

void do_dragon_low_claw( CHAR_DATA *ch, char *argument)
{
  int dam = number_range( 1200, 1550);
  CHAR_DATA *victim;
  int amult = ch->pcdata->powers[DRAGON_AGE] / 100;
  char buf[MAX_STRING_LENGTH];
   if((victim = ch->fighting) == NULL)
  { 
   send_to_char( "They aren't here.\n\r", ch );
   return;
  } 
  if(ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  } 
  if( IS_NPC(ch))
   return;
  if( !IS_DRAGON(ch))
  { 
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_TORMENT] < 2)
  { 
   send_to_char("You lack the skills to torment your foes.\n\r", ch);
   return;
  }
  if( ch->move < 200 )
  {
   send_to_char("You are too tired to do that.\n\r", ch);
   return;
  }
  ch->move -= 200;  
  might_mult( ch, dam);
  age_mult( amult, dam);
  sprintf( buf, "You slash at $N's legs with a low claw attack. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_CHAR);
  sprintf( buf, "$n slashes at your legs with a devastating low claw attack. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_VICT);
  sprintf( buf, "$n slashes at $N's legs with a devastating low claw attack. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  if( victim->stance[0] == STANCE_MANTIS)
   dam = dam * 1.5;
  if( victim->stance[0] == STANCE_DRAGON)
   dam = dam * 0.8;
  victim->hit -= dam;
  if( victim->hit < -10)
    victim->hit = -10;
  update_pos(victim);
  WAIT_STATE(ch, DRAGON_LAG);
  return; 
}

void do_dragon_taillash( CHAR_DATA *ch, char *argument)
{
  int dam = number_range( 1200, 1550);
  int amult = ch->pcdata->powers[DRAGON_AGE]/100;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];
   
  if((victim = ch->fighting) == NULL)
  {
   send_to_char( "They aren't here.\n\r", ch );
   return;
  }
  if(ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( IS_NPC(ch))
   return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_TORMENT] < 3)
  {
   send_to_char("You lack the skills to torment your foes.\n\r", ch);
   return;
  }
  if( ch->move < 500 )
  {
   send_to_char("You are too tired to do that.\n\r", ch);
   return;
  }
  ch->move -= 500;  
  might_mult( ch, dam);
  age_mult( amult, dam);
  WAIT_STATE(ch, DRAGON_LAG);
  sprintf( buf, "Your tail slaps $N in the face. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_CHAR);
  sprintf( buf, "You get smacked hard by $n's tail. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_VICT);
  sprintf( buf, "$N gets smacked hard by $n's tail. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  if( victim->stance[0] == STANCE_SWALLOW)
   dam = dam * 1.5;
  if( victim->stance[0] == STANCE_MONKEY)
   dam = dam * 0.8;
  victim->hit -= dam;
  update_pos(victim);
  return;
}

void do_dragon_kick( CHAR_DATA *ch, char *argument)
{
   int dam = number_range(1200, 1550);
   int amult = ch->pcdata->powers[DRAGON_AGE] /100;
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
  if((victim = ch->fighting) == NULL)
  {
   send_to_char( "They aren't here.\n\r", ch );
   return;
  }
  if(ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( IS_NPC(ch))
   return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_TORMENT] < 4)
  {
   send_to_char("You lack the skills to torment your foes.\n\r", ch);
   return;
  }
  if( ch->move < 500 )
  {
   send_to_char("You are to tired to do that.\n\r", ch);
   return;
  }
  ch->move -= 500;
  might_mult( ch, dam);
  age_mult( amult, dam);
  WAIT_STATE(ch, DRAGON_LAG);
  sprintf( buf, "You lash at $N with your hind legs. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_CHAR);
  sprintf( buf, "$n kicks you with a powerful hind leg. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_VICT);
  sprintf( buf, "$N gets kicked by $n's powerful hind legs. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  if( victim->stance[0] == STANCE_DRAGON)
   dam = dam * 1.5;
  if( victim->stance[0] == STANCE_TIGER)
   dam = dam * 0.8;
  victim->hit -= dam;
  update_pos(victim);
  return;
}

void do_dragon_bite( CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  int dam = number_range(1200, 1550);
  int amult = ch->pcdata->powers[DRAGON_AGE]/100;
  char buf[MAX_STRING_LENGTH];
  if((victim = ch->fighting) == NULL)
  {
   send_to_char( "They aren't here.\n\r", ch );
   return;
  }
  if(ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return;
  }
  if( IS_NPC(ch))
   return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_TORMENT] < 4)
  {
   send_to_char("You lack the skills to torment your foes.\n\r", ch);
   return;
  }
  if( ch->move < 500 )
  {
   send_to_char("You are to tired to do that.\n\r", ch);
   return;
  }
  ch->move -= 500;
  might_mult( ch, dam);
  age_mult( amult, dam);
  WAIT_STATE(ch, DRAGON_LAG);
  sprintf( buf, "You take a bite out of $N. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_CHAR);
  sprintf( buf, "$n open their huge maw and bites you hard. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_VICT);
  sprintf( buf, "$n takes a huge bite out of $N. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_NOTVICT);
  if( victim->stance[0] == STANCE_MONKEY)
   dam = dam * 1.5;
  if( victim->stance[0] == STANCE_MANTIS)
   dam = dam * 0.8;
  victim->hit -= dam;
  update_pos(victim);
  return;
}

void do_dragonbreath( CHAR_DATA *ch, char *argument)
{
  int dam = 0;
  int cost = 0;
  int i;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];

  if((victim = ch->fighting) == NULL)
  {
   send_to_char( "They aren't here.\n\r", ch );
   return;
  }
  if(ch->position != POS_FIGHTING)
  {
   send_to_char("You must be fighting.\n\r", ch);
   return; 
  }
  if( IS_NPC(ch))
   return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_FLAME] < 1)
  {
   send_to_char("You are not yet skilled in the arts of dragonbreath.\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[BREATH_TIMER] > 0)
  {
   send_to_char("You havent recovered from your last breath yet.\n\r", ch);
   return;
  }
  for(  i = 0; i != ch->pcdata->powers[DRAGON_FLAME]; i++)
  {
    cost += 350;
  }
  cost = number_range( cost-600, cost);
  if( cost < 1)
   cost = 1;
  dam = cost * 1.5;
  if( ch->mana < cost)
  {
   send_to_char("You lack the magical stamina to breath fire.\n\r", ch);
   return;
  }
  sprintf( buf, "You take a deep breath and cover $N in flame. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_CHAR);
  sprintf( buf, "$n takes a deep breath and covers you in burning dragonfire. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_VICT);
  sprintf( buf, "$n takes a deep breath and covers $N in dragonfire. [%d]\n\r", dam);
  act(buf, ch, NULL, victim, TO_NOTVICT);  
  ch->pcdata->powers[BREATH_TIMER] += 3;
  victim->hit -= dam;
  update_pos(victim);
  ch->mana -= cost;
  WAIT_STATE( ch, 24);
  return;
}

void do_bodyblock( CHAR_DATA *ch, char *argument)
{
  if(IS_NPC(ch))
  return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->pcdata->powers[DRAGON_MIGHT] < 6)
  {
   send_to_char("You arent strong enough.\n\r", ch);
   return;
  }
  if( !IS_SET(ch->newbits, NEW_JAWLOCK))
  {
   send_to_char("You begin to block your foes from fleeing.\n\r", ch);
   SET_BIT(ch->newbits, NEW_JAWLOCK);
   return;
  }
  if( IS_SET(ch->newbits, NEW_JAWLOCK))
  {
   send_to_char("You no longer block your foes from fleeing.\n\r", ch);
   REMOVE_BIT(ch->newbits, NEW_JAWLOCK);
   return;
  }
 return;
}

void do_wings( CHAR_DATA *ch, char *argument)
{
  if( IS_NPC(ch))
  return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( IS_AFFECTED( ch, AFF_FLYING))
  {
   REMOVE_BIT( ch->affected_by, AFF_FLYING);
   send_to_char("You keep your wings close to your body.\n\r", ch);
   return; 
  }
  if( !IS_AFFECTED(ch, AFF_FLYING))
  {
   SET_BIT( ch->affected_by, AFF_FLYING);
   send_to_char("You unfurl your wings and take to the sky.\n\r", ch);
   return;
  }
  return;
}

void do_dragon_create( CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj = create_object(get_obj_index(27950), 6);
  
  if( IS_NPC(ch))
  return;
  if( !IS_DRAGON(ch))
  {
   send_to_char("Huh?\n\r", ch);
   return;
  }
  if( ch->practice < 500 || ch->pcdata->quest < 8000)
  {
   send_to_char("You need 500 primal AND 8000 quest points.\n\r", ch);
   return;
  }
  ch->practice -= 500;
  ch->pcdata->quest -= 8000;
  obj_to_char( obj, ch);
  if (obj->questowner != NULL) 
    free_string(obj->questowner);
    obj->questowner = str_dup(ch->pcdata->switchname);
  send_to_char("An enourmous circlet appears in your claws.\n\r", ch);
  return;
}

void do_clutch( CHAR_DATA *ch, char *argument)
{
 char buf[MAX_STRING_LENGTH];
 CHAR_DATA *gch;

 if( !IS_DRAGON(ch) && !IS_IMMORTAL(ch))
 {
  send_to_char("Huh?\n\r", ch);
  return;
 }
 if( IS_NPC(ch))
  return;
 send_to_char("[      Name      ] [ Hits ] [ Mana ] [ Move ] [  Exp  ]\n\r", ch);
 for( gch = char_list; gch != NULL; gch = gch->next)
 {
  if( !IS_DRAGON(gch) || IS_NPC(gch))
  continue;
  sprintf( buf, "[%-16s] [%-6d] [%-6d] [%-6d] [%7d]\n\r", capitalize(gch->name),
           gch->hit, gch->mana, gch->move, gch->exp);
  send_to_char(buf, ch);
 }
 return;
}
