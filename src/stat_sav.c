#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void save_config args(( void ));
void load_config args(( void ));

void do_configure(CHAR_DATA *ch, char *argument)
{
 char buf[MAX_STRING_LENGTH];
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char arg3[MAX_INPUT_LENGTH];
 char *wholearg;
 CHAR_DATA *vch;
 int ox, oy;

 argument = one_argument(argument,arg1);
 wholearg = argument;
 argument = one_argument(argument,arg2);
 argument = one_argument(argument,arg3);

 if( arg1[0] == '\0' )
 {
  stc("Current Configuration:\n\r",ch);
  sprintf(buf,"  Mobile Hp     *= %d / %d\n\r",
   config_info->mob_hp_mult, config_info->mob_hp_div);
   stc(buf,ch);
  sprintf(buf,"  Mobile Level  *= %d / %d\n\r",
   config_info->mob_level_mult, config_info->mob_level_div);
   stc(buf,ch);
  sprintf(buf,"  Mobile Damage *= %d / %d\n\r",
   config_info->mob_dam_mult, config_info->mob_dam_div);
   stc(buf,ch);
  sprintf(buf,"  Mobile Exp    *= %d / %d\n\r",
   config_info->research_exp_mult, config_info->research_exp_div);
   stc(buf,ch);
  sprintf(buf,"  AutoHelp      == %s\n\r",
   config_info->autohelp);
   stc(buf,ch);
  sprintf(buf,"  TitleNone     == %s\n\r",
   config_info->title_none);
   stc(buf,ch);
  sprintf(buf,"  Title00       == %s\n\r",
   config_info->title00);
   stc(buf,ch);
  sprintf(buf,"  Title05       == %s\n\r",
   config_info->title05);
   stc(buf,ch);
  sprintf(buf,"  Title10       == %s\n\r",
   config_info->title10);
   stc(buf,ch);
  sprintf(buf,"  Title15       == %s\n\r",
   config_info->title15);
   stc(buf,ch);
  sprintf(buf,"  Title20       == %s\n\r",
   config_info->title20);
   stc(buf,ch);
  sprintf(buf,"  Title25       == %s\n\r",
   config_info->title25);
   stc(buf,ch);
  sprintf(buf,"  Title30       == %s\n\r",
   config_info->title30);
   stc(buf,ch);
  sprintf(buf,"  Title35       == %s\n\r",
   config_info->title35);
   stc(buf,ch);
  sprintf(buf,"  Title40       == %s\n\r",
   config_info->title40);
   stc(buf,ch);
  sprintf(buf,"  Title45       == %s\n\r",
   config_info->title45);
   stc( buf, ch);
  sprintf(buf,"  Title50       == %s\n\r",
   config_info->title50);
   stc( buf, ch);
  sprintf(buf,"  Imm07         == %s\n\r",
   config_info->imm07);  
   stc(buf,ch);
  sprintf(buf,"  Imm08         == %s\n\r",
   config_info->imm08);  
   stc(buf,ch);
  sprintf(buf,"  Imm09         == %s\n\r",
   config_info->imm09);  
   stc(buf,ch);
  sprintf(buf,"  Imm10         == %s\n\r",
   config_info->imm10);  
   stc(buf,ch);
  sprintf(buf,"  Imm11         == %s\n\r",
   config_info->imm11);  
   stc(buf,ch);
  sprintf(buf,"  Imm12         == %s\n\r",
   config_info->imm12);  
   stc(buf,ch);
  return;
 }

 if( !str_prefix(arg1,"autohelp") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure autohelp <help page>",ch);
   return;
  }
  free_string(config_info->autohelp);
  config_info->autohelp = str_dup(wholearg);
  stc("Successfully updated Auto Help.\n\r",ch);
  save_config();
  return;
 }	

 if( !str_cmp(arg1,"titlenone") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure titlenone <string>",ch);
   return;
  }
  free_string(config_info->title_none);
  config_info->title_none = str_dup(wholearg);
  stc("Successfully updated titlenone.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title00") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title00 <string>",ch);
   return;
  }
  free_string(config_info->title00);
  config_info->title00 = str_dup(wholearg);
  stc("Successfully updated title00.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title05") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title05 <string>",ch);
   return;
  }
  free_string(config_info->title05);
  config_info->title05 = str_dup(wholearg);
  stc("Successfully updated title05.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title10") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title10 <string>",ch);
   return;
  }
  free_string(config_info->title10);
  config_info->title10 = str_dup(wholearg);
  stc("Successfully updated title10.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title15") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title15 <string>",ch);
   return;
  }
  free_string(config_info->title15);
  config_info->title15 = str_dup(wholearg);
  stc("Successfully updated title15.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title20") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title20 <string>",ch);
   return;
  }
  free_string(config_info->title20);
  config_info->title20 = str_dup(wholearg);
  stc("Successfully updated title20.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title25") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title25 <string>",ch);
   return;
  }
  free_string(config_info->title25);
  config_info->title25 = str_dup(wholearg);
  stc("Successfully updated title25.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title30") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title30 <string>",ch);
   return;
  }
  free_string(config_info->title30);
  config_info->title30 = str_dup(wholearg);
  stc("Successfully updated title30.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title35") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title35 <string>",ch);
   return;
  }
  free_string(config_info->title35);
  config_info->title35 = str_dup(wholearg);
  stc("Successfully updated title35.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title40") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title40 <string>",ch);
   return;
  }
  free_string(config_info->title40);
  config_info->title40 = str_dup(wholearg);
  stc("Successfully updated title40.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title45") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title45 <string>",ch);
   return;
  }
  free_string(config_info->title45);
  config_info->title45 = str_dup(wholearg);
  stc("Successfully updated title45.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"title50") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure title50 <string>",ch);
   return;
  }
  free_string(config_info->title50);
  config_info->title50 = str_dup(wholearg);
  stc("Successfully updated title50.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"imm07") )
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure imm07 <string>",ch);
   return;
  }
  free_string(config_info->imm07);
  config_info->imm07 = str_dup(wholearg);
  stc("Successfully updated imm07.\n\r",ch);
  save_config();
  return;
 }

 if( !str_cmp(arg1,"imm08") )  
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure imm08 <string>",ch);  
   return;
  }
  free_string(config_info->imm08);  
  config_info->imm08 = str_dup(wholearg);  
  stc("Successfully updated imm08.\n\r",ch);  
  save_config();
  return;
 }

 if( !str_cmp(arg1,"imm09") )  
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure imm09 <string>",ch);  
   return;
  }
  free_string(config_info->imm09);  
  config_info->imm09 = str_dup(wholearg);  
  stc("Successfully updated imm09.\n\r",ch);  
  save_config();
  return;
 }

 if( !str_cmp(arg1,"imm10") )  
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure imm10 <string>",ch);  
   return;
  }
  free_string(config_info->imm10);  
  config_info->imm10 = str_dup(wholearg);  
  stc("Successfully updated imm10.\n\r",ch);  
  save_config();
  return;
 }

 if( !str_cmp(arg1,"imm11") )  
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure imm11 <string>",ch);  
   return;
  }
  free_string(config_info->imm11);  
  config_info->imm11 = str_dup(wholearg);  
  stc("Successfully updated imm11.\n\r",ch);  
  save_config();
  return;
 }

 if( !str_cmp(arg1,"imm12") )  
 {
  if( wholearg[0] == '\0' )
  {
   stc("Syntax: configure imm12 <string>",ch);  
   return;
  }
  free_string(config_info->imm12);  
  config_info->imm12 = str_dup(wholearg);  
  stc("Successfully updated imm12.\n\r",ch);  
  save_config();
  return;
 }

 if( !is_number(arg2) || !is_number(arg3) )
 {
  stc("Syntax:  configure <variable> <x> <y>\n\r",ch);
  stc("where x:y is the ratio you wish to set the variable to.\n\r",ch);
  stc("eg. configure hitpoints 2 1\n\r",ch);
  stc("Doubles the number of hp every mob has.\n\r",ch);
  stc("Configure on it's own lists all variables.\n\r",ch);
  return;
 }

 if( atoi(arg3) == 0 || atoi(arg2)==0 )
 {
  stc("Values of zero are not allowed.\n\r",ch);
  return;
 }

 if( !str_prefix(arg1,"hitpoints") )
 {
  ox = config_info->mob_hp_mult;
  oy = config_info->mob_hp_div;

  config_info->mob_hp_mult = atoi(arg2);
  config_info->mob_hp_div  = atoi(arg3);

  for( vch = char_list; vch != NULL; vch = vch->next )
  {
   if( IS_NPC(vch) )
   {
    vch->hit = vch->hit * oy / ox;
    vch->hit = vch->hit * config_info->mob_hp_mult / config_info->mob_hp_div;
    vch->max_hit = vch->max_hit * oy / ox;
    vch->max_hit = vch->max_hit * config_info->mob_hp_mult / config_info->mob_hp_div;
   }
  }
  stc("Successful update of mobile hitpoints.\n\r",ch);
  save_config();
  return;
 }

 if( !str_prefix(arg1,"damage") )
 {
  ox = config_info->mob_dam_mult;
  oy = config_info->mob_dam_div;

  config_info->mob_dam_mult = atoi(arg2);
  config_info->mob_dam_div  = atoi(arg3);

  for( vch = char_list; vch != NULL; vch = vch->next )
  {
   if( IS_NPC(vch) )
   {
    vch->damroll = vch->damroll * oy / ox;
    vch->damroll = vch->damroll * config_info->mob_hp_mult / config_info->mob_hp_div; 
   }
  }
  stc("Successful update of mobile damage.\n\r",ch);
  save_config();
  return;
 }

 if( !str_prefix(arg1,"level") )
 {
  ox = config_info->mob_level_mult;
  oy = config_info->mob_level_div;

  config_info->mob_level_mult = atoi(arg2);
  config_info->mob_level_div  = atoi(arg3);

  for( vch = char_list; vch != NULL; vch = vch->next )
  {
   if( IS_NPC(vch) )
   {
    int lvl;
    lvl = vch->level;
    vch->level = vch->level * oy / ox;
    vch->level = vch->level * config_info->mob_level_mult / config_info->mob_level_div;
    if( vch->level < lvl )
    vch->level = lvl;
   }
  }
  stc("Successful update of mobile level.\n\r",ch);
  save_config();
  return;
 }

 if( !str_prefix(arg1,"exp") )
 {
  config_info->research_exp_mult = atoi(arg2);
  config_info->research_exp_div  = atoi(arg3);

  stc("Successful update of exp.\n\r",ch);
  save_config();
  return;
 }

 if( !str_prefix(arg1,"points") )
 {
  config_info->prac_exp_mult = atoi(arg2);
  config_info->prac_exp_div  = atoi(arg3);

  stc("Successful update of Disc Points.\n\r",ch);
  save_config();
  return;
 }

 stc("Cannot update a non-existant variable.\n\r",ch);
 stc("config with no arguments to list.\n\r",ch);
 return;
}

void initialise_config()
{
 if (config_info == NULL)
  config_info = alloc_perm( sizeof(*config_info) );

 config_info->ap_exp_points     = 1000000;
 config_info->ap_exp_mult       = 1.00;
 config_info->mob_hp_mult	= 1;
 config_info->mob_hp_div	= 1;
 config_info->mob_level_mult	= 1;
 config_info->mob_level_div	= 1;
 config_info->mob_dam_mult	= 1;
 config_info->mob_dam_div	= 1;
 config_info->prac_exp_mult	= 1;
 config_info->prac_exp_div	= 1;
 config_info->train_exp_mult	= 1;
 config_info->train_exp_div	= 1;
 config_info->research_exp_mult	= 1;
 config_info->research_exp_div	= 1;
 config_info->research_time_mult	= 1;
 config_info->research_time_div	= 1;
 config_info->max_players		= 0;
 config_info->autohelp = str_dup("help");
 config_info->title_none = str_dup("titlenone");
 config_info->title00 = str_dup("title00");
 config_info->title05 = str_dup("title05");
 config_info->title10 = str_dup("title10");
 config_info->title15 = str_dup("title15");
 config_info->title20 = str_dup("title20");
 config_info->title25 = str_dup("title25");
 config_info->title30 = str_dup("title30");
 config_info->title35 = str_dup("title35");
 config_info->title40 = str_dup("title40");
 config_info->title45 = str_dup("title45");
 config_info->title45 = str_dup("title50");
 config_info->imm07 = str_dup("imm07");
 config_info->imm08 = str_dup("imm08");
 config_info->imm09 = str_dup("imm09");
 config_info->imm10 = str_dup("imm10");
 config_info->imm11 = str_dup("imm11");
 config_info->imm12 = str_dup("imm12");

 load_config();
 return;
}

void load_config( void )
{
 FILE *fp;
 char *word;
 bool done = FALSE;
 bool found = FALSE;
        
 if( (fp = fopen( CONFIG_FILE, "r")) == NULL)
 {
  log_string("No configuration file found...Creating new.");
  save_config();
  return;
 }
 log_string("Loading configuration data...");
 word = fread_word(fp); 
 while(!done)
 {
  found = FALSE;
  if( !str_cmp( word, "ApExpPoints") )
  {
   config_info->ap_exp_points = fread_number(fp);
   found = TRUE;
  }
  if( !str_cmp( word, "ApExpMult") )
  {
   config_info->ap_exp_mult = atof( fread_string(fp) );
   found = TRUE;
  }
  if( !str_cmp(word,"MobHpMult") )
  {
   config_info->mob_hp_mult = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"MobHpDiv") )
  {
   config_info->mob_hp_div = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"MobLevelMult") )
  {
   config_info->mob_level_mult = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"MobLevelDiv") )
  {
   config_info->mob_level_div = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"MobDamMult") )
  {
   config_info->mob_dam_mult = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"MobDamDiv") )
  {
   config_info->mob_dam_div = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"ExpMult") )
  {
   config_info->research_exp_mult = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"ExpDiv") )
  {
   config_info->research_exp_div = fread_number(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"AutoHelp") )
  {
   config_info->autohelp = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"TitleNone") )
  {
   config_info->title_none = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title00") )
  {
   config_info->title00 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title05") )
  {
   config_info->title05 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title10") )
  {
   config_info->title10 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title15") )
  {
   config_info->title15 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title20") )
  {
   config_info->title20 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title25") )
  {
   config_info->title25 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title30") )
  {
   config_info->title30 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title35") )
  {
   config_info->title35 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Title40") )
  {
   config_info->title40 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word, "Title45") )
  {
   config_info->title45 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word, "Title50") )
  {
   config_info->title50 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Imm07") )
  {
   config_info->imm07 = fread_string(fp);
   found = TRUE;
  }
  else if( !str_cmp(word,"Imm08") )  
  {
   config_info->imm08 = fread_string(fp);  
   found = TRUE;
  }
  else if( !str_cmp(word,"Imm09") )  
  {
   config_info->imm09 = fread_string(fp);  
   found = TRUE;
  }
  else if( !str_cmp(word,"Imm10") )  
  {
   config_info->imm10 = fread_string(fp);  
   found = TRUE;
  }
  else if( !str_cmp(word,"Imm11") )  
  {
   config_info->imm11 = fread_string(fp);  
   found = TRUE;
  }
  else if( !str_cmp(word,"Imm12") )  
  {
   config_info->imm12 = fread_string(fp);  
   found = TRUE;
  }
  else if( !str_cmp(word,"#END") )
  {
   done = TRUE;
   found = TRUE;
   break;
  }
  if(!found)
  {
   fclose(fp);
   return;
  }
  if(!done)
   word = fread_word(fp);
 }
 log_string("Configuration data loaded...");
 fclose(fp);
 return;
}  

void save_config( void )
{
 FILE *fp;

 if( (fp = fopen( CONFIG_FILE, "w")) == NULL)
 {
  log_string("Can not open CONFIG_FILE for writing.");
  return;
 }

 fprintf(fp,"MobHpMult    %d\n",config_info->mob_hp_mult);
 fprintf(fp,"MobHpDiv     %d\n",config_info->mob_hp_div);
 fprintf(fp,"MobLevelMult %d\n",config_info->mob_level_mult);
 fprintf(fp,"MobLevelDiv  %d\n",config_info->mob_level_div);
 fprintf(fp,"MobDamMult   %d\n",config_info->mob_dam_mult);
 fprintf(fp,"MobDamDiv    %d\n",config_info->mob_dam_div);
 fprintf(fp,"ExpMult      %d\n",config_info->research_exp_mult);
 fprintf(fp,"ExpDiv       %d\n",config_info->research_exp_div);
 fprintf(fp,"AutoHelp     %s~\n",config_info->autohelp);
 fprintf(fp,"TitleNone    %s~\n",config_info->title_none);
 fprintf(fp,"Title00      %s~\n",config_info->title00);
 fprintf(fp,"Title05      %s~\n",config_info->title05);
 fprintf(fp,"Title10      %s~\n",config_info->title10);
 fprintf(fp,"Title15      %s~\n",config_info->title15);
 fprintf(fp,"Title20      %s~\n",config_info->title20);
 fprintf(fp,"Title25      %s~\n",config_info->title25);
 fprintf(fp,"Title30      %s~\n",config_info->title30);
 fprintf(fp,"Title35      %s~\n",config_info->title35);
 fprintf(fp,"Title40      %s~\n",config_info->title40);
 fprintf(fp,"Title45      %s~\n",config_info->title45);
 fprintf(fp,"Title50      %s~\n",config_info->title50);
 fprintf(fp,"Imm07        %s~\n",config_info->imm07);
 fprintf(fp,"Imm08        %s~\n",config_info->imm08);
 fprintf(fp,"Imm09        %s~\n",config_info->imm09);
 fprintf(fp,"Imm10        %s~\n",config_info->imm10);
 fprintf(fp,"Imm11        %s~\n",config_info->imm11);
 fprintf(fp,"Imm12        %s~\n",config_info->imm12);
 fprintf(fp,"ApExpPoints  %d\n" ,config_info->ap_exp_points);
 fprintf(fp,"ApExpMult    %1.3f~\n",config_info->ap_exp_mult); 
 fprintf(fp,"#END\n");
 log_string("Configuration successfully saved.");
 fclose(fp);

 return;  
}
