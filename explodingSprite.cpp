#include <iostream>
#include <cmath>
#include "explodingSprite.h"

explodingSprite::explodingSprite (const Sprite& s):
Sprite(s),
chunks(),
freeList(),
frames()
{
makeChunks(Gamedata::getInstance().getXmlInt(s.getName()+"/chunk/size"));
}


	explodingSprite::~explodingSprite()
	{
		for(unsigned int i = 0; i< frames.size(); i++)
		{
			delete frames[i];
		}
		chunks.clear();
		freeList.clear();
	}
	
	 void explodingSprite::draw() const
	{
		std::list<Chunk>::const_iterator iter= chunks.begin();
		while(iter != chunks.end())
		{
			iter->draw();
			++iter;
		}
	}
	
	 void explodingSprite::update(Uint32 ticks)
	{
		
		std::list<Chunk>::iterator iter1 = chunks.begin();
		while (iter1 != chunks.end())
		{
			iter1->update(ticks);
			if(iter1->goneTooFar())
			{
				freeList.push_back(*iter1);
				iter1 = chunks.erase(iter1);
			}
			else
			++iter1;
		}
		
	}
	
	void explodingSprite::makeChunks(unsigned int n) { 
  // Break the SDL_Surface into n*n squares; where each square
  // has width and height of frameWidth/n and frameHeight/n
  unsigned int chunk_width = std::max(1u, getFrame()->getWidth()/n);
  unsigned int chunk_height = std::max(1u, getFrame()->getHeight()/n);
  int speedx = static_cast<int>(velocityX()); // Wanna test for zero...
  int speedy = static_cast<int>(velocityY()); // Make sure it's an int.
  if (speedx == 0) speedx = 1; // Make sure it's not 0;
  if (speedy == 0) speedy = 1; // Make sure it's not 0;

  // Get the SDL_Surface so we can chunk it:
  SDL_Surface* spriteSurface(getFrame()->getSurface()); 
  Sint16 source_y = getFrame()->getSourceY();
  while  (source_y < getFrame()->getHeight() ) {
    Sint16 source_x = getFrame()->getSourceX();
    while ( source_x < getFrame()->getWidth() ) {
      // Give each chunk it's own speed/direction:
      float sx = (rand() % speedx + 40) * (rand()%2?-1:1); // 'cause %0 is 
      float sy = (rand() % speedy + 40) * (rand()%2?-1:1); // float except
      Frame* frame = 
            new Frame(spriteSurface, chunk_width, chunk_height,
                  source_x, source_y
                );
      Chunk chunk(
                Vector2f(X()+source_x,   // x coord of destination 
                         Y()+source_y), // y coord of destination
                Vector2f(sx, sy),
                getName()+"/chunk",
                frame);
      // chunks uses value semantics, as does frames, but there's
      // a big difference:
      chunks.push_back(chunk);
      frames.push_back(frame);
      source_x += chunk_width;
    }
    source_y += chunk_height;
  }
}

