#ifndef SKARM_H
#define SKARM_H

#include <exec/types.h>
#include <intuition/intuition.h>
#include <graphics/rastport.h>

void startaApp(char *kommando);
void oppnaFonster(void);

extern struct Screen *screen;
extern struct Window *window;
extern struct RastPort *rp;

int initSkarm(void);
void stangSkarm(void);

#endif