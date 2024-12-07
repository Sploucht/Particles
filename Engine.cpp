#include "Engine.h"


void Engine::input()
{
	Event event;
	while (window.pollEvent(event))
	{
		//Close window
		if (event.type == Event::Closed)
		{
			window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//TODO : Create loop to construct ~5 particles
				//Pass the x,y coords to particles
				for(int i = 0; i < 5; i++)
				{
					m_particles.push_back(m_Window, rand() % 25 + 25, event.mouseButton);
				}
			}
		}
}

void Engine::update(float dtAsSeconds)
{
	for (int i = 0; i < m_particles.size())
	{
		if (m_particles(i).getTTL() > 0.0)	// Time to Live > 0
		{
			m_particles(i).update(dtAsSeconds);	// Update the particle
			i++;
		}
		else
		{
			m_particles(i).erase();		//Remove the particle (Don't increment i)
		}
	}
}

void Engine::draw()
{
	m_windwow.clear();
	for (Particle P : m_particles)	//loop through each particle 
					//(pass by reference will be better if we have a lot of particles)
	{
		m_window.draw(P);	//pass each element into m_window
		m_window.display();	//display 
	}
}

Engine::Engine()
{
	m_window.create(VideoMode::getDesktopMode())
}

void Engine::run()
{
	Clock clock;
	while (m_window.isOpen())
	{
		input();
		update(clock.getElapsedTime());
		draw();
		clock.restart();
	}
}
