#include "PlayerProjectile.h"
#include "BossProjectile.h"
#include <HalTec\OrientedBoundingBox.h>
#include <HalTec\Texture.h>
#include "Player.h"
#include "Boss.h"

PlayerProjectile::PlayerProjectile(std::string texture_path, Transform transform)
	: Projectile(texture_path, transform, PhysicsProperties(10.0f, 1.0f, 5000.0f, 0.1f, true, false, false))
{

}

PlayerProjectile::~PlayerProjectile()
{

}

void PlayerProjectile::OnOverlap(const CollisionManifold&, Rigidbody& other)
{
	if (dynamic_cast<Player*>(&other))
	{
		return;
	}
	else if (dynamic_cast<BossProjectile*>(&other))
	{
		other.SetAlive(false);
	}

	if (!(dynamic_cast<Boss*>(&other)))
	{
		SetAlive(false);
	}
}

void PlayerProjectile::Update(double DeltaTime)
{
	if (mIsAlive)
	{
		mCollider->Update(DeltaTime);
	}
}

void PlayerProjectile::Render()
{
	if (mIsAlive)
	{
		mTexture->Render(mRenderer, mTransform.Position, mTransform.Rotation);
		mCollider->Render(mRenderer);
	}
}
