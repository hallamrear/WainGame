#pragma once
#include <HalTec\GameStates.h>

class Credit :
    public GameState
{
private:
	class Texture* texture;
	float t;

public:

	void Start();
	void Update(double);
	void Render(SDL_Renderer&);
	void End();
};

