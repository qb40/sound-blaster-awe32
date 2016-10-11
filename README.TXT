
    ********************************************************************
    **                    Creative Technology Ltd.                    **
    **                                                                **
    **                        Sound Blaster AWE32                     **
    **                  Developer Information Package                 **
    **                                                                **
    **                          Version 3.01                          **
    **                                                                **
    **                          2 August 1996                         **
    **                                                                **
    ********************************************************************

    CONTENT
    =======
    1) Introduction
    2) What's new
    3) Files in this package
    4) Getting Assistance
    5) License Agreement/Limitation And Disclaimer Of Warranties

-------------------------------------------------------------------------

    1) Introduction
    ===============
    This developer information pack is designed to allows you to develop
    your own applications/drivers that will fully tap the capabilites of
    the SB AWE32 family cards. You will get a set of full featured libraries
    /API based on MIDI messages.


    2) What's new
    ==============

      a) Windows API
         - Vienna2 API added
         - Win95 support added
         - NRPN support for SF2

      b) EMU8000 programmmer's guide
         - EMU8000 registers documentation added
         NOTE : This is a highly technical documentation. It is meant for
                experienced developers who would like to develop special
                purpose drivers/libraries/applications that are not
                supported or not possible to be implemented by our
                libraries/API.

       c) SF2 DOS library
          Now the AWEDIP comes with 2 sets of libraries. 1 to support
          SBK (SoundFont 1) and another set to support SF2 (SoundFont 2).

          Major differences between these 2 sets of libraries:

                                SBKLIB             SF2LIB
                                ======             ======
          i)  NRPN support       YES                 NO
          ii) Embeded GM         YES                 NO
              preset

          When using SF2 library, your application need to load the GM
          presets at runtime. A SF2 GM preset ( SYNTHGM.SF2 ) is included.
          This gives you an option to load customized presets.


      d) Documentation in Adobe Acrobat Portable Document Format(PDF)
         - Documentation are now in PDF format for easy viewing and printing.
           You can download a copy of the Acrobat Reader from
           http://www.adobe.com


    3) Files in this package
    ========================

    Files                    Description
    =====                    ==========
    README.TXT               This file
    DOC  <DIR>
      |-  ADIP300.PDF        AWEDIP documentation
      |-  EMU8KPGM.PDF       EMU8000 Programmer's Guide

    DOS  <DIR>
      |- DEMO <DIR>          DOS Demo code
      |- EMUINC <DIR>        Include file for EMU8000 register level programming
      |- LIB
          |- SBKLIB <DIR>    SoundFont 1 Library
          |- SF2LIB <DIR>    SoundFont 2 Library

    WINDOWS <DIR>
      |- DLL    <DIR>        Latest DLL
      |- DRV    <DIR>        Latest Drivers
      |- INC    <DIR>        Include files
      |- SAMPLE <DIR>        Collections of Windows sample code.


   4) Getting Assistance
   =====================

   At Creative, we believe in providing you with the best support possible.
   If you have any comments, suggestions, questions, or problems concerning
   this developers information pack, please feel free to contact us at the
   following addresses :

   Inside U.S.A., Canada and South America
   ---------------------------------------

   Creative Labs
   1901 McCarthy Blvd.
   Milpitas, CA  95035

   Tel : (408) 428 6600
   Fax : (408) 432 6611

   Inside Europe
   -------------

   Creative Labs
   European Developer Relations
   Unit 3, Ruscombe Business Park
   Ruscombe
   Berkshire
   RG10 9NN
   UK

   Tel : (44) 1734 828237/8
   Fax : (44) 1734 828260

   In Asia-Pacific and everywhere else
   ----------------------------------

   Developer Support Group
   Creative Labs, Inc.
   1523 Cimarron Plaza
   Oklahoma 74075,
   U.S.A.

   Fax   : +405 742 6633
   Email : devsup@creaf.com


   5) License Agreement/Limitation And Disclaimer Of Warranties

   IMPORTANT NOTE :  BY DOWNLOADING AND/OR USING THE SOFTWARE AND/OR MANUAL
                     ACCOMPANYING THIS LICENSE AGREEMENT, YOU ARE HEREBY
                     AGREEING TO THE FOLLOWING TERMS AND CONDITIONS:

   The software and related written materials, including any instructions
   for use, are provided on an "AS IS" basis, without warranty of any kind,
   express or implied. This disclaimer of warranty expressly includes, but
   is not limited to, any implied warranties of merchantability and/or of
   fitness for a particular purpose. No oral or written information given
   by Creative Technology Ltd., Its suppliers, distributors, dealers,
   employees, or agents, shall create or otherwise enlarge the scope of any
   warranty hereunder. Licensee assumes the entire risk as to the quality and
   the performance of such software and licensee application. Should the
   software, and/or Licensee application prove defective, you, as Licensee
   (and not Creative Technology Ltd., its suppliers, distributors, dealers or
   agents), assume the entire cost of all necessary correction, servicing,
   or repair.

                            RESTRICTIONS ON USE
                            ===================

   Creative Technology Ltd. retains title and ownership of the manual and
   software as well as ownership of the copyright in any subsequent copies of
   the manual and software, irrespective of the form of media on or in which
   the manual and software are recorded or fixed. By downloading and/or using
   this manual and software, Licensee agrees to be bound to the terms of this
   agreement and further agrees that :

       (1) CREATIVE'S BBS/FTP/COMPUSERVE ARE THE ONLY ONLINE SITES WHERE
           USERS MAY DOWNLOAD ELECTRONIC FILES CONTAINING THE MANUAL AND/OR
           SOFTWARE,
       (2) LICENSEE SHALL USE THE MANUAL AND/OR SOFTWARE ONLY FOR THE
           PURPOSE OF DEVELOPING LICENSEE APPLICATIONS COMPATIBLE WITH
           CREATIVE'S SB AWE32 SERIES OF PRODUCTS, UNLESS OTHERWISE
           AGREED TO BY FURTHER WRITTEN AGREEMENT FROM CREATIVE
           TECHNOLOGY LTD.; AND,
       (3) LICENSEE SHALL NOT DISTRIBUTE OR COPY THE MANUAL FOR ANY REASON
           OR BY ANY MEANS (INCLUDING IN ELECTRONIC FORM) OR DISTRIBUTE,
           COPY, MODIFY, ADAPT, REVERSE ENGINEER, TRANSLATE OR PREPARE ANY
           DERIVATIVE WORK BASED ON THE MANUAL OR SOFTWARE OR ANY ELEMENT
           THEREOF OTHER THAN FOR THE ABOVE SAID PURPOSE, WITHOUT THE EXPRESS
           WRITTEN CONSENT OF CREATIVE TECHNOLOGY LTD.. CREATIVE TECHNOLOGY
           LTD. RESERVES ALL RIGHTS NOT EXPRESSLY GRANTED TO LICENSEE IN THIS
           LICENSE AGREEMENT.



                           LIMITATION OF LIABILITY
                           =======================

   In no event will Creative Technology Ltd., or anyone else involved in the
   creation, production, and/or delivery of this software product be liable
   to licensee or any other person or entity for any direct or other damages,
   including, without limitation, any interruption of services, lost profits,
   lost savings, loss of data, or any other consequential, incidental, special,
   or punitive damages, arising out of the purchase, use, inability to use, or
   operation of the software, and/or Licensee application, even if Creative
   Technology Ltd. or any authorised Creative Technology Ltd. dealer has been
   advised of the possibility of such damages. Licensee accepts said disclaimer
   as the basis upon which the software is offered at the current price and
   acknowledges that the price of the software would be higher in lieu of said
   disclaimer. Some states do not allow the limitation or exclusion of
   liability for incidental or consequential damages so the above limitations
   and exclusions may not apply to you.

   Information in this document is subject to change without notice. Creative
   Technology Ltd. shall have no obligation to update or otherwise correct any
   errors in the manual and software even if Creative Technology Ltd. is aware
   of such errors and Creative Technology Ltd. shall be under no obligation
   to provide to Licensee any updates, corrections or bug-fixes which Creative
   Technology Ltd. may elect to prepare.

   Creative Technology Ltd. does not warrant that the functions contained in
   the manual and software will be uninterrupted or error free and Licensee
   is encouraged to test the software for Licensee's intended use prior
   to placing any reliance thereon.


   Copyright 1995-1996 by Creative Technology Ltd. All rights reserved.
   Sound Blaster AWE32 is a trademark of Creative Technology Ltd.
   SoundFont is a registered trademark of E-Mu Systems, Inc.
