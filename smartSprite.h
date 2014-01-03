#ifndef SMARTSPRITE_H
#define SMARTSPRITE_H
#include <iostream>
#include "multisprite.h"
#include "ioManager.h"
#include "player.h"

class SmartSprite : public MultiSprite {
public:
  SmartSprite(const string& name, 
              const Player& p) :
   MultiSprite(name), 
   io(IOManager::getInstance()), 
   enemy(p), 
   currentMode(NORMAL) { }

  SmartSprite(const SmartSprite &s) :
   MultiSprite(s.getName()), 
   io(IOManager::getInstance()), 
   enemy(s.enemy),
   currentMode(NORMAL) { }
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  
  static void incrSafeDistance() {++safeDistance; }
  static void decrSafeDistance() { --safeDistance; }
  static float getSafeDistance() { return safeDistance; }

  void draw();
  void update(Uint32 ticks);
  enum MODE {NORMAL, ATTACK};
private:	
  IOManager& io;	
  const Player & enemy;
  float static safeDistance;
  MODE currentMode;
  SmartSprite& operator=(const SmartSprite & rhs);
};


#endif
