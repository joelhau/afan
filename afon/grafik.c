/* grafik.c * Hur ritar Afån? * Funktioner för bakgrund, ikoner och markering. */

#include <proto/graphics.h>
#include <string.h>
#include <dos/dos.h>
#include <proto/dos.h>
#include "meny.h"
#include <dos/datetime.h>
#include "grafik.h"
#include "ikoner.h"
#include "skarm.h"
//struct TextFont *font = NULL;
#define aa 0xE5

void uppdatera_skarm(struct RastPort* rp) {
        draw_background(rp); //rita bakgrund
    
    //rita ikoner
    markIcon(rp, &app);

    draw_icon(rp, 0, 50, 50);//wings
    draw_icon(rp, 1, 145, 50);//moonstone
    draw_icon(rp, 2, 240, 50);//kalender
    draw_icon(rp, 3, 50, 100);//klocka
    draw_icon(rp, 5, 145, 100);
    draw_icon(rp, 5, 240, 100);
    draw_icon(rp, 4, 50, 150);//Telefon
    draw_icon(rp, 5, 145, 150);
    draw_icon(rp, 5, 240, 150);
    draw_ram(rp);
    draw_time(rp);

}


void draw_ram(struct RastPort* rp) {

    //int hojd=480;
    //int bredd=630;
  //lilla topplisten 
    SetAPen(rp, 7); //st?ller in f?rg o rita med
    Move(rp, 3, 3); //vit övre
    Draw(rp, app.screenW-3, 3);
    Move(rp, 3, 3); //vit vänster
    Draw(rp, 3, app.screenH-3);

    SetAPen(rp, 26); //ruta grå
    RectFill(rp, 4, 4, app.screenW-3, 15);

    SetAPen(rp, 22); //st?ller in f?rg o rita med
    Move(rp, 3, 16); //list under mörkgrå
    Draw(rp, app.screenW-3, 16);

    SetAPen(rp, 1); //st?ller in f?rg o rita med
    Move(rp, 3, 17); //list underst svart
    Draw(rp, app.screenW-3, 17);
    
    

    //yttre ram övre

    SetAPen(rp, 7); //st?ller in f?rg o rita med
    Move(rp, 0, 0); //vit övre
    Draw(rp, app.screenW, 0); //vita övreram    


    SetAPen(rp, 26); //st?ller in f?rg o rita med
    Move(rp, 1, 1); //grå övre
    Draw(rp, app.screenW-1, 1);


    SetAPen(rp, 1); //st?ller in f?rg o rita med
    Move(rp, 2, 2); //svart övre
    Draw(rp, app.screenW-2, 2);

    //yttre ram vänster--------------------------------------------------------

    SetAPen(rp, 7); //st?ller in f?rg o rita med
    Move(rp, 0, 0); //vit sida vänster
    Draw(rp, 0, app.screenH); //vita övreram    

    SetAPen(rp, 26); //st?ller in f?rg o rita med
    Move(rp, 1, 1); //grå
    Draw(rp, 1, app.screenH-1); 
    
    SetAPen(rp, 1); //st?ller in f?rg o rita med
    Move(rp, 2, 2); //
    Draw(rp, 2, app.screenH-2); //svart    

 //yttre ram höger--------------------------------------------------------

    SetAPen(rp, 7); //st?ller in f?rg o rita med
    Move(rp, 0, 0); //vit sida vänster
    Draw(rp, 0, app.screenH); //vita övreram    

    SetAPen(rp, 26); //st?ller in f?rg o rita med
    Move(rp, 1, 1); //grå
    Draw(rp, 1, app.screenH-1); 
    
    SetAPen(rp, 1); //st?ller in f?rg o rita med
    Move(rp, 2, 2); //
    Draw(rp, 2, app.screenH-2); //svart    






   
    
}



void draw_icon(struct RastPort* rp, int iconIndex, int startX, int startY)
{

    for (int y = 0; y < ICON_HEIGHT; y++) { //forloopar f?r o rita iconer
        for (int x = 0; x < ICON_WIDTH; x++) {
            UBYTE color = iconSet[iconIndex][y][x]; //plockar f?rgcoder fr?n arrayet iconset
            if (color != 0) {                           //ritar inga pixlr p? 0:or
                SetAPen(rp, color); //st?ller in f?rg o rita med
                WritePixel(rp, startX + x, startY + y); //ritar en pixel
            }
        }
    }

}

void draw_background(struct RastPort* rp)
{
    int tmp_height = 15;//va 8 vid 30
     for (int i = 0; i < 13; i++) {
         SetAPen(rp, i+19); // st�ll f�rg
         RectFill(rp, 0, i*tmp_height, app.screenW, i*tmp_height + tmp_height);//supersnitsig regnb�ge,typ
       
     }

    // for (int i = 32; i < 64; i++) {
    //     SetAPen(rp, i); // st�ll f�rg
    //     RectFill(rp, 0, i*tmp_height, 300, i*tmp_height + tmp_height);//supersnitsig regnb�ge,typ
    //   
    // }
    
}

void markIcon(struct RastPort* rp, struct APhone* app)
{
    //95 ca i r�relse
    int markJumpX = 95;
    int markJumpY = 50;
    int markWidth = 40;
    int markHeight = 40;
    int markStartX = -50;
    int markStartY = -5;
        SetAPen(rp, 7); // st�ll f�rg
        RectFill(rp, (app->mnuX * markJumpX) + markStartX, 
            (app->mnuY * markJumpY) + markStartY, 
            (app->mnuX * markJumpX)+markWidth + markStartX, 
            (app->mnuY * markJumpY) + markHeight + markStartY);//supersnitsig ruta runt icon
}

void draw_time(struct RastPort* rp) { 
    struct DateStamp ds; struct DateTime dt; 
    static char dateStr[40]; static char timeStr[40]; //deklarera font 
    DateStamp(&ds); 
    SetAPen(rp, 1); //st?ller in f?rg o rita med
    SetBPen(rp, 26); //st?ller in f?rg o rita med
  
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
        Move(rp, 10, 12); 
        Text(rp, "A-F\xE5n 1.0", 9); 
        Move(rp, 230, 12); 
        Text(rp, timeStr, strlen(timeStr)); 
    } 
}
