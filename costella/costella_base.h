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
** costella_base.h: 
**  
**   Header file for costella_base.c.
**
**   This version: November 4, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_BASE_H_
#define _COSTELLA_BASE_H_



/* Include files. 
*/

#include "costella_types.h"
#include "costella_debug.h"
#include <stdio.h>



/* Public interface.
*/

int costella_base_initialize( void* (*pfMalloc)( unsigned int uiNumBytes ), 
  void* (*pfRealloc)( void* pv, unsigned int uiNewNumBytes ), int (*pfFree)(
  void* pv ), FILE* pfileError );
int costella_base_finalize( FILE* pfileError );



/* Definitions convenient for clarity.
*/

#define COSTELLA_FALSE 0
#define COSTELLA_TRUE (!COSTELLA_FALSE)



/* COSTELLA_MALLOC:
**
**   Standardized interface to the memory allocation function registered in
**   CostellaBaseInitialize().
**
**   lpo:  Pointer to the allocated memory, once allocated.
**
**   lmc:  Number of objects to allocate.
**
**   Evaluates to the failure flag.
*/

#define COSTELLA_MALLOC( lpo, lmc ) \
  ( (lpo) ? COSTELLA_TRUE : !( (lpo) = gpfCostellaBaseMalloc( ( (lmc) ? \
  (COSTELLA_MC) (lmc) : 1 ) * (COSTELLA_MC) sizeof( *(lpo) ) ) ) )



/* COSTELLA_REALLOC:
**
**   Standardized interface to the memory reallocation function registered 
**   in CostellaBaseInitialize().
**
**   lpo:  Pointer previously allocated, to be reallocated.
**
**   lmc:  Number of objects to reallocate.
**
**   Evaluates to the failure flag.
*/

#define COSTELLA_REALLOC( lpo, lmc ) \
  ( !(lpo) ? COSTELLA_TRUE : !( (lpo) = gpfCostellaBaseRealloc( (lpo), ( \
  (lmc) ? (COSTELLA_MC) (lmc) : 1 ) * (COSTELLA_MC) sizeof( *(lpo) ) ) ) )



/* COSTELLA_FREE:
**
**   Standardized interface to the memory freeing function registered in
**   CostellaBaseInitialize(). Only frees the pointer if it is non-null.
**
**   lpo:  Pointer to the memory to free.
**
**   Evaluates to the failure flag.
*/

#define COSTELLA_FREE( lpo ) \
  ( (lpo) = (lpo) ? ( gpfCostellaBaseFree( (lpo) ) ? 0 : (lpo) ) : 0 )



/* COSTELLA_MALLOC_OFFSET: 
**
**   Allocate memory for an offset array.
**
**   lpo:  Offset pointer to the allocated memory, once allocated.
**
**   lmi{Min,Max}:  {Min,Max}imum offset required.
**
**   Evaluates to the failure flag.
*/

#define COSTELLA_MALLOC_OFFSET( lpo, lmiMin, lmiMax ) \
  ( (lpo) || (COSTELLA_MI) (lmiMin) > (COSTELLA_MI) (lmiMax) ? \
  COSTELLA_TRUE : ( ( (lpo) = gpfCostellaBaseMalloc( (COSTELLA_MC) ( \
  (COSTELLA_MI) (lmiMax) - (COSTELLA_MI) (lmiMin) + 1 ) * (COSTELLA_MC) \
  sizeof( *(lpo) ) ) ) ? ( (lpo) -= (lmiMin), COSTELLA_FALSE ) : \
  COSTELLA_TRUE ) )



/* COSTELLA_REALLOC_OFFSET: 
**
**   Reallocate memory for an offset array.
**
**   lpo:  Pointer previously allocated, to be reallocated.
**
**   lmi{Min,Max}{Old,New}:  {Old,New} {min,max}imum offset.
**
**   Evaluates to the failure flag.
*/

#define COSTELLA_REALLOC_OFFSET( lpo, lmiMinOld, lmiMinNew, lmiMaxNew ) \
  ( (!lpo) || (lmiMinNew) > (lmiMaxNew) ? COSTELLA_TRUE : ( (lpo) = \
  gpfCostellaBaseRealloc( (lpo) + (lmiMinOld), (COSTELLA_MC) ( (lmiMaxNew) \
  - (lmiMinNew) + 1 ) * (COSTELLA_MC) sizeof( *(lpo) ) ) ) ? ( (lpo) -= \
  (lmiMinNew), COSTELLA_FALSE ) : COSTELLA_TRUE  )



/* COSTELLA_FREE_OFFSET:
**
**   Free memory allocated with COSTELLA_MALLOC_OFFSET. Only frees the 
**   pointer if it is non-null.
**
**   lpo:  Pointer returned by COSTELLA_MALLOC_OFFSET.
**
**   lmiMin:  Minimum offset specified in COSTELLA_MALLOC_OFFSET.
**
**   Evaluates to the failure flag.
*/

#define COSTELLA_FREE_OFFSET( lpo, lmiMin ) \
  ( (lpo) = (lpo) ? ( gpfCostellaBaseFree( (lpo) + (lmiMin) ) ? 0 : (lpo) \
  ) : COSTELLA_FALSE )



/* COSTELLA_INITIALIZE_ARRAY: 
**
**   Initialize an array to zero. 
**
**   lax:  Array pointer.
**
**   lmcLength:  Length of the array.
**
**   ltype:  Type of object being initialized.
*/

#define COSTELLA_INITIALIZE_ARRAY( lax, lmcLength, ltype ) \
{ \
  ltype* lpx; \
  ltype* lpxEnd; \
  \
  for( lpx = (lax), lpxEnd = lpx + (lmcLength); lpx != lpxEnd; lpx++ ) \
  { \
    *lpx = 0; \
  } \
}



/* COSTELLA_INITIALIZE_OFFSET_ARRAY: 
**
**   Initialize an offset array to zero. 
**
**   lax:  Array pointer.
**
**   lmi{Min,Max}:  {Min,Max}imum offset.
**
**   ltype:  Type of object being initialized.
**
**   Inline macro. 
*/

#define COSTELLA_INITIALIZE_OFFSET_ARRAY( lax, lmiMin, lmiMax, ltype ) \
{ \
  ltype* lpx; \
  ltype* lpxLast; \
  \
  for( lpx = (lax) + lmiMin, lpxLast = (lax) + lmiMax; lpx <= lpxLast; \
    lpx++ ) \
  { \
    *lpx = 0; \
  } \
}



/* COSTELLA_SHIFT_RIGHT_TOWARDS_ZERO:
**
**   Fast division macro that uses the right bit-shift operation. Shifting 
**   right "towards zero" means that the absolute value of the result is 
**   never greater than that of the true (fractional) division.
**
**   lsx:  Number to be divided.
**
**   lubNumBits:  Number of bits to shift it right.
*/

#define COSTELLA_SHIFT_RIGHT_TOWARDS_ZERO( lsx, lubNumBits ) \
  ( (lsx) < 0 ? -( -(lsx) >> (lubNumBits) ) : (lsx) >> (lubNumBits) )



/* COSTELLA_SHIFT_RIGHT_FLOOR:
**
**   Fast division macro that use the right bit-shift operation. 
** 
**   The result is not greater than the true (fractional) division, ie, is 
**   not to the right of it on the number line.
**
**   lsx:  Number to be divided.
**
**   lubNumBits:  Number of bits to shift it right.
*/

#ifdef COSTELLA_SHIFT_RIGHT_IS_SIGNED

  #define COSTELLA_SHIFT_RIGHT_FLOOR( lsx, lubNumBits ) \
    ( (lsx) >> (lubNumBits) )

#else

  #define COSTELLA_SHIFT_RIGHT_FLOOR( lsx, lubNumBits ) \
    ( (lsx) < 0 ? -1 - ( ( -1 - (lsx) ) >> (lubNumBits) ) : (lsx) >> \\
      (lubNumBits) )

#endif



/* Function definition, flow, and error-handling procedures. Start with the
** error node structure.
*/

typedef struct COSTELLA_ERROR_NODE_tag
{
  COSTELLA_B bAllocated;
  COSTELLA_C* acFunctionName, * acError, * acCleanupError;
  struct COSTELLA_ERROR_NODE_tag* penNext, * penCleanupNext;
}
COSTELLA_ERROR_NODE;



/* COSTELLA_FUNCTION:
**
**   Declare a function. Definition of such a function requires that 
**   costella_body.h be first included, which redefines this macro.
*/

#define COSTELLA_FUNCTION( lfFunctionName, larglist ) \
  COSTELLA_ERROR_NODE* lfFunctionName larglist;



/* COSTELLA_END_FUNCTION:
**
**   End a function.
*/

#define COSTELLA_END_FUNCTION \
      \
      COSTELLA_DELETE_DAUGHTERS; \
      return 0; \
    } \
    i_l_bCostellaError++; \
  }



/* COSTELLA_ANSI_FUNCTION:
**
**   Declare an "ANSI" function, i.e. one that does not follow the function
**   paradigm of the Costella libraries, but which must call such functions
**   and be able to use the error-handling functionality built into them. 
**   Definition of such a function requires that costella_body.h be first 
**   included, which redefines this macro.
*/

#define COSTELLA_ANSI_FUNCTION( lfFunctionName, returntype, larglist ) \
  returntype lfFunctionName larglist;



/* COSTELLA_END_ANSI_FUNCTION:
**
**   End a function.
*/

#define COSTELLA_END_ANSI_FUNCTION( returnvalue ) \
      \
      COSTELLA_DELETE_DAUGHTERS; \
      return (returnvalue); \
    } \
    i_l_bCostellaError++; \
  }



/* COSTELLA_END_ANSI_FUNCTION_NO_RETURN_VALUE:
**
**   End a function that has no return value.
*/

#define COSTELLA_END_ANSI_FUNCTION_NO_RETURN_VALUE \
      \
      COSTELLA_DELETE_DAUGHTERS; \
      return; \
    } \
    i_l_bCostellaError++; \
  }



/* COSTELLA_FUNCTION_POINTER:
**
**   Declare or define a pointer to a COSTELLA_FUNCTION.
*/

#define COSTELLA_FUNCTION_POINTER( lpf, larglist ) \
  COSTELLA_ERROR_NODE* (*lpf) larglist



/* COSTELLA_RETURN:
**
**   Return from a function. If there has been no error, return null. 
**   Otherwise, try to allocate memory for a copy of the error node, and
**   copy it. If the allocation fails, use the static automatic copy, but
**   delete its daughter nodes to avoid re-entrant problems.
*/

#define COSTELLA_RETURN \
{ \
  if( i_l_bCostellaError ) \
  { \
    COSTELLA_ERROR_NODE* i_l_pen = 0; \
    \
    if( COSTELLA_MALLOC( i_l_pen, 1 ) ) \
    { \
      COSTELLA_DELETE_DAUGHTERS; \
      i_l_enCostella.bAllocated = COSTELLA_FALSE; \
      return &i_l_enCostella; \
    } \
    else \
    { \
      *i_l_pen = i_l_enCostella; \
      i_l_pen->bAllocated = COSTELLA_TRUE; \
      return i_l_pen; \
    } \
  } \
  COSTELLA_DELETE_DAUGHTERS; \
  return 0; \
} \
 


/* COSTELLA_ANSI_RETURN:
**
**   Return from an ANSI function that has a non-void return type.
*/

#define COSTELLA_ANSI_RETURN( returnvalue ) \
  \
  COSTELLA_DELETE_DAUGHTERS; \
  return (returnvalue); \



/* COSTELLA_ANSI_RETURN_NO_VALUE:
**
**   Return from an ANSI function that has a void return type.
*/

#define COSTELLA_ANSI_RETURN_NO_VALUE \
  \
  COSTELLA_DELETE_DAUGHTERS; \
  return; \



/* COSTELLA_FUNDAMENTAL_ERROR:
**
**   Macro for registering a fundamental error, i.e., one that has no 
**   daughter error tree.
*/

#define COSTELLA_FUNDAMENTAL_ERROR( lacError ) \
  COSTELLA_DELETE_DAUGHTERS; \
  COSTELLA_ERROR( lacError ); \



/* COSTELLA_CALL:
**
**   Macro for calling a function and automatically handling the error
**   structure.
*/

#define COSTELLA_CALL( lcall ) \
  ( COSTELLA_DELETE_DAUGHTERS, i_l_enCostella.penNext = lcall )



/* COSTELLA_ERROR:
**
**   Macro for registering an error string after a COSTELLA_CALL().
*/

#define COSTELLA_ERROR( lacError ) \
  i_l_bCostellaError = COSTELLA_TRUE; \
  i_l_enCostella.acError = lacError; 
 


/* COSTELLA_CLEANUP_FUNDAMENTAL_ERROR:
**
**   Macro for registering a fundamental error encountered during cleanup.
*/

#define COSTELLA_CLEANUP_FUNDAMENTAL_ERROR( lacError ) \
  COSTELLA_DELETE_CLEANUP_DAUGHTER; \
  COSTELLA_CLEANUP_ERROR( lacError ); 



/* COSTELLA_CLEANUP_CALL:
**
**   Macro for calling a function while cleaning up from another error.
*/

#define COSTELLA_CLEANUP_CALL( lcall ) \
  ( COSTELLA_DELETE_CLEANUP_DAUGHTER, i_l_enCostella.penCleanupNext = \
  lcall )



/* COSTELLA_CLEANUP_ERROR:
**
**   Macro for registering an error string after a COSTELLA_CLEANUP_CALL().
*/

#define COSTELLA_CLEANUP_ERROR( lacError ) \
  i_l_bCostellaError = COSTELLA_TRUE; \
  i_l_enCostella.acCleanupError = lacError; \



/* COSTELLA_DELETE_DAUGHTERS:
**
**   Macro used internally to delete the daughter error trees if they have
**   been allocated.
*/

#define COSTELLA_DELETE_DAUGHTERS \
  ( i_l_enCostella.penNext ? (void) costella_base_error_node_delete( \
  &i_l_enCostella.penNext ) : (void) COSTELLA_DELETE_CLEANUP_DAUGHTER )



/* COSTELLA_DELETE_CLEANUP_DAUGHTER:
**
**   Macro used internally to delete the cleanup daughter error tree if it
**   has been allocated.
*/

#define COSTELLA_DELETE_CLEANUP_DAUGHTER \
  ( i_l_enCostella.penCleanupNext ? (void) \
  costella_base_error_node_delete( &i_l_enCostella.penCleanupNext ) : \
  (void) 0 )



/* COSTELLA_IGNORE_ERRORS:
**
**   Macro for specifying that trapped errors should be ignored.
*/

#define COSTELLA_IGNORE_ERRORS \
  COSTELLA_DELETE_DAUGHTERS



/* COSTELLA_IGNORE_CLEANUP_ERROR:
**
**   Macro for specifying that a trapped cleanup error should be ignored.
*/

#define COSTELLA_IGNORE_CLEANUP_ERROR \
  COSTELLA_DELETE_CLEANUP_DAUGHTER



/* COSTELLA_ERROR_ROOT_NODE:
**
**   Alias for the root node of the error tree.
*/

#define COSTELLA_ERROR_ROOT_NODE \
  (i_l_enCostella.penNext)



/* COSTELLA_ERROR_FPRINT:
**
**   Print the error chain to a file.
*/

#define COSTELLA_ERROR_FPRINT( pfile ) \
  costella_base_error_fprint( (pfile), COSTELLA_ERROR_ROOT_NODE, 0, 0 )



/* COSTELLA_ERROR_APP_FPRINT:
**
**   Print the error chain to a file, with application information.
*/

#define COSTELLA_ERROR_APP_FPRINT( pfile, lacApplicationName, \
    lacApplicationError ) \
  costella_base_error_fprint( (pfile), COSTELLA_ERROR_ROOT_NODE, \
    (lacApplicationName), (lacApplicationError) )



/* Function prototypes.
*/

COSTELLA_FUNCTION( CostellaBaseInitialize, ( COSTELLA_O* (*pfMalloc)( 
  COSTELLA_MC mc ), COSTELLA_O* (*pfRealloc)( COSTELLA_O* po, COSTELLA_MC mc
  ), COSTELLA_B (*pfFree)( COSTELLA_O* po ) ) )
COSTELLA_FUNCTION( CostellaBaseFinalize, ( void ) )

void costella_base_error_fprint( FILE* pfile, COSTELLA_ERROR_NODE* penRoot,
  COSTELLA_C* acApplicationName, COSTELLA_C* acApplicationError );
void costella_base_sprint_error_tree( COSTELLA_C* ac, COSTELLA_ERROR_NODE* 
  penRoot );
void costella_base_error_node_delete( COSTELLA_ERROR_NODE** ppen );



/* Callback function typedef.
*/

typedef COSTELLA_ERROR_NODE* (*COSTELLA_CALLBACK_FUNCTION)( COSTELLA_O*
  poPassback );



/* Declaration of global variables defined in costella_base.c.
*/

#ifndef _COSTELLA_BASE_C_

  extern COSTELLA_O* (*gpfCostellaBaseMalloc)( COSTELLA_MC mc );
  extern COSTELLA_O* (*gpfCostellaBaseRealloc)( COSTELLA_O* po, COSTELLA_MC 
    mc );
  extern COSTELLA_B (*gpfCostellaBaseFree)( COSTELLA_O* po );

#endif



/* File is now included.
*/

#endif



/* Copyright (c) 1989-2007 John P. Costella.
**
** End of file.
*/
