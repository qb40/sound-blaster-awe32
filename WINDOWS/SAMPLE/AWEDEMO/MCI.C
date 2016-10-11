/*****************************************************************************
*                                                                            *
* MCI.C SB AWE32 MCI Support module                                          *
*                                                                            *
* (C) Copyright Creative Technology Ltd. 1992-94. All rights reserved        *
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
******************************************************************************/

/*****************************************************************************
*   File name   : MCI.C                                                      *
*                                                                            *
*   Programmer  : Nigel Tan/Cheng Kok Hoong                                  *
*                 Creative Technology Ltd, 1994. All rights reserved.        *
*                                                                            *
*   MCI provides support routines for calling MCISEQ                         *
*                                                                            *
******************************************************************************/

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "mci.h"

DWORD FAR PASCAL MCIOpenDevice (LPWORD lpwDeviceID, LPSTR szFileName)
{
    DWORD dwError;
    MCI_OPEN_PARMS MciOpenParm;

    MciOpenParm.dwCallback = 0L;
    MciOpenParm.wDeviceID = 0;
    MciOpenParm.lpstrDeviceType = NULL;
    MciOpenParm.lpstrElementName = (LPSTR) szFileName;
    MciOpenParm.lpstrAlias = NULL;
    dwError = mciSendCommand(0, MCI_OPEN, MCI_WAIT| MCI_OPEN_ELEMENT,
        (DWORD)(LPMCI_OPEN_PARMS)&MciOpenParm);

    *lpwDeviceID = MciOpenParm.wDeviceID;

    return dwError;
}

DWORD FAR PASCAL MCIPlayDevice (HWND hWnd, WORD wDeviceID)
{
    DWORD dwError;
    MCI_PLAY_PARMS MciPlayParm;

    // Send command to play.
    MciPlayParm.dwCallback = (unsigned long)hWnd;
    MciPlayParm.dwFrom = 0;
    MciPlayParm.dwTo = 0;
    dwError = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
        (DWORD) (LPMCI_PLAY_PARMS)&MciPlayParm);

    return dwError;
}

DWORD FAR PASCAL MCIEndPlay (WORD wDeviceID)
{
    DWORD dwError;
    MCI_GENERIC_PARMS MciGenParm;

    MciGenParm.dwCallback = 0L;
    dwError = mciSendCommand(wDeviceID, MCI_STOP, MCI_WAIT,
        (DWORD)(LPMCI_GENERIC_PARMS)&MciGenParm);

    if(dwError) return dwError;

    return dwError;
}

DWORD FAR PASCAL MCICloseDevice (WORD wDeviceID)
{
    DWORD dwError;
    MCI_GENERIC_PARMS MciGenParm;

    MciGenParm.dwCallback = 0L;

    dwError = mciSendCommand(wDeviceID, MCI_CLOSE, MCI_WAIT,
                    (DWORD)(LPMCI_GENERIC_PARMS)&MciGenParm);

    return dwError;
}
