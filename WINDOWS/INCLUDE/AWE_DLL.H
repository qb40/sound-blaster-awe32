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
 Name:          AWE_DLL.H

 Description:   Entire API message function header file for accessing
                AWEMAN.DLL. This header file includes the necessary header
                files inorder to access the AWEMAN.DLL. These header files
                contains public APIs which is essential to address the APIs
                provided by AWEMAN.DLL or AWEMAN32.DLL.

 Purpose:       Developer API Guide.

 Author:        E.W. Khor
                Copyright 1993-96 Creative Technology Ltd.

 Date:          Version 1.0 - 27th Jan 1994

 Revision:      Version 1.40 - 23rd April 1995
                    Support for 16 and 32 bit.

=============================================================================
*/

#ifndef _AWE_DLL_H
#define _AWE_DLL_H


#include "awe_api.h"    /* Header file for public APIs. Public */

#include "errormsg.h"   /* Header file containing legal error messages for */
                        /* DLL and MIDI driver */
#include "indices.h"    /* Header file containing macro for Effect Type, */
                        /* Variation and Synth emulation */

/*
-----------------------------------------------------------------------------

      GLOBAL MACRO AND EXTERN DEFINITION
      
-----------------------------------------------------------------------------
*/

/*
    Type define a AWEHANDLE. This handle, when instantiate, is actually the
    handle to a block of memory containing the last-used and necessary
    settings of the AWE hardware. This object is required as it allows the
    AWEMAN.DLL to have multiple callers opening it. This handle serves as and
    identification of proper initialization and acquisition of AWEMAN.DLL.

    Example:

    :

    int         nCounter;
    AWEHANDLE   hMyID;      // hMyID stores a copy of AWEManager returned ID.

    :
    (Acquire AWE Manager)
    :

*/

#ifndef _WIN32
    DECLARE_HANDLE32(AWEHANDLE);
#else
    typedef DWORD AWEHANDLE;
#endif


/*
    The LPFNAWEMANAGER type is used to declare a pointer to the message 
    function. This message function will receive all necessary messages from 
    the application who wishes to access its API. This type declaration is 
    useful especially when dynamic linking is used.

    Example:
      
    :
      
    AWEHANDLE         hMyID;
    LPFNAWEMANAGER    lpfnAWEManager;
      
    :
    lpfnAWEManager = (Retrieve Address of function);
    :

*/

#ifndef _WIN32
    typedef LRESULT (FAR PASCAL *LPFNAWEMANAGER) (AWEHANDLE, UINT, LPARAM, LPARAM);
#else
    typedef LRESULT (WINAPI *LPFNAWEMANAGER) (AWEHANDLE, UINT, LPARAM, LPARAM);
#endif


#ifdef __cplusplus
extern "C" 
{
#endif

/*
    Message Entry Function. This function provides entry to the AWE DLL
    Manager. An error message will be returned via the LRESULT return type.

*/

#ifndef _WIN32
    LRESULT FAR PASCAL _export AWEManager( AWEHANDLE hUserID, UINT unMsg, LPARAM lParam1, LPARAM lParam2 );
#else
    LRESULT WINAPI AWEManager( AWEHANDLE hUserID, UINT unMsg, LPARAM lParam1, LPARAM lParam2 );
#endif

#ifndef _WIN32

/*
    Activates the AWE Control application (obsolete).
    lpszAppTitle contains a null-terminated string to the caption title of
    control panel. If NULL is specified, the default name 'AWE Control' will
    be used.
    bButton turns on or off the button which allows control panel to switch
    to Controller BOB.
*/
LRESULT FAR PASCAL _export AWEControlPanel( LPCSTR lpszAppTitle, BOOL bButton );

/*
    Activates the AWE Controller Break-Out-Box (obsolete).
    lpszAppTitle contains a null-terminated string to the caption title of
    control panel. If NULL is specified, the default name 'AWE Control' will
    be used.
    bButton turns on or off the button which allows to Controller BOB switch
    to control panel.
*/
LRESULT FAR PASCAL _export AWEControllerBOB( LPCSTR lpszAppTitle, BOOL bButton );

#endif

#ifdef __cplusplus
}
#endif

#endif      /* _AWEDLL_H */
