/* Copyright (c) 2007 John P. Costella.
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
** costella_debug_default.h: 
**  
**   Header file setting the COSTELLA_DEBUG flag if the symbol _DEBUG is
**   defined.
**
**   This version: March 28, 2007.
**
**   Written in ANSI C.
*/



/* Include file only once.
*/

#ifndef _COSTELLA_DEBUG_DEFAULT_H_
#define _COSTELLA_DEBUG_DEFAULT_H_



/* Set flag.
*/

#ifdef _DEBUG

  #define COSTELLA_DEBUG 1

#else

  #undef COSTELLA_DEBUG

#endif



/* File is now included.
*/

#endif



/* Copyright (c) 2007 John P. Costella.
**
** End of file.
*/
