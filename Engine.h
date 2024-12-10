#pragma once
#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
	bool soundPlayed = false;
	bool hit = false;
	SoundBuffer buffer;
	Sound sound;
	//Text
	Text text;
	float TimePart = 1;
	Font font;
	float SpriteY = 500;
	// A regular RenderWindow
	float m_framerate;
	RenderWindow m_Window;
	Sprite sprite;
	Texture texture;
	int SpriteNeg = 1;
	//vector for Particles
	vector<Particle> m_particles;
	mutex particles_mutex;
	atomic<bool> timeStop{false};
	atomic<int> Game{0};
	// Private functions for internal use only
	void updateParticleRange(size_t start, size_t end, float dtAsSeconds);
	void loadText(Text& text);
	void input();
	void update(float dtAsSeconds);
	void draw();
	void MakeNoise();

public:
	// The Engine constructor
	Engine();
	float getFramerate() {return m_framerate;}
	void MakeSprite();
	void MoveSprite(float dt);
	// Run will call all the private functions
	void run();

};
