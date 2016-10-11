/****************************************************************************\
*                                                                            *
* DEMO Sample Demo for SB AWE32 Developer's Information Package              *
*                                                                            *
* (C) Copyright Creative Technology Ltd. 1992-96. All rights reserved        *
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
* Creative has no warranty obligations or liability for any Sample Files.    *
*                                                                            *
\****************************************************************************/

/****************************************************************************\
*    File name   : DEMO.C                                                    *
*                                                                            *
*    Programmer  : Tan Khir Hien                                             *
*                  Creative Technology Ltd, 1994. All rights reserved.       *
*                                                                            *
\****************************************************************************/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "ctaweapi.h"


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ                C O M P I L E R   D E P E N D E N C I E S               ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/

#if defined(__TURBOC__)
#define _inp    inportb
#define _outp   outportb
#endif

#if defined(__WATCOMC__)
#define _inp    inp
#define _outp   outp
#endif

#if defined(__HIGHC__)
#define kbhit   _kbhit
#define getch   _getch
#define memicmp _memicmp
#pragma Align_members(1)
#endif

#if defined(__SC__)
#define _inp   inp
#define _outp  outp
#endif


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ                            D E F I N E S                               ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/


/*  DSP defines  */
#define MPU_ACK_OK          0xfe
#define MPU_RESET_CMD       0xff
#define MPU_ENTER_UART      0x3f

/*  PIC defines  */
#define PIC1Ack             0x20
#define PIC1Mask            0x21
#define PIC2Ack             0xa0
#define PIC2Mask            0xa1
#define PIC1IntBase         0x08
#define PIC2IntBase         0x70
#define PIC2IRQChain        0x02

/* MIDI states */
#define READING             0
#define SYSEX               1
#define UNDEFINED           2

/*  macros  */
#define SBCPort(x)          ((x)+wSBCBaseAddx)
#define MPUPort(x)          ((x)+wMpuBaseAddx)


/* WAV header */
typedef struct {
     DWORD     dwFileID;               
     DWORD     dwFileLen;
     DWORD     dwFormatType;         
     DWORD     dwFormatID;       
     DWORD     dwFormatLen;
     WORD      wFormatTag;
     WORD      wChannels;
     DWORD     dwSamplesPerSec;
     DWORD     dwAvgSample;
     WORD      wBlockAlign;
     WORD      wBitsPerSample;
} PCMRIFF;


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ                    G L O B A L   V A R I A B L E S                     ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/

WORD wSBCBaseAddx   = 0x220;            /* Sound Blaster base address */
WORD wEMUBaseAddx   = 0x620;            /* EMU8000 subsystem base address */
WORD wMpuBaseAddx   = 0x330;            /* MPU401 base address */
WORD wMIDIRecv      = 0;                /* Number of MIDI bytes received */
WORD wMIDIState     = UNDEFINED;        /* MIDI parsing state */
BYTE bMIDIBuf[32]   = {0};              /* MIDI read buffer */
BYTE bMIDIMsgLen[]  = {3, 3, 3, 3, 2, 2, 3, 0};

/* SoundFont variables */
WAVE_PACKET wpWave          = {0};
SOUND_PACKET spSound        = {0};
char* pPresets[MAXBANKS]    = {0};
long lBankSizes[MAXBANKS]   = {0};
char Packet[PACKETSIZE]     = {0};


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ InitSoundBlaster                                                       ณ*/
/*ณ Initializes Sound Blaster to ready to receive data state.              ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
int
InitSoundBlaster (void)
{
    volatile DWORD dwCount;

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x40) --dwCount;
    _outp(MPUPort(1), MPU_RESET_CMD);

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x80) --dwCount;
    _inp(MPUPort(0));

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x40) --dwCount;
    _outp(MPUPort(1), MPU_RESET_CMD);

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x80) --dwCount;
    _inp(MPUPort(0));

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x40) --dwCount;
    _outp(MPUPort(1), MPU_ENTER_UART);

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x80) --dwCount;
    if (!dwCount) return TRUE;
    if (_inp(MPUPort(0)) != MPU_ACK_OK) return TRUE;

    /* mask MPU-401 interrupt */
    _outp(SBCPort(0x4), 0x83);
    _outp(SBCPort(0x5), _inp(SBCPort(0x5)) & ~0x04);

    return FALSE;
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ CleanUpSoundBlaster                                                    ณ*/
/*ณ Cleans up Sound Blaster to normal state.                               ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
void
CleanUpSoundBlaster (void)
{
    volatile DWORD dwCount;

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x40) --dwCount;
    _outp(MPUPort(1), MPU_RESET_CMD);
    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    _inp(MPUPort(0));

    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    dwCount = 0x2000;
    while (dwCount && _inp(MPUPort(1)) & 0x40) --dwCount;
    _outp(MPUPort(1), MPU_RESET_CMD);
    for (dwCount=0; dwCount<0x2000; dwCount++) ;
    _inp(MPUPort(0));
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ GetHexWord                                                             ณ*/
/*ณ Convert hex string into WORD.                                          ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
WORD
GetHexWord(const char ** s)
{
    WORD n = 0;

    while (**s) {
        if (**s >= '0' && **s <= '9')
            n = n * 16 + (*(*s)++ - '0');
        else
            if (**s >= 'a' && **s <= 'f')
                n = n * 16 + (*(*s)++ - 'a') + 10;
            else
                if (**s >= 'A' && **s <= 'F')
                    n = n * 16 + (*(*s)++ - 'A') + 10;
                else
                    break;
    }

    return n;
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ GetEnvSettings                                                         ณ*/
/*ณ Searches the base addresses of sound blaster card.                     ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
void
GetEnvSettings (void)
{
    WORD srcAddx = 0xffff;
    WORD mpuAddx = 0xffff;
    WORD emuAddx = 0xffff;
    const char* szBlaster;

    szBlaster = getenv("BLASTER");
    if (!szBlaster) return;
    
    while (*szBlaster) {
        if (*szBlaster == 'a' || *szBlaster == 'A') {
            ++szBlaster;
            if (*szBlaster) {
                if (*szBlaster == ':') ++szBlaster;
                srcAddx = GetHexWord(&szBlaster);
            }
        }
        if (*szBlaster == 'e' || *szBlaster == 'E') {
            ++szBlaster;
            if (*szBlaster) {
                if (*szBlaster == ':') ++szBlaster;
                emuAddx = GetHexWord(&szBlaster);
            }
        }
        if (*szBlaster == 'p' || *szBlaster == 'P') {
            ++szBlaster;
            if (*szBlaster) {
                if (*szBlaster == ':') ++szBlaster;
                mpuAddx = GetHexWord(&szBlaster);
            }
        }

        /* skip until blank or end of string */
        while (*szBlaster && *szBlaster != ' ') ++szBlaster;
        while (*szBlaster == ' ') ++szBlaster;
    }

    if (mpuAddx != 0xffff) wMpuBaseAddx = mpuAddx;
    if (srcAddx != 0xffff) wSBCBaseAddx = srcAddx;
    if (emuAddx != 0xffff)
        wEMUBaseAddx = emuAddx;
    else
        if (srcAddx != 0xffff)
            wEMUBaseAddx = srcAddx + 0x400;
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ OpenSBK                                                                ณ*/
/*ณ Locate and open a SBK file.                                            ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
FILE*
OpenSBK (const char* szSBKFile)
{
    FILE* fp;
    char szFilePath[80];
    const char* szSoundEnv;

    szSoundEnv = getenv("SOUND");
    if (szSoundEnv) {
        strcpy(szFilePath, szSoundEnv);
        strcat(szFilePath, "\\SFBANK\\");
        strcat(szFilePath, szSBKFile);
        fp = fopen(szFilePath, "rb");
        if (fp) return fp;
    }

    return fopen(szSBKFile, "rb");
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ LoadSBK                                                                ณ*/
/*ณ Scan the command line to load the specified SBK file                   ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
int
LoadSBK(int argc, char** argv)
{
    int i;
    FILE* fp;
    char szSBKFile[40];
    
    for (i=1; i<argc; i++)
        if (memicmp(argv[i], "/SBK:", 5) == 0) {
            strcpy(szSBKFile, argv[i]+5);
            break;
        }

    /* if no SBK file specified */
    if (i == argc) {

#ifdef SBKLIB
        /* use embeded preset objects */
        spSound.bank_no = 0;                    /* load as Bank 0 */
        spSound.total_banks = 1;                /* use 1 bank first */
        lBankSizes[0] = 0;                      /* ram is not needed */
        spSound.banksizes = lBankSizes;
        awe32DefineBankSizes(&spSound);
        awe32SoundPad.SPad1 = awe32SPad1Obj;
        awe32SoundPad.SPad2 = awe32SPad2Obj;
        awe32SoundPad.SPad3 = awe32SPad3Obj;
        awe32SoundPad.SPad4 = awe32SPad4Obj;
        awe32SoundPad.SPad5 = awe32SPad5Obj;
        awe32SoundPad.SPad6 = awe32SPad6Obj;
        awe32SoundPad.SPad7 = awe32SPad7Obj;

        return FALSE;
#else
        /* For SF2 lib, GM presets are not included. */
        /* Applications should load the GM presets.  */
        /* SYNTHGM.SF2 ( GM presets in SF2 format ) are loaded as default */
        strcpy(szSBKFile, "synthgm.sf2" ) ;
#endif

    }

    fp = OpenSBK(szSBKFile);
    if (!fp) {
        printf("ERROR:  Cannot open %s\n", szSBKFile);
        return TRUE;
    }
    
    /* allocate ram */
    spSound.bank_no = 0;                        /* load as Bank 0 */
    spSound.total_banks = 1;                    /* use 1 bank first */
    lBankSizes[0] = spSound.total_patch_ram;    /* use all available ram */
    spSound.banksizes = lBankSizes;
    awe32DefineBankSizes(&spSound);

    /* request to load */
    spSound.data = Packet;
    fread(Packet, 1, PACKETSIZE, fp);
    if (awe32SFontLoadRequest(&spSound)) {
        fclose(fp);
        printf("ERROR:  Cannot load SoundFont file %s\n", szSBKFile);
        return TRUE;
    }
    
    /* stream samples */
    fseek(fp, spSound.sample_seek, SEEK_SET);
    for (i=0; i<spSound.no_sample_packets; i++) {
        fread(Packet, 1, PACKETSIZE, fp);
        awe32StreamSample(&spSound);
    }
    
    /* setup SoundFont preset objects */
    fseek(fp, spSound.preset_seek, SEEK_SET);
    pPresets[0] = (char*) malloc((unsigned) spSound.preset_read_size);
    fread(pPresets[0], 1, (unsigned) spSound.preset_read_size, fp);
    spSound.presets = pPresets[0];
    if (awe32SetPresets(&spSound)) {
        fclose(fp);
        printf("ERROR:  Invalid SoundFont file %s\n", szSBKFile);
        return TRUE;
    }
    fclose(fp);
    
    /* calculate actual ram used */
    if (spSound.no_sample_packets) {
        lBankSizes[0] = spSound.preset_seek - spSound.sample_seek + 160;
        spSound.total_patch_ram -= lBankSizes[0];
    }
    else
        lBankSizes[0] = 0;          /* no sample in SBK file */

    return FALSE;
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ LoadWAV                                                                ณ*/
/*ณ Scan the command line to load the specified WAV file                   ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
int
LoadWAV(int argc, char** argv)
{
    int i;
    int bank;
    FILE* fp;
    PCMRIFF riff;
    long sampsize;
    
    for (i=1; i<argc; i++) {
        if (*argv[i] == '/') {
            argv[i] = NULL;
            continue;
        }
        
        fp = fopen(argv[i], "rb");
        if (!fp) {
            printf("ERROR:  Cannot open %s\n", argv[i]);
            return TRUE;
        }
        if (fread(&riff, 1, sizeof(PCMRIFF), fp) != sizeof(PCMRIFF)) {
            fclose(fp);
            printf("ERROR:  %s is not a WAV file\n", argv[i]);
            return TRUE;
        }
        if (riff.wChannels == 2) {
            fclose(fp);
            printf("ERROR:  %s is a stereo WAV file\n", argv[i]);
            return TRUE;
        }
        riff.dwFormatLen -= 16;
        riff.dwFormatLen += 4;
        fseek(fp, riff.dwFormatLen, SEEK_CUR);
        fread(&sampsize, 1, sizeof(long), fp);

        if (sampsize+160 > spSound.total_patch_ram) {
            fclose(fp);
            printf("ERROR:  Not enough patch ram to load %s\n", argv[i]);
            return TRUE;
        }
        
        /* allocate patch ram */
        bank = spSound.total_banks;
        lBankSizes[bank] = sampsize;
        if (riff.wBitsPerSample == 8) lBankSizes[bank] *= 2;
        lBankSizes[bank] +=160;
        spSound.total_patch_ram -= lBankSizes[bank];
        spSound.total_banks += 1;
        awe32DefineBankSizes(&spSound);

        /* setup WAVE_PACKET */
        wpWave.tag = 0x101;
        wpWave.bank_no = (short) bank;
        wpWave.data = Packet;
        wpWave.sample_size = (riff.wBitsPerSample == 16) ? sampsize/2 : sampsize;
        wpWave.samples_per_sec = riff.dwSamplesPerSec;
        wpWave.bits_per_sample = riff.wBitsPerSample;
        wpWave.no_channels = 1;
        wpWave.looping = 0;
        wpWave.startloop = 0;
        wpWave.endloop = wpWave.sample_size;
        wpWave.release = 0;

        /* request to load WAV */
        if (awe32WPLoadRequest(&wpWave)) {
            fclose(fp);
            printf("ERROR:  Request to load %s failed\n", argv[i]);
            return TRUE;
        }

        /* stream the raw PCM samples */
        wpWave.data = Packet;
        do 
            fread(Packet, 1, PACKETSIZE, fp);
        while (!awe32WPStreamWave(&wpWave));
        fclose(fp);

        /* build SoundFont preset objects */
        pPresets[bank] = (char*) malloc(wpWave.preset_size);
        wpWave.presets = pPresets[bank];
        if (awe32WPBuildSFont(&wpWave)) {
            printf("ERROR:  Cannot build SoundFont preset objects\n");
            return TRUE;
        }
    }
    
    return FALSE;
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ SendMidi                                                               ณ*/
/*ณ Send Midi event to MIDI engine                                         ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
void
SendMidi (void)
{
    switch (bMIDIBuf[0] >> 4) {
        case 0x8:
            awe32NoteOff((WORD) bMIDIBuf[0]&15, bMIDIBuf[1], bMIDIBuf[2]);
            break;
        case 0x9:
            awe32NoteOn((WORD) bMIDIBuf[0]&15, bMIDIBuf[1], bMIDIBuf[2]);
            break;
        case 0xa:
            awe32PolyKeyPressure((WORD) bMIDIBuf[0]&15, bMIDIBuf[1], bMIDIBuf[2]);
            break;
        case 0xb:
            awe32Controller((WORD) bMIDIBuf[0]&15, bMIDIBuf[1], bMIDIBuf[2]);
            break;
        case 0xc:
            awe32ProgramChange((WORD) bMIDIBuf[0]&15, bMIDIBuf[1]);
            break;
        case 0xd:
            awe32ChannelPressure((WORD) bMIDIBuf[0]&15, bMIDIBuf[1]);
            break;
        default:
            awe32PitchBend((WORD) bMIDIBuf[0]&15, bMIDIBuf[1], bMIDIBuf[2]);
            break;
    }
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ SendSysEx                                                              ณ*/
/*ณ Send SysEx event to MIDI engine                                        ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
void
SendSysEx (void)
{
    awe32Sysex(0, bMIDIBuf, wMIDIRecv);
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ ProcessMidi                                                            ณ*/
/*ณ MIDI stream processing                                                 ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
void
ProcessMidi (BYTE bMidi)
{
    if (bMidi & 0x80) {
        if (bMidi == 0xf7 && wMIDIState == SYSEX) {
            bMIDIBuf[wMIDIRecv++] = bMidi;
            SendSysEx();
            wMIDIRecv = 1;
            return;
        }
        wMIDIRecv = 0;
        bMIDIBuf[wMIDIRecv++] = bMidi;
        if ((bMidi >> 4) != 0xf)
            wMIDIState = READING;
        else
            wMIDIState = (WORD) ((bMidi == 0xf0) ? SYSEX : UNDEFINED);
        return;
    }

    if (wMIDIState == UNDEFINED) return;

    bMIDIBuf[wMIDIRecv++] = bMidi;
    if (wMIDIState == SYSEX) {
        if (wMIDIRecv == 32) wMIDIState = UNDEFINED;
    }
    else
        if (wMIDIRecv == (WORD) bMIDIMsgLen[(bMIDIBuf[0]>>4)-8]) {
            SendMidi();
            wMIDIRecv = 1;
        }
}


/*ีออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออธ*/
/*ณ main                                                                   ณ*/
/*ณ Where the fun begins...                                                ณ*/
/*ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ*/
int
main(int argc, char** argv)
{
    int i;
    int k, key;
    int ret = 1;
    
    printf("\n");
    printf("SB AWE32 Developer's Information Pack\n");
    printf("DOS Real/Protected Mode API demonstration\n");
    printf("Copyright (c) Creative Technology Ltd., 1994.\n");
    printf("\n");
    
    if (argc == 1) 

#ifdef SBKLIB
        printf("Usage:  DEMO [/SBK:sbkfile] [wav1.wav ...]\n\n", argv[0]);
#else
        printf("Usage:  DEMO [/SF2:sf2file] [wav1.wav ...]\n\n", argv[0]);
#endif

    GetEnvSettings();
    
    if (awe32Detect(wEMUBaseAddx)) {
        printf("ERROR:  SB AWE32 not detected\n");
        return ret;
    }
    
    if (awe32InitHardware()) {
        printf("ERROR:  SB AWE32 initialization failed\n");
        return ret;
    }
    
    if (InitSoundBlaster()) {
        printf("ERROR:  MPU401 initialization failed\n");
        return ret;
    }
    
    awe32TotalPatchRam(&spSound);
    
    if (LoadSBK(argc, argv)) goto cleanup;  /* load SBK file */
    if (LoadWAV(argc, argv)) goto cleanup;  /* load any specified WAV file */

    awe32InitMIDI();

#ifdef SBKLIB
    awe32InitNRPN();        /* pull in NRPN.OBJ and NRPNVAR.OBJ      */
                            /* NRPN is not supported in SF2 library */
#endif

    ret = 0;
    key = 1;
    for (i=1; i<argc; i++) {
        if (!argv[i]) continue;
        printf("Press '%d' for %s\n", key++, argv[i]);
    }
    printf("\nPress <Esc> to stop ...\n");
    
    loop:
        if (kbhit()) {
            k = getch();
            if (k == 27) goto cleanup;
            k = k - '0';
            if (k > 0 && k < key) {
                awe32Controller(15, 0, (WORD) k);
                awe32ProgramChange(15, 0);
                awe32NoteOn(15, 60, 127);
            }
        }
        if ((_inp(MPUPort(1)) & 0x80) == 0)
            ProcessMidi((BYTE) _inp(MPUPort(0)));
        goto loop;
        
    cleanup:

    /* free allocated memory */
    awe32ReleaseAllBanks(&spSound);
    for (i=0; i<spSound.total_banks; i++)
        if (pPresets[i]) free(pPresets[i]);
    CleanUpSoundBlaster();
    awe32Terminate();

    return ret;
}
