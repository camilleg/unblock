/* Copyright (c) 1989-2007 John P. Costella.
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
** costella_body.h: 
**  
**   Start function body.
**
**   This version: July 9, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_BODY_H_
#define _COSTELLA_BODY_H_



/* COSTELLA_FUNCTION:
**
**   Define a function.
*/

#undef COSTELLA_FUNCTION

#define COSTELLA_FUNCTION( lfFunctionName, larglist ) \
  COSTELLA_ERROR_NODE* lfFunctionName larglist \
  COSTELLA_FUNCTION_COMMON( lfFunctionName )



/* COSTELLA_ANSI_FUNCTION:
**
**   Declare an "ANSI" function, i.e. one that does not follow the function
**   paradigm of the Costella libraries, but which must call such functions
**   and be able to use the error-handling functionality built into them. 
*/

#undef COSTELLA_ANSI_FUNCTION

#define COSTELLA_ANSI_FUNCTION( lfFunctionName, returntype, larglist ) \
  returntype lfFunctionName larglist \
  COSTELLA_FUNCTION_COMMON( lfFunctionName )



/* COSTELLA_FUNCTION_COMMON:
**
**   Common initialization commands for the preceding macros.
*/

#define COSTELLA_FUNCTION_COMMON( lfFunctionName ) \
  { \
    static COSTELLA_C i_l_acCostellaFunctionName[] = #lfFunctionName; \
    static COSTELLA_ERROR_NODE i_l_enCostella; \
    COSTELLA_B i_l_bCostellaError = COSTELLA_FALSE; \
    \
    i_l_enCostella.acFunctionName = i_l_acCostellaFunctionName; \
    i_l_enCostella.acError = 0; \
    i_l_enCostella.acCleanupError = 0; \
    i_l_enCostella.penNext = 0; \
    i_l_enCostella.penCleanupNext = 0; \
    { 



/* File is now included.
*/

#endif



/* Copyright (c) 1989-2007 John P. Costella.
**
** End of file.
*/
