;/****************************************************************************\
;*                                                                            *
;*  CTAWEAPI.INC SB AWE32 DOS Assembly Header                                 *
;*                                                                            *
;*  Copyright (c) Creative Technology Ltd. 1994-96. All rights reserved       *
;*  worldwide.                                                                *
;*                                                                            *
;*  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY     *
;*  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE       *
;*  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR     *
;*  PURPOSE.                                                                  *
;*                                                                            *
;*  You have a royalty-free right to use, modify, reproduce and               *
;*  distribute the Sample Files (and/or any modified version) in              *
;*  any way you find useful, provided that you agree to                       *
;*  the Creative's Software Licensing Aggreement and you also agree that      *
;*  Creative has no warranty obligations or liability for any Sample Files.   *
;*                                                                            *
;\****************************************************************************/
;
;/****************************************************************************\
;*      File name       : CTAWEAPI.INC                                        *
;*                                                                            *
;*      Programmer      : Creative SB AWE32 Team                              *
;*                        Creative Technology Ltd, 1994. All rights reserved. *
;*                                                                            *
;*      Version         : 2.5b                                                *
;*                                                                            *
;\****************************************************************************/

MAXBANKS    EQU     64

SCRATCH     STRUC
    dwCurOffset     DD  0
    dwSampleSize    DD  0
    dwWorkArea1     DD  0
                    DD  0
    wBass           DW  0
    wTreble         DW  0
    dwSampleStart   DD  MAXBANKS+1 DUP(0)
ifdef FAROBJ
    lpSoundPad      DF  MAXBANKS DUP(0)
else
    lpSoundPad      DD  MAXBANKS DUP(0)
endif
    dwWorkArea2     DD  8 DUP(0)
SCRATCH     ENDS

GCHANNEL    STRUC
            DB          24 DUP(0)
GCHANNEL    ENDS

MIDICHANNEL STRUC
            DB          30 DUP(0)
MIDICHANNEL ENDS

SOUNDFONT   STRUC
            DB          138 DUP(0)
SOUNDFONT   ENDS

NRPNCHANNEL STRUC
            DB          90 DUP(0)
NRPNCHANNEL ENDS

SOUNDPAD    STRUC
    SPad1   DD          0
    SPad2   DD          0
    SPad3   DD          0
    SPad4   DD          0
    SPad5   DD          0
    SPad6   DD          0
    SPad7   DD          0
SOUNDPAD    ENDS

AWE32ERR_ALLOCMEM       EQU     1
AWE32ERR_SBKFORMAT      EQU     2
AWE32ERR_DRAMSIZE       EQU     3
AWE32ERR_SBKREAD        EQU     4

