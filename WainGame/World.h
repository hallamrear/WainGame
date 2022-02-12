#pragma once
#include <HalTec/GameStates.h>
#include <HalTec/Transform.h>
#include <vector>
#include <iostream>

class World :
    public GameState
{
	class Player* mPlayer;
	class Boss* mWain;
	std::vector<class Rigidbody*> mRigidbodies;
	bool mMusicChanged;
	float mDeathTimer;

	Rigidbody* portal;
	void CleanupDeadEntities();
	Transform mWorldBoxTransform;
	class BoundingBox* mWorldBox;

public:
	World() = default;
	~World() = default;

	void Start();
	void Update(double);
	void Render(SDL_Renderer&);
	void End();
};

