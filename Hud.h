#include <iostream>
#include <cmath>
#include "ioManager.h"
#include "vector2f.h"


class Hud{
	public:
		void draw() const;
		Hud(int,int,int,int);

	private:
		Hud();
		Hud(Hud&);
		Hud& operator=(Hud&);


                int locX,locY,height,width;

		const IOManager& io;
		const Uint8 red;
		const Uint8 green;
		const Uint8 blue;
		void drawHud() const;	
};
