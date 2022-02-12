#include "Credit.h"
#include <HalTec\Game.h>
#include <HalTec\Texture.h>
#include <HalTec/TextureCache.h>

void Credit::Start()
{
	texture = TextureCache::GetTexture("Textures/Fin.bmp");
}

void Credit::Update(double dt)
{
	t += dt;

	if (t > 3.0f)
	{
		exit(0);
	}
}

void Credit::Render(SDL_Renderer& renderer)
{
	texture->Render(renderer, Vector2f(), 0.0f);
}

void Credit::End()
{
}
