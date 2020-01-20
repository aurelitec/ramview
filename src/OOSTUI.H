/*------------------------------------------------------------------------*/
/*                                                                        */
/*  OOSTUI 1.0 for C/C++                                                  */
/*  Object Oriented Simple Text User Interface                            */
/*                                                                        */
/*  OOSTUI.H: header file                                                 */
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

#if !defined(_OOSTUI_H)
#define _OOSTUI_H

const char EOS = '\0';

struct TMouseEvent
{
    int x, y;
    int button;
};

struct TCommandEvent
{
    int command;
};

class TSView
{

protected:
    int left, top, right, bottom, width, height;

public:
    TSView(int aLeft, int aTop, int aRight, int aBottom);

    virtual unsigned int isA(void) = 0;

    virtual void handleKeyEvent(unsigned &key)
    {
    }

    virtual void handleMouseEvent(TMouseEvent &mouse)
    {
    }

    virtual void handleCommand(TCommandEvent &comm)
    {
    }

    virtual void draw(void) = 0;

    virtual ~TSView()
    {
    }

    friend TSDialogBox;
};

void defaulthalt(void)
{
}

void (*sviewhalt)(void) = defaulthalt;

inline TSView::TSView(int aLeft, int aTop, int aRight, int aBottom)
{
    left = aLeft;
    top = aTop;
    right = aRight;
    bottom = aBottom;
    width = aRight - aLeft + 1;
    height = aBottom - aTop + 1;
}

#endif // _OOSTUI_H