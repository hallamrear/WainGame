#include "PostState.h"
#include <HalTec\Animation.h>
#include <HalTec\StateDirector.h>
#include "WorldPiece.h"

#include <HalTec\InputManager.h>
#include <iostream>

void PostState::Start()
{
	bodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(900, 0.0f), 90.0f)));
	bodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(-900.0f, 0.0f), 90.0f)));
	bodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(0.0f, 660.0f), 0.0000f)));
	bodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(0.0f, -660.0f), 0.0000f)));

	alex = new WorldPiece("Textures/Alex.bmp", Transform(Vector2f(200.0f, -1000.0f), 0.0000f));
	animation = new AnimationController("Textures/Player.bmp", 6, 8, 1.0f, true);
	animation->SetAnimation(1);

	animationTransform.Position = Vector2f(-540.0f, -318.0f);
	target.X =  200.0f;
	target.Y = -50.0f;

	InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_MIDDLE_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { std::cout << InputManager::Get()->GetMouseWorldPosition().X << ", " << InputManager::Get()->GetMouseWorldPosition().Y << std::endl; });

	slideTotal = 3.0f;

	playerTimings.push_back(1.0f); timers.push_back(0.0f);
	playerTimings.push_back(2.0f); timers.push_back(0.0f);
	playerTimings.push_back(1.0f); timers.push_back(0.0f);
	playerTimings.push_back(2.0f); timers.push_back(0.0f);
	playerTimings.push_back(1.0f); timers.push_back(0.0f);

}

void PostState::Update(double DeltaTime)
{
	for (size_t i = 0; i < bodies.size(); i++)
	{
		bodies[i]->Update(DeltaTime);
	}

	if (currentTiming < timers.size())
	{
		timers[currentTiming] += DeltaTime;

		if (timers[currentTiming] > playerTimings[currentTiming])
			currentTiming++;

		switch (currentTiming)
		{
		case 0:
			break;
		case 1:
			if (animation->GetCurrentAnimationIndex() != 0)
				animation->SetAnimation(0);

			animationTransform.Position = LerpPoint(Vector2f(-540.0f, -318.0f), Vector2f(-210.0f, -318.0f), timers[currentTiming] / playerTimings[currentTiming]);
			break;
		case 2:
			if (animation->GetCurrentAnimationIndex() != 1)
				animation->SetAnimation(1);
			break;
		case 3:
			alex->Update(DeltaTime);

			slideTime += DeltaTime;

			if (slideTime < slideTotal)
				alex->GetTransform().Position = LerpPoint(alex->GetTransform().Position, target, 0.5f * (slideTime / slideTotal));
			break;

		case 4:
			StateDirector::SetState(GameStateIdentifier::GAME_STATE_2);
			break;

		default:
			break;
		}
	}

	animation->Update(DeltaTime);
}

void PostState::Render(SDL_Renderer& renderer)
{
	alex->Render();

	for (size_t i = 0; i < bodies.size(); i++)
	{
		bodies[i]->Render();
	}
	animation->Render(renderer, animationTransform);
}

void PostState::End()
{

}
