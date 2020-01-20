/*------------------------------------------------------------------------*/
/*                                                                        */
/*  OOSTUI 1.0 for C/C++                                                  */
/*  Object Oriented Simple Text User Interface                            */
/*                                                                        */
/*  OOSTUI.CPP: Source code                                               */
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
#include "oostui.h"
#include <conio.h>
#include <alloc.h>
#include <dos.h>
#include "vtools.cpp"
#include <stdarg.h>

#include "macros.h"

char shadowAttr = 7;

const unsigned short
    kbUp = 0x4800,
    kbDown = 0x5000,
    kbPgUp = 0x4900,
    kbPgDn = 0x5100,
    kbCtrlPgUp = 0x8400,
    kbCtrlPgDn = 0x7600;

//--------------------------------------------------------------------------
// TSListBox
//--------------------------------------------------------------------------

#define OOSTUI_TSListBox 0x0005

class TSListBox : public TSView
{

public:
    char palette[2];

    TSListBox(int aLeft, int aTop, int aRight, int aBottom, long aRange);

    virtual unsigned int isA(void)
    {
        return (OOSTUI_TSListBox);
    }

    virtual void handleKeyEvent(unsigned &key);

    virtual char *getString(long item)
    {
        return (0);
    }

    virtual void draw(void);

    long range;
    long curItem;

    int lastDelta;
    int delta;
};

TSListBox::TSListBox(int aLeft, int aTop, int aRight, int aBottom, long aRange)
    : TSView(aLeft, aTop, aRight, aBottom),
      range(aRange),
      curItem(1L),
      delta(1),
      lastDelta(0)
{
}

void TSListBox::handleKeyEvent(unsigned &key)
{
    lastDelta = -1;
    switch (key)
    {
    case kbUp: // line up
        if (curItem > 1)
        {
            lastDelta = delta;
            if (delta > 1)
                delta--;
            curItem--;
        }
        break;
    case kbDown: // line down
        if (curItem < range)
        {
            lastDelta = delta;
            if (delta < height)
                delta++;
            curItem++;
        }
        break;
    case kbPgUp: // page up
        if (curItem > 1)
        {
            long newcur = curItem - (height - 1);
            if (newcur - delta > 0)
                curItem = newcur;
            else
            {
                curItem = 1;
                delta = 1;
            }
            lastDelta = 0;
        }
        break;
    case kbPgDn: // page down
        if (curItem < range)
        {
            long newcur = curItem + (height - 1);
            if (newcur + (height - delta) <= range)
                curItem = newcur;
            else
            {
                curItem = range;
                delta = (int)min(height, range);
            }
            lastDelta = 0;
        }
        break;
    case kbCtrlPgUp: // ctrl-page up
        if (curItem > 1)
        {
            curItem = 1;
            delta = 1;
            lastDelta = 0;
        }
        break;
    case kbCtrlPgDn: // ctrl-page down
        if (curItem < range)
        {
            curItem = range;
            delta = (int)min(height, range);
            lastDelta = 0;
        }
        break;
    default:
        return;
    }
    if (lastDelta != -1)
        draw();
    key = 0;
}

void TSListBox::draw(void)
{
    if (!lastDelta) // this means we have to redraw the whole list
    {
        win(left, top, right, bottom, *palette);
        long itemOfs = curItem - delta;
        for (int i = 1; i <= height; i++)
        {
            if (i == delta)
                textattr(*(palette + 1));
            else
                textattr(*palette);
            gotoxy(1, i);
            if (i + itemOfs <= range)
                cputs(getString(i + itemOfs));
        }
        return;
    }

    if (lastDelta != delta)
    {
        window(left, top, right, bottom);
        fillattr(left, top + lastDelta - 1, right - 1, top + lastDelta - 1, *palette);
        fillattr(left, top + delta - 1, right - 1, top + delta - 1, *(palette + 1));
    }
    else
    {
        window(left, top, right, bottom);
        if (delta == 1)
        {
            textattr(*palette);
            cputs(getString(curItem + 1));
            gotoxy(1, 1);
            insline();
            textattr(*(palette + 1));
            cputs(getString(curItem));
        }
        else if (delta == height)
        {
            gotoxy(1, height);
            textattr(*palette);
            cputs(getString(curItem - 1));
            cputs("\r\n");
            textattr(*(palette + 1));
            cputs(getString(curItem));
        }
    }
}

//--------------------------------------------------------------------------
// TSFrame
//--------------------------------------------------------------------------

#define OOSTUI_TSFrame 0x0002

class TSFrame : public TSView
{

protected:
    const char *frameChars;

public:
    char palette[1];

    TSFrame(int aLeft, int aTop, int aRight, int aBottom, int frType);

    virtual unsigned int isA(void)
    {
        return (OOSTUI_TSFrame);
    }

    virtual void draw(void);

private:
    static const char *near simpleFrame;
    static const char *near doubleFrame;
};

const char *near TSFrame::simpleFrame = "�Ŀ���";
const char *near TSFrame::doubleFrame = "�ͻ�ȼ";

TSFrame::TSFrame(int aLeft, int aTop, int aRight, int aBottom, int frType)
    : TSView(aLeft, aTop, aRight, aBottom)
{
    if (frType == -1)
        frameChars = 0;
    else
        frameChars = frType ? doubleFrame : simpleFrame;
}

void TSFrame::draw(void)
{
    if (frameChars)
    {
        window(left, top, right + 1, bottom);
        textattr(*palette);

        static char buffer[80];

        memset(buffer, *(frameChars + 1), width);
        *(buffer + width) = 0;
        *(buffer + width - 1) = *(frameChars + 2);
        *buffer = *frameChars;
        gotoxy(1, 1);
        cputs(buffer);

        gotoxy(1, height);
        *(buffer + width - 1) = *(frameChars + 5);
        *buffer = *(frameChars + 4);
        cputs(buffer);

        int vertline = *(frameChars + 3);
        for (register int i = 2; i <= height - 1; i++)
        {
            gotoxy(1, i);
            putch(vertline);
            gotoxy(width, i);
            putch(vertline);
        }
    }
}

//--------------------------------------------------------------------------
// TSDialogBox
//--------------------------------------------------------------------------

#define OOSTUI_TSDialogBox 0x0002

class TSDialogBox : public TSView
{

protected:
    char *title;
    TSFrame frame;
    void *buffer;
    int drawOK;

public:
    char palette[3];

    TSDialogBox(int aLeft, int aTop, int aRight, int aBottom,
                const char *aTitle, int frType = 0);

    virtual unsigned int isA(void)
    {
        return (OOSTUI_TSDialogBox);
    }

    void open(int cursortype = _NOCURSOR);
    void close(int cursortype = _NOCURSOR);

    virtual void draw(void);

    int canDraw(void)
    {
        return (drawOK);
    }

    void makeLocal(TSView *v);

    virtual ~TSDialogBox(void);
};

TSDialogBox::TSDialogBox(int aLeft, int aTop, int aRight, int aBottom,
                         const char *aTitle, int frType)
    : TSView(aLeft, aTop, aRight, aBottom), title(strdup(aTitle)),
      frame(aLeft, aTop, aRight, aBottom, frType),
      drawOK(0)

{
    if ((buffer = malloc((right - top + 3) * (bottom - top + 2) * 2)) != 0)
        drawOK = 1;
    else
        sviewhalt();
    if (!title)
        sviewhalt();
}

void TSDialogBox::open(int cursortype)
{
    if (drawOK)
    {
        gettext(left, top, right + 2, bottom + 1, buffer);
        draw();
        _setcursortype(cursortype);
    }
}

void TSDialogBox::close(int cursortype)
{
    if (drawOK)
    {
        puttext(left, top, right + 2, bottom + 1, buffer);
        _setcursortype(cursortype);
    }
}

void TSDialogBox::draw(void)
{
    if (drawOK)
    {
        // draw shadows
        //    win(left + 2, bottom + 1, right + 2, bottom + 1, shadowAttr);
        //    win(right + 1, top + 1, right + 2, bottom, shadowAttr);
        fillattr(left + 2, bottom + 1, right + 2, bottom + 1, shadowAttr);
        fillattr(right + 1, top + 1, right + 2, bottom, shadowAttr);
        // draw dialog box background
        win(left, top + 1, right, bottom, *(palette));
        // draw frame
        *(frame.palette) = *(palette + 2);
        frame.draw();
        // draw caption bar and display title
        if (*palette != *(palette + 1))
            win(left, top, right, top, *(palette + 1));
        ccputs(title, 1);
        //
        window(left + 1, top + 1, right - 1, bottom);
        textattr(*(palette));
    }
}

void TSDialogBox::makeLocal(TSView *v)
{
    v->left += left;
    v->top += top;
    v->right += left;
    v->bottom += top;
}

TSDialogBox::~TSDialogBox(void)
{
    free(title);
    free(buffer);
}

//--------------------------------------------------------------------------
// Miscellaneous
//--------------------------------------------------------------------------

unsigned inline biosGetKey(void)
{
    _AX = 0;
    geninterrupt(0x16);
    return (_AX);
}

int executeKey(TSView *view, unsigned ok, unsigned cancel)
{
    unsigned key;

    while (1)
    {
        key = biosGetKey();
        if (key == ok)
            return 1;
        if (key == cancel)
            return 0;
        view->handleKeyEvent(key);
    }
}

// execute key with confirmation

int executeCKey(TSView *view, unsigned ok, unsigned cancel)
{
    unsigned key;

    while (1)
    {
        key = biosGetKey();
        view->handleKeyEvent(key);
        if (key == ok)
            return 1;
        if (key == cancel)
            return 0;
    }
}

void setPalette(char *palette, ...)
{
    int pal;
    va_list ap;
    va_start(ap, palette);
    while ((pal = va_arg(ap, int)) != 0)
    {
        *palette = pal;
        palette++;
    }
    va_end(ap);
}

void biosSetBlink(unsigned char blink)
{
    asm     push    bp
    asm     mov     ax, 0x1003
    asm     mov     bl, blink
    asm     int     0x10
    asm     pop     bp
}

