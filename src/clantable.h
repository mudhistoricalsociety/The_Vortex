/*
 * Initial version created by Dral  21 Aug 1999,
 * based on kingdom code from Incoherent Dreams.
 */
/********************************************************************
 * This code was modified by Listy (listy@godwars.com),             *
 * Nuitari (nuitari@godwars.com, and Adreno (adreno@godwars.com)    *
 * Use of this code is prohibited unless with written permission by *
 * the authors. This acknowledgement must not be removed from this  *
 * code.                                                            *
 * (c) 1999 Implementors of God Wars Rogue Edition                  *
 ********************************************************************/

#ifndef CLANTABLE_H
#define CLANTABLE_H

#include "merc.h"

// ---- config options
// don't use this atm, as the support code for it doesn't exist yet
#undef CHAR_CLANS_AS_NUMBER

#define CLANTABLE_FILENAME "../area/clantabl.dat"
#define MIN_CLAN_NAME_LENGTH 2
#define MAX_CLAN_NAME_LENGTH 16
//#define MAGE_CLAN_NAME "Conclave of Magi"

// ---- public functions
void load_clanpktable ();
void transfer_clanpk_leadership(CHAR_DATA *old_leader, CHAR_DATA *new_leader);

/* This doesn't handle non-avatar related kills,
 * mainly because they should never happen.
 */
void register_clanpk (CHAR_DATA *killer, CHAR_DATA *victim);
void register_arena_clanpk (CHAR_DATA *killer, CHAR_DATA *victim);
DECLARE_DO_FUN(do_found_clan);
DECLARE_DO_FUN(do_clantable);
DECLARE_DO_FUN(do_clanpk_leader);
DECLARE_DO_FUN(do_clanpk_coleader);
#endif
