#include <iostream>
#include "ConsoleGameEngine.h"
#include <string>
#include "audiere.h"

enum ProjectPropories {
	Width	= 300,
	Height	= 300,
	MaxDepth= 20,

	MaxSpeed= 20,

	N		= 200
};

class Star {
public:
	float x, y, z;
	
	Star() {
		static int i = rand() % MaxDepth;

		x = rand() % Width - Width/2;
		y = rand() % Height - Height/2;
		z = ++i % MaxDepth;
	}
};

class Interstellar:public olcConsoleGameEngine
{
private:
	float speed = 0, tail = -0.05f, i;

	short tone;

	Star space[N];

	void Show(Star& s, float &depth) {
		if (s.z + depth >= 1.0f) {
			if (depth > speed * tail * 0.3f) tone = PIXEL_QUARTER;
			else if (depth > speed * tail * 0.14f) tone = PIXEL_THREEQUARTERS;
			else if (depth > speed * tail * 0.04f) tone = PIXEL_HALF;
			else tone = PIXEL_SOLID;

			FillCircle(s.x / (s.z + depth) + Width / 2, s.y / (s.z + depth) + Height / 2, std::powf((s.z + depth)/2, -1.0f), tone, FG_WHITE);
		}
	}

protected:
	virtual bool OnUserCreate() {
		return true;
	}

	virtual bool OnUserUpdate(float fElapsed) {
		if (m_keys[VK_UP].bHeld) if (speed < MaxSpeed) speed += 0.8f * fElapsed;
		if (m_keys[VK_DOWN].bHeld) if (speed > 0) speed -= 0.8f * fElapsed;

		if (m_keys[VK_RIGHT].bHeld) if (tail < 1) tail += 0.5f * fElapsed;
		if (m_keys[VK_LEFT].bHeld) if (tail > 0) tail -= 0.5f * fElapsed;
		
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');
		
		for (auto& s : space) {
			s.z -= speed * fElapsed;

			if (s.z < -speed) s.z = MaxDepth - 1;
		}
			
		for (i = speed*tail; i >= 0; i-=0.01f) {
			for (auto& s : space) {
				Show(s,i);
			}
		}


		return true;
	}
};

int main()
{
	srand(time(NULL));

	audiere::AudioDevicePtr device = audiere::OpenDevice();
	audiere::OutputStreamPtr sound = OpenSound(device, "sound.wav", false);
	sound->play();

	if (!sound->isPlaying()) return -1;

	Interstellar project;
	project.ConstructConsole(Width, Height, 2, 2);
	project.Start();
}