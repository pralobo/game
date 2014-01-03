#include <iostream>
#include <string>
#include "sprite.h"
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "manager.h"
#include <cmath>
#include <algorithm>
#include "random.h"
#include "sound.h"
#include "smartSprite.h"

SDLSound sound_mgr;

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.

  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  
  std::list<Drawable*>::iterator oIt=objects.begin();
  while ( oIt != objects.end() ) {
   	delete *oIt;
        ++oIt;
  }
  std::list<ExplodingSprite*>::iterator expIt=explodingObjects.begin();
  while ( expIt != explodingObjects.end() ) {
    	delete *expIt;
    	++expIt;
  } 
  std::list<Drawable*>::const_iterator cit = chalices.begin();
  while ( cit != chalices.end() ) {
    delete *cit;
    ++cit;
  }


  

}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  helpCatch(true),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  avgFps(0),
  screen( io.getScreen() ),
  
  bworld("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  fworld("front", Gamedata::getInstance().getXmlInt("frontFactor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  BLUE(SDL_MapRGB(screen->format, 0x00, 0x00, 0xcd)),
  RED(SDL_MapRGB(screen->format, 0xff, 0, 0)),
  hud(Gamedata::getInstance().getXmlInt("hudLocX"),Gamedata::getInstance().getXmlInt("hudLocY"),Gamedata::getInstance().getXmlInt("hudHeight"),Gamedata::getInstance().getXmlInt("hudWidth")),
player(),
objects(),
chalices(),
explodingObjects(),
orbExploded(false),
bar(screen),
gameOver(false),
score(0),
createone(false)

{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);


  for(unsigned int i=0;i<5;i++)
		objects.push_back(new MultiSprite("bat"));
  objects.push_back(new SmartSprite("smartbat", player));
  sprites.reserve(2);
  sprites.push_back( new Sprite("cloud") );
  sprites.push_back( new Sprite("cloud") ); 
  viewport.setObjectToTrack(player.getSprite());
}



void Manager::draw() const {
  bworld.draw();
  fworld.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  player.draw();
  bar.draw();

  std::list<ExplodingSprite*>::const_iterator expIt=explodingObjects.begin();
  while ( expIt != explodingObjects.end() ) {
    (*expIt)->draw();
    ++expIt;
  }
  std::list<Drawable*>::const_iterator it = objects.begin();
  while ( it != objects.end() ) {
    (*it)->draw();
    ++it;
  }

  std::list<Drawable*>::const_iterator cit = chalices.begin();
  while ( cit != chalices.end() ) {
    (*cit)->draw();
    ++cit;
  }


  std::ostringstream oss;
  oss <<"Average FPS for the last "<<Gamedata::getInstance().getXmlInt("lastFrames")<<" frames: ";
    
  io.printMessageValueAt(oss.str(), avgFps, 10, 30);
  io.printMessageAt("Ghost's Health", 10, 10);
  io.printMessageValueAt("Score", score, 10, 80);
  if(helpCatch){
      hud.draw();
      printHelp();
  }
  if(gameOver){


 	std::ostringstream oss;
  	oss <<"Game over, your final score is "<<score;
	io.printMessageCenteredAt(oss.str(),300);
  }
  SDL_Flip(screen);
}
void Manager::createLife(){

	chalices.push_back(new Sprite("chalice"));
}
void Manager::update() { 
  ++clock;

  Uint32 ticks = clock.getElapsedTicks();
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }

  std::list<Drawable*>::iterator cit = chalices.begin();
  while ( cit != chalices.end() ) {
    (*cit)->update(ticks);

    		if(player.collision(*cit)) {
				bar.update(ticks, true, 0);
      				delete *cit;
      				cit = chalices.erase(cit);
				createone=false;
		}
		else{
    			++cit;
		}

  }

  player.update(ticks);
  
  std::list<Drawable*>::iterator it = objects.begin();









  while ( it != objects.end() ) {
    	(*it)->update(ticks);

    		if(player.collision(*it)) {
				
				bar.update(ticks, false, 0);
				player.cupdate(ticks);
		}


	if ( player.collidedWith((*it)) ){
		sound_mgr[2];
		explodingObjects.push_back(new ExplodingSprite(*it));

		
      		delete *it;
 		score++;
      		it = objects.erase(it);
		
	}
	else
		it++;
		
  }
        
  std::list<ExplodingSprite*>::iterator expIt=explodingObjects.begin();
  		while ( expIt != explodingObjects.end() ) {
    		(*expIt)->update(ticks);
		ExplodingSprite* exsprite = dynamic_cast<ExplodingSprite*>(*expIt);
		if(exsprite->chunkCount() == 0){	
	 			if((*expIt)->getName() == "smartbat"){
					delete exsprite;
		      			expIt = explodingObjects.erase(expIt);
		  			objects.push_back(new SmartSprite("smartbat",player));
				}
				else{
					delete exsprite;
					expIt = explodingObjects.erase(expIt);
		  			objects.push_back(new MultiSprite("bat"));
				}
					return;
		}else{			
		 ++expIt;
		}
 	 }
  bworld.update();
  fworld.update();
  viewport.update(); // always update viewport last*/ 
}
void Manager::printHelp() const{
  

  io.printMessageAt("**************Statistics**************", 10, 100);
  io.printMessageValueAt("->Seconds: ", clock.getSeconds(), 10, 120);

  io.printMessageValueAt("-->FPS : ", clock.getFps(), 10, 140);

  string message = "-->Tracking sprite: "+ sprites[currentSprite]->getName(); 
  io.printMessageAt(message, 10, 160);
 
  io.printMessageAt("**************Key Help**************", 10, 180);

  io.printMessageAt("-->Press up,down,left and right arrow keys to move ghost", 10, 200);
  io.printMessageAt("-->Press space bar to annihilate the bats", 10, 220);
  io.printMessageAt("-->Press Q to Quit", 10, 240);


  
  SDL_Flip(screen);
  
}

void Manager::setNumberOfSprites(int number) {

viewport.setObjectToTrack(player.getSprite());
  if ( number > static_cast<int>( objects.size() ) ) {
    number = (number - objects.size())/2;
    for (int i = 0; i < number; ++i) {
      objects.push_back( new MultiSprite("bat") );
      objects.push_back( new SmartSprite("smartbat",player) );
    }
  }
  else {
    number = objects.size() - number;
    for (int i = 0; i < number; ++i) {
      delete objects.back();
      objects.pop_back();
    }
  }
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;
  orbExploded = false;
  createone= false;
 
  int lastFrames=Gamedata::getInstance().getXmlInt("lastFrames"),sumFps=0;
  viewport.setObjectToTrack(player.getSprite());
  while ( not done ) {
    if(gameOver){ 
	done = true;
	SDL_Delay(5000); 
    }
    if(bar.getCurrentHealth() <= 0 ){
         sound_mgr[3];
	 gameOver=true; 
	       
	 
     }  
    
    if(bar.getCurrentHealth() >= 90 && bar.getCurrentHealth() <= 100){
	if(!createone){
		createone=true;
		createLife();
	}
    }
    if(lastFrames == 0){
     avgFps=sumFps/100;
     sumFps=clock.getFps();
     lastFrames=100;
    }
    else{
      --lastFrames;
      sumFps+=clock.getFps();
    }
  
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false;
      player.stop();
    }

    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
        currentSprite = (currentSprite+1) % sprites.size();
        viewport.setObjectToTrack(sprites[currentSprite]);
      }
      if (keystate[SDLK_s] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }
      if(keystate[SDLK_SPACE]){
	sound_mgr[1];
        player.shoot(); 
      }
      if(keystate[SDLK_F1] && !helpCatch){
        helpCatch=true; 
      }
      else{
	helpCatch=false;
      }
     if (keystate[SDLK_LEFT]) {
        player.goleft();
     }
     if (keystate[SDLK_RIGHT]) {
        player.goright();
     }
     if (keystate[SDLK_UP]) {
        player.goup();
     }
     if (keystate[SDLK_DOWN]) {
        player.godown();
     }
    }
      draw();
      update();
   
  }
}
