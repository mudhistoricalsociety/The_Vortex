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
#define POWER_TYPE 0
#define POWER_LEVEL 1
//#define ACK
#define DAM_ORIGINAL

/* Class Values */
typedef enum
{
 CLASS_NONE, CLASS_APOCRYPHIC, CLASS_ALIEN, CLASS_CYBORG, CLASS_MONK, CLASS_SHADOW,
 CLASS_WOLF, CLASS_JEDI, CLASS_VAMPIRE, CLASS_DRAGON, CLASS_DROW
} class_type;

#define IS_MXP(ch) (IS_SET((ch)->act,PLR_MXP))
#define IS_CLASS(ch, value) ((ch)->class == value)
#define GET_POWERS(ch, field) ((ch)->pcdata->powers[field])
#define INC_POWERS(ch, val) ((ch)->pcdata->powers[val]++)
#define DEC_POWERS(ch, val) ((ch)->pcdata->powers[val]--)
#define SET_BIT_POWERS(ch, value, bit) (SET_BIT((ch)->pcdata->powers[value], bit))
#define REM_BIT_POWERS(ch, value, bit) (REMOVE_BIT((ch)->pcdata->powers[value], bit))
#define FIGHT_TIMER( ch, time)                \
{                                             \
 ch->fight_timer += time;                     \
 if( ch->fight_timer > 30)                    \
  ch->fight_timer = 30;                       \
}

/* ability skills */
#define ASKILL(ch, field) ((ch)->pcdata->askills[field])
#define AS_TOTAL	0
#define AS_FINAL	2
#define AS_ATTACK	3
#define AS_DEFENSE	4
#define AS_TRUESIGHT	5
#define AS_SWORDSMAN	6
#define AS_STANCES	7
#define AS_SPELLS	8
#define AS_READAURA	9
#define AS_BITS		20
#define ASKILL_MAX_LEV 100

#define ABIT_TRUESIGHT	BIT_1
#define ABIT_READAURA   BIT_2

/*Drow port -Kline*/

typedef enum
{
 DROW_NONE, DROW_BITS, DROW_POINTS, DROW_MAGIC
} drow_basic_stuff;

#define DROW_WAR		BIT_1
#define DROW_MAG		BIT_2
#define DROW_CLE		BIT_3
#define DPOWER_DROWFIRE		BIT_4
#define DPOWER_DARKNESS		BIT_5
#define DPOWER_DROWSIGHT	BIT_6
#define DPOWER_ARMS		BIT_7
#define DPOWER_WEB		BIT_8
#define DPOWER_SPIDERFORM	BIT_10
#define DPOWER_DROWHATE		BIT_11
#define DPOWER_DROWSHIELD	BIT_12
#define DPOWER_LEVITATION	BIT_13
#define DPOWER_GAROTTE		BIT_14
#define DPOWER_DGAROTTE		BIT_15
#define DPOWER_DROWPOISON	BIT_16
#define DPOWER_EARTHSHATTER	BIT_17
#define DPOWER_SPEED		BIT_18
#define DPOWER_TOUGHSKIN	BIT_19
#define DPOWER_SCRY		BIT_20
#define DROW_HATE		BIT_21
#define DROW_SPIDERFORM		BIT_22
#define DROW_SHIELD		BIT_23
#define DROW_TOUGHSKIN		BIT_24

/*Blah blah Jedi via Kline
 *They look simple...But aren't!
 */
typedef enum
{
 JEDI_NONE, JEDI_STANCE, JEDI_ATTACK, JEDI_DEFENSE, JEDI_RANK, JEDI_LIGHT,
 JEDI_DARK, JEDI_NEUTRAL, JEDI_FORCE_POOL, JEDI_STANCE_COUNT, JEDI_ATTACK_COUNT,
 JEDI_DEFENSE_COUNT, JEDI_FORCE_HEAL, JEDI_FORCE_ABSORB, JEDI_FORCE_SHIELD,
 JEDI_FORCE_MIND_TRICK, JEDI_FORCE_SIGHT, JEDI_FORCE_SPEED, JEDI_FORCE_JUMP,
 JEDI_FORCE_PUSH, JEDI_FORCE_PULL, JEDI_FORCE_GRIP, JEDI_FORCE_RAGE,
 JEDI_FORCE_DRAIN, JEDI_FORCE_LIGHTNING, JEDI_CHAIN_LENGTH, JEDI_STANCE_STYLE,
 JEDI_BITS, JEDI_COMBO_OLD, JEDI_COMBO_NEW, JEDI_FORCE_POOL_MAX
} jedi_basic_stuff;

typedef enum
{
 JEDI_RANK_NONE, JEDI_RANK_APPRENTICE, JEDI_RANK_INITIATE, JEDI_RANK_GUARDIAN,
 JEDI_RANK_KNIGHT, JEDI_RANK_MASTER, JEDI_RANK_LORD
} jedi_rank_stuff;

typedef enum
{
 JEDI_STYLE_NONE, JEDI_STYLE_LIGHT, JEDI_STYLE_MEDIUM, JEDI_STYLE_HEAVY
} jedi_style_stuff;

typedef enum
{
 JEDI_COMBO_NONE, JEDI_COMBO_JOVER, JEDI_COMBO_JFLIP, JEDI_COMBO_JUPPER, JEDI_COMBO_JBACK,
 JEDI_COMBO_JSLICE, JEDI_COMBO_JSWIPE, JEDI_COMBO_JSLASH, JEDI_COMBO_JROLL
} jedi_combo_stuff;

typedef enum
{
 JEDI_COMBO_AFF_NONE, JEDI_COMBO_AFF_HASTE, JEDI_COMBO_AFF_SLOW, JEDI_COMBO_AFF_SHEAL,
 JEDI_COMBO_AFF_MHEAL, JEDI_COMBO_AFF_LHEAL, JEDI_COMBO_AFF_SDAM, JEDI_COMBO_AFF_MDAM,
 JEDI_COMBO_AFF_LDAM, JEDI_COMBO_AFF_TOUGHNESS, JEDI_COMBO_AFF_ANTI_SANCT,
 JEDI_COMBO_AFF_STUN, JEDI_COMBO_AFF_STOP_FIGHT, JEDI_COMBO_AFF_UNSTANCE,
 JEDI_COMBO_AFF_DISARM, JEDI_COMBO_AFF_UBER_JEDI, JEDI_COMBO_AFF_LEECH_HEALTH,
 JEDI_COMBO_AFF_LEECH_MANA, JEDI_COMBO_AFF_LEECH_MOVE, JEDI_COMBO_AFF_LEECH_ALL,
 JEDI_COMBO_AFF_EQUILIZER
} jedi_combo_affects_stuff;

#define JEDI_LIGHT_WS_MOD	1.5
#define JEDI_LIGHT_DAM_MOD	0.75
#define JEDI_HEAVY_WS_MOD	1.5
#define JEDI_HEAVY_DAM_MOD	1.25
#define JEDI_WS_MOD_SPEED	2
#define JEDI_WS_MOD_UBER	1.5
#define JEDI_DAM_MOD_UBER	1.15
#define JEDI_SKILL_CAP		10
#define JEDI_ADVANCE_COUNT	100
#define JEDI_WAIT		12
#define JEDI_MIN_DAM		500
#define JEDI_MAX_DAM		1100
#define JEDI_MIN_RAND		50
#define JEDI_MAX_RAND		150
#define JEDI_PEAK_MOD		2.5

#define JEDI_ABSORB	BIT_1
#define JEDI_SHIELD	BIT_2
#define JEDI_MIND_TRICK	BIT_3
#define JEDI_SIGHT	BIT_4
#define JEDI_SPEED	BIT_5
#define JEDI_RAGE	BIT_6
#define JEDI_UBER	BIT_7
#define JEDI_TOUGH	BIT_8
#define JEDI_UNRAGE	BIT_9

/* Here there be dragons */
typedef enum
{ DRAGON_NONE, DRAGON_MIGHT, DRAGON_TOUGH, DRAGON_SPEED, DRAGON_MAGIC,
  DRAGON_BTIME, DRAGON_STIME, DRAGON_FLAGS, DRAGON_AGE, DRAGON_POINTS,
  DRAGON_FORM, DRAGON_DAYS
} dragon_stuff;

#define POINT_STEP 200

#define DRAGON_WINGS     BIT_1
#define DRAGON_FIRE      BIT_2
#define DRAGON_ICE       BIT_3
#define DRAGON_ACID      BIT_4
#define DRAGON_LIGHTNING BIT_5
#define DRAGON_POISON    BIT_6
#define DRAGON_GRAB      BIT_7
#define DRAGON_FLY       BIT_8

#define DRAGON_FORM_DRAGON    1
#define DRAGON_FORM_HUMAN     2

#define DRAGON_TYPE_FIRE      1
#define DRAGON_TYPE_LIGHTNING 2
#define DRAGON_TYPE_ACID      3
#define DRAGON_TYPE_POISON    4
#define DRAGON_TYPE_ICE       5

/* vamps */
typedef enum
{
 VAMP_NONE, VAMP_BITS, DISC_OBTEN, DISC_PRESENCE, DISC_QUIETUS, DISC_THAUM,
 DISC_AUSPEX, DISC_DOMINATE, DISC_OBFUSCATE, DISC_PROTEAN, DISC_SERPENTIS,
 DISC_VICIS, DISC_DAIMOINON, DISC_ANIMALISM, DISC_CELERITY, DISC_POTENCE,
 DISC_FORTITUDE, VAMP_BLOOD, VAMP_FORM, VAMP_GEN, VAMP_AGE, VAMP_POINTS, VAMP_BEAST,
 VAMP_KILLS
} vamp_basic;
/* vamp bits */
#define BIT_CONCEAL     BIT_1
#define BIT_DISGUISE	BIT_2
#define BIT_SCALES	BIT_3
#define BIT_BEAST	BIT_4
#define BIT_GOLCANDA	BIT_5
#define BIT_DARK_GOLC	BIT_6
#define BIT_FLAMEHANDS	BIT_7
#define BIT_SHADE	BIT_8
#define BIT_HORNS	BIT_9
#define BIT_WINGS	BIT_10
#define BIT_EXO		BIT_11
#define BIT_TAIL	BIT_12
#define BIT_HEAD	BIT_13
#define BIT_BAAL	BIT_14
#define BIT_FLASHING	BIT_15
#define BIT_POTENCE	BIT_16
#define BIT_TIDE	BIT_17

#define BIT_COIL	BIT_19
#define BIT_INCONNU	BIT_20
#define BIT_ACID	BIT_21
/* vamp forms */
typedef enum
{
 FORM_NORMAL, FORM_SERPENT, FORM_ZULO, FORM_DRAGON, FORM_BAT, FORM_WOLF,
 FORM_OBJECT, FORM_MIST, FORM_EARTH, FORM_PLASMA
} vamp_forms;
/* vamp age */
typedef enum
{
 AGE_NONE, AGE_CAITIFF, AGE_ANCILLA, AGE_ELDER, AGE_METHUSELAH
} vamp_ages;

/* wolves */
#define DISC_BEAR	1
#define DISC_LYNX	2
#define DISC_BOAR	3
#define DISC_OWL	4
#define DISC_SPIDER	5
#define DISC_WOLF	16
#define DISC_HAWK	7
#define DISC_MANTIS	8
#define DISC_RAPTOR	9
#define DISC_LUNA	10
#define DISC_PAIN	11
#define DISC_CONG	12
#define WOLF_BITS	13
#define WOLF_FORM	14
#define WOLF_POINTS	15
#define FORM_HOMID	1
#define FORM_GLABRO	2
#define FORM_CRINOS	3
#define FORM_HISPO	4
#define FORM_LUPUS	5
/* wolf bits */
#define BIT_SCLAW	BIT_1
#define BIT_FCLAW	BIT_2
#define BIT_REND	BIT_3
#define BIT_TOUGH	BIT_4
#define BIT_JAW		BIT_5
#define BIT_SENSES	BIT_6
#define BIT_QUILLS	BIT_7
#define BIT_SLAM	BIT_8
#define BIT_HOMID	BIT_9
#define BIT_GLABRO	BIT_10
#define BIT_CRINOS	BIT_11
#define BIT_HISPO	BIT_12
#define BIT_LUPUS	BIT_13
#define BIT_CLAWS	BIT_14
#define BIT_FANGS	BIT_15
#define BIT_SHIELD	BIT_16
#define BIT_RAGED	BIT_17
#define BIT_VANISH	BIT_18
#define BIT_GONE	BIT_19
/* aliens */
#define ALIEN_SPEED  1
#define ALIEN_STR    2
#define ALIEN_SIZE   3
#define ALIEN_ARMOR  4
#define ALIEN_SENSE  5
#define ALIEN_BITS   6
#define ALIEN_PAIN   7
#define ALIEN_PRIMAL 8
#define ALIEN_DNA    9
#define ALIEN_CLAWS  10
#define ALIEN_TAIL   11
#define ALIEN_CLASS  12
#define ALIEN_TIMER  13
/* alien bits */ 
#define ALIEN_CLOAK  1
/* alien subclasses */
#define ALIEN_DRONE 0
#define ALIEN_RUN   1
#define ALIEN_WAR   2
#define ALIEN_PRAE  3
#define ALIEN_SCORP 4
#define ALIEN_GOR   5

/* shadow */
#define SHADOW_BACKSTAB	1
#define SHADOW_TRAP	2
#define SHADOW_RUNE	3
#define SHADOW_TRACK	4
#define SHADOW_COUNTER	5
#define SHADOW_BITS	6
#define SHADOW_LEV_BS	7
#define SHADOW_LEV_TRAP	8
#define SHADOW_LEV_RUNE	9
#define SHADOW_LEV_TRK	10
#define SHADOW_COST	250
#define SHADOW_LEV_GAIN	30

#define SHADOW_INVIS	1
#define SHADOW_FOLLOW	2
#define SHADOW_FOLLGO	4

/* monks */
typedef enum
{ MONK_NONE, MONK_BITS, MONK_TECH, MONK_CURR_CHI, MONK_MAX_CHI, MONK_CURR_FOC, MONK_MAX_FOC,
  MONK_COMBO, MONK_CHAIN, MONK_CUSTOM, MONK_BLOCK, MONK_CLASS
} monk_values;

typedef enum
{
 PATH_NONE, PATH_INFERNO, PATH_PEARL, PATH_TSUNAMI, PATH_GAIA, PATH_TEMPEST
} path_values;

/********************************************************
 *                     Bits for monks                   *
 ********************************************************/
#define MONK_CUST 1
#define MONK_STONEBODY 2
#define MONK_STONEHAND 4
#define MONK_MICHAEL   8
#define MONK_BEAR      16
#define MONK_BAHAMUT   32
#define MONK_FIDO      64
#define MONK_MIST      128
#define MONK_EFREET    256
#define MONK_WOLF      512

/********************************************************
*                     Monk techniques                   *
*********************************************************/
#define TECH_ELBOW       1
#define TECH_BACKFIST    2 
#define TECH_SWEEP       4 
#define TECH_REVERSE     8 
#define TECH_KNEETHRUST  16 
#define TECH_THRUSTKICK  32
#define TECH_ROUNDHOUSE  64
#define TECH_HEARTPUNCH  128
#define TECH_PALMSTRIKE  256
#define TECH_DENJIN      512
#define TECH_CUSTOM      1024
#define TECH_VOICE       2048
#define TECH_BLOCK       4096
#define TECH_CELEST      8192
#define TECH_STANCE      16384
#define TECH_UNARMED     32768
#define TECH_HEALTOUCH   65536
#define TECH_DEATHTOUCH  131072
#define TECH_HEATMETAL   262144
#define TECH_CLAIR       524288
#define TECH_CHI         1048576

#define MONK_MIN_DAM 350
#define MONK_MAX_DAM 700
#define MAX_CUSTOM 100
#define MAX_MONK_BLOCK 100
#define MAX_CHI 6 
#define MAX_FOCUS 150
#define MONK_SET(ch, value) (IS_SET(GET_POWERS((ch), MONK_BITS), value))
#define HAS_TECH(ch, value) (IS_SET(GET_POWERS((ch), MONK_TECH), value))
#define MONK_WAIT 6 /* basic waitstate */
#define CHI_COST 500000 /* cost in focus to get a point of chi */

typedef enum
{
 COMBO_NONE, COMBO1_STAGE1, COMBO1_STAGE2, COMBO2_STAGE1, COMBO2_STAGE2,
 COMBO3_STAGE1, COMBO3_STAGE2, COMBO4_6_STAGE1, COMBO4_6_STAGE2, COMBO5_STAGE1, 
 COMBO5_STAGE2, COMBO5_STAGE3, COMBO6_STAGE3, COMBO6_STAGE4, COMBO7_STAGE1, 
 COMBO7_STAGE2, COMBO8_STAGE1, COMBO8_STAGE2, COMBO8_STAGE3, COMBO9_STAGE2,
 COMBO9_STAGE3, COMBO9_STAGE4
} combo_values;

typedef enum
{
 CHAIN_NONE, CHAIN1_STAGE1, CHAIN1_STAGE2, CHAIN2_STAGE1, CHAIN2_STAGE2,
 CHAIN3_STAGE1, CHAIN3_STAGE2, CHAIN4_STAGE1, CHAIN4_STAGE2, CHAIN5_STAGE1,
 CHAIN5_STAGE2
} chain_value;

typedef enum
{
 COMBO_MOVE, COMBO_MANA, COMBO_STUN, COMBO_DAM1, COMBO_DAM2, COMBO_DAM3,
 COMBO_HEAL1, COMBO_HEAL2, COMBO_HEAL3, COMBO_CHAIN1, COMBO_CHAIN2, COMBO_CHAIN3,
 COMBO_CHAIN4, COMBO_CHAIN5
} combo_types;

#define SPC_NEWBIEPASS  1
#define	WOLF_POLYAFF	6

#define HEAD_NORMAL		      0
#define HEAD_WOLF		      1
#define HEAD_EAGLE		      2
#define HEAD_LION		      4
#define HEAD_SNAKE		      8

#define TAIL_NONE		      0
#define TAIL_POISONOUS		      1
#define TAIL_SCORPION		      2

#define INVIS_TIME		20
#define CHAT_TIME		21


/* paintaball defines */
#define PAINT_ON              0
#define PAINT_DEATHS          1
#define PAINT_KILLS           2

/* Powerblock defines */
#define SCHOOL_NONE      0
#define SCHOOL_FIRE      1
#define SCHOOL_WATER     2
#define SCHOOL_EARTH     3
#define SCHOOL_WIND      4
#define SCHOOL_HOLY      5
#define SCHOOL_EVIL      6
#define SCHOOL_TIME      7
#define SCHOOL_SPACE     8
#define SCHOOL_PLANT     9
#define SCHOOL_ANIMAL    10
#define SCHOOL_ARCANE    11
#define SCHOOL_LUCK      12 
#define SCHOOL_ORDER     13
#define SCHOOL_CHAOS     14
#define SCHOOL_LIFE      15
#define SCHOOL_DEATH     16
#define MAX_BLOCK 16

/* GEm stuff*/
#define GEM_UNCUT    1
#define GEM_SIMPLE   2
#define GEM_COMPLEX  4
#define GEM_MASTER   8
#define GEM_UNPOLISHED 16
#define GEM_POLISHED   32
#define GEM_CRACKED    64
#define GEM_FLAWED     128
#define GEM_HAZY       256
#define GEM_CLEAR      512
#define GEM_FLAWLESS   1024
#define GEM_DIAMOND    1
#define GEM_RUBY       2
#define GEM_EMERALD    3
#define GEM_SAPPHIRE   4
#define GEM_ONYX       5
#define GEM_TOURMALINE 6
#define GEM_JADE       7
#define GEM_QUARTZ     8
#define GEM_OPAL       9
#define MAX_GEM        10

/* new affect bits */
#define AFF2_AURA      1
#define AFF2_REGEN     2
#define AFF2_CONTAGION 4 
#define AFF2_SLOW      8
#define AFF2_MANASHIELD 16

/* area bits */
#define AFF_AREA_FLOOD 1

/* room bits */
#define AFF_ROOM_DEFILE    1
#define AFF_ROOM_MINED     2
#define AFF_ROOM_WHIRLWIND 4
#define AFF_ROOM_FIRETRAP  8
#define AFF_ROOM_ICED      16
#define AFF_ROOM_TEARGAS   32
#define AFF_ROOM_DART	   64
#define AFF_ROOM_POISON	   128
#define AFF_ROOM_EXPLOSION 256
#define AFF_ROOM_SLEEP	   512
#define AFF_ROOM_NEGATIVITY 1024
#define AFF_ROOM_DRAIN	   2048

/* ore type */
#define ORE_COPPER      1
#define ORE_IRON        2
#define ORE_TIN         3

/* metal types */
#define METAL_COPPER    1
#define METAL_BRONZE    2
#define METAL_IRON      3
#define METAL_STEEL     4

/* group types defs */
#define GROUP_1 1  /*fire/chaos*/
#define GROUP_2 2  /*earth/plant*/
#define GROUP_3 3  /*holy/order*/
#define GROUP_4 4  /*time/luck*/
#define GROUP_5 5  /*arcane/space*/
#define GROUP_6 6  /*life/water*/
#define GROUP_7 7  /*death/wind*/
#define GROUP_8 8  /*animal/evil*/

#define HAS_SOUND(ch) (IS_SET((ch)->act, PLR_MSP))

typedef enum
{
SND_NONE, SND_CHAR, SND_ROOM, SND_AREA, SND_WORLD
} sound_type;

