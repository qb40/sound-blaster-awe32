/*****************************************************************************
*                                                                            *
* WINAWE32 main header file                                                  *
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
/*****************************************************************************\
*    File name   : WINAWE32.H                                                *
*                                                                            *
*    Programmer  : Cheng Kok Hoong                                           *
*                  Creative Technology Ltd, 1994. All rights reserved.       *
*                                                                            *
******************************************************************************/

#include "resource.h"


#ifndef WIN32
  #define APIENTRY   FAR PASCAL
  #define LRESULT    BOOL 
#endif                   
       

 int APIENTRY WinMain(HANDLE, HANDLE, LPSTR, int);
 BOOL InitApplication(HANDLE);
 BOOL InitInstance(HANDLE, int);
 long APIENTRY  MainWndProc(HWND, UINT, WPARAM, LPARAM);
 LRESULT CALLBACK About(HWND, UINT , WPARAM, LPARAM);
 LRESULT CALLBACK TriggerMe(HWND, UINT, WPARAM, LPARAM);

