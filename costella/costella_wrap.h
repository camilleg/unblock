/* Copyright (c) 2006-2007 John P. Costella.
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
** costella_wrap.h: 
**  
**   Header file for costella_wrap.c.
**
**   This version: July 11, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_WRAP_H_
#define _COSTELLA_WRAP_H_



/* Include files. 
*/

#include "costella_base.h"



/* Structures.
*/

typedef struct 
{
  void (*pf)( void );
}
COSTELLA_WRAP_FUNCTION_POINTER;


typedef struct 
{
  int (*pfCallback)( void* pvPassback, void* pv );
  void* pvPassback;
}
COSTELLA_WRAP_CALLBACK;


typedef struct 
{
  int (*pfProgress)( void* pvPassback );
  void* pvPassback;
}
COSTELLA_WRAP_PROGRESS;



/* Function prototypes.
*/

COSTELLA_FUNCTION( CostellaWrapInitialize, ( void ) )
COSTELLA_FUNCTION( CostellaWrapFinalize, ( void ) )

COSTELLA_FUNCTION( CostellaWrapCallbackNew, ( COSTELLA_WRAP_CALLBACK** ppwc,
  int (*pfCallback)( void* pvPassback, void* pv ), void* pvPassback ) )
COSTELLA_FUNCTION( CostellaWrapCallbackDelete, ( COSTELLA_WRAP_CALLBACK** 
  ppwc ) )
COSTELLA_FUNCTION( CostellaWrapCallback, ( COSTELLA_O* poPassback, 
  COSTELLA_O* po ) )

COSTELLA_FUNCTION( CostellaWrapProgress, ( COSTELLA_O* poPassback ) )

COSTELLA_FUNCTION( CostellaWrapNewCopy, ( COSTELLA_O** 
  ppoNewCopy, COSTELLA_O* poExisting, COSTELLA_O* poPassback ) )
COSTELLA_FUNCTION( CostellaWrapDelete, ( COSTELLA_O** ppo, COSTELLA_O* 
  poPassback ) )
COSTELLA_FUNCTION( CostellaWrapCompare, ( COSTELLA_SB* psbCompare, 
  COSTELLA_O* po1, COSTELLA_O* po2, COSTELLA_O* poPassback ) )



/* File is now included.
*/

#endif



/* Copyright (c) 2006-2007 John P. Costella.
**
** End of file.
*/
