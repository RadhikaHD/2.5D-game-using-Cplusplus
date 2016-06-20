#ifndef Bullet__H
#define Bullet__H
#include <string>
#include <vector>
#include "drawable.h"
#include "frameFactory.h"
#include "collisionStrategy.h"

class Bullet : public Drawable {
public:
  Bullet(const std::string&,
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  Bullet(const std::string&, const Vector2f& pos, const Vector2f& vel);
  Bullet(const std::string& n);

  Bullet(const Bullet& s);
  virtual ~Bullet();
  Bullet& operator=(const Bullet&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);

  bool goneTooFar() const
	{
		return tooFar;
	}

	void reset()
	{
		tooFar = false;
		distance = 0;
	}

   bool collidedWith(const Drawable* d) const;
   bool checkForCollisions() const;


private:

  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Bullet*) const;
  float distance;
  float maxdistance;
  bool tooFar;
  CollisionStrategy * strategy;
};
#endif
