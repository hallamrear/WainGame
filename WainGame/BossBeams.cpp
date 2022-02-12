#include "BossBeams.h"
#include "Boss.h"
#include "Player.h"
#include <HalTec\Texture.h>
#include <HalTec\OrientedBoundingBox.h>

BossBeam::BossBeam(std::string texture_path, Transform transform)
	: Projectile(texture_path, transform, PhysicsProperties(10.0f, 1.0f, 5000.0f, 0.1f, true, false, true))
{
	mLifespan = 0.0f;
	mMaxLifespan = 4.0f;
	mCollider = new OrientedBoundingBox(mTransform.Position, mTransform.Rotation, mTexture->Width, mTexture->Height);
}

BossBeam::~BossBeam()
{

}

void BossBeam::OnCollision(const CollisionManifold&, Rigidbody& other)
{
	if (!(dynamic_cast<Boss*>(&other)) && !(dynamic_cast<BossBeam*>(&other)))
	{
		if (dynamic_cast<Player*>(&other))
		{
			other.SetAlive(false);
		}
	}
}

void BossBeam::Update(double DeltaTime)
{
	if (mIsAlive)
	{
		mLifespan += DeltaTime;

		if (mLifespan > mMaxLifespan)
			SetAlive(false);

		mCollider->Update(DeltaTime);
	}
}

void BossBeam::Render()
{
	if (mIsAlive)
	{
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);
		mCollider->Render(mRenderer);
	}
}
