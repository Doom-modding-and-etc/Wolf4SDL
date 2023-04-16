/* Nuked OPL3
 * Copyright (C) 2013-2020 Nuke.YKT
 *
 * This file is part of Nuked OPL3.
 *
 * Nuked OPL3 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 2.1
 * of the License, or (at your option) any later version.
 *
 * Nuked OPL3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Nuked OPL3. If not, see <https://www.gnu.org/licenses/>.

 *  Nuked OPL3 emulator.
 *  Thanks:
 *      MAME Development Team(Jarek Burczynski, Tatsuyuki Satoh):
 *          Feedback and Rhythm part calculation information.
 *      forums.submarine.org.uk(carbon14, opl3):
 *          Tremolo and phase generator calculation information.
 *      OPLx decapsulated(Matthew Gambrell, Olli Niemitalo):
 *          OPL2 ROMs.
 *      siliconpr0n.org(John McMaster, digshadow):
 *          YMF262 and VRC VII decaps and die shots.
 *
 * version: 1.8
 */

#ifndef OPL_OPL3_H
#define OPL_OPL3_H

#if defined(OPL_SHARED_LIB) && defined(_WIN32)

#ifdef OPL_SHARED_LIB_SRC
#define OPL_API __declspec(dllexport)
#else
#define OPL_API __declspec(dllimport)
#endif

#else

#define OPL_API

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OPL_ENABLE_STEREOEXT
#define OPL_ENABLE_STEREOEXT 0
#endif

#define OPL_WRITEBUF_SIZE   1024
#define OPL_WRITEBUF_DELAY  2

#ifdef USE_HEADER_TYPE
#include <inttypes.h>
#endif

typedef struct _opl3_slot opl3_slot;
typedef struct _opl3_channel opl3_channel;
typedef struct _opl3_chip opl3_chip;


#ifdef OLD_MSVC
typedef unsigned long long uint64_t;
#else
#ifndef DEVCPP
typedef unsigned long long uint64_t;
#endif
#endif

struct _opl3_slot {
    opl3_channel *channel;
    opl3_chip *chip;
    signed short out;
    signed short fbmod;
    signed short *mod;
    signed short prout;
    unsigned short eg_rout;
    unsigned short eg_out;
    unsigned char eg_inc;
    unsigned char eg_gen;
    unsigned char eg_rate;
    unsigned char eg_ksl;
    unsigned char *trem;
    unsigned char reg_vib;
    unsigned char reg_type;
    unsigned char reg_ksr;
    unsigned char reg_mult;
    unsigned char reg_ksl;
    unsigned char reg_tl;
    unsigned char reg_ar;
    unsigned char reg_dr;
    unsigned char reg_sl;
    unsigned char reg_rr;
    unsigned char reg_wf;
    unsigned char key;
    unsigned int pg_reset;
    unsigned int pg_phase;
    unsigned short pg_phase_out;
    unsigned char slot_num;
};

struct _opl3_channel {
    opl3_slot *slots[2];
    opl3_channel *pair;
    opl3_chip *chip;
    signed short *out[4];

#if OPL_ENABLE_STEREOEXT
    signed int leftpan;
    signed int rightpan;
#endif

    unsigned char chtype;
    unsigned short f_num;
    unsigned char block;
    unsigned char fb;
    unsigned char con;
    unsigned char alg;
    unsigned char ksv;
    unsigned short cha, chb;
    unsigned short chc, chd;
    unsigned char ch_num;
};

typedef struct _opl3_writebuf {
    uint64_t time;
    unsigned short reg;
    unsigned char data;
} opl3_writebuf;

struct _opl3_chip {
    opl3_channel channel[18];
    opl3_slot slot[36];
    unsigned short timer;
    uint64_t eg_timer;
    unsigned char eg_timerrem;
    unsigned char eg_state;
    unsigned char eg_add;
    unsigned char newm;
    unsigned char nts;
    unsigned char rhy;
    unsigned char vibpos;
    unsigned char vibshift;
    unsigned char tremolo;
    unsigned char tremolopos;
    unsigned char tremoloshift;
    unsigned int noise;
    signed short zeromod;
    signed int mixbuff[4];
    unsigned char rm_hh_bit2;
    unsigned char rm_hh_bit3;
    unsigned char rm_hh_bit7;
    unsigned char rm_hh_bit8;
    unsigned char rm_tc_bit3;
    unsigned char rm_tc_bit5;

#if OPL_ENABLE_STEREOEXT
    unsigned char stereoext;
#endif

    /* OPL3L */
    signed int rateratio;
    signed int samplecnt;
    signed short oldsamples[4];
    signed short samples[4];

    uint64_t writebuf_samplecnt;
    unsigned int writebuf_cur;
    unsigned int writebuf_last;
    uint64_t writebuf_lasttime;
    opl3_writebuf writebuf[OPL_WRITEBUF_SIZE];
};

OPL_API void OPL3_Generate(opl3_chip *chip, signed short *buf);
OPL_API void OPL3_GenerateResampled(opl3_chip *chip, signed short *buf);
OPL_API void OPL3_Reset(opl3_chip *chip, unsigned int samplerate);
OPL_API void OPL3_WriteReg(opl3_chip *chip, unsigned short reg, unsigned char v);
OPL_API void OPL3_WriteRegBuffered(opl3_chip *chip, unsigned short reg, unsigned char v);
OPL_API void OPL3_WriteRegDelayed(opl3_chip* chip, unsigned short reg, unsigned char v, uint64_t delay);
OPL_API void OPL3_GenerateStream(opl3_chip *chip, signed short *sndptr, size_t numsamples);
OPL_API void OPL3_Generate4Ch(opl3_chip *chip, signed short *buf4);
OPL_API void OPL3_Generate4ChResampled(opl3_chip *chip, signed short *buf4);
OPL_API void OPL3_Generate4ChStream(opl3_chip *chip, signed short *sndptr1, signed short *sndptr2, size_t numsamples);

#ifdef __cplusplus
}
#endif

#endif
