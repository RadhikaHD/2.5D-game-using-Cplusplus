#ifndef player__H
#define player__H
#include <string>
#include <vector>
#include <list>
#include "drawable.h"
#include "frameFactory.h"
#include "bullet.h"
#include "ioManager.h"
#include "health.h"

class explodingSprite;
class player : public Drawable {
public:
  player(const std::string&, const std::vector<Frame*>&);
  player(const std::string& n);
  player(const player&);
  virtual ~player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void up();
  void down();
  void left();
  void right();
  void reset();
  void shoot();
  void HUD();

  virtual void decrementHealthBar() {
      healthBar.decrementlife();
    }

    virtual void incrementHealthBar() {
      healthBar.increment(); 
    }

  virtual bool isHealthEmpty() {
      return healthBar.isEmpty();
    }

  virtual void resetHealth() {
	   healthBar.reset();
	}

	bool collidedWith(const Drawable* d) const {
		return strategy1->execute(*this, *d);
	}

  virtual const Frame* getFrame() const {
    return frames[currentFrame];
  }

  void explode();
  bool bulletCollision(const Drawable* d);


protected:
  explodingSprite* explosion;
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
  std::list<Bullet> freeList;
  std::list<Bullet> bulletList;

  Health healthBar;
  CollisionStrategy* strategy1;

  void advanceFrame(Uint32 ticks);
  player& operator=(const player&);
};
#endif
