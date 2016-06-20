#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "frameFactory.h"
#include "aaline.h"
#include "scaledSprite.h"
#include "extractSurface.h"
#include "explodingSprite.h"
#include "smartSprite.h"
#include "player.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  bool play();
  void switchSprite();
  void collisionOccured();
  void playerCollided();
  void toggleInvincible();
 

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World world;
  World world1;
  Viewport& viewport;

  std::vector<Drawable*> sprites;
  std::vector<Drawable*>::iterator currentSprite;
  std::vector<ScaledSprite*> scaledSprites;
  std::vector<ScaledSprite*>::iterator currentScaledSprite;
  std::vector<Drawable*> lives;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  int HUD_WIDTH;
  int HUD_HEIGHT;
  int HUDflag;
  int HUDstartX;
  int HUDstartY;
  bool win;
  bool lose;
  bool invincible;

  void draw() const;
  void update();
  void drawHUD(SDL_Surface* screen, int x, int y) const;

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
