#include "health.h"

Health::Health() :
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(20, 100)),
  totalLength(200),
  currentLength(200),
  thick(14),
  increments(20),
  interval(1000),
  deltaTime(0),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  color(RED) {
}

Health::Health(const std::string& name) :
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startX"),
		 Gamedata::getInstance().getXmlInt(name+"/startY") )
	),
  totalLength( Gamedata::getInstance().getXmlInt(name+"/totallength") ),
  currentLength( Gamedata::getInstance().getXmlInt(name+"/currentlength") ),
  thick( Gamedata::getInstance().getXmlInt(name+"/thick") ),
  increments( Gamedata::getInstance().getXmlInt(name+"/increments") ),
  interval( Gamedata::getInstance().getXmlInt(name+"/interval") ),
  deltaTime(0),
  RED( SDL_MapRGB(screen->format, 0x00, 0x00, 0xff) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  color( SDL_MapRGB(screen->format, Gamedata::getInstance().getXmlInt(name+"/red"),
  Gamedata::getInstance().getXmlInt(name+"/green"), Gamedata::getInstance().getXmlInt(name+"/blue")) )
  { }


Health::Health(int sx, int sy, int tl, int cl,
               float t, int inc, Uint32 c, float sp):
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(sx, sy)),
  totalLength(tl),
  currentLength(cl),
  thick(t),
  increments(inc),
  interval(sp),
  deltaTime(SDL_GetTicks()),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xff, 0xff, 0xff) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
    color(c) {
}

void Health::drawBox() const {
  Draw_AALine(screen, start[0], start[1],
                      start[0]+totalLength, start[1],
                      thick, GRAY);
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-8,
                      start[0]+totalLength, start[1]-8,
                      1.0, BLACK);
  Draw_AALine(screen, start[0], start[1]+8,
                      start[0]+totalLength, start[1]+8,
                      1.0, BLACK);
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-8,
                      start[0]-1, start[1]+8,
                      2.0, BLACK);
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-8,
                      start[0]+totalLength+1, start[1]+8,
                      2.0, BLACK);
}

void Health::draw() const {
  drawBox();
  Draw_AALine(screen, start[0], start[1],
                      start[0]+currentLength, start[1],
                      thick, color);
}
void Health::update(Uint32 ticks) {
  deltaTime += ticks;
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
    currentLength -= (increments);
  }
}

void Health::decrementlife() {
	if (currentLength > 0) currentLength -= increments;
}

void Health::increment() {
	if (currentLength < totalLength) currentLength += (increments*8);
}

void Health::incrementlife() {
	if (currentLength < totalLength) currentLength += increments;
}
