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

char* get_status_name( CHAR_DATA *ch)
{
  if( IS_NPC(ch))
   return NULL;
  if( ch->level == LEVEL_IMPLEMENTOR)
   return config_info->imm12;
  if( ch->level == LEVEL_HIGHJUDGE)
   return config_info->imm11;
  if( ch->level == LEVEL_JUDGE)
   return config_info->imm10;
  if( ch->level == LEVEL_ENFORCER)
   return config_info->imm09;
  if( ch->level == LEVEL_QUESTMAKER)
   return config_info->imm08;
  if( ch->level == LEVEL_BUILDER)
   return config_info->imm07;
  if( (ch->level < LEVEL_BUILDER) && (ch->level > 2))
  {
   if( ch->race == 0)
    return config_info->title_none;
   if( (ch->race >= 1) && (ch->race < 5))
    return config_info->title00;
   if( (ch->race >= 5) && (ch->race < 10))
    return config_info->title05;
   if( (ch->race >= 10) && (ch->race < 15))
    return config_info->title10;
   if( (ch->race >= 15) && (ch->race < 20))
    return config_info->title15;
   if( (ch->race >= 20) && (ch->race < 25))
    return config_info->title20;
   if( (ch->race >= 25) && (ch->race < 30))
    return config_info->title25;
   if( (ch->race >= 30) && (ch->race < 35))
    return config_info->title30;
   if( (ch->race >= 35) && (ch->race < 40))
    return config_info->title35;
   if( ch->race >= 40)
    return config_info->title40;
  }
  return "Mortal";
}

char* get_class_name( CHAR_DATA *ch)
{
  switch( ch->class)
  {
   case CLASS_NONE:
    return "None"; break;
   case CLASS_APOCRYPHIC:
    return "Apocryphic"; break;
   case CLASS_ALIEN:
    return "Alien"; break;
   case CLASS_CYBORG:
    return "Cyborg"; break;
   case CLASS_MONK:
    return "Monk"; break;
   case CLASS_SHADOW:
    return "Shadow"; break;
   case CLASS_WOLF:
    return "Werewolf"; break;
   case CLASS_JEDI:
    return "Jedi"; break;
   case CLASS_VAMPIRE:
    return "Vampire"; break;
   case CLASS_DRAGON:
    return "Dragon"; break;
   default:
    return "Unkown"; break;
  }
  return "BUG";
}

char* get_class_rank( CHAR_DATA *ch)
{
  if( ch->class == CLASS_MONK)
  {
   switch(GET_POWERS(ch, MONK_CLASS))
   {
    default:
     return "None"; break;
    case PATH_INFERNO:
     return "Inferno"; break;
    case PATH_PEARL:
     return "Pearl"; break;
    case PATH_TSUNAMI:
     return "Tsunami"; break;
    case PATH_GAIA:
     return "Gaia"; break;
    case PATH_TEMPEST:
     return "Tempest"; break;
   }
  }
  if( ch->class == CLASS_ALIEN)
  {
   switch( GET_POWERS(ch, ALIEN_CLASS))
   {
    default:
     return "Drone"; break;
    case ALIEN_RUN:
     return "Runner"; break;
    case ALIEN_WAR:
     return "Warrior"; break;
    case ALIEN_PRAE:
     return "Praetorian"; break;
    case ALIEN_SCORP:
     return "Scorpion"; break;
    case ALIEN_GOR:
     return "Gorilla"; break;
   }
  }
  return "None";
}

char* get_player_name( CHAR_DATA *ch)
{
 if( IS_NPC(ch))
  return ch->name;
 else
  return ch->pcdata->switchname;
}

char* get_title( CHAR_DATA *ch)
{
 if( !IS_NPC(ch))
  return ch->pcdata->title;
 else
  return "None";
}

char* get_clan_rank( CHAR_DATA *ch)
{
  CLAN_DATA *clan;

  if( (clan = find_clan_name(ch->clan)) == NULL)
   return "None";
  else
   return clan->rank_name[ch->clan_rank];
}

char* act_who( CHAR_DATA *ch, const char *format)
{
 char *fill_string;
 char ret_val[MAX_STRING_LENGTH];
 char word[MAX_STRING_LENGTH];
 char buf[MAX_STRING_LENGTH];

 if( format[0] == '\0'
  || format == NULL)
  return "\n\r";

 fill_string = strdup(format);
 buf[0] = '\0';
 ret_val[0] = '\0';

 while( fill_string[0] != '\0')
 {
  fill_string = one_argument(fill_string, word);
  if( word[0] != '$')
  {
   sprintf( buf, "%s ", word);
   buf[0] = UPPER(buf[0]);
   strcat(ret_val, buf);
   continue;
  }
  else
  {
   switch( word[1])
   {
   case '1':
    sprintf( buf, "%-10s ", get_player_name(ch)); break;
   case '2':
    sprintf( buf, "%-10s ", get_class_name(ch));  break;
   case '3':
    sprintf( buf, "%-8s ", ch->clan);            break;
   case '4':
    sprintf( buf, "%-12s ", get_status_name(ch)); break;
   case '5':
    sprintf( buf, "%-8s ", get_class_rank(ch));  break;
   case '6': 
    sprintf( buf, "%-12s ", get_clan_rank(ch));   break;
   case '7': 
    sprintf( buf, "%-20s ", get_title(ch));       break;
   case '8': 
    sprintf( buf, "%2d ", ch->race);           break;
   case '9': 
    sprintf( buf, "%2d ", ch->generation);     break;
   case '0':
    sprintf( buf, "%2d ", ch->pcdata->souls);  break;
   default:
    sprintf( buf, "NULL ");                    break;
   }
   strcat(ret_val, buf);
  } 
 }
 strcat(ret_val, "\n\r"); 
 ret_val[0] = UPPER(ret_val[0]);
 return str_dup(ret_val);
}

void do_new_who( CHAR_DATA *ch, char *argument)
{
 BUFFER *mortal    = buffer_new( MAX_STRING_LENGTH);
 BUFFER *titlenone = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title00   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title05   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title10   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title15   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title20   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title25   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title30   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title35   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *title40   = buffer_new( MAX_STRING_LENGTH);
 BUFFER *imm07     = buffer_new( MAX_STRING_LENGTH);
 BUFFER *imm08     = buffer_new( MAX_STRING_LENGTH); 
 BUFFER *imm09     = buffer_new( MAX_STRING_LENGTH); 
 BUFFER *imm10     = buffer_new( MAX_STRING_LENGTH); 
 BUFFER *imm11     = buffer_new( MAX_STRING_LENGTH); 
 BUFFER *imm12     = buffer_new( MAX_STRING_LENGTH);
 DESCRIPTOR_DATA *d;
 bool avafound = FALSE;
 bool immfound = FALSE;
 bool morfound = FALSE;
 char buf[MAX_STRING_LENGTH];
 int exp = 0;

 buffer_clear( mortal);
 buffer_clear( titlenone);
 buffer_clear( title00);
 buffer_clear( title05);
 buffer_clear( title10);
 buffer_clear( title15);
 buffer_clear( title20);
 buffer_clear( title25);
 buffer_clear( title30);
 buffer_clear( title35);
 buffer_clear( title40);
 buffer_clear( imm07  );
 buffer_clear( imm08  );
 buffer_clear( imm09  );
 buffer_clear( imm10  );
 buffer_clear( imm11  );
 buffer_clear( imm12  );

 if( IS_NPC(ch))
  return;
 if( ch->pcdata->who == NULL
  || ch->pcdata->who[0] == '\0'
  || strlen(ch->pcdata->who) < 1)
 {
  stc("Resetting your who to default.\n\r", ch);
  ch->pcdata->who = strdup( "");
  return;
 }
 for( d = descriptor_list; d; d = d->next)
 {
  if( d->connected != CON_PLAYING)
   continue;
  if( !can_see(ch, d->character))
   continue;
  if( IS_NPC(d->character))
   continue;
  if( d->character->level == LEVEL_IMPLEMENTOR)
  {
   bprintf( imm12, act_who( d->character, ch->pcdata->who));
   immfound = TRUE;
  }
  else if( d->character->level == LEVEL_HIGHJUDGE)
  {
   bprintf( imm11, act_who( d->character, ch->pcdata->who));
   immfound = TRUE;
  }
  else if( d->character->level == LEVEL_JUDGE)
  {
   bprintf( imm10, act_who( d->character, ch->pcdata->who));
   immfound = TRUE;
  }
  else if( d->character->level == LEVEL_ENFORCER)
  {
   bprintf( imm09, act_who( d->character, ch->pcdata->who));
   immfound = TRUE;
  }
  else if( d->character->level == LEVEL_QUESTMAKER)
  {
   bprintf( imm08, act_who( d->character, ch->pcdata->who));
   immfound = TRUE;
  }
  else if( d->character->level == LEVEL_BUILDER)
  {
   bprintf( imm07, act_who( d->character, ch->pcdata->who));
   immfound = TRUE;
  }
  else if( (d->character->level < 7) && (d->character->level > 2))
  {
    avafound = TRUE;
    if( d->character->race >= 40)
     bprintf( title40, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 40) && (d->character->race >= 35))
     bprintf( title35, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 35) && (d->character->race >= 30))
     bprintf( title30, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 30) && (d->character->race >= 25))
     bprintf( title25, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 25) && (d->character->race >= 20))
     bprintf( title20, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 20) && (d->character->race >= 15))
     bprintf( title15, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 15) && (d->character->race >= 10))
     bprintf( title10, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 10) && (d->character->race >= 5))
     bprintf( title05, act_who(d->character, ch->pcdata->who));
    else if( (d->character->race < 5) && (d->character->race >= 1))
     bprintf( title00, act_who(d->character, ch->pcdata->who));
    else
     bprintf( titlenone, act_who( d->character, ch->pcdata->who));
  }
  else
  {
   bprintf( mortal, act_who( d->character, ch->pcdata->who));
   morfound = TRUE;
  }
 }
 if( immfound)
 {
 stc("|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- IMMORTALS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|\n\r",ch);
 if( imm12->data[0] != '\0');
  stc( imm12->data, ch);
 if( imm11->data[0] != '\0');
  stc( imm11->data, ch);
 if( imm10->data[0] != '\0');
  stc( imm10->data, ch);
 if( imm09->data[0] != '\0');
  stc( imm09->data, ch);
 if( imm08->data[0] != '\0');
  stc( imm08->data, ch);
 if( imm07->data[0] != '\0');
  stc( imm07->data, ch);
 stc("\n\r", ch);
 }
 if( avafound)
 {
  stc("|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- AVATARS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|\n\r", ch);
 if( title40->data[0] != '\0');
  stc( title40->data, ch);
 if( title35->data[0] != '\0');
  stc( title35->data, ch);
 if( title30->data[0] != '\0');
  stc( title30->data, ch);
 if( title25->data[0] != '\0');
  stc( title25->data, ch);
 if( title20->data[0] != '\0');
  stc( title20->data, ch);
 if( title15->data[0] != '\0');
  stc( title15->data, ch);
 if( title10->data[0] != '\0');
  stc( title10->data, ch);
 if( title05->data[0] != '\0');
  stc( title05->data, ch);
 if( title00->data[0] != '\0');
  stc( title00->data, ch);
 if( titlenone->data[0] != '\0');
  stc( titlenone->data, ch);
 stc("\n\r", ch);
 }
 if( morfound)
 {
  stc("|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-  MORTALS  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|\n\r", 
ch);
  if( mortal->data[0] != '\0')
   stc(mortal->data, ch);
 }
 exp += (int) ((exp_who - 1) * 100);
 exp += ((global_exp_mult - 1) * 100);
 stc("|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- STATISTICS =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|\n\r", 
ch);
 sprintf( buf,
     "|           New players this reboot: %3d         Old players this reboot: %3d           |\n\r",
      new_players, old_players);
 stc( buf, ch);
 sprintf( buf, 
     "|                           Experience Bonus [%3d]                                      |\n\r", 
exp);
 stc( buf, ch);
 if( deathmatch)
  stc("|                            Deathmatch Mode: On                                        |\n\r", 
ch);
 else
  stc("|                            Deathmatch Mode: Off                                       |\n\r", 
ch);
 stc("|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|\n\r", 
ch);
 buffer_free( imm12);
 buffer_free( imm11);
 buffer_free( imm10);
 buffer_free( imm09);
 buffer_free( imm08);
 buffer_free( imm07);
 buffer_free( title40);
 buffer_free( title35);
 buffer_free( title30);
 buffer_free( title25);
 buffer_free( title20);
 buffer_free( title15);
 buffer_free( title10);
 buffer_free( title05);
 buffer_free( title00);
 buffer_free( titlenone);
 buffer_free( mortal);
 return;
}

void do_setwho(CHAR_DATA *ch, char *argument)
{
 char buf[MAX_STRING_LENGTH];

 if( IS_NPC(ch))
  return;
 one_argument( argument, buf);
 if( !str_cmp(buf, "off"))
 {
  ch->pcdata->who = strdup( "" );
  return;
 }
 stc("Your new who display will look like this:\n\r", ch);
 stc( act_who(ch, argument), ch);
 smash_tilde(argument);
 free_string( ch->pcdata->who);
 ch->pcdata->who = str_dup( argument);
 return;
}
