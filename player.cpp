#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

player::~player()
{
  //for ( unsigned int i = 0; i < frames.size(); ++i ) {
  //  delete frames[i];
  //}
  bulletList.clear();
  freeList.clear();

 /*
    std::list<Bullet>::iterator iter1 = bulletList.begin();
	while ( iter1 != bulletList.end() ) {
				iter1 = bulletList.erase(iter1);
				++iter1;
			}

    std::list<Bullet>::iterator iter2 = bulletList.begin();
  	while ( iter2 != freeList.end() ) {
				iter2 = freeList.erase(iter1);
				++iter2;
			}
  */

}

void player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		if(velocityX()>0)
		{
			currentFrame = (currentFrame+1) % (numberOfFrames/2);
        }
		else
		{
			currentFrame = ((currentFrame+1) % (numberOfFrames/2)) + numberOfFrames/2;
		}

		timeSinceLastFrame = 0;
	}

}

player::player( const std::string& name,
                          const std::vector<Frame*>& f) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  frames( f ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  leftOrRight(0),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  freeList(),
  bulletList(),
  healthBar(Gamedata::getInstance().getXmlStr(name + "/healthhud")),
  strategy1(new MidPointCollisionStrategy)
{}

player::player(const std::string& name):
Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  leftOrRight(0),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  freeList(),
  bulletList(),
  healthBar(Gamedata::getInstance().getXmlStr(name + "/healthhud")),
  strategy1(new MidPointCollisionStrategy)
{}

player::player(const player& s) :
  Drawable(s),
  explosion(s.explosion),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  leftOrRight(s.leftOrRight),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  freeList(s.freeList),
  bulletList(s.bulletList),
  healthBar(),
  strategy1(new MidPointCollisionStrategy)
  { }

void player::draw() const {
    if (explosion) {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
  std::list<Bullet>::const_iterator iter = bulletList.begin();
		while ( iter != bulletList.end() ) {
			iter->draw();
			++iter;
		}
  healthBar.draw();
}

void player::explode() {
  if ( explosion ) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new explodingSprite(sprite);
}

void player::down()  {

	velocityY( abs( velocityY() ) );
}

void player::up()  {

	velocityY( -abs( velocityY() ) );
}

void player::left()  {

	velocityX( -abs( velocityX() ) );
    leftOrRight = 2;
}

void player::right()  {

	velocityX( abs( velocityX() ) );
    leftOrRight = 1;
}

void player::reset()  {
	setPosition( Vector2f(120, 140));
}

void player::HUD()  {
	IOManager::getInstance().printMessageValueAt("Total Free Bullets: ", freeList.size(), 10, 65);
	IOManager::getInstance().printMessageValueAt("Total Bullets In Use: ", bulletList.size(), 10, 85);

}

void player::shoot() {
	float x,velx,vely;
	float y = Y()-25.0;
	if ( freeList.empty() ) {
	string name =  "bullet";
	if(velocityX()>0)
		{
			x = X() + frameWidth;
			velx = (Gamedata::getInstance().getXmlInt("bullet/speedX"))+velocityX();
			vely = (Gamedata::getInstance().getXmlInt("bullet/speedY"))+velocityY();
			Bullet b(name,Vector2f(x,y),Vector2f(velx,vely));
			bulletList.push_back(b);
		}
		else
		{
			x = X() - frameWidth - 100.0;
			velx = ((Gamedata::getInstance().getXmlInt("bullet/speedX"))+velocityX());
			vely = (Gamedata::getInstance().getXmlInt("bullet/speedY"))+velocityY();
			Bullet b(name,Vector2f(x,y),Vector2f(velx,vely));
			bulletList.push_back(b);
		}
  }
  else {
		  if(velocityX()>0)
	  	{
	  		x = X() + frameWidth;
	  		velx = (Gamedata::getInstance().getXmlInt("bullet/speedX"))+velocityX();
	  		vely = (Gamedata::getInstance().getXmlInt("bullet/speedY"))+velocityY();
	  	}
	  	else
	  	{
	  		x = X() - frameWidth - 100.0;
	  		velx = ((Gamedata::getInstance().getXmlInt("bullet/speedX"))+velocityX());
	  		vely = (Gamedata::getInstance().getXmlInt("bullet/speedY"))+velocityY();
	  	}
		string name1 =  "bullet";
		Bullet b = freeList.front();
	  	freeList.pop_front();
	  	b.reset();
	  	b.setVelocity(Vector2f(velx,vely));
	  	b.setPosition(Vector2f(x,y));
	  	bulletList.push_back(b);
	  }
}

bool player::bulletCollision(const Drawable* d)
{
	if ( bulletList.empty())
	{
		return false;

    }

    std::list<Bullet>::iterator iter = bulletList.begin();
    std::list<Bullet>::iterator iter1 = bulletList.end();
		while ( iter != iter1 ) {
			if ( iter->collidedWith(d) ) {
				freeList.push_back(*iter);
				iter1 = bulletList.erase(iter);
				return true;
			}
			else {
				++iter;
			}
		}
	return false;

}

void player::update(Uint32 ticks) {
    if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );

  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );

  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
     leftOrRight = 1;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
    leftOrRight = 2;
  }

    std::list<Bullet>::iterator iter1 = bulletList.begin();
	while ( iter1 != bulletList.end() ) {
		iter1->update(ticks);
			if ( iter1->goneTooFar() ) {
				freeList.push_back(*iter1);
				iter1 = bulletList.erase(iter1);
				}
			else ++iter1;
 }

  healthBar.update(ticks);

}
