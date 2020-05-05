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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> /* unlink() */
#include "merc.h"
#include "clantable.h"

bool	check_social	args( ( CHAR_DATA *ch, char *command, char *argument ) );
void	make_preg	args( ( CHAR_DATA *mother, CHAR_DATA *father ) );
char    last_command[MAX_STRING_LENGTH];
int can_interpret args( (CHAR_DATA *ch, int cmd) );

int can_interpret (CHAR_DATA *ch, int cmd)
{
    bool cando = FALSE;

    if (ch->level == 12)
	cando = TRUE;

    if (cmd_table[cmd].level > get_trust(ch)) return 0;

    if (cmd_table[cmd].race == 0
        && cmd_table[cmd].discipline == 0)
	cando = TRUE;

    if (cmd_table[cmd].race > 0
         && cmd_table[cmd].discipline == 0
         && ch->class == cmd_table[cmd].race)
	cando = TRUE;

    if (!cando) return 0;


    if ( ch->position < cmd_table[cmd].position )
	return -1; 

    return 1;
}

/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2



/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;

bool  check_disabled (const struct cmd_type *command);
BAN_DATA *ban_first;
DISABLED_DATA *disabled_first;
#define END_MARKER    "END" /* for load_disabled() and save_disabled()*/


/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */

    /* Name / Function / Min Position / Level / Log / Class,DiscLevel,DiscName*/
    { "north",		do_north,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "east",		do_east,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "south",		do_south,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "west",		do_west,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "up",			do_up,			POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "down",		do_down,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "cast",		do_cast,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "inventory",	do_inventory,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "steal",          do_steal,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "crecall",        do_clan_recall,         POS_STANDING,    0,  LOG_NORMAL, 0,0,0 },
    { "crack",		do_crack,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
//    { "drag",		do_drag,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
/*KlineMark    { "stealsoul",      do_stealsoul,           POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
    { "absorb",         do_absorb,              POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
*/    { "decapitate",     do_decapitate,          POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
    { "for",            do_for,                 POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },  
    { "force",          do_force,               POS_DEAD,        7,  LOG_ALWAYS, 0,0,0  },
    { "diagnose",	do_diagnose,  	        POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "dismount",	do_dismount,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "enter",		do_enter,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "get",		do_get,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "kill",		do_kill,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "look",		do_look,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "ls",		do_look,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "meditate",	do_meditate,	        POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "mount",		do_mount,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "msp",            do_msp,                 POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
    { "order",		do_order,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "rest",		do_rest,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sit",		do_sit,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "stand",		do_stand,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "tell",		do_tell,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wake",		do_wake,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "whisper",	do_whisper,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wield",		do_wear,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wizhelp",	do_wizhelp,		POS_DEAD,	 7,  LOG_NORMAL, 0,0,0	},
    { "version",	do_version,		POS_DEAD,	 1,  LOG_NORMAL, 0,0,0	},
    { "donate",	        do_donate,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "exits",		do_exits,		POS_STANDING,	 1,  LOG_NORMAL, 0,0,0  },
    { "call",           do_call,                POS_DEAD,        0,  LOG_NORMAL, 0,0,0  },
    { "quest",          do_quest,               POS_SITTING,     2,  LOG_NORMAL, 0,0,0  },
    { "group",          do_group,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0  },
    { "agree",          do_agree,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  }, 
    { "challenge",      do_challenge,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  }, 
    { "withdraw",       do_withdraw,            POS_DEAD,        3,  LOG_NORMAL, 0,0,0  }, 
    { "decline",        do_decline,             POS_DEAD,        3,  LOG_NORMAL, 0,0,0  }, 
    { "tune",           do_tune,                POS_DEAD,        0,  LOG_NORMAL, 0,0,0  },
    { "radio",          do_radio,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0  },
    /*
     * Informational commands.
     */
    { "affect",         do_affect,              POS_SITTING,     0,  LOG_NORMAL, 0,0,0},
    { "areas",		do_areas,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "arena",		do_arena,		POS_STANDING,	 1,  LOG_NORMAL, 0,0,0},
    { "commands",	do_commands,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "compare",	do_compare,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "consider",       do_consider,            POS_SITTING,     0,  LOG_NORMAL, 0,0,0},
    { "conspire",       do_clan1,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0},
    { "clanspy",        do_spyclan,             POS_STANDING,    0,  LOG_NORMAL, 0,0,0},
    { "clanattack",     do_clan_attack,         POS_STANDING,    0,  LOG_NORMAL, 0,0,0},
    { "credits",	do_credits,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "equipment",	do_equipment,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "examine",	do_examine,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0},
    { "help",		do_help,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "report",		do_report,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "exp",            do_weaplist,	        POS_FIGHTING,    0,  LOG_NORMAL, 0,0,0},
    { "level",          do_weaplist,	        POS_FIGHTING,    0,  LOG_NORMAL, 0,0,0},
    { "score",		do_score,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0},
    { "scan",           do_scan,                POS_SITTING,     0,  LOG_NORMAL, 0,0,0  },
    { "skill",		do_skill,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0 },
    { "seance",         do_clan6,               POS_DEAD,        0,  LOG_NORMAL, 0, 0, 0 },
    { "asylum",         do_clan2,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0 },
    { "kink",         do_clan3,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0 },
    { "darkspeak",         do_clan4,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0 },
    { "scheme",         do_clan5,               POS_DEAD,        0,  LOG_NORMAL, 0,0,0 },
    { "klineownsus",	do_clan7,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "spells",		do_spell,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0 },
    { "socials",	do_socials,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "time",		do_time,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "weather",	do_weather,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "who",		do_who,			POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "who2",		do_who2,			POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "bounty",         do_bounty,              POS_SITTING,     0,  LOG_ALWAYS, 0,0,0 },
    { "bountylist",     do_bounty_list,         POS_SITTING,     0,  LOG_NORMAL, 0,0,0 },
    { "chant",         do_summon_wear_loc,      POS_STANDING,    2,  LOG_NORMAL, 0, 0, 0 }, 
    { "pray",		do_pray,		POS_MEDITATING,	 1,  LOG_NORMAL, 0, 0, 0 },
    { "inscribe",	do_one_ring,		POS_STANDING,    1,  LOG_NORMAL, 0, 0, 0 }, 
    { "quicken",	do_quicken,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "makeeq",		do_make_eq,		POS_DEAD,	 11, LOG_NORMAL, 0, 0, 0 },
    { "commit",         do_commit,              POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "repent",         do_repent,              POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    /* Kline Wuz Here */
    { "beep",		do_beep,		POS_STANDING,	 2,  LOG_NORMAL, 0,0,0 },
    { "readaura",	do_readaura,		POS_STANDING,	 3,  LOG_NORMAL, 0,0,0 },
    { "classconfig",	do_class_config,	POS_DEAD,	12,  LOG_ALWAYS, 0,0,0 },
    { "outputsocials",  do_output_socials,	POS_DEAD,	12,  LOG_ALWAYS, 0,0,0 },
    { "reloadlib",	do_reload_lib,		POS_DEAD,	12,  LOG_ALWAYS, 0,0,0 },
    { "dpconv",		do_dpconv,		POS_STANDING,	 3,  LOG_NORMAL, 0,0,0 },
    { "testwho",	do_testwho,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "testscore",	do_testscore,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "autostance",	do_autostance,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0 },
    { "knockup",	do_knockup,		POS_DEAD,	 7,  LOG_NORMAL, 0,0,0 },
    { "class",		do_autoclass,		POS_SITTING,     0,  LOG_NORMAL, 0,0,0 },
    { "classme",	do_autoclass,		POS_SITTING,     0,  LOG_NORMAL, 0,0,0 },
    { "selfclass",	do_autoclass,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "autoclass",      do_autoclass,           POS_SITTING,     0,  LOG_NORMAL, 0,0,0 }, 
    { "rolldirt",	do_rolldirt,		POS_STANDING,	 0,  LOG_NORMAL, 0, 0, 0 },
    { "smother",	do_smother,		POS_STANDING,    0,  LOG_NORMAL, 0, 0, 0 },
    { "ethereal",	do_ethereal,		POS_STANDING,    0,  LOG_NORMAL, 0, 0, 0 },
    { "sharpen",	do_sharpen,		POS_STANDING,    0,  LOG_NORMAL, 0, 0, 0 },
    { "token",		do_token,		POS_STANDING,	 0,  LOG_NORMAL, 0, 0, 0 },
    { "stun",		do_stun,		POS_DEAD,	11,  LOG_ALWAYS, 0, 0, 0 },
    { "colscale",	do_colscale,		POS_DEAD,	 0,  LOG_NORMAL, 0, 0, 0 },
    { "delevel",	do_delevel,		POS_DEAD,	 7,  LOG_ALWAYS, 0, 0, 0 },
    { "desperation",	do_final_blow,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "askills",	do_askills,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "newhelp",	do_newhelp,		POS_STANDING,	 0,  LOG_NORMAL, 0, 0, 0 },
    { "apconvert",	do_apconv,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "pipe",		do_pipe,		POS_DEAD,	 0,  LOG_NORMAL, 0, 0, 0 },
    { "info",		do_info,		POS_DEAD,	11,  LOG_ALWAYS, 0, 0, 0 },
    { "deathmatch",	do_deathmatch,		POS_DEAD,	 8,  LOG_ALWAYS, 0, 0, 0 },
    { "regroup",	do_regroup,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "groupconfig",	do_group_config,	POS_DEAD,	11,  LOG_ALWAYS, 0, 0, 0 },
    { "morphcheck",	do_morph_check,		POS_DEAD,	 0,  LOG_NORMAL,0,0,0 },
    /* Vampires */
    /* Shadow, moved to parse sooner */
    { "shadow",         do_shadow,              POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "sharpen",        do_sharpen,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "gourge",         do_gourge,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "bloodwater",     do_bloodwater,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "spew",           do_spew,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "darkness",       do_vampdarkness,        POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "dragonform",     do_dragonform,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "objform",        do_objform,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "baal",           do_baal,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "inferno",        do_inferno,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "zombie",         do_zombie,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "fleshcraft",     do_fleshcraft,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "entrance",       do_entrance,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "tendrils",       do_tendrils,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "lamprey",        do_lamprey,             POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "assassinate",    do_assassinate,         POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "mindblast",      do_mindblast,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "tongue",         do_tongue,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
//    { "bloodgate",      do_vamp_gate,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "pigeon",         do_pigeon,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "embrace",        do_embrace,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "decay",          do_decay,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "infirmity",      do_infirmity,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "guardian",       do_guardian,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "servant",        do_servant,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "scales",         do_scales,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "cserpent",       do_cserpent,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "scream",         do_scream,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "bloodrod",       do_bloodrod,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "shadowgaze",     do_shadowgaze,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "share",          do_share,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "frenzy",         do_frenzy,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "drain",          do_drain,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "flamehands",     do_flamehands,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "shade",          do_shade,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "bonemod",        do_bonemod,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "clone",          do_clone,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "coil",           do_coil,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "tide",           do_tide,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "flash",          do_flash,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "death",          do_death,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "acidblood",      do_acid,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "return",         do_retrn,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "rot",            do_rot,                 POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "hades",          do_hades,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "potency",        do_potency,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "plasma",         do_plasma,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
//    { "shadowstep",     do_shadowstep,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "earthmeld",      do_earthmeld,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "theft",          do_theft,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "zuloform",       do_zuloform,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "change",         do_change,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "regenerate",     do_regenerate,          POS_DEAD,        3,  LOG_NORMAL, 0,0,0},
    { "fcommand",       do_fcommand,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "bloodme",	do_bloodme,		POS_DEAD,	 11, LOG_NORMAL, 0,0,0},
    /* Drow */
    { "strangle",	do_strangle,		POS_FIGHTING,	 3,  LOG_NORMAL, 0,0,0},
    { "drowfire",       do_drowfire,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "drowpowers",	do_drowpowers,		POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "levitate",	do_levitate,		POS_STANDING,	 3,  LOG_NORMAL, 0,0,0},
    { "chaosblast",	do_chaosblast,		POS_FIGHTING,	 3,  LOG_NORMAL, 0,0,0},
    { "drowhate",       do_drowhate,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "spiderform",     do_spiderform,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "drowshield",     do_drowshield,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "darkness",       do_darkness,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "earthshatter",   do_earthshatter,        POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "garotte",        do_garotte,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "dgarotte",       do_dgarotte,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    /* Jedi */
    { "jedigain",	do_jedi_show_powers,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0},
    { "combatstyle",	do_combat_style,	POS_STANDING,	 3,  LOG_NORMAL, 0,0,0},
    { "sabercraft",	do_saber_craft,		POS_STANDING,	 3,  LOG_NORMAL, 0,0,0},
    { "jover",		do_jover,		POS_FIGHTING,	 3,  LOG_NORMAL, 0,0,0},
    { "jflip",          do_jflip,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "jupper",         do_jupper,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "jback",          do_jback,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "jslice",         do_jslice,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "jswipe",         do_jswipe,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "jslash",         do_jslash,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "jroll",          do_jroll,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "forceheal",	do_jedi_force_heal,	POS_FIGHTING,	 3,  LOG_NORMAL, 0,0,0},
    { "forceabsorb",    do_jedi_force_absorb,   POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "forceshield",    do_jedi_force_shield,   POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "forcemindtrick", do_jedi_force_mind_trick,POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "forcesight",     do_jedi_force_sight,    POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "forcespeed",     do_jedi_force_speed,    POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "forcejump",      do_jedi_force_jump,     POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "forcepush",      do_jedi_force_push,     POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "forcepull",      do_jedi_force_pull,     POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "forcegrip",      do_jedi_force_grip,     POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "forcedrain",     do_jedi_force_drain,    POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "forcerage",      do_jedi_force_rage,     POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "forcelighting",  do_jedi_force_lightning,POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    /* dragons */
    { "fly",            do_dragon_fly,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "breath",         do_dbreath,             POS_FIGHTING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "dragongain",     do_dragongain,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "dscan",          do_dscan,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "snatch",         do_snatch,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "grab",           do_grab,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "dthrow",         do_dthrow,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "drelease",       do_drelease,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "highclaw",       do_highclaw,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "tailslap",       do_tailslap,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "plummet",        do_plummet,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0},
    { "ddrop",          do_ddrop,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    { "wingbuffet",     do_wingbuffet,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0},
    /* Shadow */
    { "conceal",	do_conceal,		POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "mastery",	do_mastery,		POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "circle",		do_circle,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "tumble",		do_tumble,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "thirdeye",	do_thirdeye,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "traproom",	do_traproom,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    /* Wolf, moved to parse sooner */
    { "run",            do_run,                 POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "runeweave",	do_runeweave,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    /* Wolf */
    { "silverclaws",	do_sclaws,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "moonbeam",	do_moonbeam,            POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
//    { "moongate",       do_moongate,            POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "gmotherstouch",  do_gmotherstouch,       POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "motherstouch",   do_motherstouch,        POS_FIGHTING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "flameclaws",     do_flameclaws,          POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "rend",           do_rend,                POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "skin",           do_skin,                POS_STANDING,    3,  LOG_NORMAL, 0, 0, 0 },
    { "jawlock",	do_jawlock,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "perception",	do_perception,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "roar",		do_roar,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "quills",		do_quills,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "slam",		do_slam,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "shred",		do_shred,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "talons",		do_talons,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "devour",		do_devour,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
//    { "callwild",	do_call_wild,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "wither",		do_wither,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
//    { "wburrow",	do_wburrow,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "shift",		do_shift,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "claw",           do_claw,                POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  }, 
    { "claws",		do_claws,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "fangs",		do_fangs,		POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "shield",		do_shield,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "vanish",		do_vanish,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "web",		do_web,			POS_FIGHTING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "disciplines",	do_disciplines,		POS_STANDING,	 3,  LOG_NORMAL, 0, 0, 0 },
    { "setleader",      do_setleader,           POS_SITTING,    11,  LOG_ALWAYS, 0,0,0 },
    { "setcoleader",    do_setcoleader,         POS_SITTING,    11,  LOG_ALWAYS, 0,0,0 },
    { "showrace",       do_showrace,            POS_SITTING,     0,  LOG_NORMAL, 0,0,0 },
    { "setdeca",	do_set_deca,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0 },
    { "setdecap",       do_set_decap,           POS_SITTING,     0,  LOG_ALWAYS, 0,0,0 },
    { "setwho",         do_setwho,              POS_SITTING,     0,  LOG_NORMAL, 0,0,0 },
    { "wizlist",	do_wizlist,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "npack",		do_pack,		POS_STANDING,	 7,  LOG_NORMAL, 0,0,0 },
    /*
     * Configuration commands.
     */
    { "aclaw",          do_aclaw,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
//    { "agate",          do_arcanic_gate,        POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "amight",         do_arcanic_might,       POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "anum",           do_anum,                POS_DEAD,       11,  LOG_NORMAL, 0,0,0  },
//    { "celestial",      do_celestial,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "checkwizlist",   do_checkwizlist,        POS_DEAD,       11,  LOG_NORMAL, 0,0,0  },
    { "chi",            do_chi,                 POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "clairvoyance",   do_clairvoyance,        POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "at",             do_at,                  POS_DEAD,        7,  LOG_NORMAL, 0,0,0  },
//    { "atravel",        do_atravel,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "atail",          do_atail,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
//    { "atransport",     do_atransport,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "backfist",       do_backfist,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "balance",        do_balance,             POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "bash",           do_bash,                POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  }, 
    { "bet",            do_bet,                 POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "bodyslide",      do_bodyslide,           POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
//    { "burrow",         do_burrow,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "calm",           do_calm,                POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "cleanse",        do_cleanse,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "cloak",          do_cloak,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "convert",        do_convert,             POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "curse",          do_curse,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "customcombo",    do_customcombo,         POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "cutgem",         do_cutgem,              POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "deathtouch",     do_deathtouch,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "defile",         do_defile,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "denjin",         do_denjin,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "detox",          do_detox,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "dshift",         do_dshift,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "elbow",          do_elbow,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "entangle",       do_entangle,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "essence",        do_essence,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "evolve",         do_evolve,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "explode",        do_explode,             POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "fanatisism",     do_fanatisism,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "firebolt",       do_firebolt,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "firetrap",       do_firetrap,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "flex",           do_flex,                POS_RESTING,     3,  LOG_NORMAL, 0,0,0  },
    { "flood",          do_flood,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "ice",            do_ice,                 POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "whirlwind",      do_whirlwind,           POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "forge",          do_new_forge,           POS_STANDING,    1,  LOG_NORMAL, 0,0,0  },
    { "frighten",       do_frighten,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "gainpower",      do_gainschool,          POS_STANDING,    0,  LOG_NORMAL, 0,0,0  },
//    { "gate",           do_gate,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "give",           do_give,                POS_SITTING,     0,  LOG_NORMAL, 0,0,0  },
    { "givelife",       do_givelife,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "godsbless",      do_godsbless,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "grant",          do_grant,               POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "gust",           do_gust,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "heal",           do_heal,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "healtouch",      do_healtouch,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "heartpunch",     do_heartpunch,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "heatmetal",      do_heatmetal,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "hilo",           do_hilo,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "holdperson",     do_holdperson,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "infect",         do_infect,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "inlay",          do_inlay,               POS_STANDING,    1,  LOG_NORMAL, 0,0,0  },
    { "kneethrust",     do_kneethrust,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "liferestore",    do_life_restore,        POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "mine",           do_mine,                POS_STANDING,    1,  LOG_NORMAL, 0,0,0  },
    { "metamorph",      do_metamorph,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "pain",           do_pain,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "palmstrike",     do_palmstrike,           POS_FIGHTING,   3,  LOG_NORMAL, 0,0,0  },
    { "parasite",       do_parasite,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "path",           do_path,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "poison",         do_poison,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "poisonweapon",   do_poisonw,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "polish",         do_polish,              POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "primalurge",     do_primal_urge,         POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "rage",           do_rage,                POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "rainfire",       do_rainfire,            POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "rename",         do_rename,              POS_STANDING,    2,  LOG_NORMAL, 0,0,0  },
    { "relax",          do_relax,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "reverse",        do_reverse,             POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "roottravel",     do_roottravel,          POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "roundhouse",     do_roundhouse,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "scry",           do_scry,                POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "shatter",        do_shatter,             POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "slash",          do_slash,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "slow",           do_slow,                POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "smelt",          do_smelt,               POS_STANDING,    1,  LOG_NORMAL, 0,0,0  },
    { "shadowplane",    do_shadowplane,         POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "spit",           do_spit,                POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "stasis",         do_stasis,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "strike",         do_strike,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "summon",         do_summon,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "sweep",          do_sweep,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "syphon",         do_syphon,              POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "taillash",       do_taillash,            POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "technique",      do_technique,           POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "tempstasis",     do_temp_stasis,         POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "thornblast",     do_thornblast,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "thrustkick",     do_thrustkick,          POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
//    { "travel",         do_travel,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "voice",          do_voice,               POS_FIGHTING,    3,  LOG_NORMAL, 0,0,0  },
    { "powers",         do_powers,              POS_SITTING,     0,  LOG_NORMAL, 0,0,0  },
    { "alignment",	do_alignment,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "channels",	do_channels,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "config",		do_config,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "password",	do_password,	        POS_DEAD,	 0,  LOG_NEVER,  0,0,0	},
    { "title",		do_title,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "wimpy",		do_wimpy,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "ansi",		do_ansi,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "autoexit",	do_autoexit,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "autoloot",	do_autoloot,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "autosac",	do_autosac,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "blank",		do_blank,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "brief",		do_brief,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "cprompt",	do_cprompt,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "prompt",		do_prompt,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},

    /*
     * Communication commands.
     */
    { "answer",		do_answer,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gs",		do_gocial,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "gocial",		do_gocial,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "chat",		do_chat,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { ".",		do_chat,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "loner",          do_loner,               POS_STANDING,    3,  LOG_NEVER, 0,0,0  },
    { "clan",           do_clan,                POS_STANDING,    12, LOG_NORMAL, 0,0,0  },
    { "clantable",      do_clantable,	        POS_STANDING,	 12, LOG_NORMAL, 0,0,0  },
    { "ct",	        do_clantalk,	        POS_SLEEPING,    0,  LOG_NORMAL, 0,0,0	},
    { "emote",		do_emote,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { ",",		do_emote,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gtell",		do_gtell,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { ";",		do_gtell,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "music",		do_music,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "note",		do_note,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "pose",		do_emote,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "question",	do_question,	        POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "newtalk",	do_newtalk,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "reply",		do_reply,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "say",		do_say,			POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "'",		do_say,			POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "shout",		do_shout,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "yell",		do_yell,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "ignore",		do_ignore,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "unignore",	do_unignore,	        POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    /*Object manipulation commands.*/
    { "activate",	do_activate,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "brandish",	do_brandish,	        POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "close",		do_close,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "draw",		do_draw,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "drink",		do_drink,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "drop",		do_drop,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "eat",		do_eat,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "empty",		do_empty,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "fill",		do_fill,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hold",		do_wear,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "lock",		do_lock,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "morph",		do_morph,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "open",		do_open,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "pick",		do_pick,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    //{ "press",		do_press,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
   // { "pull",		do_pull,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "put",		do_put,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "quaff",		do_quaff,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recite",		do_recite,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "remove",		do_remove,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "remort",         do_remort,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0  },
    { "sheath",		do_sheath,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "take",		do_get,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "read",		do_read,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    { "shoot",		do_shoot,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "throw",		do_throw,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "turn",		do_turn,		POS_MEDITATING,	 0,  LOG_NORMAL, 0,0,0	},
    //{ "twist",		do_twist,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sacrifice",	do_sacrifice,	        POS_STANDING,	 1,  LOG_NORMAL, 0,0,0	},
    { "unlock",		do_unlock,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "wear",		do_wear,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "zap",		do_zap,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "junk",           do_sacrifice,	        POS_STANDING,    0,  LOG_NORMAL, 0,0,0	},
    /* Combat commands.*/
    { "generation",     do_generation,	        POS_STANDING,	10,  LOG_ALWAYS, 0,0,0 },
    { "backstab",	do_backstab,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "berserk",	do_berserk,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "bs",		do_backstab,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    //{ "circle",         do_circle,              POS_FIGHTING,    0,  LOG_NORMAL, 0,0,0 },
    { "disarm",		do_disarm,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "flee",		do_flee,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "fightstyle",	do_fightstyle,	        POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hurl",		do_hurl,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "kick",		do_kick,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "punch",		do_punch,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "rescue",		do_rescue,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "stance",		do_stance,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    /* Miscellaneous commands. */
    { "artifact",	do_artifact,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "blindfold",	do_blindfold,	        POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "claim",		do_claim,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "complete",	do_complete,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "finger",		do_finger,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "follow",		do_follow,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "gag",		do_gag,			POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "gift",		do_gift,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hide",		do_hide,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "home",		do_home,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "hunt",		do_hunt,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "locate",		do_locate,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "practice",	do_practice,	        POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "qui",		do_qui,			POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "quit",		do_quit,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recall",		do_recall,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "escape",		do_escape,		POS_DEAD,	 3,  LOG_NORMAL, 0,0,0	},
    { "/",		do_recall,		POS_FIGHTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "recharge",	do_recharge,	        POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "safe",		do_safe,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "save",		do_save,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "savecommands",   do_savecommands,        POS_DEAD,        0,  LOG_ALWAYS, 0,0,0  },
    { "sleep",		do_sleep,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0	},
    { "sneak",		do_sneak,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "spy",		do_spy,			POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    { "notravel",	do_notravel,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0 },
    { "nosummon",	do_nosummon,	        POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "train",          do_train,		POS_SITTING,     0,  LOG_NORMAL, 0,0,0  },
    { "tie",		do_tie,			POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "track",		do_track,		POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "unpolymorph",    do_unpolymorph,	        POS_STANDING,	 4,  LOG_NORMAL, 0,0,0 },
    { "untie",		do_untie,		POS_STANDING,	 3,  LOG_NORMAL, 0,0,0	},
    { "visible",	do_visible,		POS_SLEEPING,	 0,  LOG_NORMAL, 0,0,0 },
    { "where",		do_where,		POS_SITTING,	 0,  LOG_NORMAL, 0,0,0	},
    //{ "setleader",      do_clanpk_leader,       POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    //{ "setcoleader",    do_clanpk_coleader,     POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "tick",		do_tick,                POS_DEAD,       11,  LOG_ALWAYS, 0,0,0},
    { "form",		do_form,                POS_DEAD,       11,  LOG_ALWAYS, 0,0,0},
    { "resetarea",	do_resetarea,           POS_DEAD,        7,  LOG_ALWAYS, 0,0,0},
    { "transfer",       do_transfer,            POS_DEAD,        7,  LOG_NORMAL,0,0,0 },
    { "freeze",		do_freeze,              POS_DEAD,       10,  LOG_ALWAYS,0,0,0 },  
    { "ban",		do_ban,	                POS_DEAD,	11,  LOG_ALWAYS, 0,0,0	},
    { "banish",         do_banish,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0 },
    { "configure",	do_configure,           POS_DEAD,       12,  LOG_ALWAYS,0,0,0 },
    /* Start of OLC Shit. Hmm */
    { "tag",            do_tag,                 POS_STANDING,    2,  LOG_NORMAL, 0,0,0 },
    { "paintball",      do_paintball,           POS_DEAD,        1,  LOG_NORMAL, 0,0,0 },
    { "setpaintball",   do_setpaintball,        POS_DEAD,        8,  LOG_ALWAYS, 0,0,0 },
    { "damn",		do_damn,		POS_STANDING,	11,  LOG_NORMAL, 0,0,0 },
    { "hset",		do_hset,                POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "hedit",		do_hedit,               POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "hlist",		do_hlist,	        POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "revenge",        do_kale_owns_you,       POS_DEAD,       12,  LOG_NORMAL, 0,0,0 },
    { "mcreate",	do_mcreate, 	        POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "delete",         do_delete,              POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "ocreate",	do_ocreate,	        POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "redit",		do_redit,         	POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "rformat",        do_rformat,             POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "redeem",         do_redeem,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0 },
    { "clanname",	do_clanname,            POS_STANDING,	10,  LOG_ALWAYS, 0,0,0	},
    { "humanform",	do_humanform,           POS_SITTING,	 2,  LOG_NORMAL, 0,0,0 },
    { "truesight",	do_truesight,           POS_SITTING,	 2,  LOG_NORMAL, 0,0,0  },
    /* Immortal commands.    */
    { "trust",		do_trust,	        POS_DEAD,	12,  LOG_ALWAYS, 0,0,0 },
    { "allow",		do_allow,	        POS_DEAD,	11,  LOG_ALWAYS, 0,0,0	},
    { "bind",		do_bind,	        POS_DEAD,	10,  LOG_ALWAYS, 0,0,0	},
    { "clearrace",      do_clearrace,           POS_DEAD,       10,  LOG_ALWAYS, 0,0,0  },
    { "clearstats",	do_clearstats,          POS_STANDING,	 0,  LOG_NORMAL, 0,0,0	},
    { "create",		do_create,	        POS_STANDING,	 8,  LOG_NORMAL, 0,0,0	},
    { "deny",		do_deny,	        POS_DEAD,	10,  LOG_ALWAYS, 0,0,0	},
    { "disable",	do_disable,	        POS_DEAD,	11,  LOG_ALWAYS, 0,0,0 },
    { "disconnect",	do_disconnect,          POS_DEAD,	10,  LOG_ALWAYS, 0,0,0	},
    { "freeze",		do_freeze,	        POS_DEAD,	9,  LOG_ALWAYS, 0,0,0	},
    { "paradox",	do_paradox,	        POS_DEAD,	11, LOG_ALWAYS, 0,0,0 },
    { "qset",		do_qset,	        POS_DEAD,	 7,  LOG_ALWAYS, 0,0,0	},
    { "qstat",		do_qstat,	        POS_DEAD,	 7,  LOG_ALWAYS, 0,0,0	},
    { "qtrust",		do_qtrust,	        POS_DEAD,	10,  LOG_ALWAYS, 0,0,0	},
    { "ntrust",		do_ntrust,	        POS_DEAD,	 7,  LOG_ALWAYS, 0,0,0	},
    { "reboo",		do_reboo,	        POS_DEAD,	11,  LOG_NORMAL, 0,0,0	},
    { "reboot",		do_reboot,	        POS_DEAD,	11,  LOG_ALWAYS, 0,0,0	},
    { "copyover",       do_copyover,            POS_DEAD,       11,  LOG_NORMAL, 0,0,0  },
    { "shutdow",	do_shutdow,	        POS_DEAD,	12,  LOG_NORMAL, 0,0,0	},
    { "shutdown",	do_shutdown,            POS_DEAD,	12,  LOG_ALWAYS, 0,0,0	},
    { "users",		do_users,	        POS_DEAD,	 8,  LOG_NORMAL, 0,0,0	},
    { "omni",		do_omni,	        POS_DEAD,	 7,  LOG_NORMAL, 0,0,0 },
    { "wizlock",	do_wizlock,	        POS_DEAD,	11,  LOG_ALWAYS, 0,0,0	},
    { "expmult",        do_expmult,             POS_DEAD,       10,  LOG_ALWAYS, 0,0,0 },
    { "setlog",	        do_setlog,	        POS_STANDING,	 0,  LOG_ALWAYS, 0,0,0 },
    { "setexit",        do_setexit,	        POS_STANDING,    0,  LOG_ALWAYS, 0,0,0 },
    { "makeclan",       do_makeclan,            POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "delclan",        do_delclan,             POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "makeimmortal",   do_makeimmortal,        POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "delimmortal",    do_delimmortal,         POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "setimmortal",    do_setimmortal,         POS_DEAD,       11,  LOG_ALWAYS, 0,0,0 },
    { "savewizlist",    do_savewizlist,         POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "showclan",       do_show_clan,           POS_DEAD,        3,  LOG_NORMAL, 0,0,0 },
    { "setclan",        do_setclan,             POS_DEAD,        3,  LOG_NORMAL, 0,0,0 },
    { "induct",         do_induct,              POS_STANDING,    3,  LOG_NORMAL, 0,0,0 },
    { "mclear",		do_mclear,	        POS_DEAD,        9,  LOG_ALWAYS, 0,0,0	},
    { "mload",		do_mload,		POS_DEAD,        7,  LOG_ALWAYS, 0,0,0	},
    { "undeny",         do_undeny,  	        POS_DEAD,       10,  LOG_ALWAYS, 0,0,0 },
    { "mset",		do_mset,		POS_DEAD,       10,  LOG_ALWAYS, 0,0,0	},
    { "noemote",	do_noemote,		POS_DEAD,	 9,  LOG_NORMAL, 0,0,0	},
    { "notell",		do_notell,		POS_DEAD,	 9,  LOG_NORMAL, 0,0,0	},
    { "oclone",		do_oclone,		POS_DEAD,	 8, LOG_ALWAYS, 0,0,0	},
    { "oload",		do_oload,		POS_DEAD,	 11, LOG_ALWAYS, 0,0,0	},
    { "oset",		do_oset,		POS_DEAD,	 10, LOG_ALWAYS, 0,0,0	},
    { "otransfer",	do_otransfer,		POS_DEAD,	 8,  LOG_ALWAYS, 0,0,0	},
    { "pload",		do_pload,		POS_DEAD,	12,  LOG_ALWAYS, 0,0,0	},
    { "preturn",	do_preturn,		POS_DEAD,	11,  LOG_NORMAL, 0,0,0	},
    { "pset",		do_pset,		POS_DEAD,	11,  LOG_ALWAYS, 0,0,0	},
    { "purge",		do_purge,		POS_DEAD,	 7,  LOG_NORMAL, 0,0,0	},
    { "release",	do_release,		POS_DEAD,	 9,  LOG_ALWAYS, 0,0,0	},
    { "restore",	do_restore,	        POS_DEAD,	 8,  LOG_ALWAYS, 0,0,0	},
    { "rset",		do_rset,	        POS_DEAD,	 7,  LOG_ALWAYS, 0,0,0	},
    { "silence",	do_silence,	        POS_DEAD,	 7,  LOG_NORMAL, 0,0,0 },
    { "sla",		do_sla,			POS_DEAD,	10,  LOG_NORMAL, 0,0,0	},
    { "slay",		do_slay,		POS_DEAD,	10,  LOG_ALWAYS, 0,0,0	},
    { "sset",		do_sset,	        POS_DEAD,	10,  LOG_NORMAL, 0,0,0 },
    { "transfer",	do_transfer, 		POS_DEAD,	 7,  LOG_ALWAYS, 0,0,0	},
    { "transport",	do_transport,  		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	},
    { "astat",		do_astat,		POS_DEAD,	 	 10, LOG_NORMAL, 0,0,0 },
    { "alock",		do_alock,	POS_DEAD,	 	 10, LOG_NORMAL, 0,0,0 },
    { "aclear",		do_aclear,	POS_DEAD,	 	 10, LOG_NORMAL, 0,0,0 },
    { "abusy",		do_abusy,	POS_DEAD,	 	 10, LOG_NORMAL, 0,0,0 },
    { "bamfin",		do_bamfin,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "bamfout",	do_bamfout,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "echo",		do_echo,	POS_DEAD,	 	 10,  LOG_NORMAL, 0,0,0	},
    { "goto",		do_goto,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "holylight",	do_holylight,POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "incog",          do_incog,   POS_DEAD,          10,  LOG_NORMAL, 0,0,0 },
    { "invis",		do_invis,	POS_DEAD,	 	 10,  LOG_NORMAL, 0,0,0	},
    { "log",		do_log,	POS_DEAD,	 	 11,  LOG_ALWAYS, 0,0,0	},
    { "memory",		do_memory,	POS_DEAD,	 	 11,  LOG_NORMAL, 0,0,0	},
    { "mfind",		do_mfind,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0 },
    { "mstat",		do_mstat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "pstat",		do_pstat,	POS_DEAD,		7,	LOG_NORMAL, 0,0,0 },
    { "mwhere",		do_mwhere,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "ofind",		do_ofind,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "ostat",		do_ostat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "oswitch",	do_oswitch,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "oreturn",	do_oreturn,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "peace",		do_peace,	POS_DEAD,		 7,  LOG_NORMAL, 0,0,0	},
    { "recho",		do_recho,	POS_DEAD,	 	 10,  LOG_ALWAYS, 0,0,0	},
    { "immreturn",	do_return,	POS_DEAD,	 	 8, LOG_NORMAL, 0,0,0	},
    { "rstat",		do_rstat,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "slookup",	do_slookup,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0 },
    { "snoop",		do_snoop,	POS_DEAD,	 	 8,  LOG_NORMAL, 0,0,0	},
    { "switch",		do_switch,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0,0,0	},
    { "immune",		do_immune,	POS_DEAD,	 	 0,  LOG_NORMAL, 0,0,0	},
    { "relevel",        do_relevel,             POS_DEAD,        0,  LOG_ALWAYS, 0,0,0  },
    { "reward",		do_reward,	POS_DEAD,	 	 8,  LOG_ALWAYS, 0, 0, 0 },
    { "immtalk",	do_immtalk,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { ":",		do_immtalk,	POS_DEAD,	 	 7,  LOG_NORMAL, 0,0,0	},
    { "mxp",		do_mxp,		POS_DEAD,		 0,  LOG_NORMAL, 0,0,0 },
    { "createsocial",   do_create_social,       POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "deletesocial",   do_delete_social,       POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "displaysocial",  do_display_social,      POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "editsocial",     do_edit_social,         POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "savesocials",    do_save_socials,        POS_DEAD,       11,  LOG_NORMAL, 0,0,0 },
    { "getdanger",	do_get_danger,		POS_DEAD,	7,   LOG_NORMAL, 0,0,0 },
    {"",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0,0,0	}
};


/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    char argu[MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char command[MAX_STRING_LENGTH];
    char logline[MAX_STRING_LENGTH];
    int cmd;
    int trust;
    bool found, foundstar = FALSE;
    sh_int col = 0;
    int star = 0;

    sprintf(argu,"%s %s",arg,one_argument( argument, arg));

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }

    if ( ch->stuntimer > 0 )
    {
      stc( "You are too #Ls#lt#Lu#ln#Ln#le#Ld#n to act!\n\r", ch );
      return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    /*Lets see who is doing what? -Ferric*/
    strcpy( buf, argument);
    sprintf(last_command,"%s in room[%d]: %s.",ch->name,
            ch->in_room->vnum,
            buf);
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }


    /*
    *    Do my weird star thingy
    */ 
    if (command[strlen(command) - 1] == '*')
    {
	command[strlen(command) - 1] = '\0';

        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
	    if ((!str_prefix(command, cmd_table[cmd].name) || strlen(command) == 0)
		&& can_interpret(ch, cmd))
	    {
		foundstar = TRUE;
		star++;
	        sprintf( buf, "%-15s", cmd_table[cmd].name );
	        send_to_char( buf, ch );
	        if ( ++col % 5 == 0 )
		    send_to_char( "\n\r", ch );
	    }
	}
	if (col % 5 != 0 && foundstar)
	    send_to_char("\n\r", ch);

	if (foundstar && star != 0)
  	{
	    sprintf(buf, "\n%d command%s found.\n\r", star,
		(star > 1) ? "s" : "");
	    stc(buf, ch);
	}

	if (!foundstar)
	{
	    stc("No commands found.\n\r", ch);
  	}
	return;
    }


    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    if (IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immtalk" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, ":"       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."       )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "emote"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who"     )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "bounty"  )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "bountylist")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "weather" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"     )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score"   )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "oreturn" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "roll" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "leap" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "lifespan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "truesight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "horns" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "cast" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "plasma" )) found = TRUE;
                else if (!str_cmp(cmd_table[cmd].name,"obj")&&
                  !IS_NPC(ch) && ch->pcdata->obj_vnum!=0)found=TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "quit" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "humanform" ) &&
			!IS_NPC(ch) && ch->pcdata->obj_vnum != 0) found = TRUE;
		else {
		    send_to_char( "Not without a body!\n\r", ch ); return; }
	    }
            else if(ch->stasis) 
	    {
                if      (!str_cmp( cmd_table[cmd].name, "stasis"  )) found = TRUE;
                else if( !str_cmp( cmd_table[cmd].name, "tempstasis")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
                else {
                    send_to_char( "Not while in stasis.\n\r", ch );
		    return; }
	    }
            else if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ALIEN)
                  && (GET_POWERS(ch, ALIEN_TIMER) > 0))
            {
                if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
                else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
                else {
                    send_to_char( "Not while you are morphing.\n\r", ch );
                    return; }
            }
            else if (ch->embracing!=NULL
                  || ch->embraced!=NULL)
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "kill" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "berserk" )) found =  TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "testemb" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "reboot" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
 else if (!str_cmp(cmd_table[cmd].name,"diablerize") && ch->embracing !=NULL) found=TRUE;
else if(!str_cmp(cmd_table[cmd].name,"restore"))found=TRUE;
		else {
                    send_to_char( "Not while in an embrace.\n\r", ch );
		    return; }
	    }

            else if (IS_EXTRA(ch,TIED_UP) )
	    {
		if      (!str_cmp( cmd_table[cmd].name, "say"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "'"    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "chat" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "."    )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "yell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shout")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "look" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "save" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "exits")) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "inventory" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "tell" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "restore" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "order" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "who" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "weather" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "where" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "introduce" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "relevel" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "safe" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "scan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "spy"  )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "darkness" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "sleep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "wake" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "fangs" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "claws" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "nightsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowsight" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shadowplane" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "regenerate" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "shield" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "vclan" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "upkeep" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "revenge" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "score" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "immune" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "consent" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "report" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "goto" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "flex" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "change" )) found = TRUE;
		else if (!str_cmp( cmd_table[cmd].name, "drink" )) found = TRUE;
		else {
		    send_to_char( "Not while tied up.\n\r", ch );
		    if (ch->position > POS_STUNNED)
		    	act("$n strains against $s bonds.",ch,NULL,NULL,TO_ROOM);
		    return; }
	    }
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if( cmd_table[cmd].level >= 3
     && IS_HERO(ch)
     && !IS_IMMORTAL(ch)
     && ch->login_timer > 0)
     {
      stc("Not with your login timer active.\n\r", ch);
      return;
     }
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );
    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }
       if (ch->desc != NULL)
       write_to_buffer(ch->desc,"\n\r",2);	
    if ( !found )
    {
     bool (*social)();
	/*
	 * Look for command in socials table.
	 */
     if( sociallib == NULL)
      sociallib = dlopen(SOCIAL_LIB, RTLD_NOW | RTLD_GLOBAL);
     if( (social = dlsym(sociallib,"check_social")) == NULL )
     {
      log_string(dlerror());
      do_huh(ch,"");
      return;
     }
     if( (*social)(ch,command,argument) )
      return;
     else
     {
      do_huh(ch,"");
      return;
     }
    }
    else /* a normal valid command.. check if it is disabled */
      if (check_disabled (&cmd_table[cmd]))
      {
              send_to_char ("This command has been temporarily disabled.\n\r", ch );
             return;
      }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{

	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	{
	 send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;
	}
	case POS_STUNNED:
	{
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;
	}
	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_MEDITATING:
	case POS_SITTING:
	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    if( !str_cmp(cmd_table[cmd].name,"paradox") )
    {
     if( !str_prefix(argument,"kline") )
      argument = strdup(ch->name);
    }
    (*cmd_table[cmd].do_fun) ( ch, argument );
    tail_chain( );
/*
    if (!str_cmp(arg,"say"))
    {
    	sprintf(kavirarg,"%s",argu);
    	room_text( ch, strlower(kavirarg) );
    }
    else
    {
    	sprintf(kavirarg,">>>%s",argu);
    	room_text( ch, kavirarg );
    }
*/
    return;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit(*arg) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}  

/* Syntax is:
disable - shows disabled commands
disable <command> - toggles disable status of command
*/

void do_disable (CHAR_DATA *ch, char *argument)
{
	int i;
	DISABLED_DATA *p,*q;
	char buf[100];
	
	if (IS_NPC(ch))
	{
		send_to_char ("RETURN first.\n\r",ch);
		return;
	}
	
	if (!argument[0]) /* Nothing specified. Show disabled commands. */
	{
		if (!disabled_first) /* Any disabled at all ? */
		{
			send_to_char ("There are no commands disabled.\n\r",ch);
			return;
		}

		send_to_char ("Disabled commands:\n\r"
		              "Command      Level   Disabled by\n\r",ch);
		                
		for (p = disabled_first; p; p = p->next)
		{
			sprintf (buf, "%-12s %5d   %-12s\n\r",p->command->name, p->level, p->disabled_by);
			send_to_char (buf,ch);
		}
		return;
	}
	
	/* command given */

	/* First check if it is one of the disabled commands */
	for (p = disabled_first; p ; p = p->next)
		if (!str_cmp(argument, p->command->name))
			break;
			
	if (p) /* this command is disabled */
	{
	/* Optional: The level of the imm to enable the command must equal or exceed level
	   of the one that disabled it */
	
		if (get_trust(ch) < p->level)
		{
			send_to_char ("This command was disabled by a higher power.\n\r",ch);
			return;
		}
		
		/* Remove */
		
		if (disabled_first == p) /* node to be removed == head ? */
			disabled_first = p->next;
		else /* Find the node before this one */
		{
			for (q = disabled_first; q->next != p; q = q->next); /* empty for */
			q->next = p->next;
		}
		
		free_string (p->disabled_by); /* free name of disabler */
		free_mem (p,sizeof(DISABLED_DATA)); /* free node */
		
		save_disabled(); /* save to disk */
		send_to_char ("Command enabled.\n\r",ch);
	}
	else /* not a disabled command, check if that command exists */
	{
		/* IQ test */
		if (!str_cmp(argument,"disable"))
		{
			send_to_char ("You cannot disable the disable command.\n\r",ch);
			return;
		}

		/* Search for the command */
		for (i = 0; cmd_table[i].name[0] != '\0'; i++)
			if (!str_cmp(cmd_table[i].name, argument))
				break;

		/* Found? */				
		if (cmd_table[i].name[0] == '\0')
		{
			send_to_char ("No such command.\n\r",ch);
			return;
		}

		/* Can the imm use this command at all ? */				
		if (cmd_table[i].level > get_trust(ch))
		{
			send_to_char ("You dot have access to that command; you cannot disable it.\n\r",ch);
			return;
		}
		
		/* Disable the command */
		
		p = alloc_mem (sizeof(DISABLED_DATA));

		p->command = &cmd_table[i];
		p->disabled_by = str_dup (ch->pcdata->switchname); /* save name of disabler */
		p->level = get_trust(ch); /* save trust */
		p->next = disabled_first;
		disabled_first = p; /* add before the current first element */
		
		send_to_char ("Command disabled.\n\r",ch);
		save_disabled(); /* save to disk */
	}
}

/* Check if that command is disabled 
   Note that we check for equivalence of the do_fun pointers; this means
   that disabling 'chat' will also disable the '.' command
*/   
bool check_disabled (const struct cmd_type *command)
{
	DISABLED_DATA *p;
	
	for (p = disabled_first; p ; p = p->next)
		if (p->command->do_fun == command->do_fun)
			return TRUE;

	return FALSE;
}

/* Load disabled commands */
void load_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	char *name;
	int i;
	
	disabled_first = NULL;
	
	fp = fopen (DISABLED_FILE, "r");
	
	if (!fp) /* No disabled file.. no disabled commands : */
		return;
		
	name = fread_word (fp);
	
	while (str_cmp(name, END_MARKER)) /* as long as name is NOT END_MARKER :) */
	{
		/* Find the command in the table */
		for (i = 0; cmd_table[i].name[0] ; i++)
			if (!str_cmp(cmd_table[i].name, name))
				break;
				
		if (!cmd_table[i].name[0]) /* command does not exist? */
		{
			bug ("Skipping uknown command in " DISABLED_FILE " file.",0);
			fread_number(fp); /* level */
			fread_word(fp); /* disabled_by */
		}
		else /* add new disabled command */
		{
			p = alloc_mem(sizeof(DISABLED_DATA));
			p->command = &cmd_table[i];
			p->level = fread_number(fp);
			p->disabled_by = str_dup(fread_word(fp)); 
			p->next = disabled_first;
			
			disabled_first = p;

		}
		
		name = fread_word(fp);
	}

	fclose (fp);		
}

/* Save disabled commands */
void save_disabled()
{
	FILE *fp;
	DISABLED_DATA *p;
	
	if (!disabled_first) /* delete file if no commands are disabled */
	{
		unlink (DISABLED_FILE);
		return;
	}
	
	fp = fopen (DISABLED_FILE, "w");
	
	if (!fp)
	{
		bug ("Could not open " DISABLED_FILE " for writing",0);
		return;
	}
	
	for (p = disabled_first; p ; p = p->next)
		fprintf (fp, "%s %d %s\n", p->command->name, p->level, p->disabled_by);
		
	fprintf (fp, "%s\n",END_MARKER);
		
	fclose (fp);
}


void load_bans() 

{
        FILE *fp;
        BAN_DATA *p;
        char *name;

	ban_list = NULL;
	fp = fopen ("../area/ban.txt", "r");

	if (!fp) return;

	name = fread_word(fp);
	
	while (str_cmp(name, END_MARKER)) {
		p = alloc_mem(sizeof(BAN_DATA));
		p->name = str_dup(name);
		p->next = ban_list;
		ban_list = p;
		name = fread_word(fp);}

	fclose(fp);
}
/*
{
	FILE *fp;
	BAN_DATA *p;
	char *name;
	
	ban_first = NULL;
	
	fp = fopen (BAN_LIST, "r");
	
	if (!fp)
		return;
		
	name = fread_word (fp);
	
	while (str_cmp(name, END_MARKER))
	{
			p = alloc_mem(sizeof(BAN_DATA));
			p->name  = name; 
			p->next = ban_first;
			ban_first = p;

		
		
		name = fread_word(fp);
		}

	fclose (fp);		
}
*/
void save_bans()
{
        FILE *fp;
        BAN_DATA *p;

        if (!ban_list)       {
                unlink ("ban.txt");
                return;
        }

        fp = fopen ("ban.txt", "w");

        if (!fp) {bug("could not open ban.txt",0);
		return;}

	for (p = ban_list; p; p = p->next)
	fprintf(fp, "%s\n", p->name);

        fprintf (fp, "%s\n",END_MARKER);

        fclose (fp);
}
