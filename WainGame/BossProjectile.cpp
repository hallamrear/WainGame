#include "BossProjectile.h"
#include "Boss.h"
#include "Player.h"
#include <HalTec\Texture.h>
#include <HalTec\OrientedBoundingBox.h>

BossProjectile::BossProjectile(std::string texture_path, Transform transform)
	: Projectile(texture_path, transform, PhysicsProperties(10.0f, 1.0f, 5000.0f, 0.1f, true, false, false))
{

}

BossProjectile::~BossProjectile()
{

}

void BossProjectile::OnOverlap(const CollisionManifold&, Rigidbody& other)
{
	Boss* boss = dynamic_cast<Boss*>(&other);
	if (!boss)
	{
		if (dynamic_cast<Player*>(&other))
		{
			other.SetAlive(false);
			SetAlive(false);
		}
	}
}

void BossProjectile::Update(double DeltaTime)
{
	if (mIsAlive)
	{
		mCollider->Update(DeltaTime);
	}
}

void BossProjectile::Render()
{
	if (mIsAlive)
	{
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);
		mCollider->Render(mRenderer);
	}
}
