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


/*
=============================================================================
 Name:          INDICES.H

 Description:   Defines the required index to access effects type and
                variations and synthesizer emulation of the AWEMAN.DLL.

 Purpose:       Header file meant for accessing effects type and variations
                of AWEMAN.DLL.

 Author:        E.W. Khor
                Copyright 1993-96 Creative Technology Ltd.

 Date:          version 1.0    -     27th Jan 1994

 Revision:
                version 1.1    -     2nd July 1994
                Version 1.2    -     12th July 1994
                Version 1.22   -     15th August 1994
                   Addition of new structure CSmplObject for new API.
                Version 1.40   -           7th Dec 1994
                   Make changes for 16 and 32 bit.
                   Addition of new structure CDevObject.
                        
=============================================================================
*/

#ifndef  _INDICES_H
#define  _INDICES_H

#ifdef _WIN32
#pragma pack(2)
#endif

enum SBANK {
    GENERAL_MIDI = 0,
    ROLAND_GS,
    MT_32,
    USER_CUSTOM_BANK
};

enum TYPEINDEX {
    REVERB_CHORUS = 0,
    QSOUND
};
            
enum SUBINDEX {
    REVERB = 0,
    CHORUS,
    BASS,
    TREBLE
};
            
enum VARIINDEX {
    ROOM_1 = 0,
    ROOM_2,
    ROOM_3,
    HALL_1,
    HALL_2,
    PLATE,
    DELAY,
    PANNING_DELAY,

    CHORUS_1 = 0,
    CHORUS_2,
    CHORUS_3,
    CHORUS_4,
    FEEDBACK_DELAY,
    FLANGER,
    SHORT_DELAY,
    SHORT_DELAY_FB
};

enum FLAG_OPERATION {
    OPER_FILE,
    OPER_MEMORY
};
               
/*
This CParamObject is usually used when required to pass several indices to
the DLL. This happens when configuring an effects type, synth emulation or
during query session. The LPPARAMOBJECT type define a far pointer to the
parameter object.
*/
typedef struct {

    enum SBANK      m_SBankIndex;   /* Follows strictly to the available Synth Emulation available */
    WORD            m_UBankIndex;   /* Valid number are from 1 to 127 */
    WORD            m_InstrIndex;   /* Instrument Index from 0 to 127 */
   
    enum TYPEINDEX  m_TypeIndex;    /* Follows strictly to the available Effects Type available */
    WORD            m_SubIndex;     /* A sub index of the Type Index */
    WORD            m_VariIndex[6]; /* 6 sets of variations! Extendibility thought */

} CParamObject;

#ifndef _WIN32
    typedef CParamObject FAR * LPPARAMOBJECT;
#else
    typedef CParamObject * LPPARAMOBJECT;
#endif

typedef const LPPARAMOBJECT  LPCPARAMOBJECT;


/*
The CBufferObject is used to specified a buffer area to the DLL. This buffer
area will usually be filled with some strings or values. In some occassions,
the buffer itself, actually carries information to the DLL, like filename
etc. The LPBUFFEROBJECT type define a far pointer to the buffer object.
*/

typedef struct {

    DWORD   m_Size;         /* The size of the buffer attached */
    LPSTR   m_Buffer;       /* The far pointer to the buffer itself */
    DWORD   m_SizeUsed;     /* The number of buffer characters used by DLL */
    WORD    m_Flag;         /* Scratch Variable */

} CBufferObject;

#ifndef _WIN32
    typedef CBufferObject FAR * LPBUFFEROBJECT;
#else
    typedef CBufferObject * LPBUFFEROBJECT;
#endif

typedef const LPBUFFEROBJECT  LPCBUFFEROBJECT;


/*
The CSmplObject is used to retrieve ROM sample data from hardware via driver.
*/

typedef struct {

    DWORD   m_dwStartAddr;      /* Start address of ROM smpl */
    DWORD   m_dwEndAddr;        /* End address of ROM smpl */
    UINT    m_hFile;            /* File handle for storing ROM smpl */

    /* extended for 32bit on 31st June 95 */
    char    m_Filename[260];    /* Maximum char needed for full path */

} CSmplObject;

#ifndef _WIN32
    typedef CSmplObject FAR * LPSMPLOBJECT;
#else
    typedef CSmplObject * LPSMPLOBJECT;
#endif

typedef const LPSMPLOBJECT  LPCSMPLOBJECT;


/*
The CMiscObject is added for expansion. It allows multiple messages to use
the same thunking function. The LPMISCOBJECT type define a far pointer to
the misc object.
*/

typedef struct {

    LPSTR   m_Buffer;       // FAR * to buffer
    DWORD   m_dwMsg;        // Message Number
    DWORD   m_dwParam1;
    DWORD   m_dwParam2;
    DWORD   m_dwParam3;

} CMiscObject;

#ifndef _WIN32
    typedef CMiscObject FAR * LPMISCOBJECT;
#else
    typedef CMiscObject * LPMISCOBJECT;
#endif

typedef const LPMISCOBJECT  LPCMISCOBJECT;


enum DEVCAPS {
    REVERB_CHORUS_CAP   = 1,
    QSOUND_CAP          = 2,
    AUDIO_3D_CAP        = 4,
    REGISTRY_CAP        = 8,
    FMTHRU_CAP          = 16,
    MPU401_EMU_CAP      = 32,
    VIENNA_CAP          = 512
};

typedef struct {
    WORD    m_SizeOf;           /* Sizeof this structure */
    DWORD   m_BaseAddr;         /* Base address of device */
    DWORD   m_DevNode;          /* Device node of device */
    DWORD   m_RomId;            /* ROM ID of device */
    DWORD   m_RomVer;           /* ROM version of device */
    DWORD   m_hTask;            /* Task acquiring device */
    DWORD   m_DevCaps;          /* 1  - Reverb/Chorus */
                                /* 2  - QSound */
                                /* 4  - 3D Audio API */
                                /* 8  - Registry support */
                                /* 16 - FM Pass through support */
                                /* 32 - MPU401 emulation */

    char    m_DevName[32];      /* Device name registered */
    char    m_SndEngine[16];	/* Sound Engine employed */
    char    m_RegKey[256];      /* Registry Key */

} CDevObject, FAR * LPDEVOBJECT;


/*
Following structures are for Vienna support
*/

typedef struct {

    DWORD   dwcbSize;           /* buffer size */
    DWORD   dwFlags;            /* flags for future expansion */
    DWORD   dwSampleHandle;     /* handle returned by driver */
    DWORD   dwSampleOffset;     /* set to 0 at first call */
    DWORD   dwReserved;
    WORD    wFormatTag;
    DWORD   dwSampleSize;       /* number of samples */
    DWORD   dwSamplesPerSec;    /* sampling rate */
    WORD    wBitsPerSample;
    WORD    wChannels;
    short   iSample[1];

} CViSmplObject, FAR* LPVISMPLOBJECT;

typedef struct {

    DWORD   dwFlags;            /* 1 = ROM sample */
    DWORD   dwReserved;
    DWORD   dwSampleHandle;
    LONG    lTuning;
    DWORD   dwStart;            /* set to 0 for RAM sample */
    DWORD   dwEnd;              /* set to 0 for RAM sample */
    DWORD   dwStartLoop;
    DWORD   dwEndLoop;

} CViPlayObject, FAR* LPVIPLAYOBJECT;

#ifdef _WIN32
#pragma pack()
#endif

#endif   /* _INDICES_H */

