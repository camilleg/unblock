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
** costella_image.c: 
**
**   Generic image manipulation library for the Costella libraries.
**
**   Library begun: December 30, 2005.
**   This version: November 12, 2007.
**
**   Written in ANSI C.
*/



/* Include files.
*/

#include "costella_image.h"



/* Flag preventing the redeclaration of the variables defined below.
*/

#define _COSTELLA_IMAGE_C_



/* Externally visible global variables.
*/

COSTELLA_IMAGE giCostellaImageNull;
COSTELLA_IMAGE_GRAY_PIXEL gigpCostellaImageNull;
COSTELLA_IMAGE_ALPHA_PIXEL giapCostellaImageNull;
COSTELLA_IMAGE_COLOR_PIXEL gicpCostellaImageNull;



/* Internal global variables.
*/

static COSTELLA_B gbInitialized = COSTELLA_FALSE;



/* Start function bodies.
*/

#include "costella_body.h"



/* costella_image_initialize:
**
**   Public interface for initializing this library. 
*/

COSTELLA_ANSI_FUNCTION( costella_image_initialize, int, ( FILE* pfileError )
  )
{
  if( COSTELLA_CALL( CostellaImageInitialize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_image_finalize:
**
**   Public interface for finalizing this library. 
*/

COSTELLA_ANSI_FUNCTION( costella_image_finalize, int, ( FILE* pfileError ) )
{
  if( COSTELLA_CALL( CostellaImageFinalize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* CostellaImageInitialize: 
**
**   Initialize the library. 
*/

COSTELLA_FUNCTION( CostellaImageInitialize, ( void ) )
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
    COSTELLA_ERROR( "Initializing" );
    gbInitialized = COSTELLA_FALSE;
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaImageFinalize: 
**
**   Finalize the library. 
*/

COSTELLA_FUNCTION( CostellaImageFinalize, ( void ) )
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



/* Copyright (c) 2005-2007 John P. Costella.
**
** End of file.
*/
