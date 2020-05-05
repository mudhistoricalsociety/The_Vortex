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

void do_sclaws( CHAR_DATA *ch, char *argument )
{
 if(IS_NPC(ch))
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_LUNA) < 5)
 {
  stc("You need level 5 in Luna to use silver claws.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SCLAW) )
 {
  stc("Your claws turn into pure silver!\n\r",ch);
  act("$n's claws turn to pure silver!",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_SCLAW);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SCLAW) )
 {
  stc("Your claws return to their normal state.\n\r",ch);
  act("$n's claws are no longer composed of pure silver.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_SCLAW);
  return;
 }
 return;
}

void do_moonbeam( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam = 0;
 int cost = 100 * GET_POWERS(ch,DISC_LUNA);

 argument = one_argument(argument,arg);
  
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_LUNA) < 8 )
 {
  stc("You need to obtain level 8 in Luna to use moonbeam.\n\r",ch);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You do not have enough energy to summon a moonbeam.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc( "Who's flesh do you wish to sear?\n\r", ch );
  return;
 }
 if( ( victim = get_char_room(ch,arg) ) == NULL )
 {
  stc( "They aren't here.\n\r", ch );
  return;
 }
 if( victim == ch )
 {
  stc("Not the brightest are we?\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not in safe.\n\r",ch);
  return;
 }
 if( weather_info.sunlight == SUN_DARK || weather_info.sunlight == SUN_SET )
  dam = number_range(150,300);
 else
  dam = number_range(75,150);
 dam *= GET_POWERS(ch,DISC_LUNA);
 dam += number_range(1,GET_POWERS(ch,DISC_LUNA)*100);

 sprintf(arg,"You sear $N's flesh with a deadly beam of moonlight! [%d]",dam);
 act(arg,ch,NULL,victim,TO_CHAR);
 sprintf(arg,"$n sears $N's flesh with a deadly beam of moonlight! [%d]",dam);
 act(arg,ch,NULL,victim,TO_NOTVICT);
 sprintf(arg,"$n sears your flesh with a deadly beam of moonlight! [%d]",dam);
 act(arg,ch,NULL,victim,TO_VICT);

 hurt_person(ch,victim,dam);
 set_fighting(ch,victim);
 update_pos(victim);
 ch->mana -= cost;
 WAIT_STATE(ch,GET_POWERS(ch,DISC_LUNA)*2);
 return;
}

void do_moongate( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 OBJ_DATA  *obj;
 char arg[MAX_INPUT_LENGTH];
 int cost = 15000;
 
 one_argument( argument, arg );
 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_LUNA) < 6 )
 {
  stc("You need to obtain level 6 Luna to use moongate.\n\r",ch);
  return;
 }
 if( ch->move < cost )
 {
  stc("You don't have enough stamina to summon a moongate.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc( "Who do you wish to gate to?\n\r", ch );
  return;
 }
 if( (victim = get_char_world( ch, arg ) ) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( (ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68) )
 {
  stc("You cannot do this whilst you are in the arena.\n\r", ch);
  return;
 }
 if ((victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68))
 {
  stc("You cannot do this whilst they are in the arena.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc("Not the brightest are we?\n\r",ch);
  return;
 }
 if( IS_IMMORTAL(victim) )
 {
  stc("Not on immortals.\n\r",ch);
  return;
 }
 if( is_ds_range(ch) || is_ds_range(victim) )
  return;
 if( victim->in_room == NULL
  || IS_NPC(victim)
  || IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
  || IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)
  || IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
  || IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
  || IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
  || IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
  || victim->in_room->vnum == ch->in_room->vnum)
 {
  stc( "You failed.\n\r", ch );
  return;
 }

 obj = create_object( get_obj_index( OBJ_VNUM_GATE2 ), 0 );
 obj->value[0] = victim->in_room->vnum;
 obj->value[3] = ch->in_room->vnum;
 obj->timer = 5;
 if( IS_AFFECTED(ch,AFF_SHADOWPLANE) )
  obj->extra_flags = ITEM_SHADOWPLANE;
 obj_to_room( obj, ch->in_room );
 
 obj = create_object( get_obj_index( OBJ_VNUM_GATE2 ), 0 );
 obj->value[0] = ch->in_room->vnum;
 obj->value[3] = victim->in_room->vnum;
 obj->timer = 5;
 if( IS_AFFECTED(victim,AFF_SHADOWPLANE) )
  obj->extra_flags = ITEM_SHADOWPLANE;
 obj_to_room( obj, victim->in_room );
    
 act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM);
 stc("A look of concentration passes over your face.\n\r",ch);
 act("$p appears in front of $n in a beam of moonlight.",ch,obj,NULL,TO_ROOM);
 act("$p appears in front of you in a beam of moonlight.",ch,obj,NULL,TO_CHAR);
 act("$p appears in front of $n in a beam of moonlight.",victim,obj,NULL,TO_ROOM);
 act("$p appears in front of you in a beam of moonlight.",ch,obj,victim,TO_VICT);
 ch->move -= cost;
 WAIT_STATE(ch,12);
 return;
}

void do_gmotherstouch( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = GET_POWERS(ch,DISC_LUNA) * 200;
 int dam = GET_POWERS(ch,DISC_LUNA) * 75;
  dam += number_range(200,450);
 
 argument = one_argument(argument,arg);
  
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_LUNA) < 4 )
 {
  stc("You need to obtain level 4 in Luna to use grandmother's touch.\n\r",ch);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You do not have enough energy to summon the grandmother's touch.\n\r",ch);
  return;
 }
 if( ( victim = get_char_room(ch, arg) ) == NULL )
 {
  stc( "They aren't here.\n\r", ch );
  return;
 }
 if( arg[0] == '\0' )
  victim = ch;

 if( !can_heal(ch, victim) ) return;

 act2("You surround $N in a healing beam of moonlight. [$D]",ch, NULL, victim, TO_CHAR,dam);
 act2("$n surrounds $N in a healing beam of moonlight. [$D]",ch, NULL, victim, TO_NOTVICT,dam);
 act2("$n surrounds you in a healing beam of moonlight. [$D]",ch, NULL, victim, TO_VICT,dam);
 victim->hit += dam;
 ch->mana -= cost;
 WAIT_STATE(ch,15);
 return;
}

void do_motherstouch( CHAR_DATA *ch, char *argument )
{
 int cost = GET_POWERS(ch,DISC_LUNA) * 100;
 int dam = GET_POWERS(ch,DISC_LUNA) * 40;
  dam += number_range(100,200);

 if( IS_NPC(ch) )
  return;
 if( GET_POWERS(ch,DISC_LUNA) < 3 )
 {
  stc("You need to obtain level 3 in Luna to use mother's touch.\n\r",ch);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You don't have enough energy to use mother's touch.\n\r",ch);
  return;
 }

 act2("You are surrounded by a beam of moonlight, and feel healed. [$D]", ch, NULL, NULL, TO_CHAR,dam);
 act2("$n is surrounded by a beam of moonlight, and looks healed. [$D]",ch, NULL, NULL, TO_ROOM,dam);
 ch->hit += dam;
 ch->mana -= cost;
 WAIT_STATE(ch,12);
 return;
}

void do_flameclaws( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_LUNA) < 1 )
 {
  stc("You need to obtain level 1 in the Luna totem to use flame claws.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW) )
 {
  stc("The flames surrounding your claws fade.\n\r",ch);
  act("The flames surrounding $n's claws fade.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_FCLAW);
  return;
 }
 else if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FCLAW) )
 {
  stc("Your claws are surrounded by blazing flames.\n\r",ch);
  act("$n's claws are surrounded by blazing flames.",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_FCLAW);
  return;
 }
 return;
}

void do_rend( CHAR_DATA *ch,char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BOAR) < 7 )
 {
  stc("You need to obtain level 7 in Boar to rend equipment.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_REND) )
 {
  stc("You will now ruthlessly rend your opponents eq.\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_REND);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_REND) )
 { 
  stc("You no longer ruthlessly rend your opponents eq.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_REND);
  return;
 }
return;
}

void do_skin( CHAR_DATA *ch,char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BEAR) < 7 )
 {
  stc("You need to obtain level 7 in Bear to toughen your skin.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_TOUGH) )
 {
  stc("Your skin hardens.\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_TOUGH);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_TOUGH) )
 { 
  stc("Your skin softens.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_TOUGH);
  return;
 }
return;
}

void do_jawlock( CHAR_DATA *ch,char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_RAPTOR) < 8 )
 {
  stc("You need to obtain level 8 in Raptor to use jawlock.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_JAW) )
 {
  stc("Your jaw grows stronger and more ready to clamp on your enemy.\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_JAW);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_JAW) )
 { 
  stc("Your jaw weakens.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_JAW);
  return;
 }
 return;
}

void do_perception( CHAR_DATA *ch,char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_RAPTOR) < 3 )
 {
  stc("You need to obtain level 3 in Raptor to heighten your perception.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SENSES) )
 {
  stc("Your senses expand to incredible proportions.\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_SENSES);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SENSES) )
 { 
  stc("Your senses return to their normal state.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_SENSES);
  return;
 }
 return;
}

void do_roar( CHAR_DATA *ch,char *argument )
{
 CHAR_DATA *victim;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BEAR) < 6 )
 {
  stc("You need to obtain level 6 in Bear to roar.\n\r",ch);
  return;
 }
 if( (victim = ch->fighting) == NULL)
 {
  stc("You arent fighting anyone.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_JAW) )
 {
  stc("That would be pointless with your jaws still clamped!\n\r",ch);
  return;
 }
 act("$n roars and screams, intimidating the hell out of $N!",ch,NULL,victim,TO_NOTVICT);
 act("You roar and scream, intimidating the hell out of $N!",ch,NULL,victim,TO_CHAR);
 act("$n roars and screams, intimidating the hell out of you!",ch,NULL,victim,TO_VICT);
 if( number_range(1,100) > 30 )
 {
  do_flee(victim,"");
  WAIT_STATE(victim,24);
 }
 WAIT_STATE(ch,12);
 return;
}
 

void do_quills( CHAR_DATA *ch,char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BEAR) < 5 )
 {
  stc("You need to obtain level 5 in Bear to extend your quills.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_QUILLS) )
 {
  stc("Sharp quills extend from your back.\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_QUILLS);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_QUILLS) )
 { 
  stc("Your quills sink into your back.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_QUILLS);
  return;
 }
 return;
}

void do_slam(CHAR_DATA *ch,char *argument)
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BEAR) < 8 )
 {
  stc("You need to obtain level 8 in Bear 8 to shoulder slam.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SLAM) )
 {
  stc("You will now shoulder slam when you get a chance.\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_SLAM);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SLAM) )
 { 
  stc("You now dont shoulderslam when given a chance.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_SLAM);
  return;
 }
 return;
}

void do_shred( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int dam, cost = 0;

 argument = one_argument( argument, arg );
 
 if( weather_info.sunlight == SUN_DARK || weather_info.sunlight == SUN_SET )
  dam = number_range(250,500);
 else
  dam = number_range(125,250);
 dam *= GET_POWERS(ch,DISC_RAPTOR);
 dam += number_range(1,GET_POWERS(ch,DISC_RAPTOR)*100);
 cost = dam/2;

 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_RAPTOR) < 6 )
 {
  stc("You need to obtain level 6 in Raptor to shred.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Shred whom?\n\r",ch);
  return;
 }
 if( ( victim = get_char_room(ch, arg) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( victim == ch )
 {
  stc("Not the brightest are you?\n\r",ch);
  return;
 }
 if( victim->fighting != NULL )
 {
  stc("You can't shred a fighting person.\n\r",ch);
  return;
 }
 if( !IS_AFFECTED(ch,AFF_SHADOWPLANE) )
 {
  stc("You can only shred from the shadowplane!\n\r",ch);
  return;
 }

 do_shadowplane(ch,"");
 if( is_safe(ch,victim) )
 {    
  stc("Not while safe.\n\r",ch);
  return;
 }
 damage(ch,victim,dam,gsn_shred);
 if( GET_POWERS(ch,DISC_RAPTOR) > 6)
  damage(ch,victim,dam,gsn_shred);
 if( GET_POWERS(ch,DISC_RAPTOR) > 7)
  damage(ch,victim,dam,gsn_shred);
 if( GET_POWERS(ch,DISC_RAPTOR) > 9)
  damage(ch,victim,dam,gsn_shred);
 WAIT_STATE(ch,24);
 return;
}

void do_run(CHAR_DATA *ch, char *argument)
{
 EXIT_DATA *pexit;
 char direct[MAX_STRING_LENGTH];
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 ROOM_INDEX_DATA *in_room, *to_room;
 int dir, count = 0;

 argument = one_argument(argument,arg);
 in_room=ch->in_room;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_RAPTOR) < 2 )
 {
  stc("You need to obtain level 2 in Raptor to run.\n\r",ch);
  return;
 }
 if( IS_AFFECTED(ch,AFF_WEBBED) )
 {
  stc( "Not with all this sticky webbing on.\n\r", ch);
  return;
 }
 if( IS_AFFECTED(ch, AFF_TENDRILS) )
 {
  stc("You are unable to move tangled in these tendrils.\n\r",ch);
  return;
 }
 if( ch->move < 1000 )
 {
  stc("You are too exhausted.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Run which direction?\n\r",ch);
  return;
 }
 if( str_cmp(arg,"w") && str_cmp(arg,"e") && str_cmp(arg,"n") && str_cmp(arg, "s") )
 {
  stc("You can only run n, s, e or w.\n\r",ch);
  return;
 }
 if( !str_cmp(arg,"w") ) sprintf(direct,"west");
 if( !str_cmp(arg,"e") ) sprintf(direct,"east");
 if( !str_cmp(arg,"s") ) sprintf(direct,"south");
 if( !str_cmp(arg,"n") ) sprintf(direct,"north");
 if( !str_cmp(arg,"w") ) dir = 3;
 if( !str_cmp(arg,"e") ) dir = 1;
 if( !str_cmp(arg,"n") ) dir = 0;
 if( !str_cmp(arg,"s") ) dir = 2;
 while( count < 10 )
 {
  if( (pexit = in_room->exit[dir]) == NULL || (to_room= pexit->to_room) == NULL)
  {
   stc("You stop running.\n\r",ch);
   act("$n comes to a screeching halt!",ch,NULL,NULL,TO_ROOM);
   WAIT_STATE(ch,10);
  }
  count++;
  sprintf(buf,"You run %s.\n\r",direct);
  stc(buf,ch);
  sprintf(buf,"%s runs %s.\n\r",ch->name,direct);
  act(buf,ch,NULL,NULL,TO_ROOM);
  move_char(ch,dir);
 }
 stc("You stop running.\n\r",ch);
 act("$n comes to a screeching halt!",ch,NULL,NULL,TO_ROOM);
 return;
}
 
void do_talons( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_STRING_LENGTH];
 CHAR_DATA *victim;
 int dam, cost = 0;

 if( weather_info.sunlight == SUN_DARK || weather_info.sunlight == SUN_SET )
  dam = number_range(125,250);
 else
  dam = number_range(75,125);
 dam *= GET_POWERS(ch,DISC_RAPTOR)/2;
 dam += number_range(1,GET_POWERS(ch,DISC_RAPTOR)*100);
 cost = dam/2;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_RAPTOR) < 10 )
 {
  stc("You need to obtain level 10 in Raptor to use talons.\n\r",ch);
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc( "You aren't fighting anyone.\n\r", ch );
  return;
 }
 if( ch->move < cost )
 {
  stc("You are too exhausted.\n\r",ch);
  return;
 }

 sprintf(arg,"Your talons strike $N incredibly hard! [%d]",dam);
 act(arg,ch,NULL,victim,TO_CHAR);
 sprintf(arg,"$n's talons strike you incredibly hard! [%d]",dam);
 act(arg,ch,NULL,victim,TO_VICT);
 sprintf(arg,"$n's talons strike $N incredibly hard! [%d]",dam);
 act(arg,ch,NULL,victim,TO_NOTVICT);

 WAIT_STATE(ch,16);
 ch->move -= cost;
 hurt_person(ch,victim,dam);
 return;
}

void do_devour( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 OBJ_DATA *obj;
 int heal;
 
 argument = one_argument(argument, arg);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_RAPTOR) < 5 )
 {
  stc("You need to obtain level 5 in Raptor to devour.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Devour what?\n\r",ch);
  return;
 }
 if( (obj = get_obj_list(ch,arg,ch->in_room->contents)) == NULL )
 {
  stc( "You can't find it.\n\r", ch );
  return;
 }
 if( obj->item_type != ITEM_CORPSE_NPC )
 {
  act("You are unable to devour $p.",ch,obj,0,TO_CHAR);
  return;
 }
 else if( obj->chobj != NULL && !IS_NPC(obj->chobj) && obj->chobj->pcdata->obj_vnum != 0 )
 {
  act("You are unable to devour $p.",ch,obj,0,TO_CHAR);
  return;
 }

 heal = number_range(1,100);
 ch->hit += heal;
 ch->mana += heal/4;
 ch->move += heal/4;
 if( ch->hit > ch->max_hit ) ch->hit = ch->max_hit;
 if( ch->mana > ch->max_mana ) ch->mana = ch->max_mana;
 if( ch->move > ch->max_move ) ch->move = ch->max_move;

 sprintf(arg,"You devour %d life from $p.",heal);
 act(arg,ch,obj,NULL,TO_CHAR);
 act("$p is devoured by $n.", ch,obj,NULL,TO_ROOM);
 extract_obj(obj);
 return;
}

void do_call_wild ( CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
    
 argument = one_argument(argument, arg);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_CONG) < 7 )
 {
  stc("You need to obtain level 7 in Congregation to call the wild.\n\r", ch);
  return;
 }
 if( ch->mana < 3000 )
 {
  stc("You do not have the required 3000 mana to use this power.\n\r", ch);
  return;
 }
 if( ch->practice < 30 )
 {
  stc("You do not have the required 30 primal to use this power.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Summon whom?\n\r",ch);
  return;
 }
 if( (victim = get_char_world(ch,arg) ) == NULL)
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( victim->in_room == NULL || victim->in_room->area != ch->in_room->area )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  stc("Not on NPC's.\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( ch == victim )
 {
  stc("Not the brightest are we?\n\r",ch);
  return;
 }
 if( is_ds_range(ch) )
  return;
 if( victim->fighting != NULL ) stop_fighting(victim,TRUE );

 act("A pack of savage dire wolves enter and drag $n away.",victim,NULL,NULL,TO_ROOM);
 char_from_room(victim);
 char_to_room(victim,ch->in_room);
 ch->mana -= 3000;
 ch->practice -= 30;
 act("Your animal friends have retrieved $N.",ch,NULL,victim,TO_CHAR);
 act("$n's wolf companions drag $N into the room.",ch,NULL,victim,TO_NOTVICT);
 act( "A pack of savage dire wolves enter the room and drag you away.", ch, NULL, victim, TO_VICT );
 do_look(victim,"auto");
 WAIT_STATE(ch, 12);
 return;
} 

void do_wither (CHAR_DATA * ch, char *argument)
{
 CHAR_DATA *victim;
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 bool blah = FALSE;
 int cost = 1000;

 argument = one_argument (argument, arg);

 if( IS_NPC (ch) )
  return;

 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_CONG) < 10 )
 {
  stc("You require congregation level 10 to use wither.\n\r", ch);
  return;
 }
 if( ch->mana < cost )
 {
  stc("You lack the magical stamina.\n\r", ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Whom do you wish to wither?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  stc("Not on mobs.\n\r",ch);
  return;
 }  
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( IS_ARM_R (victim, LOST_ARM) && IS_ARM_L (victim, LOST_ARM) )
 {
  stc("They have already lost both their arms!\n\r",ch);
  return;
 }

 WAIT_STATE(ch,36);
 ch->mana -= cost;

 if( !IS_ARM_R(victim,LOST_ARM) )
  blah = FALSE;
 else
  blah = TRUE;
 if( !blah )
 {
  if( !IS_ARM_R (victim,LOST_ARM) ) SET_BIT(victim->loc_hp[3],LOST_ARM);
  if( !IS_BLEEDING (victim,BLEEDING_ARM_R) ) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
  if( IS_BLEEDING (victim,BLEEDING_HAND_R) ) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);

  act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
  make_part(victim,"arm");

  if( IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM) )
  {
   if( (obj = get_eq_char(victim,WEAR_ARMS)) != NULL ) take_item (victim, obj);
   if( (obj = get_eq_char (victim,WEAR_WIELD)) != NULL) take_item (victim, obj);
   if( (obj = get_eq_char (victim,WEAR_HANDS)) != NULL) take_item (victim, obj);
   if( (obj = get_eq_char (victim, WEAR_WRIST_R)) != NULL) take_item (victim, obj);
   if( (obj = get_eq_char (victim, WEAR_FINGER_R)) != NULL) take_item (victim, obj);
   return;
  }
 }
 else if( blah )
 {
  if( !IS_ARM_L (victim,LOST_ARM) ) SET_BIT(victim->loc_hp[2],LOST_ARM);
  if( !IS_BLEEDING (victim,BLEEDING_ARM_L) ) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
  if( IS_BLEEDING(victim,BLEEDING_HAND_L) ) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);

  act("You concentrate deeply and $N's arm starts to wither!",ch,NULL,victim,TO_CHAR);
  act("$n's eyes glow dark red as $e gazes at you, and your arm begins withering!",ch,NULL,victim,TO_NOTVICT);
  act("$n gazes evilly at $N, and $S arm begins withering!",ch,NULL,victim,TO_VICT);
  make_part (victim, "arm");

  if( IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM) )
  {
   if( (obj = get_eq_char(victim,WEAR_ARMS) ) != NULL ) take_item (victim, obj);
   if( (obj = get_eq_char(victim,WEAR_WIELD) ) != NULL) take_item (victim, obj);
   if( (obj = get_eq_char(victim,WEAR_HANDS) ) != NULL) take_item (victim, obj);
   if( (obj = get_eq_char(victim,WEAR_WRIST_L) ) != NULL) take_item (victim, obj);
   if( (obj = get_eq_char(victim,WEAR_FINGER_L) ) != NULL) take_item (victim, obj);
    return;
  }
 }
 return;
}

void do_wburrow( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BOAR) < 5 )
 {
  stc("You need to obtain level 5 in Boar to burrow\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Burrow to who?\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch,arg) ) == NULL )
 {
  stc("They're not here.\n\r",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  stc("Not on NPC's.\n\r",ch);
  return;
 }
 if( !str_cmp(ch->clan,"None") || strlen(ch->clan) < 5 )
 {
  stc("You're not even clanned!\n\r",ch);
  return;
 }
 if( str_cmp(victim->clan,ch->clan) )
 {
  stc("You can only burrow to members of your own clan.\n\r", ch);
  return;
 }
 if( victim == ch )
 {
  stc( "Not the brightest are you?\n\r", ch);
  return;
 }
 if( (ch->in_room->vnum >= 50) && (ch->in_room->vnum <= 68) )
 {
  stc("You cannot do this whilst you are in the arena.\n\r", ch);
  return;
 }
 if( (victim->in_room->vnum >= 50) && (victim->in_room->vnum <= 68) )
 {
  stc("You cannot do this whilst they are in the arena.\n\r", ch);
  return;
 }
 if( is_ds_range(victim) )
  return;
 if( victim->in_room == NULL )
 {
  stc( "Nothing happens.\n\r", ch );
  return;
 }

 stc("You burrow into the ground.\n\r",ch);
 act("$n burrows into the ground.",ch,NULL,NULL,TO_ROOM);
 char_from_room(ch);
 char_to_room(ch,victim->in_room);
 do_look(ch,"auto");
 stc("You burrow out of the ground.\n\r",ch);
 act("$n burrows out of the ground.",ch,NULL,NULL,TO_ROOM);
 return;
}

void do_shift( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 int toform = 0;
 int cost = 10000;

 argument = one_argument(argument,arg);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_WOLF) < 10 )
 {
  stc("You need to obtain level 10 in Wolf to use shift.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc( "You can shift between the following forms: Homid, Glabro, Crinos, Hispo, Lupus.\n\r", ch );
  return;
 }
 if( !str_cmp(arg,"homid") ) toform = 1;
 else if( !str_cmp(arg,"glabro") ) toform = 2;
 else if( !str_cmp(arg,"crinos") ) toform = 3;
 else if( !str_cmp(arg,"hispo") ) toform = 4;
 else if( !str_cmp(arg,"lupus") ) toform = 5;
 else
 {
  stc( "You can shift between the following forms: Homid, Glabro, Crinos, Hispo, Lupus.\n\r", ch );
  return;
 }
 if( ch->mana < cost )
 {
  stc("You lack the energy to shift forms.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,WOLF_FORM) == toform )
 {
  stc( "You are already in that form.\n\r", ch );
  return;
 }

 switch(toform)
 {
  default:
   log_string("*** do_shift returned NULL ***");
   break;
  case 1:
   stc("Your body shifts and distorts into Homid form!\n\r",ch);
   break;
  case 2:
   stc("Your body shifts and distorts into Glabro form!\n\r",ch);
   break;
  case 3:
   stc("Your body shifts and distorts into Crinos form!\n\r",ch);
   break;
  case 4:
   stc("Your body shifts and distorts into Hispo form!\n\r",ch);
   break;
  case 5:
   stc("Your body shifts and distorts into Lupus form!\n\r",ch);
   break;
 }
 ch->mana -= cost;
 GET_POWERS(ch,WOLF_FORM) = toform;
 return;
}

void do_claws( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_WOLF) < 1 )
 {
  stc("You need to obtain level 1 in Wolf to extend your claws.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_CLAWS) )
 {
  stc("Razor sharp talons extend from your fingers!\n\r",ch);
  act("Razor sharp talons extend from $n's fingers!",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_CLAWS);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_CLAWS) )
 { 
  stc("Your talons shrink back into your fingers.\n\r",ch);
  act("$n's talons shrink back into $s fingers.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_CLAWS);
  return;
 }
 return;
}

void do_fangs( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_WOLF) < 2 )
 {
  stc("You need to obtain level 2 in Wolf to extend your fangs.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FANGS) )
 {
  stc("Razor sharp fangs extend from your gums!\n\r",ch);
  act("Razor sharp fangs extend from $n's gums!",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_FANGS);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FANGS) )
 { 
  stc("Your fangs shrink back into your gums.\n\r",ch);
  act("$n's fangs shrink back into $s gums.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_FANGS);
  return;
 }
 return;
}

void do_shield( CHAR_DATA *ch, char *argument)
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OWL) < 3 )
 {
  stc("You need to obtain level 3 in Owl to form a shield.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SHIELD) )
 {
  stc("A barely visible shield of nature forms around you!\n\r",ch);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_SHIELD);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_SHIELD) )
 { 
  stc("Your shield of nature flickers, then fades away.\n\r",ch);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_SHIELD);
  return;
 }
 return;
}

void do_rage_up( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *vch;
 CHAR_DATA *vch_next;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF))
  return;
 if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_RAGED) )
  return;
 else if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_RAGED) )
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_RAGED);

 stc("You throw back your head and howl with rage!\n\r",ch);
 act("$n throws back $s head and howls with rage!.",ch,NULL,NULL,TO_ROOM);
 stc("Coarse dark hair sprouts from your body!\n\r",ch);
 act("Coarse dark hair sprouts from $n's body!",ch,NULL,NULL,TO_ROOM);
 stc("Your eyes start glowing red!\n\r",ch);
 act("$n's eyes start glowing red!",ch,NULL,NULL,TO_ROOM);

 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_FANGS) ) do_fangs(ch,"");
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_CLAWS) ) do_claws(ch,"");

 ch->rage += 50;

 if( GET_POWERS(ch,DISC_WOLF) > 3) ch->rage += number_range(100,200);
 if( ch->rage > 300 ) ch->rage = 300;

 for( vch = char_list; vch != NULL; vch = vch_next )
 {
  vch_next	= vch->next;
  if( vch->in_room == NULL ) continue;
  if( ch == vch )
   continue;
  if( vch->trust > 6 )
   continue;
  if( !IS_NPC(vch) && vch->pcdata->chobj != NULL ) continue;
  if( !IS_NPC(vch) )
  {
   if (vch->in_room == ch->in_room) act("$n throws back $s head and howls with rage!", ch, NULL, vch, TO_VICT);
   else if( vch->in_room->area == ch->in_room->area ) stc("You hear a fearsome howl close by!\n\r", vch);
  }
  if( vch->in_room == ch->in_room )
  {
   if( is_safe(ch,vch) ) continue;
   if( vch == NULL ) continue;
   multi_hit(ch,vch,TYPE_UNDEFINED);
   multi_hit(ch,vch,TYPE_UNDEFINED);
   if( GET_POWERS(ch,DISC_WOLF) > 4 )
   {
    one_hit(ch,vch,TYPE_UNDEFINED,1);
    if( number_range(1,2) == 1 )
     one_hit(ch,vch,TYPE_UNDEFINED,2);
   }
   if( GET_POWERS(ch,DISC_BEAR) > 4 )
   {
    one_hit(ch,vch,TYPE_UNDEFINED,1);
    if( number_range(1,2) == 1 )
     one_hit(ch,vch,TYPE_UNDEFINED,2);
   }
  }
 }
 return;
}

void do_rage_down( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF))
  return;
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_RAGED) )
  return;
 REM_BIT_POWERS(ch,WOLF_BITS,BIT_RAGED);

 stc("The red glow in your eyes fades.\n\r",ch);
 act("The red glow in $n's eyes fades.",ch,NULL,NULL,TO_ROOM);
 stc("Your coarse hair shrinks back into your body.\n\r",ch);
 act("$n's coarse hair shrinks back into $s body.",ch,NULL,NULL,TO_ROOM);
 ch->rage -= 50;
 if( ch->rage < 0 ) ch->rage = 0;
 return;
}

void do_vanish( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch, CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_HAWK) < 5 )
 {
  stc("You need to obtain level 5 in Hawk to vanish from sight.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_VANISH) )
 {
  stc("You slowly fade into existance.\n\r",ch);
  act("$n slowly fades into existance.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,WOLF_BITS,BIT_VANISH);
  return;
 }
 else if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_VANISH) )
 {
  stc( "You slowly fade out of existance.\n\r", ch );
  act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,WOLF_BITS,BIT_VANISH);
 }
 return;
}

void do_web( CHAR_DATA *ch, char *argument )
{
 AFFECT_DATA af;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn;
 int cost = 0; 
 bool fail = FALSE;

 argument = one_argument(argument,arg);
   
 if( !IS_CLASS(ch,CLASS_WOLF) && !IS_CLASS(ch,CLASS_DROW) )
 {
  do_huh(ch,"");
  return;
 }
 if( IS_CLASS(ch,CLASS_DROW) && !IS_SET(GET_POWERS(ch,DROW_BITS),DPOWER_WEB) )
 {
  do_huh(ch,"");
  return;
 }
 if( IS_CLASS(ch,CLASS_WOLF) && GET_POWERS(ch,DISC_SPIDER) < 2 )
 {
  stc("You need to obtain level 2 in Spider to web your enemies.\n\r",ch);
  return;
 }
 if( IS_CLASS(ch,CLASS_DROW) )
  cost = 100;
 else
  cost = 1500;
 if( ch->mana < cost )
 {
  act2("You lack the $D mana to create a magical webbing!",ch,NULL,NULL,TO_CHAR,cost);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Web who?\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg)) == NULL)
 {
  stc("They arent here.\n\r", ch);
  return; 
 }
 if( ch == victim )
 {
  stc("Not the brightest are you?\n\r",ch);
  return;
 }
 if( is_safe(ch, victim) )
 {
  stc("Not while safe.\n\r", ch);
  return;
 }
 if( IS_AFFECTED(victim, AFF_WEBBED))
 {
  stc("They are already webbed.\n\r", ch);
  return;
 } 
 if( (sn = skill_lookup("web")) < 0)
  return;
 if( IS_CLASS(ch,CLASS_DROW) && number_percent() > 80 )
  fail = TRUE;
 else if( IS_CLASS(ch,CLASS_WOLF) && number_percent() > (GET_POWERS(ch,DISC_SPIDER) * 7) )
  fail = TRUE;
 else
  fail = FALSE;
 if( fail )
 {
  act("You shoot a web at $N, but it misses!",ch,NULL,victim,TO_CHAR);
  act("$n shoots a web at you, but it misses!",ch,NULL,victim,TO_VICT);
  act("$n shoots a web at $N, but it misses!",ch,NULL,victim,TO_NOTVICT);
 }
 else
 {
  af.type = sn;
  af.location = APPLY_AC;
  af.modifier = 400;
  af.duration = number_range(1,2);
  af.bitvector = AFF_WEBBED;
  affect_to_char(victim, &af);
  act("You shoot a web at $N, covering $M in sticky webbing!",ch,NULL,victim,TO_CHAR);
  act("$n shoots a web at you, covering you in sticky webbing!.",ch,NULL,victim,TO_VICT);
  act("$n shoots a web at $N, covering $M in sticky webbing!",ch,NULL,victim,TO_NOTVICT);
 }
 ch->mana -= cost;
 WAIT_STATE(ch, 10);
 return;
}

void wolf_disciplines( CHAR_DATA *ch, char *argument, char *argument2 )
{
 char arg[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 argument2 = one_argument(argument2,arg2);
 
 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0' && arg2[0] == '\0' )
 {
  display_class_stats(ch);
  return;
 }
 else if( !str_cmp(arg,"learn") )
 {
  if( arg2[0] == '\0' )
  {
   stc("Learn which discipline?\n\r",ch);
   return;
  }
  if( !str_cmp(arg2,"bear") )
  {
   improve_class(ch,DISC_BEAR);
   return;
  }
  else if( !str_cmp(arg2,"lynx") )
  {
   improve_class(ch,DISC_LYNX);
   return;
  }
  else if( !str_cmp(arg2,"boar") )
  {
   improve_class(ch,DISC_BOAR);
   return;
  }
  else if( !str_cmp(arg2,"owl") )
  {
   improve_class(ch,DISC_OWL);
   return;
  }
  else if( !str_cmp(arg2,"spider") )
  {
   improve_class(ch,DISC_SPIDER);
   return;
  }
  else if( !str_cmp(arg2,"wolf") )
  {
   improve_class(ch,DISC_WOLF);
   return;
  }
  else if( !str_cmp(arg2,"hawk") )
  {
   improve_class(ch,DISC_HAWK);
   return;
  }
  else if( !str_cmp(arg2,"mantis") )
  {
   improve_class(ch,DISC_MANTIS);
   return;
  }
  else if( !str_cmp(arg2,"raptor") )
  {
   improve_class(ch,DISC_RAPTOR);
   return;
  }
  else if( !str_cmp(arg2,"luna") )
  {
   improve_class(ch,DISC_LUNA);
   return;
  }
  else if( !str_cmp(arg2,"pain") )
  {
   improve_class(ch,DISC_PAIN);
   return;
  }
  else if( !str_cmp(arg2,"congregation") )
  {
   improve_class(ch,DISC_CONG);
   return;
  }
  else
  {
   stc("Learn which discipline?\n\r",ch);
   return;
  }
  return;
 }
 else if( !str_cmp(arg,"bear") )
 {
  if( GET_POWERS(ch,DISC_BEAR) == 0 )
   stc("You have no knowledge of the Bear discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_BEAR) > 0 )
   stc("[01] [Auto         ] You are empowered by the spirit of the Bear, increasing damage.\n\r",ch);
  if( GET_POWERS(ch,DISC_BEAR) > 2 )
   stc("[03] [Rage         ] You can force yourself into a wild rage.\n\r",ch);
  if( GET_POWERS(ch,DISC_BEAR) > 4 )
   stc("[05] [Quills       ] You can extend sharp quills from your backs, and your rage is improved.\n\r",ch);
  if( GET_POWERS(ch,DISC_BEAR) > 5 )
   stc("[06] [Roar         ] You know how to roar, and intimidate your enemies.\n\r",ch);
  if( GET_POWERS(ch,DISC_BEAR) > 6 )
   stc("[07] [Skin         ] You can toughen your skin, reducing damage.\n\r",ch);
  if( GET_POWERS(ch,DISC_BEAR) > 7 )
   stc("[08] [Slam         ] You can shoulder slam your opponents in combat.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"lynx") )
 {
  if( GET_POWERS(ch,DISC_LYNX) == 0 )
   stc("You have no knowledge of the Lynx discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_LYNX) > 0 )
   stc("[**] [Auto         ] Mastery of the Lynx discipline grants enhanced parry/dodge.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"boar") )
 {
  if( GET_POWERS(ch,DISC_BOAR) == 0 )
   stc("You have no knowledge of the Boar discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_BOAR) > 0 )
   stc("[**] [Auto         ] Mastery of the Boar discipline grants enhanced toughness.\n\r",ch);
  if( GET_POWERS(ch,DISC_BOAR) > 4 )
   stc("[05] [Burrow       ] You can burrow to your clan members in need.\n\r",ch);
  if( GET_POWERS(ch,DISC_BOAR) > 6 )
   stc("[07] [Rend         ] You can ruthlessly rend your opponents in combat, enhancing damage.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"owl") )
 {
  if( GET_POWERS(ch,DISC_OWL) == 0 )
   stc("You have no knowledge of the Owl discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_OWL) > 0 )
   stc("[**] [Auto         ] Mastery of the Owl discipline grants enhanced dodge.\n\r",ch);
  if( GET_POWERS(ch,DISC_OWL) > 2 )
   stc("[03] [Shield       ] You are able to form a protective shield of nature around yourself.\n\r",ch);
  if( GET_POWERS(ch,DISC_OWL) > 5 )
   stc("[06] [Shadowplane  ] You can enter the realm of shadows.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"spider") )
 {
  if( GET_POWERS(ch,DISC_SPIDER) == 0 )
   stc("You have no knowledge of the Spider discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_SPIDER) > 0 )
   stc("[**] [Auto         ] Mastery of the spider discipline grants inceased attack speed.\n\r",ch);
  if( GET_POWERS(ch,DISC_SPIDER) > 2 )
   stc("[03] [Web          ] You know how to entrap your enemies in a web.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"wolf") )
 {
  if( GET_POWERS(ch,DISC_WOLF) == 0 )
   stc("You have no knowledge of the Wolf discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_WOLF) > 0 )
   stc("[01] [Claws        ] You can extend long claws from your fingers.\n\r",ch);
  if( GET_POWERS(ch,DISC_WOLF) > 1 )
   stc("[02] [Fangs        ] You can extend long fangs from your gums.\n\r",ch);
  if( GET_POWERS(ch,DISC_WOLF) > 2 )
   stc("[03] [Calm         ] You have learned to control your anger, and calm yourself.\n\r",ch);
  if( GET_POWERS(ch,DISC_WOLF) > 4 )
   stc("[05] [Auto         ] You are in tune with the Wolf, and your rage has been enhanced.\n\r",ch);
  if( GET_POWERS(ch,DISC_WOLF) > 9 )
   stc("[10] [Shift        ] You are able to shift between the various forms of a werewolf.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"hawk") )
 {
  if( GET_POWERS(ch,DISC_HAWK) == 0 )
   stc("You have no knowledge of the Hawk discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_HAWK) > 0 )
   stc("[**] [Auto         ] Mastery of the Hawk discipline grants increased attack speed.\n\r",ch);
  if( GET_POWERS(ch,DISC_HAWK) > 4 )
   stc("[05] [Vanish       ] You have mastered the understanding of sight, enabling you to vanish from your foes.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"mantis") )
 {
  if( GET_POWERS(ch,DISC_MANTIS) == 0 )
   stc("You have no knowledge of the Mantis discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_MANTIS) > 0 )
   stc("[**] [Auto         ] Mastery of the Mantis discipline grants enhanced parry.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"raptor") )
 {
  if( GET_POWERS(ch,DISC_RAPTOR) == 0 )
   stc("You have no knowledge of the Raptor discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_RAPTOR) > 1 )
   stc("[02] [Run          ] You can rapidly sprint across land.\n\r",ch);
  if( GET_POWERS(ch,DISC_RAPTOR) > 2 )
   stc("[03] [Perception   ] Your perception has been heightened, aiding in critical blows.\n\r",ch);
  if( GET_POWERS(ch,DISC_RAPTOR) > 4 )
   stc("[05] [Devour       ] You can devour corpses and drain the life from them.\n\r",ch);
  if( GET_POWERS(ch,DISC_RAPTOR) > 5 )
   stc("[06] [Shred        ] You can shred your enemies ruthlessly from the shadows.\n\r",ch);
  if( GET_POWERS(ch,DISC_RAPTOR) > 7 )
   stc("[08] [Jawlock      ] You can lock your jaw, to prevent your enemies from fleeing.\n\r",ch);
  if( GET_POWERS(ch,DISC_RAPTOR) > 9 )
   stc("[10] [Talons       ] You can claw at your enemies with long, sharp talons.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"luna") )
 {
  if( GET_POWERS(ch,DISC_LUNA) == 0 )
   stc("You have no knowledge of the Luna discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 0 )
   stc("[01] [Flameclaws   ] You are able to set your claws on fire.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 2 )
   stc("[03] [Motherstouch ] You can heal your wounds with the touch of your mother.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 3 )
   stc("[04] [Gmotherstouch] You can heal your wounds with the touch of your grandmother.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 4 )
   stc("[05] [Silverclaws  ] You can turn your claws to pure silver.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 5 )
   stc("[06] [Moongate     ] You can create a gate of moonlight to others.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 7 )
   stc("[08] [Moonbeam     ] You can sear the flesh of your enemies with a beam of moonlight.\n\r",ch);
  if( GET_POWERS(ch,DISC_LUNA) > 9 )
   stc("[10] [Auto         ] At night you are protected from harm in the moon's rays.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"pain") )
 {
  if( GET_POWERS(ch,DISC_PAIN) == 0 )
   stc("You have no knowledge of the Pain discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_PAIN) > 0 )
   stc("[**] [Auto         ] Mastery of the Pain discipline grants enhanced toughness.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"congregation") )
 {
  if( GET_POWERS(ch,DISC_CONG) == 0 )
   stc("You have no knowledge of the Congregation discipline.\n\r",ch);
  if( GET_POWERS(ch,DISC_CONG) > 6 )
   stc("[07] [Callwild     ] You can summon wolves of the wild and draw your enemies to you.\n\r",ch);
  if( GET_POWERS(ch,DISC_CONG) > 9 )
   stc("[10] [Wither       ] You can summon the forces of nature to rip off your opponents limbs.\n\r",ch);
  return;
 }
 return;
}

void do_wolf_rage( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_BEAR) < 3 )
 {
  stc("You need to obtain level 3 in Bear to force your anger.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_RAGED) )
 {
  stc("You are already in a frenzied rage!\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,WOLF_BITS),BIT_RAGED) )
 {
  ch->rage += number_range(8,15);
  stc("You start snarling angrilly.\n\r",ch);
  act("$n starts snarling angrilly.",ch,NULL,NULL,TO_ROOM);
  if( ch->rage >= 100 ) do_rage_up(ch,"");
  WAIT_STATE(ch,12);
  return;
 } 
 return;
}

void do_wolf_calm( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_WOLF) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_WOLF) < 3 )
 {
  stc("You need to obtain level 3 in Wolf to calm your anger.\n\r",ch);
  return;
 }
 if( ch->fighting != NULL )
 {
  stc("You can't control your rage in combat!\n\r",ch);
  return;
 }
 if( ch->rage < 100 )
 {
  stc("You are not in a frenzied rage!\n\r",ch);
  return;
 }

 ch->rage -= number_range(8,15);                                           
 stc("You take a deep breath and calm yourself.\n\r",ch);
 act("$n takes a deep breath and tries to calm $mself.",ch,NULL,NULL,TO_ROOM);
 if( ch->rage < 100 ) do_rage_down(ch,"");
 WAIT_STATE(ch,12);
 return;
}

void do_wolfpowers( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 stc("Bear        : rage, quills, roar, skin, slam\n\r",ch);
 stc("Boar        : burrow, rend\n\r",ch);
 stc("Owl         : scry, shield, shadowplane\n\r",ch);
 stc("Spider      : web\n\r",ch);
 stc("Wolf        : claws, fangs, calm, shift\n\r",ch);
 stc("Hawk        : vanish\n\r",ch);
 stc("Raptor      : run, perception, devour, shred, jawlock, talons\n\r",ch);
 stc("Luna        : flameclaws, motherstouch, gmothertouch, silverclaws, moongate, moonbeam\n\r",ch);
 stc("Congregation: callwild, wither\n\r",ch);
 return;
}
