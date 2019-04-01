#include<iostream>
#include<stdlib.h>
#include "screen.h"
using namespace std;
namespace caveofprograming {

screen::screen():
		m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer(NULL),m_buffer2(NULL){}


bool screen::init() {

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return false;

		m_window = SDL_CreateWindow("particle fire explosion",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_hieght, SDL_WINDOW_SHOWN);
		if (m_window == NULL) {
			SDL_Quit();
			return false;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		if (m_renderer == NULL)
		{
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, screen_width, screen_hieght);
		if (m_texture == NULL)
		{
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}
		 m_buffer = new Uint32[screen_width * screen_hieght];
		 m_buffer2 = new Uint32[screen_width * screen_hieght];

		memset(m_buffer, 0, screen_width*screen_hieght * sizeof(Uint32));
		memset(m_buffer2, 0, screen_width*screen_hieght * sizeof(Uint32));

		return true;
	}

void screen::setPixle(int x, int y, Uint8 red, Uint8 green, Uint8 blue) 
{
	if (x < 0 || x >= screen_width || y < 0 || y >= screen_hieght)
		return;

		Uint32 color = 0;

		color += red;
		color <<= 8;
		color += green;
		color <<= 8;
		color += blue;
		color <<= 8;
		color += 0xFF;

		m_buffer[(y * screen_width) + x ] = color;
	}

void screen::update() 
{

		SDL_UpdateTexture(m_texture, NULL, m_buffer, screen_width * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
}

bool screen::processEvent() 
{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return false;
		}

		return true;
}
void screen::boxBlur(){

	Uint32 * temp = m_buffer;
	m_buffer = m_buffer2;
	m_buffer2 = temp;

	for (int y = 0; y < screen_hieght; y++){
		for (int x = 0; x < screen_width; x++){

			int redtotal = 0;
			int greentotal = 0;
			int bluetotal = 0;

			for (int row = -1; row <= 1; row++){
				for (int col = -1; col <= 1; col++){
					int currentx = x+col;
					int currenty = y+row;

					if (currentx >= 0 && currentx < screen_width && currenty >= 0 && currenty < screen_hieght) {
						Uint32 color = m_buffer2[currenty * screen_width + currentx];

						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redtotal += red;
						bluetotal += blue;
						greentotal += green;
					}
				}
			}
			Uint8 red = redtotal / 9;
			Uint8 green = greentotal / 9;
			Uint8 blue = bluetotal / 9;

			setPixle(x, y, red, green, blue);
		}
	}
}

void screen::close() 
{
		delete[] m_buffer;
		delete[] m_buffer2;

		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyTexture(m_texture);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

particle::particle():m_x(0),m_y(0){
	init();
}
particle::~particle() {

}

void particle::init() {
	m_x = 0;
	m_y = 0;
	m_direction = (2 * M_PI * rand()) / RAND_MAX;
	m_speed = (0.03 * rand()) / RAND_MAX;

	m_speed *= m_speed;
}
void particle::update(int intrval){

	m_direction += intrval * 0.002;

	double xspeed = m_speed * cos(m_direction);
	double yspeed = m_speed * sin(m_direction);

	m_x += xspeed * intrval;
	m_y += yspeed * intrval;

	if (m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1)
		init();

	if (rand() < RAND_MAX / 100)
		init();

}
swarm::swarm():last_time(0)
{
	 m_particle = new particle[NPARICLS];
}
swarm::~swarm() {
	delete [] m_particle;
}

void swarm::update(int elasped) {

	int intrval = elasped - last_time;

	for (int i = 0; i <swarm::NPARICLS; i++)
		m_particle[i].update(intrval);

	last_time = elasped;
}

}