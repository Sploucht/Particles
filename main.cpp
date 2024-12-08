#include "Engine.h"

int main()
{
	// Declare an instance of Engine
	Engine engine;
	// Start the engine
	engine.run();
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Chaos Game!!", Style::Default);
	Particle P(window, 4, Vector2i(1,1));
	P.unitTests();
	// Quit in the usual way when the engine is stopped
	return 0;
}
