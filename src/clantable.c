/*
 * Initial version created by Dral  21 Aug 1999,
 * based on kingdom code from Incoherent Dreams.
 */

// standard includes
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// mud includes
#include "merc.h"
#include "clantable.h"


// external vars
//extern bool fBootDb; // from db.c, true if the mud is still loading

// internal defines
#define CLANPK_CURRENT_FILEFORMAT_VERSION 3
#define CLANPK_DO_CLANTABLE_CLAN_NAME_LEGEND "clan name"
#define CLANPK_DO_CLANTABLE_CLAN_LEADER_NAME_LEGEND "leader"
#define CLANPK_DO_CLANTABLE_CLAN_COLEADER_NAME_LEGEND "  coleader  "
// internal structures
typedef struct clanpktable_data CLANPKTABLE_DATA;

struct clanpktable_data {
    int num;
    char *name;
    char *leader_name;
    char *coleader_name;
	// number of times any member of this clan has ...
    int kills; // ...killed a player who's not of this clan
    int deaths; // ...been killed by a player who's not of this clan
    int self_kills; // ...killed any other member of this clan
    int clan_points;
    int arena_kills;
    int arena_deaths;
    int arena_self_kills;
    CLANPKTABLE_DATA *next;
};

// internal vars
static CLANPKTABLE_DATA *clanpk_list;
static CLANPKTABLE_DATA *clanless_clan; // normally same as first clan in clanpk_list
static bool loading_clanpktable;
static bool pending_save_clanpktable;

// internal functions
void save_clanpktable ();
CLANPKTABLE_DATA *create_clanpktable_data(char *clan_name, char *leader_name,char *coleader_name,
 int kills, int deaths, int self_kills, int cp,
 int arena_kills, int arena_deaths, int arena_self_kills);
CLANPKTABLE_DATA *get_clan_by_clanname (char *clan_name);
CLANPKTABLE_DATA *get_clan_by_char (CHAR_DATA *ch);
//CLANPKTABLE_DATA *get_clan_by_num (int num);
void internal_register_clanpk (CHAR_DATA *killer, CHAR_DATA *victim, bool is_arenakill);
void clanpktable__set_clan(CHAR_DATA *ch, CLANPKTABLE_DATA *clan);
void clanpktable__set_leader(CHAR_DATA *ch, CLANPKTABLE_DATA *clan);
void clanpktable__set_coleader(CHAR_DATA *ch, CLANPKTABLE_DATA *clan);
void dump_clantable();
//CLANPKTABLE_DATA *raw_get_clan_by_num(int num);
CLANPKTABLE_DATA *get_clanless_clan();
CLANPKTABLE_DATA *get_first_clan();
CLANPKTABLE_DATA *get_next_clan(CLANPKTABLE_DATA *a_clan);
//char *ratio2str(int value, int max, char *buf); // appends to buf and returns buf


void load_clanpktable ()
{
	char buf[MAX_STRING_LENGTH];
	FILE *fp;
	int i = 1;
	int fileformat_version;
	CLANPKTABLE_DATA *a_clan;

	char *name;
	char *leader_name;
        char *coleader_name;
       
	int kills;
	int deaths;
        int cp;
	int self_kills;
	int arena_kills;
	int arena_deaths;
	int arena_self_kills;

	loading_clanpktable = TRUE;
	pending_save_clanpktable = FALSE;

	if ((fp = fopen (CLANTABLE_FILENAME, "r")) == NULL) {
		sprintf (buf, "Error: load_clanpktable: %.200s not found!", CLANTABLE_FILENAME);
		log_string (buf);
		exit (1);
	}

	if (feof(fp)) {
		log_string("Warning: load_clanpktable: empty clantable data file, no clans available");
		// create a reasonable default entry for clan 0
		a_clan = create_clanpktable_data("", "","", 0, 0, 0, 0, 0, 0, 0);
		if (a_clan == NULL) {
			log_string("Error: load_clanpktable: empty clantable data file...");
			log_string("...and I couldn't even create a default clan");
			fclose (fp);
			exit (1);
		}
	}
	fileformat_version = fread_number(fp);
	fread_to_eol(fp);

	if ( (fileformat_version < 1) || (fileformat_version > CLANPK_CURRENT_FILEFORMAT_VERSION) ) {
		log_string("Error: load_clanpktable: unable to read clantable file...");
		sprintf(buf, "...this version of load_clantable() can only read version 1 to %d files",
		 CLANPK_CURRENT_FILEFORMAT_VERSION);
		fclose(fp);
		exit (1);
	}

	if (fileformat_version == 1) {
		// file format version 1 didn't include clan 0 stats
		a_clan = create_clanpktable_data("", "","", 0, 0, 0, 0, 0, 0, 0);
		if (a_clan == NULL) {
			log_string("Error: load_clanpktable: reading ver 1 file: couldn't create the default clan");
			fclose (fp);
			exit (1);
		}
		save_clanpktable ();
		i = 1;
	} else {
		i = 0;
	}

	for ( ; !feof(fp); i++) {
		name = fread_string(fp); // 1: clan name
		leader_name = fread_string(fp); // 2: leader name
                coleader_name = fread_string(fp);
		kills = fread_number(fp); // 3: clan kills
		deaths = fread_number(fp); // 4: clan deaths
		self_kills = fread_number(fp); // 5: clan self kills (intra clan kills)
		if ( fileformat_version < 3 )
		{
                        cp = 0;
			arena_kills = 0;
			arena_deaths = 0;
			arena_self_kills = 0;
		}
		else
     		{
                        cp = fread_number(fp);
			arena_kills = fread_number(fp); // 6: clan kills in arena
			arena_deaths = fread_number(fp); // 7: clan deaths in arena
			arena_self_kills = fread_number(fp); // 8: clan self kills (intra clan kills) in arena
		}

		a_clan = create_clanpktable_data(
			name,            // 1: clan name
			leader_name,     // 2: leader name
                        coleader_name,
			kills,           // 3: clan kills
			deaths,          // 4: clan deaths
			self_kills,      // 5: clan self kills (intra clan kills)
                        cp,
			arena_kills,     // 6: clan kills in arena
			arena_deaths,    // 7: clan deaths in arena
			arena_self_kills // 8: clan self kills (intra clan kills) in arena
		);

		if (a_clan == NULL) {
			sprintf(buf, "Error: load_clanpktable: couldn't create clan %d", i);
			log_string(buf);
			fclose (fp);
			exit (1);
		}

		fread_to_eol(fp);
	}
/*#ifdef MAGE_CLAN_NAME
	if (get_clan_by_clanname(MAGE_CLAN_NAME) == NULL)
	{
		a_clan = create_clanpktable_data(MAGE_CLAN_NAME, "", 0, 0, 0, 0, 0, 0, 0);

		if (a_clan == NULL)
		{
			sprintf(buf, "Error: load_clanpktable: mage clan \"%.30s\" didn't exist in %s",
			 MAGE_CLAN_NAME, CLANTABLE_FILENAME);
			log_string(buf);
			log_string  ("Error: load_clanpktable: and an error occured trying to create it.");
			fclose (fp);
			exit (1);
		}

		sprintf(buf, "Warning: load_clanpktable: mage clan \"%.30s\" didn't exist in %s",
		 MAGE_CLAN_NAME, CLANTABLE_FILENAME);
		log_string(buf);
		log_string  ("Warning: load_clanpktable: had to create it afresh with 0 in all stats.");
		save_clanpktable ();
	}
#endif*/
	sprintf(buf, "debug: load_clanpktable: loaded %d clans from a version %d %s",
	 i, fileformat_version, CLANTABLE_FILENAME);
	log_string(buf);

	fclose (fp);

	loading_clanpktable = FALSE;

#ifndef DEBUG_CLANPK
	if ( pending_save_clanpktable || (fileformat_version < CLANPK_CURRENT_FILEFORMAT_VERSION) )
#endif
	{
		save_clanpktable();
	}
}

// only need to call this in functions where clanpktable_data members
// are changed directly
void save_clanpktable ()
{
	char buf[MAX_STRING_LENGTH];
	FILE *fp;
	CLANPKTABLE_DATA *a_clan;

	if (loading_clanpktable)
	{
		pending_save_clanpktable = TRUE;
		return;
	}

	pending_save_clanpktable = FALSE;

	fclose( fpReserve );

	if ((fp = fopen (CLANTABLE_FILENAME, "w")) == NULL) {
		sprintf (buf, "Error: unable to open %.200s for writing!", CLANTABLE_FILENAME);
		log_string (buf);
		if ( !loading_clanpktable )
		{
			fpReserve = fopen( NULL_FILE, "r" );
		}
		return;
	}

	// file format version
	fprintf (fp, "3\n");

	for (a_clan = get_first_clan(); a_clan != NULL; a_clan=get_next_clan(a_clan) ) {
		fprintf (fp, "%s~\n", a_clan->name);
		fprintf (fp, "%s~\n", a_clan->leader_name);
                fprintf (fp, "%s~\n", a_clan->coleader_name);
		fprintf (fp, "%d\n",  a_clan->kills);
		fprintf (fp, "%d\n",  a_clan->deaths);
		fprintf (fp, "%d\n",  a_clan->self_kills);
                fprintf (fp, "%d\n",  a_clan->clan_points);
		fprintf (fp, "%d\n",  a_clan->arena_kills);
		fprintf (fp, "%d\n",  a_clan->arena_deaths);
		fprintf (fp, "%d\n",  a_clan->arena_self_kills);
	}

	fclose (fp);
	fpReserve = fopen( NULL_FILE, "r" );
}

/* This is one of the few clan pk table functions that
 * might return a NULL.  It does this - rather than return
 * clan 0 - to distinguish between not finding the named
 * clan, and finding clan 0.
 */
CLANPKTABLE_DATA *get_clan_by_clanname (char *clan_name)
{
#ifdef DEBUG_CLANPK_VERBOSE
	char buf[MAX_STRING_LENGTH];
#endif
	CLANPKTABLE_DATA *a_clan;

	if ( (clan_name == NULL) || (clan_name[0] == '\0') )
	{
		// please excuse this messy code - I'm tired
		if (clan_name == NULL) {
			bug("get_clan_by_name: NULL clan_name given", 0);
		}
		return get_clanless_clan();
	}

	for (a_clan=get_first_clan(); a_clan!=NULL; a_clan=get_next_clan(a_clan))
	{
#ifdef DEBUG_CLANPK_VERBOSE
		sprintf(buf, "get_clan_by_clanname: checking against clan %d", a_clan->num);
		log_string(buf);
#endif
		if (!str_cmp(a_clan->name, clan_name))
		{
			//clans[i].name == clan_name
			//so we found the clan we're looking for
#ifdef DEBUG_CLANPK_VERBOSE
			log_string("get_clan_by_clanname: found");
#endif
			return a_clan;
		}
	}

	// if we didn't return in the for loop, we didn't find the clan
	// so return NULL to indicate failure
#ifdef DEBUG_CLANPK_VERBOSE
	log_string("get_clan_by_clanname: not found");
#endif
	return NULL;
}

CLANPKTABLE_DATA *get_clan_by_char (CHAR_DATA *ch)
{
#ifdef CHAR_CLANS_AS_NUMBER
	int clan_num;
#else
	char *clan_name;
#endif
	char buf[MAX_STRING_LENGTH];
	CLANPKTABLE_DATA *the_clan;

	if (ch == NULL)
	{
		bug("get_clan_by_char: null char given", 0);
		// if an invalid clan is found, return "clanless"/"final" (ie, no clan)
		return get_clanless_clan();
	}


	clan_name = ch->clan;

	if (clan_name == NULL)
	{
		clan_name = ch->clan = str_dup(get_clanless_clan()->name);
	}

	the_clan = get_clan_by_clanname(clan_name);

	// if that clan doesn't exist...
	if (the_clan == NULL)
	{
      	// ...report that...
		sprintf(buf, "get_clan_by_char: %s->clan=%.30s, doesn't exist, changing to clan 0",
		 ch->name, clan_name);
		bug(buf, 0);
		// ...and use the default clan
		the_clan = get_clanless_clan();
		clanpktable__set_clan(ch, the_clan);
      }

	return the_clan;
}

/*
CLANPKTABLE_DATA *get_clan_by_num (int clan_num)
{
	int i;
	char buf[MAX_STRING_LENGTH];
	CLANPKTABLE_DATA *result;

	if ( (clan_num<0) || (clan_num>=MAX_CLANS) )
	{
		sprintf(buf, "get_clan_by_num: %d out of range [0-%d]", clan_num, MAX_CLANS-1);
		bug(buf, 0);
		// if an invalid clan is found, return "clanless"/"rogue" (ie, no clan)
		clan_num = 0;
	}

	result = raw_get_clan_by_num(clan_num);

	i = result->num;
	if ( i != clan_num )
	{
		CLANPKTABLE_DATA *other_clan = raw_get_clan_by_num(i);

		sprintf(buf, "get_clan_by_num: clan %d[%.30s] thinks it's clan %d[%.30s]",
		 clan_num, result->name, i, (other_clan==NULL?"No such clan":other_clan->name));
		bug(buf, 0);

		// if an invalid clan is found, return "clanless"/"rogue" (ie, no clan)
		result = raw_get_clan_by_num(0);
	}

	return result;

}
*/

/* This doesn't handle non-avatar related kills,
 * mainly because they should never happen.
 */
void register_clanpk (CHAR_DATA *killer, CHAR_DATA *victim)
{
	internal_register_clanpk(killer, victim, FALSE);
}

void register_arena_clanpk (CHAR_DATA *killer, CHAR_DATA *victim)
{
	internal_register_clanpk(killer, victim, TRUE);
}

void internal_register_clanpk (CHAR_DATA *killer, CHAR_DATA *victim, bool is_arenakill)
{
	char buf[MAX_STRING_LENGTH];
	CLANPKTABLE_DATA *killer_clan, *victim_clan;
	char *killer_name, *victim_name;

	// most kills are players killing a mob, so check
	// for that first
	if ( IS_NPC(victim) || IS_NPC(killer) )
	{
		// ignore kills involving mobs
		// TODO (maybe): check to see if the mob a clan guardian
		//               or charmed by anyone and, if so,
		//               calculate the real clans involved
		return;
	}

	// this function mustn't be passed any NULL args
	killer_name = (killer==NULL?"NULL":killer->name);
	victim_name = (victim==NULL?"NULL":victim->name);
	if ( (killer == NULL) || (victim == NULL) )
	{
		sprintf(buf, "register_clanpk: invalid args: killer=%s, victim=%s", killer_name, victim_name);
		bug(buf, 0);
		return;
	}
	// at this point we know this kill was a pk (player killing a player)

	// check for clan numbers
	// nb: we don't need to log invalid clans,
	//     because get_clan_by_num() already does
	//     this (and returns us clan 0 - clanless)
	killer_clan = get_clan_by_char(killer);
	victim_clan = get_clan_by_char(victim);

	if (killer_clan == victim_clan)
	{
		// intra clan kill (self_kill)
		if (killer_clan != get_clanless_clan())
		{
			// ohh! a clannie just killed one of their own!
			// TODO: announce intra clan kill
		}
		if (is_arenakill)
		{
			killer_clan->arena_self_kills++;
		}
		else
		{
			killer_clan->self_kills++;
		}
		// nb: I don't add to deaths on purpose
		//     so to get all deaths you must look
		//     at deaths+self_kills
	}
	else
	{
		if (killer_clan->num != 0)
		{
			if (victim_clan->num != 0)
			{
				//TODO: announce clankill by killer_clan of victim_clan
			}
			else
			{
				//TODO (maybe): announce non-clankill by killer_clan
			}
		}
		else if (victim_clan->num != 0)
		{
			//TODO (maybe): announce non-clandeath of victim_clan
		}
		if (is_arenakill)
		{
			killer_clan->arena_kills++;
			victim_clan->arena_deaths++;
		}
		else
		{
			killer_clan->kills++;
                        killer_clan->clan_points += victim->race;
			victim_clan->deaths++;
		}
	}

	save_clanpktable();
}

void transfer_clanpk_leadership(CHAR_DATA *old_leader, CHAR_DATA *new_leader)
{
	char buf[MAX_STRING_LENGTH];
	CLANPKTABLE_DATA *clan_a;
	CLANPKTABLE_DATA *clan_b;

	if ( (old_leader==NULL) || (new_leader==NULL) )
	{
		sprintf(buf, "transfer_clanpk_leadership: old_leader==%.30s new_leader==%.30s - both much be non-NULL", (old_leader==NULL?"NULL":old_leader->name), (new_leader==NULL?"NULL":new_leader->name));
		bug(buf, 0);
		return;
	}
	clan_a = get_clan_by_char(old_leader);
	clan_b = get_clan_by_char(new_leader);

	if (clan_a != clan_b)
	{
		// the intended leader isn't in the same clan as the old leader.
		// as leadership changing for some clans involves changes
		// to ch->generation, I can't handle changing leader unless
		// all other attribues have changed (eg, ch->clan and ch->generation)
		sprintf(buf, "transfer_clanpk_leadership: old_leader \"%.30s\" is in clan \"%.30s\" but new_leader \"%.30s\" is in clan \"%.30s\" - they must be in the same clan _before_ leadership can be transfered by this function.", old_leader->name, new_leader->name, clan_a->name, clan_b->name);
		bug(buf, 0);
		return;
	}

	if (str_cmp(old_leader->name, clan_a->leader_name))
	{
		sprintf(buf, "transfer_clanpk_leadership: \"%.30s\" isn't the leader of \"%.30s\" - can't transfer leadership to \"%.30s\".", old_leader->name, clan_a->leader_name, new_leader->name);
		bug(buf, 0);
		return;
	}

	clanpktable__set_leader(new_leader, clan_a);
}

CLANPKTABLE_DATA *create_clanpktable_data(char *clan_name, char *leader_name,char *coleader,
 int kills, int deaths, int self_kills, int cp,
 int arena_kills, int arena_deaths, int arena_self_kills)
{
	char buf[MAX_STRING_LENGTH];
	CLANPKTABLE_DATA *a_clan, *new_clan = NULL;

	if (clanpk_list == NULL) {
		// this is the first clan created
		// assume it's meant to be the "default" clan
		// ie, the clan that people belong to if they're not in a clan
#ifdef DEBUG_CLANPK_VERBOSE
		log_string("create_clanpktable_data: default clan (clan 0)");
#endif
	} else {
		// for all normal clans, have to check the name is unique
#ifdef DEBUG_CLANPK_VERBOSE
		log_string("create_clanpktable_data: checking to see if another clan is using that name");
#endif
		a_clan = get_clan_by_clanname(clan_name);
		if (a_clan != NULL) {
			sprintf(buf, "create_clanpktable_data: clan %.30s[%d] already uses that name",
			 a_clan->name, a_clan->num);
			bug(buf, 0);
			return NULL;
		}
	}

	new_clan = (CLANPKTABLE_DATA *) malloc(sizeof(CLANPKTABLE_DATA));

	if (new_clan == NULL)
	{
		sprintf(buf, "create_clanpktable_data: malloc: no memory to create clan: %.30s",
		 clan_name);
		bug(buf, 0);
	}
	else
	{
		new_clan->name = str_dup(clan_name);
		new_clan->leader_name = str_dup(leader_name);
                new_clan->coleader_name = str_dup(coleader);
		new_clan->kills = kills;
		new_clan->deaths = deaths;
		new_clan->self_kills = self_kills;
                new_clan->clan_points = cp;
		new_clan->arena_kills = arena_kills;
		new_clan->arena_deaths = arena_deaths;
		new_clan->arena_self_kills = arena_self_kills;

		// add new clans to the end of the list,
		// uses very slightly more cpu, but the
		// list makes more sense (to me) that way
		new_clan->next = NULL;

		if (clanpk_list == NULL)
		{
			// if this is the first clan
			clanless_clan = new_clan;
			clanpk_list = new_clan;
			new_clan->num = 0;
		}
		else
		{
			// find tail
			for (a_clan=clanpk_list; (a_clan->next)!=NULL; a_clan=a_clan->next)
			{
				// do nothing, just loop through until a_clan is the last clan
			}
			// a_clan should now be the last clan in the list
			a_clan->next = new_clan;
			new_clan->num = a_clan->num + 1;
		}
		

		if (!loading_clanpktable)
		{
			save_clanpktable();
		}
	}

	return new_clan;
}

void do_found_clan(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	//CLANPKTABLE_DATA *ch_clan;
	CLANPKTABLE_DATA *new_clan;

	if (IS_NPC(ch))
	{
		send_to_char("Sorry, an NPC may not found a clan.\n\r", ch);
		return;
	}

	if ( !IS_IMMORTAL(ch) )
	{
		sprintf(buf, "Sorry, you don't have the power required to found a new clan.\n\r");
		send_to_char(buf, ch);
		return;
	}
	/*ch_clan = get_clan_by_char(ch);
	if ( !str_cmp(ch->name, ch_clan->leader_name) )
	{
		send_to_char("You cannot found a new clan while you are the leader of your current clan:\n\r", ch);
		sprintf(buf, " %s\n\r",
		 get_clan_by_char(ch)->name);
		send_to_char(buf, ch);
		return;
	}*/

	if (strlen(argument) < MIN_CLAN_NAME_LENGTH)
	{
		send_to_char("Clan name too short.\n\r", ch);
		return;
	}
	if (strlen(argument) > MAX_CLAN_NAME_LENGTH)
	{
		argument[MAX_CLAN_NAME_LENGTH + 1] = '\0';
		sprintf(buf, "Clan name too long, try \"%s\" instead.\n\r", argument);
		send_to_char(buf, ch);
		return;
	}
	smash_tilde(argument);

	if ( get_clan_by_clanname(argument) != NULL )
	{
		send_to_char("A clan with that name already exists.\n\r", ch);
		return;
	}

	// at this point, we know the clan:
	//  is being created by an immortal,
	//  has the right length name, which doesn't contain any tilde,
	//  doesn't already exist
	// so lets go ahead and create the clan
	new_clan = create_clanpktable_data(str_dup(argument), str_dup(""), str_dup(""),0, 0, 0, 0, 0, 0,0);

	if (new_clan == NULL)
	{
		send_to_char("Eep! An internal error occured while trying to found your clan.\n\r", ch);
		send_to_char("(Probably caused by there being too many clans.)\n\r", ch);
		send_to_char("Please inform the admin ASAP.\n\r", ch);
		return;
	}
	//don't do this any more, this function is now for immortal use only
	//// and set ch as being a member
	//clanpktable__set_clan(ch, new_clan);

	sprintf(buf, "Created clan: %s\n\r", new_clan->name);
	send_to_char(buf, ch);
}

void clanpktable__set_clan(CHAR_DATA *ch, CLANPKTABLE_DATA *clan)
{
#ifdef CHAR_CLANS_AS_NUMBER
	ch->clan = clan->num;
#else
	free_string(ch->clan);
	ch->clan = str_dup(clan->name);
#endif
}

void clanpktable__set_leader(CHAR_DATA *ch, CLANPKTABLE_DATA *clan)
{
	free_string ( clan->leader_name );
	clan->leader_name = str_dup ( ch->name );

	save_clanpktable ();
}

void clanpktable__set_coleader(CHAR_DATA *ch, CLANPKTABLE_DATA *clan)
{
  free_string(clan->coleader_name);
  clan->coleader_name = str_dup(ch->name);
  save_clanpktable();
}

void do_clantable(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	int i;
	int clan_count = 0;
	float kill_ratio;
	// should only need to be 5 (eg, "0.79\0")
	// but this allows room for error
	char ratio_buf[10];
	int total_fights;
	CLANPKTABLE_DATA *a_clan;
	int max_clan_name_len;
	int max_clan_leader_name_len;
        int max_clan_coleader_name_len;
	char horizontal_seperator[MAX_STRING_LENGTH];
	int hs_pos; // index (POSition) withint the Horizontal_Seperator
	int name_padding;
      	int leader_name_padding;
        int coleader_padding;

	max_clan_name_len = 0;
	max_clan_leader_name_len = 0;
        max_clan_coleader_name_len = 0;

	for (a_clan=get_first_clan(); a_clan!=NULL; a_clan=get_next_clan(a_clan))
	{
		/* these shouldn't happen with get_next_clan
		if (a_clan->num == -1) {
			// unused slot
			continue;
		}
		else if (a_clan->num != i)
		{
			// invalid clan
			continue;
		}
		else
		*/
		if ( a_clan->num == 0 )
		{
			continue;
		}
		else
		{
			clan_count++;
			if ( strlen(a_clan->name) > max_clan_name_len )
			{
				max_clan_name_len = strlen(a_clan->name);
			}
			if ( strlen(a_clan->leader_name) > max_clan_leader_name_len )
			{
				max_clan_leader_name_len = strlen(a_clan->leader_name);
			}
		}
	}

	if ( (max_clan_name_len+2) < strlen(CLANPK_DO_CLANTABLE_CLAN_NAME_LEGEND) )
	{
		max_clan_name_len = strlen(CLANPK_DO_CLANTABLE_CLAN_NAME_LEGEND) - 2;
	}
	if ( (max_clan_leader_name_len+2) < strlen(CLANPK_DO_CLANTABLE_CLAN_LEADER_NAME_LEGEND) )
	{
		max_clan_leader_name_len = strlen(CLANPK_DO_CLANTABLE_CLAN_LEADER_NAME_LEGEND) - 2;
	}
        if( (max_clan_coleader_name_len+2) < strlen(CLANPK_DO_CLANTABLE_CLAN_COLEADER_NAME_LEGEND) )
        {
             max_clan_coleader_name_len = strlen(CLANPK_DO_CLANTABLE_CLAN_COLEADER_NAME_LEGEND) - 2;
        }
	hs_pos = 0;
	for (i = 0; i < (max_clan_name_len+2); i++)
	{
		horizontal_seperator[hs_pos++] = '-';
	}
	horizontal_seperator[hs_pos++] = '+';
	for (i = 0; i < (max_clan_leader_name_len+2); i++)
	{
		horizontal_seperator[hs_pos++] = '-';
	}
        horizontal_seperator[hs_pos++] = '+';
        for( i = 0; i < (max_clan_coleader_name_len+2); i++)
        {
                horizontal_seperator[hs_pos++] = '-';
        }
	horizontal_seperator[hs_pos++] = '+';
	for (i = 0; i < strlen(" kills deaths ratio clanpoints "); i++)
	{
		horizontal_seperator[hs_pos++] = '-';
	}
	horizontal_seperator[hs_pos++] = '+';
	for (i = 0; i < strlen(" awin  aloss aintr "); i++)
	{
		horizontal_seperator[hs_pos++] = '-';
	}
	horizontal_seperator[hs_pos++] = '\0';

	sprintf(buf, "/%s\\\n\r", horizontal_seperator);
	send_to_char(buf, ch);

	name_padding = (max_clan_name_len+2) - strlen(CLANPK_DO_CLANTABLE_CLAN_NAME_LEGEND);
	leader_name_padding =
	 (max_clan_leader_name_len+2) - strlen(CLANPK_DO_CLANTABLE_CLAN_LEADER_NAME_LEGEND);
        coleader_padding =
         (max_clan_coleader_name_len+2) - strlen(CLANPK_DO_CLANTABLE_CLAN_COLEADER_NAME_LEGEND);
	sprintf(buf, "|%*s%s%*s|%*s%s%*s|%*s%s%*s| kills deaths ratio clanpoints | awin  aloss aintr |\n\r",
	  // left padding for "clan name"
	 ( name_padding - (name_padding/2) ), "",
	  // "clan name"
	 CLANPK_DO_CLANTABLE_CLAN_NAME_LEGEND,
	  // right padding for "clan name"
	 ( name_padding / 2 ), "",
	  // left padding for "leader"
	 ( leader_name_padding - (leader_name_padding/2) ), "",
	 // "leader"
	 CLANPK_DO_CLANTABLE_CLAN_LEADER_NAME_LEGEND,
	 // right padding for "leader"
	 ( leader_name_padding / 2 ), "",
         // left padding co-leader
         (coleader_padding - (coleader_padding/2)), "",
         CLANPK_DO_CLANTABLE_CLAN_COLEADER_NAME_LEGEND,
         ( coleader_padding / 2 ), "");
	send_to_char(buf, ch);

	sprintf(buf, "+%s+\n\r", horizontal_seperator);
	send_to_char(buf, ch);
	for (a_clan=get_first_clan(); a_clan!=NULL; a_clan=get_next_clan(a_clan))
	{
#ifdef DEBUG_CLANPK
		sprintf(buf, "do_clantable: [%02d:%.20s]", a_clan->num, a_clan->name);
		log_string(buf);
#endif
		/* these shouldn't happen with get_next_clan
		if (a_clan->num == -1) {
			// unused slot
			continue;
		}
		else if (a_clan->num != i)
		{
			// invalid clan
			continue;
		}
		else
		*/
		if ( a_clan->num == 0 )
		{
			continue;
		}
		else
		{
			clan_count++;
			total_fights = a_clan->kills + a_clan->deaths;
			if ( total_fights < 1 )
			{
				sprintf(ratio_buf, "None");
			}
			else
			{
				kill_ratio = (float) a_clan->kills / total_fights;
				sprintf(ratio_buf, "%1.2f", kill_ratio);
			}
			sprintf(buf, "| %-*.*s | %-*.12s | %-*.12s | %5d  %5d  %-.4s %10d | %5d %5d %5d |\n\r",
			 max_clan_name_len, MAX_CLAN_NAME_LENGTH, a_clan->name,
			 max_clan_leader_name_len, a_clan->leader_name,
                         max_clan_coleader_name_len, a_clan->coleader_name,
			 a_clan->kills, a_clan->deaths, ratio_buf,a_clan->clan_points,
			 a_clan->arena_kills, a_clan->arena_deaths, a_clan->arena_self_kills);
			/*
			sprintf(buf, " | %-18s | %-12s | %5d  %5d   %-.4s |\n\r",
			 a_clan->name, a_clan->leader_name, a_clan->kills, a_clan->deaths, ratio_buf);
			*/
			send_to_char(buf, ch);
		}
	}
	sprintf(buf, "\\%s/\n\r", horizontal_seperator);
	send_to_char(buf, ch);
	/*
	send_to_char(" \\--------------------+--------------+---------------------/\n\r", ch);
	*/
}

void do_clanpk_leader(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char *old_leader_name;
	CHAR_DATA *new_leader;
	CLANPKTABLE_DATA *the_clan;


	if ( !IS_IMMORTAL(ch) )
	{
		do_huh(ch, argument);
		return;
	}

	new_leader = get_char_world(ch, argument);
	if ( new_leader == NULL )
	{
		sprintf(buf, "Unable to find player \"%.30s\", are you sure they're loaded?",
		 argument);
		send_to_char(buf, ch);
		return;
	}
	if ( IS_NPC(new_leader) )
	{
		sprintf(buf, "No, \"%.30s\" is an NPC.", new_leader->name);
		send_to_char(buf, ch);
		return;
	}

	the_clan = get_clan_by_char(new_leader);
	if ( the_clan == get_clanless_clan() )
	{
		sprintf(buf, "No, \"%.30s\" isn't in a clan.", new_leader->name);
		send_to_char(buf, ch);
	}

	old_leader_name = the_clan->leader_name;
	if (strlen(old_leader_name) == 0)
	{
		sprintf(buf, "Ok, giving leadership of \"%.30s\" to \"%.30s\".",
		 the_clan->name, new_leader->name);
	}
	else
	{
		sprintf(buf, "Ok, usurping \"%.30s\" in favour of \"%.30s\" as leader of \"%.30s\".",
		 the_clan->leader_name, new_leader->name, the_clan->name);
	}
	send_to_char(buf, ch);
        if( !IS_SET(new_leader->clanbits, CLAN_LDR))
         SET_BIT(new_leader->clanbits, CLAN_LDR);
        if( IS_SET(new_leader->clanbits, CLAN_CO))
         REMOVE_BIT(new_leader->clanbits, CLAN_CO);
	clanpktable__set_leader(new_leader, the_clan);
}

void do_clanpk_coleader(CHAR_DATA *ch, char *argument)
{
        char buf[MAX_STRING_LENGTH];  
        char *old_coleader_name;
        CHAR_DATA *new_coleader;
        CLANPKTABLE_DATA *the_clan;
         
                
        if ( !IS_IMMORTAL(ch) )
        {
                do_huh(ch, argument);
                return;
        }
        
        new_coleader = get_char_world(ch, argument);
        if ( new_coleader == NULL )
        {
                sprintf(buf, "Unable to find player \"%.30s\", are you sure they're loaded?",
                 argument);
                send_to_char(buf, ch);
                return;
        }
        if ( IS_NPC(new_coleader) )
        {
                sprintf(buf, "No, \"%.30s\" is an NPC.", new_coleader->name);
                send_to_char(buf, ch);
                return;
        }
                 
        the_clan = get_clan_by_char(new_coleader);
        if ( the_clan == get_clanless_clan() )
        {
                sprintf(buf, "No, \"%.30s\" isn't in a clan.", new_coleader->name);
                send_to_char(buf, ch);
        }
        
        old_coleader_name = the_clan->coleader_name;
        if (strlen(old_coleader_name) == 0)
        {
                sprintf(buf, "Ok, giving co-leadership of \"%.30s\" to \"%.30s\".",
                 the_clan->name, new_coleader->name);
        }
        else
        {
                sprintf(buf, "Ok, usurping \"%.30s\" in favour of \"%.30s\" as co-leader of \"%.30s\".",
                 the_clan->coleader_name, new_coleader->name, the_clan->name);
        }
        send_to_char(buf, ch);
        if( !IS_SET(new_coleader->clanbits, CLAN_CO))
         SET_BIT(new_coleader->clanbits, CLAN_CO);
        if( IS_SET(new_coleader->clanbits, CLAN_LDR))
         REMOVE_BIT(new_coleader->clanbits, CLAN_LDR);
        clanpktable__set_coleader(new_coleader, the_clan);
}

void dump_clantable()
{
	char buf[MAX_STRING_LENGTH];
	int clan_count = 0;
	CLANPKTABLE_DATA *a_clan;

	log_string("/--------|--------------------+--------------+---------------\\");
	log_string("|  i/num |     clan  name     |    leader    |  kills deaths |");
	log_string("+--------|--------------------+--------------+---------------+");
	for (a_clan=get_first_clan(); a_clan!=NULL; a_clan=get_next_clan(a_clan))
	{
		clan_count++;
		sprintf(buf, "| %02d/%02d  | %-18s | %-12s | %5d  %5d  |",
		 clan_count, a_clan->num,
		 a_clan->name, a_clan->leader_name,
		 a_clan->kills, a_clan->deaths);
		log_string(buf);
	}
	log_string("\\--------|--------------------+--------------+---------------/");
}

/*
CLANPKTABLE_DATA *raw_get_clan_by_num(int num)
{
	CLANPKTABLE *cur;
	CLANPKTABLE *prev;
	int i;
#ifdef DEBUG_CLANPK_VERBOSE
	char buf[MAX_STRING_LENGTH];

	sprintf(buf, "raw_get_clan_by_num(%d)", num);
	log_string(buf);
#endif

	cur = prev = clanpk_list;
	for (i=0; (i<num) && (cur!=NULL); i++) {
		prev = cur;
		cur = cur->next;
	}
	return cur;
}
*/

CLANPKTABLE_DATA *get_clanless_clan()
{
	return clanless_clan;
}

CLANPKTABLE_DATA *get_first_clan()
{
#ifdef DEBUG_CLANPK_VERBOSE
	char buf[MAX_STRING_LENGTH];
#endif
	CLANPKTABLE_DATA *a_clan;

	a_clan = clanpk_list;
#ifdef DEBUG_CLANPK_VERBOSE
	sprintf(buf, "get_first_clan: [%02d:%.20s]", a_clan->num, a_clan->name);
	log_string(buf);
#endif

	return a_clan;
}

CLANPKTABLE_DATA *get_next_clan(CLANPKTABLE_DATA *a_clan)
{
#ifdef DEBUG_CLANPK
	char buf[MAX_STRING_LENGTH];
#endif
	CLANPKTABLE_DATA *another_clan = NULL;

	if (a_clan == NULL)
	{
		// should probably bug() here
		return NULL;
	}
	another_clan = a_clan->next;
#ifdef DEBUG_CLANPK
	if (another_clan == NULL)
	{
		sprintf(buf, "get_next_clan: exiting, no next clan found");
	}
	else
	{
		sprintf(buf, "get_next_clan: [%02d:%.20s]",
		 another_clan->num, another_clan->name);
	}
	log_string(buf);
#endif

	return another_clan;
}

/*
// appends to buf and returns buf
char *ratio2str(int value, int max, char *buf)
{
	// but that has p
	if ( max < 1 )
	{
		sprintf(buf, "None");
	}
	else if ( value < 1 )
	{
		sprint(buf, "0.00");
	}
	else if ( value >= max )
	{
		sprint(buf, "1.00");
	}
	else
	{
		// would probably be better to use sprintf with a "%02f"
		sprintf(buf, "0.%02d", (int)((value*100)/max));
	}
	return buf;
}
*/

