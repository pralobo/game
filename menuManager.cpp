#include <cmath>
#include <sstream>
#include "menuManager.h"
#include "manager.h"
#include "sound.h"

 Manager manager;
 SDLSound sound;

MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  menuBg("menuBack"),
  clock( Clock::getInstance() ),
  backColor(),
  menu(),
  viewport( Viewport::getInstance() ),
  safeDistance(200)
{ 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = Gamedata::getInstance().getXmlInt("Red");
  backColor.g = Gamedata::getInstance().getXmlInt("Green");
  backColor.b = Gamedata::getInstance().getXmlInt("Blue");
  atexit(SDL_Quit); 
}

void MenuManager::drawBackground() const {
  //viewport.draw();
  
  menuBg.draw();
}

void MenuManager::displayHelp(){

   IOManager& io = IOManager::getInstance().getInstance();
   SDL_Event event;
   bool done = false;
   io.clearString();
 while ( not done ) {
 
 
   	viewport.update();
    	drawBackground();
    	io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("helpMsg1"), Gamedata::getInstance().getXmlInt("helpMsgLoc"));
	io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("helpMsg2"), Gamedata::getInstance().getXmlInt("helpMsgLoc")+20);
	io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("helpMsg3"), Gamedata::getInstance().getXmlInt("helpMsgLoc")+50);
	io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("helpMsg4"), Gamedata::getInstance().getXmlInt("helpMsgLoc")+80);
	io.printMessageCenteredAt(Gamedata::getInstance().getXmlStr("helpMsg5"), Gamedata::getInstance().getXmlInt("helpMsgLoc")+110);
    	viewport.setObjectToTrack(&menuBg);
    	SDL_Flip(screen);


    	SDL_PollEvent(&event);
   	
    	if (event.type ==  SDL_QUIT) { break; }
    		if(event.type == SDL_KEYDOWN) {
      			switch ( event.key.keysym.sym ) {
        			case SDLK_ESCAPE :
        				case SDLK_q : {
          						done = true;
          						break;
        					}
				 default: break;
			}
		}
        }




}
void MenuManager::setLevel() {
  bool keyCatch = false;
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  //bool nameDone = false;
  const string msg("Choose Difficulty Level:");
  io.clearString();
 while ( not done ) {
 
 
   viewport.update();
    drawBackground();
    io.printStringAfterMessage(msg, 200, 180);
    viewport.setObjectToTrack(&menuBg);
    menu.drawsub();
    SDL_Flip(screen);


    SDL_PollEvent(&event);
   
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE :
        case SDLK_q : {
          done = true;
          break;
        }
        case SDLK_RETURN : {
          if ( !keyCatch ) {
            menu.lightOn();
            if ( menu.getsubIconClicked() == "Easy" ) {  
            	std::cout<<"Chose Easy"<<std::endl;
		manager.setNumberOfSprites(5);
		done = true;
            }
            if ( menu.getsubIconClicked() == "Medium" ) {
                std::cout<<"Chose Medium"<<std::endl;
		manager.setNumberOfSprites(10);
 		done = true;
              // After we get the parameter, we must pass it to Manager:
             // manager.setNumberOfOrbs( numberOfOrbs );
            }
            if ( menu.getsubIconClicked() == "Hard" ) {
              // Here is where we explain how to play the game"
              std::cout << "Hard" << std::endl;
		manager.setNumberOfSprites(15);
		done = true;
            }
          
          }
          break;
        }
        case SDLK_DOWN   : {
          if ( !keyCatch ) {
            menu.incrementsub();
	    sound[0];
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrementsub();
	    sound[0];
          }
          break;
        }
        default          : break;
      }
      keyCatch = true;
    }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
      menu.lightOff();
    }
  }



}

void MenuManager::play() {
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event event;
  bool done = false;
  // Here, we need to create an instance of the Manager,
  // the one that manages the game:
 

  while ( not done ) {
 
 
   viewport.update();
    drawBackground();
    viewport.setObjectToTrack(&menuBg);
    menu.draw();
    SDL_Flip(screen);


    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE :
        case SDLK_q : {
          done = true;
          break;
        }
        case SDLK_RETURN : {
          if ( !keyCatch ) {
            menu.lightOn();
            if ( menu.getIconClicked() == Gamedata::getInstance().getXmlStr("words0") ) {
              // Here is where we call the play() function in Manager:
              manager.unpause();
              manager.play();
              manager.pause();
              	 
            }
            if ( menu.getIconClicked() == Gamedata::getInstance().getXmlStr("words1") ) {
              	setLevel();
		manager.pause();
              // After we get the parameter, we must pass it to Manager:
             // manager.setNumberOfOrbs( numberOfOrbs );
            }
            if ( menu.getIconClicked() == Gamedata::getInstance().getXmlStr("words2") ) {
              // Here is where we explain how to play the game"
		displayHelp();
		manager.pause();
             
            }
            if ( menu.getIconClicked() == Gamedata::getInstance().getXmlStr("words3") ) {
              viewport.update();
              drawBackground();
              menu.draw();
              SDL_Flip(screen);
              SDL_Delay(250);
              done = true;
            }
          }
          break;
        }
        case SDLK_DOWN   : {
          if ( !keyCatch ) {
            menu.increment();
	    sound[0];
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrement();
	    sound[0];
          }
          break;
        }
        default          : break;
      }
      keyCatch = true;
    }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
      menu.lightOff();
    }
  }
}
