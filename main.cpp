#include<iostream>
#include<SDL.h>
#include<math.h>
#include"screen.h"
#include<stdlib.h>
#include<time.h>
#undef main

using namespace std;
using namespace caveofprograming;

int main()
{	
	srand(time(NULL));
	SDL_Init(SDL_INIT_AUDIO);	
	SDL_AudioSpec wavspec;
	Uint32 wavlength;
	Uint8 * wavbuffer;
	SDL_LoadWAV("b.wav", &wavspec, &wavbuffer, &wavlength);
	SDL_AudioDeviceID DeviceId = SDL_OpenAudioDevice(NULL, 0, &wavspec, NULL, 0);
	int sucssec = SDL_QueueAudio(DeviceId, wavbuffer, wavlength);
	SDL_PauseAudioDevice(DeviceId, 0);
	screen screen;
	if (screen.init() == false)
		cout << "error";
	swarm swarm;
	//event loop,
	while (true)
	{
		int elasped = SDL_GetTicks();//number of milisecond that the program is running

		swarm.update(elasped);

		unsigned char green = (unsigned char) ((1 + sin(elasped * 0.0001)) * 128);
		unsigned char red = (unsigned char) ((1 + sin(elasped * 0.0002)) * 128);
		unsigned char blue = (unsigned char) ((1 + sin(elasped * 0.0003)) * 128);

		const particle * const pparticle = swarm.getParticle();

		for (int  i = 0; i <swarm::NPARICLS; i++){
			particle particle = pparticle[i];

			int x = (particle.m_x + 1) * screen::screen_width/2;
			int y = particle.m_y * screen::screen_width/2 + screen::screen_hieght/2;

			screen.setPixle(x,y,red,green,blue);
		}

		screen.boxBlur();
		screen.update();

		//cheak for masseges/events
		if (screen.processEvent() == false)
			break;
	}

	SDL_CloseAudioDevice(DeviceId);
	SDL_FreeWAV(wavbuffer);
	screen.close();

	return 0;
}