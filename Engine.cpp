#include "Engine.h"
//Helper function to update a range of particles
void Engine::updateParticleRange(size_t start, size_t end, float dtAsSeconds)
{
	for (size_t i = start; i < end && i < m_particles.size();)
		{
			if(m_particles[i].getTTL() > 0.0)
			{
				m_particles[i].update(dtAsSeconds, timeStop);
				i++;
			}
			else
			{
				lock_guard<mutex> lock(particles_mutex);
				m_particles.erase(m_particles.begin() + i);
			}
		}
}
void Engine::MakeNoise()
{
	buffer.loadFromFile("MarioFall.wav");
	sound.setBuffer(buffer);
}
void Engine::MakeSprite()
{
	GameRect.setSize(sf::Vector2f(100, 20));
	GameRect.setOutlineColor(sf::Color::Red);
	GameRect.setOutlineThickness(5);
	GameRect.setPosition(1000, 400);
	texture.loadFromFile("Mario.png");
	sprite.setTexture(texture);
	sprite.setPosition(Vector2f(1600, 250));
}
void Engine::MoveSprite(float dt)
{
	const float moveSpeed = 200.0;
	Vector2f PartPos;
	PartPos.x = 0.0;
	PartPos.y = 0.0;
	if (m_particles.size() == 1)
	{
		PartPos = Vector2f(m_Window.mapCoordsToPixel(m_particles[0].GetCenterCoord(), m_particles[0].GetCartPlane()));
	}
	FloatRect spriteBounds = sprite.getGlobalBounds();
    
   
	if (m_particles.size() == 1 && spriteBounds.contains(PartPos))
	{
	        m_particles.erase(m_particles.begin()); 
		hit = true;
	}
	if (hit && !soundPlayed)  
    	{
        	sound.play();
        	soundPlayed = true; 
	}
	if(hit)
	{
		sprite.move(0, 500 * dt);  
	}
	else
	{
		if(SpriteY > 500) SpriteNeg = 1;
		if(SpriteY < 0) SpriteNeg = -1;
		sprite.move(0, moveSpeed * dt * SpriteNeg);  
        	SpriteY -= moveSpeed * dt * SpriteNeg; 
	}
}

void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num1 && Game == 0) Game = 1; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num2 && Game == 0) Game = 2; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Num3 && Game == 0) Game = 3; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && Game == 3) Game = 0; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && Game == 0) event.type = Event::Closed; 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && Game == 2)
		{
			Game = 0; 
			sprite.setPosition(Vector2f(1600, 250));
			hit = false;
			soundPlayed = false; 
		} 
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && Game == 1) Game = 0;
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
					const size_t MAX_PARTICLES = 1000;	//Change this limit if needed
					if (m_particles.size() < MAX_PARTICLES)
					{	
						//Pass the x,y coords to particles
						for(int i = 0; i < 5; i++)
						{
							m_particles.push_back(Particle(m_Window,
								rand() % 50 + 25,
								Vector2i(static_cast<int>(event.mouseButton.x),
										static_cast<int>(event.mouseButton.y))));
						}
					}
				}
			}
		}
		if(Game == 2)
		{
			static Vector2f original;
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
			{
				sprite.setPosition(Vector2f(1600, 250));
				hit = false;
				soundPlayed = false; 
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left && m_particles.size() == 0)
				{
					timeStop = true;
					original.x = event.mouseButton.x;
					original.y = event.mouseButton.y;
					m_particles.push_back(Particle(m_Window, rand() % 50 + 25, Vector2i(event.mouseButton.x, event.mouseButton.y)));
				}
				if (event.mouseButton.button == sf::Mouse::Right && m_particles.size() == 1)
				{
					m_particles[0].setSpeed(original - Vector2f(event.mouseButton.x, event.mouseButton.y));
					timeStop = false;
				}
				
			}
		}
		if(Game == 3)
		{
			if (event.type == sf::Event::MouseMoved)
			{
				GameRect.setPosition(Vector2f(event.mouseMove.x, 650));
			}
		}	
	}
}

void Engine::Game3Part(float dt)
{
	Vector2i PartRand;
	PartRand.x = rand() % 100 + 1000;
	PartRand.y = 200;
	TimePart -= dt;
	Vector2f PartPos3;
	PartPos3.x = 0.0;
	PartPos3.y = 0.0;
	FloatRect RectBounds = GameRect.getGlobalBounds();
	if (TimePart < 0) 
	{
		for(int i = 0; i < 5; i++)
		{
			m_particles.push_back(Particle(m_Window, rand() % 50 + 25, PartRand));
		}
		TimePart = 1;
	}
	for(int i = 0; i < m_particles.size(); i++)
	{
		PartPos3 = Vector2f(m_Window.mapCoordsToPixel(m_particles[i].GetCenterCoord(), m_particles[i].GetCartPlane()));
		if (RectBounds.contains(PartPos3))
		{
	        	m_particles.erase(m_particles.begin() + i); 
			count++;
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
		size_t end = (i == thread_count - 1) ? particle_count : (i + 1) * batch_size;
		threads.emplace_back(&Engine::updateParticleRange, this, start, end, dtAsSeconds);
	}
	setThreadCount(threads.size());
	//wait for all threads to complete
	for(auto &thread : threads)
	{
		thread.join();
	}
}

Engine::Engine()
{
	MakeNoise();
	MakeSprite();
	//Text text;
	font.loadFromFile("ARIAL.TTF");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::White);
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
		if (Game == 2) 
		{
			m_Window.draw(sprite);
		}
		if (Game == 3)
		{
			m_Window.draw(GameRect);
		}
		loadText(text);
		m_Window.draw(text);
	}
	m_Window.display();	
}

void Engine::loadText(Text& text)
{
	
	stringstream ss;
	ss << "Particles" << endl;
	ss << "FPS : " << getFramerate() << endl;
	ss << "Particle Count: " << m_particles.size() << endl;
	ss << "Thread Count: " << getThreadCount() << endl;
	if(Game == 3)
	{
		ss << "Mouse Pos " << GameRect.getPosition().x << ", " << GameRect.getPosition().y << endl;
	}
	if(Game == 0)
	{
		ss << "Press 1 for Particle Creation \n Press 2 for Hit the Mario \n Press ESC to Exit" << endl;
	}
	if(Game == 1)
	{
		ss << "Press ESC to exit to game select \n Left click to make 5 Particles \n Press Spacebar to stop time" << endl;
	}
	if(Game == 2 && hit) 
	{
		ss << "YOU WIN \n Press ESC to go back to game select \n Press R to play again" << endl;
	}
	if(Game == 2 && !hit) 
	{
		ss << "Press ESC to exit to game select \n Left Click to make Particle \n Right Click to throw Particle \n You win if you his Mario" << endl;
	}
	text.setString(ss.str());
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
		m_framerate = 1.f / clock.getElapsedTime().asSeconds();	//FPS
		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		input();
		update(dt);
		MoveSprite(dt);
		if(Game == 3)
		{
			Game3Part(dt);
		}
		draw();
	}
}
