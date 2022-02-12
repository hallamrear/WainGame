#include "World.h"
#include <HalTec\Texture.h>
#include <HalTec\TextureCache.h>
#include <HalTec\Helper.h>
#include <HalTec\Vector2.h>
#include <HalTec\Camera.h>
#include "Player.h"
#include "Boss.h"
#include "WorldPiece.h"
#include "BossProjectile.h"
#include "PlayerProjectile.h"
#include "ExplosionEffect.h"
#include "Portal.h"

#include <iostream>
#include <HalTec\InputManager.h>
#include <HalTec\BoundingBox.h>
#include <HalTec\StateDirector.h>

#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <Mmsystem.h>
#include "BossBeams.h"

void World::Start()
{
	mRigidbodies = std::vector<Rigidbody*>();
	mPlayer = new Player(mRigidbodies, Transform(Vector2f(-500.0f, 0.0f)));
	mWain = new Boss("Textures/WainSmaller.bmp", Transform(), mRigidbodies, *mPlayer);
	mWorldBox = new BoundingBox(mWorldBoxTransform.Position, 1300.0f, 800.0f);
	mMusicChanged = false;
	mDeathTimer = 0.0f;

	PlaySound(TEXT("Sounds/Boss.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

	/* RIG */ mRigidbodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(900, 0.0f), 90.0f)));
	/* LEF */ mRigidbodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(-900.0f, 0.0f), 90.0f)));
	/* TOP */ mRigidbodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(0.0f, 660.0f), 0.0000f)));
	/* BOT */ mRigidbodies.push_back(new WorldPiece("Textures/Wall.bmp", Transform(Vector2f(0.0f, -660.0f), 0.0000f)));

	mWain->AddPathPoint(Vector2f( 378.0f,-111.0f));
	mWain->AddPathPoint(Vector2f( 410.0f, 169.0f));
	mWain->AddPathPoint(Vector2f( 145.0f, 256.0f));
	mWain->AddPathPoint(Vector2f(-120.0f, 261.0f));
	mWain->AddPathPoint(Vector2f(-384.0f, 249.0f));
	mWain->AddPathPoint(Vector2f(-427.0f,-20.0f ));
	mWain->AddPathPoint(Vector2f(-180.0f,-118.0f));
	mWain->AddPathPoint(Vector2f(-410.0f, 106.0f));
	mWain->AddPathPoint(Vector2f(-226.0f, 264.0f));
	mWain->AddPathPoint(Vector2f( 134.0f, 248.0f));
	mWain->AddPathPoint(Vector2f( 390.0f, 123.0f));
	mWain->AddPathPoint(Vector2f( 308.0f,-104.0f));

	mRigidbodies.push_back(new WorldPiece("Textures/Platform.bmp", Transform(Vector2f(-471.0f,-84.0f ), 0.0f)));
	mRigidbodies.push_back(new WorldPiece("Textures/Platform.bmp", Transform(Vector2f(-194.0f, 77.0f ), 0.0f)));
	mRigidbodies.push_back(new WorldPiece("Textures/Platform.bmp", Transform(Vector2f( 161.0f, 138.0f), 0.0f)));
	mRigidbodies.push_back(new WorldPiece("Textures/Platform.bmp", Transform(Vector2f( 367.0f,-60.0f ), 0.0f)));
	mRigidbodies.push_back(new WorldPiece("Textures/Platform.bmp", Transform(Vector2f( 157.0f,-155.0f), 0.0f)));
	mRigidbodies.push_back(new WorldPiece("Textures/Platform.bmp", Transform(Vector2f(-286.0f,-169.0f), 0.0f)));

	mWain->GetTransform().Position = Vector2f(100.0f, 100.0f);
	mPlayer->GetTransform().Position = Vector2f(-500.0f, -280.0f);
}

void World::End()
{
	delete mWain;
	mWain = nullptr;

	delete mPlayer;
	mPlayer = nullptr;

	delete mWorldBox;
	mWorldBox = nullptr;

	for (size_t i = 0; i < mRigidbodies.size(); i++)
	{
		delete mRigidbodies[i];
		mRigidbodies[i] = nullptr;
	}

	mRigidbodies.clear();
}

void World::Update(double DeltaTime)
{
	mWorldBox->Update(DeltaTime);
	mPlayer->Update(DeltaTime);
	mWain->Update(DeltaTime);

	CollisionManifold manifold;
	int size = mRigidbodies.size();
	Rigidbody* body;
	for(int i = 0; i < size; i++)
	{
		body = mRigidbodies[i];

		if (body)
		{
			if (body->GetCollider() && dynamic_cast<BossBeam*>(body) == nullptr)
			{
				if (!Collision::CheckCollision(*mWorldBox, *body->GetCollider(), &manifold))
				{
					body->SetAlive(false);
				}
			}

			if (dynamic_cast<PlayerProjectile*>(body))
			{
				if (body->GetIsAlive() == false)
				{
					mRigidbodies.push_back(new ExplosionEffect(body->GetTransform().Position));
				}
			}
			body->Update(DeltaTime);
		}
	}

	body = nullptr;

	if (mWain->GetIsAlive() == false && mMusicChanged == false)
	{
		mMusicChanged = true;
		PlaySoundA("Sounds/Empty.wav", NULL, SND_ASYNC | SND_FILENAME);
		PlaySoundA("Sounds/Victory.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

		mRigidbodies.push_back(new Portal("Textures/Portal.bmp", Transform(Vector2f(-540.0f, -218.0f))));
		portal = mRigidbodies.back();
	}
	
	if (mPlayer->GetIsAlive() == false)
	{
		if (mMusicChanged == false)
		{
			mMusicChanged = true;
			PlaySoundA("Sounds/Empty.wav", NULL, SND_ASYNC | SND_FILENAME);
			PlaySoundA("Sounds/TheBoss.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
		}

		mDeathTimer += DeltaTime;
		
		if (mDeathTimer > 1.8f)
		{
			PlaySoundA("Sounds/Empty.wav", NULL, SND_ASYNC | SND_FILENAME);
			StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYING);
		}
	}

	if (portal)
	{
		if (portal->GetIsAlive() == false)
		{
			StateDirector::SetState(GameStateIdentifier::GAME_STATE_1);
		}
	}

	CleanupDeadEntities();
}

void World::CleanupDeadEntities()
{
	std::vector<int> toDelete;

	int size = mRigidbodies.size();
	for (size_t i = 0; i < size; i++)
	{
		if (mRigidbodies[i] == nullptr)
		{
			toDelete.push_back(i);
		}
		else
		if (mRigidbodies[i]->GetIsAlive() == false)
		{
			toDelete.push_back(i);

			delete mRigidbodies[i];
			mRigidbodies[i] = nullptr;
		}
	}

	for (size_t i = toDelete.size(); i > 0; i--)
	{
  		mRigidbodies.erase(mRigidbodies.begin() + toDelete[i - 1]);
	}
}

void World::Render(SDL_Renderer& renderer)
{
	for (Rigidbody* body : mRigidbodies)
	{
		if (body)
			body->Render();
	}

	mWain->Render();
	mPlayer->Render();

	mWorldBox->Render(renderer);
}
