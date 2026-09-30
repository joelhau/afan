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

void startaApp(char *kommando)
{
    if (font)
    {
        CloseFont(font);
        font = NULL;
    }

    stangSkarm();

    Execute(kommando, NULL, NULL);

    if (!initSkarm())
        return;

    {
        struct TextAttr ta = {
            .ta_Name = "topaz.font",
            .ta_YSize = 8,
            .ta_Style = FS_NORMAL,
            .ta_Flags = FPF_ROMFONT
        };

        font = OpenFont(&ta);
    }
}

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
    LONG error;

    loggFel("INIT START", 0);

    screen = NULL;
    window = NULL;
    rp = NULL;

    if (!GfxBase)
    {
        GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 0);

        if (GfxBase)
            loggFel("graphics.library OK", 0);
        else
            loggFel("graphics.library FEL", -1);
    }
    else
    {
        loggFel("graphics.library redan öppen", 0);
    }

    if (!IntuitionBase)
    {
        IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 0);

        if (IntuitionBase)
            loggFel("intuition.library OK", 0);
        else
            loggFel("intuition.library FEL", -1);
    }
    else
    {
        loggFel("intuition.library redan öppen", 0);
    }

    if (!GfxBase || !IntuitionBase)
    {
        loggFel("LIBRARY CHECK FEL", 0);
        stangSkarm();
        return 0;
    }

    loggFel("OPEN SCREEN", 0);

    screen = OpenScreenTags(NULL,
        SA_Left, 0,
        SA_Top, 0,
        SA_Width, app.screenW,
        SA_Height, app.screenH,
        SA_Depth, 6,
        SA_Type, CUSTOMSCREEN,
        SA_Title, (ULONG)"APhone",
        SA_ErrorCode, (ULONG)&error,
        TAG_DONE);

    if (!screen)
    {
        loggFel("OpenScreenTags FEL", error);
        stangSkarm();
        return 0;
    }

    loggFel("SCREEN OK", 0);

    rp = &screen->RastPort;

    loggFel("OPEN WINDOW", 0);

    window = OpenWindowTags(NULL,
        WA_CustomScreen, (ULONG)screen,
        WA_Left, 0,
        WA_Top, 0,
        WA_Width, app.screenW,
        WA_Height, app.screenH,
        WA_Borderless, TRUE,
        WA_IDCMP, IDCMP_RAWKEY,
        TAG_DONE);

    if (!window)
    {
        loggFel("OpenWindowTags FEL", 0);
        stangSkarm();
        return 0;
    }

    loggFel("WINDOW OK", 0);
    loggFel("INIT KLAR", 0);
struct TextAttr ta = {
    .ta_Name = "topaz.font",
    .ta_YSize = 8,
    .ta_Style = FS_NORMAL,
    .ta_Flags = FPF_ROMFONT
};

font = OpenFont(&ta);

if (!font)
    loggFel("FONT FEL", 0);
else
    loggFel("FONT OK", 0);
    return 1;
}

void stangSkarm(void)
{
    if (window)
    {
        CloseWindow(window);
        window = NULL;
    }

    if (screen)
    {
        CloseScreen(screen);
        screen = NULL;
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
