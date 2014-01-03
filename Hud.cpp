#include "Hud.h"
#include "aaline.h"
#include "gamedata.h"
#include "viewport.h"
#include "clock.h"

Hud::Hud(int x,int y,int l,int w) :
locX(x),
locY(y),
height(l),
width(w),
io(IOManager::getInstance()),
red(255),
green(129),
blue(0)
{}

void Hud::draw () const {
      
    drawHud();

    
}

void Hud::drawHud() const {
    Uint32 color = SDL_MapRGB(io.getScreen()->format, red, green, blue);
    Draw_AALine(io.getScreen(), locX, locY, locX+width, locY, 2.f, color); //top
    Draw_AALine(io.getScreen(), locX, locY+height, locX+width, locY+height, 2.f, color); //bottom
    Draw_AALine(io.getScreen(), locX, locY, locX, locY+height, 2.f, color); //left
    Draw_AALine(io.getScreen(), locX+width, locY, locX+width, locY+height, 2.f, color); //right

}
