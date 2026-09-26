/* grafik.c * Hur ritar Afån? * Funktioner för bakgrund, ikoner och markering. */

#include <proto/graphics.h>
#include "meny.h"
#include "grafik.h"
#include "ikoner.h"

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
    int tmp_height = 8;//va 8 vid 30
     for (int i = 0; i < 64; i++) {
         SetAPen(rp, i); // st�ll f�rg
         RectFill(rp, 0, i*tmp_height, 640, i*tmp_height + tmp_height);//supersnitsig regnb�ge,typ
       
     }

    
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

