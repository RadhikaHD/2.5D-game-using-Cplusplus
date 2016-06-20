#ifndef EXPLODINGSPRITE_H
#define EXPLODINGSPRITE_H

#include <vector>
#include <list>
#include "chunk.h"


class explodingSprite: public Sprite {
public:
	explodingSprite (const Sprite& s);
	~explodingSprite();
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	void makeChunks(unsigned int n);
	unsigned int chunkCount() const 
	{
		return chunks.size();
	}
	
	unsigned int freeCount() const
	{
		return freeList.size();
	}
	
	
private:
	std::list<Chunk> chunks;
	std::list<Chunk> freeList;
	std::vector<Frame*> frames;
	explodingSprite(const explodingSprite&);
	explodingSprite& operator=(const explodingSprite&);
	
};

#endif
