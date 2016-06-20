#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include "vector2f.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "player.h"
#include "extractSurface.h"
#include "smartSprite.h"
#include "sound.h"
#include "SDL/SDL_mixer.h"

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

SDLSound sound;

Manager::~Manager() {
  std::vector<Drawable*>::iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ptr=sprites.erase(ptr);

  }

  std::vector<ScaledSprite*>::iterator ptr1 = scaledSprites.begin();
 while ( ptr1 != scaledSprites.end() ) {
    delete (*ptr1);
    ptr1=scaledSprites.erase(ptr1);
  }

   std::vector<Drawable*>::const_iterator ptr2 = lives.begin();
  while ( ptr2 != lives.end() ) {
    delete (*ptr2);
    ++ptr2;
  }
    lives.clear();

}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  world1("back1", Gamedata::getInstance().getXmlInt("back1/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(),
  scaledSprites(),
  currentScaledSprite(),
  lives(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  HUD_WIDTH(Gamedata::getInstance().getXmlInt("hudwidth")),
  HUD_HEIGHT(Gamedata::getInstance().getXmlInt("hudheight")),
  HUDflag(Gamedata::getInstance().getXmlInt("hudflag")),
  HUDstartX(Gamedata::getInstance().getXmlInt("hudstartx")),
  HUDstartY(Gamedata::getInstance().getXmlInt("hudstarty")),
  win(false),
  lose(false),
  invincible(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

  sprites.push_back( new MultiSprite("mojo") );
  sprites.push_back( new player("blossom") );
  Sprite* playr = static_cast<Sprite*>( sprites[1] );

  unsigned int numberoflives = Gamedata::getInstance().getXmlInt("numberoflives");
  for (unsigned int i = 0; i< numberoflives; i++)
  {
	  lives.push_back(new SmartSprite("chemicalX", *playr));
  }

  currentSprite = sprites.begin();
  ++currentSprite;
  viewport.setObjectToTrack(*currentSprite);

  unsigned int numberOfFlakes = Gamedata::getInstance().getXmlInt("numberOfFlakes");
  scaledSprites.reserve( numberOfFlakes );
  currentScaledSprite = scaledSprites.begin();

  for (unsigned int i = 0; i< numberOfFlakes; i++)
	{
	  scaledSprites.push_back( new ScaledSprite("flake") );
	}
  sort(scaledSprites.begin(), scaledSprites.end(), ScaledSpriteCompare());
}

void Manager::drawHUD(SDL_Surface* screen, int x, int y) const {

  Draw_AALine(screen, x, y+HUD_HEIGHT/2,
                      x+HUD_WIDTH,y+HUD_HEIGHT/2,
                      HUD_HEIGHT, 0xff, 0xff, 0xff, 0xff/2);

  io.printMessageAt("Press T to switch sprites", 10, 170);
  io.printMessageAt("Press W to move Blossom up", 10, 195);
  io.printMessageAt("Press A to move Blossom left", 10, 220);
  io.printMessageAt("Press S to move Blossom down", 10, 245);
  io.printMessageAt("Press D to move Blossom right", 10, 270);
  io.printMessageAt("Press R to reset", 10, 295);
  io.printMessageAt("Press G to toggle invincibility", 10, 320);
  io.printMessageAt("Press SPACE to shoot", 10, 345);

}

 void Manager::toggleInvincible()
  {
      if(invincible==true)
		{
			invincible= false;
		}
      else
		{
			invincible = true;
		}
        
  }

void Manager::draw() const {

  world.draw();
   std::vector<ScaledSprite*>::const_iterator ptr1 = scaledSprites.begin();
  while ( ptr1 != scaledSprites.end() ) {
    (*ptr1)->draw();
    ++ptr1;
  }
  world1.draw();
  if(HUDflag==1)
  {
	drawHUD(screen, HUDstartX, HUDstartX);
  }
  clock.draw();
  static_cast<player* >(sprites[1])->HUD();

  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }

  std::vector<Drawable*>::const_iterator ptr3 = lives.begin();
  while ( ptr3 != lives.end() ) {
    (*ptr3)->draw();
    ++ptr3;
  }

  io.printMessageAt("Press F1 to toggle instructions", 10, 120);
  io.printMessageAt(title, 250, 10);

  if (static_cast<player*>(sprites[1])->isHealthEmpty()==true)
  {
      static_cast<player*>(sprites[1])->setVelocity(Vector2f(0,0));
      static_cast<player*>(sprites[1])->setPosition(Vector2f(300,70));
      sprites[0]->setVelocity(Vector2f(0,0));
      sprites[0]->setPosition(Vector2f(400,70));
      io.printMessageAt("YOU LOSE!", 400, 250);
      io.printMessageAt("Press R to restart", 375, 275);
  }

  if(win==true)
  {
      static_cast<player*>(sprites[1])->setVelocity(Vector2f(0,0));
      static_cast<player*>(sprites[1])->setPosition(Vector2f(300,100));
      io.printMessageAt("So once again, the day is saved, thanks to The Powerpuff Girls!", 170, 250);
      io.printMessageAt("Press R to restart", 375, 275);
      static_cast<player*>(sprites[1])->resetHealth();
  }
  
  if (invincible == true)
  io.printMessageAt("Blossom is invincible now", 320, 50);

  viewport.draw();

  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.'
       << std::setfill('0') << std::setw(4)
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  ++currentSprite;
  if ( currentSprite == sprites.end() ) {
    currentSprite = sprites.begin();
  }
  viewport.setObjectToTrack(*currentSprite);
}

void Manager::collisionOccured()
{	 //for bullets
	 std::vector<Drawable*>::const_iterator ptr = sprites.begin();
	 if (static_cast<player* >(sprites[1])->bulletCollision(*ptr))
	 {
         sound[5];
		 sprites[0]->explode();
         win = true;
	 }
}

void Manager::playerCollided()
 {  //for player

	std::vector<Drawable*>::const_iterator ptr = sprites.begin();

		if ( static_cast<player*>(sprites[1])->collidedWith((*ptr) ) )
		{
			static_cast<player*>(sprites[1])->decrementHealthBar();
            sound[2];
		}

    std::vector<Drawable*>::iterator itr = lives.begin();
    std::vector<Drawable*>::iterator itr1 = lives.end();
	while ( itr != itr1 ) {
		if ( static_cast<player*>(sprites[1])->collidedWith((*itr) ) )  {
            sound[2];
            static_cast<player*>(sprites[1])->resetHealth();
            itr1 = lives.erase(itr);
	    }
        else
		++itr;
	}

}


void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  if (invincible ==true)
  static_cast<player*>(sprites[1])->resetHealth();

  std::vector<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }

   std::vector<ScaledSprite*>::const_iterator ptr1 = scaledSprites.begin();
  while ( ptr1 != scaledSprites.end() ) {
    (*ptr1)->update(ticks);
    ++ptr1;
  }

   std::vector<Drawable*>::const_iterator ptr2 = lives.begin();
  while ( ptr2 != lives.end() ) {
    (*ptr2)->update(ticks);
    ++ptr2;
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }

  world.update();
  world1.update();

   if (clock.getSeconds() == 5)
   {
		HUDflag = 0;
   }
  collisionOccured();
  playerCollided();

  if (invincible ==true)
  static_cast<player*>(sprites[1])->resetHealth();

  viewport.update(); // always update viewport last

}

bool Manager::play() {
  SDL_Event event;
  bool done = false;
  bool restart = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_w] ) {
          //forward
           static_cast<player* >(sprites[1])->up();


        }
        if ( keystate[SDLK_a] ) {
			//left
           static_cast<player* >(sprites[1])->left();

        }
        if ( keystate[SDLK_s] ) {
			//backward
          static_cast<player* >(sprites[1])->down();

        }
        if ( keystate[SDLK_d] ) {
			//right
           static_cast<player* >(sprites[1])->right();

        }
        if ( keystate[SDLK_m] ) {
				sprites[0]->explode();
        }
        if ( keystate[SDLK_b] ) {
				sprites[1]->explode();
        }
         if ( keystate[SDLK_SPACE] ) {
				static_cast<player* >(sprites[1])->shoot();
                sound[3];
        }
        if ( keystate[SDLK_r] ) {
                restart = true;
                done = true;
                break;
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_s]) {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_g]) {
          toggleInvincible();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_F1]) {

			if (HUDflag)
			{
				HUDflag = 0;
       	    }
			else
			{
				HUDflag = 1;
			}
        }

      }
    }
    draw();
    update();
  }
  return restart;
}
