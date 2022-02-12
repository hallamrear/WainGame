#include "Boss.h"
#include <HalTec\Texture.h>
#include <HalTec\OrientedBoundingBox.h>
#include <HalTec\Settings.h>
#include <HalTec\Helper.h>
#include <HalTec\Animation.h>
#include "Projectile.h"
#include "PlayerProjectile.h"
#include "BossProjectile.h"
#include "ShootPlayerAttackPhase.h"
#include "ShootBeamAttackPhase.h"
#include "ShootCircleAttackPhase.h"
#include "Player.h"
#include "HelpFuncs.h"
#include <iostream>
#include "BossBeams.h"

#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <Mmsystem.h>

Boss::Boss(std::string texture_path, Transform transform, std::vector<Rigidbody*>& worldEntities, Player& player)
	: Rigidbody(texture_path, transform, PhysicsProperties()), mPlayer(player), mEntities(worldEntities)
{
	mExplosionSoundEffectElapsedTime = 0.0f;
	mExplosionSoundEffectCooldown = 0.1f;
	mIsStill = false;
	mIsStillTimer = 0.0f;

	srand(NULL);

	mAttackPhases.push_back(new ShootPlayerAttackPhase(mRenderer, *this, mEntities, "Textures/WainShoot_1.bmp"));
	mAttackPhases.push_back(new ShootCircleAttackPhase(mRenderer, *this, mEntities, "Textures/WainShoot_2.bmp"));
	mAttackPhases.push_back(new ShootBeamAttackPhase(mRenderer, *this, mEntities, "Textures/WainShoot_3.bmp"));
	mCurrentAttackPhase = 0;

	mHealthBarAnimation = new AnimationController("Textures/HealthBar.bmp", 11, 1, 1.0f, true);
	mDeathAnimation = new AnimationController("Textures/WainSmaller_Death.bmp", 1, 31, 5.0f, false);
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height - 26.0f);
	mCollider->IsOverlap = true;
	SetGravityEnabled(false);
	SetDragEnabled(false);

	mPointChecker = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, 25.0f, 25.0f);
	mTargetPoint = new BoundingBox(mCurrentPathTargetPosition, 5.0f, 5.0f);
	mPointChecker->IsOverlap = true;

	mMoveSpeedScale = 5.0f;

	mMaxHealth = 20;
	mHealth = mMaxHealth;

	int explosionCount = (rand() % 30) + 1;

	for (size_t i = 0; i < explosionCount; i++)
	{
		int n = rand() % 3 + 1;
		std::string location = "Textures/Explosion (" + std::to_string(n) + ").bmp";
		mDeathExplosions.push_back(new AnimationController(location, 1, 8, ((float)rand() / (RAND_MAX)) + 1, true));

		Vector2f offset;
		offset.X = ((float)rand() / (RAND_MAX)) + 1 * ((rand() % 150) - 75);
		offset.Y = ((float)rand() / (RAND_MAX)) + 1 * ((rand() % 150) - 75);
		mDeathExplosionOffsets.push_back(offset);
	}

	mChangeAttackTimer = 0.0f;
	mChangeAttackCooldown = 2.5f;
	mIsStillCooldown = 4.0f;
}

Boss::~Boss()
{

}

void Boss::FireBulletAtPlayer()
{
	mEntities.push_back(new BossProjectile("Textures/BossProjectile.bmp", mTransform.Position));
	Vector2f dir = mPlayer.GetTransform().Position - mTransform.Position;
	mEntities.back()->AddVelocity(dir.GetNormalized() * 10.0f);
	mEntities.back()->GetTransform().Rotation = ConvertToDegrees(atan(dir.X / dir.Y));
}

void Boss::FireBulletsInCircle()
{
	Vector2f point;
	for (double angle = 0; angle <= 2 * M_PI; angle += 0.5f)
	{
		point.X = mTransform.Position.X + 4.0f * (float)cos(angle);
		point.Y = mTransform.Position.Y + 4.0f * (float)sin(angle);

		mEntities.push_back(new BossProjectile("Textures/BossProjectile.bmp", mTransform.Position));
		Vector2f dir = point - mTransform.Position;
		mEntities.back()->AddVelocity(dir.GetNormalized() * 10.0f);
		mEntities.back()->GetTransform().Rotation = ConvertToDegrees(atan(dir.X / dir.Y));
	}
}

void Boss::FireBeams()
{
	mIsStill = true;

	//LR
	mEntities.push_back(new BossBeam("Textures/BossBeam.bmp", mTransform.Position));
	mEntities.back()->GetTransform().AdjustRotation(90.0f);
	//UD
	mEntities.push_back(new BossBeam("Textures/BossBeam.bmp", mTransform.Position));	
}

void Boss::OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
{
	if (dynamic_cast<PlayerProjectile*>(&other))
	{
		other.SetAlive(false);
		mHealth--;
	}

	if (dynamic_cast<Player*>(&other))
	{
		other.SetAlive(false);
	}
}

void Boss::AddPathPoint(Vector2f point)
{
	mPathPoints.push_back(point);
}

void Boss::Update(double deltaTime)
{
	if (mHealth > 0)
	{
		if (mIsStill)
		{
			mIsStillTimer += deltaTime;

			if (mIsStillTimer > mIsStillCooldown)
			{
				mIsStillTimer = 0.0f;
				mIsStill = false;
			}
		}

		if (mPathPoints.size() > 1)
		{
			if(mIsStill == false)
				mTransform.Position = LerpPoint(mTransform.Position, mPathPoints[mCurrentPathTargetIndex], mMoveSpeedScale * (float)deltaTime);

			CollisionManifold manifold;

			if (Collision::CheckCollision(*mPointChecker, *mTargetPoint, &manifold))
			{
				mCurrentPathTargetIndex = rand() % mPathPoints.size();
				mMoveSpeedScale = rand() % 5 + 2;

				if (mCurrentPathTargetIndex == mPathPoints.size())
				{
					mCurrentPathTargetIndex = rand() % mPathPoints.size();
				}

				mCurrentPathTargetPosition = mPathPoints[mCurrentPathTargetIndex];
			}
		}

		float healthPercentage = floor(((float)mHealth / (float)mMaxHealth) * 100.0f);
		float rounded = (float)RoundToNearestTen(healthPercentage);

		mHealthBarAnimation->SetAnimation(10 - ((int)rounded / 10));
		mHealthBarAnimation->Update(deltaTime);
		mAttackPhases[mCurrentAttackPhase]->Update(deltaTime);

		mChangeAttackTimer += deltaTime;

		if (mChangeAttackTimer > mChangeAttackCooldown)
		{
			mChangeAttackTimer = 0.0f;

			int chanceToChangeAttack = rand() % 100 + 1;

			if (chanceToChangeAttack < 75)
			{
				mAttackPhases[mCurrentAttackPhase]->Reset();
				mCurrentAttackPhase = rand() % 3;
			}
		}

	}
	else
	{
		if (mDeathAnimation->HasFinished())
		{
			mTransform.Position = Vector2f(0.0f, -10000.0f);
			mCurrentPathTargetPosition = Vector2f(0.0f, -10000.0f);
			SetAlive(false);
		}
		else
		{
			mExplosionSoundEffectElapsedTime += deltaTime;

			if (mExplosionSoundEffectElapsedTime > mExplosionSoundEffectCooldown)
			{
				int n = rand() % 3 + 1;
				std::string location = "Sounds/Explosion_00" + std::to_string(n) + ".wav";
				PlaySoundA(location.c_str(), NULL, SND_ASYNC | SND_FILENAME);

				mExplosionSoundEffectElapsedTime = 0.0f;
				mExplosionSoundEffectCooldown = 0.5f;
			}

			mDeathAnimation->Update(deltaTime);

			for (auto& itr : mDeathExplosions)
			{
				itr->Update(deltaTime);
			}
		}
	}

	mCollider->Update(deltaTime);
	mPointChecker->Update(deltaTime);
	mTargetPoint->Update(deltaTime);
	std::cout << mCurrentAttackPhase << std::endl;
}

void Boss::Render()
{
	mCollider->Render(mRenderer);
	mPointChecker->Render(mRenderer);
	mTargetPoint->Render(mRenderer);

	if (mHealth > 0)
	{
		if (mAttackPhases[mCurrentAttackPhase]->GetUseAttackTexture())
			mAttackPhases[mCurrentAttackPhase]->Render();
		else
			mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);

		mHealthBarAnimation->Render(mRenderer, Transform(mTransform.Position + Vector2f(0.0f, 125.0f)));
	}
	else
	{
		if (mDeathAnimation->HasFinished() == false)
		{
			mDeathAnimation->Render(mRenderer, mTransform.Position, mTransform.Rotation);

			for (int i = 0; i < mDeathExplosions.size(); i++)
			{
				mDeathExplosions[i]->Render(mRenderer, Transform(mTransform.Position + mDeathExplosionOffsets[i]));
			}
		}
	}
}
