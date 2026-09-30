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
void delay(); //delayloop

//variabler f?r scrollen o sk?rm
//#define WIDTH 640 
//#define HEIGHT 480
volatile BOOL running;
int exitCode = 0;
struct APhone app;//structen f�r delade globala variabler � pekare
  

void delay()
{
    for (volatile long i = 0; i < 500; i++) {} //loopar 500 ggr f?r o pausa lite
}





int main()
{

    loggFel("---------------------------------------------------", 0);
    loggFel("STARTAR.....", 0);
    app.mnuX = 1;
    app.mnuY = 1;
    app.screenW = 640; //320;
    app.screenH = 480;
    BOOL updateScreen = 0;
    running = TRUE;
    if (!initSkarm())
    return 1;
    struct IntuiMessage* msg;
    uppdatera_skarm(rp);
    


    while (running) {
        //updateScreen = TRUE;
        if (updateScreen)
        {
            uppdatera_skarm(rp);
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
                case 0x44: // Enter allt kul som händer vid enter
                {
                    if (app.mnuX == 1 && app.mnuY == 2) { // Klocka 1x2
                        startaAppTest("SYS:Utilities/Clock");
                    }
                                       
                    if (app.mnuX == 1 && app.mnuY == 3) { // Klocka 1x2
                        //startaAppTest("DH1:AFONSEND/AFONSEND >CON:0/0/640/200/AfonCLI");
                        //startaAppTest("DH1:AFONSEND/AFONSEND >CON:0/0/640/200/AfonCLI/CLOSE");
                        //startaAppTest("EXECUTE DH1:AFONSEND/TEST >CON:0/0/640/200/AfonCLI");
                        startaAppTest("DH1:AFON/AFONSEND");
                    }






                    
                    if (app.mnuX == 1 && app.mnuY == 1) { // app 1x1
                     System("Run >NIL: WHDLoad slave=dh0:games/moonstone/moonstone.slave data=dh0:games/moonstone/data", NULL);
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

if (font) //Stäng font
    CloseFont(font);
font = NULL;
stangSkarm();
return 0;
}
