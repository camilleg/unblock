/* Copyright (c) 2005-2007 John P. Costella.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software")
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included 
** in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
** OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING
** FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
**
**
** costella_unblock.h: 
**
**   Header file for costella_unblock.c
**
**   This version: August 2, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_UNBLOCK_H_
#define _COSTELLA_UNBLOCK_H_



/* Include file.
*/

#include "costella_image_chrominance.h"
#include "costella_image_convert.h"



/* Public interface.
*/

int costella_unblock_initialize( FILE* pfileError );
int costella_unblock_finalize( FILE* pfileError );

int costella_unblock( COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* piOut, int
  bPhotographic, int bCartoon, int (*pfProgress)( void* pvPassback ), void* 
  pvPassback, FILE* pfileError );



/* Function prototypes.
*/

COSTELLA_FUNCTION( CostellaUnblockInitialize, ( void ) )
COSTELLA_FUNCTION( CostellaUnblockFinalize, ( void ) )

COSTELLA_FUNCTION( CostellaUnblock, ( COSTELLA_IMAGE* piIn, COSTELLA_IMAGE* 
  piOut, COSTELLA_B bPhotographic, COSTELLA_B bCartoon, 
  COSTELLA_CALLBACK_FUNCTION pfProgress, COSTELLA_O* poPassback ) )



/* File is now included.
*/

#endif



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/

