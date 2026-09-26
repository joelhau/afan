/* meny.c * Vad ska Afån göra? * Huvudprogram, menylogik och input. */

#include <exec/types.h>
#include <exec/execbase.h>
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <string.h>
#include <diskfont/diskfont.h> // kr?vs f?r OpenDiskFont()
#include <stddef.h>
#include <stdlib.h>
#include <dos/dos.h>
#include <dos/datetime.h>
#include <workbench/startup.h>
#include <stdio.h>
#include <devices/input.h>
#include <devices/inputevent.h>
#include <proto/input.h>
#include "ikoner.h" //arrays för ikoner
#include "grafik.h" //ritande funktioner
#include "meny.h" //programvariabler
#include "skarm.h" //sköter skärmen



//f?rdeklarering av funktioner s? compilatorn hittar dom
void draw_icon(struct RastPort* rp, int iconIndex, int startX, int startY); //visa ikoner
void delay(); //delayloop


//variabler f?r scrollen o sk?rm
#define WIDTH 640 //640
#define HEIGHT 200

struct Interrupt inputHandler;
volatile BOOL running;
struct TextFont *font;




void delay()
{
    for (volatile long i = 0; i < 500; i++) {} //loopar 500 ggr f?r o pausa lite
}

void draw_time(struct RastPort* rp) { 
    struct DateStamp ds; struct DateTime dt; 
    static char dateStr[40]; static char timeStr[40]; //deklarera font 
    struct TextAttr ta = { .ta_Name = "topaz.font", // detta är Topaz 8 
    .ta_YSize = 8, 
    .ta_Style = FS_NORMAL, 
    .ta_Flags = FPF_ROMFONT }; 
    font = OpenFont(&ta); 
    DateStamp(&ds); 
    dt.dat_Stamp = ds; 
    dt.dat_Format = FORMAT_INT; // Internationellt format: HH:MM:SS 
    dt.dat_Flags = 0; dt.dat_StrDay = NULL; 
    dt.dat_StrDate = dateStr; dt.dat_StrTime = timeStr; 
    if (DateToStr(&dt)) { }
    else { 
        // Printf("Kunde inte hämta tiden.\n"); 
    } 
    if (font) {
        SetFont(rp, font); // använd fonten 
        Move(rp, 20, 20); 
        Text(rp, "APhone", 6); 
        Move(rp, 200, 20); 
        Text(rp, timeStr, strlen(timeStr)); 
    } 
}






int main()
{
    struct APhone app;//structen f�r delade globala variabler � pekare
    BOOL updateScreen = 0;
    app.mnuX = 1;
    app.mnuY = 1;
    running = TRUE;
    if (!initSkarm())
    return 1;
    struct IntuiMessage* msg;
    draw_background(rp); //rita bakgrund
    
    //rita ikoner
    markIcon(rp, &app);

    draw_icon(rp, 0, 50, 50);
    draw_icon(rp, 1, 145, 50);
    draw_icon(rp, 2, 240, 50);
    draw_icon(rp, 3, 50, 100);
    draw_icon(rp, 3, 145, 100);
    draw_icon(rp, 3, 240, 100);
    draw_icon(rp, 3, 50, 150);
    draw_icon(rp, 3, 145, 150);
    draw_icon(rp, 4, 240, 150);
    draw_time(rp);
    


    while (running) {
        //updateScreen = TRUE;
        if (updateScreen)
        {
            draw_background(rp); //rita bakgrund
            //rita ikoner
            markIcon(rp, &app);
            draw_icon(rp, 0, 50, 50);
            draw_icon(rp, 1, 145, 50);
            draw_icon(rp, 2, 240, 50);
            draw_icon(rp, 3, 50, 100);
            draw_icon(rp, 3, 145, 100);
            draw_icon(rp, 3, 240, 100);
            draw_icon(rp, 3, 50, 150);
            draw_icon(rp, 3, 145, 150);
            draw_icon(rp, 4, 240, 150);
            //draw_time(rp);
            updateScreen = FALSE;
        }
    

while ((msg = (struct IntuiMessage*)GetMsg(window->UserPort))){
            if (msg->Class == IDCMP_RAWKEY) {
                switch (msg->Code) {
                case 0x45: // ESC
                    running = FALSE;
                    break;
                case 0x4c: // Pil upp
                    app.mnuY--;
                    updateScreen = TRUE;
                    break;
                case 0x4d: // Pil ned
                    app.mnuY++;
                    updateScreen = TRUE;
                    break;
                case 0x4f: // Pil v�nster
                    app.mnuX--;
                    updateScreen = TRUE;
                    break;
                case 0x4e: // Pil h�ger
                    app.mnuX++;
                    updateScreen = TRUE;
                    break;
                case 0x44: // Enter
                {
                    if (app.mnuX == 2 && app.mnuY == 1) { // app 1x1
                        //IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37);
                        //if (!IntuitionBase) {
                            //printf("Kunde inte �ppna Intuition!\n");
                        //    return 1;
                        //}
                        //LONG res;
                        //WindowToBack(window);
                        //res = Execute("SYS:Utilities/Clock", 0, 0);
                        //WindowToFront(window);
                        startaApp("SYS:Utilities/Clock");
                        //updateScreen = TRUE;
                    }
                    if (app.mnuX == 1 && app.mnuY == 1) { // app 1x1
                        IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37);
                        if (!IntuitionBase) {
                            printf("Kunde inte �ppna Intuition!\n");
                            return 1;
                        }
                        LONG res;
                        WindowToBack(window);
                        System("Run >NIL: WHDLoad slave=dh0:games/moonstone/moonstone.slave data=dh0:games/moonstone/data", NULL);
                        WindowToFront(window);
                    }

                    break;
                }
                }
            }
            else if (msg->Class == IDCMP_CLOSEWINDOW) {
                running = FALSE;
            }
            ReplyMsg((struct Message*)msg);
        }



       
        delay();
    }//Dödar loop



if (font) CloseFont(font);
stangSkarm();
return 0;
}
