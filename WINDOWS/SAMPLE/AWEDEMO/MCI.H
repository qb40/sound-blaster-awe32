/*****************************************************************************
*                                                                            *
* MCI.H SB AWE32 MCI Support module header file                              *
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
*   File name   : MCI.H                                                      *
*                                                                            *
*       Programmer      : Nigel Tan/Cheng Kok Hoong                          *
*                         Creative Technology Ltd, 1994. All rights reserved.*
*                                                                            *
*       MCI provides support routines header file                            *
*                                                                            *
******************************************************************************/

#include "mmsystem.h"

extern DWORD FAR PASCAL MCIOpenDevice (LPWORD, LPSTR);
extern DWORD FAR PASCAL MCIPlayDevice (HWND, WORD);
extern DWORD FAR PASCAL MCIEndPlay (WORD);
extern DWORD FAR PASCAL MCICloseDevice (WORD);
extern DWORD FAR PASCAL MCIGetLengthString (LPSTR, LPSTR);
