// Brian Malloy        Game Construction
#include "manager.h"
#include "smartSprite.h"
#include "SDL/SDL_mixer.h"
#include "sound.h"

float SmartSprite::
  safeDistance  = Gamedata::getInstance().getXmlInt("safeDistance");

int main(int, char*[]) {
    bool res = true;
    while (res)
    {
   try {
      Manager manager;
      res = manager.play();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   }

   return 0;
}
