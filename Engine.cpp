#include "Engine.h"

void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		//Close window
		if (event.type == Event::Closed)
		{
			m_Window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//TODO : Create loop to construct ~5 particles
				//Pass the x,y coords to particles
				for(int i = 0; i < 5; i++)
				{
					m_particles.push_back(Particle(m_Window, rand() % 25 + 25, Vector2i(event.mouseButton.x, event.mouseButton.y)));
				}
			}
		}
	}
}

void Engine::update(float dtAsSeconds)
{
	for (int i = 0; i < m_particles.size();)
	{
		if (m_particles[i].getTTL() > 0.0)	// Time to Live > 0
		{
			m_particles[i].update(dtAsSeconds);	// Update the particle
			i++;
		}
		else
		{
			m_particles.erase(m_particles.begin() + i);		//Remove the particle (Don't increment i)
		}
	}
}

Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode(), "Particals", Style::Default);
}

void Engine::draw()
{
	m_Window.clear();
	for (Particle P : m_particles)	//loop through each particle 
					//(pass by reference will be better if we have a lot of particles)
	{
		m_Window.draw(P);	//pass each element into m_window
		m_Window.display();	//display 
	}
}


void Engine::run()
{
	Clock clock;
	while (m_Window.isOpen())
	{
		input();
		update(float(clock.getElapsedTime()));
		draw();
		clock.restart();
	}
}
