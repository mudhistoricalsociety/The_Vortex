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

bool check_beast( CHAR_DATA *ch )
{
 if( IS_NPC(ch) )
  return FALSE;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
  return FALSE;
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA)
  || IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BEAST)
  || IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DARK_GOLC) )
  return FALSE;
 if( number_range(1,130) < 10 )
  return TRUE;
 return FALSE;
}

void check_vkills( CHAR_DATA *ch )
{
 int chance;
 int chance_mod;
 char buf[MAX_STRING_LENGTH];

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
  return;
 if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_GOLCANDA) )
  return;

 chance = GET_POWERS(ch,VAMP_KILLS);
 chance_mod = (GET_POWERS(ch,VAMP_KILLS)/5);
 chance += chance_mod;

 if( number_range(1,130) < chance )
 {
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_GOLCANDA);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_DARK_GOLC);
  stc("You have now become one of the feared...A traitor of their own kind; a plague on humanity.\n\r",ch);
  stc("You are now...A Dark Golcanda!\n\r\n\r",ch);
  act("An inhuman power of darkness begins to course through your veins!",ch,NULL,NULL,TO_CHAR);
  act("An inhuman power of darkness begins to course through $n's veins!",ch,NULL,NULL,TO_ROOM);
  sprintf(buf,"%s has mutated into a Dark Golcanda!",ch->pcdata->switchname);
  do_info(ch,buf);
  return;
 }
 return;
}

void do_bloodme( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 GET_POWERS(ch,VAMP_BLOOD) = blood_max(ch);
 return;
}

int blood_max( CHAR_DATA *ch )
{
 int max, gen_mod, age_mod, base_mod, tide_mod;

 if( ch == NULL )
  return 0;
 base_mod = 100;
 switch( ch->generation )
 {
  case 1:  gen_mod = 7; break;
  case 2:  gen_mod = 5;  break;
  case 3:  gen_mod = 3;  break;
  case 4:  gen_mod = 1;  break;
  default: gen_mod = 0;  break;
 }
 switch( GET_POWERS(ch,VAMP_AGE) )
 {
  case AGE_NONE:       age_mod = 1; break;
  case AGE_CAITIFF:    age_mod = 3; break;
  case AGE_ANCILLA:    age_mod = 5; break;
  case AGE_ELDER:      age_mod = 7; break;
  case AGE_METHUSELAH: age_mod = 9; break;
  default:             age_mod = 0; break;
 }
 switch( ch->generation )
 {
  case 1:  tide_mod = 2.00; break;
  case 2:  tide_mod = 1.75; break;
  case 3:  tide_mod = 1.50; break;
  case 4:  tide_mod = 1.25; break;
  default: tide_mod = 1.00; break;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_TIDE) )
  max = ( ( (gen_mod + age_mod) * base_mod) * tide_mod);
 else
  max = ( (gen_mod + age_mod) * base_mod);
 return max;
}

void do_vamp_conceal( CHAR_DATA *ch, char *argument )
{
 int cost = 200;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBFUSCATE) < 5 )
 {
  stc("You need to obtain Obfuscate 5 to conceal yourself.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_CONCEAL) && (GET_POWERS(ch,VAMP_BLOOD) < cost) )
 {
  stc("You first require 200 blood to conceal yourself.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_CONCEAL) )
 {
  stc("You slowly fade back into existance.\n\r",ch);
  act("$n slowly fades back into existance.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_CONCEAL);
  return;
 }
 else if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_CONCEAL) )
 {
  stc("You slowly fade out of existance.\n\r",ch);
  act("$n slowly fades out of existance.",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_CONCEAL);
  GET_POWERS(ch,VAMP_BLOOD) -= cost;
  return;
 }
 return;
}

void do_sharpen( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_STRING_LENGTH];
 OBJ_DATA *obj;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 7 )
 {
  stc("You need to obtain Quietus 7 to sharpen.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What do you wish to sharpen?\n\r",ch);
  return;
 }
 if( ( obj = get_obj_carry(ch,arg) ) == NULL )
 {
  stc("You dont have that weapon.\n\r",ch);
  return;
 }
 if( IS_SET(obj->quest,QUEST_ARTIFACT) || obj->item_type != ITEM_WEAPON )
 {
  stc("You cant sharpen that item.\n\r",ch);
  return;
 }
 if( obj->value[0] == 18000 )
 {
  stc("This item is already razor sharp!\n\r",ch);
  return;
 }
 if( obj->value[0] != 0 )
 {
  stc(" This weapon already has a power.\n\r",ch);
  return;
 }

 obj->value[0] = 18000;
 obj->value[1] += 5;
 obj->value[2] += 10;

 sprintf(arg,"You grind away at %s until it is razor sharp!",obj->short_descr);
 stc(arg,ch);
 sprintf(arg,"%s grinds away at %s until it is razor sharp!",ch->name,obj->short_descr);
 act(arg,ch,NULL,NULL,TO_ROOM);
 return;
}

void do_gourge( CHAR_DATA *ch, char *argument  )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 8 )
 {
  stc("You need to obtain Thaumaturgy 8 to gourge.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Who do you wish to gourge?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( !IS_NPC(victim) )
 {
  stc("You cannot gourge a person.\n\r",ch);
  return;
 }
 if( victim->level > 100 )
 {
  stc("Only small creatures are defenceless enough to be gourged on.\n\r",ch);
  return;
 }
 act("$n leaps toward $N baring $s fangs!",ch,NULL,victim,TO_NOTVICT);
 act("You leap toward $N baring your fangs!",ch,NULL,victim,TO_CHAR);
 WAIT_STATE(ch,15);
 act("You rip out $N's throat, and gourge on all of $S blood!",ch,NULL,victim,TO_CHAR);
 act("$n rips out $N's throat, and gourges on all of $S blood!",ch,NULL,victim,TO_NOTVICT);
 GET_POWERS(ch,VAMP_BLOOD) += (victim->level * (GET_POWERS(ch,DISC_THAUM) / 2));
 if( GET_POWERS(ch,VAMP_BLOOD) >= blood_max(ch) )
 {
  stc("Your bloodlust is sated.\n\r",ch);
  GET_POWERS(ch,VAMP_BLOOD) = blood_max(ch);
 }
 act("$N's body falls lifelessly to the ground.",ch,NULL,victim,TO_ROOM);
 raw_kill( victim );
 return;
}

void do_bloodwater( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int dam;
 int cost = 100;
 
 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 9 )
 {
  stc("You need to obtain Thaumaturgy 9 to use blood water.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 100 blood to turn blood into water.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc( "Whose blood do you wish to turn to water?\n\r", ch );
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc( "They aren't here.\n\r", ch );
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 dam = number_range(50,100) * GET_POWERS(ch,DISC_THAUM);
 if( IS_CLASS(victim,CLASS_VAMPIRE) )
 {
  dam *= 1.5;
  GET_POWERS(victim,VAMP_BLOOD) -= dam;
 }
 else
  victim->hit -= dam;
 act2("$N screams in agony as you turn $S blood to water! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$N screams in agony as $n turns $S blood to water! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 act2("You scream in agony as $n turns your blood to water! [$D]",ch,NULL,victim,TO_VICT,dam);
 if( IS_CLASS(victim,CLASS_VAMPIRE) && IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_ACID) )
 {
  dam *= 1.25;
  act2("The acidic content of your blood causes you to scream in agony! [$D]",ch,NULL,victim,TO_VICT,dam);
  act2("The acidic content of $N's blood causes $m to scream in agony! [$D]",ch,NULL,victim,TO_ROOM,dam);
  GET_POWERS(victim,VAMP_BLOOD) -= dam;
  if( GET_POWERS(victim,VAMP_BLOOD) < 0 )
   GET_POWERS(victim,VAMP_BLOOD) = 0;
  victim->hit -= dam;
  update_pos(victim);
 }
 if( !IS_CLASS(victim,CLASS_VAMPIRE) )
  WAIT_STATE(ch,12);
 else
  WAIT_STATE(ch,6);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 return;
}

void do_spew( CHAR_DATA *ch, char *argument )
{
 int sn, level, spelltype;
 int cost = 150;
 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 6 )
 {
  stc("You need Thaumaturgy 6 to spew acid.\n\r",ch);
  return; 
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 150 blood to spew acid.\n\r",ch);
  return;
 }
 if( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( ( sn = skill_lookup("spew") ) < 0 )
  return;
 spelltype = skill_table[sn].target;
 level = GET_POWERS(ch,DISC_THAUM);
 (*skill_table[sn].spell_fun) (sn,level,ch,NULL);
 WAIT_STATE(ch,12);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 return;
}

void do_vampdarkness( CHAR_DATA *ch, char *argument )
{
 ROOM_INDEX_DATA *room;   
 int cost = 100;
 room = ch->in_room;
 if( IS_NPC(ch) )
  return;
 if( IS_CLASS(ch,CLASS_DROW) )
 {
  do_darkness(ch,argument);
  return;
 }
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 6 )
 {
  stc("You need Quietus 6 to summon darkness.\n\r",ch);
  return;
 }
 if( weather_info.sunlight != SUN_SET && weather_info.sunlight != SUN_DARK )
 {
  stc("Darkness can not be summoned in the daylight!\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 100 blood to summon darkness.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 SET_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
 act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM);
 stc("A look of concentration passes over your face.\n\r",ch);
 act("A complete darkness fills the room.",ch,NULL,NULL,TO_ALL);
 return;
}

void do_dragonform( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 int cost = 300;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 7 )
 {
  stc("You need Vicissitude 7 to transform into a dragon.\n\r",ch);
  return;
 }
 if( (GET_POWERS(ch,VAMP_FORM) != FORM_ZULO) && (GET_POWERS(ch,VAMP_FORM) != FORM_DRAGON) )
 {
  stc("You can only transform into a dragon while in Zuloform.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) == FORM_DRAGON )
 {
  GET_POWERS(ch,VAMP_FORM) = FORM_ZULO;
  stc("You transform back into zuloform.",ch);
  act("$n shrinks back into a big black monster.",ch,NULL,NULL,TO_ROOM);
/*  free_string(ch->morph);
  ch->morph = str_dup("A big black monster");
  */return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to transform into a large dragon.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 stc("You transform into a large dragon!",ch);
 act("$n's body grows and distorts into a large dragon!",ch,NULL,NULL,TO_ROOM);
 GET_POWERS(ch,VAMP_FORM) = FORM_DRAGON;
/* free_string(ch->morph);
 ch->morph = str_dup("A large black dragon");
 */return;
}

void do_objform( CHAR_DATA *ch, char *argument )
{
 OBJ_DATA *obj;                              
 OBJ_DATA *obji;
 char buf[MAX_STRING_LENGTH];
 char arg[MAX_STRING_LENGTH];
 int cost = 500;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 stc("COMMAND TEMPORARILY DISABLED\n\r", ch);
 return; 
 if( GET_POWERS(ch,DISC_VICIS) < 10 )
 {
  stc("You need Vicissitude 10 to transform into an object.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 500 blood to transform into an object.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
 {
  stc("You cannot do this while not in human form.\n\r",ch);
  return;
 }
 if( ch->pcdata->obj_vnum != 0 )
 {
  do_humanform(ch,"");
  return;
 }
 if( ch->fight_timer > 0 )
 {
  stc("Not with a fight timer.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What object in the game do you wish to assume the form of?\n\r",ch); 
  return;
 }
 if( (obji = get_obj_world(ch,arg) ) == NULL )
 {
  stc("Nothing like that is in the game.\n\r",ch);
  return;
 }
 if( IS_AFFECTED(ch,AFF_WEBBED) )
 {
  stc( "Not with all this sticky webbing on.\n\r", ch);
  return;
 }
 if( obji->item_type == ITEM_GEMSTONE ||
     obji->item_type == ITEM_HILT     ||
     obji->item_type == ITEM_COPPER   ||
     obji->item_type == ITEM_IRON     ||
     obji->item_type == ITEM_STEEL    ||
     obji->item_type == ITEM_ADAMANTITE||
     obji->item_type == ITEM_QUEST ||
     IS_SET(obji->quest,QUEST_ARTIFACT)
     || IS_SET(obji->quest,QUEST_RELIC) )
 {
  stc("You are unable to take the form of that.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 obj = create_object(get_obj_index(obji->pIndexData->vnum),60);
 obj_to_room(obj,ch->in_room);
 stc("You concentrate briefly...\n\r",ch);
 act("$n's form shrinks and distorts into $p.",ch,obj,NULL,TO_ROOM);
 act("Your form shrinks and distorts into $p.",ch,obj,NULL,TO_CHAR);
 ch->pcdata->obj_vnum = obj->pIndexData->vnum;
 sprintf(buf,"%d",ch->pcdata->obj_vnum);
 stc(buf,ch);
 obj->chobj = ch;
 ch->pcdata->chobj = obj;
 GET_POWERS(ch,VAMP_FORM) = FORM_OBJECT;
 SET_BIT(ch->extra, EXTRA_OSWITCH);
/* free_string(ch->morph);
 ch->morph = str_dup(obj->short_descr);
 */return;
}

void do_baal( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 9 )
 {
  stc("You need Daimionon 9 to summon the spirit of Baal.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_BAAL) )
 {
  stc("You already have the spirit of Baal with you!\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < blood_max(ch) )
 {
  stc("You need maximum blood to summon Baal.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) = 0;
 WAIT_STATE(ch,20);
 SET_BIT_POWERS(ch,VAMP_BITS,BIT_BAAL);
 GET_POWERS(ch,DISC_POTENCE)++;
 GET_POWERS(ch,DISC_CELERITY)++;
 GET_POWERS(ch,DISC_FORTITUDE)++;
 stc("You feel a surge of power as the spirit of Baal courses through your veins!\n\r",ch);
 return;
}

void do_inferno( CHAR_DATA *ch, char *argument )
{
 ROOM_INDEX_DATA *room;
 int cost = 200;

 room = ch->in_room;
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 6 )
  {
  stc("You need Daimionon 6 to use Inferno.\n\r",ch);
  return;
  }
 if( IS_SET(ch->in_room->room_flags,ROOM_FLAMING) )
 {
  stc("This room is already engulfed in flames!\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 200 blood to summon the fires of hell.\n\r",ch);
  return;
 }
 if( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 SET_BIT(room->room_flags,ROOM_FLAMING);
 act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM);
 stc("A look of concentration passes over your face.\n\r",ch);
 act("The room suddenly explodes into flames!",ch,NULL,NULL,TO_ALL);
 return;
}

void do_zombie(CHAR_DATA *ch, char *argument )
{
 char buf2[MAX_STRING_LENGTH]; 
 char buf [MAX_INPUT_LENGTH];
 char arg [MAX_STRING_LENGTH];
 AFFECT_DATA af;
 CHAR_DATA *victim;
 OBJ_DATA *obj;
 int cost = 300; 

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Zombie what corpse?\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 3 )
 {
  stc("You need Daimionon 3 to create a zombie.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to summon a zombie.\n\r",ch);
  return;
 }
 if( ch->pcdata->followers > 0 )
 {
  stc("Nothing happens.\n\r",ch);
  return;
 }
 if( (obj = get_obj_carry(ch,arg) ) == NULL )
 {
  stc("You dont have that corpse.",ch);
  return;
 }
 if( obj->item_type != ITEM_CORPSE_NPC || IS_SET(obj->quest,QUEST_ZOMBIE) )
 {
  stc("You can only create zombies from original corpses.\n\r",ch);
  return;
 }
 if( obj->value[2] == 6 )
 {
  stc("You cannot zombie a mount.\n\r",ch);
  return;
 }
 if( IS_SET(ch->newbits, HAS_HADES) )
 {
   stc("Hades stops you from making a zombie.\n\r", ch);
   return;
 }
 if( obj->level > 750 )
 {
  stc("You fail to make a zombie from the corpse.\n\r", ch);
  return;
 }
 victim = create_mobile( get_mob_index(obj->value[2]) );
 sprintf(buf,"the zombie of %s",victim->short_descr);
 sprintf(buf2,"the zombie of %s is here.\n\r",victim->short_descr);
 free_string(victim->short_descr);
 victim->short_descr = str_dup(buf);
 free_string(victim->name);
 victim->name = str_dup(buf);
 free_string(victim->long_descr);
 victim->long_descr = str_dup(buf2);
 SET_BIT(victim->extra,EXTRA_ZOMBIE);
 do_say(ch,"Get your ass out of that moldy dirt so I can ride you like a donkey all night long! (Kalfyre,03.26.02)");
 sprintf(buf,"%s slowly rises from the earth.\n\r",obj->short_descr);
 act(buf,ch,NULL,NULL,TO_ROOM);
 stc(buf,ch);
 char_to_room(victim,ch->in_room);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 af.duration  = 666;
 af.location  = APPLY_NONE;
 af.modifier  = 0;
 af.bitvector = AFF_CHARM;
 affect_to_char(victim,&af);
 WAIT_STATE(ch,10);
 extract_obj(obj);
 do_follow(victim,ch->name);
 ch->pcdata->followers++;
 return;
}

void do_fleshcraft( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int cost = 300;

 argument = one_argument( argument, arg );
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 2 )
 {
  stc("You need Vicissitude 2 to change your appearance.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Change to look like whom?\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
 {
  stc("You need to be in humanform, first.\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg)) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the required 300 blood to mold your flesh.\n\r",ch);
  return;
 }
 if( ch == victim )
 {
  if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DISGUISE) )
  {
   stc("You already look like yourself!\n\r",ch);
   return;
  }
  sprintf(arg,"%s's flesh molds and transforms into %s.",ch->pcdata->switchname,ch->pcdata->switchname);
  act("Your flesh molds and transforms into your true form.",ch,NULL,victim,TO_CHAR);
  act(arg,ch,NULL,victim,TO_ROOM);
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_DISGUISE);
/*  free_string(ch->morph);
  ch->morph = str_dup("");
  */return;
 }
 act("Your flesh molds and transforms into a clone of $N.",ch,NULL,victim,TO_CHAR);
 act("$n's flesh molds and transforms into a clone of $N.",ch,NULL,victim,TO_NOTVICT);
 act("$n's flesh molds and transforms into a clone of you!",ch,NULL,victim,TO_VICT);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 SET_BIT_POWERS(ch,VAMP_BITS,BIT_DISGUISE);
/* free_string(ch->morph);
 if( IS_NPC(victim) )
  ch->morph = str_dup(victim->short_descr);
 else
  ch->morph = str_dup(victim->pcdata->switchname);
 */return;
}

void do_entrance( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn, level, spelltype;
 int cost = 200;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) ) 
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_PRESENCE) < 3 )
 {
  stc("You need Presence 3 to entrance.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 200 blood to entrance.\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( ch == victim )
 {
  stc("You cannot entrance yourself.\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 stc("A look of concentration crosses your face.\n\r",ch);
 act("A look of concentration crosses over $n's face.",ch,NULL,victim,TO_ROOM);
 if( (sn = skill_lookup("charm") ) < 0 )
  return;
 spelltype = skill_table[sn].target;
 level = GET_POWERS(ch,DISC_PRESENCE) * 30;
 (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 WAIT_STATE(ch,12);
 return;
}

void do_tendrils( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn, level, spelltype;
 int cost = 100;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) ) 
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBTEN) < 6 )
 {
  stc("You need Obtenebration 6 to summon abyssal tendrils.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 100 blood to summon abyssal tendrils.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Summon abyssal tendrils on who?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( ch == victim )
 {
  stc("You cannot summon abyssal tendrils on yourself.\n\r", ch );
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( (sn = skill_lookup("web") ) < 0 )
  return;
 spelltype = skill_table[sn].target;
 level = ch->spl[spelltype] * 0.25;
 (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 WAIT_STATE(ch,12);
 return;
}

void do_lamprey( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBTEN) < 5 )
 {
  stc("You need Obtenebration 5 to lamprey.\n\r",ch);
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You aren't fighting anyone!\n\r",ch);
  return;
 }
 if( !IS_NPC(victim) )
  dam = GET_POWERS(ch,DISC_OBTEN) * number_range(50,100);
 else
  dam = GET_POWERS(ch,DISC_OBTEN) * number_range(50,150);
 act2("Your tendrils of darkness hits $N incredibly hard! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n's tendrils of darkness hits you incredibly hard! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n's tendrils of darkness hits $N incredibly hard! [$D]",ch,NULL,victim,TO_NOTVICT,dam);

 hurt_person(ch,victim,dam);
 GET_POWERS(ch,VAMP_BLOOD) += dam/5;
 if( GET_POWERS(ch,VAMP_BLOOD) >= blood_max(ch) )
 {
  stc("Your bloodlust is sated.\n\r",ch);
  GET_POWERS(ch,VAMP_BLOOD) = blood_max(ch);
 }
 if( IS_CLASS(victim,CLASS_VAMPIRE) && IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_ACID) )
 {
  dam *= 1.75;
  act2("The acidic content of $N's blood eats away at your insides [$D]!",ch,NULL,victim,TO_CHAR,dam);
  act2("$n doubles over in agony as the acidic content of $N's blood eats away at $s insides! [$D]",ch,NULL,victim,TO_ROOM,dam);
  GET_POWERS(ch,VAMP_BLOOD) -= dam;
  if( GET_POWERS(ch,VAMP_BLOOD) < 0 )
   GET_POWERS(ch,VAMP_BLOOD) = 0;
  ch->hit -= dam;
  update_pos(ch);
 }
 WAIT_STATE(ch,8);
 return;
}

void do_assassinate( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int dam;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 4 )
 {
  stc("You need Quietus 4 to assassinate.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Assassinate whom?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( victim == ch )
 {
  stc( "How can you assassinate yourself?\n\r", ch );
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( victim->fighting != NULL )
 {
  stc("You can't assassinate a fighting person!\n\r",ch);
  return;
 }
 if( victim->hit < (victim->max_hit * 0.80) )
 {
  act("$N is hurt and suspicious...you can't sneak up.",ch,NULL,victim,TO_CHAR);
  return;
 }
 if( IS_AWAKE(victim) )
  dam = GET_POWERS(ch,DISC_QUIETUS) * 200;
 else
  dam = GET_POWERS(ch,DISC_QUIETUS) * 300;
 if( IS_AFFECTED(ch,AFF_HIDE) )
  dam *= 1.25;
 if( IS_AFFECTED(ch,AFF_SNEAK) )
  dam *= 1.25;
 if( GET_POWERS(ch,DISC_QUIETUS) >= 10 )
  dam *= 1.25;
 dam += number_range(1,100);
 act2("Your assassination hits $N incredibly hard! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n's assassination hits you incredibly hard! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n's assassination hits $N incredibly hard! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 if( dam > victim->hit )
  dam = victim->hit-1;
 hurt_person(ch,victim,dam);
 set_fighting(ch,victim);
 if( victim->hit <= 1 )
 {
  victim->hit = 1;
  stc("You spin around and throw a headbutt to finish them!\n\r",ch);
  set_fighting(ch,victim);
  damage(ch,victim,11,gsn_headbutt);
 }
 WAIT_STATE(ch,18);
 return;
}

void do_mindblast( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int cost = 300;
 
 argument = one_argument(argument,arg);
 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_PRESENCE) < 2 )
 {
  stc("You require Presence 2 to mindblast.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to mindblast.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Mindblast whom?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( ch == victim )
 {
  stc("You cannot mindblast yourself!\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( victim->hit < (victim->max_hit * .90) )
 {
  stc("They are hurt and alert.\n\r",ch);
  return;
 }
 act("You close your eyes and concentrate on $N.",ch,NULL,victim,TO_CHAR);
 act("$n closes $s eyes and concentrates on you.",ch,NULL,victim,TO_VICT);
 act("$n closes $s eyes and concentrates on $N.",ch,NULL,victim,TO_NOTVICT);
 if( number_range(1,100) > (GET_POWERS(ch,DISC_PRESENCE) * 7) )
 {
  stc("You attempt to focus your mental energies...and fail!\n\r",ch);
  return;
 }
 WAIT_STATE(ch,12);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 act("You clutch your head in agony!",ch,NULL,victim,TO_VICT);
 act("$n clutches his head in agony!",ch,NULL,victim,TO_ROOM);
 victim->position = POS_STUNNED;
 victim->stuntimer = 2;
 update_pos(victim);
 return;
}

void do_tongue( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_SERPENTIS) < 4 )
 {
  stc("You need Serpentis 4 to tongue.\n\r",ch);
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You aren't fighting anyone.\n\r",ch);
  return;
 }
 if( !IS_NPC(victim) )
  dam = GET_POWERS(ch,DISC_SERPENTIS) * 30;
 else
  dam = GET_POWERS(ch,DISC_SERPENTIS) * 125;
 dam += number_range(1,100);
 act2("Your tongue of the serpent hits $N incredibly hard! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n's tongue of the serpent hits you incredibly hard! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n's tongue of the serpent hits $N incredibly hard! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,6);
 hurt_person(ch,victim,dam);
 return;
}

void do_vamp_gate( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 OBJ_DATA  *obj;
 char arg[MAX_INPUT_LENGTH];
 int cost = 500; 

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 1 )
 {
  stc("You need Daimoinon 1 to create a gate of blood.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Who do you wish to gate to?\n\r",ch);
  return;
 }
 if( (victim = get_char_world(ch,arg ) ) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 500 blood to create a gateway of blood.\n\r",ch);
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
 if( (victim == ch) 
  ||   victim->in_room == NULL
  ||   (!IS_NPC(victim) && !IS_IMMUNE(victim,IMM_SUMMON) )
  ||   (victim->in_room->vnum == ch->in_room->vnum)
  ||   is_ds_range(ch)
  ||   is_ds_range(victim) 
  ||   victim->in_room->vnum == 26 )
 {
  stc("You failed.\n\r",ch);
  return;
 }
 obj = create_object( get_obj_index(OBJ_VNUM_GATE),0);
 obj->value[0] = victim->in_room->vnum;
 obj->value[3] = ch->in_room->vnum;
 obj->timer = 5;
 if( IS_AFFECTED(ch,AFF_SHADOWPLANE) )
  obj->extra_flags = ITEM_SHADOWPLANE;
 obj_to_room(obj,ch->in_room);
 obj = create_object(get_obj_index(OBJ_VNUM_GATE),0);
 obj->value[0] = ch->in_room->vnum;
 obj->value[3] = victim->in_room->vnum;
 obj->timer = 5;
 if( IS_AFFECTED(victim,AFF_SHADOWPLANE) )
  obj->extra_flags = ITEM_SHADOWPLANE;
 obj_to_room(obj,victim->in_room);
 act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM);
 stc("A look of concentration passes over your face.\n\r",ch);
 act("$p appears in front of $n in a blast of flames!",ch,obj,NULL,TO_ROOM);
 act("$p appears in front of you in a blast of flames!",ch,obj,NULL,TO_CHAR);
 act("$p appears in front of $n in a blast of flames!",victim,obj,NULL,TO_ROOM);
 act("$p appears in front of you in a blast of flames!",ch, obj,victim,TO_VICT);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 return;
}

void do_pigeon( CHAR_DATA *ch, char *argument )
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 OBJ_DATA *obj;
 CHAR_DATA *victim;
 
 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_ANIMALISM) < 3 )
 {
  stc("You need Aimalism 3 to summon a pigeon messenger.\n\r",ch);
  return;
 }
 if( arg1[0] == '\0' )
 {
  stc("Pigeon which object?\n\r",ch);
  return;
 }
 if( arg2[0] == '\0' )
 {
  stc("Pigeon what to whom?\n\r",ch);
  return;
 }
 if( (obj = get_obj_carry(ch,arg1) ) == NULL )
 {
  stc("You are not carrying that item.\n\r",ch  );
  return;
 }
 if( (victim = get_char_world(ch,arg2) ) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  stc("Not on mobs.\n\r",ch);
  return;
 }
 if( IS_DAMNED(ch) ) return;
 act("You place $p in a pigeon's beak and it flies away.",ch,obj,NULL,TO_CHAR);
 act("$n places $p in a pigeon's beak and it flies away.",ch,obj,NULL,TO_ROOM);
 obj_from_char(obj);
 obj_to_char(obj,victim);
 act("A pigeon lands on your shoulders and flies away after handing you $p.",victim,obj,NULL,TO_CHAR);
 act("A pigeon lands on $n's shoulders and flies away after handing him $p.",victim,obj,NULL,TO_ROOM);
 do_autosave(ch,"");
 do_autosave(victim,"");
 return;
}

void do_embrace( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 int dam = 0;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Whom do you wish to embrace?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL)
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( victim->level >= 400 )
 {
  stc("They are too powerful for you to embrace.\n\r", ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 act("$n leaps toward $N baring $s fangs!",ch,NULL,victim,TO_ROOM);
 act("You leap toward $N baring your fangs!",ch,NULL,victim,TO_CHAR);
 act("$n leaps toward you baring $s fangs!",ch,NULL,victim,TO_VICT);
 WAIT_STATE(ch,15);
 if( victim->position != POS_STUNNED && victim->position != POS_SLEEPING )
 {
  act("$N lunges away from you!",ch,NULL,victim,TO_CHAR);
  act("$N lunges away from $n!",ch,NULL,victim,TO_ROOM);
  act("You lunge away from $n!",ch,NULL,victim,TO_VICT);
  return;
 }
 act("You bury your fangs deep into $N's neck, drinking all of $S blood!",ch,NULL,victim,TO_CHAR);
 act("$n buries $s fangs deep into $N's neck, drinking all of $S blood!",ch,NULL,victim,TO_NOTVICT);
 act("$n buries $s fangs deep into your deck, drinking all of your blood!",ch,NULL,victim,TO_VICT);
 GET_POWERS(ch,VAMP_BLOOD) += number_range(300,700);
 if( GET_POWERS(ch,VAMP_BLOOD) >= blood_max(ch) )
 {
  stc("Your bloodlust is sated.\n\r",ch);
  GET_POWERS(ch,VAMP_BLOOD) = blood_max(ch);
 }
 act("$N's body falls to the ground, lifeless.",ch,NULL,victim,TO_NOTVICT);
 act("$N's body falls to the ground, lifeless.",ch,NULL,victim,TO_CHAR);
 stc("Your body falls to the ground, lifeless.",victim);
 if( IS_NPC(victim) )
  raw_kill(victim);
 else
 {
  hurt_person(ch,victim,number_range(1000,4000));
  if( IS_CLASS(victim,CLASS_VAMPIRE) && IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_ACID) )
  {
   dam = number_range(100,500);
   act2("The acidic content of your blood causes you to scream in agony! [$D]",ch,NULL,victim,TO_CHAR,dam);
   act2("The acidic content of $N's blood causes $m to scream in agony! [$D]",ch,NULL,victim,TO_ROOM,dam);
   GET_POWERS(ch,VAMP_BLOOD) -= dam;
   if( GET_POWERS(ch,VAMP_BLOOD) < 0 )
    GET_POWERS(ch,VAMP_BLOOD) = 0;
   ch->hit -= dam;
   update_pos(ch);
  }
 }
 update_pos(victim);
 return;
}

void do_decay( CHAR_DATA *ch,char *argument )
{
 CHAR_DATA *victim;
 OBJ_DATA *obj;
 char arg[MAX_INPUT_LENGTH];
 int cost = 300;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 5 )
 {
  stc("You need Daimoinon 5 to cause decay of the flesh.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to cause decay of the flesh.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Deacy who's flesh?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 WAIT_STATE(ch,40);
 act("A look of concentration crosses over $n's face.",ch,NULL,victim,TO_ROOM);
 stc("A look of concentration crosses over your face.\n\r",ch);
 if( IS_ARM_R(victim,LOST_ARM) && IS_ARM_L(victim,LOST_ARM) &&
  IS_LEG_R(victim,LOST_LEG) && IS_LEG_L(victim,LOST_LEG) )
 {
  stc("All of their limbs have already decayed!\n\r",ch);
  return;
 }
 if( number_range(1,100) < 30 )
 {
  GET_POWERS(ch,VAMP_BLOOD) -= cost;
  if( !IS_ARM_R(victim,LOST_ARM) )
  {
   SET_BIT(victim->loc_hp[3],LOST_ARM);
   if( (obj = get_eq_char(victim,WEAR_HOLD) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_HANDS) ) != NULL )
    take_item(victim,obj);
   if( ( obj = get_eq_char(victim,WEAR_WRIST_R) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_FINGER_R) ) != NULL )
    take_item(victim,obj);
   make_part(victim,"arm");
   act("A supernatural force rips $N's arm off!",ch,NULL,victim,TO_ALL);
   return;
  }
  if( !IS_ARM_L(victim,LOST_ARM) )
  {
   SET_BIT(victim->loc_hp[2],LOST_ARM);
   if( (obj = get_eq_char(victim,WEAR_HOLD ) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_HANDS ) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_WRIST_L ) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_FINGER_L ) ) != NULL )
    take_item(victim,obj);
   make_part(victim,"arm");
   act("A supernatural force rips $N's arm off!",ch,NULL,victim,TO_ALL);
   return;
  }
  if( !IS_LEG_R(victim,LOST_LEG) )
  {
   SET_BIT(victim->loc_hp[5],LOST_LEG);
   if( (obj = get_eq_char(victim,WEAR_FEET) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_LEGS) ) != NULL )
    take_item(victim,obj);
   make_part(victim,"leg");
   act("A supernatural force rips $N's leg off!",ch,NULL,victim,TO_ALL);
   return;
  }
  if( !IS_LEG_L(victim,LOST_LEG) )
  {
   SET_BIT(victim->loc_hp[4],LOST_LEG);
   if( (obj = get_eq_char(victim,WEAR_FEET) ) != NULL )
    take_item(victim,obj);
   if( (obj = get_eq_char(victim,WEAR_LEGS) ) != NULL )
    take_item(victim,obj);
   make_part(victim,"leg");
   act("A supernatural force rips $N's leg off!",ch,NULL,victim,TO_ALL);
   return;
  }
 }
 else
 {
  stc("You failed.\n\r",ch);
  return;
 }
 return;
}

void do_infirmity( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 int sn, level, spelltype;
 int cost = 300;
    
 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 2 )
 {
  stc("You need Quietus 2 to inflict infirmity.\n\r",ch);
  return; 
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You require 300 blood to inflict infirmity.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Who do you wish to inflict infirmity on?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg ) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 if( ( sn = skill_lookup("infirmity") ) < 0 ) 
  return;
 spelltype = skill_table[sn].target;
 level = GET_POWERS(ch,DISC_QUIETUS) * 2;
 (*skill_table[sn].spell_fun) ( sn, level, ch, victim );
 WAIT_STATE( ch, 12 );
 return; 
}

void do_guardian(CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 AFFECT_DATA af;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 2 )
 {
  stc("You need Daimoinon 2 to summon a guardian.\n\r",ch);
  return;
 }
 if( IS_SET(ch->newbits, HAS_HADES) )
 {
   stc("Hades will not allow you to summon a guardian.\n\r", ch);
   return;
 }
 if( ch->pcdata->followers > 4 )
 {
  stc("Nothing happens.\n\r",ch);
  return;
 }
 victim = create_mobile( get_mob_index(MOB_VNUM_GUARDIAN) );
 victim->level = 200;
 victim->hit = 5000;
 victim->max_hit = 5000;
 victim->hitroll = 50;
 victim->damroll = 50;
 victim->armor = 300;
 SET_BIT(victim->act, ACT_NOEXP); 
 do_say(ch,"Come forth, creature of darkness, and do my bidding!");
 stc("A demon bursts from the ground and bows before you.\n\r",ch);
 act("$N bursts from the ground and bows before $n.",ch,NULL,victim,TO_ROOM);
 char_to_room(victim,ch->in_room);
 af.duration  = 666;
 af.location  = APPLY_NONE;
 af.modifier  = 0;
 af.bitvector = AFF_CHARM;
 affect_to_char(victim,&af);
 do_follow(victim,ch->name);
 ch->pcdata->followers++;
 return;
}

void do_servant(CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 AFFECT_DATA af;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 8 )
 {
  stc("You need Daimoinon 8 to create a servant.\n\r",ch);
  return;
 }
 if( IS_SET(ch->newbits, HAS_HADES) )
 {
   stc("Hades will not allow you to summon a servant.\n\r", ch);
    return;
 }
 if( ch->pcdata->followers > 4 )
 {
  stc("Nothing happens.\n\r",ch);
  return;
 }
 victim = create_mobile( get_mob_index(18018) );
 victim->level = 300;
 victim->hit = 9000;
 victim->max_hit = 9000;
 victim->hitroll = 100;
 victim->damroll = 100;
 victim->armor = -700;
 do_say(ch,"Come forth, creature of darkness, and do my bidding!");
 stc("A demonic servant bursts from the ground and bows before you.\n\r",ch);
 act("$N bursts from the ground and bows before $n.",ch,NULL,victim,TO_ROOM);
 char_to_room(victim,ch->in_room);
 victim->master = ch;
 af.duration  = 666;
 af.location  = APPLY_NONE;
 af.modifier  = 0;
 af.bitvector = AFF_CHARM;
 affect_to_char(victim,&af);
 SET_BIT(victim->act, ACT_NOEXP);
 do_follow(victim,ch->name);
 ch->pcdata->followers++;
 return;
}

void do_vamp_spit( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) ) 
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 1 )
 {
  stc("You need Quietus 1 to spit acid.\n\r",ch);
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You aren't fighting anyone.\n\r",ch);
  return;
 }
 dam = GET_POWERS(ch,DISC_QUIETUS) * number_range(30,60);
 dam += number_range(1,100);
 act2("Your spit of acid hits $N incredibly hard! [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n's spit of acid hits you incredibly hard! [$D]",ch,NULL,victim,TO_VICT,dam);
 act2("$n's spit of acid hits $N incredibly hard! [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 WAIT_STATE(ch,12);
 hurt_person(ch,victim,dam);
 if( victim->hit <= 10 )
  one_hit(ch,victim,20,gsn_headbutt);
 return;
}

void do_scales(CHAR_DATA *ch,char *argument)
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_SERPENTIS) < 5 )
 {
  stc("You need Serpentis 5 to get cover yourself in scales.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_SCALES) )
 {
  stc("Scales slide out of your body and surround you.\n\r",ch);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_SCALES);
  return;
 }
 else
 { 
  stc("Your scales slide back into your body.\n\r",ch);
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_SCALES);
  return;
 }
 return;
}

void do_cserpent( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;
 
 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_SERPENTIS) < 4 )
 {
  stc("You need Serpentis 4 to call a serpent.\n\r",ch);
  return;
 }
 if( IS_SET(ch->newbits, HAS_HADES) )
 {
   stc("Hades will not allow you to summon a serpent.\n\r", ch);
   return;
 }
 if( ch->pcdata->followers > 0 )
 {
  stc("Nothing happened.\n\r",ch);
  return;
 }
 victim = create_mobile( get_mob_index(MOB_VNUM_SERPENT) );
 if( victim == NULL )
 {
  stc("Error - Please inform Kline.\n\r",ch);
  return;
 }
 act("You close your eyes and concentrate a huge serpent walks in.",ch,NULL,victim,TO_CHAR);
 act("$n closes his eyes and concentrates a huge $N walks in.",ch,NULL,victim,TO_ROOM);
 char_to_room(victim,ch->in_room);
 SET_BIT(victim->act,ACT_NOEXP);
 SET_BIT(victim->act,ACT_MOUNT);
 return;
}

void do_scream( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *vch;
 CHAR_DATA *vch_next;
 int cost = 150;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBFUSCATE) < 2 )
 {
  stc("You must need Obfuscate 2 to scream.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < 150 )
 {
  stc("You lack the 150 blood to scream.\n\r",ch);
  return;
 }
 if( ch->fight_timer > 10 )
 {
  stc("Your fight timer is too big!\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 act("$n lets out a horrendous shrieking scream!",ch,NULL,NULL,TO_ROOM);
 act("You let out a horrendous shrieking scream!",ch,NULL,NULL,TO_CHAR);
 for( vch = ch->in_room->people; vch != NULL; vch = vch_next )
 {
  vch_next = vch->next_in_room;
  if( !IS_NPC(vch) && vch->pcdata->chobj != NULL )
   continue;
  if( ch == vch )
   continue;
  if( is_safe(ch,vch) )
   continue;
  if( (vch->hit < (vch->max_hit * 0.5) ) && !IS_NPC(vch) )
   continue;
  vch->hit -= ( GET_POWERS(ch,DISC_OBFUSCATE) * 50 );
  stc("You fall to the ground, clutching your ears!\n\r",vch);
  vch->position = POS_STUNNED;
  set_stun(vch,number_range(1,2));
  act("$n falls to the ground, stunned!",vch,NULL,vch,TO_NOTVICT);
 }
 FIGHT_TIMER(ch,10);
 return;
}

void do_bloodrod( CHAR_DATA *ch, char *argument )
{
 OBJ_DATA *obj;
 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 2 )
 {
  stc("You need Thaumaturgy 2 to create a blood rod.\n\r",ch);
  return;
 }
 obj = create_object(get_obj_index(OBJ_VNUM_BROD),0);
 obj_to_char(obj,ch);
 act("A Rod of Blood appears in your hands.",ch,NULL,NULL,TO_CHAR);
 act("A Rod of Blood appears in $n's hands.",ch,NULL,NULL,TO_ROOM);
 return;
}

void do_shadowgaze(CHAR_DATA *ch, char *argument  )
{
 char arg[MAX_INPUT_LENGTH];
 CHAR_DATA *victim;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBTEN) < 10 )
 {
  stc("You need Obtenebration 10 to shadow gaze.\n\r",ch);
  return;
 }
 if( !IS_AFFECTED(ch,AFF_SHADOWPLANE) )
 {
  stc("You need to be in the shadowplane first!\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Who do you wish to use shadow gaze on?\n\r", ch );
  return;
 }
 if( (victim = get_char_room(ch,arg)) == NULL )
 {
  stc("They arent here.\n\r",ch);
  return;
 }
 if( IS_AFFECTED(victim,AFF_SHADOWPLANE) )
 {
  stc("They are already in the Shadowplane!\n\r",ch);
  return;
 }
 WAIT_STATE(ch, 8);
 act("$n gazes deeply into $N's eyes, and pulls $M into the shadowplane!",ch,NULL,victim,TO_ROOM);
 act("You gaze deeply into $N's eyes, and pull $M into the shadowplane!",ch,NULL,victim,TO_CHAR);
 SET_BIT(victim->affected_by, AFF_SHADOWPLANE);
 return;
}
/*
void do_share( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 CHAR_DATA *familiar;
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_ANIMALISM) < 4 )
 {
  stc("You need Animalism 4 to share your spirit.\n\r",ch);
  return;
 }
 if( (familiar = ch->pcdata->familiar) != NULL )
 {
  sprintf(buf,"You release %s.\n\r",familiar->short_descr);
  stc(buf,ch);
  familiar->wizard = NULL;
  ch->pcdata->familiar = NULL;
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What do you wish to share your spirit with?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg) ) == NULL )
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( ch == victim )
 {
  stc("That would be pointless.\n\r",ch);
  return;
 }
 if( !IS_NPC(victim) )
 {
  stc("Not on players.\n\r",ch);
  return;
 }
 if( victim->wizard != NULL )
 {
  stc("Their spirit is already shared.\n\r",ch);
  return;
 }
 if( victim->level > (GET_POWERS(ch,DISC_ANIMALISM) * 4) )
 {
  stc("They are too powerful.\n\r",ch);
  return;
 }
 ch->pcdata->familiar = victim;
 victim->wizard = ch;
 act("You share your being with $N.",ch,NULL,victim,TO_CHAR);
 act("$n is staring at you!",ch,NULL,victim,TO_VICT);
 act("$n starts staring at $N",ch,NULL,victim,TO_NOTVICT);
 return;
}
*/
void do_share( CHAR_DATA *ch, char *argument )
{
return;
}
void do_frenzy( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_ANIMALISM) < 5 )
 {
  stc("You need level 5 Animalism to use Frenzy.\n\r",ch);
  return;
 }
 if( ch->rage >= (GET_POWERS(ch,VAMP_BEAST) * 3) )
 {
  stc("You're already raged, foo.\n\r",ch);
  return;
 }
 stc("You bare your fangs and growl as your inner beast consumes you.\n\r",ch);
 act("$n bares $s fangs and growls as $s inner beast consumes $m.",ch,NULL,NULL,TO_ROOM);
 ch->rage += number_range(20,30);
 WAIT_STATE(ch,12);
 return;
}

void do_drain( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 int dam;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 4 )
 {
  stc("You need Daimoinon 4 to drain the lifeforce of others.\n\r",ch);
  return;
 }
 if( (victim = ch->fighting) == NULL )
 {
  stc("You're not fighting anybody!\n\r",ch);
  return;
 }
 if( victim->hit < 500 )
 {
  stc("They are far to hurt to do that!\n\r",ch);
  return;
 }
 dam = GET_POWERS(ch,DISC_DAIMOINON) * number_range(60,110);
 dam += number_range(200,400);
 victim->hit -= dam;
 ch->hit += dam;   
 act2("You drain the lifeforce out of $N. [$D]",ch,NULL,victim,TO_CHAR,dam);
 act2("$n drains the lifeforce out of $N. [$D]",ch,NULL,victim,TO_NOTVICT,dam);
 act2("You feel $n draining your lifeforce. [$D]",ch,NULL,victim,TO_VICT,dam);
 WAIT_STATE(ch,18);
 return;
}

void do_flamehands( CHAR_DATA *ch, char *argument )
{ 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_PROTEAN) < 4 )
 {
  stc("You need Protean 4 to engulf your hands in flames.\n\r",ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLAMEHANDS) )
 {
  stc("Your hands ignite in blazing flames!\n\r",ch);
  act("$n's hands ignite in blazing flames!",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_FLAMEHANDS);
  return;
 }
 else if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLAMEHANDS) )
 {
  stc("The flames engulfing your hands fade away.\n\r",ch);
  act("The flames engulfing $n's hands fade away.",ch,NULL,NULL,TO_ROOM);
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_FLAMEHANDS);
  return;
 }
 return;
}

void do_shade( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;    
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBTEN) < 1 )
 {	  
  stc("You need Obtenebration 1 to shroud yourself in darkness.\n\r",ch);	  
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_SHADE) )
 {
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_SHADE);
  stc( "You step out from the shadows.\n\r",ch);
  act("$n emerges from the shadows.",ch,NULL,NULL,TO_ROOM);    
 }
 else 
 {
  stc("You slowly blend with the shadows.\n\r",ch);
  act("$n blends in with the shadows and disappears.",ch,NULL,NULL,TO_ROOM);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_SHADE);
 }
 return;
}

void do_bonemod( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 3 )
 {
  stc("You need Vicissitude 3 to modify your bone structure.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc( "Syntax: bonemod <modification>\n\r",ch );
  stc( "Horns, Wings, Head, Exoskeleton, Tail\n\r",ch);
  return;
 }
 if( !str_prefix(arg,"horns") )
 {
  if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_HORNS) )
  {
   SET_BIT_POWERS(ch,VAMP_BITS,BIT_HORNS);
   stc("You pull a set of horns out of your head.\n\r",ch);
   act("$n pulls a set of horns from his head!",ch,NULL,NULL,TO_ROOM);
   return;
  }
  else
  {
   REM_BIT_POWERS(ch,VAMP_BITS,BIT_HORNS);
   stc("You push your horns back into your head.\n\r",ch);
   act("$n pushes $s horns back into $s head.",ch,NULL,NULL,TO_ROOM); 
   return;
  }
 }
 if( !str_prefix(arg,"wings") )
 {
  if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_WINGS) )
  {
   SET_BIT_POWERS(ch,VAMP_BITS,BIT_WINGS);
   stc("You pull a pair of leathery wings from your back.\n\r",ch);
   act("$n pulls a pair of leathery wings from $s back!",ch,NULL,NULL,TO_ROOM);
   return;
  }
  else
  {
   REM_BIT_POWERS(ch,VAMP_BITS,BIT_WINGS);
   stc("You push your wings into your back.\n\r",ch);
   act("$n pushes $s wings into $s back.",ch,NULL,NULL,TO_ROOM); 
   return;
  }
 }
 if( !str_prefix(arg,"exoskeleton") )
 {
  if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_EXO) )
  {
   SET_BIT_POWERS(ch,VAMP_BITS,BIT_EXO);
   stc("Your skin is covered by a hard exoskeleton.\n\r",ch);
   act("$n's skin is covered by a hard exoskeleton!",ch,NULL,NULL,TO_ROOM);
   return;
  }
  else
  {
   REM_BIT_POWERS(ch,VAMP_BITS,BIT_EXO);
   stc("Your exoskeleton slowly disappears under your skin.\n\r",ch);
   act("$n's hard exoskeleton disappears under $s skin.",ch,NULL,NULL,TO_ROOM); 
   return;
  }
 }
 if( !str_prefix(arg,"tail") )
 {
  if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_TAIL) )
  {
   SET_BIT_POWERS(ch,VAMP_BITS,BIT_TAIL);
   stc("Your spine extends out into a long, pointed tail.\n\r",ch);
   act("$n's spine extends to form a long pointed tail!",ch,NULL,NULL,TO_ROOM);
   return;
  }
  else
  {
   REM_BIT_POWERS(ch,VAMP_BITS,BIT_TAIL);
   stc("Your tail slowly retracts into your spine.\n\r",ch);
   act("$n's tail shrinks and vanishes into $s spine.",ch,NULL,NULL,TO_ROOM); 
   return;
  }
 }  
 if( !str_prefix(arg,"head") )
 {
  if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_HEAD) )
  {
   SET_BIT_POWERS(ch,VAMP_BITS,BIT_HEAD);
   stc("Your head transforms into that of a fierce lion.\n\r",ch);
   act("$n's head transforms into that of a fierce lion!",ch,NULL,NULL,TO_ROOM);
   return;
  }
  else
  {
   REM_BIT_POWERS(ch,VAMP_BITS,BIT_HEAD);
   stc("Your head slowly resumes it's normal form.\n\r",ch);
   act("$n's head resumes its normal form.",ch,NULL,NULL,TO_ROOM); 
   return;
  }
 } 
 do_bonemod(ch,"");
 return;
}

void vamp_disciplines( CHAR_DATA *ch, char *argument, char *argument2 )
{
 char arg[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 argument2 = one_argument(argument2,arg2);

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
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
  if( !str_cmp(arg2,"obtenebration") )
  {
   improve_class(ch,DISC_OBTEN);
   return;
  }
  else if( !str_cmp(arg2,"presence") )
  {
   improve_class(ch,DISC_PRESENCE);
   return;
  }
  else if( !str_cmp(arg2,"quietus") )
  {
   improve_class(ch,DISC_QUIETUS);
   return;
  }
  else if( !str_cmp(arg2,"thaumaturgy") )
  {
   improve_class(ch,DISC_THAUM);
   return;
  }
  else if( !str_cmp(arg2,"auspex") )
  {
   improve_class(ch,DISC_AUSPEX);
   return;
  }
  else if( !str_cmp(arg2,"dominate") )
  {
   improve_class(ch,DISC_DOMINATE);
   return;
  }
  else if( !str_cmp(arg2,"obfuscate") )
  {
   improve_class(ch,DISC_OBFUSCATE);
   return;
  }
  else if( !str_cmp(arg2,"protean") )
  {
   improve_class(ch,DISC_PROTEAN);
   return;
  }
  else if( !str_cmp(arg2,"serpentis") )
  {
   improve_class(ch,DISC_SERPENTIS);
   return;
  }
  else if( !str_cmp(arg2,"vicissitude") )
  {
   improve_class(ch,DISC_VICIS);
   return;
  }
  else if( !str_cmp(arg2,"daimoinon") )
  {
   improve_class(ch,DISC_DAIMOINON);
   return;
  }
  else if( !str_cmp(arg2,"animalism") )
  {
   improve_class(ch,DISC_ANIMALISM);
   return;
  }
  else if( !str_cmp(arg2,"celerity") )
  {
   improve_class(ch,DISC_CELERITY);
   return;
  }
  else if( !str_cmp(arg2,"potence") )
  {
   improve_class(ch,DISC_POTENCE);
   return;
  }
  else if( !str_cmp(arg2,"fortitude") )
  {
   improve_class(ch,DISC_FORTITUDE);
   return;
  }
  else
  {
   stc("Learn which discipline?\n\r",ch);
   return;
  }
  return;
 }
 else if( !str_cmp(arg,"obtenebration") )
 {
  if( GET_POWERS(ch,DISC_OBTEN) == 0 )
	stc("You have no knowledge of the Obtenebration discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"presence") )
 {
  if( GET_POWERS(ch,DISC_PRESENCE) == 0 )
	stc("You have no knowledge of the Presence discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"quietus") )
 {
  if( GET_POWERS(ch,DISC_QUIETUS) == 0 )
	stc("You have no knowledge of the Quietus discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"thaumaturgy") )
 {
  if( GET_POWERS(ch,DISC_THAUM) == 0 )
	stc("You have no knowledge of the Thaumaturgy discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"auspex") )
 {
  if( GET_POWERS(ch,DISC_AUSPEX) == 0 )
	stc("You have no knowledge of the Auspex discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"dominate") )
 {
  if( GET_POWERS(ch,DISC_DOMINATE) == 0 )
	stc("You have no knowledge of the Dominate discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"obfuscate") )
 {
  if( GET_POWERS(ch,DISC_OBFUSCATE) == 0 )
	stc("You have no knowledge of the OBFUSCATE discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"protean") )
 {
  if( GET_POWERS(ch,DISC_PROTEAN) == 0 )
	stc("You have no knowledge of the Protean discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"sepentis") )
 {
  if( GET_POWERS(ch,DISC_SERPENTIS) == 0 )
	stc("You have no knowledge of the Serpentis discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"vicissitude") )
 {
  if( GET_POWERS(ch,DISC_VICIS) == 0 )
	stc("You have no knowledge of the Vicissitude discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"daimoinon") )
 {
  if( GET_POWERS(ch,DISC_DAIMOINON) == 0 )
	stc("You have no knowledge of the Daimoinon discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"animalism") )
 {
  if( GET_POWERS(ch,DISC_ANIMALISM) == 0 )
	stc("You have no knowledge of the Animalism discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"celerity") )
 {
  if( GET_POWERS(ch,DISC_CELERITY) == 0 )
	stc("You have no knowledge of the Celerity discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"potence") )
 {
  if( GET_POWERS(ch,DISC_POTENCE) == 0 )
	stc("You have no knowledge of the Potence discipline.\n\r",ch);
  return;
 }
 else if( !str_cmp(arg,"fortitude") )
 {
  if( GET_POWERS(ch,DISC_FORTITUDE) == 0 )
	stc("You have no knowledge of the Fortitude discipline.\n\r",ch);
  return;
 }
 return;
}

void do_vamppowers( CHAR_DATA *ch, char *argument )
{
 stc("Obtenebration: tendrils, lamprey, shadowgaze, shade, shadowstep\n\r",ch);
 stc("Presence     : entrance, mindblast, summon\n\r",ch);
 stc("Quietus      : sharpen, darkness, assassinate, infirmity, spit, flash, death\n\r",ch);
 stc("Thamaturgy   : gourge, bloodwater, spew, bloodrod, tide, potency, theft\n\r",ch);
 stc("Auspex       : scry, penetrating vision\n\r",ch);
 stc("Dominate     : fcommand\n\r",ch);
 stc("Obfuscate    : conceal, scream, return\n\r",ch);
 stc("Protean      : flamehands, earthmeld, change\n\r",ch);
 stc("Serpentis    : tongue, scales, cserpent, coil, poisonweapon\n\r",ch);
 stc("Vicissitude  : dragonform, objform, fleshcraft, bonemod, clone, acidblood, rot, plasma, zuloform\n\r",ch);
 stc("Daimoinon    : baal, inferno, zombie, bloodgate, decay, guardian, servant, drain, hades\n\r",ch);
 stc("Animalism    : pigeon, share, frenzy\n\r",ch);
 stc("Celerity     : *attacks, parry/dodge*\n\r",ch);
 stc("Potence      : *offensive damage mod*\n\r",ch);
 stc("Fortitude    : *defensive damage mod*\n\r",ch);
 stc("Innate       : embrace, regenerate\n\r",ch);
 return;
}

void do_clone( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 OBJ_DATA *obj;
 int cost = 50; 

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 1 )
 {
  stc("You need Vicissitude 1 to clone an item.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 50 blood require to clone an item.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Make a clone of what object?\n\r",ch);
  return;
 }
 if( (obj = get_obj_carry(ch,arg)) == NULL )
 {
  stc( "You dont have that item.\n\r",ch);
  return;
 }
 if( IS_SET(obj->extra_flags, ITEM_ONE_RING) )
 {
   stc("You are not powerful enough to clone Sauron's magic!\n\r", ch);
   return;
 }
 if( obj->pIndexData->vnum >= 26000 && obj->pIndexData->vnum <= 26058 )
 {
	stc("You cannot clone objects from a dream!\n\r", ch);
	return;
 }
 if( obj->points > 0
  || IS_SET(obj->extra_flags,ITEM_FORGED)
  || IS_SET(obj->quest,QUEST_ARTIFACT)
  || IS_SET(obj->quest,QUEST_RELIC)
  || obj->chobj != NULL
  || obj->item_type == ITEM_GEMSTONE 
  || obj->item_type == ITEM_HILT     
  || obj->item_type == ITEM_COPPER   
  || obj->item_type == ITEM_IRON     
  || obj->item_type == ITEM_STEEL    
  || obj->item_type == ITEM_ADAMANTITE
  || obj->item_type == ITEM_QUEST
  || obj->item_type == ITEM_QUESTCARD
  || obj->item_type == ITEM_GEM
  || obj->item_type == ITEM_ORE
  || obj->item_type == ITEM_METAL)
 {
  stc("You cant clone that object.\n\r",ch);
  return;
 }
 do_oclone(ch,arg);
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 WAIT_STATE(ch,6);
 return;
}

void do_coil( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_SERPENTIS) < 8 )
 {
  stc("You need Serpentis 8 to coil your body.\n\r", ch);
  return;
 }
 if( !IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_COIL) )
 {
  stc("You prepare to coil your victims.\n\r",ch);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_COIL);
  return;
 }
 else
 {
  stc("You relax your body once again.\n\r",ch);
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_COIL);
  return;
 }
 return;
}

void do_tide( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 10 )
 {
  stc("You need Thaumaturgy 10 to use summon a tide of vitae.\n\r", ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < blood_max(ch) )
 {
  stc("You require full blood to summon a tide of vitae.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_TIDE) )
 {
  stc("The tide is already with you.\n\r",ch);
  return;
 }
 SET_BIT_POWERS(ch,VAMP_BITS,BIT_TIDE);
 stc("You feel a tide of vitae rush over you.\n\r",ch);
 GET_POWERS(ch,VAMP_BLOOD) = 0;
 WAIT_STATE(ch,20);
 return;
}

void do_flash( CHAR_DATA *ch, char *argument )
{
 int cost = 300;
 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 9 )
 {
  stc("You need Quietus 9 to use to quicken your reflexes.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to quicken your reflexes.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_FLASHING) )
 {
  stc("You are already moving swiftly.\n\r",ch);
  return;
 }
 SET_BIT_POWERS(ch,VAMP_BITS,BIT_FLASHING);
 GET_POWERS(ch,DISC_CELERITY)++;
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 act("$n starts moving with lightening speed.",ch,NULL,NULL,TO_ROOM);
 stc("You start moving with lightening speed.\n\r",ch);
 WAIT_STATE(ch,20);
 return;
}

void do_death( CHAR_DATA *ch, char *argument )
{
 ROOM_INDEX_DATA *room;
 int cost = 200;
 
 room = ch->in_room;
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_QUIETUS) < 5 )
 {
  stc("You need Quietus 5 to use create a deathly silence.\n\r",ch);
  return;
 }
 if( RTIMER(ch->in_room,RTIMER_SILENCE) != 0 )
 {
  act("The silence leaves the room.",ch,NULL,NULL,TO_ALL);
  SET_RTIMER(ch->in_room,RTIMER_SILENCE,0);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 200 blood to create a deathly silence.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 SET_RTIMER(room,RTIMER_SILENCE,10);
 act("A look of concentration passes over $n's face.",ch,NULL,NULL,TO_ROOM);
 stc("A look of concentration passes over your face.\n\r",ch);
 act("An eerie silence fills the room.",ch,NULL,NULL,TO_ALL);
 return;
}

void do_acid( CHAR_DATA *ch, char *argument )
{
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 9 )
 {
  stc("You Vicissitude 9 to acidify your blood.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) != blood_max(ch) )
 {
  stc("Your blood must be full to change the acid content of it.\n\r", ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) = 0;
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_ACID) )
 {
  stc("Your blood returns to normal.\n\r",ch);
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_ACID);
  return;
 }
 else
 {
  stc("Your blood becomes acidic.\n\r",ch);
  SET_BIT_POWERS(ch,VAMP_BITS,BIT_ACID);
  return;
 }
 return;
}

void do_retrn( CHAR_DATA *ch, char *argument )
{
 ROOM_INDEX_DATA *location;
 int cost = 300;

 location = get_room_index(27000);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBFUSCATE) < 8 )
 {
  stc("You need Obfuscate 8 to return to Caine.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to return to Caine.\n\r",ch);
  return;
 }
 if( ch->in_room == location )
  return;
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 act("$n raises $s arms to the sky.",ch,NULL,NULL,TO_ROOM);
 stc("Your raise your arms to the sky.",ch);
 act("$n disappears.",ch,NULL,NULL,TO_ROOM);
 char_from_room(ch);
 char_to_room(ch,location);
 act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
 do_look(ch,"auto");
 return;
}

void do_rot( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
 
 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 6 )
 {
  stc("You need Vicissitude 6 to cause flesh to rot.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Cause who's flesh to rot?\n\r",ch);
  return;
 }
 if( (victim = get_char_room(ch,arg)) == NULL )
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
 act("You close your eyes and concentrate on $N.",ch,NULL,victim,TO_CHAR);
 act("$n closes $s eyes and concentrates on you.",ch,NULL,victim,TO_VICT);
 act("$n closes $s eyes and cencentrates on $N.",ch,NULL,victim,TO_NOTVICT);
 WAIT_STATE(ch,12);
 act("Your flesh begins to rot!", victim,NULL,NULL,TO_CHAR);
 act("$n's flesh begins to rot!", victim,NULL,NULL,TO_ROOM);
 SET_BIT(victim->extra, EXTRA_ROT);
 return;
}        

void do_hades( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 AFFECT_DATA af;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DAIMOINON) < 10 )
 {
  stc("You need Daimoinon 10 to summon Hades to your aid.\n\r",ch);
  return;
 }
 if( ch->pcdata->followers == 0 )
  REMOVE_BIT(ch->newbits, HAS_HADES);
 if( IS_SET(ch->newbits, HAS_HADES) )
 {
   stc("Only one Satan can exist in this world.\n\r", ch);
   return;
 }
 if( ch->pcdata->followers > 0)
 {
  stc("Hades refuses to come to your aid while others follow you.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < blood_max(ch) )
 {
  stc("Your blood must be full to summon Hades to your aid.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) = 0;
 victim = create_mobile(get_mob_index(8));
 victim->level = 350;
 victim->hit = 10000;
 victim->max_hit = 10000;
 victim->hitroll = 500;
 victim->damroll = 500;
 victim->armor = -2000;
 do_say(ch,"I command you, Hades, to rise before my feet and crush all who oppose me with your might!");
 act("The ground roars and shakes as $N bursts forth from it!",ch,NULL,victim,TO_ALL);
 char_to_room(victim,ch->in_room);
 af.duration  = 666;
 af.location  = APPLY_NONE;
 af.modifier  = 0;
 af.bitvector = AFF_CHARM;
 affect_to_char(victim,&af);
 SET_BIT(victim->act,ACT_NOEXP);
 do_follow(victim,ch->name);
 ch->pcdata->followers++;
 SET_BIT(ch->newbits, HAS_HADES);
 return;
}

void do_potency( CHAR_DATA *ch, char *argument )
{

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 3 )
 { 
  stc("You need Thaumaturgy 3 to make your blood more potent.\n\r",ch);
  return;
 }
 if( ch->generation == 1 )
 {
  stc("Your blood is already as potent as that of Caine!\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) != blood_max(ch) )
 {
  stc("Your blood must be full to make it more potent.\n\r", ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_POTENCE) )
 {
  stc("Your blood is already more potent.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) = 0;
 stc("Your blood becomes more potent.\n\r",ch);
 SET_BIT_POWERS(ch,VAMP_BITS,BIT_POTENCE);
 ch->generation--;
 WAIT_STATE(ch,20);
 return;
}


void do_plasma( CHAR_DATA *ch, char *argument )
{
 OBJ_DATA *obj;

 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 stc("COMMAND TEMPORARILY DISABLED\n\r", ch);
 return;
 if( GET_POWERS(ch,DISC_VICIS) < 5 )
 { 
  stc("You need Vicissitude 5 to turn into plasma.\n\r",ch);
  return;    
 }
 save_char_obj(ch);
 if( (obj = create_object(get_obj_index(30007),60)) == NULL )
 {
  stc("[ERROR] Missing object, 30007.  Inform Kline immediately.\n\r",ch);	
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) == FORM_PLASMA )    
 {      
  ch->pcdata->obj_vnum = 0;
  GET_POWERS(ch,VAMP_FORM) = FORM_NORMAL;
  REM_BIT_POWERS(ch,VAMP_BITS,BIT_DISGUISE);
  REMOVE_BIT(ch->extra,EXTRA_OSWITCH);
  ch->pcdata->chobj = NULL;
  obj->chobj = NULL;
/*  free_string(ch->morph);
  ch->morph = str_dup("");
  */act("$p transforms into $n.",ch,obj,NULL,TO_ROOM);
  act("Your reform your human body.",ch,obj,NULL,TO_CHAR);
  extract_obj(obj);
  return;
 }
 if( ch->fight_timer > 0 )
 {	
  stc("You cannot use plasma with your fight timer active.\n\r",ch);
  return;	
 }        
 if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )   
 {	
  stc("You need to be in human form.\n\r",ch);
  return;  
 }    
 act("$n transforms into $p and splashes to the ground.",ch,obj,NULL,TO_ROOM); 
 act("You transform into $p and splashes to the ground.",ch,obj,NULL,TO_CHAR);   
 ch->pcdata->obj_vnum = 30007;
 obj->chobj = ch;   
 ch->pcdata->chobj = obj;
 GET_POWERS(ch,VAMP_FORM) = FORM_PLASMA;
 SET_BIT_POWERS(ch,VAMP_BITS,BIT_DISGUISE);   
 SET_BIT(ch->extra,EXTRA_OSWITCH); 
/* free_string(ch->morph);    
 ch->morph = str_dup("a pool of blood");
 */obj_to_room(obj,ch->in_room);  
 return;
}

void do_shadowstep( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_OBTEN) < 7 )
 {
  stc("You need Obtenebration 7 to step through the shadows.\n\r",ch);
  return;
 }
 if( weather_info.sunlight != SUN_SET && weather_info.sunlight != SUN_DARK )
 {
  stc("You can't step through shadows in daylight!\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Shadowstep to who?\n\r",ch);
  return;
 }
 if( ( victim = get_char_world( ch, arg ) ) == NULL )
 {
  stc( "They aren't here.\n\r", ch );
  return;
 }
 if( ch == victim )
 {
  stc("Not too bright, are you?!\n\r",ch);
  return;
 }
 if( IS_IMMORTAL(victim) )
 {
  stc("Not to immortals!\r\n", ch);
  return;
 }
 if( is_ds_range(victim) )
 {
  stc("You can't shadowstep into a dream.\n\r", ch);
  return;
 }
 if( ch->fight_timer > 0 )
 {
  stc("Not with a fight timer!\n\r",ch);
  return;
 }
 if( victim->in_room == ch->in_room )
 {
  stc("But you're already there!\n\r",ch);
  return;
 }
 char_from_room(ch);
 char_to_room(ch,victim->in_room);
 act("You step into the shadows and appear before $N.", ch, NULL, victim, TO_CHAR);
 act("$n appears out of the shadows before $N.", ch, NULL, victim, TO_NOTVICT);
 act("$n appears from the shadows in front of you.", ch, NULL, victim, TO_VICT);
 do_look(ch,"auto");
 return;
}

void do_earthmeld( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_PROTEAN) < 4 )
 {
  stc("You need Protean 4 to meld into the earth.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
 {
  stc("You must be in human form.\n\r",ch);
  return;
 }
 if( ch->fight_timer > 0 )
 {
  stc("Not with a fight timer.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) == FORM_EARTH )
 {
  stc("You rise up from the ground.\n\r",ch);
  act("$n rises up from the ground.",ch,NULL,NULL,TO_ROOM);
  GET_POWERS(ch,VAMP_FORM) = FORM_NORMAL;
  return;
 }
/* free_string(ch->morph);
 ch->morph = str_dup("A mound of earth");
 */stc("You sink into the ground.\n\r",ch);
 act("$n sinks into the ground.",ch,NULL,NULL,TO_ROOM);
 return;
}

void do_theft( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_THAUM) < 4 )
 {
  stc("You need Thaumaturgy 4 to use the theft of vitae.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Steal who's blood?\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
 {
  stc("You need to be in human form.\n\r",ch);
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
 if( IS_CLASS(victim,CLASS_VAMPIRE) && GET_POWERS(victim,VAMP_BLOOD) < 1 )
 {
  stc( "There isn't enough blood to steal.\n\r", ch);
  return;
 }
 if( is_safe(ch,victim) )
 {
  stc("Not while safe.\n\r",ch);
  return;
 }
 act("A stream of blood shoots from $N into your body!",ch,NULL,victim,TO_CHAR);
 act("A stream of blood shoots from $N into $N!",ch,NULL,victim,TO_ROOM);
 act("A stream of blood shoots from your body into $N!",ch,NULL,victim,TO_VICT);
        
 GET_POWERS(ch,VAMP_BLOOD) = GET_POWERS(victim,VAMP_BLOOD);
 GET_POWERS(victim,VAMP_BLOOD) = 0;
 if( GET_POWERS(ch,VAMP_BLOOD) > blood_max(ch) )
 {
  stc("Your bloodlust is sated.\n\r",ch);
  GET_POWERS(ch,VAMP_BLOOD) = blood_max(ch);
 }
 if( IS_SET(GET_POWERS(victim,VAMP_BITS),BIT_ACID) )
 {
  stc("Their blood is a deadly acid!\n\r",ch);
  ch->hit -= (GET_POWERS(ch,VAMP_BLOOD) * 2);
  return;
 }
 return;
}

void do_zuloform( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 int cost = 300;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_VICIS) < 4 )
 {
  stc("You need Vicissitude 4 to transform into a large beast.\n\r",ch);
  return;
 }
 if( (GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL) && (GET_POWERS(ch,VAMP_FORM) != FORM_ZULO) )
 {
  stc("You can only transform while in Human or Zulo form.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_FORM) == FORM_ZULO )
 {
  GET_POWERS(ch,VAMP_FORM) = FORM_NORMAL;
  stc("You transform back into humanform.",ch);
  act("$n shrinks back into a human.",ch,NULL,NULL,TO_ROOM);
//  free_string(ch->morph);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 300 blood to transform into a large beast.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 stc("You transform into a large beast!",ch);
 act("$n's body grows and distorts into a large beast!",ch,NULL,NULL,TO_ROOM);
 GET_POWERS(ch,VAMP_FORM) = FORM_ZULO;
/* free_string(ch->morph);
 ch->morph = str_dup("A large black monster");
 */return;
}

void do_change( CHAR_DATA *ch, char *argument )
{
 char arg[MAX_INPUT_LENGTH];
 int cost = 100;

 argument = one_argument(argument,arg); 
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_PROTEAN) < 3 )
 {
  stc("You need Protean 3 to change your form.\n\r",ch);
  return;
 }
 if( IS_SET(GET_POWERS(ch,VAMP_BITS),BIT_DISGUISE) )
 {
  stc("You can't change forms while assuming the form of another.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r",ch);
  return;
 }
 if( !str_cmp(arg,"bat") )
 {
  if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
  {
   stc("You can only polymorph from human form.\n\r",ch);
   return;
  }
  if( GET_POWERS(ch,VAMP_BLOOD) < cost )
  {
   stc("You lack the 100 blood to change forms.\n\r",ch);
   return;
  }
  GET_POWERS(ch,VAMP_BLOOD) -= cost;
  act( "You transform into bat form.", ch, NULL, NULL, TO_CHAR );
  act( "$n transforms into a bat.", ch, NULL, NULL, TO_ROOM );
  GET_POWERS(ch,VAMP_FORM) = FORM_BAT;
  sprintf(arg,"%s the vampire bat",ch->name);
/*  free_string(ch->morph);
  ch->morph = str_dup(arg);
  */return;
 }
 else if( !str_cmp(arg,"wolf") )
 {
  if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
  {
   stc("You can only polymorph from human form.\n\r",ch);
   return;
  }
  if( GET_POWERS(ch,VAMP_BLOOD) < cost )
  {
   stc("You lack the 100 blood to change forms.\n\r",ch);
   return;
  }
  GET_POWERS(ch,VAMP_BLOOD) -= cost;
  act( "You transform into wolf form.", ch, NULL, NULL, TO_CHAR );
  act( "$n transforms into a dire wolf.", ch, NULL, NULL, TO_ROOM );
  GET_POWERS(ch,VAMP_FORM) = FORM_WOLF;
  sprintf(arg,"%s the dire wolf",ch->name);
/*  free_string(ch->morph);
  ch->morph = str_dup(arg);
  */return;
 }
 else if( !str_cmp(arg,"mist") )
 {
  if( GET_POWERS(ch,VAMP_FORM) != FORM_NORMAL )
  {
   stc("You can only polymorph from human form.\n\r",ch);
   return;
  }
  if( GET_POWERS(ch,VAMP_BLOOD) < cost )
  {
   stc("You lack the 100 blood to change forms.\n\r",ch);
   return;
  }
  GET_POWERS(ch,VAMP_BLOOD) -= cost;
  act( "You transform into mist form.", ch, NULL, NULL, TO_CHAR );
  act( "$n transforms into a white mist.", ch, NULL, NULL, TO_ROOM );
  GET_POWERS(ch,VAMP_FORM) = FORM_MIST;
  SET_BIT(ch->affected_by,AFF_ETHEREAL);
  sprintf(arg,"%s the white mist",ch->name);
/*  free_string(ch->morph);
  ch->morph = str_dup(arg);
  */if( IS_EXTRA(ch,TIED_UP) )
  {
   act("The ropes binding you fall through your ethereal form.",ch,NULL,NULL,TO_CHAR);
   act("The ropes binding $n fall through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
   REMOVE_BIT(ch->extra,TIED_UP);
   REMOVE_BIT(ch->extra,GAGGED);
   REMOVE_BIT(ch->extra,BLINDFOLDED);
  }
  if( IS_AFFECTED(ch,AFF_WEBBED) )
  {
   act("The webbing entrapping $n falls through $s ethereal form.",ch,NULL,NULL,TO_ROOM);
   stc("The webbing entrapping you falls through your ethereal form.\n\r",ch);
   REMOVE_BIT(ch->affected_by,AFF_WEBBED);
  }
  return;
 }
 else if ( !str_cmp(arg,"human") )
 {
  if( GET_POWERS(ch,VAMP_FORM) == FORM_NORMAL )
  {
   stc( "You are already in human form.\n\r", ch );
   return;
  }
  if( GET_POWERS(ch,VAMP_FORM) == FORM_MIST )
   REMOVE_BIT(ch->affected_by, AFF_ETHEREAL);
  act( "You transform into human form.", ch, NULL, NULL, TO_CHAR );
  act( "$n transforms into human form.", ch, NULL, NULL, TO_ROOM );
  GET_POWERS(ch,VAMP_FORM) = FORM_NORMAL;
/*  free_string(ch->morph);
  ch->morph = str_dup("");
  */return;
 }
 else
  stc("You can change between 'human', 'bat', 'wolf' and 'mist' forms.\n\r",ch);
 return;
}

void do_poisonw( CHAR_DATA *ch, char *argument )
{
 OBJ_DATA  *obj;
 char arg[MAX_INPUT_LENGTH];
 int cost = 100;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_SERPENTIS) < 3 )
 {
  stc("You need Serpentis 3 to use poison your blades.\n\r",ch);
  return;
 }
 if( ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL )
  && ((obj = get_eq_char(ch,WEAR_HOLD)) == NULL) )
 {
  stc("You must wield the weapon you wish to poison.\n\r",ch);
  return;
 }
 if( obj->value[0] >= 18025 )
 {
  stc("This weapon cannot be poisoned.\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 100 blood to coat a weapon with poison.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 act("You run your tongue along $p, poisoning it.",ch,obj,NULL,TO_CHAR);
 act("$n runs $s tongue along $p, poisoning it.",ch,obj,NULL,TO_ROOM);
 if( obj->value[0] == 18000 )
  obj->value[0] += 24;
 else
  obj->value[0] = 24;
 obj->level = 100;
 return;
}

void do_regenerate( CHAR_DATA *ch, char *argument )
{
 char arg [MAX_INPUT_LENGTH];
 int cost = 25;
 int gain = 0;

 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( ch->rage > 0 )
 {
  stc("You cannot control your regenerative powers while you are filled with rage.\n\r",ch);
  return;
 }
 if( ch->position == POS_FIGHTING )
 {
  stc("You cannot regenerate while fighting.\n\r",ch);
  return;
 }
 if( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
 {
  stc("But you are already completely regenerated!\n\r",ch);
  return;
 }
 if( GET_POWERS(ch,VAMP_BLOOD) < cost )
 {
  stc("You lack the 25 blood to regenerate yourself.\n\r",ch);
  return;
 }
 GET_POWERS(ch,VAMP_BLOOD) -= cost;
 if (ch->hit < 1 )
 {
  gain = 1;
  WAIT_STATE(ch,6);
 }
 else
 {
  gain = ( (cost * (GET_POWERS(ch,VAMP_AGE) * 1.5)) + number_range(100,200) );
  WAIT_STATE(ch,8);
 }
 ch->hit += gain;
 ch->mana += gain;
 ch->move += gain;
 if( ch->hit >= ch->max_hit && ch->mana >= ch->max_mana && ch->move >= ch->max_move )
 {
  stc("Your body has completely regenerated.\n\r",ch);
  act("$n's body completely regenerates itself.",ch,NULL,NULL,TO_ROOM);
 }
 else
 {
  act2("Your body slowly regenerates itself. [$D]",ch,NULL,NULL,TO_CHAR,gain);
  act2("$n's body slowly regenerates itself. [$D]",ch,NULL,NULL,TO_ROOM,gain);
 }
 return;
}

void do_vamp_summon(CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg [MAX_INPUT_LENGTH];
    
 argument = one_argument(argument,arg);
 if( IS_NPC(ch) )
  return;
 if( GET_POWERS(ch,DISC_PRESENCE) < 4 )
 {
  stc("You need Presence 4 to summon another.\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("Summon whom?\n\r",ch);
  return;
 }
 if( (victim = get_char_world(ch,arg)) == NULL)
 {
  stc("They aren't here.\n\r",ch);
  return;
 }
 if( victim == ch
  ||   victim->in_room == NULL
  ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
  ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
  ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
  ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
  ||   victim->fighting != NULL
  ||   (!IS_NPC(victim) && !IS_IMMUNE(victim, IMM_SUMMON) )
  ||   is_ds_range(victim) 
  ||   victim->level >= 400 )

 {
  stc("You failed.\n\r",ch);
  return;
 }
 WAIT_STATE(ch,12);
 act("$n disappears suddenly.",victim,NULL,NULL,TO_ROOM);
 char_from_room(victim);
 char_to_room(victim,ch->in_room);
 act("$n arrives suddenly.",victim,NULL,NULL,TO_ROOM);
 act("$N has summoned you!",victim,NULL,ch,TO_CHAR);
 do_look(victim,"auto");
 return;
}
/*
void do_fcommand( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];

 arg = argument;
 if( IS_NPC(ch) )
  return;
 if( !IS_CLASS(ch,CLASS_VAMPIRE) )
 {
  do_huh(ch,"");
  return;
 }
 if( GET_POWERS(ch,DISC_DOMINATE) < 3 )
 {
  stc("You need Dominate 3 to command others over great distances.\n\r",ch);
  return;
 }
 if( (victim = ch->pcdata->familiar) == NULL )
 {
  stc("You have no familiar to command!\n\r",ch);
  return;
 }
 if( arg[0] == '\0' )
 {
  stc("What do you wish to make your familiar do?\n\r",ch);
  return;
 }
 interpret(victim,argument);
 return;
}
*/
void do_fcommand( CHAR_DATA *ch, char *argument )
{
return;
}
/*
void do_testemb(CHAR_DATA *ch, char *argument  )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    one_argument( argument,arg);
 
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch,CLASS_VAMPIRE))
    {
	stc("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' && ch->embracing==NULL)
    {
        stc( "Who do you wish to embrace?\n\r", ch );
	return;
    }

    if (ch->embracing != NULL)
    {
	if ((victim=ch->embracing) != NULL)
	{
	    stc("You retract your fangs.",ch);
	    act("$N retracts his fangs.",ch,NULL,NULL,TO_ROOM);
	    stop_embrace(ch,victim);
	    return;
	}
    }

    if ((victim = get_char_room( ch, arg)) == NULL && ch->embracing==NULL)
    {
	stc("They arent here.\n\r",ch);
	return;
    }

    if (IS_NPC(victim) || !IS_CLASS(ch,CLASS_VAMPIRE))
    {
	stc("You can only embrace vampires.\n\r",ch);
	return;
    }
  
    if (victim->position != POS_MORTAL)
    {
	stc("You can only embrace mortally wounded vampires.\n\r",ch);
	return;
    }
    sprintf(buf, "%s leaps toward %s baring his fangs.",ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf, "You leap toward %s baring your fangs.\n\r",victim->name);
    stc(buf,ch);
    WAIT_STATE(ch, 15);
    
    sprintf(buf, "You sink your teeth into their throat.\n\r");
    stc(buf,ch);
    sprintf(buf, "%s sinks their teeth into %s's throat.",
	ch->name,victim->name);
    act(buf,ch,NULL,NULL,TO_ROOM);
    sprintf(buf, "%s sinks their teeth into your throat.\n\r",ch->name);
    stc(buf,victim);
    victim->embraced=ch;
    ch->embracing=victim;
 
    return;
}
*/
