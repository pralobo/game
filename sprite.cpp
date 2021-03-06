#include <iostream>
#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "collisionStrategy.h"

Sprite::~Sprite() { delete strategy; } 
int Sprite::randBetween(int low, int high) {
   return ((rand() % (high - low)) + low);
}

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"), 
                    Gamedata::getInstance().getXmlInt(name+"Y")), 
           Vector2f((rand()%2?1:-1)*randBetween(Gamedata::getInstance().getXmlInt("cloudSpeedMaxX"),Gamedata::getInstance().getXmlInt("cloudSpeedMinX")), 
                   0) 
           ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  strategy( new MidPointCollisionStrategy )
{ }

Sprite::Sprite(const std::string& n, 
               const Vector2f& pos, const Vector2f& vel, 
               const Frame* fm) :
  Drawable(n, pos, vel), 
  frame(fm),
  
  frameWidth(fm->getWidth()),
  frameHeight(fm->getHeight()),
    worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  strategy( new MidPointCollisionStrategy )

{ }


Sprite::Sprite(const Sprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  strategy( new MidPointCollisionStrategy )
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  strategy = new MidPointCollisionStrategy;
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
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
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}
