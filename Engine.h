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
	RenderWindow m_Window;
	bool soundPlayed = false;
	bool hit = false;
	SoundBuffer buffer;
	Sound sound;
	Font font;
	Text text;
	Sprite sprite;
	Texture texture;
	RectangleShape GameRect;
	vector<Particle> m_particles;
	mutex particles_mutex;
	atomic<bool> timeStop{false};
	atomic<int> Game{0};
	float TimePart = 1;
	float SpriteY = 500;
	float m_framerate;
	int SpriteNeg = 1;
	int thread_count;
	int count = 0;
	// Private functions for internal use only
	void loadText(Text& text);
	void input();
	void updateParticleRange(size_t start, size_t end, float dtAsSeconds);
	void update(float dtAsSeconds);
	void MakeSprite();
	void MoveSprite(float dt);
	void draw();
	void MakeNoise();
	void Game3Part(float dt);
	

public:
	// The Engine constructor
	Engine();
	float getFramerate() {return m_framerate;}
	int getThreadCount() {return thread_count;}
	int setThreadCount(int t) {return thread_count = t;}
	

	//run() calls private functions
	void run();

};
