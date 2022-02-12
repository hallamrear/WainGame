#pragma once
#include <HalTec\GameStates.h>
#include <HalTec\Vector2.h>
#include <HalTec\Transform.h>
#include <vector>

class PostState :
    public GameState
{
private:
	class WorldPiece* alex;
	class AnimationController* animation;
	std::vector<class Rigidbody*> bodies;


	Vector2f target;
	float slideTotal = 2.0f;
	float slideTime = 0.0f;

	Transform animationTransform;

	std::vector<float> timers;
	std::vector<float> playerTimings;
	int currentTiming = 0;

public:
	PostState() = default;
	~PostState() = default;

	void Start();
	void Update(double);
	void Render(SDL_Renderer&);
	void End();
};

