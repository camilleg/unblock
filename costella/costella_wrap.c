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
** costella_wrap.c: 
**  
**   Wrapping of callback functions for public interface.
**
**   Library begun: December 12, 2006.
**   This version: July 11, 2007.
**
**   Written in ANSI C.
*/



/* Include file.
*/

#include "costella_wrap.h"



/* Internal global variables.
*/

static COSTELLA_B gbInitialized = COSTELLA_FALSE;



/* Start function bodies.
*/

#include "costella_body.h"



/* CostellaWrapInitialize: 
**
**   Initialize the library.
*/

COSTELLA_FUNCTION( CostellaWrapInitialize, ( void ) )
{  
  /* Do nothing if already initialized.
  */

  if( gbInitialized )
  {
    COSTELLA_RETURN;
  }


  /* Set initialized flag.
  */

  gbInitialized = COSTELLA_TRUE;


  /* Initialize other libraries.
  */

  if( COSTELLA_CALL( CostellaBaseInitialize( 0, 0, 0 ) ) )
  {
    COSTELLA_ERROR( "Initialization" );
    gbInitialized = COSTELLA_FALSE;
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapFinalize: 
**
**   Finalize the library. 
*/

COSTELLA_FUNCTION( CostellaWrapFinalize, ( void ) )
{
  /* Do nothing if not initialized.
  */

  if( !gbInitialized )
  {
    COSTELLA_RETURN;
  }


  /* Clear initialized flag.
  */

  gbInitialized = COSTELLA_FALSE;


  /* Finalize other libraries.
  */ 

  if( COSTELLA_CALL( CostellaBaseFinalize() ) )
  {
    COSTELLA_ERROR( "Finalizing" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapCallbackNew:
**
**   Create a new COSTELLA_WRAP_CALLBACK object.
**
**   ppwc:  Pointer to receiving pointer for the new COSTELLA_WRAP_CALLBACK
**     object.
**
**   pfCallback:  Pointer to the ANSI callback function.
**
**   pvPassback:  Pointer to the ANSI passback object.
*/

COSTELLA_FUNCTION( CostellaWrapCallbackNew, ( COSTELLA_WRAP_CALLBACK** ppwc,
  int (*pfCallback)( void* pvPassback, void* pv ), void* pvPassback ) )
{
  COSTELLA_WRAP_CALLBACK* pwc = 0;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !ppwc )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null ppwc" );
      COSTELLA_RETURN;
    }

    if( *ppwc )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Used ppwc" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Create the object.
  */

  if( COSTELLA_MALLOC( pwc, 1 ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Allocating" );
    COSTELLA_RETURN;
  }


  /* Store pointer.
  */

  *ppwc = pwc;


  /* Store information.
  */

  pwc->pfCallback = pfCallback;
  pwc->pvPassback = pvPassback;
}
COSTELLA_END_FUNCTION



/* CostellaWrapCallbackDelete:
**
**   Delete a COSTELLA_WRAP_CALLBACK object.
**
**   ppwp:  Pointer to pointer to the COSTELLA_WRAP_CALLBACK object.
*/

COSTELLA_FUNCTION( CostellaWrapCallbackDelete, ( COSTELLA_WRAP_CALLBACK** 
  ppwc ) )
{
  COSTELLA_WRAP_CALLBACK* pwc;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !ppwc )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null ppwc" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Extract pointer.
  */

  pwc = *ppwc;


  /* If the pointer is null, do nothing.
  */

  if( !pwc )
  {
    COSTELLA_RETURN;
  }


  /* Free the memory.
  */

  if( COSTELLA_FREE( *ppwc ) )
  {
    COSTELLA_FUNDAMENTAL_ERROR( "Freeing object" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapCallback:
**
**   Callback function for wrapping an ANSI callback function.
*/

COSTELLA_FUNCTION( CostellaWrapCallback, ( COSTELLA_O* poPassback, 
  COSTELLA_O* po ) )
{
  COSTELLA_WRAP_CALLBACK* pwc;
  int (*pfCallback)( void* pvPassback, void* pv );
  void* pvPassback;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !poPassback )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null poPassback" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Cast pointer.
  */

  pwc = (COSTELLA_WRAP_CALLBACK*) poPassback;


  /* Extract information.
  */

  pfCallback = pwc->pfCallback;
  pvPassback = pwc->pvPassback;


  /* Now call the ANSI callback function.
  */

  if( pfCallback && !pfCallback( pvPassback, (void*) po ) )
  {
    COSTELLA_ERROR( "Abort signaled by ANSI callback function; no further "
      "information available through this channel" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapProgress:
**
**   Callback function for wrapping an ANSI progress callback function.
*/

COSTELLA_FUNCTION( CostellaWrapProgress, ( COSTELLA_O* poPassback ) )
{
  COSTELLA_WRAP_PROGRESS* pwp;
  int (*pfProgress)( void* pvPassback );
  void* pvPassback;


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !poPassback )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null poPassback" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Cast pointer.
  */

  pwp = (COSTELLA_WRAP_PROGRESS*) poPassback;


  /* Extract information.
  */

  pfProgress = pwp->pfProgress;


  /* If there is no progress callback function, do nothing more.
  */

  if( !pfProgress )
  {
    COSTELLA_RETURN;
  }


  /* Extract information.
  */

  pvPassback = pwp->pvPassback;


  /* Now call the ANSI progress callback function.
  */

  if( !pfProgress( pvPassback ) )
  {
    COSTELLA_ERROR( "Progress aborted by ANSI application's progress "
      "function; no further information available through this channel" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapNewCopy:
**
**   Callback function for wrapping an ANSI copy creator.
*/

COSTELLA_FUNCTION( CostellaWrapNewCopy, ( COSTELLA_O** 
  ppoNewCopy, COSTELLA_O* poExisting, COSTELLA_O* poPassback ) )
{
  COSTELLA_WRAP_FUNCTION_POINTER* pwfp;
  void* (*pfNewCopy)( void* pvExisting );


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !ppoNewCopy )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null ppoNewCopy" );
      COSTELLA_RETURN;
    }

    if( *ppoNewCopy )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Used ppoNewCopy" );
      COSTELLA_RETURN;
    }

    if( !poExisting )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null poExisting" );
      COSTELLA_RETURN;
    }

    if( !poPassback )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null poPassback" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Cast pointer.
  */

  pwfp = (COSTELLA_WRAP_FUNCTION_POINTER*) poPassback;


  /* Extract function pointer.
  */

  pfNewCopy = (void* (*)( void* )) pwfp->pf;


  /* Now call the ANSI copy creator.
  */

  if( !( *ppoNewCopy = pfNewCopy( (void*) poExisting ) ) )
  {
    COSTELLA_ERROR( "ANSI application's copy creator failed; no further "
      "information available through this channel" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapDelete:
**
**   Callback function for wrapping an ANSI destructor.
*/

COSTELLA_FUNCTION( CostellaWrapDelete, ( COSTELLA_O** ppo, COSTELLA_O* 
  poPassback ) )
{
  COSTELLA_O* po;
  COSTELLA_WRAP_FUNCTION_POINTER* pwfp;
  int (*pfDelete)( void* pv );


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !ppo )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null ppo" );
      COSTELLA_RETURN;
    }

    if( !poPassback )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null poPassback" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Extract object pointer.
  */

  po = *ppo;


  /* If it is null, do nothing.
  */

  if( !po )
  {
    COSTELLA_RETURN;
  }


  /* Cast pointer.
  */

  pwfp = (COSTELLA_WRAP_FUNCTION_POINTER*) poPassback;


  /* Extract function pointer.
  */

  pfDelete = (int (*)( void* )) pwfp->pf;


  /* Now call the ANSI destructor.
  */

  if( !pfDelete( (void*) po ) )
  {
    COSTELLA_ERROR( "ANSI application's destructor failed; no further "
      "information available through this channel" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaWrapCompare:
**
**   Callback function for wrapping an ANSI comparator.
*/

COSTELLA_FUNCTION( CostellaWrapCompare, ( COSTELLA_SB* psbCompare, 
  COSTELLA_O* po1, COSTELLA_O* po2, COSTELLA_O* poPassback ) )
{
  COSTELLA_WRAP_FUNCTION_POINTER* pwfp;
  int (*pfCompare)( void* pv1, void* pv2 );


  /* Check initialization and pointers.
  */

  #ifdef COSTELLA_DEBUG
  {
    if( !gbInitialized )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Initialization" );
      COSTELLA_RETURN;
    }

    if( !psbCompare )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null psbCompare" );
      COSTELLA_RETURN;
    }

    if( !po1 )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null po1" );
      COSTELLA_RETURN;
    }

    if( !po2 )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null po2" );
      COSTELLA_RETURN;
    }

    if( !poPassback )
    {
      COSTELLA_FUNDAMENTAL_ERROR( "Null poPassback" );
      COSTELLA_RETURN;
    }
  }
  #endif


  /* Cast pointer.
  */

  pwfp = (COSTELLA_WRAP_FUNCTION_POINTER*) poPassback;


  /* Extract function pointer.
  */

  pfCompare = (int (*)( void*, void* )) pwfp->pf;


  /* Now call the ANSI comparator.
  */

  *psbCompare = (COSTELLA_SB) pfCompare( (void*) po1, (void*) po2 );
}
COSTELLA_END_FUNCTION


  
/* Copyright (c) 2006-2007 John P. Costella.
**
** End of file.
*/
