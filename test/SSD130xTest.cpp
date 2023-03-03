#include <coco/SSD130x.hpp>
#include <coco/font/tahoma_8pt.hpp>
#include <coco/debug.hpp>
#include "SSD130xTest.hpp"


Coroutine draw(Loop &loop, Buffer &buffer) {
	const int width = 128;
	const int height = 64;
	SSD130x display(buffer, width, height);
	co_await display.init();
	co_await display.enable();
	
	Bitmap bitmap = display.bitmap();
	int x = 0;
	int y = 0;
	while (true) {
		bitmap.clear();
		bitmap.drawText(0, 0, tahoma_8pt, "Hello World!");
		bitmap.drawRectangle(x, y, 10, 10);
		x = (x + 1) & (width - 1);
		y = (y + 1) & (height - 1);	

		co_await display.display();
		co_await loop.sleep(200ms);
		
		debug::toggleRed();
	}
}


int main(void) {
	debug::init();
	Drivers drivers;

	draw(drivers.loop, drivers.display);
	
	drivers.loop.run();
}
