/*------------------------------------------------------------------------*/
/*                                                                        */
/*  MacroTools for C/C++                                                  */
/*                                                                        */
/*  MACROS.H: header file                                                 */
/*                                                                        */
/*  Created in May 1997                                                   */
/*  Copyright (c) 1997 Aurelitec (https://www.aurelitec.com)                    */
/*                                                                        */
/*  Written and compiled in Borland C++ 3.1.                              */
/*                                                                        */
/*  Licensed under the MIT License. See LICENSE file in the project       */
/*  root for full license information.                                    */
/*                                                                        */
/*------------------------------------------------------------------------*/

#if !defined(__MACROS_H)
#define __MACROS_H

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#define LOWORD(l) ((unsigned int)(unsigned long)(l))
#define HIWORD(l) ((unsigned int)((((unsigned long)(l)) >> 16) & 0xFFFF))
#define MAKELONG(low, high) ((unsigned long)(((unsigned int)(low)) | (((unsigned long)((unsigned int)(high))) << 16)))

#define LOBYTE(w) ((unsigned char)(w))
#define HIBYTE(w) ((unsigned char)(((unsigned int)(w) >> 8) & 0xFF))
#define MAKEWORD(low, high) ((unsigned int)(((unsigned char)(low)) | (((unsigned int)((unsigned char)(high))) << 8)))

/* macros to work with far pointers addresses
 */

#define _MAKE_FP(seg, off) ((void far *)MAKELONG((off), (seg)))
#define _OFF(lp) LOWORD(lp)
#define _SEG(lp) HIWORD(lp)

#define MAKE_FP(seg, off) ((void _seg *)(seg) + (void near *)(off))
#define SEG(fp) ((unsigned)(void _seg *)(void far *)(fp))
#define OFF(fp) ((unsigned)(fp))

/* macros to examine and change memory locations
 */

#define mem(seg, off) *((unsigned char far *)MAKE_FP(seg, off))
#define memW(seg, off) *((unsigned int far *)MAKE_FP(seg, off))
#define memL(seg, off) *((unsigned long far *)MAKE_FP(seg, off))

#define DSmem(off) *((unsigned char *)off)
#define DSmemW(off) *((unsigned int *)off)
#define DSmemL(off) *((unsigned long *)off)

#define _mem(off) DSmem(off)
#define _memW(off) DSmemW(off)
#define _memL(off) DSmemL(off)

#define ESmem(off) *((unsigned char _es *)off)
#define ESmemW(off) *((unsigned int _es *)off)
#define ESmemL(off) *((unsigned long _es *)off)

#define CSmem(off) *((unsigned char _cs *)off)
#define CSmemW(off) *((unsigned int _cs *)off)
#define CSmemL(off) *((unsigned long _cs *)off)

#define SSmem(off) *((unsigned char _ss *)off)
#define SSmemW(off) *((unsigned int _ss *)off)
#define SSmemL(off) *((unsigned long _ss *)off)

#define now() memL(0x0040, 0x006C)

#endif // __MACROS_H