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

struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;

struct Screen *screen;
struct Window *window;
struct RastPort *rp;

void oppnaFonster(void)
{
    window = OpenWindowTags(NULL,
        WA_CustomScreen, (ULONG)screen,
        WA_Left, 0,
        WA_Top, 0,
        WA_Width, 640,
        WA_Height, 480,
        WA_Borderless, TRUE,
        WA_IDCMP, IDCMP_RAWKEY,
        TAG_DONE);
}

void startaApp(char *kommando)
{
    CloseWindow(window);
    window = NULL;

    CloseScreen(screen);
    screen = NULL;
    rp = NULL;

    Execute(kommando, NULL, NULL);

    if (!screen)
        initSkarm();
}

int initSkarm(void)
{
    screen = NULL;
    window = NULL;
    rp = NULL;

    if (!GfxBase)
        GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 0);

    if (!IntuitionBase)
        IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 0);

    if (!GfxBase || !IntuitionBase)
        return 0;

    screen = OpenScreenTags(NULL,
        SA_Left, 0,
        SA_Top, 0,
        SA_Width, 640,
        SA_Height, 480,
        SA_Depth, 6,
        SA_Type, CUSTOMSCREEN,
        SA_Title, (ULONG)"APhone",
        TAG_DONE);

    if (!screen)
        return 0;

    rp = &screen->RastPort;

    window = OpenWindowTags(NULL,
        WA_CustomScreen, (ULONG)screen,
        WA_Left, 0,
        WA_Top, 0,
        WA_Width, 640,
        WA_Height, 480,
        WA_Borderless, TRUE,
        WA_IDCMP, IDCMP_RAWKEY,
        TAG_DONE);

    if (!window)
    {
        CloseScreen(screen);
        screen = NULL;
        rp = NULL;
        return 0;
    }

    return 1;
}void stangSkarm(void)
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

//    if (IntuitionBase)
//    {
 //       CloseLibrary((struct Library*)IntuitionBase);
 //       IntuitionBase = NULL;
 //   }

   // if (GfxBase)
  //  {
  //      CloseLibrary((struct Library*)GfxBase);
  //      GfxBase = NULL;
  //  }
}