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
* sftype.h   SoundFont type defines header file                              *
*                                                                            *
*            Version 1.00                                                    *
*                                                                            *
*            Copyright (c) 1994-96, Creative Labs/Creative Technology Ltd    *
*                                                                            *
\****************************************************************************/

#ifndef _SFTYPE_H
#define _SFTYPE_H


#define LOCAL       0
#define GLOBAL      1

typedef struct _SFTYPE {

        short           startAddrs;             /* sample start address -4 (0 to * 0xffffff) */
        short           endAddrs;
        short           startloopAddrs;         /* loop start address -4 (0 to * 0xffffff) */
        short           endloopAddrs;           /* loop end address -3 (0 to * 0xffffff) */

        short           startAddrsHi;           /* high word of startAddrs */
        short           lfo1ToPitch;            /* main fm: lfo1-> pitch */
        short           lfo2ToPitch;            /* aux fm:  lfo2-> pitch */
        short           env1ToPitch;            /* pitch env: env1(aux)-> pitch */

        short           initialFilterFc;        /* initial filter cutoff */
        short           initialFilterQ;         /* filter Q */
        short           lfo1ToFilterFc;         /* filter modulation: lfo1 -> filter * cutoff */
        short           env1ToFilterFc;         /* filter env: env1(aux)-> filter * cutoff */

        short           endAddrsHi;             /* high word of endAddrs */
        short           lfo1ToVolume;           /* tremolo: lfo1-> volume */
        short           env2ToVolume;           /* Env2Depth: env2-> volume */

        short           chorusEffectsSend;      /* chorus */
        short           reverbEffectsSend;      /* reverb */
        short           panEffectsSend;         /* pan */
        short           auxEffectsSend;         /* pan auxdata (internal) */
        short           sampleVolume;           /* used internally */
        short           unused3;

        short           delayLfo1;              /* delay 0x8000-n*(725us) */
        short           freqLfo1;               /* frequency */

        short           delayLfo2;              /* delay 0x8000-n*(725us) */
        short           freqLfo2;               /* frequency */

        short           delayEnv1;              /* delay 0x8000 - n(725us) */
        short           attackEnv1;             /* attack */
        short           holdEnv1;               /* hold */
        short           decayEnv1;              /* decay */
        short           sustainEnv1;            /* sustain */
        short           releaseEnv1;            /* release */
        short           autoHoldEnv1;
        short           autoDecayEnv1;

        short           delayEnv2;              /* delay 0x8000 - n(725us) */
        short           attackEnv2;             /* attack */
        short           holdEnv2;               /* hold */
        short           decayEnv2;              /* decay */
        short           sustainEnv2;            /* sustain */
        short           releaseEnv2;            /* release */
        short           autoHoldEnv2;
        short           autoDecayEnv2;

        short           instrument;             /* */
        short           nop;
        short           keyRange;               /* */
        short           velRange;               /* */
        short           startloopAddrsHi;       /* high word of startloopAddrs */
        short           keynum;                 /* */
        short           velocity;               /* */
        short           instVol;                /* */
        short           keyTuning;
        short           endloopAddrsHi;         /* high word of endloopAddrs */

        short           coarseTune;
        short           fineTune;
        short           sampleId;
        short           sampleFlags;
        short           samplePitch;            /* SF1 only */
        short           scaleTuning;
        short           keyExclusiveClass;
        short           rootKey;

        DWORD           dwStart;
        DWORD           dwEnd;
        DWORD           dwStartLoop;
        DWORD           dwEndLoop;
        DWORD           dwPitch;
        BYTE            byOrigKey;
        char            chCorrection;

        WORD            wSampleType;
        WORD            wSampleLink;

        DWORD           dwSPadId;               /* for use with exclusive class */

} soundFont_type;

#define ZL              1
#define true            1
#define false           0

#define ZLFlag          1
#define ZRFlag          2
#define ROMFlag         0x8000

#define monoSample      1
#define rightSample     2
#define leftSample      4
#define linkedSample    8


/*ีอออออออออออออออออออออออธ*/
/*ณ    SoundFont Bank     ณ*/
/*ิอออออออออออออออออออออออพ*/

typedef char sfSampleName[20];

typedef struct {
        char            achPresetName[20];
        WORD            wPreset;
        WORD            wBank;
        WORD            wPresetBagNdx;
        DWORD           dwLibrary;
        DWORD           dwGenre;
        DWORD           dwMorphology;
} sfPreset;

typedef struct {
        WORD            wGenNdx;
        WORD            wModNdx;
} sfPresetBag;

typedef struct {
        WORD            wModSrcOper;
        WORD            wModDestOper;
        short           iAmount;
} sfModList;

typedef struct {
        WORD            wGenOper;
        short           iAmount;
} sfGenList;

typedef struct {
        char            achInst[20];
        WORD            wInstBagNdx;
} sfInst;

typedef struct {
        WORD            wInstGenNdx;
        WORD            wInstModNdx;
} sfInstBag;

typedef struct {
        WORD            wModSrcOper;
        WORD            wModDestOper;
        short           iAmount;
} sfInstModList;

typedef struct {
        WORD            wGenOper;
        short           iAmount;
} sfInstGenList;

typedef struct {
        DWORD           dwStart;
        DWORD           dwEnd;
        DWORD           dwStartLoop;
        DWORD           dwEndLoop;
} sfSample;
                                

/*ีอออออออออออออออออออออออออออธ*/
/*ณ    SoundFont 2.0 Bank     ณ*/
/*ิอออออออออออออออออออออออออออพ*/

typedef struct {
        char            achPresetName[20];
        WORD            wPreset;
        WORD            wBank;
        WORD            wPresetBagNdx;
        DWORD           dwLibrary;
        DWORD           dwGenre;
        DWORD           dwMorphology;
} sf2Preset;

typedef struct {
        WORD            wGenNdx;
        WORD            wModNdx;
} sf2PresetBag;

typedef struct {
        WORD            wModSrcOper;
        WORD            wModDestOper;
        short           iAmount;
        WORD            wModAmtSrcOper;
        WORD            wModTransOper;
} sf2ModList;

typedef struct {
        WORD            wGenOper;
        short           iAmount;
} sf2GenList;

typedef struct {
        char            achInst[20];
        WORD            wInstBagNdx;
} sf2Inst;

typedef struct {
        WORD            wInstGenNdx;
        WORD            wInstModNdx;
} sf2InstBag;

typedef struct {
        WORD            wModSrcOper;
        WORD            wModDestOper;
        short           iAmount;
        WORD            wModAmtSrcOper;
        WORD            wModTransOper;
} sf2InstModList;

typedef struct {
        WORD            wGenOper;
        short           iAmount;
} sf2InstGenList;

typedef struct {
        char            achSampleName[20];
        DWORD           dwStart;
        DWORD           dwEnd;
        DWORD           dwStartLoop;
        DWORD           dwEndLoop;
        DWORD           dwSampleRate;
        BYTE            byOriginalKey;
        char            chPitchCorrection;
        WORD            wSampleLink;
        WORD            wSampleType;
} sf2Sample;
                                

#endif
