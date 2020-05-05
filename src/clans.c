#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"

CLAN_DATA *find_clan_name(char *argument);
int get_clan_nw(CLAN_DATA *clan);
bool is_clan_leader(CHAR_DATA *ch);
bool is_clan_coleader(CHAR_DATA *ch);

void load_clans()
{
 FILE *fp;
 char *word;
 CLAN_DATA *clan;

 int idnum = 1;
 int i;

 if( (fp = fopen(CLAN_FILE, "r")) == NULL)
 {
  bug("Could not open clan file for read.", 0);
  return;
 }
 word = fread_string(fp);
 while( str_cmp(word,"#END"))
 {
  if( !str_cmp(word, "#CLAN")) 
  {
   CREATE(clan, CLAN_DATA, 1);
   CREATE(clan->fort, FORTRESS_DATA, 1);
   clan->name = fread_string(fp);
   clan->leader = fread_string(fp);
   clan->coleader = fread_string(fp);
   clan->kills = fread_number(fp);
   clan->deaths = fread_number(fp);
   clan->points = fread_number(fp);
   clan->id_num = idnum;
   idnum++;
   clan->channel = fread_string(fp);
   for( i = 0; i < 11; i++)
    clan->rank_name[i] = fread_string(fp);
   clan->fort->level = fread_number(fp);
   clan->fort->max_turrets = fread_number(fp);
   clan->fort->max_tanks = fread_number(fp);
   clan->fort->tanks = fread_number(fp);
   clan->fort->turrets = fread_number(fp);
   clan->fort->spies = fread_number(fp);
   for( i = 0; i < 4; i++)
    clan->fort->wall_str[i] = fread_number(fp);
   for( i = 0; i < 4; i++)
    clan->fort->wall_cond[i] = fread_number(fp);
   clan->fort->upper_vnum = fread_number(fp);
   clan->fort->lower_vnum = fread_number(fp);
   clan->fort->recall_vnum = fread_number(fp);
   LINK(clan, clan_list, last_clan, next, prev);
   word = fread_string(fp);
  }
 }
 fclose(fp);
 return;
}

void save_clans()
{
 FILE *fp;
 CLAN_DATA *clan;
 int i;

 if( (fp = fopen(CLAN_FILE, "w")) == NULL)
 {
  bug("Could not open clanfile for writing.\n\r", 0);
  return;
 }
 for(clan = clan_list; clan != NULL; clan = clan->next)
 {
  fprintf(fp, "#CLAN~\n");
  fprintf(fp, "%s~\n", clan->name);
  fprintf(fp, "%s~\n", clan->leader);
  fprintf(fp, "%s~\n", clan->coleader);
  fprintf(fp, "%d ", clan->kills);
  fprintf(fp, "%d ", clan->deaths);
  fprintf(fp, "%d\n", clan->points);
  fprintf(fp, "%s~\n", clan->channel);
  for( i = 0; i < 11; i++)
   fprintf( fp, "%s~\n", clan->rank_name[i]);
  fprintf(fp, "%d ", clan->fort->level);
  fprintf(fp, "%d ", clan->fort->max_turrets);
  fprintf(fp, "%d ", clan->fort->max_tanks);
  fprintf(fp, "%d ", clan->fort->tanks);
  fprintf(fp, "%d ", clan->fort->turrets);
  fprintf(fp, "%d\n", clan->fort->spies);
  for( i = 0; i < 4; i++)
   fprintf( fp, "%d ", clan->fort->wall_str[i]);
  fprintf( fp, "\n");
  for( i = 0; i< 4; i++)
   fprintf(fp, "%d ", clan->fort->wall_cond[i]);
  fprintf(fp, "\n");
  fprintf(fp, "%d %d %d\n", clan->fort->upper_vnum, clan->fort->lower_vnum, clan->fort->recall_vnum);
 }
 fprintf(fp, "#END~");
 fclose(fp);
 return;
}

bool is_clan_leader(CHAR_DATA *ch)
{
 CLAN_DATA *clan;
 bool ret_val = FALSE;

 for( clan = clan_list; clan != NULL; clan = clan->next)
 {
  if( !str_cmp(ch->name, clan->leader))
   ret_val = TRUE;
 }
 return ret_val;
}

bool is_clan_coleader(CHAR_DATA *ch)
{
 CLAN_DATA *clan;
 bool ret_val = FALSE;

 for( clan = clan_list; clan != NULL; clan = clan->next)
 {
  if( !str_cmp( ch->name, clan->coleader))
   ret_val = TRUE;
 }
 return ret_val;
}

CLAN_DATA *find_clan_name(char *argument)
{
 CLAN_DATA *clan;
 CLAN_DATA *ret_clan = NULL;
 for(clan = clan_list; clan != NULL; clan = clan->next)
 {
  if( !str_cmp( clan->name, argument))
   ret_clan = clan;
 }
 return ret_clan;
}

void register_kill( CHAR_DATA *ch, CHAR_DATA *victim)
{
 CLAN_DATA *kill_clan;
 CLAN_DATA *death_clan;
 int points;

 kill_clan = find_clan_name(ch->clan);
 death_clan = find_clan_name(victim->clan);

 if( victim->race)
  points = victim->race;
 if( victim->race == 0)
  points = 1;
 if( kill_clan != NULL)
 {
  kill_clan->kills++;
  if( is_clan_leader(victim))
   points = (victim->race * 2);
  if( is_clan_coleader(victim))
   points = (victim->race * 1.5);
  kill_clan->points += points;
 }
 if( death_clan != NULL)
  death_clan->deaths++;
 save_clans();
 return;
}

void do_setleader(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *clan;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];

 argument = one_argument( argument, arg);

 if( IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Set WHO as a clan leader.\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("Set WHO as a clan leader.\n\r", ch);
  return;
 } 
 if( (clan = find_clan_name(victim->clan)) == NULL)
 {
  stc("That clan does not exist.\n\r", ch);
  return;
 }
 clan->leader = str_dup(victim->pcdata->switchname);
 stc("Done.\n\r", ch);
 stc("You are now a clan leader.\n\r", victim);
 save_clans();
 return;
}

void do_setcoleader(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *clan;
 CHAR_DATA *victim;
 char arg[MAX_INPUT_LENGTH];
  
 argument = one_argument( argument, arg);
  
 if( IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Set WHO as a clan co-leader.\n\r", ch);
  return;
 }
 if( (victim = get_char_world(ch, arg)) == NULL)
 {
  stc("Set WHO as a clan co-leader.\n\r", ch);
  return;
 }
 if( IS_NPC(victim))
  return;
 if( (clan = find_clan_name(victim->clan)) == NULL)
 {
  stc("That clan does not exist.\n\r", ch);
  return;
 }
 clan->coleader = str_dup(victim->pcdata->switchname);
 stc("Done.\n\r", ch);
 stc("You are now a clan co-leader.\n\r", victim);
 save_clans();
 return;
}

void do_clantable(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *clan;
 char buf[MAX_STRING_LENGTH];

 if( IS_NPC(ch))
  return;
 stc("|    Name    |   Leader   |  Coleader  | Kills | Deaths | Points |\n\r", ch);
 stc("------------------------------------------------------------------\n\r", ch);
 for( clan = clan_list; clan != NULL; clan = clan->next)
 {
  sprintf(buf, "|%12s %12s %12s %7d %8d %8d|\n\r", clan->name, clan->leader, clan->coleader,
          clan->kills, clan->deaths, clan->points);
  stc(buf, ch);
 }
 return;
}

void do_makeclan(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *clan;
 CLAN_DATA *master;
 bool found = FALSE;
 int i;

 char arg[MAX_INPUT_LENGTH];
 one_argument(argument, arg);

 if(IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Make what clan.\n\r", ch);
  return;
 }
 for( master = clan_list; master != NULL; master = master->next)
 {
  if( !str_cmp(master->name,argument))
   found = TRUE;
 }
 if( found)
 {
  stc("That clan already exists.\n\r", ch);
  return;
 }
 CREATE(clan, CLAN_DATA, 1);
 CREATE(clan->fort, FORTRESS_DATA, 1);
 clan->name = str_dup(capitalize(arg));
 clan->leader = NULL;
 clan->coleader = NULL;
 clan->fort->name = NULL;
 clan->fort->max_turrets = 0;
 clan->fort->max_tanks = 0;
 clan->fort->turrets = 0;
 clan->fort->spies = 0;
 clan->fort->tanks = 0;
 clan->fort->level = 1;
 clan->fort->wall_str[0] = 0;
 clan->fort->wall_str[1] = 0;
 clan->fort->wall_str[2] = 0;
 clan->fort->wall_str[3] = 0;
 clan->fort->wall_cond[0] = 0;
 clan->fort->wall_cond[1] = 0;
 clan->fort->wall_cond[2] = 0;
 clan->fort->wall_cond[3] = 0;
 clan->kills = 0;
 clan->points = 0;
 clan->deaths = 0;
 clan->channel = NULL;
 for( i = 0; i < 11; i++)
  clan->rank_name[i] = NULL;
 LINK(clan, clan_list, last_clan, next, prev);
 save_clans();
 return;
}

void do_delclan(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *master = NULL;
 CLAN_DATA *deleted = NULL;
 /* yes this is hackish looking...get over it*/

 char arg[MAX_INPUT_LENGTH];

 one_argument( argument, arg);

 if(IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Delete which clan?\n\r", ch);
  return;
 }
 for( master = clan_list; master != NULL; master = master->next)
 {
  if( !str_cmp(argument, master->name))
   deleted = master;
 }
 if( deleted == NULL)
 {
  stc("That is not a valid clan.\n\r", ch);
  return;
 }
 sprintf(arg, "Clan %s deleted by %s", deleted->name,
    ch->pcdata->switchname);
 log_string(arg);
 UNLINK(deleted, clan_list, last_clan, next, prev);
 DISPOSE(deleted);
 save_clans();
 return;
}

void show_clan(CHAR_DATA *ch, CLAN_DATA *clan)
{
 char buf[MAX_STRING_LENGTH];
 FORTRESS_DATA *fort;

 fort = clan->fort;
 stc("--------------------------------------------------------------\n\r",ch);
 sprintf(buf,"Clan: %10s  Points: %8d   Leader: %15s\n\r",
       clan->name, clan->points, clan->leader);
 stc(buf, ch);
 sprintf(buf, "Kills: %5d  Deaths: %5d           Coleader: %15s\n\r",
        clan->kills, clan->deaths, clan->coleader);
 stc(buf, ch);
 sprintf( buf, "Channel: %15s\n\r", clan->channel);
 stc(buf, ch);
 sprintf( buf, "Wall strength: %d/%d %d/%d %d/%d %d/%d\n\r",
       fort->wall_cond[0], fort->wall_str[0], fort->wall_cond[1],
       fort->wall_str[1], fort->wall_cond[2], fort->wall_str[2],
       fort->wall_cond[3], fort->wall_str[3]);
 stc(buf, ch);
 sprintf( buf, "Tanks %d    Turrets %d      Spies %d\n\r",
       fort->tanks, fort->turrets, fort->spies);
 stc(buf, ch);
return;
}

void do_show_clan(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *clan;
 char arg[MAX_INPUT_LENGTH];
 
 one_argument(argument, arg);
 if( IS_NPC(ch))
  return;
 if( ch->clan == NULL)
 {
  stc("You don't have a clan to show.\n\r", ch);
  return;
 }
 clan = find_clan_name(ch->clan);
 if( clan == NULL)
 {
  stc("Your clan doesn't seem to exist.\n\r", ch);
  return;
 }
 show_clan(ch, clan);
 return;
}

void do_spyclan(CHAR_DATA *ch, char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 CLAN_DATA *clan;
 CLAN_DATA *spy;
 int chance = 50;

 one_argument(argument, arg);
 
 if( IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Spy on which clan.\n\r", ch);
  return;
 }
 if( ch->clan == NULL)
 {
  stc("You must belong to a clan to spy on a clan.\n\r", ch);
  return;
 }
 spy = find_clan_name(ch->clan);
 if( spy == NULL)
 {
  stc("Your clan doesnt seem to exist.\n\r", ch);
  return;
 }
 if(spy->fort->spies < 1)
 {
  stc("Your clan doesnt have any spies to send.\n\r", ch);
  return;
 }
 if( ch->clan_rank < 5)
 {
  stc("You must reach at least rank 5 in  your clan to send spies.\n\r", ch);
  return;
 }
 if( (clan = find_clan_name(argument)) == NULL)
 {
  stc("That clan doesnt seem to exist.\n\r", ch);
  return;
 }
 if( spy->fort->spies > clan->fort->spies)
  chance += 5;
 else
  chance -= 5;
 chance += number_range(-5, 5);
 if( number_range(1,100) > chance)
 {
  if( number_range(1,100) > 75)
  {
   stc("Your spy was killed by the enemy!\n\r", ch);
   sprintf( arg, "#1%s#n has killed a #1%s#n spy!", clan->name, spy->name);
   do_info(ch, arg);
   spy->fort->spies -= 1;
   return;
  }
  else
  {
   stc("Your spy failed to learn anything about the enemy.\n\r", ch);
   return;
  }
 }
 else
 {
  stc("Your spy learned the following about the enemy clan:\n\r", ch);
  show_clan(ch, clan);
  return;
 }
 return;
}

void clan_attack(CHAR_DATA *ch,CLAN_DATA *attacker, CLAN_DATA *defender)
{
 int att_str;
 int def_str;
 int att_loss;
 int def_loss;
 bool win = FALSE;
 char buf[MAX_STRING_LENGTH];
 int damage;
 int dam_loc = number_range(0,3);

 if( attacker == NULL || defender == NULL)
 {
  log_string("Null attacker or defender in clan_attack.");
  return;
 }
 if( attacker->fight_timer != 0)
 {
  stc("Your army is already out on conquest.\n\r", ch);
  return;
 }
 attacker->fight_timer += 20;
 att_str = attacker->fort->tanks;
 att_str *= 2;
 def_str = defender->fort->turrets;
 def_str *= 3;
 att_loss = attacker->fort->tanks;
 att_loss *= (number_range(10,25)/100);
 def_loss = defender->fort->turrets;
 def_loss *= (number_range(8,20)/100);
 if( att_str > def_str)
  win = TRUE;
 if( win && number_range(1,100) < 25)
  win = FALSE;
 if( win)
 {
  sprintf(buf, "%s has won a battle against %s, they destroyed %d turrets, but lost %d tanks.",
   attacker->name, defender->name, att_loss, def_loss);
  do_info(ch, buf);
  defender->fort->wall_cond[dam_loc] -= damage;
 }
 else
 {
  sprintf(buf, "%s attempted to attack %s's clan fortress but was repelled.", attacker->name,
   defender->name);
  do_info(ch, buf);
  sprintf(buf, "%s lost %d tanks and %s lost %d turrets.", attacker->name, att_loss, 
      defender->name, def_loss);
  do_info(ch, buf);
 }
 attacker->fort->tanks -= att_loss;
 defender->fort->turrets -= def_loss;
 save_clans();
 return;
}

void do_clan_attack(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *attacker;
 CLAN_DATA *defender;
 int att_nw = 0;
 int def_nw = 0;
 char arg[MAX_INPUT_LENGTH];
 one_argument(argument, arg);

 if( IS_NPC(ch))
  return;
 if( arg[0] == '\0')
 {
  stc("Attack which clan?\n\r", ch);
  return;
 }
 if( (attacker = find_clan_name(ch->name)) == NULL)
 {
  stc("Your clan doesnt seem to exist.\n\r", ch);
  return;
 }
 if( (defender = find_clan_name(argument)) == NULL)
 {
  stc("That clan doesnt seem to exist.\n\r", ch);
  return;
 }
 if( ch->clan_rank < 3)
 {
  stc("You must have at least rank 3 in your clan to initiate attacks.\n\r", ch);
  return;
 }
 att_nw = get_clan_nw(attacker);
 def_nw = get_clan_nw(defender);
 if( def_nw < (att_nw * 0.7))
 {
  stc("They are much to small for your clan to attack.\n\r", ch);
  return;
 }
 if( def_nw > (att_nw * 1.3))
 {
  stc("Are you crazy!? They would stomp you.\n\r", ch);
  return;
 }
 clan_attack(ch, attacker, defender);
 return;
}

int get_clan_nw(CLAN_DATA *clan)
{
  int value = 0;
  int i;

  value += (clan->fort->turrets * 3);
  value += (clan->fort->tanks * 2);
  for( i = 0; i < 4; i++);
   value += (clan->fort->wall_str[i] * 5);
  value += (clan->fort->level * 3);
  
  return value;
}

void do_setclan(CHAR_DATA *ch, char *argument)
{
 CLAN_DATA *clan;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];
 int value, cost;

 argument = one_argument( argument, arg1);
 argument = one_argument( argument, arg2);
 argument = one_argument( argument, arg3);

 if( IS_NPC(ch))
  return;
 if( ch->clan == NULL)
 {
  stc("You aren't in a clan.\n\r", ch);
  return;
 }
 if( (clan = find_clan_name(ch->clan)) == NULL)
 {
  stc("Your clan doesnt seem to exist.\n\r", ch);
  return;
 }
 if( !is_clan_leader(ch))
 {
  stc("Only clan leaders can modify their clan.\n\r", ch);
  return;
 }
 if( arg1[0] == '\0')
 {
  stc("Set what?\n\r", ch); 
  stc("Syntax: setclan rank <rank number> <rank name>\n\r", ch);
  stc("        setclan buy turrets <amount>.\n\r", ch);
  stc("        setclan buy tanks <amount>.\n\r", ch);
  stc("        setclan buy walls.\n\r", ch);
  stc("        setclan buy upgrade.\n\r", ch);
  return;
 }  

 if( arg2[0] == '\0')
 {
  stc("Set it to what?\n\r", ch);
  return;
 }
 if( !str_cmp( arg1, "rank"))
 {
  if( !is_number(arg2))
  {
   stc("Must supply a value from 1 to 10.\n\r", ch);
   return;
  }
  value = atoi(arg2);
  value -= 1; /* to covert from natural numbers to computer numbers*/
  if( (value < 0) || (value > 9))
  {
   stc("Value must be between 1 and 10.\n\r", ch);
   return;
  }
  if( arg3[0] == '\0')
  {
   stc("Set it to what?\n\r", ch);
   return;
  }
  if( str_len(arg3) > 15)
  {
   stc("Rank names can only be 15 characters long.\n\r", ch);
   return;
  }
  STRFREE(clan->rank_name[value]);
  smash_color(arg3);
  clan->rank_name[value] = str_dup(arg3);
  save_clans();
  stc("Done.\n\r", ch);
  return;
 }
 if( !str_cmp(arg1, "buy"))
 {
  if( !str_cmp(arg2, "tanks"))
  {
   cost = 20;
   if( arg3[0] == '\0')
    value = 1;
   if( is_number(arg3))
    value = atoi(arg3);
   cost = cost * value;
   if( clan->points < cost)
   {
    stc("Your clan can't afford that many tanks.\n\r", ch);
    return;
   }
   clan->points -= cost;
   clan->fort->tanks += value;
   return;
  }
  else if( !str_cmp( arg2, "turrets"))
  {
   cost = 15;
   if( arg3[0] == '\0')
    value = 1;
   if( is_number(arg3))
    value = atoi(arg3);
   cost = cost * value;
   if( clan->points < cost)
   {
    stc("Your clan can't afford that many turrets.\n\r", ch);
    return;
   }
   clan->points -= cost;
   clan->fort->turrets += value;
   save_clans();
   return;
  }
  else if( !str_cmp(arg2, "walls"))
  {
   cost = (clan->fort->wall_str[0] * 20);
   if( clan->fort->wall_str[0] > ((clan->fort->level) * 50))
   {
    stc("Your walls are already as strong as they can get for your current fortress type.\n\r", ch);
    return;
   }
   if( clan->points < cost)
   {
    stc("Your clan doesnt have the prestige to build stronger walls.\n\r", ch);
    return;
   }
   for( value = 0; value < 4; value++)
    clan->fort->wall_str[value] += 10;
   clan->points -= cost;
   save_clans();
   stc("Your clan walls have been strengthened.\n\r", ch);
   return;
  }
  else if( !str_cmp( arg2, "upgrade"))
  {
   cost = ((clan->fort->level+1) * 1000);
   if( cost > clan->points)
   {
    stc("You have yet to gain the prestige to upgrade your fortress.\n\r", ch);
    return;
   }
   if( clan->fort->wall_str[0] < (50 * (clan->fort->level)))
   {
    stc("The walls of your fortress must be fully upgraded.\n\r", ch);
    return;
   }
   if( clan->fort->level == 10)
   {
    stc("Your fortress has already ascended to space.\n\r", ch);
    return;
   }
   clan->points -= cost;
   clan->fort->level++;
   save_clans();
   return;
  }
  else
  {
   stc("Syntax: setclan buy <turrets,tanks,walls,upgrade> <amount>\n\r", ch);
   return;
  }
 }
 else
 {
  stc("Syntax: setclan rank <rank name>\n\r", ch);
  stc("        setclan buy turrets <amount>.\n\r", ch);
  stc("        setclan buy tanks <amount>.\n\r", ch);
  stc("        setclan buy walls.\n\r", ch);
  stc("        setclan buy upgrade.\n\r", ch);
  return;
 }
 return;
}

void do_induct( CHAR_DATA *ch, char *argument)
{
 CHAR_DATA *victim;
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);
 argument = one_argument(argument, arg3);

 if( IS_NPC(ch))
  return;
 if( !is_clan_leader(ch) && !is_clan_coleader(ch))
 {
  stc("Yout must be a clan leader ot co-leader.\n\r", ch);
  return;
 }
 if( arg1[0] == '\0')
 {
  stc("Induct who?\n\r", ch);
  return;
 }
 if( arg2[0] == '\0')
 {
  stc("Accept, banish, or rank who?.\n\r", ch);
  return;
 }
 if( (victim = get_char_room(ch, arg1)) == NULL)
 {
  stc("They are not here.\n\r", ch);
  return;
 }
 if( !str_cmp( arg2, "accept"))
 {
  if( (victim->clan != NULL) && (str_len(victim->clan) > 4))
  {
   stc("They are already clanned.\n\r", ch);
   return; 
  }
  STRFREE(victim->clan);
  victim->clan = str_dup(ch->clan);
  stc("You have been inducted into a clan.\n\r", victim);
  stc("You have accepted a new memeber to your clan.\n\r", ch);
  victim->clan_rank = 10;
  do_save(victim, "auto");
 }
 else if( !str_cmp( arg2, "banish"))
 {
  if( str_cmp(victim->clan, ch->clan))
  {
   stc("You can't banish someone not in your clan.\n\r", ch);
   return;
  }
  if( is_clan_leader(victim))
  {
   stc("You can't banish the leader.\n\r", ch);
   return;
  }
  STRFREE(victim->clan);
  victim->clan = str_dup("");
  stc("You have been banished from your clan.\n\r", victim);
  act("$N has been banished from your clan.", ch, NULL, victim, TO_CHAR);
  return;
 }
 else if( !str_cmp(arg2,"rank") )
 {
  if( str_cmp(victim->clan,ch->clan) )
  {
   stc("You can't rank someone not in your clan.\n\r", ch);
   return;
  }
 } 
 return;
} 

void do_clan_recall( CHAR_DATA *ch, char *argument )
{
 ROOM_INDEX_DATA *location;
 CHAR_DATA *mount;
 CLAN_DATA *clan;
 FORTRESS_DATA *fort;
 
 clan = find_clan_name(ch->clan);
 if( clan != NULL )
  fort = clan->fort;       
 act("$n's body flickers with red energy.",ch,NULL,NULL,TO_ROOM);
 act("Your body flickers with red energy.",ch,NULL,NULL,TO_CHAR);

 if( clan == NULL )
 {
  stc("You don't even have a clan!\n\r",ch);
  return;
 }
 if( (location = get_room_index(fort->recall_vnum) ) == NULL )
 {
  stc("Your clan's recall hasn't been set.\n\r", ch );
  return;
 }
 if( ch->in_room == location )
  return;
 if( in_arena(ch) )
 {
  stc("You can't recall from the arena.\n\r",ch);
  return;
 }
 if( (IS_AFFECTED(ch,AFF_CURSE) || (IS_SET(ch->in_room->room_flags,ROOM_NO_RECALL) && ch->in_room->vnum != 25734)) && ch->level > 2 )
 {
  stc("You are unable to recall.\n\r",ch);
  return;
 }
 act("$n disappears in a red stream of light!",ch,NULL,NULL,TO_ROOM);
 char_from_room(ch);
 char_to_room(ch,location);
 act("$n appears in the room.",ch,NULL,NULL,TO_ROOM);
 do_look(ch,"auto");
 if( (mount = ch->mount) == NULL )
  return;
 char_from_room(mount);
 char_to_room(mount,ch->in_room);
 return;
}
