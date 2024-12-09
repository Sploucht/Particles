#pragma once
#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;
	Sprite sprite;
	Texture texture;
	//vector for Particles
	vector<Particle> m_particles;
	mutex particles_mutex;
	atomic<bool> timeStop{false};
	atomic<int> Game{0};
	// Private functions for internal use only
	void updateParticleRange(size_t start, size_t end, float dtAsSeconds);
	void input();
	void update(float dtAsSeconds);
	void draw();

public:
	// The Engine constructor
	Engine();
	void MakeSprite();
	void MoveSprite();
	// Run will call all the private functions
	void run();

};
