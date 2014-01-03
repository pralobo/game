#ifndef MULTISPRITE__H
#define MULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>
using std::string;

#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const string&);

  MultiSprite(const std::string&, const Vector2f& pos, const Vector2f& vel,
              const std::vector<Frame*>& fms);

  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  float dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
#endif
