#ifndef PLAYER_H
#define PLAYER_H

#include "gamedata.h"
#include "shootingSprite.h"


class Player{
public:
	Player();
	virtual ~Player();
	
	virtual void update(Uint32 ticks);
	virtual void cupdate(Uint32 ticks);
	virtual void draw() const{ ghost->draw();}
	const Drawable* getSprite() const { return ghost; }

	const Frame* getFrame() const { return ghost->getFrame(); }
	bool collidedWith(const Drawable*) const;
	bool collision(const Drawable*) const;
	void stop();
	void shoot();
        void goright();
  	void goleft();
  	void goup();
  	void godown();
	float X() const;
	float Y() const;

private:
  	const Gamedata& gdata;
  	bool keyPressed;
  	int worldWidth;
  	int worldHeight;
  	Vector2f ghostInitialVelocity;
  	float ghostWidth;
  	float ghostHeight;
  	SDL_Surface *ghostSurface;
  	std::vector<Frame*> ghostFrames;
  	std::vector<Frame*> ghostFramesLeft;
        std::vector<Frame*> ghostCFrames;
        std::vector<Frame*> ghostCFramesLeft;
  	ShootingSprite* ghost;
	CollisionStrategy* strategy;
  	void makeghost();
  	Player(const Player&);
  	Player& operator=(const Player&);
	

};
#endif
