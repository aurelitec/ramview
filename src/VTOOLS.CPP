/*------------------------------------------------------------------------*/
/*                                                                        */
/*  OOSTUI 1.0 for C/C++                                                  */
/*  Object Oriented Simple Text User Interface                            */
/*                                                                        */
/*  VTOOLS.CPP: Video helper function(s)                                  */
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

#if !defined(_VTOOLS_H)
#define _VTOOLS_H

typedef unsigned char byte;

void fillattr(byte left, byte top, byte right, byte bottom, byte attr)
{
    for (byte y = top; y <= bottom; y++)
        for (byte x = left; x <= right; x++)
        {
            // position cursor
            asm  mov	dh, y
            asm  dec	dh
            asm  mov	dl, x
            asm  dec	dl
            asm  xor	bh, bh
            asm  mov	ah, 0x02
            asm  push bp
            asm  int 0x10
            asm  pop	bp

            // read character and attribute at cursor
            asm  xor	bh, bh
            asm  mov	ah, 0x08
            asm  push bp
            asm  int 0x10
            asm  pop	bp

            // write character and attribute at cursor
            asm  xor	bh, bh
            asm  mov	bl, attr
            asm  mov	cx, 1
            asm  mov	ah, 0x09
            asm  push bp
            asm  int 0x10
            asm  pop	bp
        }
}

#endif