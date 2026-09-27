/* meny.c * Vad ska Afån göra? * Huvudprogram, menylogik och input. 
0.1 Skriva om från början med reste
0.2 städa lite bland rutiner som inte behövs
0.3 testa olika sätt att starta appar o hantera fönster
    Liten snuskig bugg med o skapa fontpekare för många ggr
    Missa o avsluta intuition vid skärm avslut
    Felsökare med loggfil
    Det e något som inte funkar me o starta om skärm hmm
    MEN NU KAN DEN DROPPA UT I WB O STARTA OM IAF 

*/

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

volatile BOOL running;
//struct TextFont *font;
struct TextFont *font = NULL;
int exitCode = 0;




void delay()
{
    for (volatile long i = 0; i < 500; i++) {} //loopar 500 ggr f?r o pausa lite
}

void draw_time(struct RastPort* rp) { 
    struct DateStamp ds; struct DateTime dt; 
    static char dateStr[40]; static char timeStr[40]; //deklarera font 
    DateStamp(&ds); 
    SetAPen(rp, 1); //st?ller in f?rg o rita med
      SetBPen(rp, 10); //st?ller in f?rg o rita med
  
    dt.dat_Stamp = ds; 
    dt.dat_Format = FORMAT_INT; // Internationellt format: HH:MM:SS 
    dt.dat_Flags = 0; dt.dat_StrDay = NULL; 
    dt.dat_StrDate = dateStr; dt.dat_StrTime = timeStr; 
    if (DateToStr(&dt)) { }
    else { 
        // Printf("Kunde inte hämta tiden.\n"); 
    } 
    if (font) {
        SetFont(rp, font); // använd fonten 19 
        Move(rp, 20, 6); 
        Text(rp, "APhone", 6); 
        Move(rp, 200, 6); 
        Text(rp, timeStr, strlen(timeStr)); 
    } 
}

void draw_ram(struct RastPort* rp) {

    int hojd=480;
    int bredd=640;
   
    SetAPen(rp, 7); //st?ller in f?rg o rita med
    Move(rp, 0, 0); //vit övre
    Draw(rp, bredd, 0); //vita övreram    

    Move(rp, 0, 0); //vit sida vänster
    Draw(rp, 0, hojd); //vita övreram    

    Move(rp, 4, 4); //vit övre inre list
    Draw(rp, bredd-4, 4); //vita övreram    


    Move(rp, 4, 4); //vit sida inre list
    Draw(rp, 4, 10); //vita övreram    


    SetAPen(rp, 24); //övtr grå
    RectFill(rp, 1, 1, 100, 2);


    SetAPen(rp, 24); //övtr grå
    RectFill(rp, 5, 5, 100, 9);

    
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
   
    if (!font)
{
    struct TextAttr ta = {
        .ta_Name = "topaz.font",
        .ta_YSize = 8,
        .ta_Style = FS_NORMAL,
        .ta_Flags = FPF_ROMFONT
    };

    font = OpenFont(&ta);

    if (!font)
        return 1;
}

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
    draw_ram(rp);
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
            draw_time(rp);
            draw_ram(rp);
            draw_time(rp);
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
                    if (app.mnuX == 1 && app.mnuY == 2) { // Klocka 1x2
                      //  startaApp("SYS:Utilities/Clock");
                        
                        //msg = NULL;
                        //updateScreen = TRUE;
                        exitCode = 1;
                        running = FALSE;
                    }
                                       
                    if (app.mnuX == 2 && app.mnuY == 2) { // Klocka 1x2
                        startaApp("SYS:Utilities/Clock");
                        
                        //msg = NULL;
                        updateScreen = TRUE;
                        //exitCode = 1;
                        //running = FALSE;
                    }
                    
                    if (app.mnuX == 1 && app.mnuY == 1) { // app 1x1
                     //   System("Run >NIL: WHDLoad slave=dh0:games/moonstone/moonstone.slave data=dh0:games/moonstone/data", NULL);
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

           // ReplyMsg((struct Message*)msg);


       
        delay();
    }//Dödar loop

if (msg != NULL) {
    ReplyMsg((struct Message*)msg);
}



    if (font) 
        CloseFont(font);
font = NULL;
stangSkarm();
return 0;
}
