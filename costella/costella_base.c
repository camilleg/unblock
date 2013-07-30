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
** costella_base.c: 
**  
**   General functionality for the Costella libraries.
**
**   Library begun: December 12, 1989.
**   This version: October 10, 2007.
**
**   Written in ANSI C.
*/



/* Flag to ensure correct declaration of global variables defined below.
*/

#define _COSTELLA_BASE_C_



/* Include file.
*/

#include "costella_base.h"
#include <string.h>



/* Internal function prototypes.
*/

static COSTELLA_O* costella_base_malloc_ansi( COSTELLA_MC mc );
static COSTELLA_O* costella_base_realloc_ansi( COSTELLA_O* po, COSTELLA_MC 
  mc );
static COSTELLA_B costella_base_free_ansi( COSTELLA_O* po );

static void costella_base_error_fprint_tree( FILE* pfile, 
  COSTELLA_ERROR_NODE* pen, COSTELLA_UD udDepth, COSTELLA_UD udCleanup );
static void costella_base_sprint_error_tree_recurse( COSTELLA_C* ac, 
  COSTELLA_ERROR_NODE* pen, COSTELLA_UD udDepth, COSTELLA_UD udCleanup );



/* Publicly available global variables.
*/

COSTELLA_O* (*gpfCostellaBaseMalloc)( COSTELLA_MC mc ) = 
  costella_base_malloc_ansi;
COSTELLA_O* (*gpfCostellaBaseRealloc)( COSTELLA_O* po, COSTELLA_MC mc ) =
  costella_base_realloc_ansi;
COSTELLA_B (*gpfCostellaBaseFree)( COSTELLA_O* po ) =
  costella_base_free_ansi;



/* Internal global variables.
*/

static COSTELLA_B gbInitialized = COSTELLA_FALSE;



/* Start function bodies.
*/

#include "costella_body.h"



/* costella_base_initialize:
**
**   Public interface for initializing this library. 
**
**   pf{Malloc,Realloc,Free}:  Pointer to function to be used for 
**     {allocating, reallocating, freeing} memory.
*/

COSTELLA_ANSI_FUNCTION( costella_base_initialize, int, ( void* (*pfMalloc)( 
  unsigned int uiNumBytes ), void* (*pfRealloc)( void* pv, unsigned int 
  uiNewNumBytes ), int (*pfFree)( void* pv ), FILE* pfileError ) )
{
  if( COSTELLA_CALL( CostellaBaseInitialize( pfMalloc, pfRealloc, pfFree ) )
    )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* costella_base_finalize:
**
**   Public interface for finalizing this library. Should not usually be
**   used, becausee there is nothing in this library that is useful by
**   itself. Included here for completeness.
*/

COSTELLA_ANSI_FUNCTION( costella_base_finalize, int, ( FILE* pfileError ) )
{
  if( COSTELLA_CALL( CostellaBaseFinalize() ) )
  {
    COSTELLA_ERROR_FPRINT( pfileError );
    COSTELLA_ANSI_RETURN( 0 );
  } 
}
COSTELLA_END_ANSI_FUNCTION( !0 )



/* CostellaBaseInitialize: 
**
**   Initialize the library. Repeated initialization is harmless.
**
**   pfMalloc:  Function to be used by all Costella libraries to allocate 
**     memory. Must return a pointer to the memory block, or null if the 
**     allocation fails.
**
**   pfRealloc:  Function to be used by all Costella libraries to reallocate
**     memory allocated by pfMalloc.
**
**   pfFree:  Function to be used by all Costella libraries to free memory 
**     allocated by pfMalloc.
*/

COSTELLA_FUNCTION( CostellaBaseInitialize, ( COSTELLA_O* (*pfMalloc)( 
  COSTELLA_MC mc ), COSTELLA_O* (*pfRealloc)( COSTELLA_O* po, COSTELLA_MC mc
  ), COSTELLA_B (*pfFree)( COSTELLA_O* po ) ) )
{  
  COSTELLA_UB ubSizeMc, ubSizeMi;


  /* Do nothing if already initialized.
  */

  if( gbInitialized )
  {
    COSTELLA_RETURN;
  }


  /* Set initialized flag.
  */

  gbInitialized = COSTELLA_TRUE;


  /* Store the function pointers for memory allocation, reallocation, and 
  ** freeing. If these pointers are null, use the default ANSI functions
  */

  if( !pfMalloc || !pfRealloc || !pfFree )
  {
    gpfCostellaBaseMalloc = costella_base_malloc_ansi;
    gpfCostellaBaseRealloc = costella_base_realloc_ansi;
    gpfCostellaBaseFree = costella_base_free_ansi;
  }
  else
  {
    gpfCostellaBaseMalloc = pfMalloc;
    gpfCostellaBaseRealloc = pfRealloc;
    gpfCostellaBaseFree = pfFree;
  }


  /* Check that we have at least 32-bit memory addressing.
  */

  ubSizeMc = (COSTELLA_UB) sizeof( COSTELLA_MC );
  ubSizeMi = (COSTELLA_UB) sizeof( COSTELLA_MI );

  if( ubSizeMc < 4 || ubSizeMi < 4 )
  {
    gbInitialized = COSTELLA_FALSE;
    COSTELLA_FUNDAMENTAL_ERROR( "Memory model has less than 32 bits" );
    COSTELLA_RETURN;
  }
}
COSTELLA_END_FUNCTION



/* CostellaBaseFinalize: 
**
**   Finalize the library. 
*/

COSTELLA_FUNCTION( CostellaBaseFinalize, ( void ) )
{
  /* Do nothing if not initialized. 
  */

  if( !gbInitialized )
  {
    COSTELLA_RETURN;
  }


  /* Clear initialization flag.
  */

  gbInitialized = COSTELLA_FALSE;


  /* Do something useless with hidden variable to keep compilers happy.
  */

  i_l_enCostella.penNext = 0;
}
COSTELLA_END_FUNCTION



/* costella_base_malloc_ansi: 
**
**   Memory allocation function, assuming nothing more than ANSI C.
**
**   mc:  Number of bytes to allocate.
**
**   Returns pointer to the allocated memory, or null on failure.
*/

#include <stdlib.h>

static COSTELLA_O* costella_base_malloc_ansi( COSTELLA_MC mc )
{
  return malloc( mc );  
}



/* costella_base_realloc_ansi: 
**
**   Memory reallocation function, assuming nothing more than ANSI C.
**
**   po:  Pointer to memory previously allocated using 
**     costella_base_malloc_ansi().
**
**   mc:  New number of bytes to reallocate.
**
**   Returns pointer to the reallocated memory, or null on failure.
*/

static COSTELLA_O* costella_base_realloc_ansi( COSTELLA_O* po, COSTELLA_MC 
  mc )
{
  return realloc( po, mc );  
}



/* costella_base_free_ansi: 
**
**   Memory freeing function, assuming nothing more than ANSI C.
**
**   po:  Pointer to memory previously allocated using 
**     costella_base_malloc_ansi() or costella_base_realloc_ansi().
**
**   Always returns nonzero, indicating no error.
*/

static COSTELLA_B costella_base_free_ansi( COSTELLA_O* po )
{
  free( po );  

  return !0;
}



/* costella_base_error_fprint:
**
**   Print an error tree to a file.
**
**   pfile:  Pointer to the ANSI file to write to. If null, then do nothing.
**
**   penRoot:  Pointer to the root COSTELLA_ERROR_NODE.
**
**   acApplicationName:  String pointer for an application name. If null, no
**     application header is written, only the error tree.
**
**   acApplicationError:  String pointer for an application error 
**     description. If null, no application header is written, only the 
**     error tree.
*/

void costella_base_error_fprint( FILE* pfile, COSTELLA_ERROR_NODE* penRoot,
  COSTELLA_C* acApplicationName, COSTELLA_C* acApplicationError )
{
  /* If the file pointer is null, do nothing.
  */

  if( !pfile )
  {
    return;
  }


  /* Print the application information if it has been supplied.
  */

  if( acApplicationName && acApplicationError )
  {
    if( fprintf( pfile, "\nERROR:\n=====\nApplication name: "
      "%s\nApplication error: %s\n\n", acApplicationName, acApplicationError
      ) < 0 )
    {
      return;
    }
  }


  /* If there is no root error node, do nothing more.
  */

  if( !penRoot )
  {
    return;
  }


  /* Print a spacer line if we had no application information.
  */

  if( !acApplicationName || !acApplicationError )
  {
    if( fprintf( pfile, "\n" ) < 0 )
    {
      return;
    }
  }


  /* Print the error tree header.
  */

  if( !acApplicationName || !acApplicationError )
  {
    if( fprintf( pfile, 
      "Costella library error tree:\n"
      "----------------------------\n" ) < 0 )
    {
      return;
    }
  }
  else
  {
    if( fprintf( pfile, 
      "AN ERROR OCCURRED: This is the costella library error tree:\n"
      "-----------------------------------------------------------\n" ) < 0
      )
    {
      return;
    }
  }


  /* Print the error tree recursively.
  */

  costella_base_error_fprint_tree( pfile, penRoot, 0, 0 );
}



/* costella_base_error_fprint_tree:
**
**   Internal, recursive function for printing an error tree to a file.
**
**   pfile:  Pointer to the FILE to print to.
**
**   pen:  Pointer to the current COSTELLA_ERROR_NODE.
**
**   udDepth:  Current depth.
**
**   udCleanup:  Current cleanup level.
*/

static void costella_base_error_fprint_tree( FILE* pfile, 
  COSTELLA_ERROR_NODE* pen, COSTELLA_UD udDepth, COSTELLA_UD udCleanup )
{
  COSTELLA_UD ud;


  /* Check pointers.
  */

  if( !pfile || !pen )
  {
    return;
  }


  /* Print the indenting spaces.
  */

  for( ud = 0; ud < udDepth; ud++ )
  {
    if( fprintf( pfile, " " ) < 0 )
    {
      return;
    }
  }


  /* Print the opening brackets for existing cleanup mode.
  */

  for( ud = 0; ud < udCleanup; ud++ )
  {
    if( fprintf( pfile, "[" ) < 0 )
    {
      return;
    }
  }


  /* Print the function name and error string.
  */

  if( fprintf( pfile, "%s: %s", pen->acFunctionName, pen->acError ? 
    pen->acError : "CODING ERROR! Main error message not set before "
    "cleanup!" ) < 0 )
  {
    return;
  }


  /* Print the closing brackets for existing cleanup mode.
  */

  for( ud = 0; ud < udCleanup; ud++ )
  {
    if( fprintf( pfile, "]" ) < 0 )
    {
      return;
    }
  }


  /* Finish the line.
  */

  if( fprintf( pfile, "\n" ) < 0 )
  {
    return;
  }


  /* Check if this node has a next node.
  */

  if( pen->penNext )
  {
    /* It does. Do it.
    */

    costella_base_error_fprint_tree( pfile, pen->penNext, udDepth + 1, 
      udCleanup );
  }


  /* Check if this node has a cleanup string, or if the node itself could 
  ** not be allocated.
  */

  if( pen->acCleanupError || !pen->bAllocated )
  {
    /* It does. Increment cleanup level.
    */

    udCleanup++;


    /* Print the indenting spaces.
    */

    for( ud = 0; ud < udDepth; ud++ )
    {
      if( fprintf( pfile, " " ) < 0 )
      {
        return;
      }
    }


   /* Print the opening brackets for cleanup mode.
    */

    for( ud = 0; ud < udCleanup; ud++ )
    {
      if( fprintf( pfile, "[" ) < 0 )
      {
        return;
      }
    }


    /* Print the function name and cleanup error string.
    */

    if( pen->bAllocated )
    {
      if( fprintf( pfile, "%s: %s", pen->acFunctionName, pen->acCleanupError
        ) < 0 )
      {
        return;
      }
    }
    else
    {
      if( fprintf( pfile, "%s: OUT OF MEMORY! Daughter error trees "
        "deleted!", pen->acFunctionName ) < 0 )
      {
        return;
      }
    }


    /* Print the closing brackets for cleanup mode.
    */

    for( ud = 0; ud < udCleanup; ud++ )
    {
      if( fprintf( pfile, "]" ) < 0 )
      {
        return;
      }
    }


    /* Finish the line.
    */

    if( fprintf( pfile, "\n" ) < 0 )
    {
      return;
    }


    /* Check if this link has a next cleanup link.
    */

    if( pen->penCleanupNext )
    {
      /* It does. Do it.
      */

      costella_base_error_fprint_tree( pfile, pen->penCleanupNext, udDepth +
        1, udCleanup );
    }
  }
}



/* costella_base_sprint_error_tree:
**
**   Print the error tree to a string.
**
**   ac:  Pointer to string.
**
**   penRoot:  Pointer to the root COSTELLA_ERROR_NODE.
*/

void costella_base_sprint_error_tree( COSTELLA_C* ac, COSTELLA_ERROR_NODE* 
  penRoot )
{
  /* Check pointers.
  */

  if( !ac )
  {
    return;
  }


  /* Start the string.
  */

  *ac = 0;


  /* Call the recursive function.
  */

  if( penRoot )
  {
    costella_base_sprint_error_tree_recurse( ac, penRoot, 0, 0 );
  }
}



/* costella_base_sprint_error_tree_recurse:
**
**   Internal recursive function for printing the error tree to a string.
**
**   ac:  Pointer to the preallocated string, containing the error tree so 
**     far.
**
**   pen:  Pointer to the current COSTELLA_ERROR_NODE.
**
**   udDepth:  Current error depth.
**
**   udCleanup:  Current cleanup level.
*/

static void costella_base_sprint_error_tree_recurse( COSTELLA_C* ac, 
  COSTELLA_ERROR_NODE* pen, COSTELLA_UD udDepth, COSTELLA_UD udCleanup )
{
  COSTELLA_C acScratch[ 256 ];
  COSTELLA_UD ud;


  /* Check pointers.
  */

  if( !ac || !pen )
  {
    return;
  }


  /* Print the indenting spaces.
  */

  for( ud = 0; ud < udDepth; ud++ )
  {
    if( sprintf( acScratch, " " ) < 0 )
    {
      return;
    }

    strcat( ac, acScratch );
  }


  /* Print the opening brackets for existing cleanup mode.
  */

  for( ud = 0; ud < udCleanup; ud++ )
  {
    if( sprintf( acScratch, "[" ) < 0 )
    {
      return;
    }

    strcat( ac, acScratch );
  }


  /* Print the function name and error string.
  */

  if( sprintf( acScratch, "%s: %s", pen->acFunctionName, pen->acError ? 
    pen->acError : "CODING ERROR! Main error message not set before "
    "cleanup!" ) < 0 )
  {
    return;
  }

  strcat( ac, acScratch );


  /* Print the closing brackets for existing cleanup mode.
  */

  for( ud = 0; ud < udCleanup; ud++ )
  {
    if( sprintf( acScratch, "]" ) < 0 )
    {
      return;
    }

    strcat( ac, acScratch );
  }


  /* Finish the line.
  */

  if( sprintf( acScratch, "\n" ) < 0 )
  {
    return;
  }

  strcat( ac, acScratch );
  

  /* Check if this node has a next node.
  */

  if( pen->penNext )
  {
    /* It does. Do it.
    */

    costella_base_sprint_error_tree_recurse( ac, pen->penNext, udDepth + 1, 
      udCleanup );
  }


  /* Check if this node has a cleanup string, or if the node itself could 
  ** not be allocated.
  */

  if( pen->acCleanupError || !pen->bAllocated )
  {
    /* It does. Increment cleanup level.
    */

    udCleanup++;


    /* Print the indenting spaces.
    */

    for( ud = 0; ud < udDepth; ud++ )
    {
      if( sprintf( acScratch, " " ) < 0 )
      {
        return;
      }

      strcat( ac, acScratch );
    }


    /* Print the opening brackets for cleanup mode.
    */

    for( ud = 0; ud < udCleanup; ud++ )
    {
      if( sprintf( acScratch, "[" ) < 0 )
      {
        return;
      }

      strcat( ac, acScratch );
    }


    /* Print the function name and cleanup error string.
    */

    if( pen->bAllocated )
    {
      if( sprintf( acScratch, "%s: %s", pen->acFunctionName, 
        pen->acCleanupError ) < 0 )
      {
        return;
      }

      strcat( ac, acScratch );
    }
    else
    {
      if( sprintf( acScratch, "%s: OUT OF MEMORY! Daughter error trees "
        "deleted!", pen->acFunctionName ) < 0 )
      {
        return;
      }

      strcat( ac, acScratch );
    }


    /* Print the closing brackets for cleanup mode.
    */

    for( ud = 0; ud < udCleanup; ud++ )
    {
      if( sprintf( acScratch, "]" ) < 0 )
      {
        return;
      }

      strcat( ac, acScratch );
    }


    /* Finish the line.
    */

    if( sprintf( acScratch, "\n" ) < 0 )
    {
      return;
    }

    strcat( ac, acScratch );


    /* Check if this link has a next cleanup link.
    */

    if( pen->penCleanupNext )
    {
      /* It does. Do it.
      */

      costella_base_sprint_error_tree_recurse( ac, pen->penCleanupNext, 
        udDepth + 1, udCleanup );
    }
  }
}



/* costella_base_error_node_delete:
**
**   Delete a COSTELLA_ERROR_NODE.
**
**   ppen:  Pointer to pointer to the COSTELLA_ERROR_NODE to delete.
*/

void costella_base_error_node_delete( COSTELLA_ERROR_NODE** ppen )
{
  COSTELLA_ERROR_NODE* pen;


  /* Check pointer.
  */

  if( !ppen )
  {
    return;
  }


  /* Extract pointer.
  */

  pen = *ppen;


  /* If the pointer is null, do nothing.
  */

  if( !pen )
  {
    return;
  }


  /* Free its daughters.
  */

  costella_base_error_node_delete( &pen->penNext );
  costella_base_error_node_delete( &pen->penCleanupNext );


  /* Check if it was allocated.
  */

  if( pen->bAllocated )
  {
    /* It was. Free it.
    */

    COSTELLA_FREE( *ppen );
  }
}



/* Copyright (c) 1989-2007 John P. Costella.
**
** End of file.
*/
