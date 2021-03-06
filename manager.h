#include <SDL.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "sprite.h"
#include "world.h"
#include "viewport.h"
#include "multisprite.h"
#include "player.h"
#include "explodingSprite.h"
#include "Hud.h"
#include "aaline.h"
#include "health.h"
class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void pause() { clock.pause(); }
  void unpause() { clock.unpause(); }
  void setNumberOfSprites(int);
private:
  const bool env;
  bool helpCatch;
  const IOManager& io;
  Clock& clock;
  int avgFps;
  SDL_Surface * const screen;
  World bworld;
  World fworld;
  Viewport& viewport;  

  std::vector<Drawable*> sprites;
  int currentSprite;
  const Uint32 BLUE;
  const Uint32 RED;
  Hud hud;
  Player player;


  std::list<Drawable*> objects;
  std::list<Drawable*> chalices;
  std::list<ExplodingSprite*> explodingObjects;
  void draw() const;
  void update();
  void printHelp() const;
  void makeBhoot();
  void createLife(); 
  bool orbExploded;
  Health bar;
  bool gameOver;
  int score;
  bool createone;
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
