#include "Engine.h"
//Helper function to update a range of particles
void Engine::updateParticleRange(size_t start, size_t end, float dtAsSeconds)
{
	for (size_t i = start; i < end && i < m_particles.size();)
		{
			if(m_particles[i].getTTL() > 0.0)
			{
				m_particles[i].update(dtAsSeconds, timestop);
				i++
			}
			else
			{
				lock_guard<mutex> lock(particles_mutex);
				m_particles.erase(m_particles.begin() + i);
			}
		}
}
void Engine::MakeSprite()
{
	Texture texture;
	texture.loadFromFile("Mario.png");
	sprite.setTexture(texture);
	sprite.setPosition(Vector2f(1600, 800));
}
void Engine::input()
{
	MakeSprite();
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num1 && Game == 0) Game = 1; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num2 && Game == 0) Game = 2; 
		
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && Game == 0) event.type = Event::Closed; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && Game != 0) Game = 0; 
		//Close window
		if (event.type == Event::Closed)
		{
			m_Window.close();
		}
		if(Game == 1)
		{
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) timeStop = !timeStop;
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//TODO : Create loop to construct ~5 particles
					//Pass the x,y coords to particles
					for(int i = 0; i < 5; i++)
					{
						m_particles.push_back(Particle(m_Window, rand() % 50 + 25, Vector2i(event.mouseButton.x, event.mouseButton.y)));
					}
				}
			}
		}
		if(Game == 2)
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left && m_particles.size() == 0)
				{
					timeStop = true;
					original.x = event.mouseButton.x;
					original.y = event.mouseButton.y;
					m_particles.push_back(Particle(m_Window, rand() % 50 + 25, Vector2i(event.mouseButton.x, event.mouseButton.y)));
					cout << original.x << endl;
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					m_particles[0].setSpeed(original - Vector2f(event.mouseButton.x, event.mouseButton.y));
					timeStop = false;
					cout << original.x - event.mouseButton.x << endl;
				}
				
			}
		}
	}
}

void Engine::update(float dtAsSeconds)
{
	const size_t particle_count = m_particles.size();
	if (particle_count == 0) return;

	//Determine number of threads based on hardware and particle count
	unsigned int thread_count = thread::hardware_concurrency();
	//if you have 8 cpu threads and 250 particles: min(8, (250 + 99)/100) = min(8, 3) = 3 threads
	thread_count = min(thread_count, (unsigned int)((particle_count + 90) / 100));	
	thread_count = max(thread_count, 1u);	//ensures having at least 1 thread

	vector<thread> threads;
	const size_t batch_size = particle_count / thread_count;

	//Create threads to  update particles in parallel
	for (unsigned int i = 0; i < thread_count; i++)
	{
		size_t start = i * batch_size;
		//ternary operator | size_t end = (condition) ? valueIfTrue : valueIfFalse;
		size_t end = (i == thread_count - 1) ? particle_count : (i + 1) * batch size;
		threads.emplace_back(&Engine::updateParticleRange, this, start, end, dtAsSeconds);
	}

	//wait for all threads to complete
	for(auto &thread : threads)
	{
		thread.join();
	}
}

Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode(), "Particals", Style::Default);
}

void Engine::draw()
{
	m_Window.clear();
	{
		lock_guard<mutex> lock(particles_mutex);
		for (const Particle& P : m_particles)	//loop through each particle
		{
			m_Window.draw(P);	//pass each element into m_window
		}
		m_Window.draw(sprite);
	}
	m_Window.display();	
}


void Engine::run()
{
	cout << "Starting Particle unit tests..." << endl;
    	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    	p.unitTests();
    	cout << "Unit tests complete.  Starting engine..." << endl;
	Clock clock;
	while (m_Window.isOpen())
	{
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		input();
		update(dt);
		draw();
	}
}
