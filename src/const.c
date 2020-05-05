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
#include <stdio.h>
#include <time.h>
#include "merc.h"
/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
    { -100, -100,   0,  0 },  /* 0  */
    { -75, -75,   3,  1 },  /* 1  */
    { -50, -50,   3,  2 },
    { -25, -25,  10,  3 },  /* 3  */
    {   0, 0,  25,  4 },
    {  25, 25,  55,  5 },  /* 5  */
    {  50, 50,  80,  6 },
    {  75, 75,  90,  7 },
    {  100,  100, 100,  8 },
    {  125,  125, 100,  9 },
    {  150,  150, 115, 10 }, /* 10  */
    {  175,  175, 115, 11 },
    {  200,  200, 140, 12 },
    {  225,  225, 140, 13 }, /* 13  */
    {  250,  250, 170, 14 },
    {  275,  275, 170, 15 }, /* 15  */
    {  300,  300, 195, 16 },
    {  325,  325, 220, 22 },
    {  325,  325, 250, 25 }, /* 18  */
    {  350,  350, 400, 30 },
    {  375,  375, 500, 35 }, /* 20  */
    {  400,  400, 600, 40 },
    {  425,  425, 700, 45 },
    {  450,  450, 800, 50 },
    {  475, 475, 900, 55 },
    {  500, 500, 999, 60 }  /* 25   */
};
const	struct	int_app_type	int_app		[26]		=
{
    {  1 },	/*  0 */
    {  1 },	/*  1 */
    {  1 },
    {  1 },	/*  3 */
    {  1 },
    {  1 },	/*  5 */
    {  1 },
    {  1 },
    {  1 },
    {  1 },
    {  1 },	/* 10 */
    {  1 },
    {  1 },
    {  2 },
    {  2 },
    {  2 },	/* 15 */
    {  2 },
    {  3 },
    {  3 },	/* 18 */
    {  3 },
    {  4 },	/* 20 */
    {  4 },
    {  4 },
    {  6 },
    {  6 },
    {  5 }	/* 25 */
};

const	struct	wis_app_type	wis_app		[26]		=

{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 2 },
    { 2 },	/* 10 */
    { 2 },
    { 2 },
    { 2 },
    { 2 },
    { 3 },	/* 15 */
    { 3 },
    { 4 },
    { 5 },	/* 18 */
    { 5 },
    { 5 },	/* 20 */
    { 6 },
    { 6 },
    { 6 },
    { 6 },
    { 7 }	/* 25 */
};

const	struct	dex_app_type	dex_app		[26]		=
{
    {   300 },   /* 0 */
    {   250 },   /* 1 */
    {   200 },
    {   150 },
    {   100 },
    {   50  },   /* 5 */
    {   0   },
    {   0   },
    {   0   },
    {  -50  },
    {  -50  },   /* 10 */
    {  -100 },
    {  -100 },
    {  -150 },
    {  -150 },
    {  -200 },   /* 15 */
    {  -200 },
    {  -250 },
    {  -250 },
    {  -300 },
    {  -300 },   /* 20 */
    {  -350 },
    {  -350 },
    {  -400 },
    {  -400 },
    {  -450}    /* 25 */
};

const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 }    /* 25 */
};


/*
 * Liquid properties.
 * Used in world.obj.
*/


const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
    { "water",			"clear",	{  0, 1, 10 }	},  /*  0 */
    { "beer",			"amber",	{  3, 2,  5 }	},
    { "wine",			"rose",		{  5, 2,  5 }	},
    { "ale",			"brown",	{  2, 2,  5 }	},
    { "dark ale",		"dark",		{  1, 2,  5 }	},
    { "whisky",			"golden",	{  6, 1,  4 }	},  /*  5 */
    { "lemonade",		"pink",		{  0, 1,  8 }	},
    { "firebreather",		"boiling",	{ 10, 0,  0 }	},
    { "local specialty",	"everclear",	{  3, 3,  3 }	},
    { "slime mold juice",	"green",	{  0, 4, -8 }	},
    { "milk",			"white",	{  0, 3,  6 }	},  /* 10 */
    { "tea",			"tan",		{  0, 1,  6 }	},
    { "coffee",			"black",	{  0, 1,  6 }	},
    { "blood",			"red",		{  0, 0,  5 }	},
    { "salt water",		"clear",	{  0, 1, -2 }	},
    { "cola",			"cherry",	{  0, 1,  5 }	}   /* 15 */
};

/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */

#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */
    {
	"reserved",		99,
	0,			TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			""
    },

    {
	"acid blast",		2,
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(70),	20,	12,
	"acid blast",		"!Acid Blast!"
    },
/*
    {
	"SatansBless",		12,
	spell_satansbless,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(205),	5,	12,
	"",			"You feel your soul return to you."
    },
*/
    {
	"armor",		1,
	spell_armor,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 1),	 5,	12,
	"",			"You feel less protected."
    },

    {
	"bless",		1,
	spell_bless,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT( 3),	 5,	12,
	"",			"You feel less righteous."
    },

    {
	"blindness",		1,
	spell_blindness,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	 5,	12,
	"",			"You can see again."
    },

    {
	"burning hands",	2,
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!"
    },

    {
	"call lightning",	2,
	spell_call_lightning,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!"
    },

    {
	"cause critical",	2,
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!"
    },

    {
	"cause light",		2,
	spell_cause_light,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!"
    },

    {
	"cause serious",	2,
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!"
    },

    {
	"change sex",		2,
	spell_change_sex,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(82),	15,	12,
	"",			"Your body feels familiar again."
    },

    {
	"charm person",		1,
	spell_charm_person,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 5,	12,
	"",			"You feel more self-confident."
    },

    {
	"chill touch",		2,
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold."
    },

    {
	"colour spray",		2,
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!"
    },

    {
	"continual light",	1,
	spell_continual_light,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!"
    },

    {
	"control weather",	2,
	spell_control_weather,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!"
    },

    {
	"create food",		1,
	spell_create_food,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!"
    },

    {
	"create spring",	1,
	spell_create_spring,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!"
    },

    {
	"create water",		2,
	spell_create_water,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!"
    },

    {
	"cure blindness",	1,
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!"
    },

    {
	"cure critical",	2,
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(15),	20,	12,
	"",			"!Cure Critical!"
    },

    {
	"cure light",		2,
	spell_cure_light,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(16),	10,	12,
	"",			"!Cure Light!"
    },

    {
	"cure poison",		1,
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(43),	 5,	12,
	"",			"!Cure Poison!"
    },

    {
	"cure serious",		2,
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!Cure Serious!"
    },

    {
	"curse",		1,
	spell_curse,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off."
    },

   {
	"detect evil",		2,
	spell_detect_evil,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears."
    },

    {
	"detect hidden",	1,
	spell_detect_hidden,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(44),	 5,	12,
	"",			"You feel less aware of your suroundings."
    },

    {
	"detect invis",		1,
	spell_detect_invis,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects."
    },

    {
	"detect magic",		2,
	spell_detect_magic,	TAR_CHAR_SELF,	 POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"You no longer see magical auras."
    }, 

    {
	"detect poison",	2,
	spell_detect_poison,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!"
    },

    {
	"dispel evil",		2,
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!"
    },
    
    {
	"dispel magic",		1,
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!"
    },

    {
	"earthquake",		2,
	spell_earthquake,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!"
    },
    
    {
	"enchant weapon",	1,
	spell_enchant_weapon,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!"
    },

    {
	"energy drain",		1,
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(25),	35,	12,
	"energy drain",		"!Energy Drain!"
    },

    {
	"faerie fire",		2,
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away."
    },

    {
	"faerie fog",		2,
	spell_faerie_fog,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"!Faerie Fog!"
    },

    {
	"fireball",		1,
	spell_fireball,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!"
    },

    {
	"flamestrike",		2,
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!"
    },

    {
	"fly",			1,
	spell_fly,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(56),	10,	18,
	"",			"You slowly float to the ground."
    },

    {
	"haste",		1,
	spell_haste,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(83),	50,	12,
	"",			"Your actions return to normal."
    },

    {
	"giant strength",	1,
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(39),	20,	12,
	"",			"You feel weaker."
    },

    {
	"harm",			1,
	spell_harm,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!"
    },

    {
	"heal",			1,
	spell_heal,		TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!"
    },

    {
	"identify",		1,
	spell_identify,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(53),	12,	24,
	"",			"!Identify!"
    },

    {
	"infravision",		1,
	spell_infravision,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
	"",			"You no longer see in the dark."
    },

    {
	"invis",		1,
	spell_invis,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible."
    },

    {
	"know alignment",	2,
	spell_know_alignment,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!"
    },

    {
	"lightning bolt",	2,
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(30),	15,	12,
	"lightning bolt",	"!Lightning Bolt!"
    },

    {
	"locate object",	1,
	spell_locate_object,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!"
    },

    {
	"magic missile",	2,
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(32),	15,	4,
	"magic missile",	"!Magic Missile!"
    },
/*
    {
	"mass invis",		1,
	spell_mass_invis,	TAR_IGNORE,		POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"!Mass Invis!"
    },
*/
    {
	"pass door",		1,
	spell_pass_door,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again."
    },

    {
	"poison",		1,
	spell_poison,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick."
    },

    {
	"protection",		1,
	spell_protection,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(34),	 5,	12,
	"",			"You feel less protected."
    },

    {
	"refresh",		2,
	spell_refresh,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(81),	12,	18,
	"refresh",		"!Refresh!"
    },

    {
	"remove curse",		2,
	spell_remove_curse,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(35),	 5,	12,
	"",			"!Remove Curse!"
    },

    {
	"sanctuary",		1,
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(36),	75,	12,
	"",			"The white aura around your body fades."
    },

    {
	"shield",		1,
	spell_shield,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(67),	12,	18,
	"",			"Your force shield shimmers then fades away."
    },

    {
	"shocking grasp",	2,
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(37),	15,	12,
	"shocking grasp",	"!Shocking Grasp!"
    },

    {
	"sleep",		2,
	spell_sleep,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired."
    },

    {
	"stone skin",		1,
	spell_stone_skin,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(66),	12,	18,
	"",			"Your skin feels soft again."
    },

    {
	"summon",		2,
	spell_summon,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(40),	50,	12,
	"",			"!Summon!"
    },

    {
	"teleport",		2,
	spell_teleport,		TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,	 		SLOT( 2),	35,	12,
	"",			"!Teleport!"
    },
/*
    {
	"ventriloquate",	2,
	spell_ventriloquate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!"
    },
*/
    {
	"weaken",		2,
	spell_weaken,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(68),	20,	12,
	"spell",		"You feel stronger."
    },

    {
	"word of recall",	9,
	spell_word_of_recall,	TAR_CHAR_SELF,		POS_RESTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!"
    },

/*
 * Dragon breath
 */

    {
	"acid breath",		1,
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(200),	 50,	 12,
	"blast of acid",	"!Acid Breath!"
    },

    {
	"fire breath",		1,
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(201),	 50,	 12,
	"blast of flame",	"The smoke leaves your eyes."
    },

    {
	"frost breath",		1,
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(202),	 50,	 12,
	"blast of frost",	"!Frost Breath!"
    },

    {
	"gas breath",		1,
	spell_gas_breath,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(203),	 50,	 12,
	"blast of gas",		"!Gas Breath!"
    },

    {
	"lightning breath",	1,
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(204),	 50,	 12,
	"blast of lightning",	"!Lightning Breath!"
    },

    {
        "Godbless",             12,
        spell_godbless,         TAR_CHAR_DEFENSIVE,     POS_STANDING,
        NULL,                   SLOT(205),      5,      12,
        "",                     "You feel God's blessing leave you."
    },

/*
 * Fighter and thief skills.
 */

    {
	"backstab",		1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_backstab,		SLOT( 0),	 0,	24,
	"backstab",		"!Backstab!"
    },

   {
	"circle",			99,
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_circle,		SLOT(0),	0,	24,
	"circle",		"!circle!"
    }, 
    {   "counter attack",                99,
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_cattack,              SLOT(0),        0,     24,
        "counter attack",      "!counter attack!"  
    },

    { 
        "fwave",                99,
        spell_null,             TAR_IGNORE,             POS_FIGHTING,
        &gsn_fwave,             SLOT(0),         0,     24,
        "#1Force #2Wave#n",     "!force wave!"  
    },

    {
	"garotte",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_garotte,		SLOT( 0),	0,	24,
	"garotte",		"!Garotte!"
    },
    
    { 
        "hakunetsu",            99,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_hakun,             SLOT( 0),       0,      24,
        "backstab",             "!Backstab!" 
    },

     {
	"spiderform",		99,
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_spiderform,	SLOT(0),	0,	0,
	"spidery arm",		"!Spiderform!"
    },

    {
	"disarm",		1,
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_disarm,		SLOT( 0),	 0,	24,
	"",			"!Disarm!"
    },

    {
	"hide",			1,
	spell_null,		TAR_IGNORE,		POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	12,
	"",			"!Hide!"
    },

    {
	"hurl",			1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_hurl,		SLOT( 0),	 0,	24,
	"",			"!Hurl!"
    },

    {
	"kick",			1,
	spell_null,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_kick,		SLOT( 0),	 0,	 8,
	"kick",			"!Kick!"
    },

    {
	"peek",			1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!"
    },

    {
	"pick lock",		1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!"
    },

    {
	"rescue",		1,
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_rescue,		SLOT( 0),	 0,	12,
	"",			"!Rescue!"
    },

    {
	"sneak",		1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"Your footsteps are no longer so quiet."
    },

    {
	"steal",		1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	24,
	"",			"!Steal!"
    },

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */

    {
	"general purpose",	7,
	spell_general_purpose,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(501),	0,	12,
	"general purpose ammo",	"!General Purpose Ammo!"
    },

    {
	"high explosive",	7,
	spell_high_explosive,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(502),	0,	12,
	"high explosive ammo",	"!High Explosive Ammo!"
    },

/*
 * Spells added by KaVir.
 */

    {
	"guardian",		12,
	spell_guardian,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(600),	100,	12,
	"",			"!Guardian!"
    },

    {
	"soulblade",		1,
	spell_soulblade,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(601),	100,	12,
	"",			"!Soulblade!"
    },

    {
	"mana",			1,
	spell_mana,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(602),	 0,	12,
	"",			"!Mana!"
    },

    {
	"frenzy",		1,
	spell_frenzy,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(603),	 20,	12,
	"",			"Your bloodlust subsides."
    },

    {
	"darkblessing",		1,
	spell_darkblessing,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(604),	 20,	12,
	"",			"You feel less wicked."
    },

    {
	"portal",		1,
	spell_portal,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(604),	 50,	12,
	"",			"!Portal!"
    },

    {
	"energyflux",		2,
	spell_energyflux,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(605),	 0,	12,
	"",			"!EnergyFlux!"
    },

    {
	"voodoo",		1,
	spell_voodoo,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(606),	 100,	12,
	"",			"!Voodoo!"
    },

    {
	"transport",		1,
	spell_transport,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(607),	12,	24,
	"",			"!Transport!"
    },

    {
	"regenerate",		1,
	spell_regenerate,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(608),	100,	12,
	"",			"!Regenerate!"
    },

    {
	"clot",			1,
	spell_clot,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(609),	50,	12,
	"",			"!Clot!"
    },

    {
	"mend",			1,
	spell_mend,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(610),	50,	12,
	"",			"!Mend!"
    },

    {
	"punch",		1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_punch,		SLOT( 0),	 0,	10,
	"punch",		"!Punch!"
    },

    {
	"elbow",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_elbow,		SLOT( 0),	 0,	24,
	"elbow",		"!Elbow!"
    },

    {
	"headbutt",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_headbutt,		SLOT( 0),	 0,	24,
	"headbutt",		"!Headbutt!"
    },

    {
        "shiroken",             99,       
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_shiroken,          SLOT( 0),        0,     24,
        "shiroken",             "!Shiroken!"
    },
    
    {
        "claws",              99,         
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_claws,          SLOT(0),         0,     24,
        "claws",             "!Claws!"
    },
    {
       "tentacles",            99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_tentacle,        SLOT(0),          0,     24,
       "tentacles",            "!Tentacles!"
    },
    {
       "shards",            99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_shards,        SLOT(0),          0,     24,
       "shards",               "!Shards!"
    },
    {
       "venom",            99,
        spell_null,            TAR_IGNORE,             POS_STANDING,
        &gsn_venom,        SLOT(0),          0,     24,
       "venom",            "!Venom!"
    },

    {
	"blinky",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_blinky,		SLOT( 0),	0,	24,
	"surprise attack",	"!BLINKY!"
    },

    {
	"inferno",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_inferno,		SLOT( 0),	0,	24,
	"blazing flames",	"!Ooo Inferno!"
    },

    {
	"fangs",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_fangs,		SLOT( 0),	0,	24,
	"fangs",		"!Fangs!"
    },

    {
	"buffet",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_buffet,		SLOT( 0),	0,	24,
	"wing buffet",		"!Buffet!"
    },

    {
	"sweep",		99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_sweep,		SLOT( 0),	 0,	24,
	"tail sweep",		"!Sweep!"
    },

    {
       "rfangs",                99,
        spell_null,             TAR_IGNORE,              POS_STANDING,
        &gsn_rfangs,            SLOT(0),          0,    24, 
        "razor fangs",           "!Rfangs!"
    },

    {
	"knee",			99,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_knee,		SLOT( 0),	 0,	24,
	"knee",			"!Knee!"
    },

    {
        "firebolt",             99,
         spell_null,            TAR_IGNORE,             POS_STANDING,
         &gsn_firebolt,         SLOT(0),          0, 24,
         "fire bolt",           "!Firebolt"
    },
    {
       "strike",                99,
       spell_null,              TAR_IGNORE,             POS_STANDING,
       &gsn_strike,             SLOT(0),          0, 24,
       "lightning strike",      "!Strike"
    },
    {
	"quest",		2,
	spell_quest,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(611),	 500,	12,
	"",			"!Quest!"
    },

    {
	"minor creation",	1,
	spell_minor_creation,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(612),	 500,	12,
	"",			"!MinorCreation!"
    },

    {
	"brew",			1,
	spell_brew,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(613),	 100,	12,
	"",			"!Brew!"
    },

    {
	"scribe",		1,
	spell_scribe,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(614),	 100,	12,
	"",			"!Scribe!"
    },

    {
	"carve",		1,
	spell_carve,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(615),	 200,	12,
	"",			"!Carve!"
    },

    {
	"engrave",		1,
	spell_engrave,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(616),	 300,	12,
	"",			"!Engrave!"
    },

    {
	"bake",			1,
	spell_bake,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(617),	 100,	12,
	"",			"!Bake!"
    },

    {
	"mount",		2,
	spell_mount,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,			SLOT(618),	100,	12,
	"",			"!Mount!"
    },

    {
	"berserk",		2,
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_berserk,		SLOT( 0),	 0,	24,
	"",			"!Berserk!"
    },
/*
    {
        "tendrils",                  99,
        spell_tendrils,              TAR_CHAR_OFFENSIVE,     POS_STANDING,
        &gsn_tendrils,               SLOT( 0),       100,   12,
        "",                     "The tendrils surrounding you fade away."
    },
*/
    {
	"scan",			1,
	spell_scan,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(619),	6,	24,
	"",			"!Scan!"
    },

    {
	"repair",		2,
	spell_repair,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(620),	100,	24,
	"",			"!Repair!"
    },

    {
	"spellproof",		2,
	spell_spellproof,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(621),	50,	12,
	"",			"!Spellproof!"
    },

    {
	"preserve",		3,
	spell_preserve,		TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(550),	12,	24,
	"",			"!Preserve!"
    },

    {
	"track",		1,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_track,		SLOT( 0),	 0,	0,
	"",			"!Track!"
    },

    {
	"chaos blast",		99,
	spell_chaos_blast,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(559),	20,	12,
	"chaos blast",		"!Chaos Blast!"
    },

    {
         "enchant armor",       1,
         spell_enchant_armor,   TAR_OBJ_INV,            POS_STANDING,
         NULL,                  SLOT(50),               100,   24,
         "",                    "!Enchant Armor!"
    }, 

    {
	"resistance",		1,
	spell_resistance,	TAR_OBJ_INV,		POS_STANDING,
	NULL,			SLOT(560),	50,	12,
	"",			"!Resistance!"
    },

    {
	"web",			99,
	spell_web,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_web,		SLOT(561),	 100,	12,
	"",			"The web surrounding you breaks away."
    },

    {
	"improve",		1,
	spell_improve,		TAR_OBJ_INV,		POS_STANDING,
	NULL,					SLOT(566),	1500,	12,
	"",			"!Improve!"
    },

    {
	 "spew",                   13,
      spell_spew,               TAR_CHAR_OFFENSIVE,        POS_STANDING,     
      &gsn_spew,                    SLOT(0),         10,     12,
      "spew of blood",         "!Spew!"
    },

    {
	"infirmity",		13,
	spell_infirmity,        	TAR_IGNORE,	POS_STANDING,
      NULL,					SLOT( 0),	 5,	12,
	"infirmity",			"You feel stronger."
    },

    {
	"shred",		13,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_shred,		SLOT( 0),	 0,	24,
	"shredding assault",		"!Shred!"
    },

    {
	"cheapshot",		13,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_cheapshot,		SLOT( 0),	 0,	24,
	"cheapshot",		"!cheapshot!"
    },

    {
	"quills",		13,
	spell_null,		TAR_IGNORE,		POS_STANDING,
	&gsn_quills,		SLOT( 0),	 0,	24,
	"razor quills",		"!quills!"
    },

    {
        "swordstrike",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_swordstrike,            SLOT( 0),        0,     24,   
        "swordstrike",         "!swordstrike!"
    },

    {
        "venom tongue",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_venomtong,            SLOT( 0),        0,     24,
        "venom tongue",         "!VTong!"
    },  

    {
        "spiked tail",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_spiketail,            SLOT( 0),        0,     24,
        "spiked tail",         "!stail!"
    },  

    {
        "bad breath",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_badbreath,            SLOT( 0),        0,     24,
        "bad breath",         "!BBreath!"
    },  

    {
        "magma burst",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_magma,            SLOT( 0),        0,     24,
        "magma burst",         "!Magma!"
    },     

    {
        "ice shards",               13,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_shards,            SLOT( 0),        0,     24,
        "ice shards",         "!IShards!"
    }, 
    
    {
    	"drowfire",		    99,
     	spell_drowfire,		TAR_CHAR_OFFENSIVE,   POS_STANDING,
    	&gsn_drowfire,		SLOT(570),	100,	12,
      	"",			"The drowfire fades away."
    },

    {
        "dispel all",           1,
         spell_dispel_all,      TAR_IGNORE,     POS_FIGHTING,
         NULL,                  SLOT(571),     500,    12,
        "",                     "Dispel All!"
    },

    {
        "arcane magic",         99,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(572),       0,     12,
        "",                     "The arcane forces no longer affect you."
    },
    {
        "aura",                 1,
         spell_aura,            TAR_CHAR_DEFENSIVE,     POS_STANDING,
         NULL,                  SLOT(573),       250,    12,
         "",                    "Your life giving aura disipates."
    },
    {
        "regeneration",         1,
         spell_regeneration,    TAR_CHAR_DEFENSIVE,     POS_STANDING,
         NULL,                  SLOT(574),       500,    12,
         "",                    "You stop regenerating."
    },
    {
        "contagion",            1,
         spell_contagion,       TAR_CHAR_OFFENSIVE,     POS_STANDING,
         NULL,                  SLOT(575),      1000,    12,
         "",                    "The contagion lifts from your body."
    },
    {
        "slow",                1,
        spell_null,            TAR_CHAR_OFFENSIVE,    POS_STANDING,
        NULL,                   SLOT(576),     500,   12,
        "",                    "Your actions speed up to normal."
    },
    {  
        "flood",               99,
         spell_null,           TAR_IGNORE,           POS_STANDING,
        NULL,                  SLOT(577),     500,     12,
        "",                    "The flood waters recede."
    },
    {
        "defile",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(578),     500,     12,
        "",                    "The evil has dispersed."
    },
    {
       "mined",                99,
       spell_null,             TAR_IGNORE,             POS_STANDING,
       NULL,                   SLOT(579),      500,           12,
        "",                    "The earth is renewed."
    },
    {
        "prepare",              1,
        spell_prepare,          TAR_OBJ_INV,           POS_STANDING, 
        NULL,                    SLOT(580),             1500, 12,
        "",                      "!Prepare!"
    },
/*
    {
        "scry",                1,
        spell_scry,           TAR_IGNORE,             POS_STANDING,
        NULL,                   SLOT(581),       2500,    12,
        "",                     "!SCRY!"
    },
*/
    {
	"awaken",	       3,
	spell_awaken,         TAR_IGNORE,	      POS_STANDING,
	NULL,			SLOT(595),	 0,    0,
	"",			"!AWAKEN!"
    },
    {
       "upgrade",           1,
       spell_upgrade,        TAR_OBJ_INV,  POS_STANDING,
       NULL, SLOT(582), 1500, 12,
       "", "!upgrade!"
    },
    {
      "whirlwind", 99,
      spell_null, TAR_IGNORE,POS_STANDING,
      NULL, SLOT(583),500, 12,
      "", "The whirling wind dissipates."
    },
    {
        "firetrap",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(584),     500,     12,
        "",                    "A firetrap has fallen apart."
    }, 
    {
        "iced",                99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(585),     500,     12,
        "",                    "The ice on the floor has melted."
    },
    {
        "thornspray",             1,
        spell_thornspray,         TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        NULL,                   SLOT(586),       15,     12,
        "thornspray",             "!Thornspray!"
    },      
    {
	"paradox",	 	    10,
     	spell_null,		TAR_CHAR_SELF,	POS_STANDING,
    	&gsn_paradox,		SLOT( 90),	 5,	12,
        NULL,			"Your paradox fades."
    },
    { 
        "teargas",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(587),     500,     12,
        "",                    "The teargas has dissipated."
    },
    {
        "dart_trap",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(588),     500,     12,
        "",                    "A dart trap has fallen to pieces."
    },
    {
        "poison_trap",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(589),     500,     12,
        "",                    "A poison trap has fallen to pieces."
    },
    {
        "explosion_trap",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(590),     500,     12,
        "",                    "An explosion trap has fallen to pieces."
    },
    {
        "sleep_trap",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(591),     500,     12,
        "",                    "A sleep trap has fallen to pieces."
    },
    {
        "negativity_rune",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(592),     500,     12,
        "",                    "The negative energies in the room dissipate."
    },
    {
        "drain_rune",              99,
        spell_null,            TAR_IGNORE,         POS_STANDING,
        NULL,                  SLOT(593),     500,     12,
        "",                    "The energy vortex in the room vanishes."
    },
    {
        "wear location",            2,
        spell_wear_loc,         TAR_OBJ_INV,            POS_STANDING,
        NULL,                   SLOT(594),       12,     24,
        "",                     "!Wear location!"
    },
    { 
        "horns",                99,
        spell_null,             TAR_IGNORE,             POS_STANDING,
        &gsn_horns,             SLOT( 0),        0,     24,
        "horns",           "!Horns!"
    },
    {
        "manashield",                1,
        spell_manashield,            TAR_CHAR_SELF,     POS_STANDING,
        &gsn_manashield,                   SLOT( 2),        5,     12,
        "",                     "Your shield of mana flickers and dies."
    }
};

const   struct  ansi_type       ansi_table      [MAX_ANSI]              =
{
   { "gray",            "\033[0;37m", 	 0,	'8',	7 },
   { "red",             "\033[0;31m",	 1,	'r',	7 },
   { "green",           "\033[0;32m",	 2,	'g',	7 },
   { "brown",           "\033[0;33m",	 3,	'o',	7 },
   { "blue",            "\033[0;34m",	 4,	'l',	7 },
   { "magenta",         "\033[0;35m",	 5,	'p',	7 },
   { "cyan",            "\033[0;36m",	 6,	'c',	7 },
   { "black",           "\033[0;30m",	 7,	'k',	7 }, /* was 0;33 */
   { "yellow",          "\033[1;33m",	 8,	'y',	7 },
   { "white",           "\033[1;37m",    9,	'7',	7 },
   { "normal",          "\033[0;0m\033[0;40m",  'n',	'N',	13 },
   { "purple",		"\033[1;35m",   11,	'5',	7 },
   { "dark_grey",	"\033[1;30m",   12,	'0',	7 },
   { "light_blue",	"\033[1;34m",   13,	'4',	7 },
   { "light_green",	"\033[1;32m",   14,	'2',	7 },
   { "light_cyan",	"\033[1;36m",   15,	'6',	7 },
   { "light_red",	"\033[1;31m",   16,	'1',	7 },
   { "bold",		"\033[1m",    17,	'x',	4 },
   { "flashing",	"\033[5m",    18,	'b',	4 },
   { "inverse",		"\033[7m",    19,	'i',	4 },
   { "back_red",             "\033[0;41m",	 20,	'a',	7 },
   { "back_green",           "\033[0;42m",	 21,	's',	7 },
   { "back_yellow",           "\033[0;43m",	 22,	'd',	7 },
   { "back_blue",            "\033[0;44m",	 23,	'f',	7 },
   { "back_magenta",         "\033[0;45m",	 24,	'g',	7 },
   { "back_cyan",            "\033[0;46m",	 25,	'h',	7 },
   { "back_black",           "\033[0;40m",	 26,	'j',	7 },
   { "back_white",           "\033[1;47m",    27,	'l',	7 },
};

