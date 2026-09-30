#ifndef GRAFIK_H
#define GRAFIK_H

#include <graphics/rastport.h>
#include "meny.h"

//extern struct TextFont *font;

void draw_icon(struct RastPort* rp, int iconIndex, int startX, int startY);
void draw_background(struct RastPort* rp);
void markIcon(struct RastPort* rp, struct APhone* app);
void draw_time(struct RastPort* rp);
void draw_ram(struct RastPort* rp);
void uppdatera_skarm(struct RastPort* rp);


#endif