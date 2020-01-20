/*------------------------------------------------------------------------*/
/*                                                                        */
/*  RAM View Version 0.1 (pre-official release)                           */
/*  Memory dump utility                                                   */
/*                                                                        */
/*  RAMVIEW.CPP: Program source code                                      */
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

#include "fancytd.h"
#include "oostui.cpp"
#include "macros.h"

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

const unsigned short kbF10 = 0x4400;

unsigned char cBackground = MAKEATTR(8, 15);
unsigned char cAppTitle = MAKEATTR(9, 15);
unsigned char cAppInfo = MAKEATTR(15, 0);
unsigned char cAppInfoSel = MAKEATTR(15, 4);

char tempCharBuf[80];

//--------------------------------------------------------------------------
// draw interactive mode screen
//--------------------------------------------------------------------------

void drawScreen(void)
{
    textmode(C80);
    directvideo = 1;        // use Direct Video by default, for speed
    biosSetBlink(0);        // disable blinking (use light background colors)
    _setcursortype(_NOCURSOR);      // hide cursor
    win(1, 1, 80, 25, cBackground); // clear screen

    // display application title in the center of the upper line
    win(1, 1, 80, 1, cAppTitle);
    ccputs("RAM View", 1);

    // display version and author information
    win(1, 2, 80, 2, cAppInfo);
    cputs(" Version 0.1 ³ Copyright (c) 1997 Devliber");
    gotoxy(68, 1);
    cputs("³ F10 = Exit");

    win(1, 25, 80, 25, cAppInfo);
    textattr(cAppInfoSel);
    cputs(" \x18\x19, PgUp PgDn");
    textcolor(cAppInfo);
    cputs(" Navigation keys ");
    textattr(cAppInfoSel);
    cputs(" Ctrl-PgUp");
    textcolor(cAppInfo);
    cputs(" Memory Start ");
    textattr(cAppInfoSel);
    cputs(" Ctrl-PgDn");
    textcolor(cAppInfo);
    cputs(" Memory End");
}

void shutdown(const char *custommsg, int errorcode)
{
    _setcursortype(_NORMALCURSOR);
    win(1, 1, 80, 25, MAKEATTR(0, 7));
    cputs(custommsg);
    exit(errorcode);
}

//--------------------------------------------------------------------------
// TSRAMBox class, derived from TSListBox
//--------------------------------------------------------------------------
class TSRAMBox : public TSListBox
{

public:
    TSRAMBox(int aLeft, int aTop, int aRight, int aBottom, int aMax)
        : TSListBox(aLeft, aTop, aRight, aBottom, aMax)
    {
    }

    virtual char *getString(long item);
};

unsigned char vals[17];

char *TSRAMBox::getString(long item)
{
    static char buffer[100];
    int i, j;
    memset(buffer, ' ', width);

    item--;
    sprintf(buffer, "%.04X:0 ", LOWORD(item));

    for (j = 0; j <= 3; j++)
    {
        for (i = 0; i <= 3; i++)
        {
            vals[i + j * 4] = mem(item, i + j * 4);
            sprintf(tempCharBuf, " %.02X", vals[i + j * 4]);
            strcat(buffer, tempCharBuf);
            if (vals[i + j * 4] <= 0x1F)
                vals[i + j * 4] = '.';
        }
        if (j < 3)
            strcat(buffer, " ï¿½");
    }
    vals[16] = EOS;
    strcat(buffer, "  ");
    for (j = 0; j < 16; j++)
        strcat(buffer, vals);

    *(buffer + strlen(buffer)) = ' ';
    buffer[width - 1] = EOS;
    return (buffer);
}

TSRAMBox ramBox(1, 3, 80, 24, 0L);

//--------------------------------------------------------------------------
// main
//--------------------------------------------------------------------------

void new_sviewhalt(void)
{
    shutdown("Out of memory", -1);
}

int main(void)
{
    sviewhalt = new_sviewhalt;

    // enter interactive mode
    drawScreen();
    setPalette(ramBox.palette, MAKEATTR(9, 15), MAKEATTR(0, 15), 0);
    ramBox.range = 0x10000L;
    ramBox.lastDelta = 0;

    // memory dump start adress
    char *endptr;
    ramBox.curItem = strtol(_argv[1], &endptr, 0) + 1;
    if (!ramBox.curItem || ramBox.curItem > ramBox.range)
        ramBox.curItem = 1L;
    ramBox.draw();

    // run interactive
    unsigned c;
    while (1)
    {
        c = biosGetKey();
        if (c == kbF10)
            break;
        switch (c)
        {
        default:
            ramBox.handleKeyEvent(c);
        }
    }

    // restore cursor and text mode
    _setcursortype(_NORMALCURSOR);
    win(1, 1, 80, 25, MAKEATTR(0, 7));
    textmode(LASTMODE);

    return 0;
}