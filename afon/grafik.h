#ifndef GRAFIK_H
#define GRAFIK_H

#include <graphics/rastport.h>
#include "meny.h"

void draw_icon(struct RastPort* rp, int iconIndex, int startX, int startY);
void draw_background(struct RastPort* rp);
void markIcon(struct RastPort* rp, struct APhone* app);

#endif