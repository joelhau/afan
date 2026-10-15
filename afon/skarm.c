/* skarm.c * Hur får Afån en skärm? * Öppnar och stänger Amiga-skärm, fönster och grafikbibliotek. */


#include <exec/libraries.h>
#include <proto/exec.h>


#include <exec/types.h>
#include <graphics/gfxbase.h>
#include <graphics/rastport.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/graphics.h>

#include "skarm.h"
#include "meny.h"
struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;

struct Screen *screen;
struct Window *window;
struct RastPort *rp;
struct TextFont *font = NULL;


void startaAppTest(char *kommando)
{
   ScreenToBack(screen);
        Execute(kommando, NULL, NULL);
    ScreenToFront(screen);
}

void loggFel(char *text, LONG error){
    BPTR fh;

    fh = Open("PROGDIR:afon.log", MODE_READWRITE);
//
    if (fh)
    {
        Seek(fh, 0, OFFSET_END);

        FPrintf(fh, "%s: %ld\n", text, error);

        Close(fh);
    }
}

int initSkarm(void)
{
    if (!GfxBase)
    {
        GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 0);

        if (!GfxBase)
            return 0;
    }

    if (!IntuitionBase)
    {
        IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 0);

        if (!IntuitionBase)
        {
            CloseLibrary((struct Library*)GfxBase);
            GfxBase = NULL;
            return 0;
        }
    }

    screen = NULL;
struct Screen *wbScreen;

wbScreen = LockPubScreen(NULL);

window = OpenWindowTags(NULL,
    WA_Left, 0,
    WA_Top, 0,
    WA_Width, wbScreen->Width,
    WA_Height, wbScreen->Height,
    WA_Borderless, TRUE,
    WA_IDCMP, IDCMP_RAWKEY | IDCMP_CLOSEWINDOW,
    TAG_DONE);

UnlockPubScreen(NULL, wbScreen);

    if (!window)
    {
        stangSkarm();
        return 0;
    }
    rp = window->RPort;


    struct TextAttr ta = {
            .ta_Name = "topaz.font",
            .ta_YSize = 8,
            .ta_Style = FS_NORMAL,
            .ta_Flags = FPF_ROMFONT
        };

        font = OpenFont(&ta);

    return 1;
}

void stangSkarm(void)
{
    if (window)
    {
        CloseWindow(window);
        window = NULL;
    }

    rp = NULL;

    if (IntuitionBase)
    {
        CloseLibrary((struct Library*)IntuitionBase);
        IntuitionBase = NULL;
    }

    if (GfxBase)
    {
        CloseLibrary((struct Library*)GfxBase);
        GfxBase = NULL;
    }
}
