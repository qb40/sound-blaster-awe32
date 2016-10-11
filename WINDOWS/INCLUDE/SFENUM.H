/*****************************************************************************
*                                                                            *
* (C) Copyright Creative Technology Ltd. 1994-96. All rights reserved        *
* worldwide.                                                                 *
*                                                                            *
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY      *
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE        *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR      *
* PURPOSE.                                                                   *
*                                                                            *
* You have a royalty-free right to use, modify, reproduce and                *
* distribute the Sample Files (and/or any modified version) in               *
* any way you find useful, provided that you agree to                        *
* the Creative's Software Licensing Aggreement and you also agree that       *
* Creative has no warranty obligations or liability for any Sample Files     *
*                                                                            *
******************************************************************************/
/****************************************************************************\
*                                                                            *
* sfenum.h   Soundfont Enums header file                                     *
*                                                                            *
*            Version 1.00                                                    *
*                                                                            *
*            Copyright (c) 1994-1996, Creative Labs/Creative Technology Ltd  *
*                                                                            *
\****************************************************************************/

#ifndef _SFENUM_H
#define _SFENUM_H


enum GeneratorTypes
{       
/*  0 */    startAddrs,             /* sample start address -4 (0 to 0xffffff) */
/*  1 */    endAddrs,
/*  2 */    startloopAddrs,         /* loop start address -4 (0 to 0xffffff) */
/*  3 */    endloopAddrs,           /* loop end address -3 (0 to 0xffffff) */

/*  4 */    startAddrsHi,           /* high word of startAddrs */
/*  5 */    lfo1ToPitch,            /* main fm: lfo1-> pitch */
/*  6 */    lfo2ToPitch,            /* aux fm:  lfo2-> pitch */
/*  7 */    env1ToPitch,            /* pitch env: env1(aux)-> pitch */

/*  8 */    initialFilterFc,        /* initial filter cutoff */
/*  9 */    initialFilterQ,         /* filter Q */
/* 10 */    lfo1ToFilterFc,         /* filter modulation: lfo1 -> filter cutoff */
/* 11 */    env1ToFilterFc,         /* filter env: env1(aux)-> filter cutoff */

/* 12 */    endAddrsHi,             /* high word of endAddrs */
/* 13 */    lfo1ToVolume,           /* tremolo: lfo1-> volume */
/* 14 */    env2ToVolume,           /* Env2Depth: env2-> volume */

/* 15 */    chorusEffectsSend,      /* chorus */
/* 16 */    reverbEffectsSend,      /* reverb */
/* 17 */    panEffectsSend,         /* pan */
/* 18 */    auxEffectsSend,         /* pan auxdata (internal) */
/* 19 */    unused2,
/* 20 */    unused3,

/* 21 */    delayLfo1,              /* delay 0x8000-n*(725us) */
/* 22 */    freqLfo1,               /* frequency */

/* 23 */    delayLfo2,              /* delay 0x8000-n*(725us) */
/* 24 */    freqLfo2,               /* frequency */

/* 25 */    delayEnv1,              /*  delay 0x8000 - n(725us) */
/* 26 */    attackEnv1,             /*  attack */
/* 27 */    holdEnv1,               /*  hold */
/* 28 */    decayEnv1,              /*  decay */
/* 29 */    sustainEnv1,            /*  sustain */
/* 30 */    releaseEnv1,            /*  release */
/* 31 */    autoHoldEnv1,
/* 32 */    autoDecayEnv1,

/* 33 */    delayEnv2,              /*  delay 0x8000 - n(725us) */
/* 34 */    attackEnv2,             /*  attack */
/* 35 */    holdEnv2,               /*  hold */
/* 36 */    decayEnv2,              /*  decay */
/* 37 */    sustainEnv2,            /*  sustain */
/* 38 */    releaseEnv2,            /*  release */
/* 39 */    autoHoldEnv2,
/* 40 */    autoDecayEnv2,

/* 41 */    instrument,
/* 42 */    nop,
/* 43 */    keyRange,
/* 44 */    velRange,
/* 45 */    startloopAddrsHi,       /* high word of startloopAddrs */
/* 46 */    keynum,
/* 47 */    velocity,
/* 48 */    instVol,                /*   */
/* 49 */    keyTuning,
/* 50 */    endloopAddrsHi,         /* high word of endloopAddrs */

/* 51 */    coarseTune,
/* 52 */    fineTune,
/* 53 */    sampleId,
/* 54 */    sampleFlags,
/* 55 */    samplePitch,            /* SF1 only */
/* 56 */    scaleTuning,
/* 57 */    exclusiveClass,
/* 58 */    rootKey,

/* 59 */    endOper,

            /*  Special operator  */
            initialPitch = 4
};


enum ModulatorTypes
{
        /* Start of MIDI modulation operators */
        cc1_Mod,
        cc7_Vol,
        cc10_Pan,
        cc64_Sustain,
        cc91_Reverb,
        cc93_Chorus,

        ccPitchBend,
        ccIndirectModX,
        ccIndirectModY,

        endMod
};

enum scaleTuning {equalTemp,fiftyCents};

#endif
