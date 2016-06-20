#include <cmath>
#include "bullet.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::~Bullet()
{
   //delete strategy; deleting strategy causes segmentation fault
}

Bullet::Bullet(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  distance(0),
  maxdistance(Gamedata::getInstance().getXmlInt(n+ "/distance")),
  tooFar(false),
  strategy(new PerPixelCollisionStrategy)
{}

Bullet::Bullet(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel),
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  distance(0),
  maxdistance(Gamedata::getInstance().getXmlInt(n+ "/distance")),
  tooFar(false),
  strategy(new PerPixelCollisionStrategy)
{}

Bullet::Bullet(const std::string& n):
  Drawable(n, Vector2f(Gamedata::getInstance().getXmlInt(n+"/srcX"),Gamedata::getInstance().getXmlInt(n+"/srcY")), Vector2f(Gamedata::getInstance().getXmlInt(n+ "/speedX"),Gamedata::getInstance().getXmlInt(n+ "/speedY"))),
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  distance(0),
  maxdistance(Gamedata::getInstance().getXmlInt(n+ "/distance")),
  tooFar(false),
  strategy(new PerPixelCollisionStrategy)
{}

Bullet::Bullet(const Bullet& s) :
  Drawable(s),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  distance(s.distance),
  maxdistance(s.maxdistance),
  tooFar(s.tooFar),
  strategy(s.strategy)
{ }

Bullet& Bullet::operator=(const Bullet& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  distance = rhs.distance;
  maxdistance = rhs.maxdistance;
  tooFar = rhs.tooFar;
  strategy = rhs.strategy;
  return *this;
}

void Bullet::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

int Bullet::getDistance(const Bullet *obj) const {
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Bullet::update(Uint32 ticks) {

    Vector2f pos = getPosition();
    float incrx = velocityX() * static_cast<float>(ticks) * 0.002;
    float incry = 0.0;
    Vector2f incr = Vector2f(incrx,incry);
    setPosition(pos+incr);
    if ( X() == 0) {
        tooFar = true;
    }
    if ( X() == worldWidth-frameWidth) {
        tooFar = true;
    }
    float yincr = velocityY() * static_cast<float>(ticks) *0.001;
    float xincr = velocityX() * static_cast<float>(ticks) *0.001;
    distance = distance + (hypot(xincr, yincr));
	if( distance > maxdistance)
	tooFar = true;
}

bool Bullet::collidedWith(const Drawable* d) const {
 return strategy->execute(*this, *d);
}

