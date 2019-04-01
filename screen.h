#pragma once
#include<iostream>
#include<SDL.h>

namespace caveofprograming {

	class screen {

	public:
		const static int screen_width = 900;
		const static int screen_hieght = 700;

	private:
		SDL_Window * m_window;
		SDL_Renderer * m_renderer;
		SDL_Texture * m_texture;
		Uint32 * m_buffer;
		Uint32 * m_buffer2;


	public:
		screen();
		bool init();
		bool processEvent();
		void close();
		void setPixle(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
		void boxBlur();
		void update();
	};

struct particle {
		double m_x;
		double m_y;
private:
		double m_speed;
		double m_direction;
private:
	void init();
public:
		particle();
		virtual ~particle();
		void update(int);
		double getFrequnte(SDL_AudioSpec wavspec);
};

class swarm{
public:
	const static int NPARICLS= 3000;

private:
	particle* m_particle;
	int last_time;
public:
		swarm();
		virtual ~swarm();
		void update(int);
		const particle * const getParticle() { return m_particle; };
	
	};

}
