/*------------------------------------------------------------------------*/
/*                                                                        */
/*  Fancy Text Display Tools for C/C++                                    */
/*                                                                        */
/*  FANCYTD.H: header file                                                */
/*                                                                        */
/*  Created in May 1997                                                   */
/*  Copyright (c) 1997 Devliber (https://devliber.com)                    */
/*                                                                        */
/*  Written and compiled in Borland C++ 3.1.                              */
/*                                                                        */
/*  Licensed under the MIT License. See LICENSE file in the project       */
/*  root for full license information.                                    */
/*                                                                        */
/*------------------------------------------------------------------------*/

#if !defined(_FANCYTD_H)
#define _FANCYTD_H

#if !defined(_CONIO_H)
#include <conio.h>
#endif // _CONIO_H

#if !defined(_STRING_H)
#include <string.h>
#endif // _STRING_H

#define MAKEATTR(backcolor, textcolor) (backcolor * 16 + textcolor)
#define MAKEBATTR(backcolor, textcolor) (backcolor * 16 + textcolor + 128)

void inline win(int left, int top, int right, int bottom, int attr)
{
    window(left, top, right, bottom);
    textattr(attr);
    clrscr();
}

int inline ccputs(const char *str, int y)
{
    struct text_info ti;
    gettextinfo(&ti);

    gotoxy((ti.winright - ti.winleft + 1 - strlen(str)) / 2 + 1, y);
    return (cputs(str));
}

#endif // _FANCYTD_H