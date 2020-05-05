/****************************************************************************
 * All materials contained within are copyrights of their respective        *
 * authors.                                                                 *
 * This includes, but is not limited to:                                    *
 *     Ryan Mangrum                                                         *
 *     Matt Goff                                                            *
 ****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* include main header file */
#include "merc.h"

void save_social_table();
void load_social_table();
void init_social_table();
void resize_social_table( int amt);
int get_social( char *social );

/* I'm lazy --Kline */
#define MAX_BUFFER MAX_STRING_LENGTH

/* init_social_table will do the initial memory allocation
    to the social table at bootup time.  It will first set
    the size of the main table, then allocate memory to each
    of the entries, this function should probably be a macro,
    but..i dont wanna
*/
void init_social_table()
{
	int i;

	social_table = (SOCIAL_DATA **)malloc( top_social * sizeof( SOCIAL_DATA *));
	for( i = 0; i < top_social; i++)
		social_table[i] = (SOCIAL_DATA *) malloc(sizeof(SOCIAL_DATA));
	return;
}

/* the amt parameter is the amt you want to add *NOT*
   what you want the new max entry number to be,
   This function should simply have to increment the size
   of top_social, reallocate the memory for the FULL table,
   then allocate memory for each of the new entries
*/
void resize_social_table( int amt)
{
	int i;
	int total = amt + top_social;

	social_table = (SOCIAL_DATA **)realloc(social_table, total * sizeof(SOCIAL_DATA *));

	for( i = top_social; i < (top_social + amt); i++)
		social_table[i] = (SOCIAL_DATA *) malloc(sizeof(SOCIAL_DATA));
	top_social = top_social + amt;
	return;
}

/*void load_socials()
{
	FILE *fp;
	char *word;
        bool done = FALSE;
	int control = -1;

	log_string("Initializing social table...");
	init_social_table();
	if( (fp = fopen(SOCIAL_FILE, "r")) == NULL)
	{
		log_string("Could not open social_file for reading.");
		return;
	}
	word = fread_word(fp);
	while( !done)
	{
		bool found = FALSE;

		switch( word[0])
		{
			default:
				break;
			case 'C':
			case 'c':
				SREAD("Charnoarg", social_table[control]->char_no_arg);
				SREAD("Charfound", social_table[control]->char_found);
				SREAD("Charauto", social_table[control]->char_auto);
			case 'E':
			case 'e':
				if( str_cmp( word, "End"))
				{
					found = TRUE;
					done = TRUE;
					break;
				}
			case 'N':
			case 'n':
				WREAD("Name",social_table[control]->name);
			case 'O':
			case 'o':
				SREAD("Othersnoarg", social_table[control]->others_no_arg);
				SREAD("Othersfound", social_table[control]->others_found);
				SREAD("Othersauto", social_table[control]->others_auto);
			case 'S':
			case 's':
				if( str_cmp( word, "SOCIAL"))
				{
					control++;
					found = TRUE;
					if( control >= top_social)
						resize_social_table(1);
					break;
				}
			case 'V':
			case 'v':
				SREAD("Victfound", social_table[control]->vict_found);
		}
		if( !found)
		{
          	 log_string("Load_socials: unexpected word: %s", word);
          	 done = TRUE;
		}
		if( !done )
		 word = fread_word(fp);
	}
	fclose(fp);
	return;
}
*/

void load_socials()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int control = -1;
	char query[MAX_BUFFER];
	int count = 0;

	sprintf( query, "SELECT * FROM t_socials ORDER BY Name");
	log_string("Initializing social table...");
	init_social_table();
	res = query_db( query );
	log_string("Loading social table...");
	while( (row = mysql_fetch_row( res )) )
	{
		control++;
		if( control > 0 )
			resize_social_table(1);
		social_table[control]->name          = str_dup( row[SOC_COL_NAME]);
		social_table[control]->char_no_arg   = str_dup( row[SOC_COL_CNA] );
		social_table[control]->others_no_arg = str_dup( row[SOC_COL_ONA] );
		social_table[control]->char_found    = str_dup( row[SOC_COL_CF]  );
		social_table[control]->others_found  = str_dup( row[SOC_COL_OF]  );
		social_table[control]->vict_found    = str_dup( row[SOC_COL_VF]  );
		social_table[control]->char_auto     = str_dup( row[SOC_COL_CA]  );
		social_table[control]->others_auto   = str_dup( row[SOC_COL_OA]  );
		social_table[control]->changed       = FALSE;
		social_table[control]->times_used    = 0;
		count++;
	}
	sprintf(query,"%d socials loaded.", count);
	log_string(query);
	mysql_free_result( res );
	return;
}

/*void save_social_table()
{
	FILE *fp;
	int i;

	if( (fp = fopen( SOCIAL_FILE, "w")) == NULL)
	{
		log_string("Unable to open social_file for writing.");
		return;
	}
	for( i = 0; social_table[i]->name[0] != '\0'; i++)
	{
		fprintf(fp, "COMMAND\n");
		fprintf(fp, "Name         %s\n", social_table[i]->name);
		fprintf(fp, "Charnoarg    %s~\n", social_table[i]->char_no_arg);
		fprintf(fp, "Charfound    %s~\n", social_table[i]->char_found);
		fprintf(fp, "Charauto     %s~\n", social_table[i]->char_auto);
		fprintf(fp, "Othersnoarg  %s~\n", social_table[i]->others_no_arg);
		fprintf(fp, "Victfound    %s~\n", social_table[i]->vict_found); 
		fprintf(fp, "Othersfound  %s~\n", social_table[i]->others_found);
		fprintf(fp, "Othersauto   %s~\n", social_table[i]->others_auto);
	}
	fprintf(fp, "End\n");
	fclose(fp);
	return;
}
*/
void save_socials( CHAR_DATA *ch, char *argument)
{
	char soc[MAX_BUFFER];
	MYSQL_RES *res;
	int control;
	char *cna = NULL;
	char *ona = NULL;
	char *cf  = NULL;
	char *of  = NULL;
	char *vf  = NULL;
	char *ca  = NULL;
	char *oa  = NULL;
	int len[7];

	for( control = 0; control < top_social; control++)
	{
		if( social_table[control]->changed == FALSE )
			continue;
		if( !str_cmp( social_table[control]->name, "NULL") )
			continue;

		/* this is used to see if the social already exists. */
		sprintf( soc, "SELECT * from t_socials WHERE Name = '%s'", social_table[control]->name);
		res = query_db( soc );

		/*find the length of each string, we'll need this for memory allocation and escape string generation*/
		len[0] = strlen(social_table[control]->char_no_arg);
		len[1] = strlen(social_table[control]->others_no_arg);
		len[2] = strlen(social_table[control]->char_found);
		len[3] = strlen(social_table[control]->others_found);
		len[4] = strlen(social_table[control]->vict_found);
		len[5] = strlen(social_table[control]->char_auto);
		len[6] = strlen(social_table[control]->others_auto);

		/* Allocated memory need to be 2 * the length of the source string plus one for
		   null terminator.  mysql_real_escape_string requires this...kinda wierd but ok*/
		cna = malloc( (2 * len[0]) +1);
		ona = malloc( (2 * len[1]) +1);
		cf  = malloc( (2 * len[2]) +1);
		of  = malloc( (2 * len[3]) +1);
		vf  = malloc( (2 * len[4]) +1);
		ca  = malloc( (2 * len[5]) +1);
		oa  = malloc( (2 * len[6]) +1);


		/* this is used so that any reserved characters that sql may have that are also
		   in our strings, go into the DB without any issues. */
		mysql_real_escape_string( db, cna, social_table[control]->char_no_arg,   len[0]);
		mysql_real_escape_string( db, ona, social_table[control]->others_no_arg, len[1]);
		mysql_real_escape_string( db, cf,  social_table[control]->char_found,    len[2]);
		mysql_real_escape_string( db, of,  social_table[control]->others_found,  len[3]);
		mysql_real_escape_string( db, vf,  social_table[control]->vict_found,    len[4]);
		mysql_real_escape_string( db, ca,  social_table[control]->char_auto,     len[5]);
		mysql_real_escape_string( db, oa,  social_table[control]->others_auto,   len[6]);

		/* this secion of code is to update an entry that already exists */
		if( mysql_num_rows(res))
		{
			mysql_free_result( res);
			sprintf( soc, "UPDATE t_socials SET Charnoarg = '%s', Othersnoarg = '%s', Charfound = '%s', Othersfound = '%s', Victfound = '%s', Charauto = '%s',Othersauto = '%s' WHERE Name = '%s'", 
                        cna, ona, cf, of, vf, ca, oa, social_table[control]->name);
			res = query_db( soc );
			mysql_free_result( res );
		}
		/* if it doesnt exist, we shall make it so */
		else
		{
			mysql_free_result( res );
			sprintf( soc, "INSERT INTO t_socials VALUES( '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
			social_table[control]->name, cna, cf, ca, ona, vf, of, oa);
			res = query_db( soc );
			mysql_free_result( res );
		}
		/* now we need to be sure that we free up the memory we allocated for the holding strings
		    that we made for the escape stuff, otherwise we'll have a massive unused memory block,
		    we're not microsoft, so lets not do that.*/
		
		free_string( cna );
		free_string( ona );
		free_string( cf );
		free_string( of );
		free_string( vf );
		free_string( ca );
		free_string( oa );
	}
  if( ch != NULL )
   ch_output(ch,"%d socials saved.",top_social);
  return;
}

/*void edit_social( CHAR_DATA *ch, char *argument)
{
	char *arg1;
	char *arg2;
	int social;

	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);
	
	if( arg1[0] == '\0')
	{
		text_to_mobile(ch, "Edit which social.\n\r");
		return;
	}
	if( arg2[0] == '\0')
	{
		text_to_mobile(ch, "Edit what?\n\r");
		return;
	}
	if( argument[0] == '\0')
	{
		text_to_mobile(ch, "How would you like to edit that?\n\r");
		return;
	}
	if( (social = get_social( arg1)) == -1)
	{
		ch_output(ch, "Can't find social: %s.\n\r", arg1);
		return;
	}
	if( is_prefix( arg2, "name"))
	{
		social_table[social]->name = str_dup(argument);
		return;
	}
	else if( is_prefix( arg2, "charnoarg"))
	{
		social_table[social]->char_no_arg = str_dup(argument);
		return;
	}
	else if( is_prefix( arg2, "charfound"))
	{
		social_table[social]->char_found = str_dup(argument);
		return;
	}
	else if( is_prefix( arg2, "charauto"))
	{
		social_table[social]->char_auto = str_dup(argument);
		return;
	}
	else if( is_prefix( arg2, "othersnoarg"))
	{
		social_table[social]->others_no_arg = str_dup(argument);
		return;
	}
	else if( is_prefix( arg2, "othersfound"))
	{
		social_table[social]->others_found = str_dup(argument);
		return;
	}
	else if( is_prefix( arg2, "othersauto"))
	{
		social_table[social]->others_auto = str_dup(argument);
		return;
	}
	else
	{
		text_to_mobile(ch, "Options are: name, charnoarg, charfound, charauto, othersnoarg, othersfound,\n\r");
		text_to_mobile(ch, "and othersauto.\n\r");
		return;
	}
	return;
}
*/

void edit_social( CHAR_DATA *ch, char *argument)
{
	int value;
	char arg1[MAX_BUFFER];
	char arg2[MAX_BUFFER];

	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);

	if( argument[0] == '\0')
	{
		stc("Syntax: editsocial <social> <field> <value>.\n\r",ch);
		return;
	}
	if( (value = get_social( arg1)) == -1)
	{
		ch_output( ch, "%s was not found.\n\r", arg1);
		return;
	}
	if( !str_cmp( arg2, "charnoarg"))
	{
		free_string(social_table[value]->char_no_arg);
		social_table[value]->char_no_arg = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r", social_table[value]->name);
		return;
	}
	else if( !str_cmp( arg2, "othersnoarg"))
	{
		free_string(social_table[value]->others_no_arg);
		social_table[value]->others_no_arg = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r", social_table[value]->name);
		return;
	}
	else if( !str_cmp( arg2, "charfound"))
	{
		free_string(social_table[value]->char_found);
		social_table[value]->char_found = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r", social_table[value]->name);
		return;
	}
	else if( !str_cmp( arg2, "othersfound"))
	{
		free_string(social_table[value]->others_found);
		social_table[value]->others_found = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r",social_table[value]->name);
		return;
	}
	else if( !str_cmp( arg2, "victfound"))
	{
		free_string(social_table[value]->vict_found);
		social_table[value]->vict_found = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r", social_table[value]->name);
		return;
	}
	else if( !str_cmp( arg2, "charauto"))
	{
		free_string(social_table[value]->char_auto);
		social_table[value]->char_auto = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r", social_table[value]->name);
		return;
	}
	else if( !str_cmp( arg2, "othersauto"))
	{
		free_string(social_table[value]->others_auto);
		social_table[value]->others_auto = str_dup( argument );
		social_table[value]->changed = TRUE;
		ch_output(ch, "Social %s has been updated. Please be sure to save to keep changes.\n\r",social_table[value]->name);
		return;
	}
	else
	{
		stc("Valid fields are: charnoarg, othersnoarg, charfound, victfound, othersfound, charauto, and othersauto.\n\r",ch);
		return;
	}
	return;
}

void display_social( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_BUFFER];
	int social;

	argument = one_argument(argument, arg);

	if( arg[0] == '\0')
	{
		stc("Display which social?\n\r",ch);
		return;
	}
	if( (social = get_social(arg)) == -1)
	{
		ch_output(ch, "Could not find social: %s.\n\r", arg);
		return;
	}
	ch_output( ch, "Name:        %s\n\r", social_table[social]->name);
	ch_output( ch, "Charnoarg:   %s\n\r", social_table[social]->char_no_arg);
	ch_output( ch, "Charfound:   %s\n\r", social_table[social]->char_found);
	ch_output( ch, "Charauto:    %s\n\r", social_table[social]->char_auto);
	ch_output( ch, "Othersnoarg: %s\n\r", social_table[social]->others_no_arg);
	ch_output( ch, "Victfound:   %s\n\r", social_table[social]->vict_found);
	ch_output( ch, "Othersfound: %s\n\r", social_table[social]->others_found);
	ch_output( ch, "Othersauto:  %s\n\r", social_table[social]->others_auto);
	return;
}

/*void save_socials( CHAR_DATA *ch, char *argument)
{
	save_social_table();
	text_to_mobile(ch, "Done.\n\r");
	return;
}
*/

bool check_social( CHAR_DATA *ch, char *name, char *argument)
{
	int cmd;
	CHAR_DATA *victim;

	if( (cmd = get_social( name)) == -1)
		return FALSE;
	if ( argument[0] == '\0' )
    	{
        	act(social_table[cmd]->others_no_arg, ch, NULL, victim, TO_ROOM);
        	act(social_table[cmd]->char_no_arg, ch, NULL, victim, TO_CHAR);
   	}
    	else if ( ( victim = get_char_world(ch, argument ) ) == NULL )
   	{
     		ch_output( ch, "%s isn't here.\n\r", argument);
   	}
  	else if ( victim == ch )
    	{
        	act(social_table[cmd]->others_auto, ch, NULL, victim, TO_ROOM);
        	act(social_table[cmd]->char_auto, ch, NULL, victim, TO_CHAR);
    	}
    	else
    	{
        	act(social_table[cmd]->others_found, ch, NULL, victim, TO_NOTVICT);
        	act(social_table[cmd]->char_found, ch, NULL, victim, TO_CHAR);
        	act(social_table[cmd]->vict_found, ch, NULL, victim, TO_VICT);
	}
	return TRUE;
}


/*void create_social( CHAR_DATA *ch, char *argument)
{
	char arg[MAX_BUFFER];

	argument = one_argument( argument, arg);

	if( arg[0] == '\0')
	{
		text_to_mobile(ch, "Create what social?\n\r");
		return;
	}
	if( get_social( arg) > -1)
	{
		text_to_mobile(ch, "That social already exists.\n\r");
		return;
	}
	resize_social_table(1);
	social_table[top_social-1]->name = str_dup(arg);
	log_string("New social created: %s.", arg);
	return;
}*/

void create_social( CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_BUFFER];
	char buf[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1);
	
	if( arg1[0] == '\0' )
	{
		stc("Create what social?\n\r",ch);
		return;
	}
	if( get_social( arg1) != -1 )
 	{
		stc("That social already exists.\n\r",ch);
		return;
	}
	/* Now that we've made our checks, lets resize the array, and set the new command*/
	resize_social_table(1);
	social_table[top_social-1]->name          = str_dup( arg1 );
	social_table[top_social-1]->char_no_arg   = str_dup( "NOT SET" );
	social_table[top_social-1]->others_no_arg = str_dup( "NOT SET" );
	social_table[top_social-1]->char_found    = str_dup( "NOT SET" );
	social_table[top_social-1]->others_found  = str_dup( "NOT SET" );
	social_table[top_social-1]->vict_found    = str_dup( "NOT SET" );
	social_table[top_social-1]->char_auto     = str_dup( "NOT SET" );
	social_table[top_social-1]->others_auto   = str_dup( "NOT SET" );
	social_table[top_social-1]->changed       = TRUE;
	ch_output( ch, "New social: %s created.\n\r", arg1);
	sprintf(buf,"New command: %s created by %s.", arg1, ch->name);
	log_string(buf);
	return;
}


void delete_social( CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_BUFFER];
	char arg2[MAX_BUFFER];
	char buf[MAX_STRING_LENGTH];
	int value;

	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);

	if( arg2[0] == '\0' || str_cmp( arg2, "verify"))
	{
		stc("Syntax: deletesocial <social name> verify.\n\r",ch);
		return;
	}
	if( (value = get_social(arg1)) == -1)
	{
		stc("That social was not found.\n\r",ch);
		return;
	}
	free_string( social_table[value]->name          );
	free_string( social_table[value]->char_no_arg   );   
	free_string( social_table[value]->others_no_arg ); 
	free_string( social_table[value]->char_found    );
	free_string( social_table[value]->others_found  ); 
	free_string( social_table[value]->vict_found    );
	free_string( social_table[value]->char_auto     );
	free_string( social_table[value]->others_auto   );

	social_table[value]->name          = str_dup( "NULL" );
	social_table[value]->char_no_arg   = str_dup( "DELETED" );
	social_table[value]->others_no_arg = str_dup( "DELETED" );
	social_table[value]->char_found    = str_dup( "DELETED" );
	social_table[value]->others_found  = str_dup( "DELETED" );
	social_table[value]->vict_found    = str_dup( "DELETED" );
	social_table[value]->char_auto     = str_dup( "DELETED" );
	social_table[value]->others_auto   = str_dup( "DELETED" );	
	social_table[value]->changed	   = TRUE;

	ch_output( ch, "Social %s deleted.\n\r", arg1  );
	sprintf(buf,"Social %s deleted by %s.", arg1, ch->name );
	log_string(buf);
	return;
}

void list_socials( CHAR_DATA *ch, char *argument)
{
	int i;
	int count = 0;
	char buf[MAX_BUFFER];
        char buf2[MAX_BUFFER];

	buf[0] = '\0';
	buf2[0] = '\0';
	for( i = 0; i < top_social; i++)
	{
		if( social_table[i] == NULL)
		{
			log_string( "social_table out of bounds.");
			return;
		}
		sprintf(buf, "%-13s", social_table[i]->name);
		count++;
		if( count >= 8)
		{
			strcat(buf, "\n\r");
			count = 0;
		}
		strcat(buf2, buf);
	}
	stc(buf2,ch);
        ch_output(ch,"\n\r\n\r%d total socials found.",top_social);
	stc("\n\r",ch);
	return;
}

int get_social( char *social )
{
	int ret_val = -1;
	int i;

	for( i = 0; i < top_social; i++)
	{
		if( !str_prefix( social, social_table[i]->name))
		{
			ret_val = i;
			break;
		}
	}
	return ret_val;
}
