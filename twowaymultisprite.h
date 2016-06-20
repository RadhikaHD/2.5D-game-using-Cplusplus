#ifndef twowaymultisprite__H
#define twowaymultisprite__H
#include <string>
#include <vector>
#include "drawable.h"
#include "frameFactory.h"

class twowaymultisprite : public Drawable {
public:
  twowaymultisprite(const std::string&, const std::vector<Frame*>&);
  twowaymultisprite(const std::string& n);
  virtual ~twowaymultisprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;
  

  unsigned currentFrame;
  unsigned numberOfFrames;
  int leftOrRight;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  twowaymultisprite(const twowaymultisprite&);
  twowaymultisprite& operator=(const twowaymultisprite&);
};
#endif
